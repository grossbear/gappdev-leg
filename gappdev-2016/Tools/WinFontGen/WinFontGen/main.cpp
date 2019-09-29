//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include "Font/WinGDIFontGen.h"
#include "FontDataSrcSaver.h"


int main(int argc, _TCHAR* argv[])
{
    const char fontname[]={"Arial"};
	SymbolsData *symbdataobj = CWinGDIFontGen::CreateCharFont(fontname,24,33,127);

	//////////////////////////////////////
	//Save Data
	CFontDataSrcSaver::GenSrcFile(NULL,fontname,symbdataobj);
    //////////////////////////////////////

	delete (symbdataobj);
	
	
	return 0;
}