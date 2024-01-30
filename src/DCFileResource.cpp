#include "DCFileResource.h"

using namespace godot;

void DCFileResource::set_data( const String &data )
{
    _data = data;
}

String DCFileResource::get_data()
{
    return _data;
}

void DCFileResource::set_name( const String &name )
{
    _name = name;
}

String DCFileResource::get_name()
{
    return _name;
}

void DCFileResource::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "set_data", "data" ), &DCFileResource::set_data );
    ClassDB::bind_method( D_METHOD( "get_data" ), &DCFileResource::get_data );

    ClassDB::bind_method( D_METHOD( "set_name", "name" ), &DCFileResource::set_name );
    ClassDB::bind_method( D_METHOD( "get_name" ), &DCFileResource::get_name );

    ADD_PROPERTY( PropertyInfo( Variant::STRING, "data" ), "set_data", "get_data" );
    ADD_PROPERTY( PropertyInfo( Variant::STRING, "name" ), "set_name", "get_name" );
}
