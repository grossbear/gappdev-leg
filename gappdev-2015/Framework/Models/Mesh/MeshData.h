/////////////////////////////////////////////////////////////////////////////////////////////
// MeshData.h
//
// Mesh Data Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MESHDATA_H__
#define __MESHDATA_H__

#include "platformtypes.h"
#include <vector>

#define NO_SMOOTH_GEN_FLAG          0x1
#define NO_INDEX_BUFFER_GEN_FLAG    0x2

#define ARRAY_NAME_SIZE     64


/////////////////////////////////////////////////////////////////////////////////////////////
class CPoint3
{
public:
    CPoint3():x(0),y(0),z(0){}
    CPoint3(float xv, float yv, float zv):x(xv),y(yv),z(zv){}

    float x,y,z;
};

/////////////////////////////////////////////////////////////////////////////////////////////
class CNormal3
{
public:
    CNormal3():x(0),y(0),z(0){}
    CNormal3(float nx, float ny, float nz):x(nx),y(ny),z(nz){}

    float x,y,z;
};


/////////////////////////////////////////////////////////////////////////////////////////////
class CStripInfo
{
    uint startindex;
    uint vertsnum;
};


/////////////////////////////////////////////////////////////////////////////////////////////
class CFace
{
public:
    CFace():vidx1(0),vidx2(0),vidx3(0),smoothgroup(0){}
    CFace(uint v1, uint v2, uint v3, uint smgroup):
    vidx1(v1),vidx2(v2),vidx3(v3),smoothgroup(smgroup){}

    uint vidx1,vidx2,vidx3;
    ubyte smoothgroup;
};

/////////////////////////////////////////////////////////////////////////////////////////////
class CFaceDataArray
{
public:
    std::string strname;
    VARDATATYPE vartype;
    uint size;
    ubyte *data;
};



/////////////////////////////////////////////////////////////////////////////////////////////
// Main Mesh Data Store Class
class CMeshData
{
public:
    CMeshData();
    CMeshData(const CMeshData &mesh);
    virtual ~CMeshData();

    void SetVertNum(uint num);
    uint GetVertsNum() const;
    void ReleaseData();

    void SetVertPos(uint vertid, float x, float y, float z);
    void SetVertPos(uint vertid, float pos[3]);

    void GetVertPos(uint vertid, float &x, float &y, float &z);
    void GetVertPos(uint vertid, float pos[]);
    const CPoint3* GetVerts() const;
    
    void AddFace(uint v1, uint v2, uint v3, ubyte smgroup=0);
    void ReleaseFaces();
    uint GetFacesNum() const;
    const CFace* GetFaces() const;

    void CalcFaceNormals();
    void ReleaseFaceNormals();
    const CNormal3* GetFaceNormals() const;

    uint AddFacesDataArray(const char *name, VARDATATYPE vtype, uint size);
    void SetFaceVertData(uint arrayid, uint faceidx, uint facevertidx, const void *data);
    void ReleaseDataArrays();

    void StartTriStripGen();
    void SetTriStripVert(uint vid);
    void EndTriStripGen();
    void ReleaseTriStripData();

    void BuildRenderMesh(uint genflag=0x0);

protected:
    void BuildMeshConnData();
    void BuildVertsFacesRel();
    void SortFacesRelByEdges();
    bool IsDataArrayNameExist(const char *name);


    std::vector<CPoint3> m_Verts;
    std::vector<CFace> m_Faces;
    std::vector<CNormal3> m_FacesNormals;

    std::vector<std::vector<uint> > m_VertFacesRel;

    std::vector<uint> m_StripIndices;
    std::vector<CStripInfo> m_Strips;

    std::vector<CFaceDataArray> m_FaceDataArrVec;

private:
    bool HasCommonEdge(const CFace &face1, const CFace &face2, uint vertidx);
    bool IsSameFace(const CFace &face, uint vidx1, uint vidx2, uint vidx3);
};
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__MESHDATA_H__