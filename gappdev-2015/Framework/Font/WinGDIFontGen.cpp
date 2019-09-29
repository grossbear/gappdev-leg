/////////////////////////////////////////////////////////////////////////////////////////////
// WinGDIFontGen.cpp
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////

#include "WinGDIFontGen.h"


/////////////////////////////////////////////////////////////////////////////////////////////
SymbolsData* CWinGDIFontGen::CreateCharFont(const char *fontname, uint fontsize, ubyte firstchar, ubyte lastchar)
{
	if(fontsize > 255) return NULL;
    if(fontsize < 10) return NULL;
    firstchar = (firstchar <= 32) ? (33) : (firstchar);
	if(firstchar > lastchar) return NULL;

    //Creating And Setting Device Context
    HDC hDC = InitDC();
    if(!hDC) return NULL;

    //Creating Font
    HFONT hFont = ::CreateFontA((int)fontsize, 0,0,0,0,0, FALSE,FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
                                CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, fontname);
    if(!hFont) {
        DeleteDC(hDC);
        return NULL;
    }

	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

    //Creating GDI DIBitmap
    HBITMAP hBitmap,hOldBitmap;
    uint bmpwidth = fontsize;
    uint bmpheight = fontsize;
    DWORD *pBitmapBits = CreateGDIBitmap(hDC,hBitmap,hOldBitmap,bmpwidth,bmpheight);
    if(!pBitmapBits) {
        ReleaseGDI(hDC,hOldBitmap,hBitmap,hOldFont,hFont);
        return NULL;
    }

    //Getting Space Char Width
	SIZE symbolsize;
	char spacechar = char(32);
    GetTextExtentPoint32(hDC,&spacechar,1,&symbolsize);
	uint spacewidth = (uint)symbolsize.cx;

    uint symbolscnt = lastchar - firstchar + 1;
    //Allocating Memory For Symbols Data
    uint *pSymbolsSizes = new uint[symbolscnt * 3];
    ubyte *pSymbolsBits = new ubyte[symbolscnt * bmpwidth * bmpheight];
    ubyte *pdata = pSymbolsBits;
    uint *psizes = pSymbolsSizes;
    memset(pSymbolsSizes,0,sizeof(uint)*symbolscnt*3);
    memset(pSymbolsBits,0,sizeof(ubyte)*symbolscnt*bmpwidth*bmpheight);

    //Dummy Data
    DWORD *pDummyBits = new DWORD[bmpwidth];
    memset(pDummyBits,0,sizeof(DWORD)*bmpwidth);

    //Drawing Chars And Getting Char Sizes
    for(ubyte symbol = firstchar; symbol <= lastchar; symbol++)
    {
        char csymbol = (char)symbol;
        GetTextExtentPoint32(hDC,&csymbol,1,&symbolsize);

        memset(pBitmapBits,0,sizeof(DWORD)*bmpwidth*bmpheight);
        ExtTextOut(hDC,0,0, ETO_OPAQUE, NULL, (LPCSTR)&csymbol, 1, NULL);

        uint descent = GetSymbolOffset(pBitmapBits,pDummyBits,bmpwidth,bmpheight,true);
        uint cheight = GetSymbolOffset(pBitmapBits,pDummyBits,bmpwidth,bmpheight,false);
        cheight = bmpheight - cheight - descent;

        uint sizetab[3] = {symbolsize.cx,cheight,descent};
        memcpy(psizes,sizetab,sizeof(uint)*3);
        psizes += 3;
    
        //Copy Char Bits
        CopySymbol(pBitmapBits,pdata,bmpwidth,bmpheight,symbolsize.cx,cheight,descent);
        pdata += symbolsize.cx*cheight;
    }

    delete [] pDummyBits;

    //Release GDI Resources
    ReleaseGDI(hDC,hOldBitmap,hBitmap,hOldFont,hFont);

    SymbolsData *pSymbolsData = new SymbolsData();
    memset(pSymbolsData,0,sizeof(SymbolsData));
    SetSymbolsDataStruct(pSymbolsData,firstchar,lastchar,fontsize,spacewidth,pSymbolsBits,pSymbolsSizes);

    return pSymbolsData;
    
}
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
HDC CWinGDIFontGen::InitDC()
{
	HDC hDC = CreateCompatibleDC(NULL);
    SetTextColor(hDC,RGB(255,255,255));
    SetBkColor(hDC,0x00000000);
    SetTextAlign(hDC, TA_TOP);
    SetMapMode(hDC, MM_TEXT);

	return hDC;
}

/////////////////////////////////////////////////////////////////////////////////////////////
DWORD* CWinGDIFontGen::CreateGDIBitmap(HDC hDC, HBITMAP &hBitmap, HBITMAP &hOldBitmap, uint width, uint height)
{
	DWORD *pBitmapBits;
    BITMAPINFO BitmapInfo;
    memset(&BitmapInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
    BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfo.bmiHeader.biWidth = (LONG)width;
    BitmapInfo.bmiHeader.biHeight = (LONG)height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    BitmapInfo.bmiHeader.biBitCount = 32;

    hBitmap = CreateDIBSection(hDC,&BitmapInfo,DIB_RGB_COLORS,(VOID**)&pBitmapBits,NULL,0);
    hOldBitmap = (HBITMAP)SelectObject(hDC,hBitmap);

	return pBitmapBits;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWinGDIFontGen::CopySymbol(DWORD *pBitmap, ubyte *pSymbolsData, uint bmpwidth, uint bmpheight, uint width, uint height, uint descent)
{
    uint bmpidx = bmpwidth*descent;
    DWORD *pSrcBmp = pBitmap;
    ubyte *pDestData = pSymbolsData;

    //skipping descent lines
    pSrcBmp += bmpwidth*descent;

    for(uint y = 0; y < height; y++) 
    {
		pDestData = pSymbolsData + (height-y-1)*width;
        for(uint x = 0; x < width; x++) 
        {
            DWORD color = pSrcBmp[x];
            ubyte *clrch = reinterpret_cast<ubyte*>(&color);
            *pDestData++ = clrch[0];
        }
        pSrcBmp += bmpwidth;
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////
uint CWinGDIFontGen::GetSymbolOffset(const DWORD *pBitmapBits, const DWORD *pDummyData, uint width, uint height, bool updir)
{
	int idx = (updir) ? (0) : (height-1);
	int upval = (updir) ? (1) : (-1);
	int endval = (updir) ? (height-1) : (0);

	uint offset = 0;
	while(idx != endval)
	{
		int bmpidx = idx*width;
		int cmpres = memcmp(&pBitmapBits[bmpidx],pDummyData,sizeof(DWORD)*width);
		if(cmpres != 0)
			break;

        offset += 1;
		idx += upval;
	}

	return offset;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWinGDIFontGen::SetSymbolsDataStruct(SymbolsData *pSymb, uint firstchar, uint lastchar, uint fontsize, 
											uint spacewidth, ubyte *pSymbols, uint *pSymbSizes)
{
	pSymb->BitFont = false;
	pSymb->FirstSymbol = firstchar;
	pSymb->LastSymbol = lastchar;
	pSymb->FontSize = fontsize;
	pSymb->SpaceWidth = spacewidth;
	pSymb->pSymbBits = pSymbols;
	pSymb->pSymbSizes = pSymbSizes;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CWinGDIFontGen::ReleaseGDI(HDC hDC, HBITMAP hOldBmp, HBITMAP hBmp, HFONT hOldFnt, HFONT hFnt)
{
	SelectObject(hDC,hOldBmp);
    DeleteObject(hBmp);
    SelectObject(hDC, hOldFnt);
    DeleteObject(hFnt);
    DeleteDC(hDC);
}
/////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////