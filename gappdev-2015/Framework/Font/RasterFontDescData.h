/////////////////////////////////////////////////////////////////////////////////////////////
// RasterFontDescData.h
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RASTERFONTDESCDATA_H__
#define __RASTERFONTDESCDATA_H__

#include <PlatformTypes.h>

/////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
typedef struct tagCharDesc
{
    uint charsymb;
    uint posx;
    uint posy;
    uint sizex;
    uint sizey;
    uint descent;
}CharDesc;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////////////////////
struct SymbolsData
{
	SymbolsData():SpaceWidth(0),FirstSymbol(0),LastSymbol(0),FontSize(0),BitFont(false),
		pSymbCodes(0),pSymbSizes(0),pSymbBits(0){}
	~SymbolsData(){
		delete [] pSymbCodes;
		delete [] pSymbSizes;
		delete [] pSymbBits;
	}

	uint SpaceWidth;
	uint FirstSymbol;
	uint LastSymbol;
	uint FontSize;
	bool BitFont;
    uint *pSymbCodes;
	uint *pSymbSizes;
	ubyte *pSymbBits;
};
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__RASTERFONTDESCDATA_H__