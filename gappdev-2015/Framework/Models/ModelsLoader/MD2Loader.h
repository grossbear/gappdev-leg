/////////////////////////////////////////////////////////////////////////////////////////////
// DM2Loader.h
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DM2LOADER_H__
#define __DM2LOADER_H__

#include "platformtypes.h"
#include "Utils/IOStream/IDataStream.h"

enum md2primtype
{
    md2tri = 0,
    md2tristrip,
    md2trifan,
};

typedef struct
{
    md2primtype type;
    uint startidx;
    uint vtxcount;
} md2primdesc_t;

struct MD2ModelData
{
    uint    VerticesNum;        // vertex count
    uint    FramesNum;          // frames count
    float   *pVertices;         // vertex data (vertex count * 3)
    uint    TriIndNum;          // triangles indices count
    uint16t *pTriIndices;       // triangle indices data (triangles indices count * 3)
    float   *pTexCoords;        // texture coords data (tri indices count * 3 * 2)
    char    *pFrameNames;       // frames names (each frame string length = 16 bytes * frames count)
    uint    SkinsNum;           // skins count
    char    *pSkinNames;        // skin names (each name string length = 64 bytes * skin name count

    float   *pCmdsTexCoords;    // opengl commands texture coords (indices count * 2)
    uint16t *pCmdsTriIndices;   // opengl commands triangles indices
    uint    CmdsTriNum;         // commands triangles indices count

    md2primdesc_t *pPrimDesc;   // primitives types description data array
    uint    PrimDescNum;        // primitive description elements count
};

/////////////////////////////////////////////////////////////////////////////////////////////
class CMD2Loader
{
public:
    CMD2Loader();
    ~CMD2Loader();

    const MD2ModelData* LoadModel(const char *filename);
    void ReleaseModelData();

protected:
    bool ReadAndCheckFileHeader(IDataStream *stream, void *header);
    MD2ModelData* ReadData(IDataStream *stream, void *header);

private:
    void SetModelDataStruct(MD2ModelData *mdldata, void *header);
    unsigned char *ReadModelData(IDataStream *stream, uint size);

    void SetSkinNames(char *pskin, const unsigned char *names, int namesize);

    void SetVertices(float *pvtx, const unsigned char *buffer, int vertexnum, int framesnum, int framesize);
    void SetFrameNames(char *pframenames, const unsigned char *buffer, uint framesnum, uint framesize);

    float *GetCoordData(const unsigned char *buffer, int numst, int skinwidth, int skinheight);
    void SetTriData(MD2ModelData *mdldata, const unsigned char *buffer, const float *coords);

    void SetTriPrimType(MD2ModelData *mdldata, const unsigned char *buffer, int cmdsize);

    MD2ModelData *m_pModelData;
};


/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__DM2LOADER_H__