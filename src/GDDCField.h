#ifndef GDDCPARSER_GDDCFIELD_H
#define GDDCPARSER_GDDCFIELD_H

#include "godot_cpp/classes/ref_counted.hpp"

#include "Datagram.h"
#include "dclass/dcField.h"

class GDDCField : public godot::RefCounted
{
    GDCLASS( GDDCField, godot::RefCounted )
public:
    void set_dc_field( DCField *dcField );

    godot::String get_name() const;
    int get_number() const;

    godot::Ref<Datagram> client_format_update( int do_id, godot::Array args );
    godot::Ref<Datagram> ai_format_update( int do_id, int to_id, int from_id, godot::Array args );

    void receive_update( godot::Object *dist_obj, DCPacker &packer ) const;

    bool pack_args( DCPacker &packer, godot::Array args ) const;
    bool unpack_args( DCPacker &packer, godot::Array args ) const;

private:
    static void _bind_methods();

    void pack_object( DCPacker &packer, godot::Array args ) const;
    void unpack_object( DCPacker &packer, godot::Array args ) const;

    DCField *_dcField;
};

#endif // GDDCPARSER_GDDCFIELD_H
