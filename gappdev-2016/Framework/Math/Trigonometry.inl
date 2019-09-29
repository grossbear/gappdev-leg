//////////////////////////////////////////////////////////////////////////////
//
//  File:       Trigonometry.inl
//  Content:    Trigonometry inline functions
//
//////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float msin(float x)
{
    return sinf(x);
}

M_INLINE double msin(double x)
{
    return sin(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float mcos(float x)
{
    return cosf(x);
}

M_INLINE double mcos(double x)
{
    return cos(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE void  msincos(float angle, float &sinVal, float &cosVal)
{
    //printf("msincos\n");
    sinVal = msin(angle);
    cosVal = mcos(angle);
}

M_INLINE void  msincos(double angle, double &sinVal, double &cosVal)
{
    sinVal = msin(angle);
    cosVal = mcos(angle);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE void msincosf(float a,float &s, float &c)
{
    s = sinf(a);
    c = cosf(a);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE void  msincosd(double a, double &s, double &c)
{
    s = sin(a);
    c = cos(a);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float mtan(float x)
{
    return tanf(x);
}

M_INLINE double mtan(double x)
{
    return tan(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float macos(float x)
{
    return acosf(x);
}

M_INLINE double macos(double x)
{
    return acos(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float masin(float x)
{
    return asinf(x);
}

M_INLINE double masin(double x)
{
    return asin(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float matan(float x)
{
    return atanf(x);
}

M_INLINE double matan(double x)
{
    return atan(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float matan2(float x, float y)
{
    return atan2f(x,y);
}

M_INLINE double matan2(double x, double y)
{
    return atan2(x,y);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float msinh(float x)
{
    return sinhf(x);
} 

M_INLINE double msinh(double x)
{
    return sinh(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float mcosh(float x)
{
    return coshf(x);
}

M_INLINE double mcosh(double x)
{
    return cosh(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_INLINE float mtanh(float x)
{
    return tanhf(x);
}

M_INLINE double mtanh(double x)
{
    return tanh(x);
}