#include "GDDCClass.h"

#include <utility>

#include "dclass/dcPacker.h"

#include "GDDCField.h"

using namespace godot;

void GDDCClass::set_dc_class( DCClass *dcClass )
{
    _dcClass = dcClass;
}

godot::String GDDCClass::get_name() const
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
godot::Ref<Datagram> GDDCClass::client_format_update( godot::String field_name, int do_id,
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
godot::Ref<Datagram> GDDCClass::ai_format_update( godot::String field_name, int do_id, int to_id,
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
 * Extracts the update message out of the packer and packs the individual
 * args into a Godot array to be used in a `Callable`.
 */
godot::Array GDDCClass::receive_update( godot::Ref<DatagramIterator> di )
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
        return {};
    }

    godot::Array updateData;

    GDDCField dcField;
    dcField.set_dc_field( field );

    // First arg in receive_update data is the field name.
    updateData.append( dcField.get_name() );

    packer.begin_unpack( field );
    dcField.receive_update( packer, updateData );
    packer.end_unpack();

    di->Skip( packer.get_num_unpacked_bytes() );

    return updateData;
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

    ClassDB::bind_method( D_METHOD( "receive_update", "di" ), &GDDCClass::receive_update );
}
