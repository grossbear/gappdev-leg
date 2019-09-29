///////////////////////////////////////////////////////////////////////////////////////
//  PlatformTypes.h
//
//  Defines Common Platform Types
//  Created: 27-02-2007
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORMTYPES_H_
#define _PLATFORMTYPES_H_

//#define SIZE_INT_64

#ifdef _WIN32

typedef char                tbyte;      // signed byte
typedef unsigned char       ubyte;      // unsigned char
typedef char                int8t;      // signed 8-bit integer (char)
typedef unsigned char       uint8t;     // unsigned 8-bit integer (unsigned char)
typedef short               int16t;     // signed 16-bit integer
typedef unsigned short      uint16t;    // unsigned 16-bit integer
typedef int                 int32t;     // signed 32-bit integer
typedef unsigned int        uint32t;    // signed 32-bit integer

typedef unsigned short      ushort_t;
typedef unsigned int        uint_t;
typedef int                 int_t;

typedef unsigned short      ushort;
typedef unsigned int        uint;


typedef __int64             int64t;     // signed 64-bit integer
typedef unsigned __int64    uint64t;    // unsigned 64-bit integer



//typedef BOOL boolt;
typedef int boolt;
#define b_false  false;
#define b_true   true;

#ifdef SIZE_INT_64
typedef __int64             isize_t;
typedef unsigned __int64    uisize_t;
#else
typedef int                 isize_t;
typedef unsigned int        uisize_t;
#endif //SIZE_INT_64
/*
#ifdef BOOL 
typedef BOOL boolt;
#else 
typedef bool boolt;
#endif
*/


//#define FORCEINLINE __forceinline
//#define INLINE __inline 

#else //Linux

typedef char                tbyte;      // signed char
typedef unsigned char       ubyte;      // unsigned char
typedef char                int8t;      // signed 8-bit integer (char)
typedef unsigned char       uint8t;     // unsigned 8-bit integer (unsigned char)
typedef short               int16t;     // signed 16-bit integer
typedef unsigned short      uint16t;    // unsigned 16-bit integer
typedef int                 int32t;     // signed 32-bit integer
typedef unsigned int        uint32t;    // signed 32-bit integer

typedef long long           int64t;     // signed 64-bit integer
typedef unsigned long long  uint64t;    // unsigned 64-bit integer

typedef unsigned short      ushort_t;
typedef unsigned int        uint_t;
typedef int                 int_t;

typedef unsigned short      ushort;
typedef unsigned int        uint;

typedef bool boolt;
typedef bool tbool;

#ifdef SIZE_INT_64
typedef __int64             isize_t;
typedef unsigned __int64    uisize_t;
#else
typedef int                 isize_t;
typedef unsigned int        uisize_t;
#endif //SIZE_INT_64

//#define FORCEINLINE   __forceinline
//#define INLINE        __inline

#endif

#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))   //< Creates A FOURCC Code 

#define UINT_MAX_VAL    0xffffffff

#include <stdlib.h>

//ASSERTION
#ifdef _WIN32

#include <assert.h>
#define ASSERT assert

#else
#include <assert.h>
#define ASSERT assert

#endif

enum VARDATATYPE
{
    VTYPE_BYTE,
    VTYPE_UBYTE,
    VTYPE_INT16,
    VTYPE_UINT16,
    VTYPE_INT32,
    VTYPE_UINT32,
    VTYPE_INT64,
    VTYPE_UINT64,
    VTYPE_HALF,
    VTYPE_FLOAT,
    VTYPE_DOUBLE,
};
const uint vartypesizestab[] = {1,1,2,2,4,4,8,8,2,4,8};

#endif //_PLATFORMTYPES_H_