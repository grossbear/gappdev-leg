///////////////////////////////////////////////////////////////////////////////////////
//  StdFileStream.h
//
//  File Input/Output Stream Class Using Standard C Functions
//  Created: 23-11-2011
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __STD_FILE_STREAM_H__
#define __STD_FILE_STREAM_H__

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////////////
class CStd32FileStream : public IDataStream
{
public:
    CStd32FileStream();
    virtual ~CStd32FileStream();

    boolt       Open(const char *filename, OpenMode mode);
    boolt       Close();

    uint_t      Read(void *data, uint_t size);
    uint_t      Write(const void *data, uint_t size);

    boolt       IsOpened();
    boolt       Seek(isize_t offset, SeekMode pos);

    isize_t     GetPos();
    isize_t     GetSize();

    boolt       IsEOS();

protected:
    FILE        *m_file;
    int_t       m_error;
    isize_t     m_size;
};

///////////////////////////////////////////////////////////////////////////////////////


#endif //__STD_FILE_STREAM_H__