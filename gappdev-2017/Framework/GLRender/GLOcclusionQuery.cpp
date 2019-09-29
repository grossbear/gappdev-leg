///////////////////////////////////////////////////////////////////////////////////////////////
//  GLOcclusionQuery.cpp
//  OpenGL Occlusion Query Test Class
//  Created: 05-11-2014
//  Previous Version: GLRenderQuery 
//  Created: 26-12-2010
//  Modified: 11-01-2013
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "GLDevInfo.h"
#include "GLConvFunc.h"

#include "GLVerProc.h"
#include "GLArbProc.h"

#include "GLOcclusionQuery.h"

////////////////////////////////////////////////////////////////////////////////////////////////
CGLOcclusionQuery* CGLOcclusionQuery::m_pInstance = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////
CGLOcclusionQuery* CGLOcclusionQuery::GetSingleton()
{
    if(m_pInstance == NULL)
        m_pInstance = new CGLOcclusionQuery();

    return m_pInstance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CGLOcclusionQuery::CGLOcclusionQuery():m_bQueryStarted(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
CGLOcclusionQuery::~CGLOcclusionQuery()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLOcclusionQuery::CreateQueryObject()
{
    if(QueryStarted()) return 0;

    unsigned int idx = m_QueryObjects.Reserve(1);
    if(idx == 0xffffffff)
        return 0;

    GLuint *ptr = m_QueryObjects.GetItem(idx);

    GLuint objid = 0;
    glGenQueries(1,&objid);

    if(objid==0) 
    {
        m_QueryObjects.Release(idx);
        return 0;
    }

    *ptr = objid;
    
    return idx;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLOcclusionQuery::DeleteQueryObject(uint32t objidx)
{
    GLuint *ptr = m_QueryObjects.GetItem(objidx);
    if(ptr==NULL) return false;

    GLuint objid = *ptr;
    if(objid==0) return false;

    glDeleteQueries(1,&objid);

    m_QueryObjects.Release(objidx);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void    CGLOcclusionQuery::BeginQuery(uint32t objidx)
{
    GLuint *ptr = m_QueryObjects.GetItem(objidx);
    if(ptr==NULL) return ;

    GLuint objid = *ptr;
    glBeginQuery(GL_SAMPLES_PASSED_ARB, objid);

    m_bQueryStarted = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void    CGLOcclusionQuery::EndQuery()
{
    glEndQuery(GL_SAMPLES_PASSED_ARB);

    m_bQueryStarted = false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLOcclusionQuery::GetQueryObjects(uint32t objidx)
{
    GLuint *ptr = m_QueryObjects.GetItem(objidx);
    if(ptr==NULL) return 0;

    GLuint objid = *ptr;
    GLuint pix_count = 0;

    glGetQueryObjectuiv(objid,GL_QUERY_RESULT_ARB,&pix_count);

    return (uint32t)pix_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////
