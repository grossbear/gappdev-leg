/////////////////////////////////////////////////////////////////////////////////////////////
// GLDevInfo.h
//
// OpenGL Device Info Class
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLDEVINFO_H_
#define _GLDEVINFO_H_

#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"

#include "Renderapi.h"
#include "GLSupExtStruct.h"


class CGLDevInfo
{
public:
    static void InitExtensions();
    static uint32t GetDevParam(GAPIDEVCAPSPARAM param);

    static const CGLSupExtStruct& GetDevInfoStruct();

protected:
    static void GetGLSupExtInfo();
    static void SetupGLExtProc();

    static void GetVerProcAddr();
    static void GetArbProcAddr();
    static void GetExtProcAddr();
    /*
    static void InitAMD_Proc();
    static void InitARB_Proc();
    static void InitATI_Proc();
    static void InitEXT_Proc();
    static void InitNV_Proc();
    static void InitGLVer_Proc();
    */

    static const char *m_strExtensions;
    static const char *m_strVendor;
    static const char *m_strVersion;
    static const char *m_strRender;
    static const char *m_strVerGLSL;

    static CGLSupExtStruct m_GLSupExt;

};

#endif //_GLDEVINFO_H_