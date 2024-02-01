#ifndef GDDCPARSER_GDDCCLASS_H
#define GDDCPARSER_GDDCCLASS_H

#include "godot_cpp/classes/ref_counted.hpp"

#include "dclass/dcClass.h"

class GDDCClass : public godot::RefCounted
{
    GDCLASS( GDDCClass, godot::RefCounted )
public:
    void set_dc_class( DCClass *dcClass );

    godot::String get_name() const;
    int get_number() const;

private:
    static void _bind_methods();

    DCClass *_dcClass;
};

#endif // GDDCPARSER_GDDCCLASS_H
