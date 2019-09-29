/////////////////////////////////////////////////////////////////////////////////////////////
//  memory_allocator.h
//
//  Memory Allocator Abstract Class
//  Created: 12-10-2011
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MEMORY_ALLOCATOR_H__
#define __MEMORY_ALLOCATOR_H__

#include "platformtypes.h"

enum alloc_type
{
    std_alloc,
    img_alloc,
};

/////////////////////////////////////////////////////////////////////////////////////////////
class memory_allocator
{
public:
    memory_allocator(){}

    virtual void*   alloc(uisize_t size) = 0;
    virtual void    dealloc(void *ptr) = 0;

private:
    // no copy
    memory_allocator(const memory_allocator &);
    memory_allocator & operator = (const memory_allocator &);
};
/////////////////////////////////////////////////////////////////////////////////////////////

#include "memory_allocator_new.h"

class memory_allocator_selector
{
public:
    inline static memory_allocator* get_allocator(alloc_type type, uisize_t size)
    {
        return &mem_new_alloc;
    }
};

#endif //__MEMORY_ALLOCATOR_H__