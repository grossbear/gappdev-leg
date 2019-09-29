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
enum MeshPrimitiveMode
{
    MESH_MODE_LINES,
    MESH_MODE_LINE_STRIP,
    MESH_MODE_TRIANGLES,
    MESH_MODE_TRI_STRIP,
    MESH_MODE_TRI_FAN,
    MESH_MODE_QUADS,
    MESH_MODE_QUAD_STRIP,
};
typedef MeshPrimitiveMode MshPrimMode;


/////////////////////////////////////////////////////////////////////////////////////////////
class MeshPrimModeInfo
{
public:
    uint32t start_index;
    uint32t end_index;
    MeshPrimitiveMode primitive_mode;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//VertIndDataOrganizer
class CVertIndicesDataOrganazier
{
    friend class CMeshData;

public:
    CVertIndicesDataOrganazier(uint32t vert_count);
    ~CVertIndicesDataOrganazier();

    void AppendPrimitiveDataIndices(MeshPrimitiveMode mode, uint32t *indices, uint32t idxcount);

protected:
    struct IndicesData
    {
        const void *IndexArray;
        uint32t IndexCount;
        std::vector<MeshPrimModeInfo> PrimitivesInfo;
    };

    IndicesData* BuildIndicesData();
    
    struct PrimitiveData
    {
        std::vector<uint32t> Indices;
        MeshPrimitiveMode PrimitiveMode;
    };

//__CPLUSPLUS_VER_11

    class CPrimitiveSortDataFunctor {
    public:
        bool operator() (PrimitiveData &obj1, PrimitiveData &obj2);
    };

    class CVertexIndexCheckerFunctor {
    public:
        CVertexIndexCheckerFunctor(uint32t max_vtx_cnt);
        bool operator() (uint32t index);
    private:
        uint32t mMaxVertexCount;
    };


    std::list<PrimitiveData> mIndicesDataList;

    //std::vector<MeshPrimModeInfo> mPrimitiveInfo;

    uint32t mVertCount;

    uint32t *mIndices32;
    uint16t *mIndices16;
    uint32t mIndexCount;

    uint32t mMeshPrimitivesCount[MESH_MODE_QUAD_STRIP + 1];
};
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
class CMeshData
{
public:
    CMeshData(uint32t vertex_count);
    ~CMeshData();

    //reset indices data
    void SetPrimitiveData(MeshPrimModeInfo *priminfo, uint32t count);
    //remove prev primitive data and index buffer
    void CreateIndicesData(CVertIndicesDataOrganazier &IndDataOrg);

    void CreateNormalArray();
    void CreatePrimaryColorArray();
    void CreateSecondaryColorArray();
    void CreateTangentArray();
    void CreateBinormalArray();
    void CreateTexCoordArray(uint32t coords_size);

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

public:



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

    //std::vector<std::pair<uint32t, uint32t>> mMeshTriModeIndices;
    //std::vector<std::pair<uint32t, uint32t>> mMeshTriStripModeIndices;
    //std::vector<std::pair<uint32t, uint32t>> mMeshTriFanModeIndices;
    
protected:
    inline void ReleaseVertexArray();
    inline void ReleaseNormalArray();
    inline void ReleaseIndexArray();
    inline void ReleasePrimColorArray();
    inline void ReleaseSecColorArray();
    inline void ReleaseTangentArray();
    inline void ReleaseBinormalArray();
    inline void ReleaseTexCoordsArray();

};
/////////////////////////////////////////////////////////////////////////////////////////////



#endif //__MESHDATA_H__