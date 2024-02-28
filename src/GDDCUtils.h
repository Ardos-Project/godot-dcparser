#ifndef GDDCPARSER_GDDCUTILS_H
#define GDDCPARSER_GDDCUTILS_H

#include "godot_cpp/classes/object.hpp"

class GDDCUtils : public godot::Object
{
    GDCLASS( GDDCUtils, godot::Object )
public:
    enum FieldNameConvention : int32_t
    {
        FIELD_NAME_SNAKE_CASE,  // case_example
        FIELD_NAME_PASCAL_CASE, // CaseExample
        FIELD_NAME_CAMEL_CASE,  // caseExample
    };

    static godot::String GetFieldName( godot::Object *obj, const godot::String &name );

private:
    static void _bind_methods();
};

VARIANT_ENUM_CAST( GDDCUtils::FieldNameConvention );

#endif // GDDCPARSER_GDDCUTILS_H
