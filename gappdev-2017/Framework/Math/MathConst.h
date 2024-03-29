///////////////////////////////////////////////////////////////////////////////////////
//  MathConst.h
//
//  Math Constants
//  
//
///////////////////////////////////////////////////////////////////////////////////////

// Float Constants
#define M_PI        3.1415926535897932384626433832795f      // - pi
#define M_2PI       6.283185307179586476925286766559f       // - 2pi
#define MDEG2RAD    0.017453292519943295769236907684886f    // - pi/180
#define MRAD2DEG    57.295779513082320876798154814114f      // - 180/pi (1 rad)
#define M_PI_2      1.5707963267948966192313216916398f      // - pi/2
#define M_PI_4      0.785398163397448309615660845811988f    // - pi/4
#define M_1_PI      0.318309886183790671537767526745f       // - 1/pi
#define M_2_PI      0.63661977236758134307553505349006f     // - 2/pi
#define M_1_2PI     0.15915494309189533576888376337251f     // - 1/2pi

#define M_SQRT_2    1.4142135623730950488016887242097f      // - sqrt(2)
#define M_SQRT_1_2  0.7071067811865475244008443621048f      // - 1/sqrt(2)
#define M_2_SQRTPI  1.1283791670955125738961589031215f      // - 2/sqrt(pi)

#define M_E         2.7182818284590452353602874713527f      // - e
#define M_LOG2E     1.4426950408889634073599246810019f      // - log2(e)
#define M_LOG10E    0.43429448190325182765112891891f        // - log10(e)
#define M_LN2       0.69314718055994530941723212145818f     // - ln(2)
#define M_LN10      2.3025850929940456840179914546844f      // - ln(10)
#define M_PHI       1.6180339887498948482045888343656f      // - phi (Golden Number)  

// Double Const
#define _PI        3.1415926535897932384626433832795      // - pi
#define _2PI       6.283185307179586476925286766559       // - 2pi
#define DEG2RAD    0.017453292519943295769236907684886    // - pi/180
#define RAD2DEG    57.295779513082320876798154814114      // - 180/pi (1 rad)
#define _PI_2      1.5707963267948966192313216916398      // - pi/2
#define _PI_4      0.785398163397448309615660845811988    // - pi/4
#define _1_PI      0.318309886183790671537767526745       // - 1/pi
#define _2_PI      0.63661977236758134307553505349006     // - 2/pi
#define _1_2PI     0.15915494309189533576888376337251f    // - 1/2pi

#define _SQRT_2    1.4142135623730950488016887242097      // - sqrt(2)
#define _SQRT_1_2  0.7071067811865475244008443621048      // - 1/sqrt(2)
#define _2_SQRTPI  1.1283791670955125738961589031215      // - 2/sqrt(pi)

#define _E         2.7182818284590452353602874713527      // - e
#define _LOG2E     1.4426950408889634073599246810019      // - log2(e)
#define _LOG10E    0.43429448190325182765112891891        // - log10(e)
#define _LN2       0.69314718055994530941723212145818     // - ln(2)
#define _LN10      2.3025850929940456840179914546844      // - ln(10)
#define _PHI       1.6180339887498948482045888343656      // - phi (Golden Number)  


#ifdef _WIN32
#define M_INT64_MAX 0x7fffffffffffffff
#define M_INT64_MIN 0x8000000000000000
#else
#define M_INT64_MAX 0x7fffffffffffffffLL
#define M_INT64_MIN 0x8000000000000000LL
#endif

#define M_INT_MAX 0x7fffffff
#define M_INT_MIN 0x80000000

#define M_DOUBLE_MAX     1.7976931348623158e+308
#define M_FLOAT_MAX      3.402823466e+38F

#define FLOAT_EPS   1e-6
#define DOUBLE_EPS  1e-12