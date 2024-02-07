#include "Datagram.h"

#include "godot_cpp/variant/utility_functions.hpp"

using namespace godot;

Datagram::Datagram() : _buf( new uint8_t[kMinDgSize] ), _bufOffset( 0 ), _bufLength( kMinDgSize )
{
}

Datagram::~Datagram()
{
    delete[] _buf;
}

/**
 * Clears this datagram of data ready for rewriting.
 * Good for re-using datagrams rather than re-alloc.
 */
void Datagram::Clear()
{
    // Wipe out the buffer offset without deleting it.
    // This should prevent redundant re-sizing.
    _bufOffset = 0;
}

/**
 * Returns the number of bytes added to this datagram.
 * @return
 */
uint16_t Datagram::Size() const
{
    return _bufOffset;
}

/**
 * Returns the underlying data pointer for this datagram.
 * @return
 */
PackedByteArray Datagram::GetData() const
{
    PackedByteArray arr;
    arr.resize( sizeof( uint16_t ) + _bufOffset );
    uint8_t *w = arr.ptrw();

    // Datagram size tag.
    memcpy( w, &_bufOffset, sizeof( uint16_t ) );
    // Datagram data.
    memcpy( w + sizeof( uint16_t ), _buf, _bufOffset );

    return arr;
}

/**
 * Adds a boolean to this datagram.
 * @param v
 */
void Datagram::AddBool( const bool &v )
{
    AddUint8( v ? 1 : 0 );
}

/**
 * Adds a signed 8-bit integer to this datagram.
 * @param v
 */
void Datagram::AddInt8( const int8_t &v )
{
    EnsureLength( 1 );
    *(int8_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 1;
}

/**
 * Adds an unsigned 8-bit integer to this datagram.
 * @param v
 */
void Datagram::AddUint8( const uint8_t &v )
{
    EnsureLength( 1 );
    *(uint8_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 1;
}

/**
 * Adds a signed 16-bit integer to this datagram.
 * @param v
 */
void Datagram::AddInt16( const int16_t &v )
{
    EnsureLength( 2 );
    *(int16_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 2;
}

/**
 * Adds an unsigned 16-bit integer to this datagram.
 * @param v
 */
void Datagram::AddUint16( const uint16_t &v )
{
    EnsureLength( 2 );
    *(uint16_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 2;
}

/**
 * Adds a signed 32-bit integer to this datagram.
 * @param v
 */
void Datagram::AddInt32( const int32_t &v )
{
    EnsureLength( 4 );
    *(int32_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 4;
}

/**
 * Adds an unsigned 32-bit integer to this datagram.
 * @param v
 */
void Datagram::AddUint32( const uint32_t &v )
{
    EnsureLength( 4 );
    *(uint32_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 4;
}

/**
 * Adds a signed 64-bit integer to this datagram.
 * @param v
 */
void Datagram::AddInt64( const int64_t &v )
{
    EnsureLength( 8 );
    *(int64_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 8;
}

/**
 * Adds an unsigned 64-bit integer to this datagram.
 * @param v
 */
void Datagram::AddUint64( const uint64_t &v )
{
    EnsureLength( 8 );
    *(uint64_t *)( _buf + _bufOffset ) = v;
    _bufOffset += 8;
}

/**
 * Adds a 32-bit floating point number to this datagram.
 * @param v
 */
void Datagram::AddFloat32( const float &v )
{
    EnsureLength( 4 );
    *(float *)( _buf + _bufOffset ) = v;
    _bufOffset += 4;
}

/**
 * Adds a 64-bit floating point number to this datagram.
 * @param v
 */
void Datagram::AddFloat64( const double &v )
{
    EnsureLength( 8 );
    *(double *)( _buf + _bufOffset ) = v;
    _bufOffset += 8;
}

/**
 * Adds a string to this datagram.
 * NOTE: Strings are limited to a max-length of a uint16_t (65k)
 * @param v
 */
void Datagram::AddString( const godot::String &v )
{
    EnsureLength( v.length() + 2 ); // +2 for length tag.
    AddUint16( v.length() );
    memcpy( _buf + _bufOffset, v.utf8().get_data(), v.length() );
    _bufOffset += v.length();
}

/**
 * Adds a blob of arbitrary data to this datagram.
 * NOTE: Blobs are limited to a max-length of a uint16_t (65k)
 * @param v
 */
void Datagram::AddBlob( const PackedByteArray &v )
{
    EnsureLength( v.size() + 2 ); // +2 for length tag.
    AddUint16( v.size() );
    memcpy( _buf + _bufOffset, &v[0], v.size() );
    _bufOffset += v.size();
}

/**
 * Adds a location pair to this datagram.
 * @param parentId
 * @param zoneId
 */
void Datagram::AddLocation( const uint32_t &parentId, const uint32_t &zoneId )
{
    AddUint32( parentId );
    AddUint32( zoneId );
}

void Datagram::EnsureLength( const size_t &length )
{
    // Make sure we don't overflow.
    size_t newOffset = _bufOffset + length;
    if ( newOffset > kMaxDgSize )
    {
        ERR_PRINT( ( "Datagram exceeded max size! " +
                     String( std::to_string( _bufOffset ).c_str() ) + " => " +
                     String( std::to_string( newOffset ).c_str() ) )
                       .utf8()
                       .get_data() );
        return;
    }

    // Do we need to resize the buffer?
    if ( newOffset > _bufLength )
    {
        const size_t newLength = _bufLength + kMinDgSize + length;

        // Copy our old buffer into a new one.
        auto *tempBuf = new uint8_t[newLength];
        memcpy( tempBuf, _buf, _bufLength );

        // Clear out the old buffer and assign the new one.
        delete[] _buf;
        _buf = tempBuf;
        _bufLength = newLength;
    }
}

void Datagram::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "clear" ), &Datagram::Clear );

    ClassDB::bind_method( D_METHOD( "size" ), &Datagram::Size );
    ClassDB::bind_method( D_METHOD( "get_data" ), &Datagram::GetData );

    ClassDB::bind_method( D_METHOD( "add_bool", "value" ), &Datagram::AddBool );
    ClassDB::bind_method( D_METHOD( "add_int8", "value" ), &Datagram::AddInt8 );
    ClassDB::bind_method( D_METHOD( "add_uint8", "value" ), &Datagram::AddUint8 );

    ClassDB::bind_method( D_METHOD( "add_int16", "value" ), &Datagram::AddInt16 );
    ClassDB::bind_method( D_METHOD( "add_uint16", "value" ), &Datagram::AddUint16 );

    ClassDB::bind_method( D_METHOD( "add_int32", "value" ), &Datagram::AddInt32 );
    ClassDB::bind_method( D_METHOD( "add_uint32", "value" ), &Datagram::AddUint32 );

    ClassDB::bind_method( D_METHOD( "add_int64", "value" ), &Datagram::AddInt64 );
    ClassDB::bind_method( D_METHOD( "add_uint64", "value" ), &Datagram::AddUint64 );

    ClassDB::bind_method( D_METHOD( "add_float32", "value" ), &Datagram::AddFloat32 );
    ClassDB::bind_method( D_METHOD( "add_float64", "value" ), &Datagram::AddFloat64 );

    ClassDB::bind_method( D_METHOD( "add_string", "value" ), &Datagram::AddString );
    ClassDB::bind_method( D_METHOD( "add_blob", "value" ), &Datagram::AddBlob );

    ClassDB::bind_method( D_METHOD( "add_location", "parent_id", "zone_id" ),
                          &Datagram::AddLocation );
}
