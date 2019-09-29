///////////////////////////////////////////////////////////////////////////////////////
//  Trigonometry.cpp
//
//  Trigonometrical Functions
//  
//
///////////////////////////////////////////////////////////////////////////////////////

#include "PlatformTypes.h"

#include <math.h>

#include "MathConst.h"
#include "MathDefs.h"

#include "MathLibDefs.h"

#include "Trigonometry.h"


///////////////////////////////////////////////////////////////////////////////////////
// Triginometry Functions
///////////////////////////////////////////////////////////////////////////////////////

#define     TABLESIZE_SIN   256
#define     TWOPISCALE      ((float)TABLESIZE_SIN * (M_1_2PI))

INTFLOAT tsin [] = 
{
    #include "sin_table256.h"
};

///////////////////////////////////////////////////////////////////////////////////////
// Sinus Function Using Precalculated Table
float m_tsinf(float angle)
{
    ASSERT(angle >= -10000.0f && angle <= 10000.0f);

    INTFLOAT fi;    
    fi.f = angle * TWOPISCALE + bias.f;
    int i = fi.i & (TABLESIZE_SIN - 1);
    return tsin[i].f;
}

///////////////////////////////////////////////////////////////////////////////////////
// Cosinus Function Using Precalculated Table
float m_tcosf(float angle)
{
    ASSERT(angle >= -10000.0f && angle <= 10000.0f);

    INTFLOAT  fi;
    fi.f = angle * TWOPISCALE + bias.f;
    int i = (fi.i + (TABLESIZE_SIN/4)) & (TABLESIZE_SIN - 1);
    return tsin[i].f;
}

///////////////////////////////////////////////////////////////////////////////////////
// SinCos Function Using Precalculated Table
void m_tsincosf(float angle, float &SinVal, float &CosVal)
{
    ASSERT(angle >= -10000.0f && angle <= 10000.0f);

    INTFLOAT fi;    
    fi.f = angle * TWOPISCALE + bias.f;

    int i = fi.i & (TABLESIZE_SIN - 1);
    SinVal = tsin[i].f;

    i = (fi.i + (TABLESIZE_SIN/4)) & (TABLESIZE_SIN - 1);
    CosVal = tsin[i].f;

    return ;
}