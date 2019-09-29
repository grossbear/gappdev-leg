//////////////////////////////////////////////////////////////////////////////////
//	GLARBRendereElement.h
//	Contains Vertex Buffers IDs And Necessary Info For Rendering Object
//	Created: August 2006
//	
//////////////////////////////////////////////////////////////////////////////////

typedef struct tagGLARBRenderElement
{
	VertexDesc VertStruct;

	uint VertexNum;
	uint IndexNum;
	uint TexNum; //thats mean number of tex coord Arrays in render element

	bool dynamic; //is data in dynamic buffer

	uint *IndexBuffer;
	uint VertexBufferID;
	uint NormalBufferID;
	uint PrimaryColorBufferID;
	uint SecondaryColorBufferID;
	uint VertexWeightBufferID;
	uint TexBuffersID[32];

	C3DObjectDesc *ObjDesc;
	
	Matrix4x4 ModelTransformMatrix;
	//C3DObject* pObj3D;
}GLARBRenderElement;