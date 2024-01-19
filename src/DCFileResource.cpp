#include "DCFileResource.h"

using namespace godot;

void DCFileResource::set_data( const String &data, const String &name )
{
    _data = data;
    _name = name;
}

String DCFileResource::get_data()
{
    return _data;
}

String DCFileResource::get_name()
{
    return _name;
}

void DCFileResource::_bind_methods()
{
}
