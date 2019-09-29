#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../../Types/types.h"
#include "../Image.h"
#include "../ImageIO.h"
#include "TGA.h"

struct TGAHeader {
	ubyte  descLen;
	ubyte  cmapType;
	ubyte  imageType;
	ushort cmapStart;
	ushort cmapEntries;
	ubyte  cmapBits;
	ushort xOffset;
	ushort yOffset;
	ushort width;
	ushort height;
	ubyte  bpp;
	ubyte  attrib;
};


char* CTGA::GetLoadString()
{
	return "Targa Image Files (*.tga)\t*.tga\t\n";
}

char* CTGA::GetSaveString()
{
	return "Targa Image Files (*.tga)\t*.tga\t\n";
}

CImage* CTGA::LoadImage(char *file_name,char* errors)
{
	TGAHeader header;
	FILE *file = fopen(file_name,"rb");
	CImage *Image = NULL;

	if(!file)
	{
		char string[] = {"Can't open selected file"};
		strncpy(errors,string,sizeof(string));
		//fclose(file);
		return NULL;
	}

	fread(&header.descLen,sizeof(ubyte)*3,1,file);
	fread(&header.cmapStart,sizeof(ushort)*2,1,file);
	fread(&header.cmapBits,sizeof(ubyte),1,file);
	fread(&header.xOffset,sizeof(ushort)*4,1,file);
	fread(&header.bpp,sizeof(ubyte)*2,1,file);

	if(header.imageType != 2)
	{
		char string[] = {"Image Compression Not Supported"};
		strncpy(errors,string,sizeof(string));
		fclose(file);
		return NULL;
	}

	switch(header.bpp)
	{
	case 24:
	case 32:
		{
			uint nChannels;
			FORMAT fmt;
			if(header.bpp==24)
			{
				fmt=FORMAT_RGB8;
				nChannels = 3;
			}
			else
			{
				fmt=FORMAT_RGBA8;
				nChannels = 4;
			}
			Image = new CImage();
			ubyte *data = (ubyte*)Image->Create(fmt,header.width,header.height,1,1);
			
			fread(data,header.width*header.height*nChannels,1,file);

			ubyte tmp;
			for(uint i=0; i<header.width*header.height*nChannels; i+=nChannels)
			{
				tmp = data[0];
				data[0] = data[2];
				data[2] = tmp;
				data+=nChannels;
			}
		}
		break;
	default:
		{
			char string[] = {"Other TGA Types Are Not Supported"};
			strncpy(errors,string,sizeof(string));
			fclose(file);
			return NULL;
		}
	}

	fclose(file);
	
	return Image;
}

bool CTGA::SaveImage(CImage *Image,char *file_name,bool swapBGR)
{

	return false;
}

/*#ifdef _WIN32
bool CTGA::SaveImage(HWND hWnd,CImage *Img,char *file_name)
{

	return true;
}
#endif
*/