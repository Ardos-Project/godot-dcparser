#include "DatagramIterator.h"

using namespace godot;

DatagramIterator::DatagramIterator() : _offset( 0 )
{
}

DatagramIterator::~DatagramIterator() = default;

void DatagramIterator::SetData( Ref<Datagram> dg )
{
    _dg = dg;
    _offset = 0;
}

/**
 * Reads a boolean from the datagram.
 * @return
 */
bool DatagramIterator::GetBool()
{
    return GetUint8();
}

/**
 * Reads a signed 8-bit integer from the datagram.
 * @return
 */
int8_t DatagramIterator::GetInt8()
{
    EnsureLength( 1 );
    const int8_t v = *(int8_t *)( _dg->GetBytes() + _offset );
    _offset += 1;
    return v;
}

/**
 * Reads an unsigned 8-bit integer from the datagram.
 * @return
 */
uint8_t DatagramIterator::GetUint8()
{
    EnsureLength( 1 );
    const uint8_t v = *(uint8_t *)( _dg->GetBytes() + _offset );
    _offset += 1;
    return v;
}

/**
 * Reads a signed 16-bit integer from the datagram.
 * @return
 */
int16_t DatagramIterator::GetInt16()
{
    EnsureLength( 2 );
    const int16_t v = *(int16_t *)( _dg->GetBytes() + _offset );
    _offset += 2;
    return v;
}

/**
 * Reads an unsigned 16-bit integer from the datagram.
 * @return
 */
uint16_t DatagramIterator::GetUint16()
{
    EnsureLength( 2 );
    const uint16_t v = *(uint16_t *)( _dg->GetBytes() + _offset );
    _offset += 2;
    return v;
}

/**
 * Reads a signed 32-bit integer from the datagram.
 * @return
 */
int32_t DatagramIterator::GetInt32()
{
    EnsureLength( 4 );
    const int32_t v = *(int32_t *)( _dg->GetBytes() + _offset );
    _offset += 4;
    return v;
}

/**
 * Reads an unsigned 32-bit integer from the datagram.
 * @return
 */
uint32_t DatagramIterator::GetUint32()
{
    EnsureLength( 4 );
    const uint32_t v = *(uint32_t *)( _dg->GetBytes() + _offset );
    _offset += 4;
    return v;
}

/**
 * Reads a signed 64-bit integer from the datagram.
 * @return
 */
int64_t DatagramIterator::GetInt64()
{
    EnsureLength( 8 );
    const int64_t v = *(int64_t *)( _dg->GetBytes() + _offset );
    _offset += 8;
    return v;
}

/**
 * Reads an unsigned 64-bit integer from the datagram.
 * @return
 */
uint64_t DatagramIterator::GetUint64()
{
    EnsureLength( 8 );
    const uint64_t v = *(uint64_t *)( _dg->GetBytes() + _offset );
    _offset += 8;
    return v;
}

/**
 * Reads a 32-bit floating point number from the datagram.
 * @return
 */
float DatagramIterator::GetFloat32()
{
    EnsureLength( 4 );
    const float v = *(float *)( _dg->GetBytes() + _offset );
    _offset += 4;
    return v;
}

/**
 * Reads a 64-bit floating point number from the datagram.
 * @return
 */
double DatagramIterator::GetFloat64()
{
    EnsureLength( 8 );
    const double v = *(double *)( _dg->GetBytes() + _offset );
    _offset += 8;
    return v;
}

/**
 * Reads a string from the datagram.
 * @return
 */
godot::String DatagramIterator::GetString()
{
    const uint16_t length = GetUint16();
    EnsureLength( length );
    String str;
    str.parse_utf8( (char *)( _dg->GetBytes() + _offset ), length );
    _offset += length;
    return str;
}

/**
 * Returns the current read offset in bytes.
 * @return
 */
uint16_t DatagramIterator::Tell() const
{
    return _offset;
}

/**
 * Increases the read offset by the number of bytes.
 * @param bytes
 */
void DatagramIterator::Skip( const size_t &bytes )
{
    _offset += bytes;
}

/**
 * Sets the current read offset (in bytes).
 * @param offset
 */
void DatagramIterator::Seek( const size_t &offset )
{
    _offset = offset;
}

/**
 * Seeks to the beginning of this datagrams payload (sender).
 */
void DatagramIterator::SeekPayload()
{
    _offset = 0;

    const uint8_t channels = GetUint8();
    for ( int i = 0; i < channels; ++i )
    {
        GetUint64();
    }
}

/**
 * Returns the remaining read size in bytes.
 * @return
 */
size_t DatagramIterator::GetRemainingSize() const
{
    return _dg->Size() - _offset;
}

void DatagramIterator::EnsureLength( const size_t &length ) const
{
    // Make sure we don't overflow reading.
    const size_t newOffset = _offset + length;
    if ( newOffset > _dg->Size() )
    {
        ERR_PRINT( ( "DatagramIterator tried to read past Datagram length! Offset: " +
                     String( std::to_string( newOffset ).c_str() ) +
                     ", Size: " + String( std::to_string( _dg->Size() ).c_str() ) )
                       .utf8()
                       .get_data() );
    }
}

void DatagramIterator::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "set_data", "datagram" ), &DatagramIterator::SetData );

    ClassDB::bind_method( D_METHOD( "get_bool" ), &DatagramIterator::GetBool );
    ClassDB::bind_method( D_METHOD( "get_int8" ), &DatagramIterator::GetInt8 );
    ClassDB::bind_method( D_METHOD( "get_uint8" ), &DatagramIterator::GetUint8 );

    ClassDB::bind_method( D_METHOD( "get_int16" ), &DatagramIterator::GetInt16 );
    ClassDB::bind_method( D_METHOD( "get_uint16" ), &DatagramIterator::GetUint16 );

    ClassDB::bind_method( D_METHOD( "get_int32" ), &DatagramIterator::GetInt32 );
    ClassDB::bind_method( D_METHOD( "get_uint32" ), &DatagramIterator::GetUint32 );

    ClassDB::bind_method( D_METHOD( "get_int64" ), &DatagramIterator::GetInt64 );
    ClassDB::bind_method( D_METHOD( "get_uint64" ), &DatagramIterator::GetUint64 );

    ClassDB::bind_method( D_METHOD( "get_float32" ), &DatagramIterator::GetFloat32 );
    ClassDB::bind_method( D_METHOD( "get_float64" ), &DatagramIterator::GetFloat64 );

    ClassDB::bind_method( D_METHOD( "get_string" ), &DatagramIterator::GetString );

    ClassDB::bind_method( D_METHOD( "tell" ), &DatagramIterator::Tell );
    ClassDB::bind_method( D_METHOD( "skip", "bytes" ), &DatagramIterator::Skip );
    ClassDB::bind_method( D_METHOD( "seek", "bytes" ), &DatagramIterator::Seek );
    ClassDB::bind_method( D_METHOD( "seek_payload" ), &DatagramIterator::SeekPayload );

    ClassDB::bind_method( D_METHOD( "get_remaining_size" ), &DatagramIterator::GetRemainingSize );
}
