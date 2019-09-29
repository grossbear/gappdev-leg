////////////////////////////////////////////////////////////////////////////////////////////////
//	3DObject.h 
//	3DObject class declaration
//	Created: 16th February 2006
//	Modified: 27th May 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _3D_OBJECT_H_
#define _3D_OBJECT_H_

typedef struct tagTexList
{
	ubyte  TexCountCoord;
	float* pTexCoordArray;
	float CoordDist[4];
	tagTexList* next;
}TexList;

//vertex declaration
//1 - vertex position
//2 - normal
//3 - primary color
//4 - secondary color
//5 - fog coordinate
//6 - indexed array
//7 - vertex blend weight
//8 - blend indicies
//9 - none
//20- texture0 coords
#define VERTEX_POSITION_FLAG				1
#define VERTEX_NORMAL_FLAG					2
#define VERTEX_PRIMARY_COLOR_FLAG			4
#define VERTEX_SECONDARY_COLOR_FLAG			8
#define VERTEX_FOG_COORDINATE_FLAG			16
#define VERTEX_INDEXED_FLAG					32
#define VERTEX_BLEND_WEIGHT_FLAG			64
#define VERTEX_BLEND_INDICES_FLAG			128

typedef struct tVertexDesc
{
	uint64 VertDesc;
	uint64 TexCount;
}VertexDesc;

class C3DObject
{
public:
	C3DObject();
	C3DObject(C3DObject *copy){C3DObject(); this->Copy3DObject(copy);}
	~C3DObject();

	void MultMatrix(const Matrix4x4 &M){ModelMatrix*=M;}
	void MultMatrixAndTransform(const Matrix4x4 &M);
	void SetMatrix(const Matrix4x4 &M){ModelMatrix=M;}
	void Transform(const Matrix4x4 &M);
	void CloneMesh();
	bool CopyFromCloneMesh();
	void Copy3DObject(C3DObject* obj3D);

	bool CreateVertexArray(uint vertnum,bool CreateNormalArray=true);
	bool CreateIndexArray(uint indnum);
	bool CreatePrimaryColorArray();
	bool CreateSecondaryColorArray();
	bool AddTexCoordArray(ubyte CoordCount);
	void CalcCoordDist(uint Tn); 
	void SwapIndecies();
	void Release();

	const Matrix4x4& GetModelMatrix(){return ModelMatrix;}

	Vector3D* GetVertexArray(){return pVertexArray;}
	Vector3D* GetNormalArray(){return pNormalArray;}

	ubyte* GetPrimaryColorArray(){return pPrimaryColorArray;}
	ubyte* GetSecondaryColorArray(){return pSecondaryColorArray;}
	uint  GetVertexNum(){return vertexnum;}

	uint* GetIndexArray(){return pIndexArray;}
	uint  GetIndexNum(){return indexnum;}

	uint  GetTexNum(){return texnum;}
	float* GetTexCoordArray(uint Tn);
	ubyte GetTexCountCoord(uint Tn);

	tVertexDesc GetVertexDescription(){return VertStruct;}

	//bad use can cause program crash !!!
	void SetNewVertexPointer(Vector3D *Ptr){pVertexArray = Ptr;}

protected:
	Matrix4x4 ModelMatrix;

	uint vertexnum;
	Vector3D *pVertexArray;
	Vector3D *pNormalArray;

	Vector3D *pPrimaryVertexArray;
	Vector3D *pPrimaryNormalArray;

	ubyte *pPrimaryColorArray;
	ubyte *pSecondaryColorArray;

	uint indexnum;
	uint* pIndexArray;

	uint texnum;
	ubyte* pTexCountCoord;
	//float** ppTexCoords;
	
	TexList* TexCoordsList;

	tVertexDesc VertStruct;
};


#endif  //_3D_OBJECT_H_