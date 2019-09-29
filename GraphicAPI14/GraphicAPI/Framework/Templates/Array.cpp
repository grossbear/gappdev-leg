#include <memory.h>
#include <stdio.h>
#include "../Types/types.h"
#include "Array.h"

/*
template <class TStruct> 
TArray<TStruct>::TArray()
{
	num=0;	
	pArray=NULL;
	ptr=NULL;
	FreeSlots=NULL;
	pos=0;
}

template <class TStruct>
TArray<TStruct>::~TArray()
{
	delete [] pArray;
	delete [] FreeSlots;
	num=0;
	pos=0;
	ptr=NULL;
}


template <class TStruct>
uint TArray<TStruct>::AddElement(TStruct elem)
{
	uint index;

	if(pArray==NULL)
	{
		num=20;
		pArray = new TStruct[num];
		FreeSlots = new bool[num];

		memset(FreeSlots,0,num*sizeof(bool));
		FreeSlots[0]=true;

		ptr = pArray[0];
		pos = 0;
		memcpy(pArray[0],elem,sizeof(TStruct));

		return 1;
	}
	else
	{
		for(uint i=0; i<num; i++)
		{
			if(!FreeSlots[i])
			{
				FreeSlots[i]=true;
				memcpy(pArray[i],elem,sizeof(TStruct));
				ptr = pArray[0];
				pos=0;

				return i+1;
			}
		}
	}

	//we are still in function
	uint num2 = num+5;
	TStruct *pNewArray = new TStruct[num2];
	bool *pNewSlots = new bool[num2];

	memset(pNewSlots,0,num2*sizeof(bool));

	memcpy(pNewArray,pArray,num*sizeof(TStruct));
	memcpy(pNewSlots,FreeSlots,num*sizeof(TStruct));

	memcpy(pNewArray[num],elem,sizeof(TStruct));

	delete [] pArray;
	delete [] FreeSlots;

	pArray = pNewArray;
	FreeSlots = pNewSlots;
	ptr = pArray[0];
	pos = 0;
	index = num+1;
	num = num2;

	return index;
}


template <class TStruct>
bool TArray<TStruct>::DeleteElement(uint index)
{
	if(index==0 || index>num)
		return false;

	if(!FreeSlots[index-1])
		return false;

	FreeSlots[index-1]=false;
	ptr=pArray[0];
	pos=0;

	return true;
}

template <class TStruct>
TStruct* TArray<TStruct>::GetElement(uint index)
{
	if(index==0 || index>num)
		return NULL;

	if(!FreeSlots[index-1])
		return NULL;

	return pArray[index-1];
}

template <class TStruct>
void TArray<TStruct>::SetPointerToStart()
{
	ptr=pArray[0];
	pos=0;
}

template <class TStruct>
TStruct* TArray<TStruct>::GetElementAndMovePointer()
{
	for(uint i=pos; i<num; i++)
	{
		if(FreeSlots[i])
		{
			pos+=1;
			ptr+=1;

			return pArray[i];
		}
		else
		{
			pos+=1;
			ptr+=1;
		}
	}

	return NULL;
}
*/