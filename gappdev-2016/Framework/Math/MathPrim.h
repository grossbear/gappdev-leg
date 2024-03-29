///////////////////////////////////////////////////////////////////////////////////////
//  MathPrim.h
//
//  Primary Math Functions 
//  
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _MATHPRIM_H_
#define _MATHPRIM_H_


///////////////////////////////////////////////////////////////////////////////////////
// Converting Functions From Integer To Float Type And Vice Versa
///////////////////////////////////////////////////////////////////////////////////////
M_API float     mitof(int32t i);
///////////////////////////////////////////////////////////////////////////////////////
M_API int32t    mftoi(float f);

///////////////////////////////////////////////////////////////////////////////////////
// Making Float Number
M_API float     mmakef(int32t sign, int32t exp, int32t mantissa);
///////////////////////////////////////////////////////////////////////////////////////
// Splitting Float Number
M_API void      msplitf(float f, int32t &sign, int32t &exp, int32t &mant);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Making Double Number
M_API double    mmaked(int64t sign, int64t exp, int64t mantissa);
///////////////////////////////////////////////////////////////////////////////////////
// Splitting Double Number
M_API void      msplitd(double d, int64t &sign, int64t &exp, int64t &mant);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Lomont Compare Function
// Fast Function To Compare Two Floating Point Numbers
M_API boolt mlcmp(float af, float bf, int32t maxDiff);

///////////////////////////////////////////////////////////////////////////////////////
// Checking If Value Is Not A Number
M_API int32t    misnan(float f);
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t    misnan(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Signaling NaN
M_API int32t    msnan(float f);
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t    msnan(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Quiet NaN
M_API int32t  mqnan(float f);
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  mqnan(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Infinity
M_API int32t  misinf(float f);
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  misinf(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Positive Infinity
M_API int32t  mpinf(float f);
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  mpinf(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Negative Infinity
M_API int32t  mninf(float f);
///////////////////////////////////////////////////////////////////////////////////////
M_API int64t  mninf(double d);


///////////////////////////////////////////////////////////////////////////////////////
boolt   mnear0(float f);
///////////////////////////////////////////////////////////////////////////////////////
boolt   mnear0(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Absolute Value Functions
template <class Ttype>
Ttype   mabs(Ttype i);
///////////////////////////////////////////////////////////////////////////////////////
template int8t   mabs<int8t>(int8t i);
///////////////////////////////////////////////////////////////////////////////////////
template int16t  mabs<int16t>(int16t i);
///////////////////////////////////////////////////////////////////////////////////////
template int32t  mabs<int32t>(int32t i);
///////////////////////////////////////////////////////////////////////////////////////
template int64t  mabs<int64t>(int64t i);
///////////////////////////////////////////////////////////////////////////////////////
float    mabs(float f);
///////////////////////////////////////////////////////////////////////////////////////
double   mabs(double d);


///////////////////////////////////////////////////////////////////////////////////////
// Negate Values
float   mnegate(float f);
///////////////////////////////////////////////////////////////////////////////////////
double  mnegate(double d);


///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Greater Than 0
/////////////////////////////////////////////////////////////////////////////////////// 
int32t mgre0(float f);
///////////////////////////////////////////////////////////////////////////////////////
int64t mgre0(double d);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Greater Or Equals Zero
///////////////////////////////////////////////////////////////////////////////////////
int32t  mgreq0(float f);
///////////////////////////////////////////////////////////////////////////////////////
int64t  mgreq0(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Equals Zero
///////////////////////////////////////////////////////////////////////////////////////
boolt   mis0(float f);
///////////////////////////////////////////////////////////////////////////////////////
boolt   mis0(double d);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Less Or Equals Zero
///////////////////////////////////////////////////////////////////////////////////////
int32t mlesseq0(float f);
///////////////////////////////////////////////////////////////////////////////////////
int64t mlesseq0(double d);
///////////////////////////////////////////////////////////////////////////////////////
#define mlse0 mlesseq0

///////////////////////////////////////////////////////////////////////////////////////
// Check If Value Is Less Than 0
///////////////////////////////////////////////////////////////////////////////////////
int32t  mless0(float f);
///////////////////////////////////////////////////////////////////////////////////////
int64t  mless0(double d);
///////////////////////////////////////////////////////////////////////////////////////
#define mls0 mless0


///////////////////////////////////////////////////////////////////////////////////////
// Clamp Float Value To 0
///////////////////////////////////////////////////////////////////////////////////////
float   mclamp0(float f);
///////////////////////////////////////////////////////////////////////////////////////
double  mclamp0(double d);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Clamp Value To 1
///////////////////////////////////////////////////////////////////////////////////////
float   mclamp1(float f);
///////////////////////////////////////////////////////////////////////////////////////
double  mclamp1(double d);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Clamp Value To [0,1]
///////////////////////////////////////////////////////////////////////////////////////
float   mclamp01(float f);
///////////////////////////////////////////////////////////////////////////////////////
double  mclamp01(double d);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Clamp Value To [0, inf)
///////////////////////////////////////////////////////////////////////////////////////
M_API float   mclampnneg(float f);
///////////////////////////////////////////////////////////////////////////////////////
M_API double  mclampnneg(double d);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Function Template Clamping Value
///////////////////////////////////////////////////////////////////////////////////////
template <class Ttype>
Ttype   mclamp(const Ttype &min, const Ttype &max, const Ttype &i);
///////////////////////////////////////////////////////////////////////////////////////
template int8t      mclamp<int8t>(const int8t &min, const int8t &max, const int8t &i);
///////////////////////////////////////////////////////////////////////////////////////
template uint8t     mclamp<uint8t>(const uint8t &min, const uint8t &max, const uint8t &i);
///////////////////////////////////////////////////////////////////////////////////////
template int16t     mclamp<int16t>(const int16t &min, const int16t &max, const int16t &i);
///////////////////////////////////////////////////////////////////////////////////////
template uint16t    mclamp<uint16t>(const uint16t &min, const uint16t &max, const uint16t &i);
///////////////////////////////////////////////////////////////////////////////////////
template int32t     mclamp<int32t>(const int32t &min, const int32t &max, const int32t &i);
///////////////////////////////////////////////////////////////////////////////////////
template uint32t    mclamp<uint32t>(const uint32t &min, const uint32t &max, const uint32t &i);
///////////////////////////////////////////////////////////////////////////////////////
template int64t     mclamp<int64t>(const int64t &min, const int64t &max, const int64t &i);
///////////////////////////////////////////////////////////////////////////////////////
template uint64t    mclamp<uint64t>(const uint64t &min, const uint64t &max, const uint64t &i);
///////////////////////////////////////////////////////////////////////////////////////
M_API float      mclamp(const float &min, const float &max, const float &f);
///////////////////////////////////////////////////////////////////////////////////////
M_API double     mclamp(const double &min, const double &max, const double &d);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Check If Number Is Power Of 2
///////////////////////////////////////////////////////////////////////////////////////
template <class Ttype>
boolt mispow2(Ttype n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<int8t>(int8t n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<uint8t>(uint8t n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<int16t>(int16t n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<uint16t>(uint16t n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<int32t>(int32t n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<uint32t>(uint32t n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<int64t>(int64t n);
///////////////////////////////////////////////////////////////////////////////////////
template boolt mispow2<uint64t>(uint64t n);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Higher Power Of 2
///////////////////////////////////////////////////////////////////////////////////////
int8t       mhpow2(int8t n);
uint8t      mhpow2(uint8t n);
int16t      mhpow2(int16t n);
uint16t     mhpow2(uint16t n);
int32t      mhpow2(int32t n);
uint32t     mhpow2(uint32t n);
int64t      mhpow2(int64t n);
uint64t     mhpow2(uint64t n);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Lower Power Of 2
///////////////////////////////////////////////////////////////////////////////////////
int8t       mlpow2(int8t n);
uint8t      mlpow2(uint8t n);
int16t      mlpow2(int16t n);
uint16t     mlpow2(uint16t n);
int32t      mlpow2(int32t n);
uint32t     mlpow2(uint32t n);
int64t      mlpow2(int64t n);
uint64t     mlpow2(uint64t n);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Packing Values From [-1,1] To [0,1]
///////////////////////////////////////////////////////////////////////////////////////
float   mpack01(float x);
double  mpack01(double y);

void    mpack01(float out[], const float in[], int32t size);

void    mpack01(float &xout, float &yout, float &zout, float xin, float yin, float zin);

void    mpack01_3(float out[], const float in[]);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Unpacking Values From [0,1] To [-1,1]
///////////////////////////////////////////////////////////////////////////////////////
float   munpack01(float x);
double  munpack01(double y);

void    munpack01(float out[], const float in[], int32t size);

void    munpack01(float &xout, float &yout, float &zout, float xin, float yin, float zin);

void    munpack01_3(float out[], const float in[]);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Coverting Radians To Degrees
///////////////////////////////////////////////////////////////////////////////////////
float   mtorad(float deg);
///////////////////////////////////////////////////////////////////////////////////////
double  mtorad(double deg);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Coverting Degrees To Radians
///////////////////////////////////////////////////////////////////////////////////////
float   mtodeg(float rad);
double  mtodeg(double rad);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Calculates The Floor Of A Value x
///////////////////////////////////////////////////////////////////////////////////////
float   mfloor(float x);
double  mfloor(double x);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Calculates The Ceiling Of A Value x
///////////////////////////////////////////////////////////////////////////////////////
float   mceil(float x);
double  mceil(double x);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Finds Fraction Part Of Number x
///////////////////////////////////////////////////////////////////////////////////////
float   mfrc(float x);
double  mfrc(double x);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Normalizing Angle To [-PI,PI]
///////////////////////////////////////////////////////////////////////////////////////
//float   mnorma(float rad);
//double  mnorma(double rad);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Set To Zero If Value Is Nearly Zero
///////////////////////////////////////////////////////////////////////////////////////
float   malign0(float x);
double  malign0(double x);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Set To One If Value Is Nearly One
///////////////////////////////////////////////////////////////////////////////////////
float   malign1(float x);
double  malign1(double x);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Getting Maximum Value 
///////////////////////////////////////////////////////////////////////////////////////
template <class Ttype>
Ttype   mmax(Ttype x, Ttype y);
///////////////////////////////////////////////////////////////////////////////////////
template int8t      mmax<int8t>(int8t x, int8t y);
///////////////////////////////////////////////////////////////////////////////////////
template uint8t     mmax<uint8t>(uint8t x, uint8t y);
///////////////////////////////////////////////////////////////////////////////////////
template int16t     mmax<int16t>(int16t x, int16t);
///////////////////////////////////////////////////////////////////////////////////////
template uint16t    mmax<uint16t>(uint16t x, uint16t);
///////////////////////////////////////////////////////////////////////////////////////
template int32t     mmax<int32t>(int32t x, int32t y);
///////////////////////////////////////////////////////////////////////////////////////
template uint32t    mmax<uint32t>(uint32t x, uint32t y);
///////////////////////////////////////////////////////////////////////////////////////
template int64t     mmax<int64t>(int64t x, int64t y);
///////////////////////////////////////////////////////////////////////////////////////
template uint64t    mmax<uint64t>(uint64t x, uint64t y);
///////////////////////////////////////////////////////////////////////////////////////
template float      mmax<float>(float x, float y);
///////////////////////////////////////////////////////////////////////////////////////
template double     mmax<double>(double x, double y);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Minimum Value 
///////////////////////////////////////////////////////////////////////////////////////
template <class Ttype>
Ttype   mmin(Ttype x, Ttype y);
///////////////////////////////////////////////////////////////////////////////////////
template int8t      mmin<int8t>(int8t x, int8t y);
///////////////////////////////////////////////////////////////////////////////////////
template uint8t     mmin<uint8t>(uint8t x, uint8t y);
///////////////////////////////////////////////////////////////////////////////////////
template int16t     mmin<int16t>(int16t x, int16t y);
///////////////////////////////////////////////////////////////////////////////////////
template uint16t    mmin<uint16t>(uint16t x, uint16t y);
///////////////////////////////////////////////////////////////////////////////////////
template int32t     mmin<int32t>(int32t x, int32t y);
///////////////////////////////////////////////////////////////////////////////////////
template uint32t    mmin<uint32t>(uint32t x, uint32t y);
///////////////////////////////////////////////////////////////////////////////////////
template int64t     mmin<int64t>(int64t x, int64t y);
///////////////////////////////////////////////////////////////////////////////////////
template uint64t    mmin<uint64t>(uint64t x, uint64t y);
///////////////////////////////////////////////////////////////////////////////////////
template float      mmin<float>(float x,float y);
///////////////////////////////////////////////////////////////////////////////////////
template double     mmin<double>(double x, double y);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Sgn
///////////////////////////////////////////////////////////////////////////////////////
template <class Ttype>
Ttype msgn(Ttype x);
///////////////////////////////////////////////////////////////////////////////////////
template int8t msgn<int8t>(int8t x);
///////////////////////////////////////////////////////////////////////////////////////
template int16t msgn<int16t>(int16t x);
///////////////////////////////////////////////////////////////////////////////////////
template int32t msgn<int32t>(int32t x);
///////////////////////////////////////////////////////////////////////////////////////
template int64t msgn<int64t>(int64t x);
///////////////////////////////////////////////////////////////////////////////////////
float msgn(float x);
///////////////////////////////////////////////////////////////////////////////////////
double msgn(double x);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Natural Logarithm (Base e) Of x
///////////////////////////////////////////////////////////////////////////////////////
float   mln(float x);
double  mln(double x);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Logarithm Base 10 Of X
///////////////////////////////////////////////////////////////////////////////////////
float   mlog10(float x);
double  mlog10(double x);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Exponent
///////////////////////////////////////////////////////////////////////////////////////
float   mexp(float x);
double  mexp(double x);
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// Calculate x Raised To The Power y
///////////////////////////////////////////////////////////////////////////////////////
float   mpow(float x, float y);
double  mpow(double x, double y);
///////////////////////////////////////////////////////////////////////////////////////
float   mpow(float x, int32t y);
double  mpow(double x, int32t y);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Inverting Number 
///////////////////////////////////////////////////////////////////////////////////////
float   minvert(float x);
double  minvert(double x);

///////////////////////////////////////////////////////////////////////////////////////
// Splits A Floating-Point Value Into Fractional And Integer Parts
///////////////////////////////////////////////////////////////////////////////////////
float   mmodf(float x, float *y);
double  mmodf(double x, double *y);

///////////////////////////////////////////////////////////////////////////////////////
// Calculates The Floating-Point Remainder
///////////////////////////////////////////////////////////////////////////////////////
float   mfmod(float x, float y);
double  mfmodf(double x, double y);

///////////////////////////////////////////////////////////////////////////////////////
// Float Const Multiplication Functions
///////////////////////////////////////////////////////////////////////////////////////
float   mmultpi(float x);
float   mmult2pi(float x);
float   mmultpi2(float x);
float   mmultpi4(float x);

///////////////////////////////////////////////////////////////////////////////////////
// Double Const Multiplication Functions
///////////////////////////////////////////////////////////////////////////////////////
double  mmultpi(double x);
double  mmult2pi(double x);
double  mmultpi2(double x);
double  mmultpi4(double x);

//float (*pfun1)(float);

#include "MathPrim.inl"

#endif //_MATHPRIM_H_
