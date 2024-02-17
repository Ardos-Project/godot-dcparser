#ifndef GDDCPARSER_GDDCCLASS_H
#define GDDCPARSER_GDDCCLASS_H

#include "godot_cpp/classes/object.hpp"
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
    godot::Ref<Datagram> ai_format_generate( godot::Object *dist_obj, uint32_t do_id,
                                             uint32_t parent_id, uint32_t zone_id,
                                             uint64_t district_channel_id,
                                             uint64_t from_channel_id );

    godot::Array receive_update( godot::Ref<DatagramIterator> di );

    bool pack_required_field( DCPacker &packer, godot::Object *dist_obj,
                              const DCField *field ) const;

private:
    static void _bind_methods();

    DCClass *_dcClass;
};

#endif // GDDCPARSER_GDDCCLASS_H
