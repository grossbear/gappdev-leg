/////////////////////////////////////////////////////////////////////////////
// BitArray.h
// 
// A One Dimentional Array Of Bits
/////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _BITARRAY_H_
#define _BITARRAY_H_


#include <assert.h>
#include <memory.h>
#include <stdio.h>

class CBitArray
{
public:
    CBitArray();
    CBitArray(unsigned size);
    CBitArray(const CBitArray& bitarray);

    ~CBitArray();

    unsigned Size() const;
    void Clear();
    void AddElems(unsigned elem);
    void Resize(unsigned newSize);

    void SetBit(unsigned pos);
    void ClearBit(unsigned pos);
    void FlipBit(unsigned pos);

    // Assignment Operators
    CBitArray &operator =  (const CBitArray& bitarray);
    CBitArray &operator |= (const CBitArray& bitarray);
    CBitArray &operator &= (const CBitArray& bitarray);
    CBitArray &operator ^= (const CBitArray& bitarray);

    // Unary Oprators
    bool        operator [] (unsigned pos);
    const bool  operator [] (unsigned pos) const;
    CBitArray operator ~ ();


    // Binary Operators
    CBitArray operator | (const CBitArray& bitarray) const;
    CBitArray operator & (const CBitArray& bitarray) const;
    CBitArray operator ^ (const CBitArray& bitarray) const;

    bool operator == (const CBitArray& bitarray) const;
    bool operator != (const CBitArray& bitarray) const;

private:

	typedef unsigned long store_type;
	enum
	{
	    bits_per_byte = 8,
	    cell_size     = sizeof(store_type) * bits_per_byte
	};

    unsigned mLength;
    unsigned mNumBits;

    store_type *mpStore;
    store_type mSingleWord;

    void Init(unsigned size);

    // Get the index and bit offset for a given bit number.
    static unsigned GetIndex(unsigned bit_num)
    {
      return bit_num / cell_size;
    }

    static unsigned GetOffset(unsigned bit_num)
    {
      return bit_num % cell_size;
    }

    inline bool IsBitSet(unsigned pos) const;
    inline void Trim();

    inline void Set(unsigned pos, bool val);
};

#endif //_BITARRAY_H_