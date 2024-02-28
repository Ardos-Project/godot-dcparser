#include "godot_cpp/core/class_db.hpp"

#include "GDDCUtils.h"

using namespace godot;

String GDDCUtils::GetFieldName( Object *obj,
                                const godot::String &name )
{
    Variant fieldNameType = obj->get( "FIELD_NAME_TYPE" );
    if ( fieldNameType.get_type() == godot::Variant::NIL )
    {
        // Default to snake_case (GDScript).
        fieldNameType = GDDCUtils::FIELD_NAME_SNAKE_CASE;
    }

    auto fieldType = static_cast<FieldNameConvention>(fieldNameType.operator int32_t());
    switch ( fieldType )
    {
        case FIELD_NAME_SNAKE_CASE:
            return name.to_snake_case();
        case FIELD_NAME_PASCAL_CASE:
            return name.to_pascal_case();
        case FIELD_NAME_CAMEL_CASE:
            return name.to_camel_case();
        default:
            return name.to_snake_case();
    }
}

/// Bind our methods so GDScript can access them.
void GDDCUtils::_bind_methods()
{
    BIND_ENUM_CONSTANT( FIELD_NAME_SNAKE_CASE )
    BIND_ENUM_CONSTANT( FIELD_NAME_PASCAL_CASE )
    BIND_ENUM_CONSTANT( FIELD_NAME_CAMEL_CASE )
}
