#include "GDDCClass.h"

#include <utility>

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
    dcField.set_dc_field(field);

    return dcField.client_format_update(do_id, std::move(args));
}

/// Bind our methods so GDScript can access them.
void GDDCClass::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "get_name" ), &GDDCClass::get_name );
    ClassDB::bind_method( D_METHOD( "get_number" ), &GDDCClass::get_number );

    ClassDB::bind_method( D_METHOD( "client_format_update", "field_name", "do_id", "args" ),
                          &GDDCClass::client_format_update );
}
