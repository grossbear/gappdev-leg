///////////////////////////////////////////////////////////////////////////////////////////////
//  GLTextureSystem.h 
//  OpenGL Textures Managment Singleton
//  Created 29-05-2010
//  Modified 12-01-2013
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GL_TEXTURESYSTEM_H_
#define _GL_TEXTURESYSTEM_H_

#define TEXOBJ_PTR_NUM 2
#define TEXOBJ_BLOCK_SIZE 2

#define SAMPLER_BLOCK_SIZE  16

#define TEX_TARGETS_SIZE 32

#include "Renderapi.h"
#include "Utils/BitArray.h"
#include "Utils/ArrayMap.h"


////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL Texture Manager Class
class CGLTextureSystem
{
public:
    static CGLTextureSystem* GetSingleton();
    ~CGLTextureSystem();

    //--uint32t CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT fmt, const void *data);
    uint32t CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT fmt, const void **data);
    bool    DeleteTexture(uint32t texidx);
    //--uint32t CreateRenderTarget(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT rtfmt, TEXFMT dsfmts);

    bool    UpdateTexture(uint32t texidx, uint32t miplevel, uint32t face, uint32t align, uint32t xoff, uint32t yoff, uint32t zoff, const void *data);

    bool    BindRenderTarget(uint32t texidx, uint32t face=0);
    bool    UnbindRenderTarget();

    bool    SetTexFilter(uint32t texidx, TEXFLT flt);
    bool    GetTexFilter(uint32t texidx, TEXFLT &flt);

    bool    SetTexAnisotropy(uint32t texidx, uint32t val);
    bool    GetTexAnisotropy(uint32t texidx, uint32t &val);

    bool    SetTexAddr(uint32t texidx, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR);
    bool    GetTexAddr(uint32t texidx, TEXADDR &addrS, TEXADDR &addrT, TEXADDR &addrR);

    bool    GetTexWidth(uint32t texidx, uint32t &width);
    bool    GetTexHeight(uint32t texidx, uint32t &height);
    bool    GetTexDepth(uint32t texidx, uint32t &depth);

    bool    SetTexture(uint32t texIdx, uint32t tex_unit);
    bool    SetTexture(const uint32t *pTexIdx, uint32t tex_unit_num);
    void    DisableTextures();

    uint32t CreateTextureSampler();
    bool    DeleteTextureSampler(uint32t sampleridx);
    bool    SetTexSamplerFilter(uint32t sampleridx, TEXFLT flt);
    bool    SetTexSamplerMinFilter(uint32t sampleridx, TEXFLT flt);
    bool    SetTexSamplerMagFilter(uint32t sampleridx, TEXFLT flt);
    bool    SetTexSamplerAddr(uint32t sampleridx, TEXADDR wrap);
    bool    SetTexSamplerAddr(uint32t sampleridx, TEXADDR sWrap, TEXADDR tWrap, TEXADDR rWrap);
    bool    SetTexSamplerMinLod(uint32t sampleridx, int32t lod);
    bool    SetTexSamplerMaxLod(uint32t sampleridx, int32t lod);
    bool    SetTexSamplerMaxAnisotroty(uint32t sampleridx, int32t anisotropy);
    bool    SetTexSamplerCompare(uint32t sampleridx, GAPIFUNCTION func, bool modeon);

    bool    SetTextureSampler(uint32t texidx, uint32t texunit);
    bool    SetTextureSamplers(const uint32t *pTexIdx, uint32t texunitcnt);
    void    DisableTextureSamplers();

    void    EnableTexEnvStatesCheck(bool flag);
    void    EnableAdditionalTexEnvStates();
    void    DisableAdditionalTexEnvStates();


protected:
    typedef struct sGLTexture
    {
        GLuint  texid;
        GLenum  target;
        GLint   internalformat;
        GLenum  format;
        GLenum  datatype;
        GLsizei width;
        GLsizei height;
        GLsizei depth;
        GLint   anisotropy;
        GLenum  magfilter;
        GLenum  minfilter;
        bool    genmipmaps;

        GLuint  fbo; // Framebuffer Object, For Rendering To Texture
        GLuint  rbo; // Depth Buffer
        GLuint  sbo; // Stencil Buffer

        //Texture Address
        GLint   wraps;
        GLint   wrapr;
        GLint   wrapt;
    }GLTexture;

    CGLTextureSystem();

    bool IsSupportedTexFormat(TEXFMT fmt);
    uint32t CalcMapSize(TEXFMT fmt, uint32t w, uint32t h, uint32t d, uint32t miplevels, uint32t align);
    GLenum GetTexType(GLsizei w, GLsizei h, GLsizei d, uint32t flags);
    void SetPixelAlign(GLint stride);

    uint32t CreateTextureImage(GLsizei width, GLsizei height, GLsizei depth, uint32t flags, TEXFMT fmt, const void *data);
    uint32t CreateCubeMapTexture(GLsizei width, GLsizei height, GLsizei depth, uint32t flags, TEXFMT fmt, const void **data);
    uint32t CreateRenderTarget(GLsizei width, GLsizei height, GLsizei depth, uint32t flags, TEXFMT rtfmt, TEXFMT dsfmt);

    //uint32t CreateRenderTarget2D(GLsizei w, GLsizei h, TEXFMT rtfmt, TEXFMT dsfmt, bool genmipmaps);
    bool CheckFrameBufferStatus();

    // Setting Texture Filters And Wrap Modes
    // Texture Must Be Active
    void SetupTexFilters(GLTexture &texobj, TEXFLT flt);
    void SetupTexWrap(GLTexture &texobj, TEXADDR addr);
    void SetupTexWrap(GLTexture &texobj, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR);

    inline void SetTexObjParams(GLTexture &texobj, GLsizei w, GLsizei h, GLsizei d, GLenum target, GLuint texid, GLint intfmt, 
        GLenum fmt, GLenum type, bool genMipMaps, GLuint fbo, GLuint dbuf, GLuint sbuf);

    static CGLTextureSystem *m_pInstance;
    CArrayMap<CGLTextureSystem::GLTexture,TEXOBJ_BLOCK_SIZE> m_TextureObjects;
    CArrayMap<GLuint,SAMPLER_BLOCK_SIZE> m_TexSamplerObjects;

    GLenum  mTexTargets[TEX_TARGETS_SIZE];
    bool    m_bTexUnitsActive;

    GLuint  mTexSamplersActive[TEX_TARGETS_SIZE];

    GLuint  mActiveRenderTexture;
    GLenum  mActiveRenderTarget;
    bool    mGenRTMipMaps;

    GLint   mViewport[4];

    bool    m_bTexEnvStatesCheck;
};



#endif //_GL_TEXTURESYSTEM_H_
