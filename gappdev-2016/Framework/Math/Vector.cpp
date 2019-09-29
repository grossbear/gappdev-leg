///////////////////////////////////////////////////////////////////////////////////////
//  Vector.cpp
//
//  Vector Classes Templates Methods Definitions
//
///////////////////////////////////////////////////////////////////////////////////////
#include "PlatformTypes.h"

#include <math.h>
#include <memory.h>

#include "MathConst.h"
#include "MathDefs.h"

#include "MathLibDefs.h"

#include "MathPrim.h"
#include "Sqrt.h"
#include "Trigonometry.h"

#include "PrimFunc.h"

#include "Vector.h"

#include "Algebra.h"

////////////////////////////////////////////////////////////////////////////
// Rotate 2D Vector 
template <class T>
void CMVec2Rotate(CMVector2D<T> &vOut, const CMVector2D<T> &vIn, T rad)
{
	T cosA;
	T sinA;

	// Calculating Sinus And Cosinus Angle Values
    MVEC2SINCOS(rad, sinA, cosA);

    T x = vIn.x*cosA + vIn.y*sinA;
    T y = -vIn.x*sinA + vIn.x*cosA;

    vOut.x = x;
	vOut.y = y;

	return ;
}


////////////////////////////////////////////////////////////////////////////
// Returns The Length Of A 2D Vector
template <class T>
T CMVec2Length(const CMVector2D<T> &v)
{
    return MVEC2SQRT(v.x, v.y);
}

////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of A 2D Vector 
template <class T>
T CMVec2LengthSq(const CMVector2D<T> &v)
{
	return v.x * v.x + v.y * v.y;
}

////////////////////////////////////////////////////////////////////////////
// Normalizing 2D Vector
template <class T>
void CMVec2Normalize(CMVector2D<T> &vOut, const CMVector2D<T> &vIn)
{
	T revSq = MVEC2REVSQRT(vIn.x, vIn.y); 

	vOut.x *= revSq;
	vOut.y *= revSq;

	return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Length Between Two 2D Points
template <class T>
T CMVec2Distance(const CMVector2D<T> &p0, const CMVector2D<T> &p1)
{
    CMVector2D<T> vec;
    CMVec2Sub(vec,p1,p0);

    return MVEC2SQRT(vec.x,vec.y);
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Square Length Between Two 2D Points
template <class T>
T CMVec2DistanceSq(const CMVector2D<T> &p0, const CMVector2D<T> &p1)
{
    CMVector2D<T> vec;
    CMVec2Sub(vec,p1,p0);

    return vec.x*vec.x + vec.y*vec.y;
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Angle In Radians Between Two 2D Vectors
template <class T>
T CMVec2Angle(const CMVector2D<T> &v1, const CMVector2D<T> &v2)
{
    T len1 = CMVec2Length(v1);
    T len2 = CMVec2Length(v2);

    ASSERT(!mnear0(len1));
    ASSERT(!mnear0(len2));
    ASSERT(!mnear0(len1*len2));

    return MVEC2ACOS(CMVec2Dot(v1,v2)/(len1*len2));
}


///////////////////////////////////////////////////////////////////////////////////////
// Getting Reflected 2D Vector
template <class T>
void CMVec2Reflect(CMVector2D<T> &vR, const CMVector2D<T> &vL, const CMVector2D<T> &vN)
{
    //R = 2*N*(NoL) - L

    T dot = CMVec2Dot(vN,vL);
    T two = T(2);

    CMVector2D<T> res(vN);
    res *= two;
    res *= dot;

    CMVec2Sub(vR,res,vL);
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Refracted 2D Vector
template <class T>
void CMVec2Refract(CMVector2D<T> &vR, const CMVector2D<T> &vL, const CMVector2D<T> &vN, T etaRatio)
{
    T one = T(1);

    T cosI = CMVec2Dot(vL,vN);
    T cosT2 = one - etaRatio*etaRatio*(one - cosI*cosI);

    CMVector2D<T> T1 = vL * (-etaRatio);
    CMVector2D<T> T2 = vN * (etaRatio * cosI - MVEC2SQRT(mabs(cosT2)));

    CMVector2D<T> Res;
    CMVec2Add(Res,T1,T2);

    vR = Res*(T)mgre0(cosT2);
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Square Interpolation, Using Two 2D Vectors
template <class T>
void CMVec2Sqrp(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, T factor)
{
    T f2 = factor*factor;

    T x = v1.x + ((v2.x - v1.x)*f2);
    T y = v1.y + ((v2.y - v1.y)*f2);

    vOut.x = x;
    vOut.y = y;

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cosinus Interpolation, Using Two 2D Vectors
template <class T>
void CMVec2Cosrp(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, T factor)
{
    T ft = mmultpi(factor);
    T f = (T(1) - MVEC2COS(ft))*T(0.5);
    
    T x = v1.x + ((v2.x - v1.x)*ft);
    T y = v1.y + ((v2.y - v1.y)*ft);

    vOut.x = x;
    vOut.y = y;

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Point In Barycentric Coordinates, Using The Specified 2D Vectors
template <class T>
void CMVec2BaryCentric(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2,
                       const CMVector2D<T> &v3, T f, T g)
{
    vOut.x = (T(1)-f-g) * (v1.x) + f * (v2.x) + g * (v3.x);
    vOut.y = (T(1)-f-g) * (v1.y) + f * (v2.y) + g * (v3.y);

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Quadratic Interpolation, Using The Specified 2D Vectors
template <class T>
void CMVec2Quadratic(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2,
                     const CMVector2D<T> &v3, T factor)
{
    T f2 = factor*factor;
    T val = T(1) - factor;
    T val2 = val*val;
    T facval = factor*val*T(2);

    //pOut = (*pV1)*val*val + ((*pV2)*factor*val)*T(2) + (*pV3)*f2; 
    vOut.x = v1.x*val2 + v2.x*facval + v3.x*f2;
    vOut.y = v1.y*val2 + v2.y*facval + v3.y*f2;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cubic Interpolation, Using The Specified 2D Vectors
template <class T>
void CMVec2Cubic(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2,
                 const CMVector2D<T> &v3, const CMVector2D<T> &v4, T factor)
{
    T x2 = factor*factor;
    T f = factor;

    CMVector2D<T> Vec12(v1 - v2);
    CMVector2D<T> P((v4 - v3) - Vec12);
    CMVector2D<T> Q(Vec12 - P);
    CMVector2D<T> R(v3 - v1);

    vOut = P*x2*f + Q*x2 + R*f + v2;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Catmull-Rom Spline Interpolation, Using The Specified 2D Vectors
// Calculate Value Between Points V2 And V3
template <class T>
void CMVec2CatmullRom(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, 
                      const CMVector2D<T> &v3, const CMVector2D<T> &v4, T x)
{
    CMVector2D<T> C1( v2 * T(2));
    CMVector2D<T> C2( (v1 * T(-1)) + v3);
    CMVector2D<T> C3( (v1 * T(2)) + (v2 * T(-5)) + (v3 * T(4)) + (v4 * T(-1)) );
    CMVector2D<T> C4( (v1 * T(-1)) + (v2 * T(3)) + (v3 * T(-3)) + (v4) );

    vOut = ( (((C4*x + C3)*x + C2)*x + C1) ) * T(0.5);

    return ;
}


///////////////////////////////////////////////////////////////////////////////////////
// Performs A Hermite Spline Interpolation, Using The Specified 2D Vectors
//
// Hermite Interpolate
// Calculate Value Between Points V2 And V3
// Tension: 1 Is High, 0 Normal, -1 Is Low
// Bias: 0 Is Even,
//         Positive Is Towards First Segment,
//         Negative Towards The Other
template <class T>
void CMVec2Hermite(CMVector2D<T> &vOut, const CMVector2D<T> &v1,
                   const CMVector2D<T> &v2, const CMVector2D<T> &v3,
                   const CMVector2D<T> &v4, T x, T tension, T bias)
{
    T x2 = x*x;
    T x3 = x2*x;

    CMVector2D<T>   m1 = (v2 - v1)*(T(1) + bias)*(T(1) - tension)*T(0.5);
                    m1 += (v3 - v2)*(T(1) - bias)*(T(1) - tension)*T(0.5);
    CMVector2D<T>   m2 = (v3 - v2)*(T(1) + bias)*(T(1) - tension)*T(0.5);
                    m2 += (v4 - v3)*(T(1) - bias)*(T(1) - tension)*T(0.5);

    T a1 = x3*T(2) - x2*T(3) + T(1);
    T a2 = x3 - x2*T(2) + x;
    T a3 = x3 - x2;
    T a4 = x3*T(-2) + x2*T(3);

    vOut = v2*a1 + m1*a2 + m2*a3 + v3*a4;
}


///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Vector3D Methods

////////////////////////////////////////////////////////////////////////////
// Rotate A 3D Vector In X Axis, Using The Specified Angle In Radians
template <class T>
void CMVec3RotateX(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T rad)
{
	T cosA;
	T sinA;

	// Calculating Sinus And Cosinus Angle Values
    MVEC3SINCOS(rad, sinA, cosA);

    T y = vIn.y*cosA - vIn.z*sinA;//y*cosAngle - z*sinAngle
    T z = vIn.y*sinA + vIn.z*cosA;//y*sinAngle + z*cosAngle

    vOut.x = vIn.x;
	vOut.y = y;
    vOut.z = z;
}


///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 3D Vector In Y Axis, Using The Specified Angle In Radians
template <class T>
void CMVec3RotateY(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T rad)
{
	T cosA;
	T sinA;

	// Calculating Sinus And Cosinus Angle Values
    MVEC3SINCOS(rad, sinA, cosA);

    T x = vIn.x*cosA + vIn.z*sinA;
    T z = -vIn.x*sinA + vIn.z*cosA;

    vOut.x = x;
	vOut.y = vIn.y;
    vOut.z = z;
}


///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 3D Vector In Z Axis, Using The Specified Angle In Radians
template <class T>
void CMVec3RotateZ(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T rad)
{
	T cosA;
	T sinA;

	// Calculating Sinus And Cosinus Angle Values
    MVEC3SINCOS(rad, sinA, cosA);

    T x = vIn.x*cosA - vIn.y*sinA;
    T y = vIn.x*sinA + vIn.y*cosA;

    vOut.x = x;
	vOut.y = y;
    vOut.z = vIn.z;
}


///////////////////////////////////////////////////////////////////////////////////////
// Returns The Length Of A 3D Vector
template <class T>
T CMVec3Length(const CMVector3D<T> &v)
{
    return MVEC3SQRT(v.x, v.y, v.z);
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of A 3D Vector
template <class T>
T CMVec3LengthSq(const CMVector3D<T> &v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Length Between Two 3D Points
template <class T>
T CMVec3Distance(const CMVector3D<T> &p0, const CMVector3D<T> &p1)
{
    CMVector3D<T> vec;
    CMVec3Sub(vec,p1,p0);

    return MVEC3SQRT(vec.x,vec.y,vec.z);
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Square Length Between Two 3D Points
template <class T>
T CMVec3DistanceSq(const CMVector3D<T> &p0, const CMVector3D<T> &p1)
{
    CMVector3D<T> vec;
    CMVec3Sub(vec,p1,p0);

    return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Normalize A 3D Vector
template <class T>
void CMVec3Normalize(CMVector3D<T> &vOut, const CMVector3D<T> &vIn)
{
    T revsq = MVEC3REVSQRT(vIn.x, vIn.y, vIn.z);

    vOut.x *= revsq;
    vOut.y *= revsq;
    vOut.z *= revsq;

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Angle In Radians Between Two 3D Vectors
template <class T>
T CMVec3Angle(const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
    T len1 = CMVec3Length(v1);
    T len2 = CMVec3Length(v2);

    ASSERT(!mnear0(len1));
    ASSERT(!mnear0(len2));
    ASSERT(!mnear0(len1*len2));

    return MVEC3ACOS(CMVec3Dot(v1,v2)/(len1*len2));
}


///////////////////////////////////////////////////////////////////////////////////////
// Getting Reflected 3D Vector
template <class T>
void CMVec3Reflect(CMVector3D<T> &vR, const CMVector3D<T> &vL, const CMVector3D<T> &vN)
{
    //R = 2*N*(NoL) - L

    T dot = CMVec3Dot(vN,vL);
    T two = T(2);

    CMVector3D<T> res = vN;
    res *= two;
    res *= dot;

    CMVec3Sub(vR,res,vL);
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Refracted 3D Vector
template <class T>
void CMVec3Refract(CMVector3D<T> &vR, const CMVector3D<T> &vL, const CMVector3D<T> &vN, T etaRatio)
{
    T one = T(1);

    T cosI = CMVec3Dot(vL,vN);
    T cosT2 = one - etaRatio*etaRatio*(one - cosI*cosI);

    CMVector3D<T> T1 = vL * (-etaRatio);
    CMVector3D<T> T2 = vN * (etaRatio * cosI - MVEC3SQRT(mabs(cosT2)));

    CMVector3D<T> Res;
    CMVec3Add(Res,T1,T2);

    vR = Res*(T)mgre0(cosT2);
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Catmull-Rom Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3CatmullRom(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, 
                      const CMVector3D<T> &v3, const CMVector3D<T> &v4,  T f)
{
    CMVector3D<T> C1( v2 * T(2));
    CMVector3D<T> C2( (v1 * T(-1)) + v3);
    CMVector3D<T> C3( (v1 * T(2)) + (v2 * T(-5)) + (v3 * T(4)) + (v4 * T(-1)) );
    CMVector3D<T> C4( (v1 * T(-1)) + (v2 * T(3)) + (v3 * T(-3)) + (v4) );

    vOut = ( (((C4*f + C3)*f + C2)*f + C1) ) * T(0.5);
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns A Point In Barycentric Coordinates, Using The Specified 3D Vectors
template <class T>
void CMVec3BaryCentric(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2,
                       const CMVector3D<T> &v3, T f, T g)
{
    T one = T(1);
    vOut.x = (one-f-g) * (v1.x) + f * (v2.x) + g * (v3.x);
    vOut.y = (one-f-g) * (v1.y) + f * (v2.y) + g * (v3.y);
    vOut.z = (one-f-g) * (v1.z) + f * (v2.z) + g * (v3.z);

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Hermite Spline Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3Hermite(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, 
                   const CMVector3D<T> &v3, const CMVector3D<T> &v4, T x, T tension, T bias)
{
    T x2 = x*x;
    T x3 = x2*x;
    //T one = T(1);
    //T halfone = T(0.5);

    CMVector3D<T>   m1 = (v2 - v1)*(T(1) + bias)*(T(1) - tension)*T(0.5);
                    m1 += (v3 - v2)*(T(1) - bias)*(T(1) - tension)*T(0.5);
    CMVector3D<T>   m2 = (v3 - v2)*(T(1) + bias)*(T(1) - tension)*T(0.5);
                    m2 += (v4 - v3)*(T(1) - bias)*(T(1) - tension)*T(0.5);

    T a1 = x3*T(2) - x2*T(3) + T(1);
    T a2 = x3 - x2*T(2) + x;
    T a3 = x3 - x2;
    T a4 = x3*T(-2) + x2*T(3);

    vOut = v2*a1 + m1*a2 + m2*a3 + v3*a4;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Quadratic Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3Quadratic(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2,
                     const CMVector3D<T> &v3, T factor)
{
    T f2 = factor*factor;
    T val = T(1) - factor;
    T val2 = val*val;
    T facval = factor*val*T(2);

    
    vOut.x = v1.x*val2 + v2.x*facval + v3.x*f2;
    vOut.y = v1.y*val2 + v2.y*facval + v3.y*f2;
    vOut.z = v1.z*val2 + v2.y*facval + v3.z*f2;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cubic Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3Cubic(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2,
                 const CMVector3D<T> &v3, const CMVector3D<T> &v4, T factor)
{
    T x2 = factor*factor;
    T f = factor;

    CMVector3D<T> Vec12(v1 - v2);
    CMVector3D<T> P((v4 - v3) - Vec12);
    CMVector3D<T> Q(Vec12 - P);
    CMVector3D<T> R(v3 - v1);

    vOut = P*x2*f + Q*x2 + R*f + v2;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Square Interpolation, Using Two 3D Vectors
template <class T>
void CMVec3Sqrp(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, T factor)
{
    T f2 = factor*factor;

    T x = v1.x + ((v2.x - v1.x)*f2);
    T y = v1.y + ((v2.y - v1.y)*f2);
    T z = v1.z + ((v2.z - v1.z)*f2);

    vOut.x = x;
    vOut.y = y;
    vOut.z = z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cosinus Interpolation, Using Two 3D Vectors
template <class T>
void CMVec3Cosrp(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, T factor)
{   
    T ft = mmultpi(factor);
    T f = (T(1) - MVEC3COS(ft))*T(0.5);
    
    T x = v1.x + ((v2.x - v1.x)*ft);
    T y = v1.y + ((v2.y - v1.y)*ft);
    T z = v1.z + ((v2.z - v1.z)*ft);

    vOut.x = x;
    vOut.y = y;
    vOut.z = z;
}


////////////////////////////////////////////////////////////////////////////
// 4D Vector

// DO POPRAKI - Cross Product in 4 dimentions 
///////////////////////////////////////////////////////////////////////////////////////
// Cross Product Of Two 4D Vectors
template <class T>
void CMVec4Cross(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
    vOut.x = v1.y*v2.z - v1.z*v2.y;
    vOut.y = v1.z*v2.x - v1.x*v2.z;
    vOut.z = v1.x*v2.y - v1.y*v2.x;
}

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 4D Vector In X Axis, Using The Specified Angle In Radians
template <class T>
void CMVec4RotateX(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T rad)
{
	T cosA;
	T sinA;

	// Calculating Sinus And Cosinus Angle Values
    MVEC4SINCOS(rad, sinA, cosA);

    T y = vIn.y*cosA - vIn.z*sinA;//y*cosAngle - z*sinAngle
    T z = vIn.y*sinA + vIn.z*cosA;//y*sinAngle + z*cosAngle

    vOut.x = vIn.x;
	vOut.y = y;
    vOut.z = z;
    vOut.w = vIn.w;
}


///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 4D Vector In Y Axis, Using The Specified Angle In Radians
template <class T>
void CMVec4RotateY(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T rad)
{
	T cosA;
	T sinA;

	// Calculating Sinus And Cosinus Angle Values
    MVEC4SINCOS(rad, sinA, cosA);

    T x = vIn.x*cosA + vIn.z*sinA;//x*cosAngle + z*sinAngle
    T z = -vIn.x*sinA + vIn.z*cosA;//-x*sinAngle + z*cosAngle

    vOut.x = x;
	vOut.y = vIn.y;
    vOut.z = z;
    vOut.w = vIn.w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 4D Vector In Z Axis, Using The Specified Angle In Radians
template <class T>
void CMVec4RotateZ(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T rad)
{
	T cosA;
	T sinA;

	// Calculating Sinus And Cosinus Angle Values
    MVEC4SINCOS(rad, sinA, cosA);

    T x = vIn.x*cosA - vIn.y*sinA;//x*cosAngle - y*sinAngle
    T y = vIn.x*sinA + vIn.y*cosA;//x*sinAngle + y*cosAngle

    vOut.x = x;
	vOut.y = y;
    vOut.z = vIn.z;
    vOut.w = vIn.w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Length Of A 4D Vector
template <class T>
T CMVec4Length(const CMVector4D<T> &v)
{
    return MVEC4SQRT(v.x, v.y, v.z, v.w);
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of A 4D Vector
template <class T>
T CMVec4LengthSq(const CMVector4D<T> &v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Length Between Two 4D Points
template <class T>
T CMVec4Distance(CMVector4D<T> &p0, CMVector4D<T> &p1)
{
    CMVector4D<T> vec;
    CMVec4Sub(vec,p1,p0);

    return MVEC4SQRT(vec.x,vec.y,vec.z,vec.w);
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Square Length Between Two 4D Points
template <class T>
T CMVec4DistanceSq(CMVector4D<T> &p0, CMVector4D<T> &p1)
{
    CMVector4D<T> vec;
    CMVec4Sub(vec,p1,p0);

    return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Normalize A 4D Vector
template <class T>
void CMVec4Normalize(CMVector4D<T> &vOut, const CMVector4D<T> &vIn)
{
    T revsq = MVEC4REVSQRT(vIn.x, vIn.y, vIn.z, vIn.w);

    vOut.x *= revsq;
    vOut.y *= revsq;
    vOut.z *= revsq;
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Angle In Radians Between Two 4D Vectors
template <class T>
T CMVec4Angle(const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
    T len1 = CMVec4Length(v1);
    T len2 = CMVec4Length(v2);

    ASSERT(!mnear0(len1));
    ASSERT(!mnear0(len2));
    ASSERT(!mnear0(len1*len2));

    return macos(CMVec4Dot(v1,v2)/(len1*len2));
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Angle In Degrees Between Two 4D Vectors
template <class T>
T CMVec4AngleDeg(const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
    T len1 = CMVec4Length(v1);
    T len2 = CMVec4Length(v2);

    ASSERT(!mnear0(len1));
    ASSERT(!mnear0(len2));
    ASSERT(!mnear0(len1*len2));

    return mtodeg(MVEC4ACOS(CMVec4Dot(v1,v2)/(len1*len2)));
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Reflected 4D Vector
template <class T>
void CMVec4Reflect(CMVector4D<T> &vR, const CMVector4D<T> &vL, const CMVector4D<T> &vN)
{
    //R = 2*N*(NoL) - L

    T dot = CMVec4Dot(vN,vL);
    T two = T(2);

    CMVector4D<T> res = vN;
    res *= two;
    res *= dot;

    CMVec4Sub(vR,res,vL);
}

///////////////////////////////////////////////////////////////////////////////////////
// Getting Refracted 4D Vector
template <class T>
void CMVec4Refract(CMVector4D<T> &vR, const CMVector4D<T> &vL, const CMVector4D<T> &vN, T etaRatio)
{
    T one = T(1);

    T cosI = CMVec4Dot(vL,vN);
    T cosT2 = one - etaRatio*etaRatio*(one - cosI*cosI);

    CMVector4D<T> T1 = vL * (-etaRatio);
    CMVector4D<T> T2 = vN * (etaRatio * cosI - MVEC4SQRT(mabs(cosT2)));

    CMVector4D<T> Res;
    CMVec4Add(Res,T1,T2);

    vR = Res*(T)mgre0(cosT2);
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Catmull-Rom Interpolation, Using The Specified 4D Vectors
template <class T>
void CMVec4CatmullRom(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, 
                      const CMVector4D<T> &v3, const CMVector4D<T> &v4,  T f)
{
    CMVector4D<T> C1( v2 * T(2));
    CMVector4D<T> C2( (v1 * T(-1)) + v3);
    CMVector4D<T> C3( (v1 * T(2)) + (v2 * T(-5)) + (v3 * T(4)) + (v4 * T(-1)) );
    CMVector4D<T> C4( (v1 * T(-1)) + (v2 * T(3)) + (v3 * T(-3)) + (v4) );

    vOut = ( (((C4*f + C3)*f + C2)*f + C1) ) * T(0.5);
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns A Point In Barycentric Coordinates, Using The Specified 4D Vectors
template <class T>
void CMVec3BaryCentric(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2,
                       const CMVector4D<T> &v3, T f, T g)
{
    vOut.x = (T(1)-f-g) * (v1.x) + f * (v2.x) + g * (v3.x);
    vOut.y = (T(1)-f-g) * (v1.y) + f * (v2.y) + g * (v3.y);
    vOut.z = (T(1)-f-g) * (v1.z) + f * (v2.z) + g * (v3.z);
    vOut.w = (T(1)-f-g) * (v1.w) + f * (v2.w) + g * (v3.w);
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Hermite Spline Interpolation, Using The Specified 4D Vectors
template <class T>
void CMVec4Hermite(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, 
                   const CMVector4D<T> &v3, const CMVector4D<T> &v4, T x, T tension, T bias)
{
    T x2 = x*x;
    T x3 = x2*x;

    CMVector4D<T>   m1 = (v2 - v1)*(T(1) + bias)*(T(1) - tension)*T(0.5);
                    m1 += (v3 - v2)*(T(1) - bias)*(T(1) - tension)*T(0.5);
    CMVector4D<T>   m2 = (v3 - v2)*(T(1) + bias)*(T(1) - tension)*T(0.5);
                    m2 += (v4 - v3)*(T(1) - bias)*(T(1) - tension)*T(0.5);

    T a1 = x3*T(2) - x2*T(3) + T(1);
    T a2 = x3 - x2*T(2) + x;
    T a3 = x3 - x2;
    T a4 = x3*T(-2) + x2*T(3);

    vOut = v2*a1 + m1*a2 + m2*a3 + v3*a4;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Quadratic Interpolation, Using The Specified 4D Vectors
template <class T>
void CMVec4Quadratic(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2,
                     const CMVector4D<T> &v3, T factor)
{
    T f2 = factor*factor;
    T val = T(1) - factor;
    T val2 = val*val;
    T facval = factor*val*T(2);

    vOut.x = v1.x*val2 + v2.x*facval + v3.x*f2;
    vOut.y = v1.y*val2 + v2.y*facval + v3.y*f2;
    vOut.z = v1.z*val2 + v2.y*facval + v3.z*f2;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cubic Interpolation, Using The Specified 4D Vectors
template <class T>
void CMVec4Cubic(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2,
                 const CMVector4D<T> &v3, const CMVector4D<T> &v4, T factor)
{
    T x2 = factor*factor;
    T f = factor;

    CMVector4D<T> Vec12(v1 - v2);
    CMVector4D<T> P((v4 - v3) - Vec12);
    CMVector4D<T> Q(Vec12 - P);
    CMVector4D<T> R(v3 - v1);

    vOut = P*x2*f + Q*x2 + R*f + v2;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Square Interpolation, Using Two 4D Vectors
template <class T>
void CMVec4Sqrp(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, T factor)
{
    T f2 = factor*factor;

    T x = v1.x + ((v2.x - v1.x)*f2);
    T y = v1.y + ((v2.y - v1.y)*f2);
    T z = v1.z + ((v2.z - v1.z)*f2);
    T w = v1.w + ((v2.w - v1.w)*f2);

    vOut.x = x;
    vOut.y = y;
    vOut.z = z;
    vOut.w = w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cosinus Interpolation, Using Two 4D Vectors
template <class T>
void CMVec4Cosrp(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, T factor)
{   
    T ft = mmultpi(factor);
    T f = (T(1) - MVEC4COS(ft))*T(0.5);
    
    T x = v1.x + ((v2.x - v1.x)*ft);
    T y = v1.y + ((v2.y - v1.y)*ft);
    T z = v1.z + ((v2.z - v1.z)*ft);
    T w = v1.w + ((v2.w - v1.w)*ft);

    vOut.x = x;
    vOut.y = y;
    vOut.z = z;
    vOut.w = w;
}


///////////////////////////////////////////////////////////////////////////////////////