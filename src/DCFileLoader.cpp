#include "DCFileLoader.h"

#include "godot_cpp/classes/file_access.hpp"

#include "DCFileResource.h"

using namespace godot;

Variant DCFileLoader::_load( const godot::String &p_path, const godot::String &original_path,
                             bool use_sub_threads, int32_t cache_mode ) const
{
    if ( !FileAccess::file_exists( p_path ) )
    {
        return Variant();
    }

    DCFileResource *dcFile = memnew( DCFileResource );
    dcFile->set_name( p_path );
    dcFile->set_data( FileAccess::get_file_as_string( p_path ) );

    return Variant( dcFile );
}

PackedStringArray DCFileLoader::_get_recognized_extensions() const
{
    PackedStringArray arr;
    arr.push_back( "dc" );

    return arr;
}

bool DCFileLoader::_handles_type( const godot::StringName &p_type ) const
{
    return p_type.match( "DC" );
}

String DCFileLoader::_get_resource_type( const String &p_path ) const
{
    String el = p_path.get_extension().to_lower();
    if ( el == "dc" )
    {
        return "DC";
    }

    return "";
}
