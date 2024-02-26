#include "UniqueIdAllocator.h"

using namespace godot;

const uint32_t UniqueIdAllocator::IndexEnd = (uint32_t)-1;
const uint32_t UniqueIdAllocator::IndexAllocated = (uint32_t)-2;

UniqueIdAllocator::UniqueIdAllocator() = default;

UniqueIdAllocator::~UniqueIdAllocator()
{
    delete[] _table;
}

void UniqueIdAllocator::set_range( uint32_t min, uint32_t max )
{
    _min = min;
    _max = max;

    _size = _max - _min + 1; // +1 because min and max are inclusive.

    _table = (uint32_t *)malloc( _size * sizeof( uint32_t ) );

    for ( uint32_t i = 0; i < _size; ++i )
    {
        _table[i] = i + 1;
    }
    _table[_size - 1] = IndexEnd;
    _next_free = 0;
    _last_free = _size - 1;
    _free = _size;
}

/**
 * Returns an id between _min and _max (that were passed to the constructor).
 * IndexEnd is returned if no ids are available.
 */
uint32_t UniqueIdAllocator::allocate()
{
    if ( _next_free == IndexEnd )
    {
        // ...all ids allocated.
        return IndexEnd;
    }
    uint32_t index = _next_free;

    _next_free = _table[_next_free];
    _table[index] = IndexAllocated;

    --_free;

    uint32_t id = index + _min;
    return id;
}

/**
 * This may be called to mark a particular id as having already been allocated
 * (for instance, by a prior pass).  The specified id is removed from the
 * available pool.
 *
 * Because of the limitations of this algorithm, this is most efficient when
 * it is called before the first call to allocate(), and when all the calls to
 * initial_reserve_id() are made in descending order by id.  However, this is
 * a performance warning only; if performance is not an issue, any id may be
 * reserved at any time.
 */
void UniqueIdAllocator::initial_reserve_id( uint32_t id )
{
    uint32_t index = id - _min; // Convert to _table index.

    if ( _free == 1 )
    {
        // We just reserved the last element in the free chain.
        _next_free = IndexEnd;
    }
    else if ( _next_free == index )
    {
        // We're reserving the head of the free chain.
        _next_free = _table[index];
    }
    else
    {
        // Since we don't store back pointers in the free chain, we have to search
        // for the element in the free chain that points to this index.

        /*
         * However, there is an easy optimal case: because we expect that this call
         * will be made before any calls to allocate(), hopefully is it still true
         * that the _table is still set up such that _table[i] = i+1 (and if the
         * numbers are reserved in descending order, this will be true at least for
         * all i <= index).  Thus, the free link to slot [index] is expected to be the
         * slot right before it, or if not, it usually won't be far before it.
         */

        uint32_t prev_index = index;
        while ( prev_index > 0 && _table[prev_index - 1] != index )
        {
            --prev_index;
        }
        if ( prev_index > 0 && _table[prev_index - 1] == index )
        {
            // We've found it.
            --prev_index;
        }
        else
        {
            // OK, it wasn't found below; we have to search above.
            prev_index = index + 1;
            while ( prev_index < _size && _table[prev_index] != index )
            {
                ++prev_index;
            }
        }

        _table[prev_index] = _table[index];

        if ( index == _last_free )
        {
            _last_free = prev_index;
        }
    }

    _table[index] = IndexAllocated;
    --_free;
}

/**
 * Checks the allocated state of an index. Returns true for
 * indices that are currently allocated and in use.
 */
bool UniqueIdAllocator::is_allocated( uint32_t id )
{
    if ( id < _min || id > _max )
    {
        // This id is out of range, not allocated.
        return false;
    }

    uint32_t index = id - _min; // Convert to _table index.
    return _table[index] == IndexAllocated;
}

/**
 * Free an allocated index (index must be between _min and _max that were
 * passed to the constructor).
 *
 * Since 1.11.0, returns true if the index has been freed successfully
 * or false if the index has not been allocated yet, instead of
 * triggering an assertion.
 */
bool UniqueIdAllocator::free_id( uint32_t id )
{
    if ( id < _min || id > _max )
    {
        // Attempt to free out-of-range id.
        return false;
    }

    uint32_t index = id - _min; // Convert to _table index.

    if ( _table[index] != IndexAllocated )
    {
        // Attempt to free non-allocated id.
        return false;
    }

    if ( _next_free != IndexEnd )
    {
        _table[_last_free] = index;
    }

    _table[index] = IndexEnd; // Mark this element as the end of the list.
    _last_free = index;

    if ( _next_free == IndexEnd )
    {
        // ...the free list was empty.
        _next_free = index;
    }

    ++_free;
    return true;
}

/**
 * return the decimal fraction of the pool that is used.  The range is 0 to
 * 1.0 (e.g.  75% would be 0.75).
 */
float UniqueIdAllocator::fraction_used() const
{
    return float( _size - _free ) / _size;
}

/// Bind our methods so GDScript can access them.
void UniqueIdAllocator::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "set_range", "min", "max" ), &UniqueIdAllocator::set_range );

    ClassDB::bind_method( D_METHOD( "allocate" ), &UniqueIdAllocator::allocate );
    ClassDB::bind_method( D_METHOD( "initial_reserve_id", "id" ),
                          &UniqueIdAllocator::initial_reserve_id );

    ClassDB::bind_method( D_METHOD( "is_allocated", "id" ), &UniqueIdAllocator::is_allocated );

    ClassDB::bind_method( D_METHOD( "free_id", "id" ), &UniqueIdAllocator::free_id );
    ClassDB::bind_method( D_METHOD( "fraction_used" ), &UniqueIdAllocator::fraction_used );
}
