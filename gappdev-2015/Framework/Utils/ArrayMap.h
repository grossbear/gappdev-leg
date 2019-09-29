/////////////////////////////////////////////////////////////////////////////
//  ArrayMap.h
//
//  ArrayMap Template Class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CARRAYMAP_H__
#define __CARRAYMAP_H__

#include <bitset>
#include <map>
#include <memory.h>

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
class CArrayMap
{
protected:
    

public:
    CArrayMap();
    ~CArrayMap();

    unsigned Reserve(unsigned num);
    T* GetItem(unsigned index);
    bool Release(unsigned index, unsigned count=1);
    unsigned Size();
    void Clear();

protected:
    typedef struct ArrayBlock
    {
        T elem_array[N];
        std::bitset<N> bitset_array;
    };

    void AddNewArrayBlock();
    void ReserveItems(ArrayBlock &block, unsigned pos, unsigned count);

    std::map<unsigned,ArrayBlock> mArrayMap;
    unsigned mMaxElem;
    unsigned mKeyOff;
};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
CArrayMap<T,N>::CArrayMap()
{
    //mKeyOff = N - 1;
    //mKeyOff = mKeyOff | mKeyOff >> 1;
    //mKeyOff = mKeyOff | mKeyOff >> 2;
    //mKeyOff = mKeyOff | mKeyOff >> 4;
    //mKeyOff = mKeyOff | mKeyOff >> 8;
    //mKeyOff = mKeyOff | mKeyOff >> 16;
    //mKeyOff = mKeyOff + 1;
    
    unsigned max_count = (1<<16)-1;
    mMaxElem = (N > max_count) ? (max_count) : (N);
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
CArrayMap<T,N>::~CArrayMap()
{
}


/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
unsigned CArrayMap<T,N>::Reserve(unsigned num)
{
    if(num > mMaxElem)
        return 0xffffffff;

    // Search String Pattern
    std::string bitstr;
    bitstr.assign((size_t)num,'0');
    // Searching Through Arrays For Free Elements
    std::map<size_t,ArrayBlock>::iterator it = mArrayMap.begin();
    for( ; it != mArrayMap.end(); it++)
    {
        ArrayBlock &block = it->second;
        unsigned key = it->first;
        unsigned count = block.bitset_array.count();
        if(count > mMaxElem-num)
            continue;
        
        std::string strblock = block.bitset_array.to_string();
        size_t pos = strblock.rfind(bitstr);
        if(pos == std::string::npos)
            continue;

        pos = strblock.size() - pos - 1;
        ReserveItems(block,(unsigned)pos,num);
        return ((key<<16) | ((unsigned)pos))+1;

    }

    if(mArrayMap.size() == 0xfffe)
        return 0;
    AddNewArrayBlock();
    std::map<size_t,ArrayBlock>::reverse_iterator revit = mArrayMap.rbegin();
    unsigned key = revit->first;
    ArrayBlock &block = revit->second;
    ReserveItems(block,0,num);

    return (key<<16)+1;
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
T* CArrayMap<T,N>::GetItem(unsigned index)
{
    index -= 1;
    unsigned key = index >> 16;
    if(!mArrayMap.count(key))
        return NULL;

    unsigned idx = index & 0xffff;
    if(idx >= mMaxElem)
        return NULL;

    ArrayBlock &block = mArrayMap[key];

    return &block.elem_array[idx];
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
bool CArrayMap<T,N>::Release(unsigned index, unsigned count)
{
    index -= 1;
    unsigned key = index >> 16;
    if(!mArrayMap.count(key))
        return false;

    unsigned idx = index & 0xffff;
    if(idx >= mMaxElem)
        return false;

    ArrayBlock &block = mArrayMap[key];

    for(unsigned i = 0; i < count; i++)
        block.bitset_array.set(idx+i,0);

    return true;
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
unsigned CArrayMap<T,N>::Size()
{
    unsigned size = (unsigned)mArrayMap.size()*mMaxElem;
    return size;
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
void CArrayMap<T,N>::Clear()
{
    mArrayMap.clear();
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
void CArrayMap<T,N>::AddNewArrayBlock()
{
    unsigned new_elem_idx = (unsigned)mArrayMap.size() + 1;
    mArrayMap[new_elem_idx] = CArrayMap<T,N>::ArrayBlock();
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
void CArrayMap<T,N>::ReserveItems(ArrayBlock &block, unsigned pos, unsigned count)
{
    memset(&block.elem_array[pos],0,sizeof(T)*count);
    for(unsigned i = 0; i < count; i++)
        block.bitset_array.set(pos+i);
}
/////////////////////////////////////////////////////////////////////////////

#endif //__CARRAYMAP_H__