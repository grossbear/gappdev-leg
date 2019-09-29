/////////////////////////////////////////////////////////////////////////////////////////////
// md2data.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#include "md2data.h"
#include "Models/ModelsLoader/MD2Loader.h"


/////////////////////////////////////////////////////////////////////////////////////////////
md2data::md2data(const MD2ModelData *mdldata):
m_VertNum(0),
m_pVertices(NULL),
m_IndNum(0),
m_pIndices(NULL),
m_FramesNum(0),
m_pFrameNames(NULL),
m_SkinsNum(0),
m_pSkinNames(NULL)
{
    if(mdldata == NULL) return ;

    m_VertNum = mdldata->VerticesNum;
    m_FramesNum = mdldata->FramesNum;
    m_pVertices = new float[m_VertNum*m_FramesNum*3];
    memcpy(m_pVertices,mdldata->pVertices,m_VertNum*m_FramesNum*3*sizeof(float));

    m_IndNum = mdldata->TriIndNum*3;
    m_pIndices = new uint16t[m_IndNum];
    memcpy(m_pIndices,mdldata->pTriIndices,sizeof(uint16t)*m_IndNum);

    if(m_FramesNum > 0) {
        m_pFrameNames = new char[m_FramesNum*16];
        memcpy(m_pFrameNames,mdldata->pFrameNames,sizeof(char)*m_FramesNum);
    }

    if(m_SkinsNum > 0) {
        m_pSkinNames = new char[m_SkinsNum*64];
        memcpy(m_pSkinNames,mdldata->pSkinNames,sizeof(char)*m_SkinsNum);
    }

    

    SetMeshData(mdldata);
}

/////////////////////////////////////////////////////////////////////////////////////////////
md2data::~md2data()
{
    delete [] m_pSkinNames;
    delete [] m_pFrameNames;
    delete [] m_pIndices;
    delete [] m_pVertices;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void md2data::SetMeshData(const MD2ModelData *mdldata)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
void Setmd2RendData(md2renddata *rnddata, const md2data *pdata, IGraphicApi *gapi)
{
    memset(rnddata,0,sizeof(md2renddata));
    
    rnddata->VertNum = pdata->GetVerticesNum();
    rnddata->VertexBufferIdx = gapi->CreateVertexBuffer(GAPI_BUFUSAGE_DYNAMIC,rnddata->VertNum*3*sizeof(float),pdata->GetFrameVerts(0));
    
    rnddata->IndNum = pdata->GetIndicesNum();
    rnddata->IndexBufferIdx = gapi->CreateVertexBuffer(GAPI_BUFUSAGE_DYNAMIC,rnddata->IndNum*sizeof(uint16t),pdata->GetIndices());
    
    rnddata->m_ModelViewProjMtx[0] = rnddata->m_ModelViewProjMtx[5] = 
        rnddata->m_ModelViewProjMtx[10] = rnddata->m_ModelViewProjMtx[15] = 1.f;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void Rendermd2RendData(md2renddata *rnddata, IGraphicApi *gapi, uint shaderidx, uint flags)
{
    uint vertid = gapi->GetVertexAttribLocation(shaderidx,"vVertex");
    gapi->SetVertexAttribBuffer(vertid,3,GAPI_FLOAT,false,0,0,rnddata->VertexBufferIdx);
    gapi->SetIndexBuffer(rnddata->IndexBufferIdx);
    gapi->SetShader(shaderidx);
    gapi->SetShaderUniform(GAPI_UNIFORM_MTX44F,"TransformMtx",rnddata->m_ModelViewProjMtx);

    gapi->DrawElements(GAPI_TRIANGLES,rnddata->IndNum,GAPI_UINT16,NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void Releasemd2RendData(md2renddata *rnddata, IGraphicApi *gapi)
{
    gapi->DeleteVertexBuffer(rnddata->VertexBufferIdx);
    gapi->DeleteVertexBuffer(rnddata->IndexBufferIdx);
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