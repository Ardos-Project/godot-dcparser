#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/version.hpp"

#include "GDDCParser.h"
#include "Version.h"

using namespace godot;

String GDDCParser::version()
{
    return VersionInfo::VERSION_STR.data();
}

/*!
@brief Get the godot-cpp version string for this extension.

@details
The version string is generated using godot-cpp's core/version.hpp.

@return The version string (e.g. "godot-cpp v4.2.0-stable").
*/
String GDDCParser::godotCPPVersion()
{
    return "godot-cpp v" + uitos( GODOT_VERSION_MAJOR ) + "." +
           godot::uitos( GODOT_VERSION_MINOR ) + "." + uitos( GODOT_VERSION_PATCH ) + "-" +
           GODOT_VERSION_STATUS;
}

/// Bind our methods so GDScript can access them.
void GDDCParser::_bind_methods()
{
    ClassDB::bind_static_method( "GDDCParser", D_METHOD( "version" ), &GDDCParser::version );
    ClassDB::bind_static_method( "GDDCParser", D_METHOD( "godot_cpp_version" ),
                                 &GDDCParser::godotCPPVersion );
}
