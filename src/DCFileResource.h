#ifndef GDDCPARSER_DCFILERESOURCE_H
#define GDDCPARSER_DCFILERESOURCE_H

#include "godot_cpp/classes/resource.hpp"

namespace godot
{
    class ClassDB;
}

class DCFileResource : public godot::Resource
{
    GDCLASS( DCFileResource, godot::Resource )
public:
    void set_file_data( const godot::String &data );
    godot::String get_file_data();

    void set_file_name( const godot::String &name );
    godot::String get_file_name();

private:
    static void _bind_methods();

    godot::String _data;
    godot::String _name;
};

#endif // GDDCPARSER_DCFILERESOURCE_H