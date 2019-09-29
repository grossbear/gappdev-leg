/////////////////////////////////////////////////////////////////////////////////////////////
// ImageIOManager.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS

#include "Image.h"
#include "ImageIOManager.h"
#include "Utils/IOStream/IDataStream.h"
#include "Utils/IOStream/FileStream.h"

#ifdef _SET_IMAGE_IO_FORMATS
#include "BMImageIO/BMP.h"
#endif //_SET_IMAGE_IO_FORMATS

#define EXTSTR_MAX_SIZE 16

/////////////////////////////////////////////////////////////////////////////////////////////
CBMImageIOSystem *CBMImageIOSystem::m_pInstance = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////
CBMImageIOSystem::CBMImageIOSystem()
{
    
}

/////////////////////////////////////////////////////////////////////////////////////////////
CBMImageIOSystem::~CBMImageIOSystem()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
CBMImageIOSystem* CBMImageIOSystem::GetSingleton()
{
    if(m_pInstance == NULL)
        m_pInstance = new CBMImageIOSystem();

    return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CBMImageIOSystem::LoadBMImage(const char *filename, uint32t loadflags)
{
    if(filename==NULL) return NULL;

    char strext[EXTSTR_MAX_SIZE+1]={0};

    if(!GetFileExtension(strext,filename)) return NULL;

    ToLower(strext);

    CFileStream fs;
    boolt opened = fs.Open(filename,open_read);
    if(!opened)
        return NULL;

    CBMImage *image = NULL;
    if(m_imgio_map.find(strext) != m_imgio_map.end())
    {
        image = m_imgio_map[strext]->LoadBMImage(&fs,loadflags);
    }

    if(!image)
    {
        std::map<std::string,CBMImageIO*>::iterator it = m_imgio_map.begin();
        for(; it != m_imgio_map.end(); it++)
        {
            image = it->second->LoadBMImage(&fs,loadflags);
            if(image != NULL)
                break;
        }
    }

    return image;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool   CBMImageIOSystem::SaveBMImage(const CBMImage *image, const char *filename, ImgIOParam *params)
{
    if(image==NULL) return false;
    if(filename==NULL) return false;

    char strext[EXTSTR_MAX_SIZE+1]={0};

    if(!GetFileExtension(strext,filename)) return NULL;

    ToLower(strext);

    CFileStream fs;
    boolt opened = fs.Open(filename,open_read_write);
    if(!opened)
        return false;

    if(m_imgio_map.find(strext) != m_imgio_map.end())
    {
        return m_imgio_map[strext]->SaveBMImage(&fs,image,params);
    }
    
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CBMImageIOSystem::AddImageIO(CBMImageIO *imageio)
{
    if(imageio == NULL)
        return false;

    const char *ext = imageio->GetExtString();
    char strext[EXTSTR_MAX_SIZE+1]={0};
    size_t extlen = strlen(ext);
    extlen = (extlen>EXTSTR_MAX_SIZE)?(EXTSTR_MAX_SIZE):(extlen);
    memcpy(strext,ext,sizeof(char)*extlen);

    ToLower(strext);
    char *ptr = strext;
    size_t pos = 0;
    while(*ptr != ('\0'))
    {
        if(! ((*ptr >= ('0') && *ptr <= ('9')) || (*ptr >= ('a') && *ptr <= ('z'))) )
            break;
        ptr++;
        pos++;
    }

    if(pos == 0)
        return false;

    strext[pos] = ('\0');
    std::string strkey = strext;
    if(m_imgio_map.find(strkey) == m_imgio_map.end())
    {
        m_imgio_map[strkey] = imageio;
        return true;
    }


    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool  CBMImageIOSystem::GetFileExtension(char *ext, const char *filename)
{
    uint32t len = strlen(filename);
    const char *pos = NULL;
    const char *pos2 = NULL;
    const char *string = filename;
    bool end = false;

    pos = strstr(string,".");
    if(pos==NULL) return false;
    if(pos-filename == len) return false;

    string = pos+1;

    while(!end)
    {
        pos2 = strstr(string,".");
        if(pos2==NULL)
        {
            uint32t namesize = pos - filename;
            if(len - namesize > EXTSTR_MAX_SIZE) return false;
            strcpy(ext,&pos[1]);
            return true;
        }
    
        if(pos2-filename == len) return false;
        string = pos2+1;
        pos = pos2;   
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CBMImageIOSystem::ToLower(char *ext)
{
    char *ptr = ext;
    while(*ptr != ('\0'))
        *ptr++ = tolower(*ptr);
    
}

#ifdef _SET_IMAGE_IO_FORMATS
void CBMImageIOSystem::SetImageIOMap()
{
	CBMPImageIO *bmpimageio = new CBMPImageIO();
	AddImageIO(bmpimageio);
}
#endif //_SET_IMAGE_IO_FORMATS