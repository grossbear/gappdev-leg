/////////////////////////////////////////////////////////////////////////////////////////////
// MeshData.h
//
// Mesh Data Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MESHDATA_H__
#define __MESHDATA_H__

#include <PlatformTypes.h>
#include <vector>
#include <list>
#include <map>


/////////////////////////////////////////////////////////////////////////////////////////////
class CPoint3
{
public:
    CPoint3():x(0.0),y(0.0),z(0.0){}
    CPoint3(float xv, float yv, float zv):x(xv),y(yv),z(zv){}

    float x, y, z;
};

/////////////////////////////////////////////////////////////////////////////////////////////
class CNormal3
{
public:
    CNormal3():nx(0.0),ny(0.0),nz(0.0){}
    CNormal3(float xv, float yv, float zv):nx(xv),ny(yv),nz(zv){}

    float nx, ny, nz;
};

/////////////////////////////////////////////////////////////////////////////////////////////
enum MeshPrimitiveType
{
    MESH_PRIMITIVE_TYPE_LINES,
    MESH_PRIMITIVE_TYPE_LINE_STRIP,
    MESH_PRIMITIVE_TYPE_TRIANGLES,
    MESH_PRIMITIVE_TYPE_TRIANGLE_STRIP,
    MESH_PRIMITIVE_TYPE_TRIANGLE_FAN,
    MESH_PRIMITIVE_TYPE_QUADS,
    MESH_PRIMITIVE_TYPE_QUAD_STRIP,
    MESH_PRIMITIVE_TYPE_COUNT
};
typedef MeshPrimitiveType MshPrimType;


/////////////////////////////////////////////////////////////////////////////////////////////
class MeshPrimitiveTypeInfo
{
public:
    uint32t StartIndex;
    uint32t EndIndex;
    MeshPrimitiveType PrimitiveType;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//VertIndDataOrganizer
// Class For Organizing, Sorting Vertex Indices Data And Adding Restart Indices
class CVertIndicesDataOrganazier
{
    friend class CMeshData;

public:
    CVertIndicesDataOrganazier(uint32t vert_count);
    ~CVertIndicesDataOrganazier();

    void AppendPrimitiveDataIndices(MeshPrimitiveType mode, uint32t *indices, uint32t idxcount);

protected:
    //Indices Data Return Struct To CMeshData Class After Bulding Data 
    struct IndicesData
    {
        const void *IndexArray;
        uint32t IndexCount;
        std::vector<MeshPrimitiveTypeInfo> PrimitivesInfo;
    };

    //Keeps Current Indices Data That Came From Outside
    struct PrimitiveData
    {
        std::vector<uint32t> Indices;
        MeshPrimitiveType PrimitiveType;
    };

    //Index Data Type Spec
    enum IndexDataType {
        INDEX_UI16,
        INDEX_UI32
    };

#ifndef __CPLUSPLUS_VER_11
    /////////////////////////////////////////////////////////////////////////
    // Some Functors That Manages Indices Data
    class CPrimitiveSortDataFunctor {
    public:
        bool operator() (PrimitiveData &obj1, PrimitiveData &obj2);
    };
    /////////////////////////////////////////////////////////////////////////
    class CVertexIndexCheckerFunctor {
    public:
        CVertexIndexCheckerFunctor(uint32t max_vtx_cnt);
        bool operator() (uint32t index);
    private:
        uint32t mMaxVertexCount;
    };
#endif //__CPLUSPLUS_VER_11

    /////////////////////////////////////////////////////////////////////////
    // Special Functor That Process Index data
    class CPrimitiveIndDataProc {
    public:
        CPrimitiveIndDataProc(CVertIndicesDataOrganazier::IndicesData *ptr, IndexDataType data_type);
        void operator() (const PrimitiveData &primitive_data);
    private:
        bool HasRestartIndex(const PrimitiveData &primitive_data);
        void SetRestartIndex(uint32t start_idx);
        void CopyIndexData(const PrimitiveData &primitive_data, uint32t start_idx);
        CVertIndicesDataOrganazier::IndicesData *mIndicesDataPtr;
        IndexDataType mIdxDataType;
    };
    /////////////////////////////////////////////////////////////////////////

    // Primitves That Should Have Restart Index
    static const bool mPrimitiveRestartIdxFlagsTab[MESH_PRIMITIVE_TYPE_COUNT];

    // Building Indices Data Method
    IndicesData* BuildIndicesData();

    //List That Store Incoming Primitive Indices Data
    std::list<PrimitiveData> mIndicesDataList;
    
    uint32t mVertCount;
    uint32t *mIndices32;
    uint16t *mIndices16;
    uint32t mIndexCount;

    uint32t mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_COUNT];
};
/////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////
// Primary Class That Keeps Primary Mesh Data
class CMeshData
{
public:
    CMeshData(uint32t vertex_count);
    ~CMeshData();

    //reset indices data
    void SetPrimitiveData(MeshPrimitiveTypeInfo *priminfo, uint32t count);
    //remove prev primitive data and index buffer
    void CreateIndicesData(CVertIndicesDataOrganazier &IndDataOrg);

    void CreateNormalArray();
    void CreatePrimaryColorArray();
    void CreateSecondaryColorArray();
    void CreateTangentArray();
    void CreateBinormalArray();
    void CreateTexCoordArray(uint32t coords_size);

    void ResetMeshPrimitiveIterator();
    void NextPrimitive();
    void NextPrimitiveType();

public:
    uint32t GetVertexCount() const;
    uint32t GetIndexCount() const;

    CPoint3 *Vertices;
    CNormal3 *Normals;

    const uint32t *IndicesUI32;
    const uint16t *IndicesUI16;

    uint32t *PrimaryColors;
    uint32t *SecondaryColors;
    CNormal3 *Tangents;
    CNormal3 *Binormals;

    const static uint32t MAX_TEX_COORD_ARRAYS_COUNT = 8;
    uint32t GetTexCoordsArraysCount() const;
    uint32t GetTexCoordSize(uint32t arrayidx) const;

    union {
        struct {
            float *TexCoord0;
            float *TexCoord1;
            float *TexCoord2;
            float *TexCoord3;
            float *TexCoord4;
            float *TexCoord5;
            float *TexCoord6;
            float *TexCoord7;
        };
        float *TexCoords[MAX_TEX_COORD_ARRAYS_COUNT];
    };

    uint32t PrimitiveStartIndex;
    uint32t PrimitiveEndIndex;
    MeshPrimitiveType PrimitiveType;

protected:
    uint32t mVertexCount;
    CPoint3 *mVertices;
    CNormal3 *mNormals;

    //Index Buffer
    uint32t *mIndexData32;
    uint16t *mIndexData16;
    uint32t mIndexCount;

    uint32t *mPrimaryColors;
    uint32t *mSecondaryColors;
    
    CNormal3 *mTangents;
    CNormal3 *mBinormals;

    float *mTexCoords[MAX_TEX_COORD_ARRAYS_COUNT];
    uint32t mTexCoordsSizes[MAX_TEX_COORD_ARRAYS_COUNT];

    std::vector<MeshPrimitiveTypeInfo> mPrimitivesTypeInfo;
    
protected:
    inline void ReleaseVertexArray();
    inline void ReleaseNormalArray();
    inline void ReleaseIndexArray();
    inline void ReleasePrimColorArray();
    inline void ReleaseSecColorArray();
    inline void ReleaseTangentArray();
    inline void ReleaseBinormalArray();
    inline void ReleaseTexCoordsArray();

#ifndef __CPLUSPLUS_VER_11
protected:
    class CPrimitiveSortDataFunctor {
    public:
        bool operator() (MeshPrimitiveTypeInfo &obj1, MeshPrimitiveTypeInfo &obj2);
    };
#endif //__CPLUSPLUS_VER_11
};
/////////////////////////////////////////////////////////////////////////////////////////////



#endif //__MESHDATA_H__