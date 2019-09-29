/////////////////////////////////////////////////////////////////////////////////////////////
// WinGDIFontGen.h
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WINGDIFONTGEN_H__
#define __WINGDIFONTGEN_H__

//#include "../Image/Image.h"
#include "RasterFontDescData.h"
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
class CWinGDIFontGen
{
public:
	static SymbolsData* CreateCharFont(const char *fontname,  uint size, ubyte firstchar, ubyte lastchar);

protected:
	static HDC InitDC();
	static DWORD *CreateGDIBitmap(HDC hdc, HBITMAP &hBitmap, HBITMAP &hOldBitmap, uint width, uint height);
	static void CopySymbol(DWORD *pBitmap, ubyte *pSymbolData, uint bmpwidth, uint bmpheight, uint width, uint height, uint descent);
	static uint GetSymbolOffset(const DWORD *pBitmapBits, const DWORD *pDummyData, uint width, uint height, bool updir);
	static inline void SetSymbolsDataStruct(SymbolsData *pSymb, uint firstchar, uint lastchar, uint fontsize, 
											uint spacewidth, ubyte *pSymbols, uint *pSymbSizes);
	static inline void ReleaseGDI(HDC hDC, HBITMAP hOldBmp, HBITMAP hBmp, HFONT hOldFnt, HFONT hFnt);

};

/////////////////////////////////////////////////////////////////////////////////////////////

#endif //__WINGDIFONTGEN_H__