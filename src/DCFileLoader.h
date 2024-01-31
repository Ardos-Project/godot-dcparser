#ifndef GDDCPARSER_DCFILELOADER_H
#define GDDCPARSER_DCFILELOADER_H

#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/resource_format_loader.hpp"
#include "godot_cpp/classes/resource_loader.hpp"

class DCFileLoader : public godot::ResourceFormatLoader
{
    GDCLASS( DCFileLoader, godot::ResourceFormatLoader )
public:
    godot::Variant _load( const godot::String &p_path, const godot::String &original_path,
                          bool use_sub_threads, int32_t cache_mode ) const override;
    godot::PackedStringArray _get_recognized_extensions() const override;
    bool _handles_type( const godot::StringName &p_type ) const override;
    godot::String _get_resource_type( const godot::String &p_path ) const override;

protected:
    static void _bind_methods(){};
};

#endif // GDDCPARSER_DCFILELOADER_H
