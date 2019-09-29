/////////////////////////////////////////////////////////////////////////////////////////////
// GLRender3.cpp
//
// OpenGL 3.x Rendering Core Class
/////////////////////////////////////////////////////////////////////////////////////////////


#include "GLRender3.h"
#include "GLDevContext.h"
#include "GLDevInfo.h"
#include "GLTextureSystem.h"
#include "GLShaderSystem.h"
#include "GLVBOSystem.h"
#include "GLConvFunc.h"
#include "GLVerProc.h"
#include "GLArbProc.h"
#include "GLTextureSystem.h"
#include "GLOcclusionQuery.h"

#include <GL/glu.h>
#pragma  comment(lib,"opengl32.lib")
#pragma  comment(lib,"glu32.lib")


/////////////////////////////////////////////////////////////////////////////////////////////
CGLRender3::CGLRender3()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
CGLRender3::~CGLRender3()
{
    CGLDevContext::DestroyRenderContext();
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::Create(const GAPICREATIONINFO *info)
{
    bool res = CGLDevContext::CreateRenderContext(info);
    if(!res)
        return false;

    CGLDevInfo::InitExtensions();
    glShadeModel(GL_SMOOTH);//--

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::Clear(bool color, bool depth, bool stencil)
{
    GLbitfield mask = 0;
    mask |= (color)?(GL_COLOR_BUFFER_BIT):(0);
    mask |= (depth)?(GL_DEPTH_BUFFER_BIT):(0);
    mask |= (stencil)?(GL_STENCIL_BUFFER_BIT):(0);

    glClear(mask);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::Render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.5,0.5,1, 0,0,0, 0,1,0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1,1,-1,1,-1,1);

    glMatrixMode(GL_MODELVIEW);

    float m1[16],m2[16];
    glGetFloatv(GL_MODELVIEW_MATRIX,m1);
    glGetFloatv(GL_PROJECTION_MATRIX,m2);
    int xn = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::Resize(int width, int height)
{
    glViewport(0, 0, width, height);
    
    /*
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, width, 0, height, -1.0, 1.0);
    float matrix[16]={0};
    glGetFloatv(GL_PROJECTION_MATRIX,matrix);
    glMatrixMode(GL_MODELVIEW);
    */
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SwapGraphicBuffers()
{
    bool res = CGLDevContext::SwapRenderBuffers();
    return res;
}
/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Device Caps
uint32t CGLRender3::GetDevCaps(GAPIDEVCAPSPARAM devparam)
{
    if(devparam < 3)
    {
        if(devparam == GAPI_DEVPARAM_MULTISAMPLE_SAMPLES)
            return (uint32t)CGLDevContext::GetMaxMultiSamples();
        else if(devparam == GAPI_DEVPARAM_VERTICAL_SYNC)
            return (uint32t)CGLDevContext::IsVSyncSupported();
        else if(devparam == GAPI_DEVPARAM_VERTICAL_SYNC_TEAR)
            return (uint32t)CGLDevContext::IsVSyncTearSupported();
    }
    
    return CGLDevInfo::GetDevParam(devparam);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Scissor Setting Functions
void CGLRender3::EnableScissorTest(bool flag)
{
    (flag) ? (glEnable(GL_SCISSOR_TEST)) : (glDisable(GL_SCISSOR_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsScissorTestEnabled()
{
    return glIsEnabled(GL_SCISSOR_TEST)==GL_TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetScissorRect(int x, int y, int width, int height)
{
    glScissor(x,y,width,height);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::GetScissorRect(int scissor_box[4])
{
    glGetIntegerv(GL_SCISSOR_BOX,scissor_box);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Depth Setting Functions
void CGLRender3::EnableDepthTest(bool flag)
{
    (flag) ? (glEnable(GL_DEPTH_TEST)) : (glDisable(GL_DEPTH_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::EnableDepthWrite(bool flag)
{
    (flag) ? (glDepthMask(GL_TRUE)) : (glDepthMask(GL_FALSE));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetDepthFunc(GAPIFUNC func)
{
    glDepthFunc(CGLConvFunc::GetGLFunc(func));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetClearDepth(float depth)
{
    GLclampd fClearDepth = (GLclampd)max(0.0f,min(depth,1.0f));
    glClearDepth(fClearDepth);
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsDepthTestEnabled()
{
    return glIsEnabled(GL_DEPTH_TEST)==GL_TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsDepthWriteEnabled()
{
    return glIsEnabled(GL_DEPTH_WRITEMASK)==GL_TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
float CGLRender3::GetClearDepth()
{
    float depthvalue;
    glGetFloatv(GL_DEPTH_CLEAR_VALUE,&depthvalue);
    return depthvalue;
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIFUNC CGLRender3::GetDepthFunc()
{
    GLenum func;
    glGetIntegerv(GL_DEPTH_FUNC,(GLint*)&func);

    return CGLConvFunc::GetGAPIFunc(func);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Stencil Setting Functions
void CGLRender3::EnableStencilTest(bool flag)
{
    (flag) ? (glEnable(GL_STENCIL_TEST)) : (glDisable(GL_STENCIL_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetStencilWriteMask(uint32t mask)
{
    glStencilMask((GLuint)mask);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsStencilTestEnabled()
{
    return (glIsEnabled(GL_STENCIL_TEST)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetStencilFunc(GAPIFUNC func, uint32t ref, uint32t mask)
{
    glStencilFunc(CGLConvFunc::GetGLFunc(func),(GLint)ref,(GLuint)mask);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetStencilOp(GAPIOP fail, GAPIOP zfail, GAPIOP pass)
{
    glStencilOp(CGLConvFunc::GetGLOp(fail),CGLConvFunc::GetGLOp(zfail),CGLConvFunc::GetGLOp(pass));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::EnableTwoSideStencil(bool flag)
{
    (flag) ? (glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT)) : (glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetClearStencil(uint32t val)
{
    glClearStencil((GLint)val);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsTwoSideStencilEnabled()
{
    return (glIsEnabled(GL_STENCIL_TEST_TWO_SIDE_EXT)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetStencilWriteMask()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_WRITEMASK,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIFUNC CGLRender3::GetStencilFunc()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_FUNC,&param);

    return CGLConvFunc::GetGAPIFunc((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetStencilBits()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_BITS,&param);

    return (uint32t) param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetStencilClearVal()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_CLEAR_VALUE,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetStencilFuncMask()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_VALUE_MASK,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetStencilFuncRef()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_REF,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIOP CGLRender3::GetStencilOpFail()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_FAIL,&param);

    return CGLConvFunc::GetGAPIOp((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIOP CGLRender3::GetStencilOpDepthFail()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL,&param);

    return CGLConvFunc::GetGAPIOp((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIOP CGLRender3::GetStencilOpDepthPass()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS,&param);

    return CGLConvFunc::GetGAPIOp((GLenum)param);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetStencilFuncSeparate(GAPIFUNC frontfunc, GAPIFUNC backfunc, uint32t ref, uint32t mask)
{
    glStencilFuncSeparate(CGLConvFunc::GetGLFunc(frontfunc),CGLConvFunc::GetGLFunc(backfunc),(GLint)ref, (GLuint)mask);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetStencilOpSeparate(bool frontface, GAPIOP fail, GAPIOP zfail, GAPIOP pass)
{
    GLenum face = (frontface==true)?(GL_FRONT):(GL_BACK);
    glStencilOpSeparate(face,CGLConvFunc::GetGAPIOp(fail),CGLConvFunc::GetGAPIOp(zfail),CGLConvFunc::GetGAPIOp(pass));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetStencilMaskSeparate(bool frontface, uint32t mask)
{
    GLenum face = (frontface==true)?(GL_FRONT):(GL_BACK);
    glStencilMaskSeparate(face,(GLuint)mask);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetRect(int32t x1, int32t y1, int32t x2, int32t y2)
{
    glRecti(x1,y1,x2,y2);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetRect(float x1, float y1, float x2, float y2)
{
    glRectf(x1,y1,x2,y2);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetRasterPos(int32t x, int32t y)
{
    glRasterPos2i(x,y);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetRasterPos(float x, float y)
{
    glRasterPos2f(x,y);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetColor(ubyte red, ubyte green, ubyte blue, ubyte alpha)
{
    glColor4ub(red,green,blue,alpha);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetColor(ubyte color[4])
{
    glColor4ubv(color);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetColor(unsigned color)
{
    glColor4ubv((const GLubyte*)&color);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawBitmap(int width, int height, float xorig, float yorig, float xmove, float ymove, const ubyte *bitmap)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(width,height,xorig,yorig,xmove,ymove,bitmap);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Matrix Handling
void CGLRender3::SetProjMatrix(const float matrix[16])
{  
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrix);
    glMatrixMode(GL_MODELVIEW);
    
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, g_width, 0, g_height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    */
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Viewport Setting
void CGLRender3::SetViewport(int x, int y, int width, int height)
{
    glViewport(x,y,width,height);
    glLoadIdentity();
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::GetViewport(int rect[4])
{
    glGetIntegerv(GL_VIEWPORT,rect);
}

//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Alpha Test
void CGLRender3::EnableAlphaTest(bool flag)
{
    (flag) ? (glEnable(GL_ALPHA_TEST)) : (glDisable(GL_ALPHA_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsAlphaTestEnabled()
{
    return (glIsEnabled(GL_ALPHA_TEST)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetAlphaFunc(GAPIFUNC func, float ref)
{
    glAlphaFunc(CGLConvFunc::GetGLFunc(func),(GLclampf)ref);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIFUNC CGLRender3::GetAlphaTestFunc()
{
    GLint param;
    glGetIntegerv(GL_ALPHA_TEST_FUNC, &param);

    return CGLConvFunc::GetGAPIFunc((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
float CGLRender3::GetAlphaTestRef()
{
    GLfloat param;
    glGetFloatv(GL_ALPHA_TEST_REF, &param);

    return param;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Alpha To Coverage Test
void CGLRender3::EnableSampleAlphaToMask(bool flag)
{
    (flag) ? (glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB)) : (glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsSampleAlphaToMaskEnabled()
{
    return (glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetSampleCoverage(float val, bool invert)
{
    glSampleCoverage((GLclampf)val,(GLboolean)invert);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::GetSampleCoverage(float &val, bool &invert)
{
    GLfloat fparam = 0.0f;
    GLboolean bparam = GL_FALSE;
    glGetFloatv(GL_SAMPLE_COVERAGE_VALUE,&fparam);
    glGetBooleanv(GL_SAMPLE_COVERAGE_INVERT,&bparam);
    val = (float)fparam;
    invert = (bparam==GL_TRUE)?(true):(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Color Blending
void CGLRender3::EnableColorBlend(bool flag)
{
    (flag) ? (glEnable(GL_BLEND)) : (glDisable(GL_BLEND));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::IsColorBlendEnabled()
{
    return (glIsEnabled(GL_BLEND)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetBlendFunc(GAPIBLENDOP src, GAPIBLENDOP dest)
{
    glBlendFunc(CGLConvFunc::GetGLBlendOp(src),CGLConvFunc::GetGLBlendOp(dest));
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIBLENDOP CGLRender3::GetSrcBlendFunc()
{
    GLenum param;
    glGetIntegerv(GL_BLEND_SRC, (GLint*)&param);

    return CGLConvFunc::GetGAPIBlendOp(param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIBLENDOP CGLRender3::GetDestBlendFunc()
{
    GLenum param;
    glGetIntegerv(GL_BLEND_DST, (GLint*)&param);

    return CGLConvFunc::GetGAPIBlendOp(param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetBlendEquation(GAPIBLENDFUNC mode)
{
    glBlendEquationEXT(CGLConvFunc::GetGLBlendFunc(mode));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetBlendEquationSeparate(GAPIBLENDFUNC modeRGB, GAPIBLENDFUNC modeAlpha)
{
    glBlendEquationSeparate(CGLConvFunc::GetGLBlendFunc(modeRGB),CGLConvFunc::GetGLBlendFunc(modeAlpha));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetBlendFuncSeparate(GAPIBLENDOP srcRGB, GAPIBLENDOP destRGB, GAPIBLENDOP srcAlpha, GAPIBLENDOP destAlpha)
{
    glBlendFuncSeparate(CGLConvFunc::GetGLBlendOp(srcRGB),CGLConvFunc::GetGLBlendOp(destRGB),
        CGLConvFunc::GetGLBlendOp(srcAlpha),CGLConvFunc::GetGLBlendOp(destAlpha));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetBlendColor(float red, float green, float blue, float alpha)
{
    glBlendColorEXT(red,green,blue,alpha);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Setting Framebuffer Clear Color
void CGLRender3::SetClearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red,green,blue,alpha);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Fill Mode
void CGLRender3::SetFillMode(GAPIFILLMODE mode)
{
    static GLenum fillmodetab[] = {GL_FILL,GL_POINT,GL_LINE,GL_FILL};
    GLenum fillmode = fillmodetab[mode];
    glPolygonMode(GL_FRONT,fillmode);
	glPolygonMode(GL_BACK,fillmode);
}
//////////////////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Cull Mode
void CGLRender3::SetCullMode(bool CullFace,bool CCW)
{
    if(CCW)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);

	if(CullFace)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}
//////////////////////////////////////////////////////////////////////////////////////////////
// Setting Shading Mode (Gouraud or Flat Shading)
void CGLRender3::SetShadingMode(bool bsmooth)
{
    if(bsmooth)
		glShadeModel(GL_SMOOTH); //Gouraud shading
	else
		glShadeModel(GL_FLAT);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Occlusion Query Managment Functions
uint32t CGLRender3::CreateQueryObject()
{
    return CGLOcclusionQuery::GetSingleton()->CreateQueryObject();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::DeleteQueryObject(uint32t objidx)
{
    return CGLOcclusionQuery::GetSingleton()->DeleteQueryObject(objidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::BeginQuery(uint32t objidx)
{
    CGLOcclusionQuery::GetSingleton()->BeginQuery(objidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::EndQuery()
{
    CGLOcclusionQuery::GetSingleton()->EndQuery();
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetQueryObjects(uint32t objidx)
{
    return CGLOcclusionQuery::GetSingleton()->GetQueryObjects(objidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::QueryStarted()
{
    return CGLOcclusionQuery::GetSingleton()->QueryStarted();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Texture Managment Functions
uint32t CGLRender3::CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT fmt, const void **data)
{
    return CGLTextureSystem::GetSingleton()->CreateTexture(width,height,depth,flags,fmt,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::DeleteTexture(uint32t texidx)
{
    return CGLTextureSystem::GetSingleton()->DeleteTexture(texidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::UpdateTexture(uint32t texidx, uint32t miplevel, uint32t face, uint32t align, uint32t xoff, uint32t yoff, uint32t zoff, const void *data)
{
    return CGLTextureSystem::GetSingleton()->UpdateTexture(texidx,miplevel,face,align,xoff,yoff,zoff,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::BindRenderTarget(uint32t texidx, uint32t face)
{
    return CGLTextureSystem::GetSingleton()->BindRenderTarget(texidx,face);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::UnbindRenderTarget()
{
    return CGLTextureSystem::GetSingleton()->UnbindRenderTarget();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexFilter(uint32t texidx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexFilter(texidx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::GetTexFilter(uint32t texidx, TEXFLT &flt)
{
    return CGLTextureSystem::GetSingleton()->GetTexFilter(texidx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexAnisotropy(uint32t texidx, uint32t val)
{
    return CGLTextureSystem::GetSingleton()->SetTexAnisotropy(texidx,val);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::GetTexAnisotropy(uint32t texidx, uint32t &val)
{
    return CGLTextureSystem::GetSingleton()->GetTexAnisotropy(texidx,val);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexAddr(uint32t texidx, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR)
{
    return CGLTextureSystem::GetSingleton()->SetTexAddr(texidx,addrS,addrT,addrR);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::GetTexAddr(uint32t texidx, TEXADDR &addrS, TEXADDR &addrT, TEXADDR &addrR)
{
    return CGLTextureSystem::GetSingleton()->GetTexAddr(texidx,addrS,addrT,addrR);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::GetTexWidth(uint32t texidx, uint32t &width)
{
    return CGLTextureSystem::GetSingleton()->GetTexWidth(texidx,width);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::GetTexHeight(uint32t texidx, uint32t &height)
{
    return CGLTextureSystem::GetSingleton()->GetTexHeight(texidx,height);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::GetTexDepth(uint32t texidx, uint32t &depth)
{
    return CGLTextureSystem::GetSingleton()->GetTexDepth(texidx,depth);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexture(uint32t texIdx, uint32t tex_unit)
{
    return CGLTextureSystem::GetSingleton()->SetTexture(texIdx,tex_unit);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexture(const uint32t *pTexIdx, uint32t tex_unit_num)
{
    return CGLTextureSystem::GetSingleton()->SetTexture(pTexIdx,tex_unit_num);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DisableTextures()
{
    return CGLTextureSystem::GetSingleton()->DisableTextures();
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::CreateTextureSampler()
{
    return CGLTextureSystem::GetSingleton()->CreateTextureSampler();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::DeleteTextureSampler(uint32t sampleridx)
{
    return CGLTextureSystem::GetSingleton()->DeleteTextureSampler(sampleridx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerFilter(uint32t sampleridx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerFilter(sampleridx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerMinFilter(uint32t sampleridx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMinFilter(sampleridx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerMagFilter(uint32t sampleridx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMagFilter(sampleridx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerAddr(uint32t sampleridx, TEXADDR wrap)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerAddr(sampleridx,wrap);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerAddr(uint32t sampleridx, TEXADDR sWrap, TEXADDR tWrap, TEXADDR rWrap)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerAddr(sampleridx,sWrap,tWrap,rWrap);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerMinLod(uint32t sampleridx, int32t lod)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMinLod(sampleridx,lod);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerMaxLod(uint32t sampleridx, int32t lod)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMaxLod(sampleridx,lod);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerMaxAnisotroty(uint32t sampleridx, int32t anisotropy)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMaxAnisotroty(sampleridx,anisotropy);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTexSamplerCompare(uint32t sampleridx, GAPIFUNCTION func, bool modeon)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerCompare(sampleridx,func,modeon);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTextureSampler(uint32t texidx, uint32t texunit)
{
    return CGLTextureSystem::GetSingleton()->SetTextureSampler(texidx,texunit);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetTextureSamplers(const uint32t *pTexIdx, uint32t texunitcnt)
{
    return CGLTextureSystem::GetSingleton()->SetTextureSamplers(pTexIdx,texunitcnt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DisableTextureSamplers()
{
    return CGLTextureSystem::GetSingleton()->DisableTextureSamplers();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::EnableTexEnvStatesCheck(bool flag)
{
    return CGLTextureSystem::GetSingleton()->EnableTexEnvStatesCheck(flag);
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Shaders Management Functions
uint32t CGLRender3::CreateShader(const char *strshader)
{
    return CGLShaderSystem::GetSingleton()->CreateShader(strshader);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::DeleteShader(uint32t shaderidx)
{
    return CGLShaderSystem::GetSingleton()->DeleteShader(shaderidx);
}

//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::CreateShaderParamsGroup(uint32t reservesize)
{
    return CGLShaderSystem::GetSingleton()->CreateShaderParamsGroup(reservesize);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::DeleteShaderParamsGroup(uint32t shaderidx)
{
    return CGLShaderSystem::GetSingleton()->DeleteShaderParamsGroup(shaderidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::AddShaderParam(uint32t groupidx, const char *name, GAPIUNIFORMTYPE type, const void *data)
{
    return CGLShaderSystem::GetSingleton()->AddShaderParam(groupidx,name,type,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::UpdateShaderParam(uint32t groupidx, uint32t paramidx, const void *data)
{
    return CGLShaderSystem::GetSingleton()->UpdateShaderParam(groupidx,paramidx,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetShaderParams(uint32t shaderidx, uint32t groupidx, uint32t commgroupidx)
{
    return CGLShaderSystem::GetSingleton()->SetShaderParams(shaderidx,groupidx,commgroupidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetShaderParamIndex(uint32t groupidx, const char *name)
{
    return CGLShaderSystem::GetSingleton()->GetShaderParamIndex(groupidx,name);
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::CreateUniformBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data)
{
    return CGLShaderSystem::GetSingleton()->CreateUniformBuffer(usage,bufsize,data);
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::AddUniformBlocks(uint32t shaderidx, uint32t *bufidx, const char **names, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->AddUniformBlocks(shaderidx,bufidx,names,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetShader(uint32t shaderidx)
{
    return CGLShaderSystem::GetSingleton()->SetShader(shaderidx);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Shader Uniform Block Handling Functions
bool CGLRender3::SetShaderUniform(GAPIUNIFORMTYPE type, const char *name, const void *data)
{
    return CGLShaderSystem::GetSingleton()->SetUniform(type,name,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetShaderProgramUniform(uint32t shaderidx, GAPIUNIFORMTYPE type, const char *name, const void *data)
{
    return CGLShaderSystem::GetSingleton()->SetProgramUniform(shaderidx,type,name,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetShaderUniforms(GAPIUNIFORMDATA *data, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->SetUniforms(data,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetShaderProgramUniforms(uint32t shaderidx, GAPIUNIFORMDATA *data, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->SetProgramUniforms(shaderidx,data,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetUniformBlockSize(uint32t shaderidx, const char *blockname)
{
    return CGLShaderSystem::GetSingleton()->GetUniformBlockSize(shaderidx,blockname);
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::GetActiveUniformInfo(uint32t shaderidx, GAPIACTIVEUNIFORMTYPE type, const char **names, uint32t *pvalues, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->GetActiveUniformInfo(shaderidx,type,names,pvalues,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetUniformBlockBinding(uint32t shaderidx, const char *blockname, uint32t bindingpoint)
{
    return CGLShaderSystem::GetSingleton()->SetUniformBlockBinding(shaderidx,blockname,bindingpoint);
}
//////////////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------
uint32t CGLRender3::GetUniformBufferBindingPoint(uint32t bufidx)
{
    return CGLShaderSystem::GetSingleton()->GetUniformBufferBindingPoint(bufidx);
}
-------------------------------------------*/

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetUniformBufferBinding(uint32t bufidx, uint32t bindpoint)
{
    return CGLShaderSystem::GetSingleton()->SetUniformBufferBindingPoint(bufidx,bindpoint);
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Fragment Data Location
void CGLRender3::SetFragDataLocation(uint32t shaderidx, uint32t colornumber, const char *name)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocation(shaderidx,colornumber,name);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetFragDataLocation(uint32t shaderidx, uint32t *colorsnumbers, const char **names, uint32t count)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocation(shaderidx,colorsnumbers,names,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetFragDataLocation(uint32t shaderidx, const char *name)
{
    return (uint32t)CGLShaderSystem::GetSingleton()->GetFragDataLocation(shaderidx,name);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Blend Func Extended
void CGLRender3::SetFragDataLocationIndexed(uint32t shaderidx, uint32t colornumber, uint32t index, const char *name)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocationIndexed(shaderidx,colornumber,index,name);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetFragDataLocationIndexed(uint32t shaderidx, uint32t *colorsnumbers, uint32t *indices, const char **names, uint32t count)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocationIndexed(shaderidx, colorsnumbers,indices,names,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetFragDataIndex(uint32t shaderidx, const char *name)
{
    return CGLShaderSystem::GetSingleton()->GetFragDataIndex(shaderidx,name);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Vertex Object Managment Functions
uint32t CGLRender3::CreateVertexBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data)
{
    return CGLVBOSystem::GetSingleton()->CreateVertexBuffer(usage,bufsize,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::UpdateVertexBuffer(uint32t bufid, uint32t first, uint32t size, const void *data)
{
    return CGLVBOSystem::GetSingleton()->UpdateVertexBuffer(bufid,first,size,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::DeleteVertexBuffer(uint32t bufid)
{
    return CGLVBOSystem::GetSingleton()->DeleteVertexBuffer(bufid);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender3::SetVertexBuffer(uint32t bufferid)
{
    return CGLVBOSystem::GetSingleton()->BindBuffer(bufferid);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Direct Vertex Attribute Management Functions
void CGLRender3::SetVertexAttribPointer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, const void *pointer)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribPointer(stream,size,type,normalize,stride,pointer);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetVertexAttribBuffer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, uint32t offset, uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBuffer(stream,size,type,normalize,stride,offset,buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetVertexAttribPointerI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, const void *pointer)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribPointerI(stream,size,type,stride,pointer);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetVertexAttribBufferI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, uint32t offset, uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBufferI(stream,size,type,stride,offset,buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetVertexAttribPointerL(uint32t stream, uint32t size, uint32t stride, const void *pointer)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribPointerL(stream,size,stride,pointer);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetVertexAttribBufferL(uint32t stream, uint32t size, uint32t stride, uint32t offset, uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBufferL(stream,size,stride,offset,buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetVertexAttribBuffers(GAPIVERTATTRIBDATA *vert_attrib_data, uint32t count)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBuffers(vert_attrib_data,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender3::GetVertexAttribLocation(uint32t shaderidx, const char *attribname)
{
    /*
    GLhandleARB handle = CGLShaderSystem::GetSingleton()->GetShaderProgram(shaderidx);
    GLuint loc = glGetAttribLocation(handle,attribname);
    return loc;
    */

    return CGLShaderSystem::GetSingleton()->GetAttribLocation(shaderidx,attribname);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::SetIndexBuffer(uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetIndexBuffer(buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DisableVertexAttribPointers()
{
    CGLVBOSystem::GetSingleton()->DisableVertexAttribPointers();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Direct Drawing Functions
void CGLRender3::DrawArrays(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count)
{
    GLenum primmode = CGLConvFunc::GetGLPrimitiveType(mode);
    glDrawArrays(primmode,(GLint)first,(GLsizei)count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawElements(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices)
{
    glDrawElements(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices);
}

//--CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices);
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::MultiDrawArrays(GAPIPRIMITIVETYPE mode, const uint32t *first, const uint32t *count, uint32t drawcount)
{
    glMultiDrawArrays(CGLConvFunc::GetGLPrimitiveType(mode),(const GLint*)first,(const GLsizei*)count,(GLsizei)drawcount);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::MultiDrawElements(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount)
{
    glMultiDrawElements(CGLConvFunc::GetGLPrimitiveType(mode),(const GLsizei*)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)drawcount);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawRangeElements(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices)
{
    glDrawRangeElementsEXT(CGLConvFunc::GetGLPrimitiveType(mode),start,end,count,CGLConvFunc::GetGLDataType(type),indices);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawArraysInstanced(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instancecount)
{
    glDrawArraysInstanced(CGLConvFunc::GetGLPrimitiveType(mode),first,(GLsizei)count,(GLsizei)instancecount);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawElementsInstanced(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount)
{
    glDrawElementsInstanced(mode,(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)instancecount);
}
//////////////////////////////////////////////////////////////////////////////////////////////
/*
typedef struct {
      GLuint count;
      GLuint primCount;
      GLuint first;
      GLuint reservedMustBeZero;
    } DrawArraysIndirectCommand;
*/
void CGLRender3::DrawArraysInderict(GAPIPRIMITIVETYPE mode, const void *indirect)
{
    glDrawArraysIndirect(CGLConvFunc::GetGLPrimitiveType(mode),indirect);
}
//////////////////////////////////////////////////////////////////////////////////////////////
/*
typedef struct {
      GLuint count;
      GLuint primCount;
      GLuint firstIndex;
      GLint  baseVertex;
      GLuint reservedMustBeZero;
    } DrawElementsIndirectCommand;
*/
void CGLRender3::DrawElementsIndirect(GAPIPRIMITIVETYPE mode, GAPIDATATYPE type, const void *indirect)
{
    glDrawElementsIndirect(CGLConvFunc::GetGLPrimitiveType(mode),CGLConvFunc::GetGLDataType(type),indirect);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawArraysInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instancecount, uint32t baseinstance)
{
    glDrawArraysInstancedBaseInstance(CGLConvFunc::GetGLPrimitiveType(mode),(GLint)first,(GLsizei)count,(GLsizei)instancecount,baseinstance);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::MultiDrawArraysIndirect(GAPIPRIMITIVETYPE mode, const void *indirect, uint32t drawcount, uint32t stride)
{
    glMultiDrawArraysIndirect(CGLConvFunc::GetGLPrimitiveType(mode),indirect,(GLsizei)drawcount,(GLsizei)stride);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawElementsInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, uint32t baseinstance)
{
    glDrawElementsInstancedBaseInstance(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)instancecount,baseinstance);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex)
{
    glDrawElementsBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,basevertex);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawRangeElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex)
{
    glDrawRangeElementsBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),start,end,(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,basevertex);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawElementsInstancedBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex)
{
    glDrawElementsInstancedBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)instancecount,basevertex);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::DrawElementsInstancedBaseVertexBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex, uint32t baseinstance)
{
    glDrawElementsInstancedBaseVertexBaseInstance(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)instancecount,(GLint)basevertex,(GLuint)baseinstance);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender3::MultiDrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount, int32t *basevertex)
{
    glMultiDrawElementsBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),(const GLsizei*)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)drawcount,basevertex);
}




/////////////////////////////////////////////////////////////////////////////////////////////