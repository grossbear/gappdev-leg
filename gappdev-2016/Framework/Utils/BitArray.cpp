/////////////////////////////////////////////////////////////////////////////
// BitArray.cpp
// 
// A One Dimentional Array Of Bits
/////////////////////////////////////////////////////////////////////////////

#include "BitArray.h"

/////////////////////////////////////////////////////////////////////////////
// Constructors
//
CBitArray::CBitArray()
{
    mSingleWord = 0;
    mpStore = &mSingleWord;

    mLength = 1;
    mNumBits = 0;
}
/////////////////////////////////////////////////////////////////////////////
CBitArray::CBitArray(unsigned size)
{
    CBitArray();
    Init(size);
    Trim();
}
/////////////////////////////////////////////////////////////////////////////
CBitArray::CBitArray(const CBitArray& bitarray)
{
    *this = bitarray;
}

/////////////////////////////////////////////////////////////////////////////
// Destructor
CBitArray::~CBitArray()
{
    if(mLength > 1)
        delete mpStore;

    mpStore = NULL;
    mSingleWord = 0;

    mLength = 0;
    mNumBits = 0;
}

/////////////////////////////////////////////////////////////////////////////
unsigned CBitArray::Size() const
{
    return mNumBits;
}
/////////////////////////////////////////////////////////////////////////////
void CBitArray::Clear()
{
    memset(mpStore,0,mLength * sizeof(store_type));
}
/////////////////////////////////////////////////////////////////////////////
void CBitArray::AddElems(unsigned elems)
{
    store_type *prevStore = mpStore;

    unsigned newLen = 1 + GetIndex(mNumBits + elems - 1);
    if(newLen > mLength)
    {
        store_type *newStore = new store_type[newLen];
        memset(&newStore[mLength],0,sizeof(store_type)*(newLen-mLength));
        memcpy(newStore,mpStore,sizeof(store_type)*mLength);
        mpStore = newStore;
        delete [] prevStore;
    }
    else
        mNumBits += elems;
}
/////////////////////////////////////////////////////////////////////////////
//Clear All Data
void CBitArray::Resize(unsigned newSize)
{
    if(mLength > 1)
        delete mpStore;
        
    Init(newSize);
    Trim();
}

/////////////////////////////////////////////////////////////////////////////

//
// Unary Oprators
//
bool CBitArray::operator [] (unsigned pos)
{
    assert(pos < mNumBits);
    return IsBitSet(pos);
}
/////////////////////////////////////////////////////////////////////////////
const bool CBitArray::operator [] (unsigned pos) const
{
    assert(pos < mNumBits);
    return IsBitSet(pos);
}

// Change Value Of All Bits
CBitArray CBitArray::operator ~()
{
    for (unsigned i=0; i < mLength; i++)
        mpStore[i] = ~mpStore[i];

    Trim();
    return *this;
}

//
// Assignment Operators
//
CBitArray& CBitArray::operator = (const CBitArray &bitarray)
{
    if(this != &bitarray)
    {
        if (bitarray.mLength > mLength)
        {
            if(mLength > 1)
                delete mpStore;
            
            Init(bitarray.mNumBits);
        }
           
        memcpy(mpStore,bitarray.mpStore, sizeof(store_type)*mLength);
    }

    return *this;
}

/////////////////////////////////////////////////////////////////////////////
CBitArray& CBitArray::operator |= (const CBitArray& bitarray)
{
    assert(mNumBits == bitarray.mNumBits);
    for(unsigned i = 0; i < mLength; i++)
    {
        mpStore[i] |= bitarray.mpStore[i];
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////
CBitArray& CBitArray::operator &= (const CBitArray& bitarray)
{
    assert(mNumBits == bitarray.mNumBits);
    for(unsigned i = 0; i < mLength; i++)
    {
        mpStore[i] &= bitarray.mpStore[i];
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////
CBitArray &CBitArray::operator ^= (const CBitArray& bitarray)
{
    assert(mNumBits == bitarray.mNumBits);
    for(unsigned i = 0; i < mLength; i++)
    {
        mpStore[i] ^= bitarray.mpStore[i];
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Binary Operator

CBitArray CBitArray::operator | (const CBitArray& bitarray) const
{
    return CBitArray(*this) |= bitarray;
}
/////////////////////////////////////////////////////////////////////////////
CBitArray CBitArray::operator & (const CBitArray& bitarray) const
{
    return CBitArray(*this) &= bitarray;
}
/////////////////////////////////////////////////////////////////////////////
CBitArray CBitArray::operator ^ (const CBitArray& bitarray) const
{
    return CBitArray(*this) ^= bitarray;
}

/////////////////////////////////////////////////////////////////////////////
bool CBitArray::operator == (const CBitArray& bitarray) const
{
    if(mNumBits != bitarray.mNumBits)
        return false;

    for(unsigned i = 0; i < mLength; i++)
    {
        if(mpStore[i] != bitarray.mpStore[i])
            return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CBitArray::operator != (const CBitArray& bitarray) const
{
    return !(*this == bitarray);
}

/////////////////////////////////////////////////////////////////////////////
void CBitArray::Init(unsigned size)
{
    mNumBits = size;
    unsigned int prevLen = mLength;

    if(size == 0)
        mLength = 0;
    else
        mLength = 1 + GetIndex(size - 1);

    store_type *pStore;
    if(mLength <= 1)
        pStore = &mSingleWord;
    else
        pStore = new store_type[mLength];

    memset(pStore,0,mLength*sizeof(store_type));
    if(prevLen > 1)
        memcpy(pStore,mpStore,prevLen*sizeof(store_type));

    if(mpStore != &mSingleWord) //if(mpStore != pStore)
        delete mpStore;

    mpStore = pStore;

    /*
    // Avoid allocation if length is 1 (common case)
    if (mLength <= 1)
        mpStore = &mSingleWord;      
    else
        mpStore = new store_type[mLength];

    if (mLength > 1)
        memset(mpStore,0,mLength * sizeof(store_type));
        */
}


// Returns true if the bit at position pos is true.
bool CBitArray::IsBitSet(unsigned pos) const
{
    assert(pos < mNumBits);
    return (mpStore[GetIndex(pos)] & (1 << GetOffset(pos))) != 0;
}

// Force overhang bits at the end to 0
void CBitArray::Trim()
{
  unsigned extra_bits = mNumBits % cell_size;
  if (mLength > 0 && extra_bits != 0)
     mpStore[mLength - 1] &= ~((~(store_type) 0) << extra_bits);
}

// Set The Bit At Position Pos To True
void CBitArray::SetBit(unsigned pos)
{
    assert(pos < mNumBits);
    mpStore[GetIndex(pos)] |= 1 << GetOffset(pos);
}

// Set The Bit At Position Pos To False
void CBitArray::ClearBit(unsigned pos)
{
    assert(pos < mNumBits);
    mpStore[GetIndex(pos)] &= ~(1 << GetOffset(pos));
}

// Toggle The Bit At Position Pos
void CBitArray::FlipBit(unsigned pos)
{
    assert(pos < mNumBits);
    mpStore[GetIndex(pos)] ^= 1 << GetOffset(pos); 
}

// Set The Bit At Position Pos To The Given Value.
void CBitArray::Set(unsigned pos, bool val)
{
    val ? SetBit(pos) : ClearBit(pos);
}