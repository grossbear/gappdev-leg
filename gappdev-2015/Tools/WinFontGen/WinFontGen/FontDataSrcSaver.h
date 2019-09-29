/////////////////////////////////////////////////////////////////////////////////////////////
// FontDataSrcSaver.h
// 
// Saving Font Data As C++ Source Header
// 
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __FONTDATASRCSAVER_H__
#define __FONTDATASRCSAVER_H__

#include <stdio.h>
#include <string>
#include "Font/WinGDIFontGen.h"

struct SymbolData;

/////////////////////////////////////////////////////////////////////////////////////////////
class CFontDataSrcSaver
{
public:
    static bool GenSrcFile(const char *path, const char *fontname, const SymbolsData *symbobj);

    static bool is_space_char_func(char c) { return c == ' '; }
    static char to_lower(char c) { return (c>=65 && c<=90) ? (c+32) : (c);}

private:
    static std::string SetFontName(const char *fontname, uint fontsize);
    static FILE* OpenSrcFile(const char *path, const char *fontname);
    static void WriteHeaderInfo(FILE *file, const char *fontname);
    static void WriteUIntValue(FILE *file, const char *fontname, const char *valname, uint value);
    static void WriteSizesData(FILE *file, const char *fontname, const uint *psizes, uint count);
    static void WriteFontBitmapData(FILE *file, const char *name, const SymbolsData *symbobj);

    static std::string PrintSymbolCode(uint symbol);
    static std::string PrintSymbolDataLine(const ubyte *pdata, uint width);
};

/////////////////////////////////////////////////////////////////////////////////////////////


#endif //__FONTDATASRCSAVER_H__
/////////////////////////////////////////////////////////////////////////////////////////////