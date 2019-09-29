/////////////////////////////////////////////////////////////////////////////////////////////
// getproc.h
//
// OpenGL Get Function Pointers Defines
/////////////////////////////////////////////////////////////////////////////////////////////

// Getting Function Address From Driver
#define OPENGL_GET_PROC(p,n) n = (p)wglGetProcAddress(#n);
//if (NULL == n) \
//{ \
//LOG_ERROR("Loading extension '%s' fail (%d)\n", #n, GetLastError()); \
//return false; \
//}

#define OPENGL_GET_PROC_RESULT(p,n) \
    n = (p)wglGetProcAddress(#n); \
    //if (NULL == n) \
    //{ \
    //LOG_ERROR("Loading extension '%s' fail (%d)\n", #n, GetLastError()); \
    //return false; \
    //}

#define OPENGL_GET_SUP_EXT(s,n) \
    s.find(#n) != std::string::npos; \

// Checking OpenGL Errors
#define OPENGL_CHECK_FOR_ERRORS() \
    if ((g_OpenGLError = glGetError()) != GL_NO_ERROR) \
    //--LOG_ERROR("OpenGL error 0x%X\n", (unsigned)g_OpenGLError);