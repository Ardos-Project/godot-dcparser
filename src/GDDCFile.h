#ifndef GDDCPARSER_GDDCFILE_H
#define GDDCPARSER_GDDCFILE_H

#include "godot_cpp/classes/object.hpp"

#include "dclass/dcFile.h"

namespace godot
{
    class ClassDB;
}

class GDDCFile : public godot::Object, public DCFile
{
    GDCLASS( GDDCFile, godot::Object )

private:
    static void _bind_methods();
};

#endif // GDDCPARSER_GDDCFILE_H
