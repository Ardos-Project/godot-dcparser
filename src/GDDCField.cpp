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
godot::Ref<Datagram> GDDCField::client_format_update( int do_id, godot::Array args )
{
    DCPacker packer;

    packer.raw_pack_uint16( CLIENT_OBJECT_SET_FIELD );
    packer.raw_pack_uint32( do_id );
    packer.raw_pack_uint16( get_number() );

    packer.begin_pack( _dcField );
    pack_args( packer, std::move( args ) );
    if ( !packer.end_pack() )
    {
        return {};
    }

    Ref<Datagram> dg;
    dg->SetBytes( reinterpret_cast<const uint8_t *>( packer.get_data() ), packer.get_length() );

    return dg;
}

bool GDDCField::pack_args( DCPacker &packer, godot::Array args ) const
{
    pack_object( packer, std::move( args ) );
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
            Variant arg = args.front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_int64( arg.operator int64_t() );
                return;
            }
            break;
        }
        case PT_uint64:
        {
            Variant arg = args.front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_uint64( arg.operator uint64_t() );
                return;
            }
            break;
        }
        case PT_int:
        {
            Variant arg = args.front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_int( arg.operator int32_t() );
                return;
            }
            break;
        }
        case PT_uint:
        {
            Variant arg = args.front();
            if ( arg.get_type() == godot::Variant::INT )
            {
                packer.pack_uint( arg.operator uint32_t() );
                return;
            }
            break;
        }
        case PT_double:
        {
            Variant arg = args.front();
            if ( arg.get_type() == godot::Variant::FLOAT || arg.get_type() == godot::Variant::INT )
            {
                packer.pack_double( arg.operator double() );
                return;
            }
            break;
        }
        case PT_string:
        {
            Variant arg = args.front();
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
        default:
            break;
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
