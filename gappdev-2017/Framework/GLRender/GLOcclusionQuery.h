///////////////////////////////////////////////////////////////////////////////////////////////
//  GLOcclusionQuery.h
//  OpenGL Occlusion Query Test Class
//  Created: 05-11-2014
//  Previous Version: GLRenderQuery 
//  Created: 26-12-2010
//  Modified: 11-01-2013
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GL_OCCLUSIONQUERY_H_
#define _GL_OCCLUSIONQUERY_H_

#include "Utils/BitArray.h"
#include "Utils/ArrayMap.h"

#define QUERYIDS_BLOCK_SIZE 16

class CGLOcclusionQuery 
{
public:
    static CGLOcclusionQuery* GetSingleton();
    ~CGLOcclusionQuery();

    uint32t CreateQueryObject();
    bool    DeleteQueryObject(uint32t objidx);
    void    BeginQuery(uint32t objidx);
    void    EndQuery();
    uint32t GetQueryObjects(uint32t objidx);
    bool    QueryStarted() const {return m_bQueryStarted;}

protected:
    CGLOcclusionQuery();

    CArrayMap<GLuint, QUERYIDS_BLOCK_SIZE> m_QueryObjects;
    bool m_bQueryStarted;
    static CGLOcclusionQuery *m_pInstance;
};

#endif //_GL_OCCLUSIONQUERY_H_