/////////////////////////////////////////////////////////////////////////////////////////////
// DM2Loader.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#include "MD2Loader.h"
#include "Utils/IOStream/FileStream.h"

#define FRAME_NAME_SIZE 16
#define SKIN_NAME_SIZE  64

/////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
// MD2 file header struct
typedef struct
{
    int ident;      // "IDP2" format ident MD2 file
    int version;    // version 8
    int skinWidth;  // texture width
    int skinHeight; // texture height
    int frameSize;  // frame size (bytes)
    int numSkins;   // texture count
    int numXYZ;     // points count
    int numST;      // texture coords 
    int numTris;    // triangles count
    int numGLcmds;  // OpenGL commends count
    int numFrames;  // frames count
    int offsetSkins;// skin offset in file (each 64 bytes)
    int offsetST;   // texure coords offset
    int offsetTris; // triangles indices offset
    int offsetFrames;//frames offset
    int offsetGLcmds;//OpenGL commands offset
    int offsetEnd;  // end of file
} md2modelheader_t;

// single point texture coord
typedef struct
{
    float s;    // s-coord
    float t;    // t-coord
} texCoord_t;

// texture coord index
typedef struct
{
    short s;
    short t;
} stIndex_t;

// single animation point data
typedef struct
{
    unsigned char vtx[3];       //position of the point
    unsigned char normalIndex;  //not used
} framepoint_t;

// single frame animation data
typedef struct
{
    float scale[3];     // points scale coeff
    float translate[3]; // points translation 
    char name[16];      // name of the model
    framepoint_t fp[1]; // frame points list
} frame_t;

// single triangle description data
typedef struct
{
    unsigned short meshIndex[3];    // vertex indices
    unsigned short stIndex[3];      // tex coord indices
} tri_t;

// single gl command vertex
typedef struct
{
   float s, t;
   int vertexIndex;
} glCommandVertex_t;

// animation
typedef struct
{
    int     first_frame;            // first frame of the animation
    int     last_frame;             // number of frames
    int     fps;                    // number of frames per second

} anim_t;

#pragma pack(pop)
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
inline void SetBuffersSizes(md2modelheader_t *pheader, uint &skindatasize, uint &coordsize, 
                            uint &tridatasize, uint &frmdatasize, uint &cmddatasize)
{
    skindatasize = pheader->offsetST - pheader->offsetSkins;
    coordsize = pheader->offsetTris - pheader->offsetST;
    tridatasize = pheader->offsetFrames - pheader->offsetTris;
    frmdatasize = pheader->offsetGLcmds - pheader->offsetFrames;
    cmddatasize = pheader->offsetEnd - pheader->offsetGLcmds;
}
/////////////////////////////////////////////////////////////////////////////////////////////
inline void ReleaseBuffers( unsigned char *skinbuffer,
                            unsigned char *coordbuffer,
                            unsigned char *tribuffer,
                            unsigned char *frmbuffer,
                            unsigned char *cmdbuffer,
                            float *coords)
{
    delete [] skinbuffer;
    delete [] coordbuffer;
    delete [] tribuffer;
    delete [] frmbuffer;
    delete [] cmdbuffer;
    delete [] coords;
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
CMD2Loader::CMD2Loader():m_pModelData(NULL)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
CMD2Loader::~CMD2Loader()
{
    ReleaseModelData();
}
/////////////////////////////////////////////////////////////////////////////////////////////
const MD2ModelData* CMD2Loader::LoadModel(const char *filename)
{
    if(m_pModelData) ReleaseModelData();
    if(filename==NULL) return NULL;

    CFileStream file;
    file.Open(filename,open_read);
    if(!file.IsOpened()) return NULL;

    md2modelheader_t header={0};
    bool bcheck = ReadAndCheckFileHeader(&file,&header);
    if(bcheck == false) 
    {
        file.Close();
        return NULL;
    }

    MD2ModelData *mdldata = ReadData(&file,&header);
    m_pModelData = mdldata;

    file.Close();
    return m_pModelData;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMD2Loader::ReleaseModelData()
{
    if(!m_pModelData) return;
    delete [] m_pModelData->pSkinNames;
    delete [] m_pModelData->pTexCoords;
    delete [] m_pModelData->pTriIndices;
    delete [] m_pModelData->pVertices;
    delete [] m_pModelData->pCmdsTexCoords;
    delete [] m_pModelData->pCmdsTriIndices;
    delete [] m_pModelData->pPrimDesc;
    delete (m_pModelData);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CMD2Loader::ReadAndCheckFileHeader(IDataStream *stream, void *phdr)
{
    md2modelheader_t *pheader = static_cast<md2modelheader_t*>(phdr);
    uint readsize = stream->Read(pheader,sizeof(md2modelheader_t));
    if(readsize != sizeof(md2modelheader_t))
        return false;

    int cmp = memcmp(&pheader->ident,"IDP2",sizeof(char)*4);
    if(cmp != 0)
        return false;

    if(pheader->version != 8)
        return false;

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
MD2ModelData* CMD2Loader::ReadData(IDataStream *stream, void *phead)
{
    md2modelheader_t *pheader = static_cast<md2modelheader_t*>(phead);
    MD2ModelData *md2modeldata = new MD2ModelData();

    // initializing model data struct
    SetModelDataStruct(md2modeldata,phead);

    // getting buffers sizes
    uint skindatasize,coordsize,tridatasize,frmdatasize,cmddatasize;
    SetBuffersSizes(pheader,skindatasize,coordsize,tridatasize,frmdatasize,cmddatasize);
    
    // reading data from file
    unsigned char *skinbuffer = ReadModelData(stream,skindatasize);
    unsigned char *coordbuffer = ReadModelData(stream,coordsize);
    unsigned char *tribuffer = ReadModelData(stream,tridatasize);
    unsigned char *frmbuffer = ReadModelData(stream,frmdatasize);
    unsigned char *cmdbuffer = ReadModelData(stream,cmddatasize);

    // copying skin names data
    SetSkinNames(md2modeldata->pSkinNames, skinbuffer, skindatasize);
    
    // getting vertices data
    SetVertices(md2modeldata->pVertices,frmbuffer,pheader->numXYZ,pheader->numFrames,pheader->frameSize);
    // reading frame names
    SetFrameNames(md2modeldata->pFrameNames,frmbuffer,pheader->numFrames,pheader->frameSize);

    // reading texture coords data
    float *coords = GetCoordData(coordbuffer,pheader->numST,pheader->skinWidth,pheader->skinHeight);
    
    // reading triangle indices and setting texture coords
    SetTriData(md2modeldata,tribuffer,coords);

    // setting triangles types
    SetTriPrimType(md2modeldata,cmdbuffer,cmddatasize);

    // releasing buffers
    ReleaseBuffers(skinbuffer,coordbuffer,tribuffer,frmbuffer,cmdbuffer,coords);


    return md2modeldata;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMD2Loader::SetModelDataStruct(MD2ModelData *mdldata, void *header)
{
    md2modelheader_t *pheader = static_cast<md2modelheader_t*>(header);

    mdldata->FramesNum = (uint)pheader->numFrames;
    mdldata->VerticesNum = (uint)pheader->numXYZ;
    mdldata->TriIndNum = (uint)pheader->numTris;
    mdldata->SkinsNum = (uint)pheader->numSkins;

    mdldata->pFrameNames = new char[mdldata->FramesNum*FRAME_NAME_SIZE];
    memset(mdldata->pFrameNames,0,sizeof(char)*mdldata->FramesNum*FRAME_NAME_SIZE);

    mdldata->pSkinNames = NULL;
    if(mdldata->SkinsNum != 0)
    {
        mdldata->pSkinNames = new char[mdldata->SkinsNum*SKIN_NAME_SIZE];
        memset(mdldata->pSkinNames,0,sizeof(char)*mdldata->SkinsNum*SKIN_NAME_SIZE);
    }

    mdldata->pVertices = new float[mdldata->VerticesNum * 3 * mdldata->FramesNum];
    mdldata->pTriIndices = new uint16t[mdldata->TriIndNum * 3];
    mdldata->pTexCoords = new float[mdldata->TriIndNum * 3 * 2];

    mdldata->pCmdsTexCoords = NULL;
    mdldata->pCmdsTriIndices = NULL;
    mdldata->CmdsTriNum = 0;
    mdldata->pPrimDesc = NULL;
    mdldata->PrimDescNum = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
unsigned char *CMD2Loader::ReadModelData(IDataStream *stream, uint size)
{
    if(size == 0) return NULL;

    unsigned char *buffer = new unsigned char [size];
    uint readed = stream->Read(buffer,size);
    
    if(readed != size) 
    {
        delete [] buffer;
        return NULL;
    }

    return buffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMD2Loader::SetSkinNames(char *pskin, const unsigned char *names, int namesize)
{
    if(pskin==NULL || names==NULL || namesize==0) return;

    memcpy(pskin,names,namesize);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMD2Loader::SetVertices(float *pvertices, const unsigned char *buffer, int vertnum, int framesnum, int framesize)
{
    // looping through animation keyframes
    for(int frmidx = 0; frmidx < framesnum; frmidx++)
    {
        // getting pointer to frame description
        const frame_t *pframe = (frame_t*)&buffer[framesize * frmidx];
        float *pvtx = pvertices + vertnum*3*frmidx;
        // decoding points positions
        for(int vertidx = 0; vertidx < vertnum; vertidx++)
        {
            pvtx[0] = pframe->scale[0]*pframe->fp[vertidx].vtx[0] + pframe->translate[0];
            pvtx[1] = pframe->scale[1]*pframe->fp[vertidx].vtx[1] + pframe->translate[1];
            pvtx[2] = pframe->scale[2]*pframe->fp[vertidx].vtx[2] + pframe->translate[2];
            pvtx += 3;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMD2Loader::SetFrameNames(char *pframenames, const unsigned char *buffer, uint framesnum, uint framesize)
{
    // looping through animation keyframes
    for(uint frmidx = 0; frmidx < framesnum; frmidx++) 
    {
        // getting pointer to frame description
        const frame_t *pframe = (frame_t*)&buffer[framesize * frmidx];
        char *strname = pframenames + FRAME_NAME_SIZE*frmidx;
        memcpy(strname,pframe->name,FRAME_NAME_SIZE*sizeof(char));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
float* CMD2Loader::GetCoordData(const unsigned char *buffer, int numst, int skinwidth, int skinheight)
{
    // reading texture coords data
    stIndex_t *stdata = (stIndex_t*)&buffer[0];
    texCoord_t *coords = new texCoord_t[numst];
    float fwidth = (float)skinwidth;
    float fheight = (float)skinheight;
    for(int stidx = 0; stidx < numst; stidx++) 
    {
        coords[stidx].s = (float)stdata[stidx].s / fwidth;
        coords[stidx].t = (float)stdata[stidx].t / fheight;
    }

    return (float*)coords;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CMD2Loader::SetTriData(MD2ModelData *mdldata, const unsigned char *buffer, const float *coords)
{
    const tri_t *tridata = (tri_t*)&buffer[0];
    const texCoord_t *texcoords = (texCoord_t*)coords;
    uint16t *ptri = mdldata->pTriIndices;
    float *pcoords = mdldata->pTexCoords;
    for(uint tridx = 0; tridx < mdldata->TriIndNum; tridx++)
    {
        ptri[0] = tridata[tridx].meshIndex[0];
        ptri[1] = tridata[tridx].meshIndex[1];
        ptri[2] = tridata[tridx].meshIndex[2];

        pcoords[0] = texcoords[ tridata[tridx].stIndex[0] ].s;
        pcoords[1] = texcoords[ tridata[tridx].stIndex[0] ].t;

        pcoords[2] = texcoords[ tridata[tridx].stIndex[1] ].s;
        pcoords[3] = texcoords[ tridata[tridx].stIndex[1] ].t;

        pcoords[4] = texcoords[ tridata[tridx].stIndex[2] ].s;
        pcoords[5] = texcoords[ tridata[tridx].stIndex[2] ].t;

        ptri += 3;
        pcoords += 6;
    }

}

#include <vector>

/////////////////////////////////////////////////////////////////////////////////////////////
void CMD2Loader::SetTriPrimType(MD2ModelData *mdldata, const unsigned char *buffer, int cmdsize)
{
    int *ptricmds = (int*)buffer;

    std::vector<float> texcoordsvec;
    std::vector<uint16t> vtxidxvec;
    std::vector<md2primdesc_t> primdatavec;

    md2primtype primtype = md2tri;
    while( int i = *(ptricmds++) )
    {
        md2primtype primtype = (i<0)?(md2trifan):(md2tristrip);
        i = (i<0)?(-i):(i);

        md2primdesc_t primdesc;
        primdesc.startidx = (uint)vtxidxvec.size();
        primdesc.vtxcount = (uint)i;
        primdesc.type = primtype;
        primdatavec.push_back(primdesc);

        for( ; i > 0; i--, ptricmds += 3 )
        {
            float *scoord = (float*)&ptricmds[0];
            float *tcoord = (float*)&ptricmds[1];
            int vtxidx = ptricmds[2];
            
            texcoordsvec.push_back(*scoord);
            texcoordsvec.push_back(*tcoord);
            vtxidxvec.push_back((uint16t)vtxidx);
        }
    }

    mdldata->PrimDescNum = (uint)primdatavec.size();
    mdldata->pPrimDesc = new md2primdesc_t[mdldata->PrimDescNum];
    memcpy(mdldata->pPrimDesc,&primdatavec[0],mdldata->PrimDescNum*sizeof(md2primdesc_t));

    mdldata->CmdsTriNum = (uint)vtxidxvec.size();
    mdldata->pCmdsTriIndices = new uint16t[mdldata->CmdsTriNum];
    mdldata->pCmdsTexCoords = new float[mdldata->CmdsTriNum*2];
    memcpy(mdldata->pCmdsTriIndices,&vtxidxvec[0],mdldata->CmdsTriNum*sizeof(uint16t));
    memcpy(mdldata->pCmdsTexCoords,&texcoordsvec[0],mdldata->CmdsTriNum*2*sizeof(float));
}

/////////////////////////////////////////////////////////////////////////////////////////////

/*
// ----------------------------------------------
// initialize the 21 MD2 model animations.
// ----------------------------------------------

anim_t CMD2Model::animlist[ 21 ] = 
{
    // first, last, fps

    {   0,  39,  9 },   // STAND
    {  40,  45, 10 },   // RUN
    {  46,  53, 10 },   // ATTACK
    {  54,  57,  7 },   // PAIN_A
    {  58,  61,  7 },   // PAIN_B
    {  62,  65,  7 },   // PAIN_C
    {  66,  71,  7 },   // JUMP
    {  72,  83,  7 },   // FLIP
    {  84,  94,  7 },   // SALUTE
    {  95, 111, 10 },   // FALLBACK
    { 112, 122,  7 },   // WAVE
    { 123, 134,  6 },   // POINT
    { 135, 153, 10 },   // CROUCH_STAND
    { 154, 159,  7 },   // CROUCH_WALK
    { 160, 168, 10 },   // CROUCH_ATTACK
    { 196, 172,  7 },   // CROUCH_PAIN
    { 173, 177,  5 },   // CROUCH_DEATH
    { 178, 183,  7 },   // DEATH_FALLBACK
    { 184, 189,  7 },   // DEATH_FALLFORWARD
    { 190, 197,  7 },   // DEATH_FALLBACKSLOW
    { 198, 198,  5 },   // BOOM
};
*/

/*
// animation list
typedef enum {
    STAND,
    RUN,
    ATTACK,
    PAIN_A,
    PAIN_B,
    PAIN_C,
    JUMP,
    FLIP,
    SALUTE,
    FALLBACK,
    WAVE,
    POINT,
    CROUCH_STAND,
    CROUCH_WALK,
    CROUCH_ATTACK,
    CROUCH_PAIN,
    CROUCH_DEATH, 
    DEATH_FALLBACK,
    DEATH_FALLFORWARD,
    DEATH_FALLBACKSLOW,
    BOOM,

    MAX_ANIMATIONS

} animType_t;
*/

//http://tfc.duke.free.fr/old/models/md2.htm