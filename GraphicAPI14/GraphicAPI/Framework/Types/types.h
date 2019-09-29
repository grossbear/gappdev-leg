#ifndef _TYPES_H_
#define _TYPES_H_

typedef int int32;              /**< signed integer */
typedef unsigned int uint32;    /**< unsigned integer */
typedef unsigned int uint;      /**< unsigned integer (same as uint32 on 32-bit platforms) */
typedef unsigned short ushort;  /**< unsigned short */
typedef unsigned char ubyte;    /**< unsigned byte */
typedef unsigned long ulong;		/**< unsigned long */



#ifdef _WIN32
#include <stddef.h>
/** \addtogroup types
    @{
*/
typedef __int64 int64;           /**< signed 64-bit integer */
typedef unsigned __int64 uint64; /**< unsigned 64-bit integer */
typedef intptr_t intptr;         /**< pointer to a signed integer */
typedef uintptr_t uintptr;       /**< pointer to an unsigned integer */
//@}
#else // Linux
typedef long long int64;
typedef unsigned long long uint64;
typedef long intptr;
typedef unsigned long uintptr;
typedef uint32 DWORD;
#endif

#endif