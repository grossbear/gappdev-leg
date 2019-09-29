////////////////////////////////////////////////////////////////////////////////////////////////
//	3DObjDesc.cpp
//	3DObjectDescriptor Class
//	Created: 26th May 2006
//  Modified: 30th August 2006 Shader Description Added
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Types/types.h"
#include "../Image/Image.h"
#include "../Templates/Array.h"
#include "TextureMgr.h"
#include "3DObjDesc.h"

#include <memory.h>
#include <string.h>

//Shader Descriptor
//Destructor
CGPUProgramDesc::~CGPUProgramDesc()
{
	tagShaderUniformDescriptor *tmpPtr;
	while(ShaderDescPtr!=NULL)
	{
		tmpPtr = ShaderDescPtr->next;
		delete [] ShaderDescPtr->name;

		if(ShaderDescPtr->UniformType >= LIGHT_POSITION && 
			ShaderDescPtr->UniformType <= TEXTURE_UNIT)
			delete [] (uint*) ShaderDescPtr->param;
		else if(ShaderDescPtr->UniformType >= USER_MATRIX4X4 &&
			USER_MATRIX4X4 <= USER_FLOAT)
			delete [] (float*) ShaderDescPtr->param;
		else if(ShaderDescPtr->UniformType == USER_INT)
			delete [] (int*) ShaderDescPtr->param;
		else if(ShaderDescPtr->UniformType == USER_BOOL)
			delete [] (bool*) ShaderDescPtr->param;

		delete (ShaderDescPtr);
		ShaderDescPtr = tmpPtr;
	}

	ShaderDescPtr = NULL;
	ShaderDescPtr2 = NULL;
}

void CGPUProgramDesc::Release()
{
	tagShaderUniformDescriptor *tmpPtr;
	while(ShaderDescPtr!=NULL)
	{
		tmpPtr = ShaderDescPtr->next;
		delete [] ShaderDescPtr->name;

		if(ShaderDescPtr->UniformType >= LIGHT_POSITION && 
			ShaderDescPtr->UniformType <= TEXTURE_UNIT)
			delete [] (uint*) ShaderDescPtr->param;
		else if(ShaderDescPtr->UniformType >= USER_MATRIX4X4 &&
			USER_MATRIX4X4 <= USER_FLOAT)
			delete [] (float*) ShaderDescPtr->param;
		else if(ShaderDescPtr->UniformType == USER_INT)
			delete [] (int*) ShaderDescPtr->param;
		else if(ShaderDescPtr->UniformType == USER_BOOL)
			delete [] (bool*) ShaderDescPtr->param;

		delete (ShaderDescPtr);
		ShaderDescPtr = tmpPtr;
	}

	ShaderDescPtr = NULL;
	ShaderDescPtr2 = NULL;
}


//Adding New Uniform Descriptor
bool CGPUProgramDesc::AddShaderUniformDesc(SHADERUNIFORMTYPE UniformType, char *UniformName, 
										   void *data)
{
	ShaderUniformDescriptor *UniDesc = new ShaderUniformDescriptor[1];
	memset(UniDesc,0,sizeof(ShaderUniformDescriptor));

	if(!UniformName)
		return false;

	//copy uniform name string
	uint len = uint(strlen(UniformName));
	UniDesc->name = new char[len+1];
	memcpy(UniDesc->name,UniformName,len);
	UniDesc->name[len] = '\0';

	//copy uniform type
	UniDesc->UniformType = UniformType;
	UniDesc->param = NULL;

	if(UniformType >= WORLD_VIEW_PROJECTION_MATRIX && UniformType <= WORLD_VIEW_MATRIX_INV)
		UniDesc->param = NULL;
	else if(UniformType >= LIGHT_POSITION && UniformType <= TEXTURE_UNIT)
	{
		UniDesc->param = new uint[1];
		memcpy(UniDesc->param,data,sizeof(uint));
	}
	else
	{
		switch(UniformType)
		{
		case USER_MATRIX4X4:
			UniDesc->param = new float[16];
			memcpy(UniDesc->param,data,sizeof(float)*16);
			break;

		case USER_MATRIX3X3:
			UniDesc->param = new float[9];
			memcpy(UniDesc->param,data,sizeof(float)*9);
			break;

		case USER_VECTOR4:
			UniDesc->param = new float[4];
			memcpy(UniDesc->param,data,sizeof(float)*4);
			break;

		case USER_VECTOR3:
			UniDesc->param = new float[3];
			memcpy(UniDesc->param,data,sizeof(float)*3);
			break;

		case USER_VECTOR2:
			UniDesc->param = new float[2];
			memcpy(UniDesc->param,data,sizeof(float)*2);
			break;

		case USER_FLOAT:
			UniDesc->param = new float[1];
			memcpy(UniDesc->param,data,sizeof(float));
			break;

		case USER_INT:
			UniDesc->param = new int[1];
			memcpy(UniDesc->param,data,sizeof(int));
			break;

		case USER_BOOL:
			UniDesc->param = new bool[1];
			memcpy(UniDesc->param,data,sizeof(bool));
			break;
		}
	}
	//Adding element to list
	if(ShaderDescPtr==NULL)
	{
		ShaderDescPtr = UniDesc;
		ShaderDescPtr2 = UniDesc;
	}
	else
	{
		ShaderDescPtr2->next = UniDesc;
		ShaderDescPtr2 = ShaderDescPtr2->next;
		ShaderDescPtr2->next = NULL;
	}

		return true;
}

//Getting Param Data From Specified Uniform Name
void* CGPUProgramDesc::GetDataPtrByName(char *UniformName)
{
	ShaderUniformDescriptor *UniDesc = ShaderDescPtr;

	while(UniDesc!=NULL)
	{	
		if(strlen(UniDesc->name)==strlen(UniformName))
		{
			bool ok = true;
			int len = (int)strlen(UniformName);
			char *ptr1 = UniDesc->name;
			char *ptr2 = UniformName;
			for(int i=0; i<len; i++)
				if(*ptr1++ != *ptr2++)
				{ok = false; break;}

			if(ok)
				return UniDesc->param;
		}
		UniDesc = UniDesc->next;
	}
	
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Default Constructor
C3DObjectDesc::C3DObjectDesc()
{
	MatID = 0;
	TexID = NULL;
	TexUnits=0;
	TexAddr = NULL;
	GPUProgram = 0;
	pShaderDesc = new CGPUProgramDesc();
}

// Copy Constructor
C3DObjectDesc::C3DObjectDesc(C3DObjectDesc *copy)
{
	//C3DObjectDesc();
	MatID = 0;
	TexID = NULL;
	TexUnits=0;
	TexAddr = NULL;
	GPUProgram = 0;
	pShaderDesc = new CGPUProgramDesc();

	if(!copy)
		return ;

	this->CopyData(copy);
}

// Destructor
C3DObjectDesc::~C3DObjectDesc()
{
	if(TexID)
	{
		delete [] TexID;
		TexID = NULL;
	}

	if(TexAddr)
	{
		delete [] TexAddr;
		TexAddr = NULL;
	}

	if(pShaderDesc)
	{
		delete (pShaderDesc);
		pShaderDesc = NULL;
	}

	MatID = 0;
	TexUnits = 0;
	GPUProgram = 0;
}

// Copy Data From Another 3DObject Descriptor
// Function Doesn't Copy Shader Description Info
bool C3DObjectDesc::CopyData(C3DObjectDesc *copy)
{
	if(!copy)
		return false;

	if(TexID!=NULL)
		delete [] TexID;
	TexID=NULL;

	if(TexAddr)
		delete [] TexAddr;
	TexAddr=NULL;

	
	MatID = copy->MatID;
	TexUnits = copy->TexUnits;
	GPUProgram = copy->GPUProgram;

	if(TexUnits)
	{
		TexID = new uint [TexUnits];
		memcpy(TexID,copy->TexID,sizeof(uint)*TexUnits);
	}
	if(!TexAddr)
	{
		TexAddr = new TexAddrStruct[TexUnits];
		memcpy(TexAddr,copy->TexAddr, sizeof(TexAddrStruct)*TexUnits);
	}

	
	////if(pShaderDesc)
		delete (pShaderDesc);

	pShaderDesc = new CGPUProgramDesc();
	ShaderUniformDescriptor *DescPtr;
	DescPtr = copy->pShaderDesc->GetShaderDescList();

	while(DescPtr!=NULL)
	{
		pShaderDesc->AddShaderUniformDesc(DescPtr->UniformType, DescPtr->name, 
			DescPtr->param);

		DescPtr = DescPtr->next;
	}

	return true;
}

void C3DObjectDesc::SetTexID(uint Ind, uint TexNum)
{
	if(Ind>=TexUnits)
		return ;

	if(!TexID)
		return ;

	TexID[Ind] = TexNum;
}

void C3DObjectDesc::SetTexAddr(uint Ind, TEXADDR U, TEXADDR V, TEXADDR W)
{
	if(Ind>=TexUnits)
		return ;

	if(!TexAddr)
		return ;

	TexAddr[Ind].U = U;
	TexAddr[Ind].V = V;
	TexAddr[Ind].W = W;
}

void C3DObjectDesc::SetTexAddrU(uint Ind, TEXADDR U)
{
	if(Ind>=TexUnits)
		return ;

	if(!TexAddr)
		return ;

	TexAddr[Ind].U = U;
}

void C3DObjectDesc::SetTexAddrV(uint Ind, TEXADDR V)
{
	if(Ind>=TexUnits)
		return ;

	if(!TexAddr)
		return ;

	TexAddr[Ind].V = V;
}

void C3DObjectDesc::SetTexAddrW(uint Ind, TEXADDR W)
{
	if(Ind >= TexUnits)
		return ;

	if(!TexAddr)
		return ;

	TexAddr[Ind].W = W;
}

// Set New Available Texture Units In 3DObject Descriptor
// Delete TexID And TexAddr Arrays
// And All Data Will Be Lost
void C3DObjectDesc::SetTexUnits(uint NewTexUnits)
{
	if(!NewTexUnits)
		return ;

	if(TexID)
		delete [] TexID;

	if(TexAddr)
		delete [] TexAddr;

	TexUnits = NewTexUnits;

	TexID = new uint [TexUnits];
	TexAddr = new TexAddrStruct[TexUnits];

	memset(TexID,0,sizeof(uint)*TexUnits);
	memset(TexAddr,0,sizeof(TexAddrStruct)*TexUnits);
	SetTexAddrStructArray();
}

// Get Texture ID From Specified Index Of The Textures IDs Array
uint C3DObjectDesc::GetTextureID(uint Index)
{
	if(Index >= TexUnits)
		return 0;

	return TexID[Index];
}

// Get Texture Address Type
TexAddrStruct C3DObjectDesc::GetTextureAddress(uint Index)
{
	TexAddrStruct TAddr;
	TAddr.U = TEXADDR_REPEAT;
	TAddr.V = TEXADDR_REPEAT;
	TAddr.W = TEXADDR_NONE;

	if(Index >= TexUnits)
		return TAddr;

	return TexAddr[Index];
}

// Set Default Values For Texture Address Array
void C3DObjectDesc::SetTexAddrStructArray()
{
	for(uint i=0; i<TexUnits; i++)
	{
		TexAddr[i].U = TexAddr[i].V = TEXADDR_REPEAT;
		TexAddr[i].W = TEXADDR_NONE;
	}
}