#include "GDDCClass.h"

#include <utility>

#include "dclass/dcAtomicField.h"
#include "dclass/dcPacker.h"
#include "dclass/dcmsgtypes.h"

#include "GDDCField.h"

using namespace godot;

void GDDCClass::set_dc_class( DCClass *dcClass )
{
    _dcClass = dcClass;
}

String GDDCClass::get_name() const
{
    return { _dcClass->get_name().c_str() };
}

int GDDCClass::get_number() const
{
    return _dcClass->get_number();
}

/**
 * Generates a datagram containing the message necessary to send an update for
 * the indicated distributed object from the client.
 */
Ref<Datagram> GDDCClass::client_format_update( godot::String field_name, int do_id,
                                               godot::Array args )
{
    DCField *field = _dcClass->get_field_by_name( field_name.utf8().get_data() );
    if ( !field )
    {
        ERR_PRINT( ( "No field named " + field_name + "in class" + get_name() ).utf8().get_data() );
        return {};
    }

    GDDCField dcField;
    dcField.set_dc_field( field );

    return dcField.client_format_update( do_id, std::move( args ) );
}

/**
 * Generates a datagram containing the message necessary to send an update for
 * the indicated distributed object from the AI.
 */
Ref<Datagram> GDDCClass::ai_format_update( godot::String field_name, int do_id, int to_id,
                                           int from_id, godot::Array args )
{
    DCField *field = _dcClass->get_field_by_name( field_name.utf8().get_data() );
    if ( !field )
    {
        ERR_PRINT( ( "No field named " + field_name + "in class" + get_name() ).utf8().get_data() );
        return {};
    }

    GDDCField dcField;
    dcField.set_dc_field( field );

    return dcField.ai_format_update( do_id, to_id, from_id, std::move( args ) );
}

/**
 * Generates a datagram containing the message necessary to generate a new
 * distributed object from the AI. This requires querying the object for the
 * initial value of its required fields.
 *
 * optional_fields is a list of fieldNames to generate in addition to the
 * normal required fields.
 */
Ref<Datagram> GDDCClass::ai_format_generate( Object *dist_obj, uint32_t do_id, uint32_t parent_id,
                                             uint32_t zone_id, uint64_t district_channel_id,
                                             uint64_t from_channel_id,
                                             PackedStringArray optional_fields )
{
    DCPacker packer;

    packer.raw_pack_uint8( 1 );
    packer.RAW_PACK_CHANNEL( district_channel_id );
    packer.RAW_PACK_CHANNEL( from_channel_id );

    bool has_optional_fields = !optional_fields.is_empty();
    if ( has_optional_fields )
    {
        packer.raw_pack_uint16( STATESERVER_CREATE_OBJECT_WITH_REQUIRED_OTHER );
    }
    else
    {
        packer.raw_pack_uint16( STATESERVER_CREATE_OBJECT_WITH_REQUIRED );
    }

    packer.raw_pack_uint32( do_id );
    // Parent is a bit overloaded; this parent is not about inheritance, this
    // one is about the visibility container parent, i.e.  the zone parent:
    packer.raw_pack_uint32( parent_id );
    packer.raw_pack_uint32( zone_id );
    packer.raw_pack_uint16( get_number() );

    // Specify all the required fields.
    int num_fields = _dcClass->get_num_inherited_fields();
    for ( int i = 0; i < num_fields; ++i )
    {
        DCField *field = _dcClass->get_inherited_field( i );
        if ( field->is_required() && field->as_molecular_field() == nullptr )
        {
            packer.begin_pack( field );
            if ( !pack_required_field( packer, dist_obj, field ) )
            {
                return {};
            }
            packer.end_pack();
        }
    }

    // Also specify the optional fields.
    int64_t num_optional_fields = optional_fields.size();
    packer.raw_pack_uint16( num_optional_fields );

    for ( int i = 0; i < num_optional_fields; i++ )
    {
        String field_name = optional_fields[i];

        DCField *field = _dcClass->get_field_by_name( field_name.utf8().get_data() );
        if ( field == nullptr )
        {
            ERR_PRINT(
                ( "No field named " + field_name + "in class" + get_name() ).utf8().get_data() );
            return {};
        }

        packer.raw_pack_uint16( field->get_number() );
        packer.begin_pack( field );
        if ( !pack_required_field( packer, dist_obj, field ) )
        {
            return {};
        }
        packer.end_pack();
    }

    Ref<Datagram> dg = memnew( Datagram );
    dg->SetBytes( reinterpret_cast<const uint8_t *>( packer.get_data() ), packer.get_length() );

    return dg;
}

/**
 * Extracts the update message out of the datagram and applies it to the
 * indicated object by calling the appropriate method.
 */
void GDDCClass::receive_update( Object *dist_obj, Ref<DatagramIterator> di )
{
    DCPacker packer;

    const char *data = (const char *)di->GetData()->GetBytes();
    packer.set_unpack_data( data + di->Tell(), di->GetRemainingSize(), false );

    int field_id = packer.raw_unpack_uint16();
    DCField *field = _dcClass->get_field_by_index( field_id );
    if ( field == nullptr )
    {
        ERR_PRINT( ( "Received update for field " + String( std::to_string( field_id ).c_str() ) +
                     ", not in class " + get_name() )
                       .utf8()
                       .get_data() );
        return;
    }

    GDDCField dcField;
    dcField.set_dc_field( field );

    packer.begin_unpack( field );
    dcField.receive_update( dist_obj, packer );
    packer.end_unpack();

    di->Skip( packer.get_num_unpacked_bytes() );
}

/**
 * Processes a big datagram that includes all of the "required" fields that
 * are sent along with a normal "generate with required" message. This is all
 * of the atomic fields that are marked "broadcast required".
 */
void GDDCClass::receive_update_broadcast_required( Object *dist_obj, Ref<DatagramIterator> di )
{
    DCPacker packer;

    const char *data = (const char *)di->GetData()->GetBytes();
    packer.set_unpack_data( data + di->Tell(), di->GetRemainingSize(), false );

    int num_fields = _dcClass->get_num_inherited_fields();
    for ( int i = 0; i < num_fields; ++i )
    {
        DCField *field = _dcClass->get_inherited_field( i );
        if ( field->as_molecular_field() == nullptr && field->is_required() &&
             field->is_broadcast() )
        {
            packer.begin_unpack( field );

            GDDCField dcField;
            dcField.set_dc_field( field );
            dcField.receive_update( dist_obj, packer );

            if ( !packer.end_unpack() )
            {
                break;
            }
        }
    }

    di->Skip( packer.get_num_unpacked_bytes() );
}

/**
 * Processes a big datagram that includes all of the "required" fields that
 * are sent along with a normal "generate with required" message. This is all
 * of the atomic fields that are marked "broadcast ownrecv". Should be used
 * for 'owner-view' objects.
 */
void GDDCClass::receive_update_broadcast_required_owner( Object *dist_obj,
                                                         Ref<DatagramIterator> di )
{
    DCPacker packer;

    const char *data = (const char *)di->GetData()->GetBytes();
    packer.set_unpack_data( data + di->Tell(), di->GetRemainingSize(), false );

    int num_fields = _dcClass->get_num_inherited_fields();
    for ( int i = 0; i < num_fields; ++i )
    {
        DCField *field = _dcClass->get_inherited_field( i );
        if ( field->as_molecular_field() == nullptr && field->is_required() &&
             ( field->is_ownrecv() || field->is_broadcast() ) )
        {
            packer.begin_unpack( field );

            GDDCField dcField;
            dcField.set_dc_field( field );
            dcField.receive_update( dist_obj, packer );

            if ( !packer.end_unpack() )
            {
                break;
            }
        }
    }

    di->Skip( packer.get_num_unpacked_bytes() );
}

/**
 * Processes a big datagram that includes all of the "required" fields that
 * are sent when an avatar is created. This is all of the atomic fields that
 * are marked "required", whether they are broadcast or not.
 */
void GDDCClass::receive_update_all_required( Object *dist_obj, Ref<DatagramIterator> di )
{
    DCPacker packer;

    const char *data = (const char *)di->GetData()->GetBytes();
    packer.set_unpack_data( data + di->Tell(), di->GetRemainingSize(), false );

    int num_fields = _dcClass->get_num_inherited_fields();
    for ( int i = 0; i < num_fields; ++i )
    {
        DCField *field = _dcClass->get_inherited_field( i );
        if ( field->as_molecular_field() == nullptr && field->is_required() )
        {
            packer.begin_unpack( field );

            GDDCField dcField;
            dcField.set_dc_field( field );
            dcField.receive_update( dist_obj, packer );

            if ( !packer.end_unpack() )
            {
                break;
            }
        }
    }

    di->Skip( packer.get_num_unpacked_bytes() );
}

/**
 * Processes a datagram that lists some additional fields that are broadcast
 * in one chunk.
 */
void GDDCClass::receive_update_other( Object *dist_obj, Ref<DatagramIterator> di )
{
    int num_fields = di->GetUint16();
    for ( int i = 0; i < num_fields; ++i )
    {
        receive_update( dist_obj, di );
    }
}

/**
 * Looks up the current value of the indicated field by calling the
 * appropriate get*() function, then packs that value into the packer.  This
 * field is presumably either a required field or a specified optional field,
 * and we are building up a datagram for the generate-with-required message.
 *
 * Returns true on success, false on failure.
 */
bool GDDCClass::pack_required_field( DCPacker &packer, godot::Object *dist_obj,
                                     const DCField *field ) const
{
    const DCParameter *parameter = field->as_parameter();
    if ( parameter != nullptr )
    {
        // This is the easy case: to pack a parameter, we just look on the class
        // object for the data element.
        std::string field_name = field->get_name();

        Callable method( dist_obj, StringName( field_name.c_str() ) );

        if ( !method.is_valid() )
        {
            // If the attribute is not defined, but the field has a default value
            // specified, quietly pack the default value.
            if ( field->has_default_value() )
            {
                packer.pack_default_value();
                return true;
            }

            // If there is no default value specified, it's an error.
            ERR_PRINT( ( "Data element " + String( field_name.c_str() ) +
                         ", required by dc file for dclass " + get_name() +
                         ", not defined on object" )
                           .utf8()
                           .get_data() );
            return false;
        }

        Variant result = method.call();

        Array arr;
        arr.append( result );

        // Now pack the value into the datagram.
        GDDCField gdField;
        gdField.set_dc_field( (DCField *)parameter );
        bool pack_ok = gdField.pack_args( packer, arr );

        return pack_ok;
    }

    if ( field->as_molecular_field() != nullptr )
    {
        ERR_PRINT( ( "Cannot pack molecular field " + String( field->get_name().c_str() ) )
                       .utf8()
                       .get_data() );
        return false;
    }

    const DCAtomicField *atom = field->as_atomic_field();

    // We need to get the initial value of this field.  There isn't a good,
    // robust way to get this; presently, we just mangle the "setFoo()" name of
    // the required field into "getFoo()" and call that.
    std::string setter_name = atom->get_name();
    if ( setter_name.empty() )
    {
        ERR_PRINT( "Required field is unnamed!" );
        return false;
    }

    if ( atom->get_num_elements() == 0 )
    {
        // It sure doesn't make sense to have a required field with no parameters.
        // What data, exactly, is required?
        ERR_PRINT( ( "Required field " + String( setter_name.c_str() ) + " has no parameters!" )
                       .utf8()
                       .get_data() );
        return false;
    }

    std::string getter_name = setter_name;
    if ( setter_name.substr( 0, 3 ) == "set" )
    {
        // If the original method started with "set", we mangle this directly to
        // "get".
        getter_name[0] = 'g';
    }
    else
    {
        // Otherwise, we add a "get_" prefix.
        getter_name = "get_" + setter_name;
    }

    Callable method( dist_obj, StringName( getter_name.c_str() ) );

    // Now we have to look up the getter on the distributed object and call it.
    if ( !method.is_valid() )
    {
        // As above, if there's no getter but the field has a default value
        // specified, quietly pack the default value.
        if ( field->has_default_value() )
        {
            packer.pack_default_value();
            return true;
        }

        // Otherwise, with no default value it's an error.
        ERR_PRINT( ( "Distributed class " + get_name() + " doesn't have getter named " +
                     String( getter_name.c_str() ) + " to match required field " +
                     String( setter_name.c_str() ) )
                       .utf8()
                       .get_data() );
        return false;
    }

    Variant result = method.call();
    if ( result.get_type() == godot::Variant::NIL )
    {
        ERR_PRINT( ( "Error when calling " + String( getter_name.c_str() ) ).utf8().get_data() );
        return false;
    }

    if ( atom->get_num_elements() == 1 )
    {
        // In this case, we expect the getter to return one object, which we wrap
        // up in an array.
        Array arr;
        arr.append( result );

        result = arr;
    }
    else
    {
        // Otherwise, it had better already be a sequence or tuple of some sort.
        if ( result.get_type() != godot::Variant::ARRAY )
        {
            ERR_PRINT( ( "Since dclass " + get_name() + " method " + String( setter_name.c_str() ) +
                         " is declared to have multiple parameters, GDScript function " +
                         String( getter_name.c_str() ) + " must return an array." )
                           .utf8()
                           .get_data() );
            return false;
        }
    }

    // Now pack the arguments into the datagram.
    GDDCField gdField;
    gdField.set_dc_field( (DCField *)parameter );
    bool pack_ok = gdField.pack_args( packer, result );

    return pack_ok;
}

/// Bind our methods so GDScript can access them.
void GDDCClass::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "get_name" ), &GDDCClass::get_name );
    ClassDB::bind_method( D_METHOD( "get_number" ), &GDDCClass::get_number );

    ClassDB::bind_method( D_METHOD( "client_format_update", "field_name", "do_id", "args" ),
                          &GDDCClass::client_format_update );
    ClassDB::bind_method(
        D_METHOD( "ai_format_update", "field_name", "do_id", "to_id", "from_id", "args" ),
        &GDDCClass::ai_format_update );
    ClassDB::bind_method( D_METHOD( "ai_format_generate", "dist_obj", "do_id", "parent_id",
                                    "zone_id", "district_channel_id", "from_channel_id",
                                    "optional_fields" ),
                          &GDDCClass::ai_format_generate );

    ClassDB::bind_method( D_METHOD( "receive_update", "dist_obj", "di" ),
                          &GDDCClass::receive_update );
    ClassDB::bind_method( D_METHOD( "receive_update_broadcast_required", "dist_obj", "di" ),
                          &GDDCClass::receive_update_broadcast_required );
    ClassDB::bind_method( D_METHOD( "receive_update_broadcast_required_owner", "dist_obj", "di" ),
                          &GDDCClass::receive_update_broadcast_required_owner );
    ClassDB::bind_method( D_METHOD( "receive_update_all_required", "dist_obj", "di" ),
                          &GDDCClass::receive_update_all_required );
    ClassDB::bind_method( D_METHOD( "receive_update_other", "dist_obj", "di" ),
                          &GDDCClass::receive_update_other );
}
