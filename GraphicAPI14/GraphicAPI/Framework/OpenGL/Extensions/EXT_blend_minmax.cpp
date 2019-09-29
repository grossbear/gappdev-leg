////////////////////////////////////////////////////////////////////////////////////////////////
//	EXT_blend_minmax.cpp 
//	EXT_blend_minmax Extension Setup
//	Created: 6th August 2006
//	
////////////////////////////////////////////////////////////////////////////////////////////////

/* GL_EXT_blend_minmax */
#ifdef GL_EXT_blend_minmax 

PFNGLBLENDEQUATIONEXTPROC	glBlendEquationEXT = NULL;

bool SetupEXTBlendMinMax()
{
	if(!IsExtensionSupported("GL_EXT_blend_minmax"))
		return false;

	//get function pointers
	glBlendEquationEXT	=	(PFNGLBLENDEQUATIONEXTPROC)
			wglGetProcAddress("glBlendEquationEXT");

	return true;
}

#endif //GL_EXT_blend_minmax 