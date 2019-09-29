/////////////////////////////////////////////////////////////////////////////////////////////
//  memory_allocator_new.h
//
//  Memory Allocator Class Using New Operator
//  Created: 12-10-2011
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MEMORY_ALLOCATOR_NEW_H__
#define __MEMORY_ALLOCATOR_NEW_H__

class memory_new_allocator : public memory_allocator
{
public:
    void*   alloc(uisize_t size);
    void    dealloc(void *ptr);
};

// uses new/delete for memory allocation
extern memory_new_allocator mem_new_alloc;


#endif //__MEMORY_ALLOCATOR_NEW_H__