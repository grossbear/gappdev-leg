#include <stdio.h>
//#include <stdlib.h>
#include <windows.h>
#include "../../Types/types.h"
#include "../Image.h"
#include "../ImageIO.h"
#include "BMP.h"



//#define WIN32_LEAN_AND_MEAN


struct BMPHeader {
	ushort bfType;
	uint   bfSize;
	ushort bfReserved1;
	ushort bfReserved2;
	uint   bfOffBits;
	uint   biSize;
	uint   biWidth;
	uint   biHeight;
	ushort biPlanes;
	ushort biBitCount;
	uint   biCompression;
	uint   biSizeImage;
	uint   biXPelsPerMeter;
	uint   biYPelsPerMeter;
	uint   biClrUsed;
	uint   biClrImportant;
};


char* CBMP::GetLoadString()
{
	return "Bitmap Files (*.bmp)\t*.bmp\t\n";
}

char* CBMP::GetSaveString()
{
	return "Bitmap Files (*.bmp)\t*.bmp\t\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
//	Loading bmp files type
///////////////////////////////////////////////////////////////////////////////////////////////
CImage* CBMP::LoadImage(char *file_name, char* errors)
{
	//BMPHeader header;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	
	FILE *file = fopen(file_name,"rb");
	if(!file)
	{
		//fclose(file);
		char string[] = {"Couldn't open selected file"};
		strncpy(errors,string,sizeof(string));
		return NULL;
	}

	//wczytuje nag³ówek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file);

	//wczytuje nag³ówek obrazu zapisanego do pliku
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,file);
	
	//fread((void*)&header,sizeof(header),1,file);

	/*fread(&header.bfType,sizeof(header.bfType),1,file);
	fread(&header.bfSize,sizeof(header.bfSize),1,file);
	fread(&header.bfReserved1,sizeof(header.bfReserved1),1,file);
	fread(&header.bfReserved2,sizeof(header.bfReserved2),1,file);
	fread(&header.bfOffBits,sizeof(header.bfOffBits),1,file);

	fread(&header.biSize,sizeof(header.biSize),1,file);
	fread(&header.biWidth,sizeof(header.biWidth),1,file);
	fread(&header.biHeight,sizeof(header.biHeight),1,file);
	fread(&header.biPlanes,sizeof(header.biPlanes),1,file);
	fread(&header.biBitCount,sizeof(header.biBitCount),1,file);
	fread(&header.biCompression,sizeof(header.biCompression),1,file);

	fread(&header.biSizeImage,sizeof(header.biSizeImage),1,file);
	fread(&header.biXPelsPerMeter,sizeof(header.biXPelsPerMeter),1,file);
	fread(&header.biYPelsPerMeter,sizeof(header.biYPelsPerMeter),1,file);
	fread(&header.biClrUsed,sizeof(header.biClrUsed),1,file);
	fread(&header.biClrImportant,sizeof(header.biClrImportant),1,file);*/
	
	//int f = sizeof(BMPHeader);

	if(bitmapFileHeader.bfType != 0x4D42)
	{
		fclose(file);
		return NULL;
	}

	if(bitmapInfoHeader.biPlanes != 1)
	{
		fclose(file);
		return NULL;
	}

	if(bitmapInfoHeader.biCompression)
	{
		fclose(file);
		char string[] = {"RLE Compression not supported!"};
		strncpy(errors,string,sizeof(string));
		return NULL;
	}

	uint width = bitmapInfoHeader.biWidth;
	uint height = bitmapInfoHeader.biHeight;
	uint depth = 1;
	uint pitch = width;
	uint nMipMaps = 1;
	uint nImages = 1;

	FORMAT fmt;
	CImage* img=NULL;

	switch(bitmapInfoHeader.biBitCount)
	{
	case 8:
		{
			if(bitmapInfoHeader.biCompression)
			{
				fclose(file);
				return NULL;
			}
			fmt=FORMAT_I8;
			img=new CImage();

			ubyte *data =(ubyte*)img->Create(fmt,width,height,depth);
			ubyte *pal=(ubyte*)img->getPalette();
			pitch = img->getPitch(); 

			fread(pal,1024,1,file);
			fseek(file,bitmapFileHeader.bfOffBits,SEEK_SET);
			fread(data,pitch*height,1,file);
		}
		break;
	case 24:
	case 32:
		{
			if(bitmapInfoHeader.biCompression)
			{
				fclose(file);
				return NULL;
			}
			uint nChannels;
			if(bitmapInfoHeader.biBitCount == 24)
			{
				fmt=FORMAT_RGB8;
				nChannels = 3;
			}
			else
			{
				fmt = FORMAT_RGBA8;
				nChannels = 4;
			}

			img = new CImage();
			ubyte *data = (ubyte*)img->Create(fmt,width,height,depth);
			pitch = img->getPitch();

			fseek(file,bitmapFileHeader.bfOffBits,SEEK_SET);
			fread(data,pitch*height*nChannels,1,file);

			//Swapping Red and Blue Color Channels
			ubyte *ptr = data;
			ubyte tmp;
			for(uint i=0; i<pitch*height*nChannels; i+=nChannels)
			{
				tmp = ptr[0];
				ptr[0] = ptr[2];
				ptr[2] = tmp;
				ptr+=nChannels;
			}
		}
		break;

	default:
		break;
	}
	fclose(file);

	return img;
}

bool CBMP::SaveImage(CImage *Image,char *file_name,bool swapBGR)
{
	//BMPHeader header={0};
	FORMAT fmt;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	memset(&bitmapFileHeader,0,sizeof(BITMAPFILEHEADER));
	memset(&bitmapInfoHeader,0,sizeof(BITMAPINFOHEADER));

	if(Image==NULL)
		return false;

	fmt = Image->getFormat();

	FILE *file=fopen(file_name,"wb");
	if(!file)
	{
		fclose(file);
		return false;
	}

	bitmapFileHeader.bfType = 0x4D42;
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biWidth = Image->getWidth();
	bitmapInfoHeader.biHeight = Image->getHeight();
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;

	switch(fmt)
	{
	case FORMAT_I1:

		break;
	case FORMAT_GREY8:

		break;

	case FORMAT_I8:

		break;

	case FORMAT_RGB8:
	case FORMAT_RGBA8:
		{
			uint nChannels = (fmt==FORMAT_RGB8) ? (3) : (4);
			uint size = Image->getPitch()*Image->getHeight()*nChannels;//pitch*bitmapInfoHeader.biHeight*nChannels;
			
			bitmapInfoHeader.biBitCount = (fmt==FORMAT_RGB8) ? (24) : (32);
			bitmapInfoHeader.biSizeImage = size;
			bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + size;

			fwrite(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,file);
			fwrite(&bitmapInfoHeader,sizeof(BITMAPINFOHEADER),1,file);

			ubyte *data = (ubyte*)Image->getPixels();
			if(swapBGR)
			{
				ubyte *ptr = data;
				for(int i=0; i<Image->getPitch()*Image->getHeight(); i++)
				{
					ubyte tmp = ptr[0];
					ptr[0] = ptr[2];
					ptr[2] = tmp;
					ptr+=nChannels;
				}
			}

			fwrite(data,Image->getPitch()*Image->getHeight()*nChannels,1,file);
			
			fclose(file);
		}
		break;

	default:
		return false;

	}

	return true;
}

/*#ifdef _WIN32
bool CBMP::SaveImage(HWND hWnd,CImage *Img,char *file_name)
{

	return true;
}
#endif*/




