/////////////////////////////////////////////////////////////////////////////////////////////
// FontManager.h
//
// Created: 10-03-2015
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FONTMANAGER_H__
#define __FONTMANAGER_H__

#include "../Renderapi.h"
#include "TextRenderer/TextRenderer.h"
#include <vector>
#include <string>
#include <map>

class CBMImage;

/////////////////////////////////////////////////////////////////////////////////////////////
class CFontManager
{
public:
    CFontManager(IGraphicApi *gapi);
    ~CFontManager();

    bool CreateFontTex(const char *fontname, uint fontsize, uint spacewidth,
                        uint firstchar, uint lastchar, const uint sizestab[][3], const ubyte *pdata);

    bool SetFontTexture(const char *fontname, uint fontsize);
    void SetTextPos(uint x, uint y);
    void PrintText(const char *text, uint color, uint ocolor, bool use_ocolor=false);

protected:
    void SetFontData(uint fontsize, uint texidx, CBMImage *image, const char *fontname);

    struct FontTexData {
        uint FontSize;
        uint FontTexIdx;
        uint TexWidth;
        uint TexHeight;
        std::vector<CharDesc> SymbolsDesc; 
    };

    std::map<std::string, std::vector<CFontManager::FontTexData> > m_FontMap;

    IGraphicApi *m_gapi;
    CTextRenderer *m_pTextRenderer;
};




////////////////////////////////////////////////////////////////////////////////////////////
#endif //__FONTMANAGER_H__