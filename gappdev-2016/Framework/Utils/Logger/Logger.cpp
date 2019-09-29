/////////////////////////////////////////////////////////////////////////////////////////////
// Logger.h
//
// Logger Functions
/////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"

static const int LOGGER_FILENAME_MAX              = 256;
static char g_LoggerFileNames[LOG_TYPE_NUM][LOGGER_FILENAME_MAX] = {0};


void LoggerCreate(LOG_TYPE log_type, const char *filename)
{
    FILE *outfile;

    memset(g_LoggerFileNames[log_type], 0, LOGGER_FILENAME_MAX);
    strncpy(g_LoggerFileNames[log_type], filename, LOGGER_FILENAME_MAX - 1);

    if ((outfile = fopen(g_LoggerFileNames[log_type], "w")) != NULL)
        fclose(outfile);
}



void LoggerDestroy()
{
    //
}

void LoggerWrite(LOG_TYPE log_type, const char *format, ...)
{
    va_list ap;
    FILE    *output;

    if ((output = fopen(g_LoggerFileNames[log_type], "a+")) == NULL)
        return;

    va_start(ap, format);
    vfprintf(output, format, ap);
    va_end(ap);

    fclose(output);
}
