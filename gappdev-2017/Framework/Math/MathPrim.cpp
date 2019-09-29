///////////////////////////////////////////////////////////////////////////////////////
//  MathFunc.h
//
//  Some Mathematics Primary Functions Definitions
//  Created: 11-03-2007
//
///////////////////////////////////////////////////////////////////////////////////////

#include "PlatformTypes.h"

#include <math.h>

#include "MathConst.h"
#include "MathDefs.h"

#include "MathLibDefs.h"

#ifdef MATH_PRIM_SSE 
#include <xmmintrin.h>
#include <memory.h>
#endif

#include "MathPrim.h"

// INTORFLOAT Union For Easy Access To Bits Of A Float.
typedef union tINTFLOAT
{
    int32t  i;          // As Integer
    float   f;          // As Float
    struct              // As Bit Fields
    {
        unsigned int    sign:1;
        unsigned int    biasedexponent:8;
        unsigned int    significand;
    }
    bits;
}
INTFLOAT;

// Bias Constant Used For Fast Conversions Between Int And Float. First Element
// In INTORFLOAT Union Is Integer -- We Are Storing Biased Exponent 23, Mantissa .1, Which
// Is Equivalent To 1.5 x 2^23. 
const INTFLOAT  bias = {((23 + 127) << 23) + (1 << 22)};

///////////////////////////////////////////////////////////////////////////////////////
// Converting Functions From Integer To Float Type And Vice Versa
///////////////////////////////////////////////////////////////////////////////////////
M_API int32t mftoi(float f)
{
    ASSERT(f >= -4194304.0f && f <= 4194304.0f);

    INTFLOAT fi;    
    fi.f = f + bias.f;
    return fi.i - bias.i;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API float mitof(int32t i)
{
    ASSERT(i >= -4194304 && i <= 4194304);

    INTFLOAT fi;    
    fi.i = i + bias.i;
    return fi.f - bias.f;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
M_API float     mmakef(int32t sign, int32t exp, int32t mant)
{
    int32t i = (sign << 31) | ((exp & 255) << 23) | (mant & ((1 << 23) - 1));
    float f = *(float*)&i;

    return f;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API void      msplitf(float f, int32t &sign, int32t &exp, int32t &mant)
{
    int32t i = *(int32t*)&f;

    sign = (i >> 31) != 0 ? 1 : 0;
    exp =  (i >> 23) & 255;
    mant = i & ((1 << 23) - 1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Making Double Number
M_API double    mmaked(int64t sign, int64t exp, int64t mant)
{
    int64t i = (sign << 63) | ((exp & 2047) << 52) | (mant & ((int64t(1) << 52) - 1));
    double d = *(double*)&i;

    return d;
}
///////////////////////////////////////////////////////////////////////////////////////
// Splitting Double Number
M_API void      msplitd(double d, int64t &sign, int64t &exp, int64t &mant)
{
    int64t i = *(int64t*)&d;

    sign = (i >> 63) != 0 ? 1 : 0;
    exp  = (i >> 52) & 2047;
    mant = i & ((int64t(1) << 52) - 1);
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Checking If Value Is Not A Number
M_API int32t    misnan(float f)
{
    int32t s,e,m;
    msplitf(f,s,e,m);

    return (e == 255) && (m != 0);
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t    misnan(double d)
{
    int64t s,e,m;
    msplitd(d,s,e,m);

    return (e == 2047) && (m != 0);
}

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Signaling NaN
M_API int32t  msnan(float f)
{
    int32t s,e,m;
    msplitf(f,s,e,m);

    return (e == 255) && (m != 0) && (m >> 22);
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  msnan(double d)
{
    int64t s,e,m;
    msplitd(d,s,e,m);

    return (e == 2047) && (m != 0) && (m >> 51);
}

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Quiet NaN
M_API int32t  mqnan(float f)
{
    int32t s,e,m;
    msplitf(f,s,e,m);

    return (e == 255) && (m != 0) && !(m >> 22);
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  mqnan(double d)
{
    int64t s,e,m;
    msplitd(d,s,e,m);

    return (e == 2047) && (m != 0) && !(m >> 51);
}

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Infinity
M_API int32t  misinf(float f)
{
    int32t s,e,m;
    msplitf(f,s,e,m);

    return (e == 255) && (m == 0);
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  misinf(double d)
{
    int64t s,e,m;
    msplitd(d,s,e,m);

    return (e == 2047) && (m == 0);
}


///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Positive Infinity
M_API int32t  mpinf(float f)
{
    int32t s,e,m;
    msplitf(f,s,e,m);

    return (s == 0) && (e == 255) && (m == 0);
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  mpinf(double d)
{
    int64t s,e,m;
    msplitd(d,s,e,m);

    return (s == 0) && (e == 2047) && (m == 0);
}

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Negative Infinity
M_API int32t  mninf(float f)
{
    int32t s,e,m;
    msplitf(f,s,e,m);

    return (s > 0) && (e == 255) && (m == 0);
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  mninf(double d)
{
    int64t s,e,m;
    msplitd(d,s,e,m);

    return (s > 0) && (e == 2047) && (m == 0);
}


///////////////////////////////////////////////////////////////////////////////////////
// Clamp Value To [0, inf)
///////////////////////////////////////////////////////////////////////////////////////
M_API float  mclampnneg(float f)
{
    int32t i = *(int32t*)&f;
    i = i >> 31;
    i = ~i;
    
    return *(float*)& (*(int32t*)&f &= i );
}
///////////////////////////////////////////////////////////////////////////////////////
M_API double mclampnneg(double d)
{
    int64t i = *(int64t*)&d;
    i = i >> 63;
    i = ~i;

    return *(double*)&(*(int64t*)&d &= i );
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Lomont Compare Function
// Fast Function To Compare Two Floating Point Numbers
M_API boolt mlcmp(float af, float bf, int32t maxDiff)
{
    int32t ai;
    int32t bi;

    ai = *(int32t*)&af;
    bi = *(int32t*)&bf;

    int32t test = (ai^bi) >> 31;

    ASSERT((test==0) || (0xffffffff == test));

    int32t diff = (((0x80000000 - ai)&(test)) | (ai & (~test))) - bi;
    int32t v1 = maxDiff + diff;
    int32t v2 = maxDiff - diff;
    return (v1 | v2) >= 0;
}


///////////////////////////////////////////////////////////////////////////////////////
// Clamping Value
///////////////////////////////////////////////////////////////////////////////////////
M_API float   mclamp(const float &min, const float &max, const float &f)
{
    float fval = f;
    fval -= min;
    fval /= (max - min);
    fval = mclamp01(fval);
    fval *= (max - min);
    fval += min;

    ASSERT((min <= fval) && (fval <= max));

    return fval;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API double   mclamp(const double &min, const double &max, const double &d)
{
    double dval = d;
    dval -= min;
    dval /= (max - min);
    dval = mclamp01(dval);
    dval *= (max - min);
    dval += min;

    ASSERT((min <= dval) && (dval <= max));

    return dval;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Higher Power Of 2
///////////////////////////////////////////////////////////////////////////////////////
M_API int8t  mhpow2(int8t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n + 1;

    return (n>>7) ? 0 : n;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint8t mhpow2(uint8t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;

    return n + 1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int16t mhpow2(int16t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n + 1;

    return (n>>15) ? 0 : n;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint16t mhpow2(uint16t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;

    return n + 1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int32t mhpow2(int32t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;
    n = n + 1;

    return (n>>31) ? 0 : n;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint32t mhpow2(uint32t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;
    
    return n + 1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t mhpow2(int64t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;
    n = n | n>>32;
    n = n + 1;

    return (n>>63) ? 0 : n;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint64t mhpow2(uint64t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;
    n = n | n>>32;

    return n + 1;
}

///////////////////////////////////////////////////////////////////////////////////////
// Lower Power Of 2
///////////////////////////////////////////////////////////////////////////////////////
M_API int8t mlpow2(int8t n)
{
    if(n & 0x80)
        return 0;

    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;

    return (n + 1)>>1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint8t mlpow2(uint8t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;

    return (n + 1)>>1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int16t mlpow2(int16t n)
{
    if(n & 0x8000)
        return 0;

    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    
    return (n + 1)>>1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint16t mlpow2(uint16t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;

    return (n + 1)>>1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int32t mlpow2(int32t n)
{
    if(n & 0x80000000)
        return 0;
    
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;

    return (n + 1)>>1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint32t mlpow2(uint32t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;

    return (n + 1)>>1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t mlpow2(int64t n)
{
    if(n & 0x8000000000000000)
        return 0;
   
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;
    n = n | n>>32;
   
    return (n + 1)>>1;
}
///////////////////////////////////////////////////////////////////////////////////////
M_API uint64t mlpow2(uint64t n)
{
    n = n - 1;
    n = n | n>>1;
    n = n | n>>2;
    n = n | n>>4;
    n = n | n>>8;
    n = n | n>>16;
    n = n | n>>32;

    return (n + 1)>>1;
}

///////////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_PRIM_SSE
void   mpack01(float out[], const float in[], int32t size)
{
    if(size <= 0)
        return ;

    int32t endsize = size%4;
    int32t nsize = (size%4) ? (size - endsize) : (size);
    int32t i = 0;
    __m128 mhalfone = _mm_set_ps(0.5f,0.5f,0.5f,0.5f);

    if(size >= 4)
    for(i = 0; i < nsize; i+=4)
    {
        __m128 mvals = _mm_loadu_ps(&in[i]);
        __m128 melem1 = _mm_mul_ps(mvals,mhalfone);
        __m128 mres = _mm_add_ps(melem1,mhalfone);
        _mm_store_ps(&out[i],mres);
    }

    for(int32t j = 0; j < endsize; j++)
    {
        __m128 mval = _mm_set_ss(in[i+j]);
        __m128 melem2 = _mm_mul_ps(mval,mhalfone);
        __m128 mres = _mm_add_ps(melem2,mhalfone);
        _mm_store_ss(&out[i+j],mres);
    }
}
#else
void   mpack01(float out[], const float in[], int32t size)
{
    if(size <= 0)
        return ;

    for(int32t i = 0; i < size; i++)
    {
        out[i] = mpack01(in[i]);
    }

}
#endif

///////////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_PRIM_SSE
void   mpack01(float &xout, float &yout, float &zout, float xin, float yin, float zin)
{
    __m128 mhalfone = _mm_set_ps(0.5f,0.5f,0.5f,0.5f);
    __m128 mvals = _mm_set_ps(0.0f,zin,yin,xin);

    __m128 melem = _mm_mul_ps(mvals,mhalfone);
    __m128 mres = _mm_add_ps(melem,mhalfone);

    float fres[4] = {0};
    _mm_store_ps(fres,mres);

    xout = fres[0];
    yout = fres[1];
    zout = fres[2];
}
#else
void   mpack01(float &xout, float &yout, float &zout, float xin, float yin, float zin)
{
    xout = mpack01(xin);
    yout = mpack01(yin);
    zout = mpack01(zin);
}
#endif


///////////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_PRIM_SSE
void   mpack01_3(float out[], const float in[])
{
    __m128 mhalfone = _mm_set_ps(0.5f,0.5f,0.5f,0.5f);
    __m128 mvals = _mm_set_ps(0.0f,in[2],in[1],in[0]);

    __m128 melem = _mm_mul_ps(mvals,mhalfone);
    __m128 mres = _mm_add_ps(melem,mhalfone);

    float fres[4] = {0};
    _mm_store_ps(fres,mres);

    memcpy(&out[0],&fres[0],sizeof(float)*3);
}
#else
void   mpack01_3(float out[], const float in[])
{
    out[0] = mpack01(in[0]);
    out[1] = mpack01(in[1]);
    out[2] = mpack01(in[2]);
}
#endif

///////////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_PRIM_SSE
void munpack01(float out[], const float in[], int32t size)
{
    if(size <= 0)
        return ;

    int32t endsize = size%4;
    int32t nsize = (size%4) ? (size - endsize) : (size);
    int32t i = 0;
    __m128 mtwo = _mm_set_ps(2.f,2.f,2.f,2.f);
    __m128 mone = _mm_set_ps(1.f,1.f,1.f,1.f);

    if(size >= 4)
    for(i = 0; i < nsize; i+=4)
    {
        __m128 mvals = _mm_loadu_ps(&in[i]);
        __m128 melem1 = _mm_mul_ps(mvals,mtwo);
        __m128 mres = _mm_sub_ps(melem1,mone);
        _mm_store_ps(&out[i],mres);
    }

    for(int32t j = 0; j < endsize; j++)
    {
        __m128 mval = _mm_set_ss(in[i+j]);
        __m128 melem2 = _mm_mul_ps(mval,mtwo);
        __m128 mres = _mm_add_ps(melem2,mone);
        _mm_store_ss(&out[i+j],mres);
    }
}
#else
void munpack01(float out[], const float in[], int32t size)
{
    if(size <= 0)
        return ;

    for(int32t i = 0; i < size; i++)
        out[i] = munpack01(in[i]); 
}
#endif

///////////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_PRIM_SSE
void munpack01(float &xout, float &yout, float &zout, float xin, float yin, float zin)
{
    __m128 mone = _mm_set_ps(1.f,1.f,1.f,1.f);
    __m128 mtwo = _mm_set_ps(2.f,2.f,2.f,2.f);

    __m128 mvals = _mm_set_ps(0.0f,zin,yin,xin);

    __m128 melem = _mm_mul_ps(mvals,mtwo);
    __m128 mres = _mm_sub_ps(melem,mone);

    float fres[4] = {0};
    _mm_store_ps(fres,mres);

    xout = fres[0];
    yout = fres[1];
    zout = fres[2];
}
#else
void munpack01(float &xout, float &yout, float &zout, float xin, float yin, float zin)
{
    xout = munpack01(in[0]);
    yout = munpack01(in[1]);
    zout = munpack01(in[2]);
}
#endif

///////////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_PRIM_SSE
void munpack01_3(float out[], const float in[])
{
    __m128 mtwo = _mm_set_ps(2.f,2.f,2.f,2.f);
    __m128 mone = _mm_set_ps(1.f,1.f,1.f,1.f);
    __m128 mvals = _mm_set_ps(0.0f,in[2],in[1],in[0]);

    __m128 melem = _mm_mul_ps(mvals,mtwo);
    __m128 mres = _mm_sub_ps(melem,mone);

    float fres[4] = {0};
    _mm_store_ps(fres,mres);

    memcpy(&out[0],&fres[0],sizeof(float)*3);
}
#else
void munpack01_3(float out[], const float in[])
{
    out[0] = munpack01(in[0]);
    out[1] = munpack01(in[1]);
    out[2] = munpack01(in[2]);
}
#endif


///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Normalizing Angle To [-PI,PI]
///////////////////////////////////////////////////////////////////////////////////////
float   mnorma(float rad)
{
    float alpha = rad + M_PI;
    alpha = alpha * M_1_2PI;
    alpha = mfrc(alpha);

    return alpha*M_2PI - M_PI;
}
///////////////////////////////////////////////////////////////////////////////////////
double  mnorma(double rad)
{
    double alpha = rad + _PI;
    alpha = alpha * _1_2PI;
    alpha = mfrc(alpha);

    return alpha*_2PI - _PI;
}
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Finds Fraction Part Of Number x
///////////////////////////////////////////////////////////////////////////////////////
float mfrc(float x)
{ 
    float intval =  mfloor(mabs(x));
    //pfloorf(x);
    if(mless0(x))
        return intval + x;
    else
        return mabs(intval - x);
}
///////////////////////////////////////////////////////////////////////////////////////
double mfrc(double x)
{ 
    double intval = mfloor(mabs(x));

    if(mless0(x))
        return intval + x;
    else
        return mabs(intval - x);
}
///////////////////////////////////////////////////////////////////////////////////////

