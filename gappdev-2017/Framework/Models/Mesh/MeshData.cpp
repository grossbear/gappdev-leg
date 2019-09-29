/////////////////////////////////////////////////////////////////////////////////////////////
// MeshData.cpp
//
// Mesh Data Class
/////////////////////////////////////////////////////////////////////////////////////////////

#include "MeshData.h"
#include "Math/MathLib.h"

#include <string>

#include <algorithm>

#define FALSE_INDEX     UINT_MAX_VAL

/////////////////////////////////////////////////////////////////////////////////////////////
//Functors
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVertIndicesDataOrganazier::CPrimitiveSortDataFunctor::operator() (PrimitiveData &obj1, PrimitiveData &obj2)
{
    return obj1.PrimitiveMode < obj2.PrimitiveMode;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CVertIndicesDataOrganazier::CVertexIndexCheckerFunctor::CVertexIndexCheckerFunctor(uint32t max_vtx_cnt):
mMaxVertexCount(max_vtx_cnt){}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVertIndicesDataOrganazier::CVertexIndexCheckerFunctor::operator() (uint32t index) 
{ 
    return (index >= mMaxVertexCount); 
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
CVertIndicesDataOrganazier::CVertIndicesDataOrganazier(uint32t vert_count):
mVertCount(vert_count),
mIndices32(NULL),
mIndices16(NULL),
mIndexCount(0)
{
    memset(mMeshPrimitivesCount, 0, sizeof(mMeshPrimitivesCount));
}
/////////////////////////////////////////////////////////////////////////////////////////////
CVertIndicesDataOrganazier::~CVertIndicesDataOrganazier()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////
void CVertIndicesDataOrganazier::AppendPrimitiveDataIndices(MeshPrimitiveMode mode, uint32t *indices, uint32t idxcount)
{
    mIndicesDataList.push_back(PrimitiveData());
    PrimitiveData &primitive_data = mIndicesDataList.front();
    primitive_data.PrimitiveMode = mode;
    primitive_data.Indices.assign(indices, indices + idxcount);

/*#ifdef __CPLUSPLUS_VER_11
    uint32t maxval = mVertCount;
    std::vector<uint32t>::const_iterator it =
        std::find_if(primitive_data.Indices.begin(), primitive_data.Indices.end(),
            [this](uint32t index) {return (index >= this->mVertCount) ? (true) : (false); });
#else*/
    std::vector<uint32t>::const_iterator it =
        std::find_if(primitive_data.Indices.begin(), primitive_data.Indices.end(), CVertexIndexCheckerFunctor(mVertCount));
//#endif //__CPLUSPLUS_VER_11

    if (it != primitive_data.Indices.end()) {
        mIndicesDataList.pop_back();
        return;
    }
    mIndexCount += idxcount;

    //Counting repeating of different primitives types
    mMeshPrimitivesCount[mode]++;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CVertIndicesDataOrganazier::IndicesData* CVertIndicesDataOrganazier::BuildIndicesData()
{
    if (mIndicesDataList.empty())
        return NULL;

    mIndicesDataList.sort(CPrimitiveSortDataFunctor());

    CVertIndicesDataOrganazier::IndicesData *pIndicesData = new CVertIndicesDataOrganazier::IndicesData();
    uint32t IndexCount = mIndexCount;
    
    IndexCount += (mMeshPrimitivesCount[MESH_MODE_LINE_STRIP] > 1) ? (mMeshPrimitivesCount[MESH_MODE_LINE_STRIP] - 1) : (0);
    IndexCount += (mMeshPrimitivesCount[MESH_MODE_TRI_STRIP] > 1) ? (mMeshPrimitivesCount[MESH_MODE_TRI_STRIP] - 1) : (0);
    IndexCount += (mMeshPrimitivesCount[MESH_MODE_TRI_FAN] > 1) ? (mMeshPrimitivesCount[MESH_MODE_TRI_FAN] - 1) : (0);
    IndexCount += (mMeshPrimitivesCount[MESH_MODE_QUAD_STRIP] > 1) ? (mMeshPrimitivesCount[MESH_MODE_QUAD_STRIP] - 1) : (0);

    /*
    MESH_MODE_LINES,
    MESH_MODE_LINE_STRIP,
    MESH_MODE_TRIANGLES,
    MESH_MODE_TRI_STRIP,
    MESH_MODE_TRI_FAN,
    MESH_MODE_QUADS,
    MESH_MODE_QUAD_STRIP,
    */
    bool PrimitiveRestartIdxFlagsTab[MESH_MODE_QUAD_STRIP + 1] = { false,true,false,true,true,false,true };

    if (IndexCount > 0xffff) {
        uint32t *IndexBufferUI32 = new uint32t[IndexCount];
       
        pIndicesData->IndexArray = IndexBufferUI32;

        uint32t index = 0;
        std::list<PrimitiveData>::iterator it = mIndicesDataList.begin();
        bool bPrimitiveWithRestartIndex = false;
        uint32t PrimitiveRestartIndCnt = 0;

        for (std::list<PrimitiveData>::iterator it = mIndicesDataList.begin(); it != mIndicesDataList.end(); it++) 
        {
            const std::vector<uint32t> &indicesvec = it->Indices;
            memcpy(&IndexBufferUI32[index], &indicesvec[0], indicesvec.size());

            MeshPrimModeInfo primitive_info;
            MeshPrimitiveMode PrimitiveMode = it->PrimitiveMode;

            primitive_info.start_index = index;
            primitive_info.end_index = index + (uint32t)indicesvec.size() - 1;
            primitive_info.primitive_mode = PrimitiveMode;
            pIndicesData->PrimitivesInfo.push_back(primitive_info);

            index += (uint32t)indicesvec.size();

            if (PrimitiveRestartIdxFlagsTab[PrimitiveMode] && bPrimitiveWithRestartIndex == false) {
                bPrimitiveWithRestartIndex = mMeshPrimitivesCount[PrimitiveMode] > 1;
                PrimitiveRestartIndCnt = mMeshPrimitivesCount[PrimitiveMode] - 1;
            }

            if (bPrimitiveWithRestartIndex) {//lead to crash!!
                IndexBufferUI32[index++] = 0xffffffff;
                PrimitiveRestartIndCnt--;
            }
             
        }

        
    }
    else {
        uint16t *IndexBufferUI16 = new uint16t[IndexCount];

        pIndicesData->IndexArray = IndexBufferUI16;
    }
   

    return NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
CMeshData::CMeshData(uint32t vert_num):mVertexCount(vert_num),
mNormals(NULL),

mIndexData32(NULL),
mIndexData16(NULL),
mIndexCount(0),

mPrimaryColors(NULL),
mSecondaryColors(NULL),
mTangents(NULL),
mBinormals(NULL),

Normals(NULL),
IndicesUI32(NULL),
IndicesUI16(NULL),
PrimaryColors(NULL),
SecondaryColors(NULL),
Tangents(NULL),
Binormals(NULL)
{
    mVertices = new CPoint3[mVertexCount];
    Vertices = mVertices;

    memset(mTexCoords, 0, sizeof(float*)*MAX_TEX_COORD_ARRAYS_COUNT);
    memset(mTexCoordsSizes, 0, sizeof(uint32t)*MAX_TEX_COORD_ARRAYS_COUNT);
    
    //std::for_each(TexCoords, TexCoords + MAX_TEX_COORD_ARRAYS_COUNT, ResetObj);
    memset(TexCoords, 0, sizeof(float*)*MAX_TEX_COORD_ARRAYS_COUNT);
}

/////////////////////////////////////////////////////////////////////////////////////////////
CMeshData::~CMeshData()
{
    ReleaseVertexArray();
    ReleaseNormalArray();
    ReleaseIndexArray();
    ReleasePrimColorArray();
    ReleaseSecColorArray();
    ReleaseTangentArray();
    ReleaseBinormalArray();
    ReleaseTexCoordsArray();
}
/////////////////////////////////////////////////////////////////////////////////////////////
//reset indices data
void CMeshData::SetPrimitiveData(MeshPrimModeInfo *priminfo, uint32t count)
{
    ReleaseIndexArray();
}
/////////////////////////////////////////////////////////////////////////////////////////////
//remove prev primitive data and index buffer
void CMeshData::CreateIndicesData(CVertIndicesDataOrganazier &IndDataOrg)
{
    ReleaseIndexArray();
    //const void *pointer = IndDataOrg.BuildIndicesData(mIndexCount);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::CreateNormalArray()
{
    if (mNormals)
        return;
    mNormals = new CNormal3[mVertexCount];
    Normals = mNormals;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::CreatePrimaryColorArray()
{
    if (mPrimaryColors)
        return;

    mPrimaryColors = new uint32t[mVertexCount];
    memset(mPrimaryColors, 0xff, mVertexCount*sizeof(uint32t));
    PrimaryColors = mPrimaryColors;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::CreateSecondaryColorArray() 
{
    if (mSecondaryColors)
        return;

    mSecondaryColors = new uint32t[mVertexCount];
    memset(mSecondaryColors, 0xff, mVertexCount*sizeof(uint32t));
    SecondaryColors = mSecondaryColors;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::CreateTangentArray()
{
    if (mTangents) return;
    mTangents = new CNormal3[mVertexCount];
    Tangents = mTangents;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::CreateBinormalArray() 
{
    if (mBinormals) return;
    mBinormals = new CNormal3[mVertexCount];
    Binormals = mBinormals;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::CreateTexCoordArray(uint32t coords_size)
{
    uint32t *ptr = std::find(mTexCoordsSizes, mTexCoordsSizes + MAX_TEX_COORD_ARRAYS_COUNT, 0);
    if (ptr == NULL) return;

    uint32t *beginptr = mTexCoordsSizes;
    uint32t free_pos = ptr - beginptr;

    coords_size = mmax(mmin(coords_size, uint32t(4)), uint32t(1));
    
    mTexCoords[free_pos] = new float[mVertexCount*coords_size];
    mTexCoordsSizes[free_pos] = coords_size;

    TexCoords[free_pos] = mTexCoords[free_pos];
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CMeshData::GetVertexCount() const
{
    return mVertexCount;
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CMeshData::GetIndexCount() const
{
    return mIndexCount;
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CMeshData::GetTexCoordsArraysCount() const
{
    const uint32t *ptr = std::find(mTexCoordsSizes, mTexCoordsSizes + MAX_TEX_COORD_ARRAYS_COUNT, 0);
    if (ptr == NULL) return MAX_TEX_COORD_ARRAYS_COUNT;

    const uint32t free_pos = ptr - (mTexCoordsSizes);
    return free_pos;
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CMeshData::GetTexCoordSize(uint32t arrayidx) const
{
    if (arrayidx >= CMeshData::MAX_TEX_COORD_ARRAYS_COUNT)
        return FALSE_INDEX;

    return mTexCoordsSizes[arrayidx];
}
/////////////////////////////////////////////////////////////////////////////////////////////

void CMeshData::ReleaseVertexArray()
{
    if (mVertices)
        delete[] mVertices;

    mVertices = NULL;
    Vertices = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseNormalArray()
{
    if (mNormals)
        delete[] mNormals;

    mNormals = NULL;
    Normals = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseIndexArray()
{
    if (mIndexData32)
        delete[] mIndexData32;
    if (mIndexData16)
        delete[] mIndexData16;

    IndicesUI32 = NULL;
    IndicesUI16 = NULL;
    mIndexData32 = NULL;
    mIndexData16 = NULL;
    mIndexCount = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleasePrimColorArray()
{
    if (mPrimaryColors)
        delete[] mPrimaryColors;
    mPrimaryColors = NULL;
    PrimaryColors = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseSecColorArray() 
{
    if (mSecondaryColors)
        delete[] mSecondaryColors;
    mSecondaryColors = NULL;
    SecondaryColors = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseTangentArray()
{
    if (mTangents)
        delete[] mTangents;
    mTangents = NULL;
    Tangents = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseBinormalArray()
{
    if (mBinormals)
        delete[] mBinormals;
    mBinormals = NULL;
    Binormals = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseTexCoordsArray()
{
    memset(mTexCoordsSizes, 0, MAX_TEX_COORD_ARRAYS_COUNT*sizeof(uint32t));
    std::for_each(mTexCoords, mTexCoords + MAX_TEX_COORD_ARRAYS_COUNT, [](float *ptr) {if (ptr) delete[] ptr; });
    
    //std::for_each(mTexCoords, mTexCoords + MAX_TEX_COORD_ARRAYS_COUNT, [](float *ptr) {ptr = NULL; });
    //std::for_each(TexCoords, TexCoords + MAX_TEX_COORD_ARRAYS_COUNT, [](float *ptr) {ptr = NULL; });

    memset(mTexCoords, 0, sizeof(float*)*MAX_TEX_COORD_ARRAYS_COUNT);
    memset(TexCoords, 0, sizeof(float*)*MAX_TEX_COORD_ARRAYS_COUNT);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


