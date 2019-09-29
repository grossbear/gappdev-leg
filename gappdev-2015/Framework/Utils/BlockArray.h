/////////////////////////////////////////////////////////////////////////////
//  BlockArray.h
//
//  BlockArray Template Class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CBLOCKARRAY_H__
#define __CBLOCKARRAY_H__

#include <vector>
#include <string>

#define INVALID_ARRAY_INDEX    0xffffffff

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
class CBlockArray 
{
public:
    CBlockArray();
    ~CBlockArray();

    unsigned Reserve(unsigned n);
    T* GetItem(unsigned index);
    bool Release(unsigned index, unsigned count=1);
    unsigned Size();
    void Clear();
    void Start();
    T* NextItem();

    T* operator [] (unsigned index);

protected:
    typedef struct TBlock
    {
        T elem_array[N];
        std::string empty_items;
        unsigned empty_count;
    };

    void AddNewBlock();

    std::vector<TBlock> m_BlocksArray;
    unsigned m_MaxBlocksCount;
    unsigned m_BlockIdxOff;
    unsigned m_UsedIdx;
};

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
CBlockArray<T,N>::CBlockArray()
{
    unsigned bitoff = 1;
    unsigned n = N-1;
    while(n=n>>1) {
        bitoff++;
    }
    m_BlockIdxOff = bitoff;
    m_MaxBlocksCount = (1<<(31-bitoff));

    m_BlocksArray.reserve(1);
    AddNewBlock();
}
/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
CBlockArray<T,N>::~CBlockArray()
{
    m_BlocksArray.clear();
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
unsigned CBlockArray<T,N>::Reserve(unsigned n)
{
    size_t str_pos = std::string::npos;
    size_t idx = 0;

    if(n > N) return 0xffffffff;

    size_t size = m_BlocksArray.size();
    for(idx = 0; idx < size; idx++)
    {
        TBlock &block = m_BlocksArray[idx];
        if(block.empty_count < n) continue;

        std::string str_zero(n,'0');
        str_pos = block.empty_items.find_first_of(str_zero);
        if(str_pos == std::string::npos) continue;
   
        std::string str_one(n,'1');
        block.empty_items.replace(str_pos,n,str_one);
        block.empty_count -= n;
        break;
    }

    if(m_MaxBlocksCount == (size+1)) return 0xffffffff;

    if(str_pos == std::string::npos)
    {
        AddNewBlock();
        idx = size;
        str_pos = 0;
    }

    unsigned index = (unsigned)(idx<<m_BlockIdxOff | str_pos);

    return index;
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
T* CBlockArray<T,N>::GetItem(unsigned index)
{
    unsigned elem_mask = ~(0xffffffff<<m_BlockIdxOff);
    unsigned elem_pos = index & elem_mask;
    unsigned block_idx = (index >> m_BlockIdxOff);

    size_t size = m_BlocksArray.size();
    if(block_idx >= size) return NULL;

    TBlock &block = m_BlocksArray[block_idx];
    return &block.elem_array[elem_pos];
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
bool CBlockArray<T,N>::Release(unsigned index, unsigned count)
{
    unsigned elem_mask = ~(0xffffffff<<m_BlockIdxOff);
    unsigned elem_pos = index & elem_mask;
    unsigned block_idx = (index >> m_BlockIdxOff);
    size_t size = m_BlocksArray.size();
    if(block_idx >= size) return false;

    TBlock &block = m_BlocksArray[block_idx];
    std::string str_one(count,'1');
    int n = block.empty_items.compare(elem_pos,count,str_one);
    if(!n) return false;
    std::string str_zero(count,'0');
    block.empty_items.replace(elem_pos,count,str_zero);
    block.empty_count += count;

    return true;
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
unsigned CBlockArray<T,N>::Size()
{
    return N*(unsigned)(m_BlocksArray.size());
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
void CBlockArray<T,N>::Clear()
{
    m_BlocksArray.clear();
    AddNewBlock();
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
void CBlockArray<T,N>::Start()
{
    m_UsedIdx = 0xffffffff;
    size_t size = m_BlocksArray.size();
    for(size_t idx = 0; idx < size; idx++) {
        TBlock &block = m_BlocksArray[idx];
        size_t str_pos = block.empty_items.find_first_of('1');
        if(str_pos != std::string::npos) {
            m_UsedIdx = (unsigned)(idx<<m_BlockIdxOff | str_pos);
            break;
        }
    }

    //unsigned index = (unsigned)(idx<<m_BlockIdxOff | str_pos);
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
T* CBlockArray<T,N>::NextItem()
{
    if(m_UsedIdx == 0xffffffff) return NULL;

    unsigned elem_mask = ~(0xffffffff<<m_BlockIdxOff);
    unsigned elem_pos = m_UsedIdx & elem_mask;
    unsigned block_idx = (m_UsedIdx >> m_BlockIdxOff);

    TBlock &block = m_BlocksArray[block_idx];
    T &elem = block.elem_array[elem_pos];
    
    elem_pos += 1;
    unsigned idx;
    for(idx = block_idx; idx < m_BlocksArray.size(); idx++)
    {
        TBlock &block = m_BlocksArray[idx];
        elem_pos = block.empty_items.find('1',elem_pos);
        if(elem_pos != std::string::npos) break;
        elem_pos = 0;
    }
    
    m_UsedIdx = 0xffffffff;
    if(idx < m_BlocksArray.size()) {
        m_UsedIdx = (idx<<m_BlockIdxOff | elem_pos);
    }

    return &elem;
}

/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
T* CBlockArray<T,N>::operator [] (unsigned index)
{
    return GetItem(index);
}
/////////////////////////////////////////////////////////////////////////////
template <class T, unsigned N>
void CBlockArray<T,N>::AddNewBlock()
{
    m_BlocksArray.resize(m_BlocksArray.size()+1);
    TBlock &block = m_BlocksArray.back();
    block.empty_items.append(N,'0');
    block.empty_count = N;
}
/////////////////////////////////////////////////////////////////////////////

#endif //__CBLOCKARRAY_H__