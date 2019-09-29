///////////////////////////////////////////////////////////////////////////////////////
//  StdFileStream.cpp
//
//  File Input/Output Stream Class Using Standard C Functions
//  Created: 23-11-2011
//
///////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "../FileStream.h"


///////////////////////////////////////////////////////////////////////////////////////
CStd32FileStream::CStd32FileStream():m_file(NULL),m_error(0),m_size(0){}

///////////////////////////////////////////////////////////////////////////////////////
CStd32FileStream::~CStd32FileStream()
{
    Close();
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CStd32FileStream::Open(const char *filename, OpenMode mode)
{
    if(filename == NULL)
        return (boolt)false;

    if(m_file != NULL)
        return (boolt)false;

    static const char mode_tab[][4] = {"rb","wb+","wb+","ab","ab+"};

    m_file = fopen(filename,&mode_tab[mode][0]);
    if(m_file == NULL)
    {
        //m_error = -1;
        return (boolt)false;
    }

    return (boolt)true;
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CStd32FileStream::Close()
{
    if(!m_file)
        return false;

    int error = fclose(m_file);
    m_file = NULL;
    m_size = 0;
    m_error = 0;
    return (boolt)(error == 0);
}

///////////////////////////////////////////////////////////////////////////////////////
uint_t CStd32FileStream::Read(void *data, uint_t size)
{
    if(m_file == NULL)
        return -1;

    size_t readed = fread(data,size,1,m_file);

    return (uint_t)readed;
}

///////////////////////////////////////////////////////////////////////////////////////
uint_t CStd32FileStream::Write(const void *data, uint_t size)
{
    if(m_file == NULL)
        return -1;
    size_t written = fwrite(data,size,1,m_file);

    return (uint_t)written;
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CStd32FileStream::IsOpened()
{
    return (boolt)(m_file != NULL);
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CStd32FileStream::Seek(isize_t offset, SeekMode mode)
{
    int error = 0;
    static const int mode_tab[] = {SEEK_SET,SEEK_CUR,SEEK_END};
#ifdef SIZE_INT_64
    error = _fseeki64(m_file,(__int64)offset,mode_tab[mode]);
#else
    error = fseek(m_file,offset,mode_tab[mode]);
#endif
    return (boolt)(error==0);
}

///////////////////////////////////////////////////////////////////////////////////////
isize_t CStd32FileStream::GetPos()
{
    if(m_file == NULL)
        return -1;

    isize_t pos = -1;
#ifdef SIZE_INT_64
    pos = _ftelli64(m_file);
#else
    pos = ftell(m_file);
#endif
    return pos;
}

///////////////////////////////////////////////////////////////////////////////////////
isize_t CStd32FileStream::GetSize()
{
    if(m_file == NULL)
        return -1;

    if(m_size == 0)
    {
        isize_t currpos = GetPos();
        Seek(0,seek_end);
        m_size = GetPos();
        Seek(currpos,seek_set);

    }

    return m_size;
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CStd32FileStream::IsEOS()
{
    if(m_file == NULL)
        return (boolt)false;
    int eof = feof(m_file);
    return (boolt)(eof==0);
}

///////////////////////////////////////////////////////////////////////////////////////