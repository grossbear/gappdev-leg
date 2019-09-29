///////////////////////////////////////////////////////////////////////////////////////
//  Win32FileStream.cpp
//
//  Win32 File Input/Output Stream Class
//  Created: 23-11-2011
//
///////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "../FileStream.h"

///////////////////////////////////////////////////////////////////////////////////////
CWin32FileStream::CWin32FileStream():m_handle(NULL),m_error(0),m_size(0){}

///////////////////////////////////////////////////////////////////////////////////////
CWin32FileStream::~CWin32FileStream()
{
    Close();
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CWin32FileStream::Open(const char *filename, OpenMode mode)
{
    if(filename == NULL)
        return (boolt)false;

    if(m_handle != NULL)
        return (boolt)false;

    /*
    open_read,
    open_read_write,
    open_create,
    open_overwrite,
    open_append,
    */

    DWORD access_flags = (mode != open_read) ? (GENERIC_READ|GENERIC_WRITE) : (GENERIC_READ);
    DWORD share_flags = FILE_SHARE_READ; //(mode == open_read) ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : (FILE_SHARE_READ);
    static const DWORD create_tab[] = {OPEN_EXISTING,OPEN_ALWAYS,CREATE_NEW,CREATE_ALWAYS,OPEN_EXISTING};
    DWORD flags = 0;

    m_handle = CreateFileA(filename,access_flags,share_flags,NULL,create_tab[mode],flags,NULL);

    return (boolt)(m_handle != INVALID_HANDLE_VALUE);
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CWin32FileStream::Close()
{
    if(!m_handle)
        return false;

    BOOL bresult = CloseHandle(m_handle);
    m_error = 0;
    m_size = 0;
    m_handle = NULL;

    return (boolt)bresult;
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CWin32FileStream::IsOpened()
{
    return (boolt)((m_handle != INVALID_HANDLE_VALUE) && (m_handle != NULL));
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CWin32FileStream::IsEOS()
{
    return (boolt)(GetPos() == m_size);
}

///////////////////////////////////////////////////////////////////////////////////////
isize_t CWin32FileStream::GetPos()
{
#ifdef SIZE_INT_64
    LARGE_INTEGER li;
    li.QuadPart = 0;
    li.LowPart = SetFilePointer(m_handle, 0, &li.HighPart, FILE_CURRENT);
    DWORD dwError = GetLastError();
    if(li.LowPart != INVALID_SET_FILE_POINTER && dwError == NO_ERROR)
        return (isize_t)li.QuadPart;
#else
    DWORD dwPtrLow = SetFilePointer(m_handle, 0, 0, FILE_CURRENT);
    DWORD dwError = GetLastError();
    if(dwPtrLow != INVALID_SET_FILE_POINTER && dwError == NO_ERROR)
        return (isize_t)dwPtrLow;
#endif //SIZE_INT_64

    return -1;
}

///////////////////////////////////////////////////////////////////////////////////////
isize_t CWin32FileStream::GetSize()
{
#ifdef SIZE_INT_64
    LARGE_INTEGER li;
    li.QuadPart = 0;
    //li.LowPart = GetFileSize(m_handle,&li.HighPart);
    BOOL result = GetFileSizeEx(m_handle,&li);
    DWORD dwError = GetLastError();
    if(result && dwError == NO_ERROR)
        return (isize_t)li.QuadPart;
#else
    DWORD dwSize = GetFileSize(m_handle,NULL);
    DWORD dwError = GetLastError();
    if(dwSize != INVALID_FILE_SIZE && dwError == NO_ERROR)
        return (int_t)dwSize;
#endif //SIZE_INT_64

    return -1;
}

///////////////////////////////////////////////////////////////////////////////////////
boolt CWin32FileStream::Seek(isize_t offset, SeekMode mode)
{
    if(m_handle == NULL)
        return false;

    static const DWORD mode_tab[] = {FILE_BEGIN,FILE_CURRENT,FILE_END};

#ifdef SIZE_INT_64
    LARGE_INTEGER li;
    li.QuadPart = (LONGLONG)offset;
    li.LowPart = SetFilePointer(m_handle, li.LowPart, &li.HighPart, mode_tab[mode]);
    DWORD dwError = GetLastError();
    if(li.LowPart == INVALID_SET_FILE_POINTER || dwError != NO_ERROR)
        return false;
#else
    DWORD dwPtrLow = SetFilePointer(m_handle, offset, 0, mode_tab[mode]);
    DWORD dwError = GetLastError();
    if(dwPtrLow == INVALID_SET_FILE_POINTER && dwError != NO_ERROR)
        return false;
#endif //SIZE_INT_64

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
uint_t CWin32FileStream::Read(void *data, uint_t size)
{
    if(m_handle == NULL)
        return 0;

    DWORD dwReaded = 0;
    if(!ReadFile(m_handle, data, (DWORD)size, &dwReaded, NULL))
        return 0;

    return (uint_t)dwReaded;
}

///////////////////////////////////////////////////////////////////////////////////////
uint_t CWin32FileStream::Write(const void *data, uint_t size)
{
    if(m_handle == NULL)
        return 0;

    DWORD dwWritten = 0;
    if(!WriteFile(m_handle, data, (DWORD)size, &dwWritten, NULL))
        return 0;

    return (uint_t)dwWritten;
}
///////////////////////////////////////////////////////////////////////////////////////



