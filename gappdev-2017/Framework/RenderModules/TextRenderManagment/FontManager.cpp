/////////////////////////////////////////////////////////////////////////////////////////////
// FontManager.cpp
//
// Created: 10-03-2015
/////////////////////////////////////////////////////////////////////////////////////////////

#include "FontManager.h"
#include "Font/FontImageConstructor.h"
#include "Image/Image.h"


/////////////////////////////////////////////////////////////////////////////////////////////
CFontManager::CFontManager(IGraphicApi *gapi):m_gapi(gapi)
{
    m_pTextRenderer = new CTextRenderer(gapi);
}
/////////////////////////////////////////////////////////////////////////////////////////////
CFontManager::~CFontManager()
{
    delete (m_pTextRenderer);
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CFontManager::CreateFontTex(const char *fontname, uint fontsize, uint spacewidth,
                        uint firstchar, uint lastchar, const uint sizestab[][3], const ubyte *pdata)
{
    bool bavailable = false;
    if(m_FontMap.count(fontname)==1) {
        std::vector<FontTexData> &fntarr = m_FontMap[fontname];
        std::vector<FontTexData>::iterator it = fntarr.begin();
        while(it != fntarr.end()) {
            if(it->FontSize == fontsize)
                return false;
            it++;
        }
    }

    // Creating Font Image
    CBMImage *image = CFontImageConstructor::CreateFontImage(firstchar,lastchar,fontsize,spacewidth,sizestab,pdata);
    if(image == NULL)
        return false;

    // Creating Font Texture
    ubyte *pixels = image->GetPixels();

    uint32t texidx = m_gapi->CreateTexture(image->GetWidth(),image->GetHeight(),1,0,(TEXFMT)image->GetFormat(),(const void**)&pixels);

    if(texidx) {
        SetFontData(fontsize,texidx,image,fontname);
    }

    delete (image);

    return texidx!=0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool CFontManager::SetFontTexture(const char *fontname, uint fontsize)
{
    if(m_FontMap.count(fontname)==0) return false;
    std::vector<FontTexData> &fntarr = m_FontMap[fontname];
    std::vector<FontTexData>::iterator it = fntarr.begin();
    while(it != fntarr.end()) {
        if(it->FontSize == fontsize) {
            m_pTextRenderer->SetFontTexIdx(it->FontTexIdx);
            m_pTextRenderer->SetFontTexSize(it->TexWidth,it->TexHeight);
            m_pTextRenderer->SetSymbolsDesc(it->SymbolsDesc);
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CFontManager::SetTextPos(uint x, uint y)
{
    m_pTextRenderer->SetTextPos(x,y);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CFontManager::PrintText(const char *text, uint color, uint ocolor, bool use_ocolor)
{
    m_pTextRenderer->SetTextColor(color);    
    m_pTextRenderer->EnableTextOutlineColor(use_ocolor);
    m_pTextRenderer->SetTextOutlineColor((use_ocolor)?(ocolor):(0));
    m_pTextRenderer->PrintText(text);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CFontManager::SetFontData(uint fontsize, uint texidx, CBMImage *image, const char *fontname)
{
    FontTexData fntdata;
    fntdata.FontSize = fontsize;
    fntdata.FontTexIdx = texidx;
    fntdata.TexWidth = image->GetWidth();
    fntdata.TexHeight = image->GetHeight();
    CharDesc *desctab = (CharDesc*)image->GetInfoData();
    uint32t size = image->GetInfoDataSize();
    m_FontMap[fontname].push_back(fntdata);
    m_FontMap[fontname].back().SymbolsDesc = std::vector<CharDesc>(desctab,desctab + size/sizeof(CharDesc));
}
/////////////////////////////////////////////////////////////////////////////////////////////