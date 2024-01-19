#include "DCFileLoader.h"

#include "godot_cpp/classes/file_access.hpp"

#include "DCFileResource.h"

using namespace godot;

Ref<Resource> DCFileLoader::load( const String &p_path, const String &p_original_path,
                                  Error *r_error, bool p_use_sub_threads, float *r_progress,
                                  ResourceLoader::CacheMode p_cache_mode )
{
    if ( r_error )
    {
        *r_error = ERR_FILE_CANT_OPEN;
    }

    if ( !FileAccess::file_exists( p_path ) )
    {
        *r_error = ERR_FILE_NOT_FOUND;
        return Ref<Resource>();
    }

    Ref<DCFileResource> dcFile;
    dcFile->set_data( FileAccess::get_file_as_string( p_path ) );

    if ( r_error )
    {
        *r_error = OK;
    }

    return dcFile;
}

void DCFileLoader::get_recognized_extensions( List<String> *p_extensions ) const
{
    p_extensions->push_back( "dc" );
}

bool DCFileLoader::handles_type( const String &p_type ) const
{
    return ( p_type == "DC" );
}

String DCFileLoader::get_resource_type( const String &p_path ) const
{
    String el = p_path.get_extension().to_lower();
    if ( el == "dc" )
    {
        return "DC";
    }

    return "";
}

void DCFileLoader::_bind_methods()
{
}
