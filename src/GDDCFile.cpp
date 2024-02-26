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

GDDCClass *GDDCFile::get_dc_class( int n ) const
{
    DCClass *dcClass = _dcFile->get_class( n );

    GDDCClass *ret = memnew(GDDCClass);
    ret->set_dc_class(dcClass);

    return ret;
}

int GDDCFile::get_num_import_modules() const
{
    return _dcFile->get_num_import_modules();
}

godot::String GDDCFile::get_import_module( int n ) const
{
    return { _dcFile->get_import_module( n ).c_str() };
}

int GDDCFile::get_num_import_symbols( int n ) const
{
    return _dcFile->get_num_import_symbols( n );
}

godot::String GDDCFile::get_import_symbol( int n, int i ) const
{
    return { _dcFile->get_import_symbol( n, i ).c_str() };
}

uint32_t GDDCFile::get_hash() const
{
    return _dcFile->get_hash();
}

/// Bind our methods so GDScript can access them.
void GDDCFile::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "clear" ), &GDDCFile::clear );

    ClassDB::bind_method( D_METHOD( "read" ), &GDDCFile::read );

    ClassDB::bind_method( D_METHOD( "get_num_classes" ), &GDDCFile::get_num_classes );
    ClassDB::bind_method( D_METHOD( "get_dc_class", "number" ), &GDDCFile::get_dc_class );

    ClassDB::bind_method( D_METHOD( "get_num_import_modules" ), &GDDCFile::get_num_import_modules );
    ClassDB::bind_method( D_METHOD( "get_import_module", "number" ), &GDDCFile::get_import_module );
    ClassDB::bind_method( D_METHOD( "get_num_import_symbols", "number" ),
                          &GDDCFile::get_num_import_symbols );
    ClassDB::bind_method( D_METHOD( "get_import_symbol", "number", "index" ),
                          &GDDCFile::get_import_symbol );

    ClassDB::bind_method( D_METHOD( "get_hash" ), &GDDCFile::get_hash );
}
