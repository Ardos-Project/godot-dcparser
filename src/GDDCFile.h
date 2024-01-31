#ifndef GDDCPARSER_GDDCFILE_H
#define GDDCPARSER_GDDCFILE_H

#include "godot_cpp/classes/ref_counted.hpp"

#include "DCFileResource.h"
#include "dclass/dcFile.h"

class GDDCFile : public godot::RefCounted
{
    GDCLASS( GDDCFile, godot::RefCounted )
public:
    GDDCFile();
    ~GDDCFile();

    void clear();

    bool read( godot::Ref<DCFileResource> file ) const;

    int get_num_classes() const;

private:
    static void _bind_methods();

    DCFile *_dcFile;
};

#endif // GDDCPARSER_GDDCFILE_H
