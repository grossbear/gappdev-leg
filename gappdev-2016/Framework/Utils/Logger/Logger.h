/////////////////////////////////////////////////////////////////////////////////////////////
// Logger.h
//
// Logger Functions
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <cstdio>
#include <cstring>
#include <cstdarg>

enum LOG_TYPE
{
    LOG_RENDER_INFO,
    LOG_RENDER_ERROR,
    LOG_PROGRAM_INFO,
    LOG_PROGRAM_ERROR,
    LOG_TYPE_NUM = 4
};

#define _USE_LOG_SYSTEM
#if defined (_USE_LOG_SYSTEM)

#if defined(_MSC_VER)
#define LOG_CREATE(log_type,logname) LoggerCreate(log_type,logname)
#define LOG_DEBUG(log_type,str,...) LoggerWrite(log_type,"%s:%s:%d DEBUG " str, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(log_type,str,...) LoggerWrite(log_type,"%s:%s:%d ERROR " str, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(log_type,str,...) LoggerWrite(log_type,"%s:%s:%d ERROR " str)
#elif defined(__MINGW32__)
#define LOG_CREATE(log_type,logname) LoggerCreate(log_type,logname)
#define LOG_DEBUG(log_type,str,...) LoggerWrite(log_type,"%s:%s:%d DEBUG " str, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(log_type,str,...) LoggerWrite(log_type,"%s:%s:%d ERROR " str, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(log_type,str,...) LoggerWrite(log_type,"%s:%s:%d ERROR " str)
#endif

#else

#define LOG_CREATE(log_type,logname) 
#define LOG_DEBUG(log_type,str,...) 
#define LOG_ERROR(log_type,str,...) 
#define LOG_INFO(log_type,str,...) 

#endif //_USE_LOG_SYSTEM

//#define ASSERT(x)          assert(x)

void LoggerCreate(LOG_TYPE log_type, const char *fileName);

void LoggerDestroy();

void LoggerWrite(LOG_TYPE log_type, const char *format, ...);

#endif //_LOGGER_H_