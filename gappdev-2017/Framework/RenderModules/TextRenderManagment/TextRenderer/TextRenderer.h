/////////////////////////////////////////////////////////////////////////////////////////////
// TextRenderer.h
//
// Created: 10-03-2015
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TEXTRENDERER_H__
#define __TEXTRENDERER_H__

#include "Renderapi.h"
#include "BaseTextRenderer.h"

class CTextRenderer : public IBaseTextRenderer
{
public:
    CTextRenderer(IGraphicApi *m_gapi);
    ~CTextRenderer();

    void SetSymbolsDesc(std::vector<CharDesc> &symbdesc);
    void SetTextColor(uint color);
    void SetTextOutlineColor(uint color);
    void EnableTextOutlineColor(bool enable);
    void SetFontTexSize(uint width, uint height);
    void SetTextPos(uint x, uint y);
    void PrintText(const char *text);
    void SetFontTexIdx(uint texidx);

protected:
    void Init();
    uint PrepareVertData(const char *text);

    void SetRenderStates();
    void ResetRenderStates();

    void RenderOutline(uint symbolcnt);

    bool IsSpecialChar(char ch);
    void UpdatePos(uint &posx, uint &posy, char ch, uint fontsize, uint spacewidth);
    void SetSpriteVertices(float *verts, float x, float y, float descent, float width, float height);
    void SetSpriteTexCoords(float *coords, float x, float y, float width, float height);
    
    
    std::vector<CharDesc> m_SymbolsDesc;
    uint m_TexWidth,m_TexHeight; 
    uint m_TextPosX,m_TextPosY;
    uint m_TexUnit;
    uint m_TextColor;
    uint m_TextOutlineColor;
    float m_fColor[4];
    float m_fOColor[4];
    bool m_bUseOutlineColor;

    uint m_FntTexIdx;
    uint m_TextShaderIdx;
    uint m_VertNameAttribId;
    uint m_CoordNameAttribId;
    uint m_TexUniformIdx;

    uint m_ColorUniformIdx;
    uint m_OffsetUniformIdx;

    float m_fVertOff[2];

    float *m_pVertData;
    float *m_pCoordData;
    uint m_VertBufObjIdx;
    uint m_CoordBufObjIdx;

    IGraphicApi *m_gapi;
};


#endif //__TEXTRENDERER_H__