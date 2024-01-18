#ifndef GDDCPARSER_GDDCFILE_H
#define GDDCPARSER_GDDCFILE_H

#include "godot_cpp/classes/ref_counted.hpp"

#include "dclass/dcFile.h"

namespace godot
{
    class ClassDB;
}

class GDDCFile : public godot::RefCounted
{
    GDCLASS( GDDCFile, godot::RefCounted )
public:
    GDDCFile();
    ~GDDCFile();

private:
    static void _bind_methods();

    DCFile *_dcFile;
};

#endif // GDDCPARSER_GDDCFILE_H
