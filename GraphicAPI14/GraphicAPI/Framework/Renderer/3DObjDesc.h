////////////////////////////////////////////////////////////////////////////////////////////////
//	3DObjDesc.h 
//	3DObjectDescriptor Class
//	Created: 26th May 2006
//  Modified: 30th August 2006 Shader Description Added
//	
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tTexAddrStruct{
	TEXTUREADDRESS	U; // Same S Coord In OpenGL
	TEXTUREADDRESS	V; // Same T Coord In OpenGL
	TEXTUREADDRESS	W; // Same R Coord In OpenGL
} TexAddrStruct;

enum SHADERUNIFORMTYPE
{
	WORLD_VIEW_PROJECTION_MATRIX = 1,
	WORLD_VIEW_PROJECTION_MATRIX_INV = 2,
	WORLD_MATRIX = 3,
	WORLD_MATRIX_INV = 4,
	VIEW_MATRIX = 5,
	PROJECTION_MATRIX = 6,
	NORMAL_MATRIX = 7,
	WORLD_VIEW_MATRIX = 8,
	WORLD_VIEW_MATRIX_INV = 9,		

	VIEW_POSITION = 10,
	VIEW_INV_POSITION = 11,

	LIGHT_POSITION = 20,
	LIGHT_WORLD_INV_POSITION = 21,
	LIGHT_WORLDVIEW_INV_POSITION = 22,
	LIGHT_DIRICTION = 23,
	LIGHT_WORLD_INV_DIRECTION = 24,
	LIGHT_WORLDVIEW_INV_DIRECTION = 25,
	LIGHT_AMBIENT_COLOR = 26,
	LIGHT_DIFFUSE_COLOR = 27,
	LIGHT_SPECULAR_COLOR = 28,
	LIGHT_ATTENUATION_VECTOR3 = 29,
	LIGHT_RANGE = 30,
	LIGHT_FALLOFF = 31,
	LIGHT_THETA = 32,
	LIGHT_PHI = 33,

	MATERIAL_AMBIENT_COLOR = 40,
	MATERIAL_DIFFUSE_COLOR = 41,
	MATERIAL_SPECULAR_COLOR = 42,
	MATERIAL_EMISSION_COLOR = 43,
	MATERIAL_KA_KD_KS_COEF_VECTOR3 = 44,
	MATERIAL_KA_KD_KS_KE_COEF_VECTOR4 = 45,
	MATERIAL_POWER = 46,

	TEXTURE_UNIT = 50,

	USER_MATRIX4X4 = 100,
	USER_MATRIX3X3 = 101,
	USER_VECTOR4 = 102,
	USER_VECTOR3 = 103,
	USER_VECTOR2 = 104,
	USER_FLOAT = 105,
	USER_INT = 106,
	USER_BOOL = 107,
};

typedef struct tagShaderUniformDescriptor
{
	char *name;
	SHADERUNIFORMTYPE UniformType;
	void *param;

	tagShaderUniformDescriptor *next;
	tagShaderUniformDescriptor *prev;
}ShaderUniformDescriptor;


//Shader Descriptor
class CGPUProgramDesc
{
public:
	CGPUProgramDesc():ShaderDescPtr(NULL),ShaderDescPtr2(NULL){}
	virtual ~CGPUProgramDesc();

	bool AddShaderUniformDesc(SHADERUNIFORMTYPE UniformType, char *UniformName, void *data);
	void *GetDataPtrByName(char *UniformName);
	ShaderUniformDescriptor *GetShaderDescList(){return ShaderDescPtr;}
	void Release();

private:
	tagShaderUniformDescriptor *ShaderDescPtr;
	tagShaderUniformDescriptor *ShaderDescPtr2;
};

//3DObject Descriptor
class C3DObjectDesc
{
public:
	C3DObjectDesc();
	C3DObjectDesc(C3DObjectDesc *copy);
	virtual ~C3DObjectDesc();

	bool CopyData(C3DObjectDesc *copy);
	void SetMaterialID(uint MatID){this->MatID = MatID;}
	void SetGPUProgramID(uint GPUProgID){GPUProgram = GPUProgID;}
	void SetTexID(uint Ind, uint TexID);
	void SetTexAddr(uint Ind, TEXADDR U, TEXADDR V, TEXADDR W);
	void SetTexAddrU(uint Ind, TEXADDR U);
	void SetTexAddrV(uint Ind, TEXADDR V);
	void SetTexAddrW(uint Ind, TEXADDR W);
	void SetTexUnits(uint TexUnits); // All Data From TexID And TexAddr Arrays Will Be Lost

	uint GetTexUnitsNum(){return TexUnits;}
	uint GetGPUProgramID(){return GPUProgram;}
	uint GetMaterialID(){return MatID;}

	uint GetTextureID(uint Index);
	TexAddrStruct GetTextureAddress(uint Index);

	CGPUProgramDesc* GetShaderDesc(){return pShaderDesc;}

private:
	void SetTexAddrStructArray();
	uint MatID;
	uint *TexID;
	uint TexUnits; // Number Of Textures That 3DObject Uses
	TexAddrStruct *TexAddr;
	uint GPUProgram;
	CGPUProgramDesc *pShaderDesc; 
};
