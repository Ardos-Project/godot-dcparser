#ifndef GDDCPARSER_GDDCFILE_H
#define GDDCPARSER_GDDCFILE_H

#include "godot_cpp/classes/ref_counted.hpp"

#include "DCFileResource.h"
#include "GDDCClass.h"
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
    GDDCClass *get_dc_class( int n ) const;

    int get_num_import_modules() const;
    godot::String get_import_module( int n ) const;
    int get_num_import_symbols( int n ) const;
    godot::String get_import_symbol( int n, int i ) const;

    uint32_t get_hash() const;

private:
    static void _bind_methods();

    DCFile *_dcFile;
};

#endif // GDDCPARSER_GDDCFILE_H
