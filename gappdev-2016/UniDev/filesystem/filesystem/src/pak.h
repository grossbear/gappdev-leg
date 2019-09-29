#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include <utility>

//TODO :: add proper subdirectory 
#include "Cfile.h"


typedef unsigned char byte;

using namespace utils;


#include "compiler/pack1"
struct STRUCT_PACKED pakheader
{
  char head[4];
  unsigned char headinfo[10];//2 bytes pak version, rest bytes unused
  unsigned int dir_offset;
  unsigned int dir_length;
};
struct STRUCT_PACKED dirsection
{
  char filename[120];
  unsigned int file_position;
  unsigned int file_length;
};
#include "compiler/pack_default"

struct pakfile
{
	pakheader header;
};



class PAKfileSystemManager
	{
	
private:
		
		int pakfiles_counter;							//file index
		dirsection *currentpak_files; 
		
		unsigned int checkout_filelength;
		unsigned int checkout_fileposition;
		
		
		typedef std::map<std::string, int> MapT;		//files map
		/*typedef MapT::iterator it;*/
		MapT _map;

		Cfile	SFile;
/*private:*/
public:
	
		PAKfileSystemManager();

public:
		
		~PAKfileSystemManager();
//		static PAKfileSystemManager* getInstance();
		
		void createPak(const char* filename);								//creates a fresh new pak file
		int extractPak(const char* file, char* output, pakfile *fp);		//gets a file from the pak into outputfile
		int insertPak(const char* file, const char* inpak, pakfile *fp);		//inserts a file into the pak
		pakfile *openPak(const char * filename);							//opens a pak and verifies it. returns a filepointer
		int closePak(pakfile *fp);									//closes the opened pak
		int checkPak(pakfile *fp);									//checks the pak (internally used only)
		byte *readFile(const char *filename, unsigned int *size);			//reads a generic file into a buffer
		bool inPak(const char *file, pakfile *fp, dirsection & sect);			//checks if a file exists within a pakfile
		dirsection *loadDirSection(pakfile *fp, int *x);        	//loads the dirsection of the given pakfile. x = number of files
		void printDirTree(pakfile *fp);								// cheap helper function that printf's out the files in a pak
		int getLongestFile(dirsection *files, int n);
		byte *extractMem(const char *file, pakfile* fp, unsigned int *size);	//loads a file from PAK to RAM
		
		int fileCheckout(const char *file, pakfile* fp, unsigned int *size);
		bool extractMemToBuffer(const char *file, pakfile* fp, byte* buffer);	
		void mapFilesFromPak();

        unsigned int getMajorVersion(const pakfile* fp) const;
        unsigned int getMinorVersion(const pakfile* fp) const;
			
	};

