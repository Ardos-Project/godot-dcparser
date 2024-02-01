#include "GDDCClass.h"

using namespace godot;

void GDDCClass::set_dc_class( DCClass *dcClass )
{
    _dcClass = dcClass;
}

godot::String GDDCClass::get_name() const
{
    return {_dcClass->get_name().c_str()};
}

int GDDCClass::get_number() const
{
    return _dcClass->get_number();
}

/// Bind our methods so GDScript can access them.
void GDDCClass::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "get_name" ), &GDDCClass::get_name );
    ClassDB::bind_method( D_METHOD( "get_number" ), &GDDCClass::get_number );
}
