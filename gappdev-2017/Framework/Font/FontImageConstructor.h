/////////////////////////////////////////////////////////////////////////////////////////////
// FontImageConstructor.h
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FONTIMAGECONSTRUCTOR_H__
#define __FONTIMAGECONSTRUCTOR_H__

#include <vector>
#include "Image/Image.h"
#include "RasterFontDescData.h"


/////////////////////////////////////////////////////////////////////////////////////////////
class CFontImageConstructor
{
public:
	static CBMImage* CreateFontImage(uint firstchar, uint lastchar, uint fontsize, uint spacewidth,
									const uint sizestab[][3], const ubyte *pdata, bool balpha=false);

protected:
    static void InitSymbDesc(std::vector<CharDesc> &symbsdesc, uint spacewidth, uint fontsize);
    static bool SetSymbolsDesc(std::vector<CharDesc> &symbsdesc, uint firstchar, uint lastchar,
                                uint imgwidth, uint imgheight,
                                const uint sizestab[][3]);
    static CBMImage* SetImage(std::vector<CharDesc> &symbsdesc, uint imgwidth, uint imgheight, const ubyte *pdata, bool bapha);  
};

/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__FONTIMAGECONSTRUCTOR_H__

