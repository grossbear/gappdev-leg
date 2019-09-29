///////////////////////////////////////////////////////////////////////////////////////
//  Quaternion.h
//
//  Quaternion Class Template Declaration
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _QUATERNION_H_
#define _QUATERNION_H_


///////////////////////////////////////////////////////////////////////////////////////
// Quaternion Class Declaration
template <class T>
class CMQuaternion
{
public:
    union{
        struct{
            T x,y,z,w;
        };
        T q[4];
    };

public:
    // Constructors
    CMQuaternion();
    CMQuaternion(CMQuaternion &q);
    CMQuaternion(CMVector3D<T> &v, T s);
    CMQuaternion(const T* const p);
    CMQuaternion(T x, T y, T z, T w);

    // Cast To Pointer  (T *)
    operator T* () const;
	operator const T* () const;
 
    // Cast To CMVector3D Type
    operator  CMVector3D<T>();

    // Cast To CMVector4D Type
    operator  CMVector4D<T>();

    // Access Grants
    T operator [] (int32t );
    T operator [] (int32t ) const;

    // Assignment Operators
    CMQuaternion& operator =  ( const CMQuaternion & );
    CMQuaternion& operator += ( const CMQuaternion & );
    CMQuaternion& operator -= ( const CMQuaternion & );
    CMQuaternion& operator *= ( const CMQuaternion & );
    CMQuaternion& operator *= ( const T & );
    CMQuaternion& operator /= ( const T & );

    // Unary Operators
    CMQuaternion operator + () const;
    CMQuaternion operator - () const;

    // Binary Operators
    CMQuaternion operator + ( const CMQuaternion & ) const;
    CMQuaternion operator - ( const CMQuaternion & ) const;
    CMQuaternion operator * ( const CMQuaternion & ) const;
    CMQuaternion operator * ( const T & ) const;
    CMQuaternion operator / ( const T & ) const;

    boolt operator == ( const CMQuaternion & ) const;
    boolt operator != ( const CMQuaternion & ) const;
};

//---------------------------
// Inline 
//---------------------------

////////////////////////////////////////////////////////////////////////////
// Adding Two Quaternion
template <class T>
void CMQuatAdd(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2);

////////////////////////////////////////////////////////////////////////////
// Subtracting Two Quaternions
template <class T>
void CMQuatSub(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2);

////////////////////////////////////////////////////////////////////////////
// Set Identity Quaternion
template <class T>
void CMQuatIdentity(CMQuaternion<T> &Q);

////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
boolt CMQuatIsIdentity(CMQuaternion<T> &Q);

////////////////////////////////////////////////////////////////////////////
// Quaternion Dot Product
template <class T>
T CMQuatDot(const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2);

////////////////////////////////////////////////////////////////////////////
// Conjugate Quaternion
template <class T>
void CMQuatConjugate(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q);

//---------------------------
// Non-Inline 
//---------------------------

////////////////////////////////////////////////////////////////////////////
// Multiplying Two Quaternions
template <class T>
void CMQuatMult(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2);

////////////////////////////////////////////////////////////////////////////
// Multiplying 3D Vector And Quaternion Treating 3D Vector As Quaternion 
// With Zero Const Part
template <class T>
void CMQuatMult(CMQuaternion<T> &qOut, const CMVector3D<T> &V1, const CMQuaternion<T> &Q1);

////////////////////////////////////////////////////////////////////////////
// Multiplying Quaternions And 3D Vector Treating 3D Vector As Quaternion 
// With Zero Const Part
template <class T>
void CMQuatMult(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMVector3D<T> &V1);

////////////////////////////////////////////////////////////////////////////
// Getting Rotating Angle
template <class T>
T CMQuatAngle(const CMQuaternion<T> &Q);

////////////////////////////////////////////////////////////////////////////
// Getting Normalised Vector Specified By Vector Part Of Quaternion
template <class T>
void CMQuatAxis(CMVector3D<T> &V, const CMQuaternion<T> &Q);

////////////////////////////////////////////////////////////////////////////
// Returns The Length Of Quaternion 
template <class T>
T CMQuatLength(const CMQuaternion<T> &Q);

////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of Quaternion
template <class T>
T CMQuatLengthSq(const CMQuaternion<T> &Q);

////////////////////////////////////////////////////////////////////////////
// Normalize A Quaternion
template <class T>
void CMQuatNormalize(CMQuaternion<T> &qOut, const CMQuaternion<T> &qIn);

////////////////////////////////////////////////////////////////////////////
// Rotate Quaternion Using Another Quaternion
template <class T>
void CMQuatRotate(CMQuaternion<T> &Q, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2);

////////////////////////////////////////////////////////////////////////////
// Rotating 3D Vector Using Quaternion
template <class T>
void CMQuatRotate(CMVector3D<T> &vOut, const CMQuaternion<T> &Q, const CMVector3D<T> &V);

////////////////////////////////////////////////////////////////////////////
// Create Quaternion For Rotating From Vector V1 To Vector V2
// Vectors Must Be Normalized
template <class T>
void CMQuatRotateArc(CMQuaternion<T> &Q, const CMVector3D<T> &V1, const CMVector3D<T> &V2);

////////////////////////////////////////////////////////////////////////////
// Bulding Quaternion From Three Euler Angles
template <class T>
void CMQuatFromAxisAngles(CMQuaternion<T> &qOut, T x, T y, T z);

////////////////////////////////////////////////////////////////////////////
// Getting Euler Angles From Quaternion
template <class T>
void CMQuatToAxisAngles(CMVector3D<T> &vOut, const CMQuaternion<T> &qIn);

////////////////////////////////////////////////////////////////////////////
// Bulding Quaternion From Matix
template <class T>
void CMMtxToQuat(CMQuaternion<T> &Q, const CMMatrix44<T> &M);
////////////////////////////////////////////////////////////////////////////
template <class T>
void CMMtxToQuat(CMQuaternion<T> &Q, const CMMatrix33<T> &M);

////////////////////////////////////////////////////////////////////////////
// Building Matrix From Quaternion
template <class T>
void CMQuatToMtx(CMMatrix44<T> &M, const CMQuaternion<T> &Q);
////////////////////////////////////////////////////////////////////////////
template <class T>
void CMQuatToMtx(CMMatrix33<T> &M, const CMQuaternion<T> &Q);

////////////////////////////////////////////////////////////////////////////
// Logarithm Of A Quaternion 
template <class T>
void CMQuatLog(CMQuaternion<T> &qOut, const CMQuaternion<T> &qIn);

////////////////////////////////////////////////////////////////////////////
// Exponent Of A Quaternion
template <class T>
void CMQuatExp(CMQuaternion<T> &qOut, const CMQuaternion<T> &qIn);

////////////////////////////////////////////////////////////////////////////
// Linear Interpolation Between Two Quaternions
template <class T>
void CMQuatLerp(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2, T weight);

////////////////////////////////////////////////////////////////////////////
// Spherical Interpolation Between Two Quaternions
template <class T>
void CMQuatSLerp(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2, T weight);

////////////////////////////////////////////////////////////////////////////
// Spherical Cubic Interpolation
template <class T>
void CMQuatSQuad(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2, 
                 const CMQuaternion<T> &A, const CMQuaternion<T> &B, T weight);

////////////////////////////////////////////////////////////////////////////
// Spline Interpolation
template <class T>
void CMQuatSpline(CMQuaternion<T> &Q, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Qm, const CMQuaternion<T> &Q2);

#include "Quaternion.inl"
#include "QuatSpec.h"

#endif //_QUATERNION_H_