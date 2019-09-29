/////////////////////////////////////////////////////////////////////////////////////////////
// GLRender3.cpp
//
// OpenGL 3.x Rendering Core Class
/////////////////////////////////////////////////////////////////////////////////////////////


#include "GLRender.h"
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
CGLRender::CGLRender()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////
CGLRender::~CGLRender()
{
    CGLDevContext::DestroyRenderContext();
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::Create(const GAPICREATIONINFO *info)
{
    bool res = CGLDevContext::CreateRenderContext(info);
    if(!res)
        return false;

    CGLDevInfo::InitExtensions();
    glShadeModel(GL_SMOOTH);//--

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::Clear(bool color, bool depth, bool stencil)
{
    GLbitfield mask = 0;
    mask |= (color)?(GL_COLOR_BUFFER_BIT):(0);
    mask |= (depth)?(GL_DEPTH_BUFFER_BIT):(0);
    mask |= (stencil)?(GL_STENCIL_BUFFER_BIT):(0);

    glClear(mask);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::Render()
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
void CGLRender::Resize(int width, int height)
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
bool CGLRender::SwapGraphicBuffers()
{
    bool res = CGLDevContext::SwapRenderBuffers();
    return res;
}
/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Device Caps
uint32t CGLRender::GetDevCaps(GAPIDEVCAPSPARAM devparam)
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
void CGLRender::EnableScissorTest(bool flag)
{
    (flag) ? (glEnable(GL_SCISSOR_TEST)) : (glDisable(GL_SCISSOR_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsScissorTestEnabled()
{
    return glIsEnabled(GL_SCISSOR_TEST)==GL_TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetScissorRect(int x, int y, int width, int height)
{
    glScissor(x,y,width,height);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::GetScissorRect(int scissor_box[4])
{
    glGetIntegerv(GL_SCISSOR_BOX,scissor_box);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Depth Setting Functions
void CGLRender::EnableDepthTest(bool flag)
{
    (flag) ? (glEnable(GL_DEPTH_TEST)) : (glDisable(GL_DEPTH_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::EnableDepthWrite(bool flag)
{
    (flag) ? (glDepthMask(GL_TRUE)) : (glDepthMask(GL_FALSE));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetDepthFunc(GAPIFUNC func)
{
    glDepthFunc(CGLConvFunc::GetGLFunc(func));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetClearDepth(float depth)
{
    GLclampd fClearDepth = (GLclampd)max(0.0f,min(depth,1.0f));
    glClearDepth(fClearDepth);
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsDepthTestEnabled()
{
    return glIsEnabled(GL_DEPTH_TEST)==GL_TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsDepthWriteEnabled()
{
    return glIsEnabled(GL_DEPTH_WRITEMASK)==GL_TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
float CGLRender::GetClearDepth()
{
    float depthvalue;
    glGetFloatv(GL_DEPTH_CLEAR_VALUE,&depthvalue);
    return depthvalue;
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIFUNC CGLRender::GetDepthFunc()
{
    GLenum func;
    glGetIntegerv(GL_DEPTH_FUNC,(GLint*)&func);

    return CGLConvFunc::GetGAPIFunc(func);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Stencil Setting Functions
void CGLRender::EnableStencilTest(bool flag)
{
    (flag) ? (glEnable(GL_STENCIL_TEST)) : (glDisable(GL_STENCIL_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetStencilWriteMask(uint32t mask)
{
    glStencilMask((GLuint)mask);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsStencilTestEnabled()
{
    return (glIsEnabled(GL_STENCIL_TEST)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetStencilFunc(GAPIFUNC func, uint32t ref, uint32t mask)
{
    glStencilFunc(CGLConvFunc::GetGLFunc(func),(GLint)ref,(GLuint)mask);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetStencilOp(GAPIOP fail, GAPIOP zfail, GAPIOP pass)
{
    glStencilOp(CGLConvFunc::GetGLOp(fail),CGLConvFunc::GetGLOp(zfail),CGLConvFunc::GetGLOp(pass));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::EnableTwoSideStencil(bool flag)
{
    (flag) ? (glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT)) : (glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetClearStencil(uint32t val)
{
    glClearStencil((GLint)val);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsTwoSideStencilEnabled()
{
    return (glIsEnabled(GL_STENCIL_TEST_TWO_SIDE_EXT)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetStencilWriteMask()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_WRITEMASK,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIFUNC CGLRender::GetStencilFunc()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_FUNC,&param);

    return CGLConvFunc::GetGAPIFunc((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetStencilBits()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_BITS,&param);

    return (uint32t) param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetStencilClearVal()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_CLEAR_VALUE,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetStencilFuncMask()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_VALUE_MASK,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetStencilFuncRef()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_REF,&param);

    return (uint32t)param;
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIOP CGLRender::GetStencilOpFail()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_FAIL,&param);

    return CGLConvFunc::GetGAPIOp((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIOP CGLRender::GetStencilOpDepthFail()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL,&param);

    return CGLConvFunc::GetGAPIOp((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIOP CGLRender::GetStencilOpDepthPass()
{
    GLint param;
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS,&param);

    return CGLConvFunc::GetGAPIOp((GLenum)param);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetStencilFuncSeparate(GAPIFUNC frontfunc, GAPIFUNC backfunc, uint32t ref, uint32t mask)
{
    glStencilFuncSeparate(CGLConvFunc::GetGLFunc(frontfunc),CGLConvFunc::GetGLFunc(backfunc),(GLint)ref, (GLuint)mask);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetStencilOpSeparate(bool frontface, GAPIOP fail, GAPIOP zfail, GAPIOP pass)
{
    GLenum face = (frontface==true)?(GL_FRONT):(GL_BACK);
    glStencilOpSeparate(face,CGLConvFunc::GetGAPIOp(fail),CGLConvFunc::GetGAPIOp(zfail),CGLConvFunc::GetGAPIOp(pass));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetStencilMaskSeparate(bool frontface, uint32t mask)
{
    GLenum face = (frontface==true)?(GL_FRONT):(GL_BACK);
    glStencilMaskSeparate(face,(GLuint)mask);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetRect(int32t x1, int32t y1, int32t x2, int32t y2)
{
    glRecti(x1,y1,x2,y2);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetRect(float x1, float y1, float x2, float y2)
{
    glRectf(x1,y1,x2,y2);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetRasterPos(int32t x, int32t y)
{
    glRasterPos2i(x,y);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetRasterPos(float x, float y)
{
    glRasterPos2f(x,y);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetColor(ubyte red, ubyte green, ubyte blue, ubyte alpha)
{
    glColor4ub(red,green,blue,alpha);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetColor(ubyte color[4])
{
    glColor4ubv(color);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetColor(unsigned color)
{
    glColor4ubv((const GLubyte*)&color);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawBitmap(int width, int height, float xorig, float yorig, float xmove, float ymove, const ubyte *bitmap)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(width,height,xorig,yorig,xmove,ymove,bitmap);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Matrix Handling
void CGLRender::SetProjMatrix(const float matrix[16])
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
void CGLRender::SetViewport(int x, int y, int width, int height)
{
    glViewport(x,y,width,height);
    glLoadIdentity();
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::GetViewport(int rect[4])
{
    glGetIntegerv(GL_VIEWPORT,rect);
}

//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Alpha Test
void CGLRender::EnableAlphaTest(bool flag)
{
    (flag) ? (glEnable(GL_ALPHA_TEST)) : (glDisable(GL_ALPHA_TEST));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsAlphaTestEnabled()
{
    return (glIsEnabled(GL_ALPHA_TEST)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetAlphaFunc(GAPIFUNC func, float ref)
{
    glAlphaFunc(CGLConvFunc::GetGLFunc(func),(GLclampf)ref);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIFUNC CGLRender::GetAlphaTestFunc()
{
    GLint param;
    glGetIntegerv(GL_ALPHA_TEST_FUNC, &param);

    return CGLConvFunc::GetGAPIFunc((GLenum)param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
float CGLRender::GetAlphaTestRef()
{
    GLfloat param;
    glGetFloatv(GL_ALPHA_TEST_REF, &param);

    return param;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Alpha To Coverage Test
void CGLRender::EnableSampleAlphaToMask(bool flag)
{
    (flag) ? (glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB)) : (glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsSampleAlphaToMaskEnabled()
{
    return (glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetSampleCoverage(float val, bool invert)
{
    glSampleCoverage((GLclampf)val,(GLboolean)invert);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::GetSampleCoverage(float &val, bool &invert)
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
void CGLRender::EnableColorBlend(bool flag)
{
    (flag) ? (glEnable(GL_BLEND)) : (glDisable(GL_BLEND));
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::IsColorBlendEnabled()
{
    return (glIsEnabled(GL_BLEND)) ? (true) : (false);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetBlendFunc(GAPIBLENDOP src, GAPIBLENDOP dest)
{
    glBlendFunc(CGLConvFunc::GetGLBlendOp(src),CGLConvFunc::GetGLBlendOp(dest));
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIBLENDOP CGLRender::GetSrcBlendFunc()
{
    GLenum param;
    glGetIntegerv(GL_BLEND_SRC, (GLint*)&param);

    return CGLConvFunc::GetGAPIBlendOp(param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
GAPIBLENDOP CGLRender::GetDestBlendFunc()
{
    GLenum param;
    glGetIntegerv(GL_BLEND_DST, (GLint*)&param);

    return CGLConvFunc::GetGAPIBlendOp(param);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetBlendEquation(GAPIBLENDFUNC mode)
{
    glBlendEquationEXT(CGLConvFunc::GetGLBlendFunc(mode));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetBlendEquationSeparate(GAPIBLENDFUNC modeRGB, GAPIBLENDFUNC modeAlpha)
{
    glBlendEquationSeparate(CGLConvFunc::GetGLBlendFunc(modeRGB),CGLConvFunc::GetGLBlendFunc(modeAlpha));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetBlendFuncSeparate(GAPIBLENDOP srcRGB, GAPIBLENDOP destRGB, GAPIBLENDOP srcAlpha, GAPIBLENDOP destAlpha)
{
    glBlendFuncSeparate(CGLConvFunc::GetGLBlendOp(srcRGB),CGLConvFunc::GetGLBlendOp(destRGB),
        CGLConvFunc::GetGLBlendOp(srcAlpha),CGLConvFunc::GetGLBlendOp(destAlpha));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetBlendColor(float red, float green, float blue, float alpha)
{
    glBlendColorEXT(red,green,blue,alpha);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Setting Framebuffer Clear Color
void CGLRender::SetClearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red,green,blue,alpha);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Fill Mode
void CGLRender::SetFillMode(GAPIFILLMODE mode)
{
    static GLenum fillmodetab[] = {GL_FILL,GL_POINT,GL_LINE,GL_FILL};
    GLenum fillmode = fillmodetab[mode];
    glPolygonMode(GL_FRONT,fillmode);
	glPolygonMode(GL_BACK,fillmode);
}
//////////////////////////////////////////////////////////////////////////////////////////////
// Setting Polygon Cull Mode
void CGLRender::SetCullMode(bool CullFace,bool CCW)
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
void CGLRender::SetShadingMode(bool bsmooth)
{
    if(bsmooth)
		glShadeModel(GL_SMOOTH); //Gouraud shading
	else
		glShadeModel(GL_FLAT);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Occlusion Query Managment Functions
uint32t CGLRender::CreateQueryObject()
{
    return CGLOcclusionQuery::GetSingleton()->CreateQueryObject();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::DeleteQueryObject(uint32t objidx)
{
    return CGLOcclusionQuery::GetSingleton()->DeleteQueryObject(objidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::BeginQuery(uint32t objidx)
{
    CGLOcclusionQuery::GetSingleton()->BeginQuery(objidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::EndQuery()
{
    CGLOcclusionQuery::GetSingleton()->EndQuery();
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetQueryObjects(uint32t objidx)
{
    return CGLOcclusionQuery::GetSingleton()->GetQueryObjects(objidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::QueryStarted()
{
    return CGLOcclusionQuery::GetSingleton()->QueryStarted();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Texture Managment Functions
uint32t CGLRender::CreateTexture(uint32t width, uint32t height, uint32t depth, uint32t flags, TEXFMT fmt, const void **data)
{
    return CGLTextureSystem::GetSingleton()->CreateTexture(width,height,depth,flags,fmt,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::DeleteTexture(uint32t texidx)
{
    return CGLTextureSystem::GetSingleton()->DeleteTexture(texidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::UpdateTexture(uint32t texidx, uint32t miplevel, uint32t face, uint32t align, uint32t xoff, uint32t yoff, uint32t zoff, const void *data)
{
    return CGLTextureSystem::GetSingleton()->UpdateTexture(texidx,miplevel,face,align,xoff,yoff,zoff,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::BindRenderTarget(uint32t texidx, uint32t face)
{
    return CGLTextureSystem::GetSingleton()->BindRenderTarget(texidx,face);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::UnbindRenderTarget()
{
    return CGLTextureSystem::GetSingleton()->UnbindRenderTarget();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexFilter(uint32t texidx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexFilter(texidx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::GetTexFilter(uint32t texidx, TEXFLT &flt)
{
    return CGLTextureSystem::GetSingleton()->GetTexFilter(texidx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexAnisotropy(uint32t texidx, uint32t val)
{
    return CGLTextureSystem::GetSingleton()->SetTexAnisotropy(texidx,val);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::GetTexAnisotropy(uint32t texidx, uint32t &val)
{
    return CGLTextureSystem::GetSingleton()->GetTexAnisotropy(texidx,val);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexAddr(uint32t texidx, TEXADDR addrS, TEXADDR addrT, TEXADDR addrR)
{
    return CGLTextureSystem::GetSingleton()->SetTexAddr(texidx,addrS,addrT,addrR);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::GetTexAddr(uint32t texidx, TEXADDR &addrS, TEXADDR &addrT, TEXADDR &addrR)
{
    return CGLTextureSystem::GetSingleton()->GetTexAddr(texidx,addrS,addrT,addrR);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::GetTexWidth(uint32t texidx, uint32t &width)
{
    return CGLTextureSystem::GetSingleton()->GetTexWidth(texidx,width);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::GetTexHeight(uint32t texidx, uint32t &height)
{
    return CGLTextureSystem::GetSingleton()->GetTexHeight(texidx,height);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::GetTexDepth(uint32t texidx, uint32t &depth)
{
    return CGLTextureSystem::GetSingleton()->GetTexDepth(texidx,depth);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexture(uint32t texIdx, uint32t tex_unit)
{
    return CGLTextureSystem::GetSingleton()->SetTexture(texIdx,tex_unit);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexture(const uint32t *pTexIdx, uint32t tex_unit_num)
{
    return CGLTextureSystem::GetSingleton()->SetTexture(pTexIdx,tex_unit_num);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DisableTextures()
{
    return CGLTextureSystem::GetSingleton()->DisableTextures();
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::CreateTextureSampler()
{
    return CGLTextureSystem::GetSingleton()->CreateTextureSampler();
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::DeleteTextureSampler(uint32t sampleridx)
{
    return CGLTextureSystem::GetSingleton()->DeleteTextureSampler(sampleridx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerFilter(uint32t sampleridx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerFilter(sampleridx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerMinFilter(uint32t sampleridx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMinFilter(sampleridx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerMagFilter(uint32t sampleridx, TEXFLT flt)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMagFilter(sampleridx,flt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerAddr(uint32t sampleridx, TEXADDR wrap)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerAddr(sampleridx,wrap);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerAddr(uint32t sampleridx, TEXADDR sWrap, TEXADDR tWrap, TEXADDR rWrap)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerAddr(sampleridx,sWrap,tWrap,rWrap);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerMinLod(uint32t sampleridx, int32t lod)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMinLod(sampleridx,lod);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerMaxLod(uint32t sampleridx, int32t lod)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMaxLod(sampleridx,lod);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerMaxAnisotroty(uint32t sampleridx, int32t anisotropy)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerMaxAnisotroty(sampleridx,anisotropy);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTexSamplerCompare(uint32t sampleridx, GAPIFUNCTION func, bool modeon)
{
    return CGLTextureSystem::GetSingleton()->SetTexSamplerCompare(sampleridx,func,modeon);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTextureSampler(uint32t texidx, uint32t texunit)
{
    return CGLTextureSystem::GetSingleton()->SetTextureSampler(texidx,texunit);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetTextureSamplers(const uint32t *pTexIdx, uint32t texunitcnt)
{
    return CGLTextureSystem::GetSingleton()->SetTextureSamplers(pTexIdx,texunitcnt);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DisableTextureSamplers()
{
    return CGLTextureSystem::GetSingleton()->DisableTextureSamplers();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::EnableTexEnvStatesCheck(bool flag)
{
    return CGLTextureSystem::GetSingleton()->EnableTexEnvStatesCheck(flag);
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Shaders Management Functions
uint32t CGLRender::CreateShader(const char *strshader)
{
    return CGLShaderSystem::GetSingleton()->CreateShader(strshader);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::DeleteShader(uint32t shaderidx)
{
    return CGLShaderSystem::GetSingleton()->DeleteShader(shaderidx);
}

//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::CreateShaderParamsGroup(uint32t reservesize)
{
    return CGLShaderSystem::GetSingleton()->CreateShaderParamsGroup(reservesize);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::DeleteShaderParamsGroup(uint32t shaderidx)
{
    return CGLShaderSystem::GetSingleton()->DeleteShaderParamsGroup(shaderidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::AddShaderParam(uint32t groupidx, const char *name, GAPIUNIFORMTYPE type, const void *data)
{
    return CGLShaderSystem::GetSingleton()->AddShaderParam(groupidx,name,type,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::UpdateShaderParam(uint32t groupidx, uint32t paramidx, const void *data)
{
    return CGLShaderSystem::GetSingleton()->UpdateShaderParam(groupidx,paramidx,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetShaderParams(uint32t shaderidx, uint32t groupidx, uint32t commgroupidx)
{
    return CGLShaderSystem::GetSingleton()->SetShaderParams(shaderidx,groupidx,commgroupidx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetShaderParamIndex(uint32t groupidx, const char *name)
{
    return CGLShaderSystem::GetSingleton()->GetShaderParamIndex(groupidx,name);
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::CreateUniformBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data)
{
    return CGLShaderSystem::GetSingleton()->CreateUniformBuffer(usage,bufsize,data);
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::AddUniformBlocks(uint32t shaderidx, uint32t *bufidx, const char **names, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->AddUniformBlocks(shaderidx,bufidx,names,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetShader(uint32t shaderidx)
{
    return CGLShaderSystem::GetSingleton()->SetShader(shaderidx);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Shader Uniform Block Handling Functions
bool CGLRender::SetShaderUniform(GAPIUNIFORMTYPE type, const char *name, const void *data)
{
    return CGLShaderSystem::GetSingleton()->SetUniform(type,name,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetShaderProgramUniform(uint32t shaderidx, GAPIUNIFORMTYPE type, const char *name, const void *data)
{
    return CGLShaderSystem::GetSingleton()->SetProgramUniform(shaderidx,type,name,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetShaderUniforms(GAPIUNIFORMDATA *data, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->SetUniforms(data,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetShaderProgramUniforms(uint32t shaderidx, GAPIUNIFORMDATA *data, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->SetProgramUniforms(shaderidx,data,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetUniformBlockSize(uint32t shaderidx, const char *blockname)
{
    return CGLShaderSystem::GetSingleton()->GetUniformBlockSize(shaderidx,blockname);
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::GetActiveUniformInfo(uint32t shaderidx, GAPIACTIVEUNIFORMTYPE type, const char **names, uint32t *pvalues, uint32t count)
{
    return CGLShaderSystem::GetSingleton()->GetActiveUniformInfo(shaderidx,type,names,pvalues,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetUniformBlockBinding(uint32t shaderidx, const char *blockname, uint32t bindingpoint)
{
    return CGLShaderSystem::GetSingleton()->SetUniformBlockBinding(shaderidx,blockname,bindingpoint);
}
//////////////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------
uint32t CGLRender::GetUniformBufferBindingPoint(uint32t bufidx)
{
    return CGLShaderSystem::GetSingleton()->GetUniformBufferBindingPoint(bufidx);
}
-------------------------------------------*/

//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetUniformBufferBinding(uint32t bufidx, uint32t bindpoint)
{
    return CGLShaderSystem::GetSingleton()->SetUniformBufferBindingPoint(bufidx,bindpoint);
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Fragment Data Location
void CGLRender::SetFragDataLocation(uint32t shaderidx, uint32t colornumber, const char *name)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocation(shaderidx,colornumber,name);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetFragDataLocation(uint32t shaderidx, uint32t *colorsnumbers, const char **names, uint32t count)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocation(shaderidx,colorsnumbers,names,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetFragDataLocation(uint32t shaderidx, const char *name)
{
    return (uint32t)CGLShaderSystem::GetSingleton()->GetFragDataLocation(shaderidx,name);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Blend Func Extended
void CGLRender::SetFragDataLocationIndexed(uint32t shaderidx, uint32t colornumber, uint32t index, const char *name)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocationIndexed(shaderidx,colornumber,index,name);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetFragDataLocationIndexed(uint32t shaderidx, uint32t *colorsnumbers, uint32t *indices, const char **names, uint32t count)
{
    CGLShaderSystem::GetSingleton()->SetFragDataLocationIndexed(shaderidx, colorsnumbers,indices,names,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetFragDataIndex(uint32t shaderidx, const char *name)
{
    return CGLShaderSystem::GetSingleton()->GetFragDataIndex(shaderidx,name);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Vertex Object Managment Functions
uint32t CGLRender::CreateVertexBuffer(GAPIBUFFERUSAGE usage, uint32t bufsize, const void *data)
{
    return CGLVBOSystem::GetSingleton()->CreateVertexBuffer(usage,bufsize,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::UpdateVertexBuffer(uint32t bufid, uint32t first, uint32t size, const void *data)
{
    return CGLVBOSystem::GetSingleton()->UpdateVertexBuffer(bufid,first,size,data);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::DeleteVertexBuffer(uint32t bufid)
{
    return CGLVBOSystem::GetSingleton()->DeleteVertexBuffer(bufid);
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool CGLRender::SetVertexBuffer(uint32t bufferid)
{
    return CGLVBOSystem::GetSingleton()->BindBuffer(bufferid);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Direct Vertex Attribute Management Functions
void CGLRender::SetVertexAttribPointer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, const void *pointer)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribPointer(stream,size,type,normalize,stride,pointer);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetVertexAttribBuffer(uint32t stream, uint32t size, GAPIDATATYPE type, bool normalize, uint32t stride, uint32t offset, uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBuffer(stream,size,type,normalize,stride,offset,buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetVertexAttribPointerI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, const void *pointer)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribPointerI(stream,size,type,stride,pointer);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetVertexAttribBufferI(uint32t stream, uint32t size, GAPIDATATYPE type, uint32t stride, uint32t offset, uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBufferI(stream,size,type,stride,offset,buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetVertexAttribPointerL(uint32t stream, uint32t size, uint32t stride, const void *pointer)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribPointerL(stream,size,stride,pointer);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetVertexAttribBufferL(uint32t stream, uint32t size, uint32t stride, uint32t offset, uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBufferL(stream,size,stride,offset,buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetVertexAttribBuffers(GAPIVERTATTRIBDATA *vert_attrib_data, uint32t count)
{
    CGLVBOSystem::GetSingleton()->SetVertexAttribBuffers(vert_attrib_data,count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint32t CGLRender::GetVertexAttribLocation(uint32t shaderidx, const char *attribname)
{
    /*
    GLhandleARB handle = CGLShaderSystem::GetSingleton()->GetShaderProgram(shaderidx);
    GLuint loc = glGetAttribLocation(handle,attribname);
    return loc;
    */

    return CGLShaderSystem::GetSingleton()->GetAttribLocation(shaderidx,attribname);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::SetIndexBuffer(uint32t buf_idx)
{
    CGLVBOSystem::GetSingleton()->SetIndexBuffer(buf_idx);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DisableVertexAttribPointers()
{
    CGLVBOSystem::GetSingleton()->DisableVertexAttribPointers();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Direct Drawing Functions
void CGLRender::DrawArrays(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count)
{
    GLenum primmode = CGLConvFunc::GetGLPrimitiveType(mode);
    glDrawArrays(primmode,(GLint)first,(GLsizei)count);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawElements(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices)
{
    glDrawElements(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices);
}

//--CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices);
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::MultiDrawArrays(GAPIPRIMITIVETYPE mode, const uint32t *first, const uint32t *count, uint32t drawcount)
{
    glMultiDrawArrays(CGLConvFunc::GetGLPrimitiveType(mode),(const GLint*)first,(const GLsizei*)count,(GLsizei)drawcount);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::MultiDrawElements(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount)
{
    glMultiDrawElements(CGLConvFunc::GetGLPrimitiveType(mode),(const GLsizei*)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)drawcount);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawRangeElements(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices)
{
    glDrawRangeElementsEXT(CGLConvFunc::GetGLPrimitiveType(mode),start,end,count,CGLConvFunc::GetGLDataType(type),indices);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawArraysInstanced(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instancecount)
{
    glDrawArraysInstanced(CGLConvFunc::GetGLPrimitiveType(mode),first,(GLsizei)count,(GLsizei)instancecount);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawElementsInstanced(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount)
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
void CGLRender::DrawArraysInderict(GAPIPRIMITIVETYPE mode, const void *indirect)
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
void CGLRender::DrawElementsIndirect(GAPIPRIMITIVETYPE mode, GAPIDATATYPE type, const void *indirect)
{
    glDrawElementsIndirect(CGLConvFunc::GetGLPrimitiveType(mode),CGLConvFunc::GetGLDataType(type),indirect);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawArraysInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t first, uint32t count, uint32t instancecount, uint32t baseinstance)
{
    glDrawArraysInstancedBaseInstance(CGLConvFunc::GetGLPrimitiveType(mode),(GLint)first,(GLsizei)count,(GLsizei)instancecount,baseinstance);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::MultiDrawArraysIndirect(GAPIPRIMITIVETYPE mode, const void *indirect, uint32t drawcount, uint32t stride)
{
    glMultiDrawArraysIndirect(CGLConvFunc::GetGLPrimitiveType(mode),indirect,(GLsizei)drawcount,(GLsizei)stride);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawElementsInstancedBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, uint32t baseinstance)
{
    glDrawElementsInstancedBaseInstance(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)instancecount,baseinstance);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex)
{
    glDrawElementsBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,basevertex);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawRangeElementsBaseVertex(GAPIPRIMITIVETYPE mode, uint32t start, uint32t end, uint32t count, GAPIDATATYPE type, const void *indices, int32t basevertex)
{
    glDrawRangeElementsBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),start,end,(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,basevertex);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawElementsInstancedBaseVertex(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex)
{
    glDrawElementsInstancedBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)instancecount,basevertex);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::DrawElementsInstancedBaseVertexBaseInstance(GAPIPRIMITIVETYPE mode, uint32t count, GAPIDATATYPE type, const void *indices, uint32t instancecount, int32t basevertex, uint32t baseinstance)
{
    glDrawElementsInstancedBaseVertexBaseInstance(CGLConvFunc::GetGLPrimitiveType(mode),(GLsizei)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)instancecount,(GLint)basevertex,(GLuint)baseinstance);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CGLRender::MultiDrawElementsBaseVertex(GAPIPRIMITIVETYPE mode, const uint32t *count, GAPIDATATYPE type, const void *const *indices, uint32t drawcount, int32t *basevertex)
{
    glMultiDrawElementsBaseVertex(CGLConvFunc::GetGLPrimitiveType(mode),(const GLsizei*)count,CGLConvFunc::GetGLDataType(type),indices,(GLsizei)drawcount,basevertex);
}




/////////////////////////////////////////////////////////////////////////////////////////////