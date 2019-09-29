/////////////////////////////////////////////////////////////////////////////////////////////
// md2data.h
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "platformtypes.h"
#include "Renderapi.h"
#include "Models/Mesh/MeshData.h"

struct MD2ModelData;

/////////////////////////////////////////////////////////////////////////////////////////////
class md2data
{
public:
    md2data(const MD2ModelData *mdldata);
    ~md2data();

    uint GetVerticesNum() const {return m_VertNum;}
    uint GetIndicesNum() const {return m_IndNum;}
    uint GetFramesNum() const {return m_FramesNum;}
    uint GetSkinsNum() const {return m_SkinsNum;}
    const float *GetFrameVerts(uint frameidx) const {return (frameidx >= m_FramesNum) ? (NULL): (m_pVertices + m_VertNum*3*frameidx);}
    const uint16t *GetIndices() const {return m_pIndices;}
    const char *GetSkinName(uint skinidx) const {return (skinidx >= m_SkinsNum || !m_SkinsNum) ? (NULL) : (m_pSkinNames + skinidx*64);}
    const char *GetFrameName(uint nameidx) const  {return (nameidx >= m_FramesNum || !m_FramesNum) ? (NULL) : (m_pFrameNames + nameidx*16);}

    const CMeshData *GetMeshData() const {return &m_MeshData;}

protected:
    void SetMeshData(const MD2ModelData *mdldata);

    uint m_VertNum;
    float *m_pVertices;

    uint m_IndNum;
    uint16t *m_pIndices;

    uint m_FramesNum;
    char *m_pFrameNames;

    uint m_SkinsNum;
    char *m_pSkinNames;

    CMeshData m_MeshData;
};

class md2renddata
{
public:
    uint VertNum;
    uint IndNum;

    uint VertexBufferIdx;
    uint IndexBufferIdx;
    uint TexCoordBufferIdx;
    uint ColorBufferIdx;

    float m_ModelViewProjMtx[16];
};

/////////////////////////////////////////////////////////////////////////////////////////////
void Setmd2RendData(md2renddata *rnddata, const md2data *pdata, IGraphicApi *gapi);
/////////////////////////////////////////////////////////////////////////////////////////////
void Rendermd2RendData(md2renddata *rnddata, IGraphicApi *gapi, uint shaderidx, uint flags);
/////////////////////////////////////////////////////////////////////////////////////////////
void Releasemd2RendData(md2renddata *rnddata, IGraphicApi *gapi);
/////////////////////////////////////////////////////////////////////////////////////////////