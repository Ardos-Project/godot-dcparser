#ifndef GDDCPARSER_DATAGRAMITERATOR_H
#define GDDCPARSER_DATAGRAMITERATOR_H

#include <memory>

#include "godot_cpp/classes/ref_counted.hpp"

#include "Datagram.h"

class DatagramIterator : public godot::RefCounted
{
    GDCLASS( DatagramIterator, godot::RefCounted )
public:
    DatagramIterator();
    ~DatagramIterator();

    void SetData( godot::Ref<Datagram> dg );

    bool GetBool();
    int8_t GetInt8();
    uint8_t GetUint8();

    int16_t GetInt16();
    uint16_t GetUint16();

    int32_t GetInt32();
    uint32_t GetUint32();

    int64_t GetInt64();
    uint64_t GetUint64();

    float GetFloat32();
    double GetFloat64();

    godot::String GetString();

    [[nodiscard]] uint16_t Tell() const;
    void Skip( const size_t &bytes );
    void Seek( const size_t &offset );
    void SeekPayload();

    [[nodiscard]] size_t GetRemainingSize() const;

private:
    static void _bind_methods();

    void EnsureLength( const size_t &length ) const;

    godot::Ref<Datagram> _dg;
    size_t _offset;
};

#endif // GDDCPARSER_DATAGRAMITERATOR_H
