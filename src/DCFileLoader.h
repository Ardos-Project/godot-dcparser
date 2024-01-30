#ifndef GDDCPARSER_DCFILELOADER_H
#define GDDCPARSER_DCFILELOADER_H

#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/resource_format_loader.hpp"
#include "godot_cpp/classes/resource_loader.hpp"

class DCFileLoader : public godot::ResourceFormatLoader
{
public:
    virtual godot::Ref<godot::Resource> load(
        const godot::String &p_path, const godot::String &p_original_path = "",
        godot::Error *r_error = nullptr, bool p_use_sub_threads = false,
        float *r_progress = nullptr,
        godot::ResourceLoader::CacheMode p_cache_mode = godot::ResourceLoader::CACHE_MODE_REUSE );
    virtual void get_recognized_extensions( godot::List<godot::String> *p_extensions ) const;
    virtual bool handles_type( const godot::String &type ) const;
    virtual godot::String get_resource_type( const godot::String &p_path ) const;
};

#endif // GDDCPARSER_DCFILELOADER_H
