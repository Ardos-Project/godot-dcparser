#include "GDDCFile.h"

using namespace godot;

GDDCFile::GDDCFile()
{
    _dcFile = new DCFile();
}

GDDCFile::~GDDCFile()
{
    _dcFile->clear();
    delete _dcFile;
}

void GDDCFile::clear()
{
    _dcFile->clear();
}

bool GDDCFile::read( Ref<DCFileResource> file ) const
{
    std::istringstream fileStream( file->get_file_data().utf8().get_data() );
    return _dcFile->read( fileStream, file->get_name().utf8().get_data() );
}

int GDDCFile::get_num_classes() const
{
    return _dcFile->get_num_classes();
}

/// Bind our methods so GDScript can access them.
void GDDCFile::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "clear" ), &GDDCFile::clear );
    ClassDB::bind_method( D_METHOD( "read" ), &GDDCFile::read );
    ClassDB::bind_method( D_METHOD( "get_num_classes" ), &GDDCFile::get_num_classes );
}
