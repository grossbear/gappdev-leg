////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	GLTextureSystem.cpp 
//	OpenGL Textures Managment Singleton
//	Created 29-05-2010
//  Modified 12-01-2013
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include <math.h>

#include "GLTextureSystem.h"
#include "GLConvFunc.h"

#include <stdio.h>
#include "GLDevInfo.h"
#include "GL/glext.h"
#include "GLVerProc.h"
#include "GLArbProc.h"
*/

#include <math.h>
#include "GLDevInfo.h"
#include "GLConvFunc.h"

#include "GLVerProc.h"
#include "GLArbProc.h"

#include "GLTextureSystem.h"

#define GLMAX(a,b) (a>b)?(a):(b)

#define LOG2F   0.30102999566398119521373889472449f

#define ISPOWOF2(n) !(n & (n-1))



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGLTextureSystem* CGLTextureSystem::m_pInstance = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGLTextureSystem::CGLTextureSystem():
mActiveRenderTexture(0),
mActiveRenderTarget(0),
mGenRTMipMaps(false),
m_bTexUnitsActive(false),
m_bTexEnvStatesCheck(false)
{
    SetPixelAlign(1);
    memset(mTexTargets,0,sizeof(GLenum)*TEX_TARGETS_SIZE);
    memset(mViewport,0,sizeof(GLint)*4);
    memset(mTexSamplersActive,0,sizeof(GLuint)*TEX_TARGETS_SIZE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGLTextureSystem::~CGLTextureSystem()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGLTextureSystem* CGLTextureSystem::GetSingleton()
{
    if(m_pInstance == NULL)
        m_pInstance = new CGLTextureSystem();

    return m_pInstance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
uint32t CGLTextureSystem::CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT fmt, const void *data)
{
    const void *tab[1]={data};
    return this->CreateTexture(width,height,depth,flags,fmt,tab);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
uint32t CGLTextureSystem::CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT texfmt, const void **data)
{
    if(width < 1) return 0;

    // Is Supporting Texture Format
    if(!IsSupportedTexFormat(texfmt))
        return 0;

    // Check Texture Size
    GLint maxTexSize = CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_SIZE);
    //Check If Texture Size Is Not Too Large
    bool islarge = (width > (uint32t)maxTexSize) ? (true) : ((height > (uint32t)maxTexSize)?(true):((depth > (uint32t)maxTexSize)?(true):(false)));
    if(islarge) return 0;


    GLenum target = this->GetTexType(width,height,depth,flags);
    if(target == GL_NONE) return 0;

    // Check Data Pointer
    const void *ppdata[6]={0};
    if(data!=NULL)
    {
        ppdata[0] = data[0];
        if(target==GL_TEXTURE_CUBE_MAP)
            for(uint32t i = 0; i < 6; i++)
                ppdata[i] = data[0];
        
    }

    // Generating OpenGL Texture Object
    GLuint texid = 0;
    glGenTextures(1,&texid);
    if(texid==0) return 0;

    GLint intfmt = CGLConvFunc::GetGLTexInterFmt(texfmt);
    GLenum fmt = CGLConvFunc::GetGLTexFmt(texfmt);
    GLenum type = CGLConvFunc::GetGLTexDataType(texfmt);

    GLsizei iwidth = (GLsizei)width;
    GLsizei iheight = (GLsizei)height;
    GLsizei idepth = (GLsizei)depth;

    // Creating Texture Object
    uint32t texidx = m_TextureObjects.Reserve(1);
    if(texidx==0)return 0;

    //
    GLint align = (flags & GAPI_TEX_WORD_ALIGN_FLAG) ? (4) : ((flags & GAPI_TEX_DWORD_ALIGN_FLAG)?(8):(0));
    if(align) SetPixelAlign(align);

    // Starting Create Texture
    GLboolean compressed = CGLConvFunc::IsCompressedFormat(texfmt);

    // Bind Texture ID
    glBindTexture(target,texid);
    switch(target)
    {
    case GL_TEXTURE_1D:
        if(compressed)
            glCompressedTexImage1D(GL_TEXTURE_1D,0,intfmt,iwidth,0,CalcMapSize(texfmt,iwidth,1,1,0,align),ppdata[0]);
        else
            glTexImage1D(GL_TEXTURE_1D,0,intfmt,iwidth,0,fmt,type,ppdata[0]);
        break;

    case GL_TEXTURE_2D:
        if(compressed)
            glCompressedTexImage2D(GL_TEXTURE_2D,0,intfmt,iwidth,iheight,0,CalcMapSize(texfmt,iwidth,iheight,1,0,align),ppdata[0]);
        else
            glTexImage2D(GL_TEXTURE_2D,0,intfmt,iwidth,iheight,0,fmt,type,ppdata[0]);
        break;

    case GL_TEXTURE_3D:
        if(compressed)
            glCompressedTexImage3D(GL_TEXTURE_3D,0,intfmt,iwidth,iheight,idepth,0,CalcMapSize(texfmt,iwidth,iheight,idepth,0,align),ppdata[0]);
        else
            glTexImage3DEXT(GL_TEXTURE_3D,0,intfmt,iwidth,iheight,idepth,0,fmt,type,ppdata[0]);
        break;

    case GL_TEXTURE_CUBE_MAP:
        if(compressed)
        {
            uint32t mapsize = CalcMapSize(texfmt,iwidth,iheight,1,0,align);
            for(uint32t face = 0; face < 6; face++)
                glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,intfmt,iwidth,iheight,0,mapsize,ppdata[face]);
        }
        else
        {
            uint32t mapsize = CalcMapSize(texfmt,iwidth,iheight,1,0,align);
            for(uint32t face = 0; face < 6; face++)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,intfmt,iwidth,iheight,0,fmt,type,ppdata[face]);
        }
        break;

    case GL_TEXTURE_RECTANGLE_ARB:
        glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,intfmt,iwidth,iheight,0,fmt,type,ppdata[0]);
        break;
    }

    // Does OpenGL Drivers Have To Generate MipMaps
    bool genmipmaps = (flags & GAPI_TEX_GENMIPMAPS_FLAG) && (target!=GL_TEXTURE_RECTANGLE_ARB);
    if(genmipmaps)
    {
        if(GL_TEXTURE_CUBE_MAP)
            for(uint32t face = 0; face < 6; face++)
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face);
        else
            glGenerateMipmap(target);
    }

    // Getting Texture Object Pointer
    CGLTextureSystem::GLTexture *tex_obj = m_TextureObjects.GetItem(texidx);
    memset(tex_obj,0,sizeof(CGLTextureSystem::GLTexture));

    // Getting Texture Size
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_WIDTH,&iwidth);
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_HEIGHT,&iheight);
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_DEPTH,&idepth);


    SetTexObjParams(*tex_obj,iwidth,iheight,idepth,target,texid,intfmt,fmt,type,genmipmaps,0,0,0);

    // Setup Texture Filters And Wrap Mode
    SetupTexFilters(*tex_obj,TEXFLT_LINEAR);
    SetupTexWrap(*tex_obj,TEXADDR_REPEAT);

    //End Of Texture Creation
    glBindTexture(target,0);

    if(align) SetPixelAlign(1);

    return texidx;
}
*/

/*
#define GAPI_TEX_GENMIPMAPS_FLAG    0x1
#define GAPI_TEX_WORD_ALIGN_FLAG    0x2
#define GAPI_TEX_DWORD_ALIGN_FLAG   0x4
#define GAPI_TEX_CUBE_MAP_FLAG      0x8
#define GAPI_TEX_RECT_FLAG          0x10
#define GAPI_TEX_RENDER_TARGET_FLAG 0x20
#define GAPI_TEX_ARRAY_FLAG         0x40
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLTextureSystem::CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT texfmt, const void **data)
{
    if(width < 1) return 0;

    // Is Supporting Texture Format
    if(!IsSupportedTexFormat(texfmt))
        return 0;

    // Check Texture Size
    GLint maxTexSize = CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_SIZE);
    //Check If Texture Size Is Not Too Large
    bool islarge = (width > (uint32t)maxTexSize) ? (true) : ((height > (uint32t)maxTexSize)?(true):((depth > (uint32t)maxTexSize)?(true):(false)));
    if(islarge) return 0;

    // Casting Formats To GLsizei Format
    GLsizei iwidth = (GLsizei)width;
    GLsizei iheight = (GLsizei)height;
    GLsizei idepth = (GLsizei)depth;

    uint32t texidx = 0;
    // Render Target
    if(texfmt & GAPI_TEX_RENDER_TARGET_FLAG)
    {
        TEXFMT dstexfmt = TEXFMT_DEPTH24;
        texidx = CreateRenderTarget(iwidth,iheight,idepth,flags,texfmt,dstexfmt);
        return texidx;
    }

    // CubeMap Texture
    if(texfmt & GAPI_TEX_CUBE_MAP_FLAG)
    {
        texidx =  CreateCubeMapTexture(iwidth,iheight,idepth,flags,texfmt,data);
        return texidx;
    }

    // Standard Texture Image
    const void *pointer = (data!=NULL)?(data[0]):(NULL);
    texidx = CreateTextureImage(iwidth,iheight,idepth,flags,texfmt,pointer);

    return texidx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLTextureSystem::CreateTextureImage(GLsizei iwidth, GLsizei iheight, GLsizei idepth, uint32t flags, TEXFMT texfmt, const void *data)
{
    GLenum target = this->GetTexType(iwidth,iheight,idepth,flags);
    if(target == GL_NONE) return 0;

    // Creating Texture Object
    uint32t texidx = m_TextureObjects.Reserve(1);
    if(texidx==0) return 0;

    // Starting Create Texture
    GLboolean compressed = CGLConvFunc::IsCompressedFormat(texfmt);

    GLint intfmt = CGLConvFunc::GetGLTexInterFmt(texfmt);
    GLenum fmt = CGLConvFunc::GetGLTexFmt(texfmt);
    GLenum type = CGLConvFunc::GetGLTexDataType(texfmt);

    // Generating OpenGL Texture Object
    GLuint texid = 0;
    glGenTextures(1,&texid);
    if(texid==0) return 0;

    // Setting Alignment
    GLint align = (flags & GAPI_TEX_WORD_ALIGN_FLAG) ? (4) : ((flags & GAPI_TEX_DWORD_ALIGN_FLAG)?(8):(0));
    if(align) SetPixelAlign(align);

    // Bind Texture ID
    glBindTexture(target,texid);
    switch(target)
    {
    case GL_TEXTURE_1D:
        if(compressed)
            glCompressedTexImage1D(GL_TEXTURE_1D,0,intfmt,iwidth,0,CalcMapSize(texfmt,iwidth,1,1,0,align),data);
        else
            glTexImage1D(GL_TEXTURE_1D,0,intfmt,iwidth,0,fmt,type,data);
        break;

    case GL_TEXTURE_1D_ARRAY:
        if(compressed)
            glCompressedTexImage2D(GL_TEXTURE_1D_ARRAY,0,intfmt,iwidth,iheight,0,CalcMapSize(texfmt,iwidth,iheight,1,0,align),data);
        else
            glTexImage2D(GL_TEXTURE_1D_ARRAY,0,intfmt,iwidth,iheight,0,fmt,type,data);
        break;

    case GL_TEXTURE_2D:
        if(compressed)
            glCompressedTexImage2D(GL_TEXTURE_2D,0,intfmt,iwidth,iheight,0,CalcMapSize(texfmt,iwidth,iheight,1,0,align),data);
        else
            glTexImage2D(GL_TEXTURE_2D,0,intfmt,iwidth,iheight,0,fmt,type,data);
        break;

    case GL_TEXTURE_2D_ARRAY:
        if(compressed)
            glCompressedTexImage3D(GL_TEXTURE_2D_ARRAY,0,intfmt,iwidth,iheight,idepth,0,CalcMapSize(texfmt,iwidth,iheight,idepth,0,align),data);
        else
            glTexImage3DEXT(GL_TEXTURE_2D_ARRAY,0,intfmt,iwidth,iheight,idepth,0,fmt,type,data);
        break;

    case GL_TEXTURE_3D:
        if(compressed)
            glCompressedTexImage3D(GL_TEXTURE_3D,0,intfmt,iwidth,iheight,idepth,0,CalcMapSize(texfmt,iwidth,iheight,idepth,0,align),data);
        else
            glTexImage3DEXT(GL_TEXTURE_3D,0,intfmt,iwidth,iheight,idepth,0,fmt,type,data);
        break;

    case GL_TEXTURE_RECTANGLE_ARB:
        glTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,intfmt,iwidth,iheight,0,fmt,type,data);
        break;
    }

    // Does OpenGL Drivers Have To Generate MipMaps
    bool genmipmaps = (flags & GAPI_TEX_GENMIPMAPS_FLAG) && (target!=GL_TEXTURE_RECTANGLE_ARB);
    if(genmipmaps)
        glGenerateMipmap(target);

    // Getting Texture Object Pointer
    CGLTextureSystem::GLTexture *tex_obj = m_TextureObjects.GetItem(texidx);
    memset(tex_obj,0,sizeof(CGLTextureSystem::GLTexture));

    // Getting Texture Size
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_WIDTH,&iwidth);
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_HEIGHT,&iheight);
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_DEPTH,&idepth);


    SetTexObjParams(*tex_obj,iwidth,iheight,idepth,target,texid,intfmt,fmt,type,genmipmaps,0,0,0);

    // Setup Texture Filters And Wrap Mode
    SetupTexFilters(*tex_obj,TEXFLT_LINEAR);
    SetupTexWrap(*tex_obj,TEXADDR_REPEAT);

    //End Of Texture Creation
    glBindTexture(target,0);

    if(align) SetPixelAlign(1);

    return texidx;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLTextureSystem::CreateCubeMapTexture(GLsizei iwidth, GLsizei iheight, GLsizei idepth, uint32t flags, TEXFMT texfmt, const void **ppdata)
{
    GLenum target = this->GetTexType(iwidth,iheight,idepth,flags);
    if(target == GL_NONE) return 0;

    // Creating Texture Object
    uint32t texidx = m_TextureObjects.Reserve(1);
    if(texidx==0) return 0;

    // Starting Create Texture
    GLboolean compressed = CGLConvFunc::IsCompressedFormat(texfmt);

    GLint intfmt = CGLConvFunc::GetGLTexInterFmt(texfmt);
    GLenum fmt = CGLConvFunc::GetGLTexFmt(texfmt);
    GLenum type = CGLConvFunc::GetGLTexDataType(texfmt);

    // Generating OpenGL Texture Object
    GLuint texid = 0;
    glGenTextures(1,&texid);
    if(texid==0) return 0;

    // Setting Alignment
    GLint align = (flags & GAPI_TEX_WORD_ALIGN_FLAG) ? (4) : ((flags & GAPI_TEX_DWORD_ALIGN_FLAG)?(8):(0));
    if(align) SetPixelAlign(align);

    // Fill Texture With Data
    // Bind Texture ID
    glBindTexture(target,texid);
    switch(target)
    {
    case GL_TEXTURE_CUBE_MAP:
        if(compressed)
        {
            uint32t mapsize = CalcMapSize(texfmt,iwidth,iheight,1,0,align);
            for(uint32t face = 0; face < 6; face++)
                glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,intfmt,iwidth,iheight,0,mapsize,ppdata[face]);
        }
        else
        {
            uint32t mapsize = CalcMapSize(texfmt,iwidth,iheight,1,0,align);
            for(uint32t face = 0; face < 6; face++)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,intfmt,iwidth,iheight,0,fmt,type,ppdata[face]);
        }
        break;

    case GL_TEXTURE_CUBE_MAP_ARRAY:
        if(compressed)
        {
            uint32t mapsize = CalcMapSize(texfmt,iwidth,iheight,1,0,align);
            for(uint32t face = 0; face < 6; face++)
                glCompressedTexImage3D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,intfmt,iwidth,iheight,idepth,0,mapsize,ppdata[face]);
        }
        else
        {
            uint32t mapsize = CalcMapSize(texfmt,iwidth,iheight,1,0,align);
            for(uint32t face = 0; face < 6; face++)
                glTexImage3DEXT(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,intfmt,iwidth,iheight,idepth,0,fmt,type,ppdata[face]);
        }
        break;
    }


    // Does OpenGL Drivers Have To Generate MipMaps
    bool genmipmaps = (flags & GAPI_TEX_GENMIPMAPS_FLAG) && (target!=GL_TEXTURE_RECTANGLE_ARB);
    if(genmipmaps)
        for(uint32t face = 0; face < 6; face++)
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face);


    // Getting Texture Object Pointer
    CGLTextureSystem::GLTexture *tex_obj = m_TextureObjects.GetItem(texidx);
    memset(tex_obj,0,sizeof(CGLTextureSystem::GLTexture));

    // Getting Texture Size
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_WIDTH,&iwidth);
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_HEIGHT,&iheight);
    glGetTexLevelParameteriv(target,0,GL_TEXTURE_DEPTH,&idepth);


    SetTexObjParams(*tex_obj,iwidth,iheight,idepth,target,texid,intfmt,fmt,type,genmipmaps,0,0,0);

    // Setup Texture Filters And Wrap Mode
    SetupTexFilters(*tex_obj,TEXFLT_LINEAR);
    SetupTexWrap(*tex_obj,TEXADDR_REPEAT);

    //End Of Texture Creation
    glBindTexture(target,0);

    if(align) SetPixelAlign(1);

    return texidx;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLTextureSystem::CreateRenderTarget(GLsizei width, GLsizei height, GLsizei depth, uint32t flags, TEXFMT rttexfmt, TEXFMT dstexfmt)
{
    GLenum target = this->GetTexType(width,height,depth,flags);
    if(target == GL_NONE) return 0;

    // Creating Texture Object
    uint32t texidx = m_TextureObjects.Reserve(1);
    if(texidx==0) return 0;

    bool createcubemap = (flags & GAPI_TEX_CUBE_MAP_FLAG)>0;
    bool genmipmaps = (flags & GAPI_TEX_GENMIPMAPS_FLAG)>0;

    GLuint texID = 0;
    glGenTextures(1,&texID);
    if(!texID){
        GLenum error = glGetError();
        m_TextureObjects.Release(texidx,1);
        return 0;
    }
    glBindTexture(target,texID);

    GLint rtintfmt = CGLConvFunc::GetGLTexInterFmt(rttexfmt);
    GLenum rtfmt = CGLConvFunc::GetGLTexFmt(rttexfmt);
    GLenum rttype = CGLConvFunc::GetGLTexDataType(rttexfmt);

    // spec texture
    switch(target)
    {
    case GL_TEXTURE_1D:
        glTexImage1D(GL_TEXTURE_1D,0,rtintfmt,width,0,rtfmt,rttype,NULL);
        break;
    case GL_TEXTURE_1D_ARRAY:
        glTexImage2D(GL_TEXTURE_1D_ARRAY,0,rtintfmt,width,height,0,rtfmt,rttype,NULL);
        break;
    case GL_TEXTURE_2D:
        glTexImage2D(GL_TEXTURE_2D,0,rtintfmt,width,height,0,rtfmt,rttype,NULL);
        break;
    case GL_TEXTURE_2D_ARRAY:
        glTexImage3DEXT(GL_TEXTURE_2D_ARRAY,0,rtintfmt,width,height,depth,0,rtfmt,rttype,NULL);
        break;
    case GL_TEXTURE_CUBE_MAP:
        for(uint32t face = 0; face < 6; face++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,rtintfmt,width,height,0,rtfmt,rttype,0);
        break;
    case GL_TEXTURE_CUBE_MAP_ARRAY:
        for(uint32t face = 0; face < 6; face++)
            glTexImage3DEXT(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face,0,rtintfmt,width,height,depth,0,rtfmt,rttype,0);
        break;
    case GL_TEXTURE_3D:
        glTexImage3DEXT(GL_TEXTURE_3D,0,rtintfmt,width,height,depth,0,rtfmt,rttype,NULL);
        break;
    }

    // Getting Texture Object
    GLTexture *tex_obj = m_TextureObjects.GetItem(texidx);
    memset(tex_obj,0,sizeof(CGLTextureSystem::GLTexture));

    //--uint32t miplevels = (genmipmaps) ? ((uint32t) ( log(float(GLMAX(width,(uint32t)GLMAX(height,depth)))) / log(2.0f) )) : (0);

    // Generating Frame Buffer Object
    GLuint rbo = 0;
    //Create Depth Buffer
    if((dstexfmt >= TEXFMT_DEPTH16) && (dstexfmt <= TEXFMT_DEPTH24))
    {
        glGenRenderbuffers(1,&rbo);
        glBindRenderbuffer(GL_RENDERBUFFER_EXT, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER_EXT, CGLConvFunc::GetGLTexInterFmt(dstexfmt), width, height);
        glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
    }

    //Create And Setup Framebuffer Object
    GLuint fbo = 0;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, fbo);
    // Attach Color FrameBuffer
    GLenum attachment = (rttype==GL_DEPTH_COMPONENT) ? (GL_DEPTH_ATTACHMENT_EXT) : (GL_COLOR_ATTACHMENT0_EXT);

    // Specify Framebuffer Texture
    switch(target)
    {
    case GL_TEXTURE_1D:
        glFramebufferTexture1D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D, texID, 0);
        break;

    case GL_TEXTURE_1D_ARRAY:
        glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D_ARRAY, texID, 0);
        break;

    case GL_TEXTURE_2D:
        glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, texID, 0);
        break;

    case GL_TEXTURE_2D_ARRAY:
        glFramebufferTexture3D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D_ARRAY, texID, 0, 0);
        break;

    case GL_TEXTURE_CUBE_MAP:
        for(uint32t face = 0; face < 6; face++)
            glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, attachment,GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texID, 0);
        break;

    case GL_TEXTURE_CUBE_MAP_ARRAY:
        for(uint32t face = 0; face < 6; face++)
            glFramebufferTexture3D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texID, 0, 0);
        break;

    case GL_TEXTURE_3D:
        glFramebufferTexture3D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_3D, texID, 0, 0);
        break;
    
    }
    
    // Attach depth buffer
    if(rbo)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rbo);

    if(rttype==GL_DEPTH_COMPONENT)
    {
        // disable color buffer if you don't attach any color buffer image,
        // for example, rendering depth buffer only to a texture.
        // Otherwise, glCheckFramebufferStatusEXT will not be complete.
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // Setting Texture Object Params
    SetTexObjParams(*tex_obj,width,height,1,target,texID,rtintfmt,rtfmt,rttype,genmipmaps,fbo,rbo,0);
    // Setup Texture Filters And Wrap Mode
    SetupTexFilters(*tex_obj,TEXFLT_LINEAR);
    SetupTexWrap(*tex_obj,TEXADDR_REPEAT);

    if (!CheckFrameBufferStatus())
    {
        DeleteTexture(texidx);
        return 0;
    } 

    return texidx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::DeleteTexture(uint32t texidx)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL && !texobj->texid) return false;

    glDeleteTextures(1,&texobj->texid);
    GLenum error = glGetError();

    if(texobj->fbo) 
        glDeleteFramebuffers(1,&texobj->fbo);
    if(texobj->rbo) 
        glDeleteRenderbuffers(1, &texobj->rbo);

    m_TextureObjects.Release(texidx,1);

    return true; 
}

/*
#define GAPI_TEX_GENMIPMAPS_FLAG    0x1
#define GAPI_TEX_WORD_ALIGN_FLAG    0x2
#define GAPI_TEX_DWORD_ALIGN_FLAG   0x4
#define GAPI_TEX_CUBE_MAP_FLAG      0x8
#define GAPI_TEX_RECT_FLAG          0x10

*/

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLTextureSystem::CreateRenderTarget(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT rtfmt, TEXFMT dsfmt)
{
    width = GLMAX(1,width);
    height = GLMAX(1,height);
    depth = GLMAX(1,depth);

    GLint maxTexSize = CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_SIZE);

    //Check If Texture Size Is Not Too Large
    bool islarge = (width > (uint32t)maxTexSize) ? (true) : ((height > (uint32t)maxTexSize)?(true):((depth > (uint32t)maxTexSize)?(true):(false)));
    if(islarge) return 0;

    bool createcubemap = (flags & GAPI_TEX_CUBE_MAP_FLAG)>0;
    bool genmipmaps = (flags & GAPI_TEX_GENMIPMAPS_FLAG)>0;

    GLenum target = GetTexType(width,height,depth,flags);

    uint32t miplevels = (genmipmaps) ? ((uint32t) ( log(float(GLMAX(width,(uint32t)GLMAX(height,depth)))) / log(2.0f) )) : (0);

    uint32t texidx = 0;
    switch(target)
    {
    case GL_TEXTURE_2D:
        texidx = this->CreateRenderTarget2D((GLsizei)width,(GLsizei)height,rtfmt,dsfmt,genmipmaps);
    }

    return texidx;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::UpdateTexture(uint32t texidx, uint32t miplevel, uint32t face, uint32t align, uint32t xoff, uint32t yoff, uint32t zoff, const void *data)
{
    GLTexture *tex_obj = m_TextureObjects.GetItem(texidx);
    if(tex_obj==NULL) return false;

    align = (align==1)?(0):(align);
    if(align) SetPixelAlign(align);

    switch(tex_obj->target)
    {
    case GL_TEXTURE_1D:
        glEnable(GL_TEXTURE_1D);
        glBindTexture(GL_TEXTURE_1D,tex_obj->texid);
        glTexSubImage1D(GL_TEXTURE_1D,miplevel,(GLint)xoff,tex_obj->width,tex_obj->format,tex_obj->datatype,data);
        if(tex_obj->genmipmaps) glGenerateMipmap(GL_TEXTURE_1D);
        glBindTexture(GL_TEXTURE_1D,0);
        glDisable(GL_TEXTURE_1D);
        break;

    case GL_TEXTURE_2D:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex_obj->texid);
        glTexSubImage2D(GL_TEXTURE_2D,miplevel,xoff,yoff,tex_obj->width,tex_obj->height,tex_obj->format,tex_obj->datatype,data);
        if(tex_obj->genmipmaps) glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
        glDisable(GL_TEXTURE_2D);
        break;

    case GL_TEXTURE_3D_EXT:
        glEnable(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D,tex_obj->texid);
        glTexSubImage3DEXT(GL_TEXTURE_3D,miplevel,xoff,yoff,zoff,tex_obj->width,tex_obj->height,tex_obj->depth,tex_obj->format,tex_obj->datatype,data);
        if(tex_obj->genmipmaps) glGenerateMipmap(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D,0);
        glDisable(GL_TEXTURE_3D);
        break;

    case GL_TEXTURE_CUBE_MAP:
        if(face>=6)
        {
            if(align) SetPixelAlign(1);
            return false;
        }

        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP,tex_obj->texid);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+face,miplevel,xoff,yoff,tex_obj->width,tex_obj->height,tex_obj->format,tex_obj->datatype,data);
        if(tex_obj->genmipmaps) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glDisable(GL_TEXTURE_CUBE_MAP);
        break;

    default:
        if(align > 1) SetPixelAlign(1);
        return false;
        break;
    }

    if(align) SetPixelAlign(1);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::BindRenderTarget(uint32t texidx, uint32t face)
{
    GLTexture *tex_obj = m_TextureObjects.GetItem(texidx);
    if(tex_obj==NULL) return false;

    if(face!=0 && tex_obj->target != GL_TEXTURE_CUBE_MAP_ARB) return false;

    // Saving Active Vieport
    glGetIntegerv(GL_VIEWPORT, mViewport); 

    // Begin Rendering To FrameBuffer
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, tex_obj->fbo);
    glViewport(0, 0, tex_obj->width, tex_obj->height);

    // Change Cubemap Face To Render To
    if(tex_obj->target == GL_TEXTURE_CUBE_MAP)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, tex_obj->texid, 0);
    }

    mActiveRenderTarget = tex_obj->target;
    mActiveRenderTexture = tex_obj->texid;
    mGenRTMipMaps = tex_obj->genmipmaps;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::UnbindRenderTarget()
{
    //Return To Main Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
    glViewport(mViewport[0], mViewport[1], mViewport[2], mViewport[3]);

    if(mGenRTMipMaps)
    {
        glEnable(mActiveRenderTarget);
        glBindTexture(mActiveRenderTarget, mActiveRenderTexture);
        glGenerateMipmap(mActiveRenderTarget);
        glDisable(mActiveRenderTarget);
    }

    mActiveRenderTarget = 0;
    mActiveRenderTexture = 0;
    mGenRTMipMaps = 0;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::SetTexFilter(uint32t texidx, TEXFLT flt)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    glBindTexture(texobj->target,texobj->texid);
    SetupTexFilters(*texobj,flt);
    glBindTexture(GL_TEXTURE_2D,0);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::GetTexFilter(uint32t texidx, TEXFLT &flt)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    flt = CGLConvFunc::GetGAPITexFlt(texobj->minfilter,texobj->anisotropy);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::SetTexAnisotropy(uint32t texidx, uint32t val)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    GLint maxaniso = CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_ANISOTROPY);
    texobj->anisotropy = (GLint(val)>maxaniso)?(maxaniso):((GLint)val);

    glBindTexture(texobj->target,texobj->texid);
    glTexParameteri(texobj->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, texobj->anisotropy);
    glBindTexture(GL_TEXTURE_2D,0);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::GetTexAnisotropy(uint32t texidx, uint32t &val)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    val = (uint32t)texobj->anisotropy;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::SetTexAddr(uint32t texidx, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    glBindTexture(texobj->target, texobj->texid);
    SetupTexWrap(*texobj,addrS,addrT,addrR);
    glBindTexture(texobj->target,0);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::GetTexAddr(uint32t texidx, TEXADDR &addrS, TEXADDR &addrT, TEXADDR &addrR)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    addrS = CGLConvFunc::GetGAPITexAddr(texobj->wraps);
    addrT = CGLConvFunc::GetGAPITexAddr(texobj->wrapt);
    addrR = CGLConvFunc::GetGAPITexAddr(texobj->wrapr);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::GetTexWidth(uint32t texidx, uint32t &width)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    width = (uint32t)texobj->width;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool  CGLTextureSystem::GetTexHeight(uint32t texidx, uint32t &height)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    height = (uint32t)texobj->height;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::GetTexDepth(uint32t texidx, uint32t &depth)
{
    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    depth = (uint32t)texobj->height;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLTextureSystem::CreateTextureSampler()
{
    uint32t sampleridx = m_TexSamplerObjects.Reserve(1);
    GLuint *psamplerid = m_TexSamplerObjects.GetItem(sampleridx);
    if(!psamplerid) return 0;

    glGenSamplers(1,psamplerid);
    if(!psamplerid) return 0;

    GLuint sampler = *psamplerid;

    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);

    return sampleridx;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::DeleteTextureSampler(uint32t sampleridx)
{
    GLuint *psamplerid = m_TexSamplerObjects.GetItem(sampleridx);
    if(!psamplerid) return false;

    glDeleteSamplers(1,psamplerid);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerFilter(uint32t sampleridx, TEXFLT flt)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    GLint texflt = CGLConvFunc::GetGLTexFlt(flt);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, texflt);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, texflt);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerMinFilter(uint32t sampleridx, TEXFLT flt)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    GLint texflt = CGLConvFunc::GetGLTexFlt(flt);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, texflt);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerMagFilter(uint32t sampleridx, TEXFLT flt)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    GLint texflt = CGLConvFunc::GetGLTexFlt(flt);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, texflt);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerAddr(uint32t sampleridx, TEXADDR wrap)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    GLint texwrap = CGLConvFunc::GetGLTexAddr(wrap);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, texwrap);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, texwrap);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, texwrap);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerAddr(uint32t sampleridx, TEXADDR sWrap, TEXADDR tWrap, TEXADDR rWrap)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    GLint texwrap_s = CGLConvFunc::GetGLTexAddr(sWrap);
    GLint texwrap_t = CGLConvFunc::GetGLTexAddr(tWrap);
    GLint texwrap_r = CGLConvFunc::GetGLTexAddr(rWrap);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, texwrap_s);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, texwrap_t);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, texwrap_r);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerMinLod(uint32t sampleridx, int32t lod)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    glSamplerParameteri(sampler, GL_TEXTURE_MIN_LOD, lod);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerMaxLod(uint32t sampleridx, int32t lod)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    glSamplerParameteri(sampler, GL_TEXTURE_MAX_LOD, lod);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerMaxAnisotroty(uint32t sampleridx, int32t anisotropy)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    glSamplerParameteri(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTexSamplerCompare(uint32t sampleridx, GAPIFUNCTION func, bool modeon)
{
    GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
    if(ptr) return false;
    GLuint sampler = *ptr;

    GLenum glfunc = CGLConvFunc::GetGLFunc(func);
    glSamplerParameteri ( sampler, GL_TEXTURE_COMPARE_MODE, (modeon)?(GL_COMPARE_REF_TO_TEXTURE):(GL_NONE) );
    glSamplerParameteri ( sampler, GL_TEXTURE_COMPARE_FUNC, glfunc );

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::SetTexture(uint32t texidx, uint32t tex_unit)
{
    uint32t maxunits = (uint32t)CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_COMBINED_TEXTURE_UNITS); //GAPI_DEVPARAM_MAX_TEXTURE_UNITS
    if(tex_unit >= maxunits) 
        return false;

    glActiveTexture(GL_TEXTURE0_ARB+tex_unit);
    if(texidx==0)
    {
        if(mTexTargets[tex_unit]!=0)
        {
            glDisable(mTexTargets[tex_unit]);
            mTexTargets[tex_unit]=0;   
        }
        if(m_bTexEnvStatesCheck)
            DisableAdditionalTexEnvStates();

        return true;
    }

    GLTexture *texobj = m_TextureObjects.GetItem(texidx);
    if(texobj==NULL) return false;

    if(mTexTargets[tex_unit]!=texobj->target)
    {
        if(mTexTargets[tex_unit]!=0)
            glDisable(mTexTargets[tex_unit]);
        glEnable(texobj->target);
    }

    assert(mActiveRenderTexture!=texobj->texid);

    if(m_bTexEnvStatesCheck)
        EnableAdditionalTexEnvStates();

    glBindTexture(texobj->target,texobj->texid);
    mTexTargets[tex_unit] = texobj->target;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::SetTexture(const uint32t *pTexIdx, uint32t tex_units_num)
{
    uint32t maxtexunits = (uint32t)CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_UNITS);
    if(tex_units_num > maxtexunits)
        return false;

    uint32t i = 0;
    for(i = 0; i < tex_units_num; i++)
    {
        if(!this->SetTexture(pTexIdx[i],i))
            return false;
    }

    // Check If Other Texture Units Are Still Active From Other Objects
    if(m_bTexUnitsActive)
    {
        for(uint32t k = i; k < maxtexunits; k++)
        {
            if(mTexTargets[k])
            {
                this->SetTexture((uint32t)0,k);
                mTexTargets[k] = 0;
            }
        }
    }

    glActiveTexture(GL_TEXTURE0_ARB);

    for(uint32t i = 0; i < TEX_TARGETS_SIZE; i++)
    {
        if(mTexTargets[i])
        {   
            m_bTexUnitsActive = true;
            break;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::DisableTextures()
{
    uint32t units = (uint32t)CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_UNITS);
    for(uint32t i = 0; i < units; i++)
    {
        if(mTexTargets[i]!=0)
        {
            glActiveTexture(GL_TEXTURE0_ARB+i);
            glDisable(mTexTargets[i]);
            if(m_bTexEnvStatesCheck)
                DisableAdditionalTexEnvStates();
        }
    }
    glActiveTexture(GL_TEXTURE0_ARB);

    memset(mTexTargets,0,sizeof(GLenum)*TEX_TARGETS_SIZE);
    m_bTexUnitsActive = false;
    m_bTexEnvStatesCheck = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTextureSampler(uint32t sampleridx, uint32t texunit)
{
    uint32t maxunits = (uint32t)CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_COMBINED_TEXTURE_UNITS); //GAPI_DEVPARAM_MAX_TEXTURE_UNITS
    if(texunit >= maxunits) 
        return false;

    if(texunit >= TEX_TARGETS_SIZE) return false;

    GLuint samplerid = 0;
    if(sampleridx)
    {
        GLuint *ptr = m_TexSamplerObjects.GetItem(sampleridx);
        if(!ptr) return false;
        samplerid = *ptr;
    }

    if(mTexSamplersActive[texunit]!=samplerid)
    {
        glBindSampler(texunit,samplerid);
        mTexSamplersActive[texunit] = samplerid;
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CGLTextureSystem::SetTextureSamplers(const uint32t *pSamplerIdx, uint32t tex_units_num)
{
    uint32t maxtexunits = (uint32t)CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_UNITS);
    if(tex_units_num > maxtexunits)
        return false;

    tex_units_num = (tex_units_num>TEX_TARGETS_SIZE)?(TEX_TARGETS_SIZE):(maxtexunits);
    for(uint32t i = 0; i < tex_units_num; i++)
    {
        if(!this->SetTextureSampler(pSamplerIdx[i],i))
            return false;
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    CGLTextureSystem::DisableTextureSamplers()
{
    uint32t maxtexunits = (uint32t)CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_UNITS);
    maxtexunits = (maxtexunits>TEX_TARGETS_SIZE)?(TEX_TARGETS_SIZE):(maxtexunits);
    for(uint32t i = 0; i < maxtexunits; i++)
        glBindSampler(i,0);

    memset(mTexSamplersActive,0,sizeof(GLuint)*TEX_TARGETS_SIZE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::IsSupportedTexFormat(TEXFMT fmt)
{
    if(CGLConvFunc::IsSignedFormat(fmt) && !(CGLDevInfo::GetDevInfoStruct().ARB_texture_snorm || CGLDevInfo::GetDevInfoStruct().EXT_texture_snorm)) 
        return false;

    if(CGLConvFunc::IsMixedFormat(fmt))
        return false;

    if(CGLConvFunc::IsCompressedFormat(fmt))
    {
        if((fmt >= TEXFMT_DXT1 && fmt <= TEXFMT_DXT5) && !CGLDevInfo::GetDevInfoStruct().EXT_texture_compression_s3tc)
            return false;
        if((fmt >= TEXFMT_ATI1N && fmt <= TEXFMT_ATI2N) && !CGLDevInfo::GetDevInfoStruct().EXT_texture_compression_rgtc)
            return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLTextureSystem::CalcMapSize(TEXFMT fmt, uint32t w, uint32t h, uint32t d, uint32t miplevels, uint32t align)
{
    uint32t mipnum = (uint32t) (logf(float(GLMAX(w,(uint32t)GLMAX(h,d)))) / LOG2F);
    if(miplevels > mipnum) return 0;
    if(CGLConvFunc::IsCompressedFormat(fmt) && (w%4 || h%4)) return 0;

    uint32t size = 0;
    uint32t level = miplevels+1;
    while(level)
    {
        uint32t woffset = (align > 0) ? (((w%align) == 0)?(0):(align - w%align)) : (0);
        uint32t fullwidth = w + align;
        if(CGLConvFunc::IsCompressedFormat(fmt))
            size += ((w+3)>>2) * ((h+3)>>2)* d;
        else
            size += fullwidth * h * d;

        w = GLMAX(w>>1,1);
        h = GLMAX(h>>1,1);
        d = GLMAX(d>>1,1);
        level--;
    }

    if(CGLConvFunc::IsCompressedFormat(fmt))
        size *= (uint32t)CGLConvFunc::GetBlockSize(fmt);
    else
        size *= (uint32t)CGLConvFunc::GetPixelSize(fmt);

    return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLenum  CGLTextureSystem::GetTexType(GLsizei w, GLsizei h, GLsizei d, uint32t flags)
{
    GLenum target = GL_NONE;
    target = (h>1)?(GL_TEXTURE_2D):(GL_TEXTURE_1D);
    target = (d>1&&h>1)?(GL_TEXTURE_3D):(target);
    target = (w==d && flags&GAPI_TEX_CUBE_MAP_FLAG)?(GL_TEXTURE_CUBE_MAP):(target);
    target = (target == GL_TEXTURE_2D && flags&GAPI_TEX_RECT_FLAG)?(GL_TEXTURE_RECTANGLE_ARB):(target);
    target = (target == GL_TEXTURE_2D && flags&GAPI_TEX_ARRAY_FLAG)?(GL_TEXTURE_2D_ARRAY):(target);
    target = (target == GL_TEXTURE_1D && flags&GAPI_TEX_ARRAY_FLAG)?(GL_TEXTURE_1D_ARRAY):(target);
    target = (target == GL_TEXTURE_CUBE_MAP && flags&GAPI_TEX_ARRAY_FLAG)?(GL_TEXTURE_CUBE_MAP_ARRAY):(target);

    return target;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::SetPixelAlign(GLint align)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_PACK_ALIGNMENT, align);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
uint32t CGLTextureSystem::CreateRenderTarget2D(GLsizei w, GLsizei h, TEXFMT rttexfmt, TEXFMT dstexfmt, bool genmipmaps)
{
    GLint rtintfmt = CGLConvFunc::GetGLTexInterFmt(rttexfmt);
    GLenum rtfmt = CGLConvFunc::GetGLTexFmt(rttexfmt);
    GLenum rttype = CGLConvFunc::GetGLTexDataType(rttexfmt);

    GLuint texID = 0;
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexImage2D(GL_TEXTURE_2D,0,rtintfmt,w,h,0,rtfmt,rttype,NULL);

    //
    //if(genmipmaps)
    //{
    //    GLsizei miplevels = (GLsizei)(log((float)(GLMAX(w,h)))/log(2.0f));
    //    for(GLsizei mipwidth=w,mipheight=h,level=1; level <= miplevels; mipwidth=GLMAX(mipwidth>>1,1), mipheight=GLMAX(mipheight>>1,1), level++)
    //        glTexImage2D(GL_TEXTURE_2D,level,rtintfmt,mipwidth,mipheight,0,rtfmt,rttype,NULL);
    //}


    uint32t texidx = m_TextureObjects.Reserve(1);
    GLTexture *tex_obj = m_TextureObjects.GetItem(texidx);
    memset(tex_obj,0,sizeof(CGLTextureSystem::GLTexture));

    GLuint rbo = 0;
    //Create Depth Buffer
    if((dstexfmt >= TEXFMT_DEPTH16) && (dstexfmt <= TEXFMT_DEPTH24))
    {
        glGenRenderbuffers(1,&rbo);
        glBindRenderbuffer(GL_RENDERBUFFER_EXT, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER_EXT, CGLConvFunc::GetGLTexInterFmt(dstexfmt), w, h);
        glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
    }

    //Create And Setup Framebuffer Object
    GLuint fbo = 0;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER_EXT, fbo);
    // Attach Color FrameBuffer
    GLenum attachment = (rttype==GL_DEPTH_COMPONENT) ? (GL_DEPTH_ATTACHMENT_EXT) : (GL_COLOR_ATTACHMENT0_EXT);
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, texID, 0);
    // Attach depth buffer
    if(rbo)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rbo);

    if(rttype==GL_DEPTH_COMPONENT)
    {
        // disable color buffer if you don't attach any color buffer image,
        // for example, rendering depth buffer only to a texture.
        // Otherwise, glCheckFramebufferStatusEXT will not be complete.
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // Setting Texture Object Params
    SetTexObjParams(*tex_obj,w,h,1,GL_TEXTURE_2D,texID,rtintfmt,rtfmt,rttype,genmipmaps,fbo,rbo,0);
    // Setup Texture Filters And Wrap Mode
    SetupTexFilters(*tex_obj,TEXFLT_LINEAR);
    SetupTexWrap(*tex_obj,TEXADDR_REPEAT);

    if (!CheckFrameBufferStatus())
    {
        DeleteTexture(texidx);
        return 0;
    } 

    return texidx;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGLTextureSystem::CheckFrameBufferStatus()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
    switch (status){
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            //INFOMSG("GL_FRAMEBUFFER_COMPLETE_EXT");
            return true;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            //ERRORMSG("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            //ERRORMSG("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
            break;
            //case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
            //	//ERRORMSG("GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT");
            //	break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            //ERRORMSG("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            //ERRORMSG("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            //ERRORMSG("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            //ERRORMSG("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            //ERRORMSG("GL_FRAMEBUFFER_UNSUPPORTED_EXT");
            break;
        default:
            //ERRORMSG("Unknown FBO error");
            break;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::SetupTexFilters(GLTexture &texobj, TEXFLT flt)
{
    //Set Requested Filter Mode
    texobj.magfilter = (flt >= TEXFLT_LINEAR) ? (GL_LINEAR) : (GL_NEAREST);
    texobj.minfilter = CGLConvFunc::GetGLTexFlt(flt);
    glTexParameteri(texobj.target, GL_TEXTURE_MAG_FILTER, texobj.magfilter);
    glTexParameteri(texobj.target, GL_TEXTURE_MIN_FILTER, texobj.minfilter);
    glTexEnvi(texobj.target,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    // Apply Anisotropic Filter Unless Explicitely Requested Otherwise
    if(flt >= TEXFLT_BILINEAR_ANISO)
    {
        texobj.anisotropy = CGLDevInfo::GetDevParam(GAPI_DEVPARAM_MAX_TEXTURE_ANISOTROPY);
        glTexParameteri(texobj.target, GL_TEXTURE_MAX_ANISOTROPY_EXT, texobj.anisotropy);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::SetupTexWrap(GLTexture &texobj, TEXADDR addr)
{
    GLint gladdr = CGLConvFunc::GetGLTexAddr(addr);
    glTexParameteri(texobj.target, GL_TEXTURE_WRAP_S, gladdr);
    if(texobj.target != GL_TEXTURE_1D) glTexParameteri(texobj.target, GL_TEXTURE_WRAP_T, gladdr);
    if(texobj.target == GL_TEXTURE_3D) glTexParameteri(texobj.target, GL_TEXTURE_WRAP_R, gladdr);

    texobj.wraps = gladdr;
    texobj.wrapr = gladdr;
    texobj.wrapt = gladdr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::SetupTexWrap(GLTexture &texobj, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR)
{
    texobj.wraps = CGLConvFunc::GetGLTexAddr(addrS);
    texobj.wrapr = CGLConvFunc::GetGLTexAddr(addrR);
    texobj.wrapt = CGLConvFunc::GetGLTexAddr(addrT);
    glTexParameteri(texobj.target, GL_TEXTURE_WRAP_S, texobj.wraps);
    if(texobj.target != GL_TEXTURE_1D) glTexParameteri(texobj.target, GL_TEXTURE_WRAP_T, texobj.wrapr);
    if(texobj.target == GL_TEXTURE_3D) glTexParameteri(texobj.target, GL_TEXTURE_WRAP_R, texobj.wrapt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::SetTexObjParams(GLTexture &texobj, GLsizei w, GLsizei h, GLsizei d, GLenum target, GLuint texid, GLint intfmt, 
                            GLenum fmt, GLenum type, bool genmipmaps, GLuint fbo, GLuint dbuf, GLuint sbuf)
{
    texobj.width = GLMAX(1,w);
    texobj.height = GLMAX(1,h);
    texobj.depth = GLMAX(1,d);
    texobj.target = target;
    texobj.texid = texid;
    texobj.internalformat = intfmt;
    texobj.format = fmt;
    texobj.datatype = type;
    texobj.anisotropy = 0;
    texobj.genmipmaps = genmipmaps;
    texobj.fbo = fbo;
    texobj.rbo = dbuf;
    texobj.sbo = sbuf;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::EnableTexEnvStatesCheck(bool flag)
{
    m_bTexEnvStatesCheck = flag;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::EnableAdditionalTexEnvStates()
{
    if(glIsEnabled(GL_POINT_SPRITE_ARB))
    {
        glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGLTextureSystem::DisableAdditionalTexEnvStates()
{
    if(glIsEnabled(GL_POINT_SPRITE_ARB))
    {
        glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_FALSE );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


