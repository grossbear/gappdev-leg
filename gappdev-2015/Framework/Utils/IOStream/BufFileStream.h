///////////////////////////////////////////////////////////////////////////////////////
//  BufFileStream.h
//
//  Input/Output Buffer File Stream Class
//  Created: 23-11-2011
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __BUFFILESTREAM_H__
#define __BUFFILESTREAM_H__

#include "FileStream.h"

/*
///////////////////////////////////////////////////////////////////////////////////////
class CBufFileStream : public IDataStream();
{
public:
    CBufFileStream();
    virtual ~CBufFileStream();

    uint_t  Read(void *data, uint_t size);
    uint_t  Write(const void *data, uint_t size);

    boolt   IsOpened();
    boolt   Seek(isize_t offset, SeekMode mode);

    boolt   Open(const char *filename, OpenMode mode);
    boolt   Close();

    isize_t GetPos();
    isize_t GetSize();

    boolt   IsEOS();

protected:

    int_t   m_buf_size;
    unsigned char *m_buffer;
    int_t   m_buf_pos;
    int_t   m_buf_fill;
};
*/

///////////////////////////////////////////////////////////////////////////////////////

#endif //__BUFFILESTREAM_H__