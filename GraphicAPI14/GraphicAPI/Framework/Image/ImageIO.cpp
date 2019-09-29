#include <windows.h>
#include <commctrl.h>
#include <memory.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"
#include "../Types/types.h"
#include "Image.h"
#include "ImageIO.h"
#include "BMI/BMP.h"
#include "BMI/TGA.h"

#pragma comment(lib,"comctl32.lib")

unsigned int getint(FILE* filePtr)
{
	int c0,c1,c2,c3;

	c0 = getc(filePtr);
	c1 = getc(filePtr);
	c2 = getc(filePtr);
	c3 = getc(filePtr);

	return	((unsigned int) c3) +
			(((unsigned int) c2)<<8)+
			(((unsigned int) c1)<<16)+
			(((unsigned int) c0)<<24);
}

CBMIFile::CBMIFile()
{
	tmpimg=new CImage();
	ImgIONum=0;
	ppImageIO = NULL;
	LoadString = NULL;
	SaveString = NULL;

	ImgIONum=2;
	ppImageIO = new CImageIO*[ImgIONum];

	CBMP* bmpfile = new CBMP();
	CTGA* tgafile = new CTGA();
	ppImageIO[0] = bmpfile;
	ppImageIO[1] = tgafile;

	char **ppstr1 = new char*[ImgIONum];
	char **ppstr2 = new char*[ImgIONum];
	ppstr1[0] = ppImageIO[0]->GetLoadString();
	ppstr2[0] = ppImageIO[0]->GetSaveString();

	ppstr1[1] = ppImageIO[1]->GetLoadString();
	ppstr2[1] = ppImageIO[1]->GetSaveString();

	//scanning strings
	uint a=0,b=0;
	for(uint x=0; x<ImgIONum; x++)
	{
		uint n=0;
		uint m=0;
		while(ppstr1[x][n++]!='\n')
		{			
			a+=1;
		}
		a+=12;//not yet checked
		while(ppstr2[x][m++]!='\n')
		{
			b+=1;
		}
		b+=12;//not yet checked
	}
	a+=84;
	b+=84;

	LoadString = new char[a];
	SaveString = new char[b];

	uint l=0;
	uint k=0;
	uint ind1=0;

	char str1[] ={"All Supported Types \t*.bmp;*.tga"};
	strncpy(LoadString,str1,sizeof(str1));
	l+=sizeof(str1);
	
	for(x=0; x<ImgIONum; x++)
	{
		uint n=0;
		uint m=0;
		while(ppstr1[x][n]!='\n')
			LoadString[l++]=ppstr1[x][n++];

		while(ppstr2[x][m]!='\n')
			SaveString[k++]=ppstr2[x][m++];
		
	}

	//strncpy(&LoadString[l],str1,sizeof(str1));
	//strncpy(&SaveString[k],str1,sizeof(str1));
	//l+=sizeof(str1);
	//k+=sizeof(str1);

	char string[] ={"All Files (*.*)\t*.*\t\n"};
	strncpy(&LoadString[l],string,sizeof(string));
	strncpy(&SaveString[k],string,sizeof(string));

	l=0;k=0;
	while(LoadString[l]!='\n')
	{
		if(LoadString[l]=='\t')
			LoadString[l]='\0';
		l+=1;
	}
	LoadString[l]='\0';

	while(SaveString[k]!='\n')
	{
		if(SaveString[k]=='\t')
			SaveString[k]='\0';
		k+=1;
	}
	SaveString[k]='\0';
	
	delete [] ppstr1;
	delete [] ppstr2;
}

CBMIFile::~CBMIFile()
{
	if(tmpimg!=NULL)
		delete (tmpimg);

	for(uint i=0; i<ImgIONum; i++)
	{
		delete (ppImageIO[i]);
	}
	//delete (ppImageIO[0]);
	//delete (ppImageIO[1]);
	//delete [] ppImageIO;

	delete [] LoadString;
	delete [] SaveString;
}


CImage* CBMIFile::Load(HWND hWnd)
{
	OPENFILENAME ofn = {0};
	char file_name[2048] = {0};
	char string[2048]={0};
	FILE *filePtr=NULL;
	CImage* image=NULL;
	ubyte* labels=NULL;
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = LoadString;
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = 2048; 
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR
								| OFN_HIDEREADONLY;

	if(GetOpenFileName(&ofn))
	{
		char *errors = NULL;
		char err[256]={0};
		for(uint i=0; i<ImgIONum; i++)
		{
			image = ppImageIO[i]->LoadImage(ofn.lpstrFile,err);
			if((!image) && (err[0]!=char(0)))
			{
				MessageBox(hWnd,err,"Loading Error",MB_OK);
				break;
			}
			if(image)
				break;
		}
		if((!image) && (err[0]==char(0)))
			MessageBox(hWnd,"Can't load image file","Error",MB_OK);

		if(errors)
			delete [] errors;
	}

	return image;
}

CImage *CBMIFile::LoadBitmapImage(char *file_name)
{
	CImage* image=NULL;
	
	char err[1024]={0};
	for(uint i=0; i<ImgIONum; i++)
	{
		image = ppImageIO[i]->LoadImage(file_name,err);
		if((!image) && (err[0]!=char(0)))
		{
			MessageBox(NULL,err,"Loading Error",MB_OK);
			break;
		}
		if(image)
			break;
	}
	if((!image) && (err[0]==char(0)))
		MessageBox(NULL,"Can't load image file","Error",MB_OK);

	return image;
}

bool CBMIFile::Save(HWND hWnd,CImage *Img)
{
	char ext[5]={0};
	char buf[20]={0};
	char file_name[2048] = {0};
	OPENFILENAME ofn = {0};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = 2048;
	ofn.lpstrFilter = SaveString;
	ofn.nMaxFileTitle = 255;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = 0;
	ofn.nFileExtension = 4;
	ofn.lpstrDefExt = ext;
	ofn.lpstrTitle = "Save File As...";
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn))
	{
		char ex[5]={0};
		char *p=file_name;
		while(p[0]!=char('\0'))
			p+=1;
		p-=3;
		strcpy(ex,p);
		for(uint i=0; i<ImgIONum; i++)
		{
			if(strcmp(ex,ppImageIO[i]->GetExt())==0)
			{
				if(!ppImageIO[i]->SaveImage(hWnd,Img,file_name))
				{
					if(!ppImageIO[i]->SaveImage(Img,file_name))
						MessageBox(hWnd,"Failed to save","Error",MB_OK);
				}
			}
		}	
		
	}

	return true;
}

