/////////////////////////////////////////////////////////////////////////////////////////////
// BaseTextRenderer.h
//
// Created: 10-03-2015
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __BASETEXTRENDERER_H__
#define __BASETEXTRENDERER_H__

#include <vector>
#include "../RasterFontDescData.h"

/////////////////////////////////////////////////////////////////////////////////////////////
class IBaseTextRenderer
{
public:
    IBaseTextRenderer(){}
    virtual ~IBaseTextRenderer(){}

    virtual void SetSymbolsDesc(std::vector<CharDesc> &symbdesc) = 0;
    virtual void SetTextColor(uint color) = 0;
    virtual void SetTextOutlineColor(uint color) = 0;
    virtual void EnableTextOutlineColor(bool enable) = 0;
    
};
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__BASETEXTRENDERER_H__