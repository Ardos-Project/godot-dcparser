#ifndef GDDCPARSER_GDDCPARSER_H
#define GDDCPARSER_GDDCPARSER_H

#include "godot_cpp/classes/object.hpp"

namespace godot
{
    class ClassDB;
}

class GDDCParser : public godot::Object
{
    GDCLASS( GDDCParser, godot::Object )

public:
    static godot::String version();
    static godot::String godotCPPVersion();

private:
    static void _bind_methods();
};

#endif // GDDCPARSER_GDDCPARSER_H
