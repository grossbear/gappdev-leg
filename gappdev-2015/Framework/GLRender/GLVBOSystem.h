/////////////////////////////////////////////////////////////////////////////////////////////
// GLVBOSystem.h
//
// OpenGL Vertex Buffer Object Management Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLVBOSYSTEM_H__
#define __GLVBOSYSTEM_H__

#include "Renderapi.h"
#include "Utils/BitArray.h"
#include "Utils/BlockArray.h"

#define VERT_BUF_OBJ_COUNT  8

/////////////////////////////////////////////////////////////////////////////////////////////
class CGLVBOSystem 
{
public:
    static CGLVBOSystem* GetSingleton();
    ~CGLVBOSystem();

    //Vertex Buffer Handling
    uint32t CreateVertexBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data);
    bool UpdateVertexBuffer(uint32t buffer_idx, uint32t first, uint32t size, const void *data);
    bool DeleteVertexBuffer(uint32t buffer_idx);
    bool BindBuffer(uint32t buffer_idx);

    //Vertex Attrib Handling 
    void SetVertexAttribPointer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, const void *pointer);
    void SetVertexAttribBuffer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, uint32t offset, uint32t buf_idx);
    void SetVertexAttribPointerI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, const void *pointer);
    void SetVertexAttribBufferI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, uint32t offset, uint32t buf_idx);
    void SetVertexAttribPointerL(uint32t stream, uint32t size, uint32t stride, const void *pointer);
    void SetVertexAttribBufferL(uint32t stream, uint32t size, uint32t stride, uint32t offset, uint32t buf_idx);

    void SetIndexBuffer(uint32t buf_idx);

    void SetVertexAttribBuffers(GAPIVERTATTRIBDATA *attribbuf, uint32t count);
	void DisableVertexAttribPointers();

protected:
    CGLVBOSystem();

    void ReleaseVertexBuffers();

    struct VertexBufferInfo
    {
        GLuint BufferID;
        GLuint BufferSize;
        GLenum BufferUsage;
    };

    CBitArray m_ActiveStreams;

    CBlockArray<CGLVBOSystem::VertexBufferInfo,VERT_BUF_OBJ_COUNT> m_VertBufArray;

    static CGLVBOSystem *m_pInstance;
};
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__GLVBOSYSTEM_H__