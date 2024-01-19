#ifndef GDDCPARSER_DCFILERESOURCE_H
#define GDDCPARSER_DCFILERESOURCE_H

#include "godot_cpp/classes/ref_counted.hpp"

namespace godot
{
    class ClassDB;
}

class DCFileResource : public godot::RefCounted
{
    GDCLASS( DCFileResource, RefCounted )
public:
    void set_data( const godot::String &data, const godot::String &name = "" );
    godot::String get_data();
    godot::String get_name();

private:
    static void _bind_methods();

    godot::String _data;
    godot::String _name;
};

#endif // GDDCPARSER_DCFILERESOURCE_H