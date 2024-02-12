#ifndef GDDCPARSER_GDDCCLASS_H
#define GDDCPARSER_GDDCCLASS_H

#include "godot_cpp/classes/ref_counted.hpp"

#include "Datagram.h"
#include "DatagramIterator.h"
#include "dclass/dcClass.h"

class GDDCClass : public godot::RefCounted
{
    GDCLASS( GDDCClass, godot::RefCounted )
public:
    void set_dc_class( DCClass *dcClass );

    godot::String get_name() const;
    int get_number() const;

    godot::Ref<Datagram> client_format_update( godot::String field_name, int do_id,
                                               godot::Array args );
    godot::Ref<Datagram> ai_format_update( godot::String field_name, int do_id, int to_id,
                                           int from_id, godot::Array args );

    godot::Array receive_update( godot::Ref<DatagramIterator> di );

private:
    static void _bind_methods();

    DCClass *_dcClass;
};

#endif // GDDCPARSER_GDDCCLASS_H
