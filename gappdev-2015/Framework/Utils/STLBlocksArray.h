/////////////////////////////////////////////////////////////////////////////
//  STLBlocksArray
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _STLBLOCKSARRAY_H_
#define _STLBLOCKSARRAY_H_

#include <stdio.h>
#include <vector>

#include "BitArray.h"

#define MAX_BLOCKS 8

template <class TElem, unsigned ElemNum>
class CSTLBlocksArray
{
public:
    CSTLBlocksArray();
    virtual ~CSTLBlocksArray();

    unsigned Reserve(unsigned num);
    bool Release(unsigned idx, unsigned num);

    TElem* operator [] (unsigned );

    unsigned Size();
    TElem* GetIFReserved(unsigned idx);

protected:
    typedef struct BlockStruct
    {
        TElem       ElemArray[ElemNum];
        CBitArray   ReservedElems;
        unsigned    FreeElemsNum;
    };

    std::vector<BlockStruct*> m_BlocksPtrVector;

    void InitBlock(BlockStruct* pBlock);
    void ReleaseBlocks();
};

/////////////////////////////////////////////////////////////////////////////
//Constructor
template <class TElem, unsigned ElemNum>
CSTLBlocksArray<TElem,ElemNum>::CSTLBlocksArray()
{
    BlockStruct* pblock = new BlockStruct;
    InitBlock(pblock);
    
    m_BlocksPtrVector.push_back(pblock);
}

/////////////////////////////////////////////////////////////////////////////
//Destructor
template <class TElem, unsigned ElemNum>
CSTLBlocksArray<TElem,ElemNum>::~CSTLBlocksArray()
{
    ReleaseBlocks();
}


/////////////////////////////////////////////////////////////////////////////
//Reserving Elements In Block
template <class TElem, unsigned ElemNum>
unsigned CSTLBlocksArray<TElem,ElemNum>::Reserve(unsigned num)
{
    if(num > ElemNum)
        return 0;

    if(num == 0)
        return 0;

    std::vector<BlockStruct*>::iterator iter = m_BlocksPtrVector.begin();
    unsigned iterIdx = 0;
    //Searching In Existing Blocks
    for(; iter < m_BlocksPtrVector.end(); iter++)
    {
        unsigned sum = 0;
        unsigned start_idx = 0;
        if((*iter)->FreeElemsNum >= num)
        {
            for(unsigned i = 0; i < ElemNum; i++)
            {
                if(!(*iter)->ReservedElems[i])
                {
                    if(sum == 0)
                        start_idx = i;
                    sum += 1;
                }
                else
                {
                    sum = 0;
                    start_idx = 0;
                }

                if(sum == num)
                    break;
            }
            if(sum < num)
                sum = 0;
        }

        if(sum == num)
        {
            unsigned end_idx = start_idx + num;
            for(unsigned i = start_idx; i < end_idx; i++)
                (*iter)->ReservedElems.SetBit(i);

            (*iter)->FreeElemsNum = (*iter)->FreeElemsNum - num;

            return iterIdx*ElemNum + start_idx + 1;
        }
        iterIdx++;
    }

    //Adding New Block And Reserving Elements
    BlockStruct* pblock = new BlockStruct;
    InitBlock(pblock);

    for(unsigned i = 0; i < num; i++)
        pblock->ReservedElems.SetBit(i);
    pblock->FreeElemsNum -= num;

    m_BlocksPtrVector.push_back(pblock);
    
    return iterIdx*ElemNum + 1;
}

/////////////////////////////////////////////////////////////////////////////
//Releasing Reserved Elements In Block
template <class TElem, unsigned ElemNum>
bool CSTLBlocksArray<TElem,ElemNum>::Release(unsigned idx, unsigned num)
{
    if(idx == 0 || num > ElemNum)
        return false;

    unsigned size = m_BlocksPtrVector.size();

    unsigned blockIdx = idx/ElemNum;
    unsigned elemIdx = idx%ElemNum - 1;

    if(((elemIdx + num) > ElemNum) || blockIdx >= size)
        return false;

    BlockStruct *pblock = m_BlocksPtrVector[blockIdx];
    pblock->FreeElemsNum += num;

    unsigned endIdx = elemIdx + num;
    for(unsigned i = elemIdx; i < endIdx;  i++)
        pblock->ReservedElems.ClearBit(i);

    return true;
}


/////////////////////////////////////////////////////////////////////////////
template <class TElem, unsigned ElemNum>
TElem* CSTLBlocksArray<TElem,ElemNum>::operator [] (unsigned idx)
{
    if(idx == 0)
        return NULL;

    unsigned size = (unsigned)m_BlocksPtrVector.size();

    unsigned blockIdx = (idx-1)/ElemNum;
    unsigned elemIdx = (idx-1)%ElemNum;

    if(blockIdx >= size)
        return NULL;

    BlockStruct *pblock = m_BlocksPtrVector[blockIdx];

    return &pblock->ElemArray[elemIdx];
}

/////////////////////////////////////////////////////////////////////////////
template <class TElem, unsigned ElemNum>
unsigned CSTLBlocksArray<TElem,ElemNum>::Size()
{
    return m_BlocksPtrVector.size()*ElemNum;
}
   
/////////////////////////////////////////////////////////////////////////////
template <class TElem, unsigned ElemNum>
TElem* CSTLBlocksArray<TElem,ElemNum>::GetIFReserved(unsigned idx)
{
    if(idx == 0)
        return NULL;

    unsigned size = m_BlocksPtrVector.size();

    unsigned blockIdx = (idx-1)/ElemNum;
    unsigned elemIdx = (idx-1)%ElemNum;

    if(blockIdx >= size)
        return NULL;

    BlockStruct *pblock = m_BlocksPtrVector[blockIdx];
    if(pblock->ReservedElems[elemIdx]) return &pblock->ElemArray[elemIdx];

    return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Protected Methods

/////////////////////////////////////////////////////////////////////////////
// Initializing New Block With Default Values
template <class TElem, unsigned ElemNum>
void CSTLBlocksArray<TElem,ElemNum>::InitBlock(BlockStruct* pBlock)
{
    pBlock->FreeElemsNum = ElemNum;
    pBlock->ReservedElems.Resize(ElemNum);
}

/////////////////////////////////////////////////////////////////////////////
// Releasing All Blocks And Data In Blocks
template <class TElem, unsigned ElemNum>
void CSTLBlocksArray<TElem,ElemNum>::ReleaseBlocks()
{
    std::vector<BlockStruct*>::iterator iter = m_BlocksPtrVector.begin();
    for(; iter < m_BlocksPtrVector.end(); iter++)
    {
        delete (*iter);
    }
    m_BlocksPtrVector.clear();
}

/////////////////////////////////////////////////////////////////////////////

#endif //_STLBLOCKSARRAY_H_