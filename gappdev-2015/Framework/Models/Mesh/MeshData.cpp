/////////////////////////////////////////////////////////////////////////////////////////////
// MeshData.cpp
//
// Mesh Data Class
/////////////////////////////////////////////////////////////////////////////////////////////

#include "MeshData.h"
#include "Math/MathLib.h"

#include <string>

#define FALSE_INDEX     UINT_MAX_VAL


/////////////////////////////////////////////////////////////////////////////////////////////
// Main Mesh Data Store Class
/////////////////////////////////////////////////////////////////////////////////////////////
CMeshData::CMeshData()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
CMeshData::CMeshData(const CMeshData &mesh)
{
    m_Verts = mesh.m_Verts;
    m_Faces = mesh.m_Faces;
    m_FacesNormals = mesh.m_FacesNormals;

    m_StripIndices = mesh.m_StripIndices;
    m_Strips = mesh.m_Strips;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CMeshData::~CMeshData()
{
    ReleaseData();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::SetVertNum(uint num)
{
    m_Verts.resize(num);
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint CMeshData::GetVertsNum() const
{
    return (uint)m_Verts.size();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseData()
{
    m_Verts.clear();
    m_Faces.clear();
    m_FacesNormals.clear();

    m_StripIndices.clear();
    m_Strips.clear();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::SetVertPos(uint vertid, float x, float y, float z)
{
    m_Verts[vertid].x = x;
    m_Verts[vertid].y = y;
    m_Verts[vertid].z = z;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::SetVertPos(uint vertid, float pos[3])
{
    m_Verts[vertid].x = pos[0];
    m_Verts[vertid].y = pos[1];
    m_Verts[vertid].z = pos[2];
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::GetVertPos(uint vertid, float &x, float &y, float &z)
{
    x = m_Verts[vertid].x;
    y = m_Verts[vertid].y;
    z = m_Verts[vertid].z;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::GetVertPos(uint vertid, float pos[])
{
    pos[0] = m_Verts[vertid].x;
    pos[1] = m_Verts[vertid].y;
    pos[2] = m_Verts[vertid].z;
}
/////////////////////////////////////////////////////////////////////////////////////////////
const CPoint3* CMeshData::GetVerts() const
{
    return &m_Verts[0];
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::AddFace(uint v1, uint v2, uint v3, ubyte smgroup)
{
    CFace face(v1,v2,v3,smgroup);
    m_Faces.push_back(face);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseFaces()
{
    m_Faces.clear();
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint CMeshData::GetFacesNum() const
{
    return (uint)m_Faces.size();
}
/////////////////////////////////////////////////////////////////////////////////////////////
const CFace* CMeshData::GetFaces() const
{
    return &m_Faces[0];
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::CalcFaceNormals()
{
    if(m_FacesNormals.empty()) 
        m_FacesNormals.reserve(m_Faces.size());

    for(uint i = 0; i < (uint)m_Faces.size(); i++) 
    {
        const CFace &face = m_Faces[i];
        const CPoint3 &point1 = m_Verts[face.vidx1];
        const CPoint3 &point2 = m_Verts[face.vidx2];
        const CPoint3 &point3 = m_Verts[face.vidx3];

        cvec3f vec1(point2.x - point1.x, point2.y - point1.y, point2.z - point1.z);
        cvec3f vec2(point3.x - point1.x, point3.y - point1.y, point3.z - point1.z);

        cvec3f normvec;
        CMVec3Cross(normvec,vec1,vec2);
        CMVec3Normalize(normvec,normvec);

        m_FacesNormals[i].x = normvec.x;
        m_FacesNormals[i].y = normvec.y;
        m_FacesNormals[i].z = normvec.z;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseFaceNormals()
{
    m_FacesNormals.clear();
}
/////////////////////////////////////////////////////////////////////////////////////////////
const CNormal3* CMeshData::GetFaceNormals() const
{
    return &m_FacesNormals[0];
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint CMeshData::AddFacesDataArray(const char *name, VARDATATYPE vtype, uint size)
{
    if(name==NULL) return FALSE_INDEX;
    if(name[0]==0) return FALSE_INDEX;
    
    if(!m_FaceDataArrVec.empty()) {
        if(IsDataArrayNameExist(name)) return FALSE_INDEX;
    }

    CFaceDataArray facedata;
    facedata.vartype = vtype;
    //setting array name
    facedata.strname = std::string(name);
    facedata.size = size;
    facedata.data = new ubyte[uint(m_Faces.size())*vartypesizestab[vtype]*size*3];
    if(facedata.data == NULL) return FALSE_INDEX;
    memset(facedata.data,0x0,uint(m_Faces.size())*vartypesizestab[vtype]*size*3);

    m_FaceDataArrVec.push_back(facedata);

    return (uint)(m_FaceDataArrVec.size()-1);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::SetFaceVertData(uint arrayid, uint faceidx, uint facevertidx, const void *data)
{
    CFaceDataArray facedata = m_FaceDataArrVec[arrayid];
    ubyte *ptr = facedata.data;
    ptr += faceidx*vartypesizestab[facedata.vartype]*facedata.size*3 + vartypesizestab[facedata.vartype]*facedata.size*facevertidx;
    memcpy(ptr,data,vartypesizestab[facedata.vartype]*facedata.size);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseDataArrays()
{
    for(size_t i = 0; i < m_FaceDataArrVec.size(); i++) 
    {
        CFaceDataArray &vecelem = m_FaceDataArrVec[i];
        delete [] vecelem.data;
    }
    m_FaceDataArrVec.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::StartTriStripGen()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::SetTriStripVert(uint vid)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::EndTriStripGen()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::ReleaseTriStripData()
{
    m_StripIndices.clear();
    m_Strips.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::BuildRenderMesh(uint genflag)
{
    BuildMeshConnData();
}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::BuildMeshConnData()
{
    BuildVertsFacesRel();
    SortFacesRelByEdges();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::BuildVertsFacesRel()
{
    m_VertFacesRel.clear();
    m_VertFacesRel.resize(m_Verts.size());

    for(uint i = 0; i < (uint)m_Faces.size(); i++) 
    {
        const CFace &face = m_Faces[i];
        m_VertFacesRel[face.vidx1].push_back(i);
        m_VertFacesRel[face.vidx2].push_back(i);
        m_VertFacesRel[face.vidx3].push_back(i);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMeshData::SortFacesRelByEdges()
{
    /*
    //Cicle around vertex and sort faces 
    for(uint vidx = 0; vidx < (uint)m_VertFacesRel.size(); vidx++) 
    {
        if(m_VertFacesRel[vidx].empty()) continue;
        //Get unsorted faces indices
        std::vector<uint> facesidx = m_VertFacesRel[vidx];
        if(facesidx.size() == 1) continue;

        //New sorted faces indices vector
        std::vector<uint> newfacesidx;
        //Put last face index into new vector and start searching next neighbour face
        newfacesidx.push_back(facesidx[facesidx.back()]);
        //Remove last face index from unsorted vector
        facesidx.pop_back();
        while(!facesidx.empty()) 
        {
            ////////////////////////////////
            //If false face index put last face index from unsorted vector
            if(newfacesidx.back() == FALSE_INDEX) {
                newfacesidx.push_back(facesidx[facesidx.back()]);
                facesidx.pop_back();
                continue;
            }
            ////////////////////////////////
            const CFace &baseface = m_Faces[newfacesidx.back()];
            bool bcomm = false;
            //Search for neighbour face with common edge
            for(uint k = 0; k < facesidx.size(); k++) {
                const CFace &face = m_Faces[ facesidx[k] ];
                bcomm = HasCommonEdge(baseface,face, vidx);
                if(bcomm) {
                    newfacesidx.push_back(facesidx[k]);
                    facesidx.erase(facesidx.begin()+k);
                    break;
                }
            }
            //If no neighbour face put false index and continue searching faces
            if(!bcomm) {
                newfacesidx.push_back(FALSE_INDEX);
            }
        }

        //Check if last face and last face has common edge or set false index
        if(newfacesidx.back() != FALSE_INDEX) {
            const CFace &stface = m_Faces[ newfacesidx.front() ];
            const CFace &lsface = m_Faces[ newfacesidx.back() ];
            bool bcomm = HasCommonEdge(stface,lsface, vidx);
            if(!bcomm) newfacesidx.push_back(FALSE_INDEX);
        }
        ////////////////////////////////
        m_VertFacesRel[vidx] = newfacesidx;
    }
    ////////////////////////////////
    */
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CMeshData::IsDataArrayNameExist(const char *name)
{
    if(name==NULL) return false;
    std::string newname(name);
    if(newname.empty()) return false;
    
    for(size_t i = 0; i < m_FaceDataArrVec.size(); i++) {
        CFaceDataArray &elem = m_FaceDataArrVec[i];
        if(newname == elem.strname) return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
bool CMeshData::HasCommonEdge(const CFace &face1, const CFace &face2, uint vertidx)
{
    uint vidxtab1[3] = {face1.vidx1,face1.vidx2,face1.vidx3};
    uint vidxtab2[3] = {face2.vidx1,face2.vidx2,face2.vidx3};
    
    uint nextpts1[2] = {-1,-1};
    uint nextpts2[2] = {-1,-1};

    for(uint i = 0; i < 3; i++) 
    {
        if(vertidx != vidxtab1[i]) {
            if(nextpts1[0] == (-1)) nextpts1[0] = vidxtab1[i];
            else nextpts1[1] = vidxtab1[i];
        }

        if(vertidx != vidxtab2[i]) {
            if(nextpts2[0] == (-1)) nextpts2[0] = vidxtab2[i];
            else nextpts2[1] = vidxtab2[i];
        }
    }

    bool bres1 = (nextpts1[0] == nextpts2[0]) || (nextpts1[0] == nextpts2[1]);
    bool bres2 = (nextpts1[1] == nextpts2[0]) || (nextpts1[1] == nextpts2[1]);

    return bres1 || bres2;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CMeshData::IsSameFace(const CFace &face, uint vidx1, uint vidx2, uint vidx3)
{
    bool bexist1 = (vidx1 == face.vidx1) || (vidx1 == face.vidx2) || (vidx1 == face.vidx3);
    bool bexist2 = (vidx2 == face.vidx1) || (vidx2 == face.vidx2) || (vidx2 == face.vidx3);
    bool bexist3 = (vidx3 == face.vidx1) || (vidx3 == face.vidx2) || (vidx3 == face.vidx3);

    //uint fvtab[] = {face.vidx1, face.vidx2, face.vidx3};
    //std::vector<uint> facevtx(fvtab, fvtab + sizeof(fvtab)/sizeof(uint));
    //uint *p1 = std::find(fvtab, fvtab + 3, vidx1);
    //uint *p2 = std::find(fvtab, fvtab + 3, vidx1);
    //uint *p3 = std::find(fvtab, fvtab + 3, vidx1);

    return bexist1 && bexist2 && bexist3;
}

/////////////////////////////////////////////////////////////////////////////////////////////