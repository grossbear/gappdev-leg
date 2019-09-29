/////////////////////////////////////////////////////////////////////////////////////////////
// GLVBOSystem.cpp
//
// OpenGL Vertex Buffer Object Management Class
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#include "GLDevInfo.h"
#include "GLConvFunc.h"
#include "GLVBOSystem.h"
#include "GLShaderSystem.h"
#include "GLVerProc.h"
#include "GLArbProc.h"

//BUFFER_OFFSET is defined as #define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

/////////////////////////////////////////////////////////////////////////////////////////////
CGLVBOSystem *CGLVBOSystem::m_pInstance = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////
CGLVBOSystem* CGLVBOSystem::GetSingleton()
{
    if(m_pInstance==NULL)
        m_pInstance = new CGLVBOSystem();
    return m_pInstance;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CGLVBOSystem::CGLVBOSystem()
{
    m_ActiveStreams.Resize(32);
}
/////////////////////////////////////////////////////////////////////////////////////////////
CGLVBOSystem::~CGLVBOSystem()
{
    ReleaseVertexBuffers();
}
/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLVBOSystem::CreateVertexBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data)
{
    GLuint BufferID = 0;
    //Generating Single Buffer Object
    glGenBuffers(1,&BufferID);

    //Binding Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER_ARB, BufferID);

    GLenum BufUsage = CGLConvFunc::GetGLBufferDataUsage(usage);

    GLuint ArrayOnjectSize = bufsize;
    GLint ParamArrayObjectSize = 0;
    glBufferData(GL_ARRAY_BUFFER_ARB, ArrayOnjectSize,(const GLvoid*)NULL,BufUsage);
    glGetBufferParameteriv( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &ParamArrayObjectSize);

    if(ParamArrayObjectSize <= 0)
    {
        //error "glBufferDataARB failed to allocate any memory!"
        //class ERROR
        glDeleteBuffers(1,&BufferID);
        return INVALID_ARRAY_INDEX;
    }

    if(data && ParamArrayObjectSize > 0)
    {
	    //Mapping Buffer To Get Memory Access
	    GLvoid* DataPointer = NULL;
	    DataPointer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(DataPointer,data,bufsize);
	    glUnmapBuffer(GL_ARRAY_BUFFER);
    }

	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);

    //Adding Data Into Vertex Buffer Info Array
    unsigned int idx = m_VertBufArray.Reserve(1);
    if(idx == INVALID_ARRAY_INDEX)
    {
        glDeleteBuffers(1,&BufferID);
        return INVALID_ARRAY_INDEX;
    }
    VertexBufferInfo *pBufInfo = m_VertBufArray.GetItem(idx);
    memset(pBufInfo,0,sizeof(VertexBufferInfo));
    pBufInfo->BufferID = BufferID;
    pBufInfo->BufferSize = (GLuint)bufsize;
    pBufInfo->BufferUsage = BufUsage;

    return idx;
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLVBOSystem::UpdateVertexBuffer(uint32t buffer_idx, uint32t first, uint32t size, const void *data)
{
    VertexBufferInfo *pBufInfo = m_VertBufArray.GetItem(buffer_idx);
    if(!pBufInfo && !pBufInfo->BufferID)
        return false;

    //Too Large Data To Copy
    if((first + size) > (uint32t)pBufInfo->BufferSize)
        return false;

    //Binding Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER_ARB, pBufInfo->BufferID);
    GLvoid* dataPointer = NULL;
	dataPointer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy((GLubyte*)(dataPointer)+first,(const GLubyte*)(data)+first,size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLVBOSystem::DeleteVertexBuffer(uint32t buffer_idx)
{

    VertexBufferInfo *pBufInfo = m_VertBufArray.GetItem(buffer_idx);
    if(!pBufInfo && !pBufInfo->BufferID)
        return false;

    glDeleteBuffers(1,&pBufInfo->BufferID);
    m_VertBufArray.Release(buffer_idx);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLVBOSystem::BindBuffer(uint32t buffer_idx) 
{
    VertexBufferInfo *pBufInfo = m_VertBufArray.GetItem(buffer_idx);
    if(!pBufInfo)
        return false;

    glBindBuffer(GL_ARRAY_BUFFER_ARB, pBufInfo->BufferID);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::ReleaseVertexBuffers()
{
    m_VertBufArray.Start();
    VertexBufferInfo *pBufInfo = m_VertBufArray.NextItem();
    while(pBufInfo!=NULL){
        glDeleteBuffers(1,&pBufInfo->BufferID);
        pBufInfo = m_VertBufArray.NextItem();
    }
    m_VertBufArray.Clear();
}


/////////////////////////////////////////////////////////////////////////////////////////////
//Vertex Attrib Handling 
void CGLVBOSystem::SetVertexAttribPointer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, const void *pointer)
{
    glEnableVertexAttribArray(stream);
    GLenum datatype = CGLConvFunc::GetGLDataType(type);
    glVertexAttribPointer(stream,(GLint)size,datatype,normalize,(GLsizei)stride,pointer);
    m_ActiveStreams.SetBit(stream);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::SetVertexAttribBuffer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, uint32t offset, uint32t buf_idx)
{
    glEnableVertexAttribArray(stream);
    this->BindBuffer(buf_idx);
    GLenum datatype = CGLConvFunc::GetGLDataType(type);
    glVertexAttribPointer(stream,(GLint)size,datatype,normalize,(GLsizei)stride,BUFFER_OFFSET(offset)); 
    m_ActiveStreams.SetBit(stream);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::SetVertexAttribPointerI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, const void *pointer)
{
    glEnableVertexAttribArray(stream);
    GLenum datatype = CGLConvFunc::GetGLDataType(type);
    glVertexAttribIPointer(stream,(GLint)size,datatype,(GLsizei)stride,pointer);
    m_ActiveStreams.SetBit(stream);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::SetVertexAttribBufferI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, uint32t offset, uint32t buf_idx)
{
    glEnableVertexAttribArray(stream);
    this->BindBuffer(buf_idx);
    GLenum datatype = CGLConvFunc::GetGLDataType(type);
    glVertexAttribIPointer(stream,(GLint)size,datatype,(GLsizei)stride,BUFFER_OFFSET(offset)); 
    m_ActiveStreams.SetBit(stream);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::SetVertexAttribPointerL(uint32t stream, uint32t size, uint32t stride, const void *pointer)
{
    glEnableVertexAttribArray(stream);
    GLenum datatype = GL_DOUBLE;
    glVertexAttribLPointer(stream,(GLint)size,datatype,(GLsizei)stride,pointer);
    m_ActiveStreams.SetBit(stream);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::SetVertexAttribBufferL(uint32t stream, uint32t size, uint32t stride, uint32t offset, uint32t buf_idx)
{
    glEnableVertexAttribArray(stream);
    this->BindBuffer(buf_idx);
    GLenum datatype = GL_DOUBLE;
    glVertexAttribLPointer(stream,(GLint)size,datatype,(GLsizei)stride,BUFFER_OFFSET(offset)); 
    m_ActiveStreams.SetBit(stream);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::SetVertexAttribBuffers(GAPIVERTATTRIBDATA *attribbuf, uint32t count)
{
    GAPIVERTATTRIBDATA *pData = attribbuf;
    for(uint32t i = 0; i < count; i++) 
    {
        GLuint stream = pData->StreamIndex;
        if(pData->AttribName[0]) {
            stream = (GLuint)CGLShaderSystem::GetSingleton()->GetVertexAttribLocation(pData->AttribName);
        }
        m_ActiveStreams.SetBit(stream);
        glEnableVertexAttribArray(stream);
        CGLVBOSystem::GetSingleton()->BindBuffer(pData->BufferIdx);
        GLenum dataType = CGLConvFunc::GetGLDataType(pData->Type);
        if(pData->DirectFunc) {
            if(dataType == GL_DOUBLE)
                glVertexAttribLPointer(stream,(GLint)pData->Size,dataType,(GLsizei)pData->Stride,BUFFER_OFFSET(pData->Offset));
            else
                glVertexAttribIPointer(stream,(GLint)pData->Size,dataType,(GLsizei)pData->Stride,BUFFER_OFFSET(pData->Offset));
        }
        else {
            glVertexAttribPointer(stream,(GLint)pData->Size,dataType,pData->Normalize,(GLsizei)pData->Stride,BUFFER_OFFSET(pData->Offset));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::SetIndexBuffer(uint32t buffer_idx)
{
    VertexBufferInfo *pBufInfo = m_VertBufArray.GetItem(buffer_idx);
    if(!pBufInfo)
        return ;

    glEnableClientState(GL_INDEX_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, pBufInfo->BufferID);
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLVBOSystem::DisableVertexAttribPointers()
{
    unsigned int size = m_ActiveStreams.Size();
    for(unsigned int i = 0; i < size; i++)
    {
        if(m_ActiveStreams[i]) {
            m_ActiveStreams.ClearBit(i);
            glDisableVertexAttribArray(i);
        }
    }

    if(glIsEnabled(GL_INDEX_ARRAY))
        glDisableClientState( GL_INDEX_ARRAY ); 
}
/////////////////////////////////////////////////////////////////////////////////////////////
