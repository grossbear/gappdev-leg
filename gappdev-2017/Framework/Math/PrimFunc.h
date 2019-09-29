///////////////////////////////////////////////////////////////////////////////////////
//  PrimFunc.h
//
//  Elementary Functions In Algebra Classes 
//  
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _ALGEBRA_FUNCTIONS_SPEC_H_
#define _ALGEBRA_FUNCTIONS_SPEC_H_

///////////////////////////////////////////////////////////////////////////////////////
// Vec2 
M_FORCE float MVEC2SQRT(float x)
{
#ifdef CMATH_VEC2_TABLE_SQRT
    return m_tsqrt(x);
#elif CMATH_VEC2_FAST_SQRT
    return m_fsqrt(x);
#else
    return sqrtf(x);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC2SQRT(double x)
{
    return sqrt(x);
}

M_FORCE float MVEC2SQRT(float x, float y)
{
#ifdef CMATH_VEC2_TABLE_SQRT
    return m_tsqrt(x*x + y*y);
#elif CMATH_VEC2_FAST_SQRT
    return m_fsqrt(x*x + y*y);
#else 
    return sqrtf(x*x + y*y);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC2SQRT(double x, double y)
{
    return sqrt(x*x + y*y);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC2REVSQRT(float x, float y)
{
    ASSERT((x*x + y*y) > FLOAT_EPS);

#ifdef CMATH_VEC2_TABLE_SQRT
    return m_rtsqrt(x*x + y*y);
#elif CMATH_VEC2_FAST_SQRT
    return m_rfsqrt(x*x + y*y);
#else
    return 1.0f/sqrtf(x*x + y*y);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC2REVSQRT(double x, double y)
{
    ASSERT((x*x + y*y) > DOUBLE_EPS);

    return sqrt(x*x + y*y);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MVEC2SINCOS(float angle, float &sina, float &cosa)
{
#ifdef CMATH_VEC2_TABLE_TRIGONOMETRY
    m_tsincosf(angle,sina,cosa);
#else
    msincosf(angle,sina,cosa);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MVEC2SINCOS(double angle, double &sina, double &cosa)
{
    msincosd(angle,sina,cosa);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC2COS(float angle)
{
#ifdef CMATH_VEC2_TABLE_TRIGONOMETRY
    return m_tcosf(angle);
#else
    return cosf(angle);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC2COS(double angle)
{
    return cos(angle);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC2ACOS(float x)
{
    return acosf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC2ACOS(double x)
{
    return acos(x);
}

///////////////////////////////////////////////////////////////////////////////////////
// Vec3 
M_FORCE float MVEC3SQRT(float x)
{
#ifdef CMATH_VEC3_TABLE_SQRT
    return m_tsqrt(x);
#elif CMATH_VEC3_FAST_SQRT
    return m_fsqrt(x);
#else
    return sqrtf(x);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC3SQRT(double x)
{
    return sqrt(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC3SQRT(float x, float y, float z)
{
#ifdef CMATH_VEC3_TABLE_SQRT
    return m_tsqrt(x*x + y*y + z*z);
#elif CMATH_VEC3_FAST_SQRT
    return m_fsqrt(x*x + y*y + z*z);
#else 
    return sqrtf(x*x + y*y + z*z);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC3SQRT(double x, double y, double z)
{
    return sqrt(x*x + y*y + z*z);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC3REVSQRT(float x, float y, float z)
{
    ASSERT((x*x + y*y + z*z) > FLOAT_EPS);

#ifdef CMATH_VEC3_TABLE_SQRT
    return m_rtsqrt(x*x + y*y + z*z);
#elif CMATH_VEC3_FAST_SQRT
    return m_rfsqrt(x*x + y*y + z*z);
#else
    return 1.0f/sqrtf(x*x + y*y + z*z);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC3REVSQRT(double x, double y, double z)
{
    ASSERT((x*x + y*y + z*z) > DOUBLE_EPS);

    return 1.0f/sqrt(x*x + y*y + z*z);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MVEC3SINCOS(float angle, float &sina, float &cosa)
{
#ifdef CMATH_VEC3_TABLE_TRIGONOMETRY
    m_tsincosf(angle,sina,cosa);
#else
    msincosf(angle,sina,cosa);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MVEC3SINCOS(double angle, double &sina, double &cosa)
{
    msincosd(angle,sina,cosa);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC3COS(float angle)
{
#ifdef CMATH_VEC3_TABLE_TRIGONOMETRY
    return m_tcosf(angle);
#else
    return cosf(angle);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC3COS(double angle)
{
    return cos(angle);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC3ACOS(float x)
{
    return acosf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC3ACOS(double x)
{
    return acos(x);
}


///////////////////////////////////////////////////////////////////////////////////////
// Vec4 
M_FORCE float MVEC4SQRT(float x)
{
#ifdef CMATH_VEC4_TABLE_SQRT
    return m_tsqrt(x);
#elif CMATH_VEC4_FAST_SQRT
    return m_fsqrt(x);
#else
    return sqrtf(x);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC4SQRT(double x)
{
    return sqrt(x);
}

M_FORCE float MVEC4SQRT(float x, float y, float z, float w)
{
#ifdef CMATH_VEC4_TABLE_SQRT
    return m_tsqrt(x*x + y*y + z*z + w*w);
#elif CMATH_VEC4_FAST_SQRT
    return m_fsqrt(x*x + y*y + z*z + w*w);
#else 
    return sqrtf(x*x + y*y + z*z + w*w);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC4SQRT(double x, double y, double z, double w)
{
    return sqrt(x*x + y*y + z*z + w*w);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC4REVSQRT(float x, float y, float z, float w)
{
    ASSERT((x*x + y*y + z*z + w*w) > FLOAT_EPS);

#ifdef CMATH_VEC4_TABLE_SQRT
    return m_rtsqrt(x*x + y*y + z*z + w*w);
#elif CMATH_VEC4_FAST_SQRT
    return m_rfsqrt(x*x + y*y + z*z + w*w);
#else
    return 1.0f/sqrtf(x*x + y*y + z*z + w*w);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC4REVSQRT(double x, double y, double z, double w)
{
    ASSERT((x*x + y*y + z*z + w*w) > DOUBLE_EPS);

    return sqrt(x*x + y*y + z*z + w*w);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MVEC4SINCOS(float angle, float &sina, float &cosa)
{
#ifdef CMATH_VEC4_TABLE_TRIGONOMETRY
    m_tsincosf(angle,sina,cosa);
#else
    msincosf(angle,sina,cosa);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MVEC4SINCOS(double angle, double &sina, double &cosa)
{
    msincosd(angle,sina,cosa);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC4COS(float angle)
{
#ifdef CMATH_VEC4_TABLE_TRIGONOMETRY
    return m_tcosf(angle);
#else
    return cosf(angle);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC4COS(double angle)
{
    return cos(angle);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MVEC4ACOS(float x)
{
    return acosf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MVEC4ACOS(double x)
{
    return acos(x);
}

///////////////////////////////////////////////////////////////////////////////////////
// Plane
M_FORCE float MPLANESQRT(float a, float b, float c)
{
#ifdef CMATH_PLANE_TABLE_SQRT
    return m_tsqrt(a*a + b*b + c*c);
#elif CMATH_PLANE_FAST_SQRT
    return m_fsqrt(a*a + b*b + c*c);
#else
    return sqrtf(a*a + b*b + c*c);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MPLANESQRT(double a, double b, double c)
{
    return sqrt(a*a + b*b + c*c);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MPLANEREVSQRT(float a, float b, float c)
{
#ifdef CMATH_PLANE_TABLE_SQRT
    return m_rtsqrt(a*a + b*b + c*c);
#elif CMATH_PLANE_FAST_SQRT
    return m_rfsqrt(a*a + b*b + c*c);
#else
    return 1.0f/sqrtf(a*a + b*b + c*c);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MPLANEREVSQRT(double a, double b, double c)
{
    return 1.0/sqrt(a*a + b*b + c*c);
}

///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Matrix3x3
M_FORCE void MMTX33SINCOS(float angle, float &sina, float &cosa)
{
#ifdef CMATH_MTX33_TABLE_TRIGONOMETRY
    m_tsincosf(angle,sina,cosa);
#else
    msincosf(angle,sina,cosa);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MMTX33SINCOS(double angle, double &sina, double &cosa)
{
    msincosd(angle,sina,cosa);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MMTX33SQRT(float x, float y, float z)
{
#ifdef CMATH_MTX33_TABLE_SQRT
    return m_tsqrt(x*x + y*y + z*z);
#elif CMATH_MTX33_FAST_SQRT
    return m_fsqrt(x*x + y*y + z*z);
#else
    return sqrtf(x*x + y*y + z*z);
#endif 
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MMTX33SQRT(double x, double y, double z)
{
    return sqrt(x*x + y*y + z*z);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MMTX33ASIN(float x)
{
    return asinf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MMTX33ASIN(double x)
{
    return asin(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MMTX33ATAN2(float y, float x)
{
    return atan2f(y,x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MMTX33ATAN2(double y, double x)
{
    return atan2(y,x);
}

///////////////////////////////////////////////////////////////////////////////////////
// Matrix4x4
M_FORCE void MMTX44SINCOS(float angle, float &sina, float &cosa)
{
#ifdef CMATH_MTX44_TABLE_TRIGONOMETRY
    m_tsincosf(angle,sina,cosa);
#else
    msincosf(angle,sina,cosa);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MMTX44SINCOS(double angle, double &sina, double &cosa)
{
    msincosd(angle,sina,cosa);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MMTX44SQRT(float x, float y, float z)
{
#ifdef CMATH_MTX44_TABLE_SQRT
    return m_tsqrt(x*x + y*y + z*z);
#elif CMATH_MTX44_FAST_SQRT
    return m_fsqrt(x*x + y*y + z*z);
#else
    return sqrtf(x*x + y*y + z*z);
#endif 
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MMTX44SQRT(double x, double y, double z)
{
    return sqrt(x*x + y*y + z*z);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MMTX44ASIN(float x)
{
    return asinf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MMTX44ASIN(double x)
{
    return asin(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MMTX44ATAN2(float y, float x)
{
    return atan2f(y,x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MMTX44ATAN2(double y, double x)
{
    return atan2(y,x);
}

///////////////////////////////////////////////////////////////////////////////////////
// Matrix4x4
M_FORCE void MQUATSINCOS(float angle, float &sina, float &cosa)
{
#ifdef CMATH_QUAT_TABLE_TRIGONOMETRY
    m_tsincosf(angle,sina,cosa);
#else
    msincosf(angle,sina,cosa);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE void MQUATSINCOS(double angle, double &sina, double &cosa)
{
    msincosd(angle,sina,cosa);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATATAN2(float y, float x)
{
    return atan2f(y,x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATATAN2(double y, double x)
{
    return atan2(y,x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATATAN(float x)
{
    return atanf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATATAN(double x)
{
    return atan(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATSIN(float x)
{
#ifdef CMATH_QUAT_TABLE_TRIGONOMETRY
    return m_tsinf(x);
#else
    return sinf(x);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATSIN(double x)
{
    return sin(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATASIN(float x)
{
    return asinf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATASIN(double x)
{
    return asin(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATACOS(float x)
{
    return acosf(x);
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATACOS(double x)
{
    return acos(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATSQRT(float x, float y, float z, float w)
{
#ifdef CMATH_QUAT_FAST_SQRT
    return m_fsqrt(x*x + y*y + z*z + w*w);
#elif CMATH_QUAT_TABLE_SQRT
    return m_tsqrt(x*x + y*y + z*z + w*w);
#else
    return sqrtf(x*x + y*y + z*z + w*w);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATSQRT(double x, double y, double z, double w)
{
    return sqrt(x*x + y*y + z*z + w*w);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATSQRT(float x, float y, float z)
{
#ifdef CMATH_QUAT_FAST_SQRT
    return m_fsqrt(x*x + y*y + z*z);
#elif CMATH_QUAT_TABLE_SQRT
    return m_tsqrt(x*x + y*y + z*z);
#else
    return sqrtf(x*x + y*y + z*z);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATSQRT(double x, double y, double z)
{
    return sqrt(x*x + y*y + z*z);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATSQRT(float x)
{
#ifdef CMATH_QUAT_FAST_SQRT
    return m_fsqrt(x);
#elif CMATH_QUAT_TABLE_SQRT
    return m_tsqrt(x);
#else
    return sqrtf(x);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATSQRT(double x)
{
    return sqrt(x);
}

///////////////////////////////////////////////////////////////////////////////////////
M_FORCE float MQUATRSQRT(float x, float y, float z, float w)
{
#ifdef CMATH_QUAT_FAST_SQRT
    return m_rfsqrt(x*x + y*y + z*z + w*w);
#elif CMATH_QUAT_TABLE_SQRT
    return 1.0f/m_tsqrt(x*x + y*y + z*z + w*w);
#else
    return 1.0f/sqrtf(x*x + y*y + z*z + w*w);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////
M_FORCE double MQUATRSQRT(double x, double y, double z, double w)
{
    return 1.0/sqrt(x*x + y*y + z*z + w*w);
}

///////////////////////////////////////////////////////////////////////////////////////

#endif //_ALGEBRA_FUNCTIONS_SPEC_H_
