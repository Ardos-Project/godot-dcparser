#include "DCFileResource.h"

using namespace godot;

void DCFileResource::set_file_data( const String &data )
{
    _data = data;
}

String DCFileResource::get_file_data()
{
    return _data;
}

void DCFileResource::set_file_name( const String &name )
{
    _name = name;
}

String DCFileResource::get_file_name()
{
    return _name;
}

void DCFileResource::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "set_file_data", "data" ), &DCFileResource::set_file_data );
    ClassDB::bind_method( D_METHOD( "get_file_data" ), &DCFileResource::get_file_data );

    ClassDB::bind_method( D_METHOD( "set_file_name", "name" ), &DCFileResource::set_file_name );
    ClassDB::bind_method( D_METHOD( "get_file_name" ), &DCFileResource::get_file_name );

    ADD_PROPERTY( PropertyInfo( Variant::STRING, "file_data" ), "set_file_data", "get_file_data" );
    ADD_PROPERTY( PropertyInfo( Variant::STRING, "file_name" ), "set_file_name", "get_file_name" );
}
