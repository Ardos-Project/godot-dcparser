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

/// Bind our methods so GDScript can access them.
void GDDCFile::_bind_methods()
{

}
