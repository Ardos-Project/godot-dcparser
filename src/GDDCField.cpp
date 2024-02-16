#include "GDDCField.h"

#include <utility>

#include "dclass/dcPacker.h"
#include "dclass/dcmsgtypes.h"

using namespace godot;

void GDDCField::set_dc_field( DCField *dcField )
{
    _dcField = dcField;
}

godot::String GDDCField::get_name() const
{
    return { _dcField->get_name().c_str() };
}

int GDDCField::get_number() const
{
    return _dcField->get_number();
}

/**
 * Generates a datagram containing the message necessary to send an update for
 * the indicated distributed object from the client.
 */
Ref<Datagram> GDDCField::client_format_update( int do_id, godot::Array args )
{
    DCPacker packer;

    packer.raw_pack_uint16( CLIENT_OBJECT_SET_FIELD );
    packer.raw_pack_uint32( do_id );
    packer.raw_pack_uint16( get_number() );

    packer.begin_pack( _dcField );
    pack_args( packer, args );
    if ( !packer.end_pack() )
    {
        return {};
    }

    Ref<Datagram> dg = memnew( Datagram );
    dg->SetBytes( reinterpret_cast<const uint8_t *>( packer.get_data() ), packer.get_length() );

    return dg;
}

/**
 * Generates a datagram containing the message necessary to send an update for
 * the indicated distributed object from the AI.
 */
Ref<Datagram> GDDCField::ai_format_update( int do_id, int to_id, int from_id, godot::Array args )
{
    DCPacker packer;

    packer.raw_pack_uint8( 1 );
    packer.RAW_PACK_CHANNEL( to_id );
    packer.RAW_PACK_CHANNEL( from_id );
    packer.raw_pack_uint16( STATESERVER_OBJECT_SET_FIELD );
    packer.raw_pack_uint32( do_id );
    packer.raw_pack_uint16( get_number() );

    packer.begin_pack( _dcField );
    pack_args( packer, std::move( args ) );
    if ( !packer.end_pack() )
    {
        return {};
    }

    Ref<Datagram> dg = memnew( Datagram );
    dg->SetBytes( reinterpret_cast<const uint8_t *>( packer.get_data() ), packer.get_length() );

    return dg;
}

/**
 * Extracts the update message out of the packer and packs the individual
 * args into a Godot array to be used in a `Callable`.
 */
void GDDCField::receive_update( DCPacker &packer, godot::Array args ) const
{
    // Unpack the arguments.
    // This *may* result in a recursive call-chain while it unpacks data
    // (if it's a more complex field type.)
    unpack_args( packer, args );
}

bool GDDCField::pack_args( DCPacker &packer, godot::Array args ) const
{
    pack_object( packer, args );
    if ( !packer.had_error() )
    {
        return true;
    }

    return false;
}

bool GDDCField::unpack_args( DCPacker &packer, godot::Array args ) const
{
    unpack_object( packer, std::move( args ) );
    if ( !packer.had_error() )
    {
        return true;
    }

    return false;
}

void GDDCField::pack_object( DCPacker &packer, godot::Array args ) const
{
    DCPackType pack_type = packer.get_pack_type();

    switch ( pack_type )
    {
        case PT_int64:
        {
            Variant arg = args.pop_front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_int64( arg.operator int64_t() );
                return;
            }
            break;
        }
        case PT_uint64:
        {
            Variant arg = args.pop_front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_uint64( arg.operator uint64_t() );
                return;
            }
            break;
        }
        case PT_int:
        {
            Variant arg = args.pop_front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_int( arg.operator int32_t() );
                return;
            }
            break;
        }
        case PT_uint:
        {
            Variant arg = args.pop_front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_uint( arg.operator uint32_t() );
                return;
            }
            break;
        }
        case PT_double:
        {
            Variant arg = args.pop_front();
            if ( arg.get_type() == godot::Variant::FLOAT || arg.get_type() == godot::Variant::INT )
            {
                packer.pack_double( arg.operator double() );
                return;
            }
            break;
        }
        case PT_string:
        {
            Variant arg = args.pop_front();
            if ( arg.get_type() == godot::Variant::STRING )
            {
                packer.pack_string( std::string( arg.operator String().utf8().get_data() ) );
                return;
            }
            break;
        }
        case PT_blob:
        {
            // TODO: How do we want to handle this?
            ERR_PRINT( "Blob packing not yet supported" );
            break;
        }
        case PT_array:
        {
            ERR_PRINT( "Array packing not yet supported" );
            break;
        }
        case PT_field:
        {
            packer.push();
            pack_object( packer, args );
            packer.pop();
            break;
        }
        case PT_class:
        {
            ERR_PRINT( "Class packing not yet supported" );
            break;
        }
        case PT_switch:
        {
            ERR_PRINT( "Switch packing not yet supported" );
            break;
        }
    }
}

void GDDCField::unpack_object( DCPacker &packer, godot::Array args ) const
{
    DCPackType pack_type = packer.get_pack_type();

    switch ( pack_type )
    {
        case PT_invalid:
        {
            args.append( Variant() );
            packer.unpack_skip();
            break;
        }
        case PT_double:
        {
            double value = packer.unpack_double();
            args.append( value );
            break;
        }
        case PT_int:
        {
            int value = packer.unpack_int();
            args.append( value );
            break;
        }
        case PT_uint:
        {
            unsigned int value = packer.unpack_uint();
            args.append( value );
            break;
        }
        case PT_int64:
        {
            int64_t value = packer.unpack_int64();
            args.append( value );
            break;
        }
        case PT_uint64:
        {
            uint64_t value = packer.unpack_uint64();
            args.append( value );
            break;
        }
        case PT_blob:
        {
            // TODO: Needs to be a bytes-like type here.
            std::string str;
            packer.unpack_string( str );
            args.append( String( str.c_str() ) );
            break;
        }
        case PT_string:
        {
            std::string str;
            packer.unpack_string( str );
            args.append( String( str.c_str() ) );
            break;
        }
        case PT_class:
        {
            ERR_PRINT( "Class unpacking not implemented" );
            break;
        }
        case PT_array:
        case PT_field:
        case PT_switch:
        {
            packer.push();
            while ( packer.more_nested_fields() )
            {
                unpack_object( packer, args );
            }
            packer.pop();
            break;
        }
    }
}

/// Bind our methods so GDScript can access them.
void GDDCField::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "get_name" ), &GDDCField::get_name );
    ClassDB::bind_method( D_METHOD( "get_number" ), &GDDCField::get_number );

    ClassDB::bind_method( D_METHOD( "client_format_update", "do_id", "args" ),
                          &GDDCField::client_format_update );
}
