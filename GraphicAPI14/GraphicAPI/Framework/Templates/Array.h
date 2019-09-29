

template <class TStruct>

class TArray
{
public:

	TArray();
	~TArray();

	uint AddElement(TStruct &elem);
	bool DeleteElement(uint ind);
	TStruct* GetElement(uint ind);
	void SetPointerToStart();
	TStruct* GetElementAndMovePointer();

protected:

	TStruct* pArray;
	
	bool* UsedSlots;
	uint num;
	TStruct* ptr;
	uint pos;

	TStruct** ppArray;
};

template <class TStruct>
TArray<TStruct>::TArray();

template <class TStruct>
TArray<TStruct>::~TArray();

template <class TStruct>
uint TArray<TStruct>::AddElement(TStruct &elem);

template <class TStruct>
bool TArray<TStruct>::DeleteElement(uint index);

template <class TStruct>
TStruct* TArray<TStruct>::GetElement(uint index);

template <class TStruct>
void TArray<TStruct>::SetPointerToStart();

template <class TStruct>
TStruct* TArray<TStruct>::GetElementAndMovePointer();

template <class TStruct> 
TArray<TStruct>::TArray()
{
	num=0;	
	pArray=NULL;
	ptr=NULL;
	UsedSlots=NULL;
	pos=0;

	ppArray=NULL;
}


template <class TStruct>
TArray<TStruct>::~TArray()
{
	delete [] pArray;
	delete [] UsedSlots;
	num=0;
	pos=0;
	ptr=NULL;

	delete [] ppArray;
}

template <class TStruct>
uint TArray<TStruct>::AddElement(TStruct &elem)
{
	uint index;

	if(pArray==NULL)
	{
		num=3;
		pArray = new TStruct[num];
		UsedSlots = new bool[num];
		ppArray = new TStruct*[num];

		memset(UsedSlots,0,num*sizeof(bool));
		UsedSlots[0]=true;

		ptr = &pArray[0];
		pos = 0;
		memcpy(&pArray[0],&elem,sizeof(TStruct));

		return 1;
	}
	else
	{
		for(uint i=0; i<num; i++)
		{
			if(!UsedSlots[i])
			{
				UsedSlots[i]=true;
				memcpy(&pArray[i],&elem,sizeof(TStruct));
				ptr = &pArray[0];
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
	memcpy(pNewSlots,UsedSlots,num*sizeof(bool));

	memcpy(&pNewArray[num],&elem,sizeof(TStruct));
	pNewSlots[num]=true;

	delete [] pArray;
	delete [] UsedSlots;

	pArray = pNewArray;
	UsedSlots = pNewSlots;
	ptr = &pArray[0];
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

	if(!UsedSlots[index-1])
		return false;

	UsedSlots[index-1]=false;
	ptr=&pArray[0];
	pos=0;

	return true;
}

template <class TStruct>
TStruct* TArray<TStruct>::GetElement(uint index)
{
	if(index==0 || index>num)
		return NULL;

	if(!UsedSlots[index-1])
		return NULL;

	return &pArray[index-1];
}

template <class TStruct>
void TArray<TStruct>::SetPointerToStart()
{
	ptr=&pArray[0];
	pos=0;
}

template <class TStruct>
TStruct* TArray<TStruct>::GetElementAndMovePointer()
{
	while(pos<num)
	{
		if(UsedSlots[pos])
		{
			pos+=1;
			ptr+=1;
	
			return ptr-1;
		}
		else
		{
			pos+=1;
			ptr+=1;
		}
	}

	return NULL;
}

