/////////////////////////////////////////////////////////////////////////////////////////////
// TextRenderer.h
//
// Created: 10-03-2015
/////////////////////////////////////////////////////////////////////////////////////////////

#include "TextRenderer.h"

const char TextShaderSrc[] = 
{
    "[[ Vertex Shader ]]\r\n"\
    "attribute vec4 vVertex;\r\n"\
    "attribute vec2 vTexCoords;\r\n"\
    "uniform vec2 OffsetVec;\r\n"\
    "varying vec2 TexCoords;\r\n"\
    "void main(void)\r\n{\r\n"\
    "gl_Position.zw = vVertex.zw;\r\n"\
	"gl_Position.xy = vVertex.xy + OffsetVec.xy;\r\n"\
    "TexCoords = vTexCoords;\r\n}"\

    "[[ Pixel Shader ]]\r\n"\
    "uniform sampler2D FontTex;\r\n"
    "uniform vec4 Color;\r\n"\
    "varying vec2 TexCoords;\r\n"\
    "void main(void)\r\n{\r\n"\
    "vec4 texColor = texture2D(FontTex,TexCoords);\r\n"\
    "texColor.a = texColor.r;\r\n"\
    "texColor = texColor*Color;\r\n"\
    "gl_FragColor = texColor;\r\n}"
};

#define MAX_SYMBOLS_COUNT   1024

/////////////////////////////////////////////////////////////////////////////////////////////
CTextRenderer::CTextRenderer(IGraphicApi *gapi):m_gapi(gapi),
m_TexWidth(0),
m_TexHeight(0),
m_TextPosX(0),
m_TextPosY(0),
m_TextColor(0xffffffff),
m_TextOutlineColor(0xff000000),
m_FntTexIdx(0),
m_TexUnit(0),
m_bUseOutlineColor(true)
{
    Init();
    SetTextColor(m_TextColor);
    SetTextOutlineColor(m_TextOutlineColor);
}
/////////////////////////////////////////////////////////////////////////////////////////////
CTextRenderer::~CTextRenderer()
{
    m_SymbolsDesc.clear();
    delete [] m_pVertData;
    delete [] m_pCoordData;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetSymbolsDesc(std::vector<CharDesc> &symbdesc)
{
    m_SymbolsDesc = symbdesc;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetTextColor(uint color)
{
    m_TextColor = color;
    m_fColor[0] = float(color&0xff)/255.f;
    m_fColor[1] = float((color>>8)&0xff)/255.f;
    m_fColor[2] = float((color>>16)&0xff)/255.f;
    m_fColor[3] = float((color>>24)&0xff)/255.f;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetTextOutlineColor(uint ocolor)
{
    m_TextOutlineColor = ocolor;
    m_fOColor[0] = float(ocolor&0xff)/255.f;
    m_fOColor[1] = float((ocolor>>8)&0xff)/255.f;
    m_fOColor[2] = float((ocolor>>16)&0xff)/255.f;
    m_fOColor[3] = float((ocolor>>24)&0xff)/255.f;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::EnableTextOutlineColor(bool enable)
{
    m_bUseOutlineColor = enable;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetFontTexSize(uint width, uint height)
{
    m_TexWidth = width;
    m_TexHeight = height;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetTextPos(uint x, uint y)
{
    m_TextPosX = x;
    m_TextPosY = y;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetFontTexIdx(uint texidx)
{
    m_FntTexIdx = texidx;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::PrintText(const char *text)
{
    if(m_FntTexIdx == 0) return ;
    uint32t texunit=0;

    uint symbcnt = PrepareVertData(text);

    m_gapi->UpdateVertexBuffer(m_VertBufObjIdx,0,symbcnt*6*3*sizeof(float),m_pVertData);
    m_gapi->UpdateVertexBuffer(m_CoordBufObjIdx,0,symbcnt*6*2*sizeof(float),m_pCoordData);

    SetRenderStates();

    m_gapi->SetVertexAttribBuffer(m_VertNameAttribId,3,GAPI_FLOAT,false,0,0,m_VertBufObjIdx);
    m_gapi->SetVertexAttribBuffer(m_CoordNameAttribId,2,GAPI_FLOAT,false,0,0,m_CoordBufObjIdx);

    RenderOutline(symbcnt);

    float nullvec[2] = {0.f,0.f};
    m_gapi->SetShaderUniform(GAPI_UNIFORM_VECTOR2F,"OffsetVec",nullvec);
    m_gapi->SetShaderUniform(GAPI_UNIFORM_VECTOR4F,"Color",m_fColor);
    m_gapi->DrawArrays(GAPI_TRIANGLES,0,symbcnt*6);

    ResetRenderStates();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::RenderOutline(uint symbolcnt)
{
    if(!m_bUseOutlineColor) return;

    m_gapi->SetShaderUniform(GAPI_UNIFORM_VECTOR4F,"Color",m_fOColor);
    float offsets[][2] = {  {-m_fVertOff[0],m_fVertOff[1]},{0.f,m_fVertOff[1]},{m_fVertOff[0],m_fVertOff[1]},
                            {-m_fVertOff[0],0.f},{m_fVertOff[0],0.f},{-m_fVertOff[0],-m_fVertOff[1]},
                            {0.f,-m_fVertOff[1]},{m_fVertOff[0],-m_fVertOff[1]} };

    for(uint i = 0; i < 8; i++)
    {
        m_gapi->SetShaderUniform(GAPI_UNIFORM_VECTOR2F,"OffsetVec",&offsets[i][0]);
        m_gapi->DrawArrays(GAPI_TRIANGLES,0,symbolcnt*6);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetRenderStates()
{
    m_gapi->EnableColorBlend(true);
    m_gapi->EnableDepthTest(false);
    m_gapi->SetBlendFunc(BLENDOP_SRC_ALPHA,BLENDOP_ONE_MINUS_SRC_ALPHA);
    m_gapi->SetTexture(m_FntTexIdx,m_TexUnit);
    m_gapi->SetShader(m_TextShaderIdx);

    m_gapi->SetShaderUniform(GAPI_UNIFORM_INT,"FontTex",&m_TexUnit);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::ResetRenderStates()
{
    m_gapi->SetTexture((uint32t)0,m_TexUnit);
    m_gapi->SetShader(0);
    m_gapi->DisableVertexAttribPointers();
    m_gapi->EnableDepthTest(true);
    m_gapi->EnableColorBlend(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::Init()
{
    m_TextShaderIdx = m_gapi->CreateShader(TextShaderSrc);
    m_VertNameAttribId = m_gapi->GetVertexAttribLocation(m_TextShaderIdx,"vVertex");
    m_CoordNameAttribId = m_gapi->GetVertexAttribLocation(m_TextShaderIdx,"vTexCoords");
  
    uint vertdatasize = MAX_SYMBOLS_COUNT*3*6;
    uint coorddatasize = MAX_SYMBOLS_COUNT*2*6;
    m_pVertData = new float[vertdatasize];
    m_pCoordData = new float[coorddatasize];
    memset(m_pVertData,0,vertdatasize*sizeof(float));
    memset(m_pCoordData,0,coorddatasize*sizeof(float));

    m_VertBufObjIdx = m_gapi->CreateVertexBuffer(GAPI_BUFUSAGE_STREAM,vertdatasize*sizeof(float),NULL);
    m_CoordBufObjIdx = m_gapi->CreateVertexBuffer(GAPI_BUFUSAGE_STREAM,coorddatasize*sizeof(float),NULL);

    m_fVertOff[0] = m_fVertOff[1] = 0.f;
}

/////////////////////////////////////////////////////////////////////////////////////////////
uint CTextRenderer::PrepareVertData(const char *text)
{
    size_t slen = strlen(text);
    //slen = (slen > MAX_SYMBOLS_COUNT) ? (MAX_SYMBOLS_COUNT) : (slen);

    int rect[4]={0};
    m_gapi->GetViewport(rect);

    float posoffx = 2.f/float(rect[2]);
    float posoffy = 2.f/float(rect[3]);

    m_fVertOff[0] = posoffx;
    m_fVertOff[1] = posoffy;

    float coordoffx = 1.f/(float)m_TexWidth;
    float coordoffy = 1.f/(float)m_TexHeight;
    
    const CharDesc &spacedesc = m_SymbolsDesc.front();
    uint firstsymb = m_SymbolsDesc[1].charsymb;
    uint lastsymb = m_SymbolsDesc.back().charsymb;

    uint posx = m_TextPosX;
    uint posy = m_TextPosY;

    float *pVert = m_pVertData;
    float *pCoord = m_pCoordData;

    uint SymbolsCount = 0;
    for(uint i = 0; i < slen; i++)
    {
        if(i >= MAX_SYMBOLS_COUNT)
            break;

        char ch = text[i];
        if(IsSpecialChar(ch)) {
            UpdatePos(posx,posy,ch,spacedesc.sizey,spacedesc.sizex);
            continue;
        }

        if(uint(ch) < firstsymb || uint(ch) > lastsymb) {
            posx += spacedesc.sizex;
            continue;
        }

        uint symbpos = uint(ch) - firstsymb;
        const CharDesc &symbol = m_SymbolsDesc[symbpos+1];

        float fposx = float(posx)*posoffx - 1.f;
        float fposy = float(posy)*posoffy - 1.f;
        float fdescent = float(symbol.descent)*posoffy;
        float fsizex = float(symbol.sizex)*posoffx;
        float fsizey = float(symbol.sizey)*posoffy;

        SetSpriteVertices(pVert,fposx,fposy,fdescent,fsizex,fsizey);
        pVert += 18;

        float coordposx = float(symbol.posx)*coordoffx;
        float coordposy = float(symbol.posy)*coordoffy;
        float coordsizex = float(symbol.sizex)*coordoffx;
        float coordsizey = float(symbol.sizey)*coordoffy;
        SetSpriteTexCoords(pCoord,coordposx,coordposy,coordsizex,coordsizey);
        pCoord += 12;

        posx += symbol.sizex;

        SymbolsCount += 1;
    }

    return SymbolsCount;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CTextRenderer::IsSpecialChar(char ch)
{
    return ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t';
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::UpdatePos(uint &posx, uint &posy, char ch, uint fontsize, uint spacewidth)
{
    if(ch == '\n') {
        posx = m_TextPosX;
        posy = posy - fontsize;
    }
    else if(ch == ' ') {
        posx += spacewidth;
    }
    else if(ch == '\t') {
        posx += spacewidth*4;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetSpriteVertices(float *verts, float x, float y, float descent, float width, float height)
{
    verts[0] = x;
    verts[1] = y + descent;
    verts[2] = 0.f;

    verts[3] = x + width;
    verts[4] = y + descent + height;
    verts[5] = 0.f;

    verts[6] = x;
    verts[7] = y + descent + height;
    verts[8] = 0.f;

    
    verts[9] = x;
    verts[10] = y + descent;
    verts[11] = 0.f;

    verts[12] = x + width;
    verts[13] = y + descent;
    verts[14] = 0.f;

    verts[15] = x + width;
    verts[16] = y + descent + height;
    verts[17] = 0.f;
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTextRenderer::SetSpriteTexCoords(float *coords, float x, float y, float width, float height)
{
    coords[0] = x;
    coords[1] = y;

    coords[2] = x + width;
    coords[3] = y + height;

    coords[4] = x;
    coords[5] = y + height;


    coords[6] = x;
    coords[7] = y;

    coords[8] = x + width;
    coords[9] = y;

    coords[10] = x + width;
    coords[11] = y + height;
}
/////////////////////////////////////////////////////////////////////////////////////////////