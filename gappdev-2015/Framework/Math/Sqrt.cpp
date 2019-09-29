///////////////////////////////////////////////////////////////////////////////////////
//  Sqrt.cpp
//
//  Square Root Function Calculation Methods
//  
//
///////////////////////////////////////////////////////////////////////////////////////

#include "PlatformTypes.h"

#include <math.h>

#include "MathConst.h"
#include "MathDefs.h"
#include "MathPrim.h"

#include "MathLibDefs.h"

#include "Sqrt.h"



///////////////////////////////////////////////////////////////////////////////////////
// Square Root Table Values
///////////////////////////////////////////////////////////////////////////////////////
INTFLOAT        tsqrt [] = 
{
    #include "sqrt_table256.h"
};
///////////////////////////////////////////////////////////////////////////////////////
float m_tsqrt(float f)
{
    if(mless0(f))
        return 0.0f;

    INTFLOAT fi;    
    fi.f = f;
    int32t n = fi.i;
    int32t e = (n >> 1) & 0x3f800000;
    n = (n >> 16) & 0xff;
    fi.i = e + tsqrt[n].i;

    return fi.f;
}
///////////////////////////////////////////////////////////////////////////////////////
float m_rtsqrt(float f)
{
    if(mless0(f))
        return 0.0f;

    INTFLOAT fi;    
    fi.f = f;
    int32t n = fi.i;
    int32t e = (n >> 1) & 0x3f800000;
    n = (n >> 16) & 0xff;
    fi.i = e + tsqrt[n].i;

    return 1.0f/fi.f;
}
///////////////////////////////////////////////////////////////////////////////////////
float m_rfsqrt(float number)
{
    int32t i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = * ( int32t * ) &y;		            // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}
///////////////////////////////////////////////////////////////////////////////////////
float m_fsqrt(float number)
{
    int32t i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = * ( int32t * ) &y;		            
	i  = 0x5f3759df - ( i >> 1 );              
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   
	y  = y * ( threehalfs - ( x2 * y * y ) );   

	return 1.0f/y;
}
///////////////////////////////////////////////////////////////////////////////////////
float m_asqrt(float x)
{
    int32t iter = 12;

    float sum = 1.0f;
    float val = 1.0f;

    float nfact1 = 1.0f;
    float factVal1 = 1.0f;
    float factVal2 = 1.0f;
    float n = 0.0f;
    float cnstVal2 = 1.0f;


    for(int32t i = 1; i < iter; i++)
    {
        float tmp1, tmp2;
        val = val*x;
        tmp1 = (i%2)?(-1.0f):(1.0f);

        factVal1 *= nfact1;
        nfact1 += 1.0f;
        factVal1 *= nfact1;
        nfact1 += 1.0f;
        
        n += 1.0f;
        tmp2 = 1.0f - 2.0f*n;
        factVal2 *= n;
        cnstVal2 *= 4.0f;

        sum = sum + (tmp1*factVal1)/(tmp2*factVal2*factVal2*cnstVal2)*val;
    }

    return sum;
}