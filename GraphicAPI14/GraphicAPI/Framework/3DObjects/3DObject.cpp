////////////////////////////////////////////////////////////////////////////////////////////////
//	3DObject.cpp 
//	3DObject class functions definitions
//	Created: 16th February 2006
//	Modified: 27th May 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <memory.h>
#include "../Math/MathLib.h"
#include "../Types/types.h"
#include "3DObject.h"

//Default 3DObject Constructor
//Just Setting All Pointers To NULL
C3DObject::C3DObject()
{
	this->pVertexArray=NULL;
	this->pNormalArray=NULL;
	this->pPrimaryVertexArray=NULL;
	this->pPrimaryNormalArray=NULL;

	this->pPrimaryColorArray=NULL;
	this->pSecondaryColorArray=NULL;
	
	this->vertexnum=0;

	this->pIndexArray=NULL;
	this->indexnum=0;

	this->texnum=0;
	this->pTexCountCoord=NULL;
	//this->ppTexCoords=NULL;

	this->TexCoordsList=NULL;	
	//Clearing Vertex Description Structure
	memset(&VertStruct,0,sizeof(tVertexDesc));
	//Setting Model Matrix to Identity Matrix
	ModelMatrix.Identity();
}


void C3DObject::Copy3DObject(C3DObject *obj3D)
{

	if(obj3D->vertexnum==0)
		return ;

	this->Release();
	this->vertexnum = obj3D->vertexnum;

	if(obj3D->pVertexArray!=NULL)
	{
		pVertexArray = new Vector3D[vertexnum];
		memcpy(pVertexArray,obj3D->pVertexArray,sizeof(Vector3D)*vertexnum);
		VertStruct.VertDesc |= VERTEX_POSITION_FLAG;
	}

	if(obj3D->pPrimaryVertexArray!=NULL)
	{
		pPrimaryVertexArray = new Vector3D[vertexnum];
		memcpy(pPrimaryVertexArray,obj3D->pPrimaryVertexArray,sizeof(Vector3D)*vertexnum);
	}

	if(obj3D->pNormalArray!=NULL)
	{
		pNormalArray = new Vector3D[vertexnum];
		memcpy(pNormalArray,obj3D->pNormalArray,sizeof(Vector3D)*vertexnum);
		VertStruct.VertDesc |= VERTEX_NORMAL_FLAG;
	}

	if(obj3D->pPrimaryNormalArray!=NULL)
	{
		pPrimaryNormalArray = new Vector3D[vertexnum];
		memcpy(pPrimaryNormalArray,obj3D->pPrimaryNormalArray,sizeof(Vector3D)*vertexnum);
	}

	if(obj3D->indexnum!=0)
	{
		this->indexnum = obj3D->indexnum;

		pIndexArray = new uint[indexnum];
		memcpy(pIndexArray,obj3D->pIndexArray,sizeof(uint)*indexnum);
		VertStruct.VertDesc |= VERTEX_INDEXED_FLAG;
	}

	if(obj3D->pPrimaryColorArray!=NULL)
	{
		pPrimaryColorArray = new ubyte[vertexnum*4];
		memcpy(pPrimaryColorArray,obj3D->pPrimaryColorArray,sizeof(ubyte)*vertexnum*4);
		VertStruct.VertDesc |= VERTEX_PRIMARY_COLOR_FLAG;
	}

	if(obj3D->pSecondaryColorArray!=NULL)
	{
		pSecondaryColorArray = new ubyte[vertexnum*3];
		memcpy(pSecondaryColorArray,obj3D->pSecondaryColorArray,sizeof(ubyte)*vertexnum*3);
		VertStruct.VertDesc |= VERTEX_SECONDARY_COLOR_FLAG;
	}

	if(obj3D->texnum!=0)
	{
		this->texnum = obj3D->texnum;

		//Handeling with textures coordinates
		TexCoordsList = new tagTexList;
		tagTexList* texptr = TexCoordsList;
		tagTexList* texobj = obj3D->TexCoordsList;

		ubyte tn = texptr->TexCountCoord = texobj->TexCountCoord;
		texptr->pTexCoordArray = new float [vertexnum*tn];
		memcpy(texptr->pTexCoordArray,texobj->pTexCoordArray,sizeof(float)*tn*vertexnum);
		texptr->next = NULL;

		uint64 flag=1;
		VertStruct.VertDesc |= (flag<<19);
		if(tn>0)
			VertStruct.TexCount |= (tn-1);

		for(uint x=1; x<texnum; x++)
		{
			texptr->next = new tagTexList;
			texptr = texptr->next;
			texobj = texobj->next;
			texptr->next = NULL;

			tn = texptr->TexCountCoord = texobj->TexCountCoord;
			texptr->pTexCoordArray = new float [vertexnum*tn];
			memcpy(texptr->pTexCoordArray,texobj->pTexCoordArray,sizeof(float)*tn*vertexnum);
			texptr->next = NULL;

			VertStruct.VertDesc |= (flag<<19+x);
			if(tn>0)
				VertStruct.TexCount |= ((tn-1)<<x*2);
		}	
	}

	ModelMatrix = obj3D->ModelMatrix;
}


C3DObject::~C3DObject()
{
	this->Release();
}

void C3DObject::Release()
{
	if(pVertexArray!=NULL)
	{
		delete [] pVertexArray;
		pVertexArray=NULL;
	}

	if(pNormalArray!=NULL)
	{
		delete [] pNormalArray;
		pNormalArray=NULL;
	}

	if(pPrimaryVertexArray!=NULL)
	{
		delete [] pPrimaryVertexArray;
		pPrimaryVertexArray=NULL;
	}

	if(pPrimaryNormalArray!=NULL)
	{
		delete [] pPrimaryNormalArray;
		pPrimaryNormalArray=NULL;
	}

	if(pPrimaryColorArray != NULL)
	{
		delete [] pPrimaryColorArray;
		pPrimaryColorArray=NULL;
	}
	
	if(pSecondaryColorArray != NULL)
	{
		delete [] pSecondaryColorArray;
		pSecondaryColorArray=NULL;
	}

	if(pIndexArray!=NULL)
	{
		delete [] pIndexArray;
		pIndexArray=NULL;
	}
	
	if(TexCoordsList!=NULL)
	{
		tagTexList* ptr = TexCoordsList;

		while(ptr!=NULL)
		{
			ptr = TexCoordsList->next;
			TexCoordsList->TexCountCoord = 0;
			delete [] TexCoordsList->pTexCoordArray;
			TexCoordsList=ptr;
		}
	}

	vertexnum=0;
	indexnum=0;
	texnum=0;
	memset(&VertStruct,0,sizeof(tVertexDesc));
}


void C3DObject::CloneMesh()
{
	if(vertexnum==0)
		return ;

	if(pVertexArray!=NULL)
	{
		if(pPrimaryVertexArray==NULL)
			pPrimaryVertexArray = new Vector3D[vertexnum];
		
		memcpy(pPrimaryVertexArray,pVertexArray,sizeof(Vector3D)*vertexnum);
	}

	if(pNormalArray!=NULL)
	{
		if(pPrimaryNormalArray==NULL)
			pPrimaryNormalArray = new Vector3D[vertexnum];
		
		memcpy(pPrimaryNormalArray,pNormalArray,sizeof(Vector3D)*vertexnum);
	}
}

bool C3DObject::CopyFromCloneMesh()
{
	if(vertexnum==0)
		return false;

	if(pPrimaryVertexArray==NULL)
		return false;

	if(pVertexArray==NULL)
		return false;

	memcpy(pVertexArray,pPrimaryVertexArray,sizeof(Vector3D)*vertexnum);

	if(pPrimaryNormalArray!=NULL && pNormalArray!=NULL)
	{
		memcpy(pNormalArray,pPrimaryNormalArray,sizeof(Vector3D)*vertexnum);
	}

	return true;
}



bool C3DObject::CreateVertexArray(uint vertnum,bool CreateNormalArray)
{
	if(pVertexArray!=NULL || pNormalArray!=NULL)
		return false;

	if(vertnum==0)
		return false;

	vertexnum=vertnum;
	pVertexArray = new Vector3D[vertexnum];
	VertStruct.VertDesc |= VERTEX_POSITION_FLAG;

	if(CreateNormalArray)
	{
		pNormalArray = new Vector3D[vertexnum];
		VertStruct.VertDesc |= VERTEX_NORMAL_FLAG;
	}

	return true;
}

bool C3DObject::CreateIndexArray(uint indnum)
{
	if(pIndexArray!=NULL)
		return false;

	if(indnum==0)
		return false;

	if(vertexnum==0)
		return false;

	indexnum=indnum;
	pIndexArray = new uint [indnum];
	VertStruct.VertDesc |= VERTEX_INDEXED_FLAG;

	return true;
}

bool C3DObject::CreatePrimaryColorArray()
{
	if(pPrimaryColorArray!=NULL)
		return false;

	pPrimaryColorArray = new ubyte[vertexnum*sizeof(ubyte)*4];

	VertStruct.VertDesc |= VERTEX_PRIMARY_COLOR_FLAG;

	return true;
}

bool C3DObject::CreateSecondaryColorArray()
{
	if(pSecondaryColorArray!=NULL)
		return false;

	pSecondaryColorArray = new ubyte[vertexnum*sizeof(ubyte)*3];

	VertStruct.VertDesc |= VERTEX_SECONDARY_COLOR_FLAG;

	return true;
}

void C3DObject::SwapIndecies()
{
	uint tmpind=0;
	for(uint i=0; i<this->indexnum; i+=3)
	{
		tmpind=this->pIndexArray[i];
		this->pIndexArray[i]=this->pIndexArray[i+2];
		this->pIndexArray[i+2]=tmpind;
	}
}

bool C3DObject::AddTexCoordArray(ubyte CoordCount)
{
	if(vertexnum==0)
		return false;

	if(CoordCount==0 || CoordCount>3)
		return false;

	uint64 i=1;
	if(texnum==0)
	{
		texnum=1;

		TexCoordsList = new tagTexList;
		ubyte tn = TexCoordsList->TexCountCoord = CoordCount;
		TexCoordsList->pTexCoordArray = new float [tn*vertexnum];
		TexCoordsList->next = NULL;
		memset(TexCoordsList->CoordDist,0,sizeof(float)*4);

		VertStruct.VertDesc |= (i<<19);
		if(tn>0)
			VertStruct.TexCount |= (tn-1);
	}
	else
	{
		tagTexList* ptr = TexCoordsList;
		while(ptr->next!=NULL)
			ptr = ptr->next;

		ptr->next = new tagTexList;
		ptr = ptr->next;
		ptr->TexCountCoord = CoordCount;
		ptr->pTexCoordArray = new float[CoordCount*vertexnum];
		ptr->next = NULL;
		memset(TexCoordsList->CoordDist,0,sizeof(float)*4);
		texnum+=1;

		VertStruct.VertDesc |= (i<<19+texnum-1);
		if(CoordCount>2)
			VertStruct.TexCount |= ((CoordCount-1)<<(texnum-1)*2);
	}

	return true;
}

void C3DObject::CalcCoordDist(uint Tn)
{
	if(Tn>texnum)
		return ;

	tagTexList* ptr = TexCoordsList;

	for(uint i=0; i<Tn; i++)
		ptr=ptr->next;

	ubyte tn = ptr->TexCountCoord;
	float *TexCoords = ptr->pTexCoordArray;
	
	float min[4],max[4];
	memset(&min,0,sizeof(float)*4);
	memset(&max,0,sizeof(float)*4);
	
	for(i=0; i<vertexnum*tn; i++)
	{
		for(uint s=0; s<tn; s++)
		{
			if(TexCoords[0]>max[s])
				max[s]=TexCoords[0];
			if(TexCoords[0]<min[s])
				min[s]=TexCoords[0];

			TexCoords++;
		}
	}

	for(i=0; i<tn; i++)
		ptr->CoordDist[i] = max[i] + abs(min[i]);
	
}

float* C3DObject::GetTexCoordArray(uint Tn)
{
	if(Tn>texnum)
		return NULL;

	tagTexList* ptr = TexCoordsList;

	for(uint i=0; i<Tn; i++)
		ptr=ptr->next;
	
	return ptr->pTexCoordArray;
}

ubyte C3DObject::GetTexCountCoord(uint Tn)
{
	if(Tn>texnum)
		return 0;

	tagTexList* ptr = TexCoordsList;

	for(uint i=0; i<Tn; i++)
		ptr=ptr->next;
	
	return ptr->TexCountCoord;
}