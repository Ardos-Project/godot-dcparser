#include "GDDCFile.h"

GDDCFile::GDDCFile()
{
    _dcFile = new DCFile();
}

GDDCFile::~GDDCFile()
{
    _dcFile->clear();
    delete _dcFile;
}

void GDDCFile::clear() {
    _dcFile->clear();
}

/// Bind our methods so GDScript can access them.
void GDDCFile::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("clear"), &GDDCFile::clear);
}
