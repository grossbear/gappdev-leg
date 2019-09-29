/////////////////////////////////////////////////////////////////////////////////////////////
// MeshData.cpp
//
// Mesh Data Class
/////////////////////////////////////////////////////////////////////////////////////////////

#include "MeshData.h"
//#include "Math/MathLib.h"

#include <string>
#include <algorithm>

#define FALSE_INDEX     UINT_MAX_VAL

#define RESTART_INDEX_UI16 0xffff
#define RESTART_INDEX_UI32 0xffffffff

/////////////////////////////////////////////////////////////////////////////////////////////
// Primitves That Should Have Restart Index
// Static Boolean Table
const bool CVertIndicesDataOrganazier::mPrimitiveRestartIdxFlagsTab[MESH_PRIMITIVE_TYPE_COUNT] =
{ 
    false,  //MESH_PRIMITIVE_TYPE_LINES
    true,   //MESH_PRIMITIVE_TYPE_LINE_STRIP
    false,  //MESH_PRIMITIVE_TYPE_TRIANGLES
    true,   //MESH_PRIMITIVE_TYPE_TRIANGLE_STRIP
    true,   //MESH_PRIMITIVE_TYPE_TRIANGLE_FAN
    false,  //MESH_PRIMITIVE_TYPE_QUADS
    true    //MESH_PRIMITIVE_TYPE_QUAD_STRIP
};

/////////////////////////////////////////////////////////////////////////////////////////////
//Functors
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __CPLUSPLUS_VER_11
bool CVertIndicesDataOrganazier::CPrimitiveSortDataFunctor::operator() (PrimitiveData &obj1, PrimitiveData &obj2)
{
    return obj1.PrimitiveType < obj2.PrimitiveType;
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
#endif //__CPLUSPLUS_VER_11

/////////////////////////////////////////////////////////////////////////////////////////////
CVertIndicesDataOrganazier::CPrimitiveIndDataProc::CPrimitiveIndDataProc(CVertIndicesDataOrganazier::IndicesData *ptr,
    IndexDataType data_type):
    mIndicesDataPtr(ptr), mIdxDataType(data_type){}
/////////////////////////////////////////////////////////////////////////////////////////////
void CVertIndicesDataOrganazier::CPrimitiveIndDataProc::operator() (const PrimitiveData &primitive_data)
{
    uint32t PrimIndCount = (uint32t)primitive_data.Indices.size();

    //Calculating Satrt And End Indices
    uint32t StartIndex = (mIndicesDataPtr->PrimitivesInfo.empty()) ? (0) : (mIndicesDataPtr->PrimitivesInfo.back().EndIndex + 1);
    uint32t EndIndex = StartIndex + PrimIndCount - 1;

    bool bRestartIndex = HasRestartIndex(primitive_data);
    StartIndex = (bRestartIndex) ? (StartIndex+1) : (StartIndex);
    EndIndex = (bRestartIndex) ? (EndIndex+1) : (EndIndex);
    
    //Getting Pointer To Index Array
    void *ptr = const_cast<void*>(mIndicesDataPtr->IndexArray);
    uint32t *IndexArray = reinterpret_cast<uint32t*>(ptr);

    if (bRestartIndex) { SetRestartIndex(StartIndex); }

    //Copy Index Data Into Single Buffer
    CopyIndexData(primitive_data, StartIndex);

    //Setting Primitive Type Info 
    MeshPrimitiveTypeInfo PrimitiveInfo;
    PrimitiveInfo.StartIndex = StartIndex;
    PrimitiveInfo.EndIndex = EndIndex;
    PrimitiveInfo.PrimitiveType = primitive_data.PrimitiveType;

    mIndicesDataPtr->PrimitivesInfo.push_back(PrimitiveInfo);
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVertIndicesDataOrganazier::CPrimitiveIndDataProc::HasRestartIndex(const PrimitiveData & primitive_data)
{
    if (mIndicesDataPtr->PrimitivesInfo.empty()) return false;
    
    MeshPrimitiveType PrevType = mIndicesDataPtr->PrimitivesInfo.back().PrimitiveType;
    MeshPrimitiveType NextType = primitive_data.PrimitiveType;

    return ((PrevType == NextType) && CVertIndicesDataOrganazier::mPrimitiveRestartIdxFlagsTab[NextType]);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CVertIndicesDataOrganazier::CPrimitiveIndDataProc::SetRestartIndex(uint32t start_idx)
{
    void *ptr = const_cast<void*>(mIndicesDataPtr->IndexArray);
    if (mIdxDataType == INDEX_UI16) {
        uint16t *IndexArray = reinterpret_cast<uint16t*>(ptr);
        IndexArray[start_idx - 1] = RESTART_INDEX_UI16;
    }
    else if (mIdxDataType == INDEX_UI32) {
        uint32t *IndexArray = reinterpret_cast<uint32t*>(ptr);
        IndexArray[start_idx - 1] = RESTART_INDEX_UI32;
    } 
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CVertIndicesDataOrganazier::CPrimitiveIndDataProc::CopyIndexData(const PrimitiveData &primitive_data, uint32t start_idx)
{
    void *ptr = const_cast<void*>(mIndicesDataPtr->IndexArray);
    size_t PrimIndCount = primitive_data.Indices.size();
    if (mIdxDataType == INDEX_UI16) {
        uint16t *IndexArray = reinterpret_cast<uint16t*>(ptr);
        for (uint32t i = 0; i < (uint32t)PrimIndCount; i++) {
            IndexArray[start_idx + i] = (uint16t)primitive_data.Indices[i];
        }
    }
    else if (mIdxDataType == INDEX_UI32) {
        uint32t *IndexArray = reinterpret_cast<uint32t*>(ptr);
        memcpy(&IndexArray[start_idx], &primitive_data.Indices.front(), PrimIndCount*sizeof(uint32t));
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//VertIndDataOrganizer
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
void CVertIndicesDataOrganazier::AppendPrimitiveDataIndices(MeshPrimitiveType mode, uint32t *indices, uint32t idxcount)
{
    mIndicesDataList.push_back(PrimitiveData());
    PrimitiveData &primitive_data = mIndicesDataList.back();
    primitive_data.PrimitiveType = mode;
    primitive_data.Indices.assign(indices, indices + idxcount);

#ifdef __CPLUSPLUS_VER_11
    uint32t maxval = mVertCount;
    std::vector<uint32t>::const_iterator it =
        std::find_if(primitive_data.Indices.begin(), primitive_data.Indices.end(),
            [this](uint32t index) {return (index >= this->mVertCount) ? (true) : (false); });
#else
    std::vector<uint32t>::const_iterator it =
        std::find_if(primitive_data.Indices.begin(), primitive_data.Indices.end(), CVertexIndexCheckerFunctor(mVertCount));
#endif //__CPLUSPLUS_VER_11

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

#ifdef __CPLUSPLUS_VER_11
    mIndicesDataList.sort([](const PrimitiveData &obj1, const PrimitiveData &obj2) { return obj1.PrimitiveType < obj2.PrimitiveType; });
#else
    mIndicesDataList.sort(CPrimitiveSortDataFunctor());
#endif //#ifdef __CPLUSPLUS_VER_11

    CVertIndicesDataOrganazier::IndicesData *pIndicesData = new CVertIndicesDataOrganazier::IndicesData();
    uint32t IndexCount = mIndexCount;

    for (uint32t i = 0; i < MESH_PRIMITIVE_TYPE_COUNT; i++) {
        if (mPrimitiveRestartIdxFlagsTab[i] && mMeshPrimitivesCount[i] > 1)
            IndexCount += mMeshPrimitivesCount[i] - 1;
    }
    
    //IndexCount += (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_LINE_STRIP] > 1) ? (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_LINE_STRIP] - 1) : (0);
    //IndexCount += (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_TRIANGLE_STRIP] > 1) ? (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_TRIANGLE_STRIP] - 1) : (0);
    //IndexCount += (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_TRIANGLE_FAN] > 1) ? (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_TRIANGLE_FAN] - 1) : (0);
    //IndexCount += (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_QUAD_STRIP] > 1) ? (mMeshPrimitivesCount[MESH_PRIMITIVE_TYPE_QUAD_STRIP] - 1) : (0);

    pIndicesData->IndexCount = IndexCount;
    if (IndexCount >= 0xffff)
        pIndicesData->IndexArray = new uint32t[IndexCount];
    else
        pIndicesData->IndexArray = new uint16t[IndexCount];

    IndexDataType IdxType = (IndexCount > 0xffff) ? (INDEX_UI32) : (INDEX_UI16);
    std::for_each(mIndicesDataList.begin(), mIndicesDataList.end(), CPrimitiveIndDataProc(pIndicesData, IdxType));
    
    return pIndicesData;
}
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//CMeshData
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
Binormals(NULL),

PrimitiveStartIndex(0),
PrimitiveEndIndex(0),
PrimitiveType(MESH_PRIMITIVE_TYPE_COUNT)
{
    mVertices = new CPoint3[mVertexCount];
    Vertices = mVertices;

    memset(mTexCoords, 0, sizeof(float*)*MAX_TEX_COORD_ARRAYS_COUNT);
    memset(mTexCoordsSizes, 0, sizeof(uint32t)*MAX_TEX_COORD_ARRAYS_COUNT);
    
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
void CMeshData::SetPrimitiveData(MeshPrimitiveTypeInfo *priminfo, uint32t count)
{
    ReleaseIndexArray();
    mPrimitivesTypeInfo.clear();
    mPrimitivesTypeInfo.insert(mPrimitivesTypeInfo.begin(), priminfo, priminfo + count);
    //Sort Primitive By Type
#ifdef __CPLUSPLUS_VER_11
    std::sort(mPrimitivesTypeInfo.begin(), mPrimitivesTypeInfo.end(), 
        [](const MeshPrimitiveTypeInfo &obj1, const MeshPrimitiveTypeInfo &obj2)-> bool {
        return obj1.PrimitiveType < obj2.PrimitiveType; } );
#else
    std::sort(mPrimitivesTypeInfo.begin(), mPrimitivesTypeInfo.end(), CMeshData::CPrimitiveSortDataFunctor());
#endif //__CPLUSPLUS_VER_11

}
/////////////////////////////////////////////////////////////////////////////////////////////
//remove prev primitive data and index buffer
void CMeshData::CreateIndicesData(CVertIndicesDataOrganazier &IndDataOrg)
{
    ReleaseIndexArray();
    CVertIndicesDataOrganazier::IndicesData *IndData = IndDataOrg.BuildIndicesData();
    if (IndData == NULL) return;

    //Getting Index Count
    mIndexCount = IndData->IndexCount;

    //Getting Pointer To Index Array
    void *ptr = const_cast<void*>(IndData->IndexArray);
    if (mIndexCount >= 0xffff) {
        mIndexData32 = reinterpret_cast<uint32t*>(ptr);
        IndicesUI32 = reinterpret_cast<const uint32t*>(ptr);
    }
    else {
        mIndexData16 = reinterpret_cast<uint16t*>(ptr);
        IndicesUI16 = reinterpret_cast<const uint16t*>(ptr);
    }

    //Copy Primitive Info Data
    mPrimitivesTypeInfo = IndData->PrimitivesInfo;

    delete (IndData);
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

    coords_size = std::max(std::min(coords_size, uint32t(4)), uint32t(1));
    
    mTexCoords[free_pos] = new float[mVertexCount*coords_size];
    mTexCoordsSizes[free_pos] = coords_size;

    TexCoords[free_pos] = mTexCoords[free_pos];
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ResetMeshPrimitiveIterator()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::NextPrimitive()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::NextPrimitiveType()
{
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

#ifndef __CPLUSPLUS_VER_11
/////////////////////////////////////////////////////////////////////////////////////////////
// Functors
/////////////////////////////////////////////////////////////////////////////////////////////
bool CMeshData::CPrimitiveSortDataFunctor::operator() (MeshPrimitiveTypeInfo &obj1, MeshPrimitiveTypeInfo &obj2)
{
    return obj1.PrimitiveType < obj2.PrimitiveType;
}
/////////////////////////////////////////////////////////////////////////////////////////////
#endif //__CPLUSPLUS_VER_11
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


