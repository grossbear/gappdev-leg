//////////////////////////////////////////////////////////////////////////////////
//D3DRendereElement.h
//////////////////////////////////////////////////////////////////////////////////

typedef struct tagD3DRenderElement
{
	VertexDesc VertStruct;
	LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration;

	uint VertexNum;
	uint IndexNum;
	uint TexNum;

	bool dynamic; //is data in dynamic buffer

	//Vertex And Index Buffers
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	LPDIRECT3DVERTEXBUFFER9 pVertexVB;
	LPDIRECT3DVERTEXBUFFER9 pNormalVB;
	LPDIRECT3DVERTEXBUFFER9 pDiffuseVB;
	LPDIRECT3DVERTEXBUFFER9 pSpecularVB;
	LPDIRECT3DVERTEXBUFFER9 pVertexWeightsVB;
	LPDIRECT3DVERTEXBUFFER9 pBlendIndicesVB;
	
	LPDIRECT3DVERTEXBUFFER9 pTextureVB[32];

	C3DObjectDesc *ObjDesc;
	
	D3DXMATRIX ModelMatrix;
	
}D3DRenderElement;