#include "pak.h"
#include "string.h" // strlen

using namespace utils;

const static char pak_sig[] = { 'P', 'A', 'C', 'K' };

#define PAK_MAJOR_VERSION 1
#define PAK_MINOR_VERSION 1

PAKfileSystemManager::PAKfileSystemManager()
{
	checkout_filelength = 0;
	checkout_fileposition = 0;
    currentpak_files = 0;
}

PAKfileSystemManager::~PAKfileSystemManager()
{
    if(currentpak_files)
        free(currentpak_files);
}

void PAKfileSystemManager::createPak(const char* filename)
{
    pakfile mypak;
	memcpy(mypak.header.head, pak_sig, 4);
    memset(mypak.header.headinfo, 0, 10);
    mypak.header.headinfo[0] = PAK_MAJOR_VERSION;
    mypak.header.headinfo[1] = PAK_MINOR_VERSION;
    mypak.header.dir_offset = sizeof(mypak.header.head) + 1;
    mypak.header.dir_length = 1;
    if(SFile.open(filename,overwrite)==err_ok)
    {	  
        SFile.write(&mypak.header,sizeof(mypak.header));
        SFile.close(); 
    }
}

int PAKfileSystemManager::checkPak(pakfile *fp)
{
	if (memcmp(fp->header.head, pak_sig, 4) != 0)
        return -1;  //ident corrupt
    if( fp->header.dir_offset < (sizeof(fp->header.head) + 1) || fp->header.dir_length < 1)
        return -2;  //header corrupt
    return 0;
}


pakfile* PAKfileSystemManager::openPak(const char * filename)
{
    pakfile *mypak = 0;

    pakfiles_counter = 0;

    SFile.open(filename, open_r);
	if (SFile.is_opened()) {
	    mypak = (pakfile*) malloc(sizeof(pakfile));
		SFile.read(&mypak->header, sizeof(pakheader));
		if (checkPak(mypak) >= 0) {
			if(currentpak_files)
				free(currentpak_files);
			currentpak_files = loadDirSection(mypak, &pakfiles_counter);
			mapFilesFromPak();
		} else {
			closePak(mypak);
			mypak = 0;
		}
	}
    return mypak;
}


int PAKfileSystemManager::insertPak(const char* file, const char* inpak, pakfile *fp)
{
    unsigned int count;
    dirsection sect;
    dirsection *files;
    int x =0;
    
//    if(sizeof(inpak) > 56) //to stay compatible to Quake 2 format, this may not be longer than 56 char
//      return -1;
    byte* buffer = readFile(file, &count);
    if(buffer==NULL)
        return -1;
       
    if(fp->header.dir_length == 1)
       {
         SFile.seek(14 ,seek_set);     
         fp->header.dir_offset = sizeof(fp->header) + sizeof(byte)*count;
         fp->header.dir_length = sizeof(dirsection);
         //write new offset and length
         SFile.write(&fp->header.dir_offset,sizeof(fp->header.dir_offset));
         SFile.write(&fp->header.dir_length,sizeof(fp->header.dir_length));
         //now write the raw binary data
         SFile.write(buffer,count);
           
         strcpy(sect.filename, inpak);
         sect.file_position = sizeof(pakheader);
         sect.file_length = sizeof(byte) * count;
         
         //now write the dirsection
         SFile.write(&sect,sizeof(sect));
      }
       else
       {
         //first check if the file already exists in the pak
         //for this purpose we read all the dirsections into a buffer and then check them


		   if( fp->header.dir_length % sizeof(dirsection) != 0 )
              return -1;                      // oh my gosh, check yer HD !

         //now check how many files are in the pak
		   x = fp->header.dir_length / sizeof(dirsection);

         //allocate space for all the files including one new
         files = static_cast<dirsection*>/*(dirsection*)*/( malloc((x+1) * sizeof(dirsection)) );
         //read all the dirsections into 'files'
         SFile.seek(fp->header.dir_offset ,seek_set);     
         SFile.read(files,sizeof(dirsection)*(x));
         //now check if the file exists
         int i;
         for( i = 0; i < x; i++)
         {

           char *name = files[i].filename;

           if( strcmp(name, inpak) == 0 )
              return -1;  // already exists
         }

         //now calculate the new offset
         SFile.seek(14 ,seek_set);     
         fp->header.dir_offset = fp->header.dir_offset + count;
         fp->header.dir_length = fp->header.dir_length + sizeof(dirsection);
         //write new offset and length
         SFile.write(&fp->header.dir_offset,sizeof(fp->header.dir_offset));
         SFile.write(&fp->header.dir_length,sizeof(fp->header.dir_length));
         //now go to end of binary data and write the file
         SFile.seek(fp->header.dir_offset - count ,seek_set);     
         SFile.write(buffer,count);
         strcpy(files[x].filename, inpak);
         files[x].file_position = fp->header.dir_offset - count;
         files[x].file_length = sizeof(byte) * count;
         //now write the entire dirsection, since the old one has been overwritten by binary data
         SFile.write(files,sizeof(dirsection)*(x+1));
                
       }                     

       
       free(buffer);

    return 0;
}



byte* PAKfileSystemManager::readFile(const char *filename, unsigned int *size)
{
    byte c;
    int b;
    unsigned int count =0;
    Cfile iFile;
    if(iFile.open(filename,open_r)!=err_ok) return NULL;
    while(iFile.read(&c, sizeof(c)) > 0)
       count++;
    byte *buffer = static_cast<byte*> /*(byte*)*/( malloc(count * sizeof(byte)) );
    iFile.seek(0 ,seek_set);     
    count = 0;
    while(!iFile.is_eof()) {
		iFile.read(  &b, 1);	
		buffer[count] = b;
		count++;
	}
	iFile.close();
	*size = count;
	return buffer;
}


int PAKfileSystemManager::closePak(pakfile *fp)
{
    SFile.close();
    free(fp);
    return 0;
}

bool PAKfileSystemManager::inPak(const char *file, pakfile *fp, dirsection & sect)
{
	if (currentpak_files) {
		MapT::const_iterator it = _map.find(file);
		if (it != _map.end()) {
			int index = it->second;
			strcpy(sect.filename, currentpak_files[index].filename);
			sect.file_position = currentpak_files[index].file_position;
			sect.file_length = currentpak_files[index].file_length;
			return true;
		}
	}
	return false;
}



int PAKfileSystemManager::extractPak(const char *file, char *output, pakfile *fp)
{
	dirsection mysect;
	Cfile iFile;
	if (fp == NULL) return -1;
	if (!inPak(file, fp, mysect)) return -1;   //not found
	iFile.open(output,overwrite);
	//now go to the address in the pak
	SFile.seek(mysect.file_position ,seek_set);     
	byte *buffer = static_cast<byte*> /*(byte*)*/( malloc(mysect.file_length * sizeof(byte)) );
	if (buffer) {
		//now read the data into the buffer
		SFile.read(buffer,mysect.file_length);
		//now write the buffer to the output file
		iFile.write(buffer,mysect.file_length);
		iFile.close();
	}
	free(buffer);
	return 0;
}

dirsection* PAKfileSystemManager::loadDirSection(pakfile *fp, int *x)
{
	dirsection *files;
	// see how many files are in the pak
	*x = fp->header.dir_length / sizeof(dirsection);

	//allocate space for all the files including one new (for possible insertion)
	files = static_cast<dirsection*> /*(dirsection*)*/( malloc(*x * sizeof(dirsection)) );

	//read all the dirsections into 'files'
	SFile.seek(fp->header.dir_offset ,seek_set);     
	SFile.read(files, sizeof(dirsection)*(*x));

	return files;
}
  
void PAKfileSystemManager::printDirTree(pakfile *fp)
{
/*  int x;

	
  if(fp == NULL) { printf("pak: File not found.\n"); return;}
	
  dirsection *myfiles = loadDirSection(fp, &x);
  
  char *name;
  x--;
  while( x >= 0)
  {
   name = myfiles[x].filename;
  printf("%25s", name);                  //give out the pathname of the file in pak
  printf("\t\t");
  printf("%8i", myfiles[x].file_length); //give out the size in bytes
  printf("  Bytes\n");
  x--;
  }*/
}


int PAKfileSystemManager::getLongestFile(dirsection *files, int n)
{
	size_t longest = 0;
	size_t curr;
	while( n >= 0)
	{
		curr = strlen(files[n].filename);
		if( curr > longest )
			longest = curr;
	n--;	
	}
    return static_cast<int>(longest);
}	


byte* PAKfileSystemManager::extractMem(const char *file, pakfile* fp, unsigned int *size)
{
	dirsection mysect;
	if (fp == NULL) return NULL;
  	if (!inPak(file, fp, mysect)) return NULL;    //not found
	SFile.seek(mysect.file_position ,seek_set);     
	byte *buffer = static_cast<byte*> /*(byte*)*/( malloc(mysect.file_length * sizeof(byte)) );
	if (buffer) {
		*size = mysect.file_length;
		SFile.read(buffer, mysect.file_length);
	}
    return buffer;
}        

void PAKfileSystemManager::mapFilesFromPak()
{
    _map.clear();
    for(int i = 0; i < pakfiles_counter; i++)
    {
        _map.insert(std::make_pair(currentpak_files[i].filename, i));
    }
}

bool PAKfileSystemManager::extractMemToBuffer(const char *file, pakfile* fp, byte* buffer)
{
    if(this->checkout_filelength != 0 && this->checkout_fileposition != 0)
    {
	    SFile.seek(this->checkout_fileposition ,seek_set);
	    if(SFile.read(buffer, this->checkout_filelength) == (int)this->checkout_filelength)
		    return true;
    }

    return false;
}

int PAKfileSystemManager::fileCheckout(const char *file, pakfile* fp, unsigned int *size)
{
	this->checkout_filelength = 0;
	this->checkout_fileposition = 0;
	if (fp == NULL) return 0;
  	//test if the file exists in the pak
	dirsection mysect;
	if (!inPak(file, fp, mysect)) return 0;
	this->checkout_filelength = mysect.file_length;
	this->checkout_fileposition = mysect.file_position;
	*size = mysect.file_length;
	return 1;
}

unsigned int PAKfileSystemManager::getMajorVersion(const pakfile* fp) const
{
	return (fp) ? fp->header.headinfo[0] : 0;
}

unsigned int PAKfileSystemManager::getMinorVersion(const pakfile* fp) const
{
	return (fp) ? fp->header.headinfo[1] : 0;
}

//#endif
