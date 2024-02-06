#ifndef GDDCPARSER_DATAGRAM_H
#define GDDCPARSER_DATAGRAM_H

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include "godot_cpp/classes/ref_counted.hpp"

// Max amount of data we can have is an uint16 (65k bytes)
// -2 for the required prepended length tag.
constexpr size_t kMaxDgSize = 0xffff - 2;
// 128 bytes seems like a good minimum datagram size.
constexpr size_t kMinDgSize = 0x80;

class Datagram : public godot::RefCounted
{
    GDCLASS( Datagram, godot::RefCounted )
public:
    Datagram();
    ~Datagram();

    void Clear();

    uint16_t Size() const;
    godot::PackedByteArray GetData() const;

    void AddBool( const bool &v );
    void AddInt8( const int8_t &v );
    void AddUint8( const uint8_t &v );

    void AddInt16( const int16_t &v );
    void AddUint16( const uint16_t &v );

    void AddInt32( const int32_t &v );
    void AddUint32( const uint32_t &v );

    void AddInt64( const int64_t &v );
    void AddUint64( const uint64_t &v );

    void AddFloat32( const float &v );
    void AddFloat64( const double &v );

    void AddString( const godot::String &v );
    void AddBlob( const godot::PackedByteArray &v );

    void AddLocation( const uint32_t &parentId, const uint32_t &zoneId );

private:
    static void _bind_methods();

    void EnsureLength( const size_t &length );

    uint8_t *_buf;
    size_t _bufOffset;
    size_t _bufLength;
};

#endif // GDDCPARSER_DATAGRAM_H
