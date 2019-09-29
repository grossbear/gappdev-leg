///////////////////////////////////////////////////////////////////////////////////////
//  Matrix.cpp
//
//  Matrix Classes Templates Methods Definitions
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
#include "Plane.h"
#include "Matrix.h"

#include "Algebra.h"

///////////////////////////////////////////////////////////////////////////////////////
// 3x3 Matrix Non-Inline Functions
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template <class T>
void CMMtx33Add(CMMatrix33<T> &Out, const CMMatrix33<T> &Mtx1, const CMMatrix33<T> &Mtx2)
{
    Out._11 = Mtx1._11 + Mtx2._11;    
    Out._12 = Mtx1._12 + Mtx2._12;    
    Out._13 = Mtx1._13 + Mtx2._13;

    Out._21 = Mtx1._21 + Mtx2._21;    
    Out._22 = Mtx1._22 + Mtx2._22;    
    Out._23 = Mtx1._23 + Mtx2._23;

    Out._31 = Mtx1._31 + Mtx2._31;    
    Out._32 = Mtx1._32 + Mtx2._32;    
    Out._33 = Mtx1._33 + Mtx2._33;
}

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting 3x3 Matrices
template <class T>
void CMMtx33Sub(CMMatrix33<T> &Out, const CMMatrix33<T> &Mtx1, const CMMatrix33<T> &Mtx2)
{
    Out._11 = Mtx1._11 - Mtx2._11;    
    Out._12 = Mtx1._12 - Mtx2._12;    
    Out._13 = Mtx1._13 - Mtx2._13;

    Out._21 = Mtx1._21 - Mtx2._21;    
    Out._22 = Mtx1._22 - Mtx2._22;    
    Out._23 = Mtx1._23 - Mtx2._23;

    Out._31 = Mtx1._31 - Mtx2._31;    
    Out._32 = Mtx1._32 - Mtx2._32;    
    Out._33 = Mtx1._33 - Mtx2._33;
}

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two 3x3 Matrices
template <class T>
void CMMtx33Mult(CMMatrix33<T> &Out, const CMMatrix33<T> &M1, const CMMatrix33<T> &M2)
{
    T m[9];
    const T *p1 = (const T*)M1;
    const T *p2 = (const T*)M2;

    m[0] = p1[0]*p2[0] + p1[1]*p2[3] + p1[2]*p2[6];
    m[1] = p1[0]*p2[1] + p1[1]*p2[4] + p1[2]*p2[7];
    m[2] = p1[0]*p2[2] + p1[1]*p2[5] + p1[2]*p2[8];

    m[3] = p1[3]*p2[0] + p1[4]*p2[3] + p1[5]*p2[6];
    m[4] = p1[3]*p2[1] + p1[4]*p2[4] + p1[5]*p2[7];
    m[5] = p1[3]*p2[2] + p1[4]*p2[5] + p1[5]*p2[8];

    m[6] = p1[6]*p2[0] + p1[7]*p2[3] + p1[8]*p2[6];
    m[7] = p1[6]*p2[1] + p1[7]*p2[4] + p1[8]*p2[7];
    m[8] = p1[6]*p2[2] + p1[7]*p2[5] + p1[8]*p2[8];

    memcpy(&Out._11, &m[0], sizeof(T)*9);
}

///////////////////////////////////////////////////////////////////////////////////////
// Creating Inverse Matrix
template <class T>
boolt CMMtx33Inverse(CMMatrix33<T> &Out, const CMMatrix33<T> &M)
{
    T m[9];
    T det[9];
    const T *p = (const T*)M;

    det[0] = p[4]*p[8] - p[7]*p[5];
    det[1] = p[3]*p[8] - p[6]*p[5];
    det[2] = p[3]*p[7] - p[6]*p[4];

    T d = p[0]*det[0] - p[1]*det[1] + p[2]*det[2];

    if (mnear0(d))
        return false;

    det[3] = p[1]*p[8] - p[7]*p[2];
    det[4] = p[0]*p[8] - p[6]*p[2];
    det[5] = p[0]*p[7] - p[6]*p[1];

    det[6] = p[1]*p[5] - p[4]*p[2];
    det[7] = p[0]*p[5] - p[3]*p[2];
    det[8] = p[0]*p[4] - p[3]*p[1];
    

    T inv = T(1)/d;

    m[0] =  det[0]*inv;
    m[1] = -det[3]*inv;
    m[2] =  det[6]*inv;

    m[3] = -det[1]*inv;
    m[4] =  det[4]*inv;
    m[5] = -det[7]*inv;

    m[6] =  det[2]*inv;
    m[7] = -det[5]*inv;
    m[8] =  det[8]*inv;

    memcpy(&Out._11, &m[0], sizeof(T)*9);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
template <class T>
void CMMtx33Transpose(CMMatrix33<T> &Out, const CMMatrix33<T> &M)
{
    T tmp;
    const T* p1 = (const T*)M;
    T *p2 = (T*)Out;

    p2[0] = p1[0];  p2[4] = p1[4];  p2[8] = p1[8];

    tmp = p1[3];
    p2[3] = p1[3];
    p2[1] = tmp;

    tmp = p1[6];
    p2[6] = p1[2];
    p2[2] = tmp;

    tmp = p1[7];
    p2[7] = p1[5];
    p2[5] = tmp;

}

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix In 2D Space
template <class T>
void CMMtx33Scale2D(CMMatrix33<T> &M, T x, T y)
{
    T *p = (T*)M;

    memset(p,0,sizeof(T)*9);

    p[0] = x;
    p[4] = y;
    p[8] = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Inverse Scale Values Matrix In 2D Space
template <class T>
void CMMtx33ScaleInv2D(CMMatrix33<T> &M, T x, T y)
{
    ASSERT(!mnear0(x));
    ASSERT(!mnear0(y));

    memset(&M._11,0,sizeof(T)*9);
    
    M._11 = T(1)/x;
    M._22 = T(1)/y;
    M._33 = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Translation Matrix In 2D Space
template <class T>
void CMMtx33Translate2D(CMMatrix33<T> *pM, T x, T y)
{
    ASSERT(pM != NULL);

    memset(&pM->_11,0,sizeof(T)*9);

    pM->_11 = pM->_22 = pM->_33 = T(1);
    pM->_31 = x;
    pM->_32 = y;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template <class T>
void CMMtx33Scale(CMMatrix33<T> *pM, T x, T y, T z)
{
    ASSERT(pM != NULL);

    memset(&pM->_11,0,sizeof(T)*9);

    pM->_11 = x;
    pM->_22 = y;
    pM->_33 = z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Inverse Scale Values Matrix
template <class T>
void CMMtx33ScaleInv(CMMatrix33<T> *pM, T x, T y, T z)
{
    ASSERT(pM != NULL);

    memset(&pM->_11,0,sizeof(T)*9);

    pM->_11 = T(1)/x;
    pM->_22 = T(1)/y;
    pM->_33 = T(1)/z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix In 2D Space
template <class T>
void CMMtx33Rotate2D(CMMatrix33<T> &M, T rad)
{
    memset(&M._11,0,sizeof(T)*9);

    T sinA, cosA;
    MMTX33SINCOS(rad,sinA,cosA);

    M._11 = cosA;
    M._12 = sinA;
    M._21 = -sinA;
    M._22 = cosA;
    M._33 = T(1);
}


///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template <class T>
void CMMtx33RotateX(CMMatrix33<T> &M, T rad)
{
    memset(&M._11,0,sizeof(T)*9);

    T sinA, cosA;
    MMTX33SINCOS(rad,sinA,cosA);

    M._11 = T(1);
    M._22 = cosA;
    M._23 = sinA;
    M._32 = -sinA;
    M._33 = cosA;
}


///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template <class T>
void CMMtx33RotateY(CMMatrix33<T> &M, T rad)
{
    memset(&M._11,0,sizeof(T)*9);

    T sinA, cosA;
    MMTX33SINCOS(rad,sinA,cosA);

    M._11 = cosA;
    M._13 = -sinA;
    M._22 = T(1);
    M._31 = sinA;
    M._33 = cosA;
}


///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template <class T>
void CMMtx33RotateZ(CMMatrix33<T> &M, T rad)
{
    memset(&M._11,0,sizeof(T)*9);

    T sinA, cosA;
    MMTX33SINCOS(rad,sinA,cosA);

    M._11 = cosA;
    M._12 = sinA;
    M._21 = -sinA;
    M._22 = cosA;
    M._33 = T(1);
}


///////////////////////////////////////////////////////////////////////////////////////
// Creates A Matrix That Rotates Around An Arbitrary Axis
template <class T>
void CMMtx33Rotate(CMMatrix33<T> &M, const CMVector3D<T> &vNormVec, T rad)
{
    memset(&M._11, 0, sizeof(T)*9);

    T sinA, cosA;
    MMTX33SINCOS(rad,sinA,cosA);

    T *m = (T*)M;
    T one = T(1);

    T xx = vNormVec.x * vNormVec.x;
    T yy = vNormVec.y * vNormVec.y;
    T zz = vNormVec.z * vNormVec.z;

    T xy = vNormVec.x * vNormVec.y;
    T yz = vNormVec.y * vNormVec.z;
    T zx = vNormVec.z * vNormVec.x;

    T xs = vNormVec.x * sinA;
    T ys = vNormVec.y * sinA;
    T zs = vNormVec.z * sinA;

    T onec = T(1) - cosA;

    m[0] = onec * xx + cosA;
    m[1] = onec * xy + zs;
    m[2] = onec * zx - ys;

    m[3] = onec * xy - zs;
    m[4] = onec * yy + cosA;
    m[5] = onec * yz + xs;

    m[6] = onec * zx + ys;
    m[7] = onec * yz - xs;
    m[8]= onec * zz + cosA;

}


///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x3 Matrix
template <class T>
void CMVec3Mult(CMVector3D<T> &Out, const CMVector3D<T> &v, const CMMatrix33<T> &M)
{
    T x = v.x*M._11 + v.y*M._21 + v.z*M._31;
    T y = v.x*M._12 + v.y*M._22 + v.z*M._32;
    T z = v.x*M._13 + v.y*M._23 + v.z*M._33;

    Out.x = x;
    Out.y = y;
    Out.z = z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 2D Vector By 2x2 Matrix
template <class T>
void CMVec2Transform(CMVector2D<T> &Out, const CMVector2D<T> &v, const CMMatrix33<T> &M)
{
    T x = v.x*M._11 + v.y*M._21 + M._31;
    T y = v.x*M._12 + v.y*M._22 + M._32;

    Out.x = x;
    Out.y = y;
}

///////////////////////////////////////////////////////////////////////////////////////
// Decomposing Transformation 3x3 Matrix
template <class T>
void CMMtx33Decompose(const CMMatrix33<T> &M, CMVector3D<T> &vScale, CMVector3D<T> &vRotation)
{
    T Sx = MMTX33SQRT(M._11, M._21, M._31);
    T Sy = MMTX33SQRT(M._12, M._22, M._32);
    T Sz = MMTX33SQRT(M._13, M._23, M._33);

    CMVec3Set(vScale, Sx, Sy, Sz);

    // Getting Rotation Angles In Radians
    vRotation.y = MMTX33ASIN(M._13);
    vRotation.x = MMTX33ATAN2(M._23*Sx,M._33);
    vRotation.z = MMTX33ATAN2(M._22*Sy,M._21);
}


///////////////////////////////////////////////////////////////////////////////////////
// 4x4 Matrix Non-Inline Functions
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template <class T>
void CMMtx44Add(CMMatrix44<T> &Out, const CMMatrix44<T> &Mtx1, const CMMatrix44<T> &Mtx2)
{
    Out._11 = Mtx1._11 + Mtx2._11;    
    Out._12 = Mtx1._12 + Mtx2._12;    
    Out._13 = Mtx1._13 + Mtx2._13;
    Out._14 = Mtx1._14 + Mtx2._14;

    Out._21 = Mtx1._21 + Mtx2._21;    
    Out._22 = Mtx1._22 + Mtx2._22;    
    Out._23 = Mtx1._23 + Mtx2._23;
    Out._24 = Mtx1._24 + Mtx2._24;

    Out._31 = Mtx1._31 + Mtx2._31;    
    Out._32 = Mtx1._32 + Mtx2._32;    
    Out._33 = Mtx1._33 + Mtx2._33;
    Out._34 = Mtx1._34 + Mtx2._34;

    Out._31 = Mtx1._31 + Mtx2._31;    
    Out._32 = Mtx1._32 + Mtx2._32;    
    Out._33 = Mtx1._33 + Mtx2._33;
    Out._34 = Mtx1._34 + Mtx2._34;
}

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Two Matrices
template <class T>
void CMMtx44Sub(CMMatrix44<T> &Out, const CMMatrix44<T> &Mtx1, const CMMatrix44<T> &Mtx2)
{
    Out._11 = Mtx1._11 - Mtx2._11;    
    Out._12 = Mtx1._12 - Mtx2._12;    
    Out._13 = Mtx1._13 - Mtx2._13;
    Out._14 = Mtx1._14 - Mtx2._14;

    Out._21 = Mtx1._21 - Mtx2._21;    
    Out._22 = Mtx1._22 - Mtx2._22;    
    Out._23 = Mtx1._23 - Mtx2._23;
    Out._24 = Mtx1._24 - Mtx2._24;

    Out._31 = Mtx1._31 - Mtx2._31;    
    Out._32 = Mtx1._32 - Mtx2._32;    
    Out._33 = Mtx1._33 - Mtx2._33;
    Out._34 = Mtx1._34 - Mtx2._34;

    Out._31 = Mtx1._31 - Mtx2._31;    
    Out._32 = Mtx1._32 - Mtx2._32;    
    Out._33 = Mtx1._33 - Mtx2._33;
    Out._34 = Mtx1._34 - Mtx2._34;
}   

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two 4x4 Matrices
template <class T>
void CMMtx44Mult(CMMatrix44<T> &Out, const CMMatrix44<T> &M1, const CMMatrix44<T> &M2)
{
    T m[16];
    const T *p1 = (const T*)M1;
    const T *p2 = (const T*)M2;

    m[0] = p1[0]*p2[0] + p1[1]*p2[4] + p1[2]*p2[8] + p1[3]*p2[12];
    m[1] = p1[0]*p2[1] + p1[1]*p2[5] + p1[2]*p2[9] + p1[3]*p2[13];
    m[2] = p1[0]*p2[2] + p1[1]*p2[6] + p1[2]*p2[10] + p1[3]*p2[14];
    m[3] = p1[0]*p2[3] + p1[1]*p2[7] + p1[2]*p2[11] + p1[3]*p2[15];

    m[4] = p1[4]*p2[0] + p1[5]*p2[4] + p1[6]*p2[8] + p1[7]*p2[12];
    m[5] = p1[4]*p2[1] + p1[5]*p2[5] + p1[6]*p2[9] + p1[7]*p2[13];
    m[6] = p1[4]*p2[2] + p1[5]*p2[6] + p1[6]*p2[10] + p1[7]*p2[14];
    m[7] = p1[4]*p2[3] + p1[5]*p2[7] + p1[6]*p2[11] + p1[7]*p2[15];

    m[8] = p1[8]*p2[0] + p1[9]*p2[4] + p1[10]*p2[8] + p1[11]*p2[12];
    m[9] = p1[8]*p2[1] + p1[9]*p2[5] + p1[10]*p2[9] + p1[11]*p2[13];
    m[10] = p1[8]*p2[2] + p1[9]*p2[6] + p1[10]*p2[10] + p1[11]*p2[14];
    m[11] = p1[8]*p2[3] + p1[9]*p2[7] + p1[10]*p2[11] + p1[11]*p2[15];

    m[12] = p1[12]*p2[0] + p1[13]*p2[4] + p1[14]*p2[8] + p1[15]*p2[12];
    m[13] = p1[12]*p2[1] + p1[13]*p2[5] + p1[14]*p2[9] + p1[15]*p2[13];
    m[14] = p1[12]*p2[2] + p1[13]*p2[6] + p1[14]*p2[10] + p1[15]*p2[14];
    m[15] = p1[12]*p2[3] + p1[13]*p2[7] + p1[14]*p2[11] + p1[15]*p2[15];

    memcpy(&Out._11, &m[0], sizeof(T)*16);
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculating Inverse Matrix
template <class T>
boolt CMMtx44Inverse(CMMatrix44<T> &Out, const CMMatrix44<T> &M)
{
    T m[16];
    T f[18];
    const T *p = (const T*)M;

    f[0] = M._33*M._44 - M._43*M._34;
    f[1] = M._32*M._44 - M._42*M._34;
    f[2] = M._32*M._43 - M._42*M._33;
    f[3] = M._31*M._44 - M._41*M._34;
    f[4] = M._31*M._43 - M._41*M._33;
    f[5] = M._31*M._42 - M._41*M._32;
    

    T det = M._11*(M._22*f[0] - M._23*f[1] + M._24*f[2]) - 
            M._12*(M._21*f[0] - M._23*f[3] + M._24*f[4]) +
            M._13*(M._21*f[1] - M._22*f[3] + M._24*f[5]) -
            M._14*(M._21*f[2] - M._22*f[4] + M._23*f[5]);

    if (mnear0(det))
        return false;

    T inv = T(1)/det;

    f[6] = M._23*M._44 - M._43*M._24;
    f[7] = M._22*M._44 - M._42*M._24;
    f[8] = M._22*M._43 - M._42*M._23;
    f[9] = M._21*M._44 - M._41*M._24;
    f[10]= M._21*M._43 - M._41*M._23;
    f[11]= M._21*M._42 - M._41*M._22;
    f[12]= M._23*M._34 - M._33*M._24;
    f[13]= M._22*M._34 - M._32*M._24;
    f[14]= M._22*M._33 - M._32*M._23;
    f[15]= M._21*M._34 - M._31*M._24;
    f[16]= M._21*M._33 - M._31*M._23;
    f[17]= M._21*M._32 - M._31*M._22;

    m[0] =  (M._22*f[0] - M._23*f[1] + M._24*f[2])*inv;
    m[1] = -(M._12*f[0] - M._13*f[1] + M._14*f[2])*inv;
    m[2] =  (M._12*f[6] - M._13*f[7] + M._14*f[8])*inv;
    m[3] = -(M._12*f[12]- M._13*f[13]+ M._14*f[14])*inv;

    m[4] = -(M._21*f[0] - M._23*f[3] + M._24*f[4])*inv;
    m[5] =  (M._11*f[0] - M._13*f[3] + M._14*f[4])*inv;
    m[6] = -(M._11*f[6] - M._13*f[9] + M._14*f[10])*inv;
    m[7] =  (M._11*f[12]- M._13*f[15]+ M._14*f[16])*inv;

    m[8] =  (M._21*f[1] - M._22*f[3] + M._24*f[5])*inv;
    m[9] = -(M._11*f[1] - M._12*f[3] + M._14*f[5])*inv;
    m[10]=  (M._11*f[7] - M._12*f[9] + M._14*f[11])*inv;
    m[11]= -(M._11*f[13]- M._12*f[15]+ M._14*f[17])*inv;

    m[12]= -(M._21*f[2] - M._22*f[4] + M._23*f[5])*inv;
    m[13]=  (M._11*f[2] - M._12*f[4] + M._13*f[5])*inv;
    m[14]= -(M._11*f[8] - M._12*f[10]+ M._13*f[11])*inv;
    m[15]=  (M._11*f[14]- M._12*f[16]+ M._13*f[17])*inv;


    memcpy(&Out._11, &m[0], sizeof(T)*16);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculating Affine Inverse Matrix
template <class T>
boolt CMMtx44InverseAffine(CMMatrix44<T> &Out, const CMMatrix44<T> &M)
{
    T res[16];
    T *m = (T*)M;

    res[0] = m[0];
    res[1] = m[4];
    res[2] = m[8];
    res[3] = 0;
    res[4] = m[1];
    res[5] = m[5];
    res[6] = m[9];
    res[7] = 0;
    res[8] = m[2];
    res[9] = m[6];
    res[10]= m[10];
    res[11]= 0;
    res[12]= -(m[12]*m[0]) - (m[13]*m[1]) - (m[14]*m[2]);
    res[13]= -(m[12]*m[4]) - (m[13]*m[5]) - (m[14]*m[6]);
    res[14]= -(m[12]*m[8]) - (m[13]*m[9]) - (m[14]*m[10]);
    res[15]= m[15];
    
    memcpy(&Out._11, &res[0], sizeof(T)*16);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
template <class T>
void CMMtx44Transpose(CMMatrix44<T> &Out, const CMMatrix44<T> &M)
{
    T tmp;
    tmp = M.m[0][1];
    Out.m[0][1] = M.m[1][0];
    Out.m[1][0] = tmp;

    tmp = M.m[0][2];
    Out.m[0][2] = M.m[2][0];
    Out.m[2][0] = tmp;

    tmp = M.m[1][2];
    Out.m[1][2] = M.m[2][1];
    Out.m[2][1] = tmp;

    tmp = M.m[0][3];
    Out.m[0][3] = M.m[3][0];
    Out.m[3][0] = tmp;

    tmp = M.m[1][3];
    Out.m[1][3] = M.m[3][1];
    Out.m[3][1] = tmp;

    tmp = M.m[2][3];
    Out.m[2][3] = M.m[3][2];
    Out.m[3][2] = tmp;
}


///////////////////////////////////////////////////////////////////////////////////////
// Create Translate Matrix
template <class T>
void CMMtx44Translate(CMMatrix44<T> &M, T x, T y, T z)
{
    memset(&M._11, 0, sizeof(T)*16);
    M._11 = M._22 = M._33 = M._44 = T(1);

    M._41 = x;
    M._42 = y;
    M._43 = z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template <class T>
void CMMtx44Scale(CMMatrix44<T> &M, T x, T y, T z)
{
    memset(&M._11, 0, sizeof(T)*16);
    M._11 = x;
    M._22 = y;
    M._33 = z;
    M._44 = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template <class T>
void CMMtx44RotateX(CMMatrix44<T> &M, T rad)
{
    memset(&M._11, 0, sizeof(T)*16);

    T sinA, cosA;
    MMTX44SINCOS(rad,sinA,cosA);

    M._11 = T(1);
    M._22 = cosA;
    M._23 = sinA;
    M._32 = -sinA;
    M._33 = cosA;
    M._44 = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template <class T>
void CMMtx44RotateY(CMMatrix44<T> &M, T rad)
{
    memset(&M._11, 0, sizeof(T)*16);

    T sinA, cosA;
    MMTX44SINCOS(rad,sinA,cosA);

    M._11 = cosA;
    M._13 = -sinA;
    M._22 = T(1);
    M._31 = sinA;
    M._33 = cosA;
    M._44 = T(1);
}


///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template <class T>
void CMMtx44RotateZ(CMMatrix44<T> &M, T rad)
{
    memset(&M._11, 0, sizeof(T)*16);

    T sinA, cosA;
    MMTX44SINCOS(rad,sinA,cosA);

    M._11 = cosA;
    M._12 = sinA;
    M._21 = -sinA;
    M._22 = cosA;
    M._33 = T(1);
    M._44 = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Creates A Matrix That Rotates Around An Arbitrary Axis
template <class T>
void CMMtx44Rotate(CMMatrix44<T> &M, const CMVector3D<T> &vNormVec, T rad)
{
    memset(&M._11, 0, sizeof(T)*16);

    T sinA, cosA;
    MMTX44SINCOS(rad,sinA,cosA);

    T *m = (T*)M;
    T one = T(1);

    T xx = vNormVec.x * vNormVec.x;
    T yy = vNormVec.y * vNormVec.y;
    T zz = vNormVec.z * vNormVec.z;

    T xy = vNormVec.x * vNormVec.y;
    T yz = vNormVec.y * vNormVec.z;
    T zx = vNormVec.z * vNormVec.x;

    T xs = vNormVec.x * sinA;
    T ys = vNormVec.y * sinA;
    T zs = vNormVec.z * sinA;

    T onec = T(1) - cosA;

    m[0] = onec * xx + cosA;
    m[1] = onec * xy + zs;
    m[2] = onec * zx - ys;

    m[4] = onec * xy - zs;
    m[5] = onec * yy + cosA;
    m[6] = onec * yz + xs;

    m[8] = onec * zx + ys;
    m[9] = onec * yz - xs;
    m[10]= onec * zz + cosA;

    m[15]= T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 4x4 Matrix
template <class T>
void CMVec4Mult(CMVector4D<T> &Out, const CMVector4D<T> &vec, const CMMatrix44<T> &M)
{
    T v[4];

    v[0] = vec.x*M._11 + vec.y*M._21 + vec.z*M._31 + vec.w*M._41;
    v[1] = vec.x*M._12 + vec.y*M._22 + vec.z*M._32 + vec.w*M._42;
    v[2] = vec.x*M._13 + vec.y*M._23 + vec.z*M._33 + vec.w*M._43;
    v[3] = vec.x*M._14 + vec.y*M._24 + vec.z*M._34 + vec.w*M._44;

    Out.x = v[0];
    Out.y = v[1];
    Out.z = v[2];
    Out.w = v[3];
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 4x4 Matrix Multiplying Only By First Three Rows
template <class T>
void CMVec4Mult3(CMVector4D<T> &Out, const CMVector4D<T> &vec, const CMMatrix44<T> &M)
{
    T v[3];

    v[0] = vec.x*M._11 + vec.y*M._21 + vec.z*M._31;
    v[1] = vec.x*M._12 + vec.y*M._22 + vec.z*M._32;
    v[2] = vec.x*M._13 + vec.y*M._23 + vec.z*M._33;

    Out.x = v[0];
    Out.y = v[1];
    Out.z = v[2];
    Out.w = vec.w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x3 Matrix
template <class T>
void CMVec4Mult(CMVector4D<T> &Out, const CMVector4D<T> &vec, const CMMatrix33<T> &M)
{
    T v[3];

    v[0] = vec.x*M._11 + vec.y*M._21 + vec.z*M._31;
    v[1] = vec.x*M._12 + vec.y*M._22 + vec.z*M._32;
    v[2] = vec.x*M._13 + vec.y*M._23 + vec.z*M._33;

    Out.x = v[0];
    Out.y = v[1];
    Out.z = v[2];
    Out.w = vec.w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 4x4 Matrix 
template <class T>
void CMVec3Mult(CMVector3D<T> &Out, const CMVector3D<T> &vec, const CMMatrix44<T> &M)
{
    T v[3];

    v[0] = vec.x*M._11 + vec.y*M._21 + vec.z*M._31 + M._41;
    v[1] = vec.x*M._12 + vec.y*M._22 + vec.z*M._32 + M._42;
    v[2] = vec.x*M._13 + vec.y*M._23 + vec.z*M._33 + M._43;
    
    Out.x = v[0];
    Out.y = v[1];
    Out.z = v[2];
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 4x4 Matrix Multiplying Only By First Three Rows
template <class T>
void CMVec3Mult3(CMVector3D<T> &Out, const CMVector3D<T> &vec, const CMMatrix44<T> &M)
{
    T v[3];

    v[0] = vec.x*M._11 + vec.y*M._21 + vec.z*M._31;
    v[1] = vec.x*M._12 + vec.y*M._22 + vec.z*M._32;
    v[2] = vec.x*M._13 + vec.y*M._23 + vec.z*M._33;
    
    Out.x = v[0];
    Out.y = v[1];
    Out.z = v[2];
}

///////////////////////////////////////////////////////////////////////////////////////
// Builds A Transformation Matrix. NULL Arguments Are Treated As Default Transformations.
template <class T>
void CMMtx44Transformation(CMMatrix44<T> &M, const CMVector3D<T> &vTranslation, const CMVector3D<T> &vRotation,
                     const CMVector3D<T> &vScale, boolt israd)
{
    CMVector3D<T> translation;
    CMVector3D<T> rotation;
    CMVector3D<T> scale(T(1),T(1),T(1));
    
    //if(Translation != NULL)
    translation = vTranslation;
    
    //if(Rotation != NULL)
    rotation = vRotation;

    //if(Scale != NULL)
    scale = vScale;

    if(!israd)
    {
        rotation.x = mtorad(rotation.x);
        rotation.y = mtorad(rotation.y);
        rotation.z = mtorad(rotation.z);
    }

    T sinval[3],cosval[3];
    for(int32t i = 0; i < 3; i++)
    {
        if(mnear0(rotation[i]))
        {
            sinval[i] = T(0);
            cosval[i] = T(1);
        }
        else
        {
            MMTX44SINCOS(rotation[i],sinval[i],cosval[i]);
        }
    }
    
    M._11 = scale[0]*cosval[1]*cosval[2],
    M._12 = scale[0]*cosval[1]*sinval[2],
    M._13 = -scale[0]*sinval[1];
    M._14 = T(0);

    M._21 = scale[1]*(sinval[0]*sinval[1]*cosval[2]-cosval[0]*sinval[2]);
    M._22 = scale[1]*(sinval[0]*sinval[1]*sinval[2]+cosval[0]*cosval[2]);
    M._23 = scale[1]*sinval[0]*cosval[1];
    M._24 = T(0);

    M._31 = scale[2]*(cosval[0]*sinval[1]*cosval[2]+sinval[0]*sinval[2]);
    M._32 = scale[2]*(cosval[0]*sinval[1]*sinval[2]-sinval[0]*cosval[2]);
    M._33 = scale[2]*cosval[0]*cosval[1];
    M._34 = T(0);

    M._41 = translation.x;
    M._42 = translation.y;
    M._43 = translation.z;
    M._44 = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Decomposing Transformation 4x4 Matrix
template <class T>
void CMMtx44Decompose(const CMMatrix44<T> &M, CMVector3D<T> &vScale, CMVector3D<T> &vRotation,
                      CMVector3D<T> &vTranslation)
{
    T Sx = MMTX33SQRT(M._11, M._21, M._31);
    T Sy = MMTX33SQRT(M._12, M._22, M._32);
    T Sz = MMTX33SQRT(M._13, M._23, M._33);
    
    CMVec3Set(vScale, Sx, Sy, Sz);

    // Getting Rotation Angles In Radians
    vRotation.y = MMTX44ASIN(M._13);
    vRotation.x = MMTX44ATAN2(M._23*Sx,M._33);
    vRotation.z = MMTX44ATAN2(M._22*Sy,M._21);
    
    // Getting Translation
    CMVec3Set(vTranslation,M._41,M._42,M._43);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A LookAt Matrix (Right-Handed)
template <class T>
void CMMtx44LookAtRH(CMMatrix44<T> &M, const CMVector3D<T> &vEye, const CMVector3D<T> &vAt,
                     const CMVector3D<T> &vUp)
{
    const T one = T(1);
    CMVector3D<T> zaxis = vEye - vAt;
    CMVec3Normalize(zaxis,zaxis);

    CMVector3D<T> xaxis;
    CMVec3Cross(xaxis,vUp,zaxis);
    CMVec3Normalize(xaxis,xaxis);

    CMVector3D<T> yaxis;
    CMVec3Cross(yaxis,zaxis,xaxis);

    M._11 = xaxis.x;  M._12 = yaxis.x;  M._13 = zaxis.x;  M._14 = T(0);
    M._21 = xaxis.y;  M._22 = yaxis.y;  M._23 = zaxis.y;  M._24 = T(0);
    M._31 = xaxis.z;  M._32 = yaxis.z;  M._33 = zaxis.z;  M._34 = T(0);
    
    M._41 = -CMVec3Dot(xaxis,vEye);
    M._42 = -CMVec3Dot(yaxis,vEye);
    M._43 = -CMVec3Dot(zaxis,vEye);
    M._44 = one;

    /*
    zaxis = normal(cameraPosition - cameraTarget)
    xaxis = normal(cross(cameraUpVector, zaxis))
    yaxis = cross(zaxis, xaxis)

    xaxis.x           yaxis.x           zaxis.x          0
    xaxis.y           yaxis.y           zaxis.y          0
    xaxis.z           yaxis.z           zaxis.z          0
    -dot(xaxis, cameraPosition)  -dot(yaxis, cameraPosition)  -dot(zaxis, cameraPosition)  1
    */
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A LookAt Matrix (Left-Handed)
template <class T>
void CMMtx44LookAtLH(CMMatrix44<T> &M, const CMVector3D<T> &vEye, const CMVector3D<T> &vAt,
                     const CMVector3D<T> &vUp)
{
    CMVector3D<T> zaxis = vAt - vEye;
    CMVec3Normalize(zaxis,zaxis);

    CMVector3D<T> xaxis;
    CMVec3Cross(xaxis,vUp,zaxis);
    CMVec3Normalize(xaxis,xaxis);

    CMVector3D<T> yaxis;
    CMVec3Cross(yaxis,zaxis,xaxis);

    M._11 = xaxis.x;  M._12 = yaxis.x;  M._13 = zaxis.x;  M._14 = T(0);
    M._21 = xaxis.y;  M._22 = yaxis.y;  M._23 = zaxis.y;  M._24 = T(0);
    M._31 = xaxis.z;  M._32 = yaxis.z;  M._33 = zaxis.z;  M._34 = T(0);
    
    M._41 = -CMVec3Dot(xaxis,vEye);
    M._42 = -CMVec3Dot(yaxis,vEye);
    M._43 = -CMVec3Dot(zaxis,vEye);
    M._44 = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template <class T>
void CMMtx44PerspectiveRH(CMMatrix44<T> &M, T w, T h, T zn, T zf)
{
    T two = T(2);

    M._11 = two*zn/w; M._12 = 0;        M._13 = 0;                M._14 = 0;
    M._21 = 0;        M._22 = 2*zn/h;   M._23 = 0;                M._24 = 0;
    M._31 = 0;        M._32 = 0;        M._33 = zf/(zn-zf);       M._34 = T(-1);
    M._41 = 0;        M._42 = 0;        M._43 = zn*zf/(zn-zf);    M._44 = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template <class T>
void CMMtx44PerspectiveLH(CMMatrix44<T> &M, T w, T h, T zn, T zf)
{
    T two = T(2);

    M._11 = two*zn/w; M._12 = T(0);     M._13 = 0;                M._14 = 0;
    M._21 = 0;        M._22 = two*zn/h; M._23 = 0;                M._24 = 0;
    M._31 = 0;        M._32 = 0;        M._33 = zf/(zf-zn);       M._34 = T(1);
    M._41 = 0;        M._42 = 0;        M._43 = zn*zf/(zn-zf);    M._44 = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template <class T>
void CMMtx44PerspectiveFOVRH(CMMatrix44<T> &M, T fov, T aspect, T zn, T zf)
{
    T sinA,cosA;
    MMTX44SINCOS(fov/T(2),sinA,cosA);

    T cot = cosA/sinA;
    T yScale = cot;
    T xScale = yScale/aspect;

    M._11 = xScale;   M._12 = 0;        M._13 = 0;             M._14 = 0;
    M._21 = 0;        M._22 = yScale;   M._23 = 0;             M._24 = 0;
    M._31 = 0;        M._32 = 0;        M._33 = zf/(zn-zf);    M._34 = T(-1);
    M._41 = 0;        M._42 = 0;        M._43 = zn*zf/(zn-zf); M._44 = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template <class T>
void CMMtx44PerspectiveFOVLH(CMMatrix44<T> &M, T fov, T aspect, T zn, T zf)
{
    T sinA,cosA;
    MMTX44SINCOS(fov/T(2),sinA,cosA);

    T cot = cosA/sinA;
    T yScale = cot;
    T xScale = yScale/aspect;

    M._11 = xScale;   M._12 = 0;        M._13 = 0;             M._14 = 0;
    M._21 = 0;        M._22 = yScale;   M._23 = 0;             M._24 = 0;
    M._31 = 0;        M._32 = 0;        M._33 = zf/(zf-zn);    M._34 = T(1);
    M._41 = 0;        M._42 = 0;        M._43 = zn*zf/(zn-zf); M._44 = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template <class T>
void CMMtx44PerspectiveOffCenterRH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf)
{
    T two = T(2);

    M._11 = two*zn/(r-l); M._12 = 0;            M._13 = 0;             M._14 = 0;
    M._21 = 0;            M._22 = two*zn/(t-b); M._23 = 0;             M._24 = 0;
    M._31 = (l+r)/(r-l);  M._32 = (t+b)/(t-b);  M._33 = zf/(zn-zf);    M._34 = T(-1);
    M._41 = 0;            M._42 = 0;            M._43 = zn*zf/(zn-zf); M._44 = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template <class T>
void CMMtx44PerspectiveOffCenterLH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf)
{
    T two = T(2);

    M._11 = two*zn/(r-l); M._12 = 0;            M._13 = 0;             M._14 = 0;
    M._21 = 0;            M._22 = two*zn/(t-b); M._23 = 0;             M._24 = 0;
    M._31 = (l+r)/(r-l);  M._32 = (t+b)/(b-t);  M._33 = zf/(zf-zn);    M._34 = T(1);
    M._41 = 0;            M._42 = 0;            M._43 = zn*zf/(zn-zf); M._44 = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Right-Handed)
template <class T>
void CMMtx44OrthoRH(CMMatrix44<T> &M, T w, T h, T zn, T zf)
{
    const T two = T(2);
    const T one = T(1);

    /*
    M._11 = two/w;    M._12 = 0;        M._13 = 0;            M._14 = 0;
    M._21 = 0;        M._22 = two/h;    M._23 = 0;            M._24 = 0;
    M._31 = 0;        M._32 = 0;        M._33 = one/(zn-zf);  M._34 = 0;
    M._41 = 0;        M._42 = 0;        M._43 = zn/(zn-zf);   M._44 = one;
   */

    M._11 = two/w;    M._12 = 0;        M._13 = 0;            M._14 = 0;
    M._21 = 0;        M._22 = two/h;    M._23 = 0;            M._24 = 0;
    M._31 = 0;        M._32 = 0;        M._33 = -two/(zf-zn); M._34 = 0;
    M._41 = 0;        M._42 = 0;        M._43 = 0;            M._44 = one;
   
}

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Left-Handed)
template <class T>
void CMMtx44OrthoLH(CMMatrix44<T> &M, T w, T h, T zn, T zf)
{
    T two = T(2);
    T one = T(1);

    M._11 = two/w;    M._12 = 0;        M._13 = 0;            M._14 = 0;
    M._21 = 0;        M._22 = two/h;    M._23 = 0;            M._24 = 0;
    M._31 = 0;        M._32 = 0;        M._33 = one/(zf-zn);  M._34 = 0;
    M._41 = 0;        M._42 = 0;        M._43 = zn/(zn-zf);   M._44 = one;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Right-Handed)
template <class T>
void CMMtx44OrthoOffCenterRH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf)
{
    T *p = (T*)M;
    static const T two = T(2);
    static const T one = T(1);

    /*
    p[0] = two/(r-l);       p[1] = 0;               p[2] = 0;                   p[3] = 0;
    p[4] = 0;               p[5] = two/(t-b);       p[6] = 0;                   p[7] = 0;
    p[8] = 0;               p[9] = 0;               p[10]= -two/(zf-zn);        p[11]= 0;
    p[12]= -((r+l)/(r-l));  p[13]= -((t+b)/(t-b));  p[14]= -(zf+zn/(zf-zn));    p[15]= one;
    */

    p[0] = two/(r-l);   p[1] = 0;           p[2] = 0;               p[3] = 0;
    p[4] = 0;           p[5] = two/(t-b);   p[6] = 0;               p[7] = 0;
    p[8] = 0;           p[9] = 0;           p[10]= two/(zn-zf);     p[11]= 0;
    p[12]= (l+r)/(l-r); p[13]= (t+b)/(b-t); p[14]= zn/(zn-zf);      p[15]= one;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Left-Handed)
template <class T>
void CMMtx44OrthoOffCenterLH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf)
{
    T *p = (T*)M;
    T two = T(2);
    T one = T(1);

    p[0] = two/(r-l);   p[1] = 0;           p[2] = 0;               p[3] = 0;
    p[4] = 0;           p[5] = two/(t-b);   p[6] = 0;               p[7] = 0;
    p[8] = 0;           p[9] = 0;           p[10]= one/(zf-zn);     p[11]= 0;
    p[12]= (l+r)/(l-r); p[13]= (t+b)/(b-t); p[14]= zn/(zn-zf);      p[15]= one;
}

///////////////////////////////////////////////////////////////////////////////////////
// Creates Matrix That Reflects Coordinate System About The Plane
template <class T>
void CMMtx44Reflect(CMMatrix44<T> &M, const CMPlane<T> &P)
{
    T one = T(1);
    T two = T(2);

    M._11 = -two * P.a * P.a + one; M._12 = -two * P.b * P.a;       M._13 = -two * P.c * P.a;       M._14 = 0;
    M._21 = -two * P.a * P.b;       M._22 = -two * P.b * P.b + one; M._23 = -two * P.c * P.b;       M._24 = 0;
    M._31 = -two * P.a * P.c;       M._32 = -two * P.b * P.c;       M._33 = -two * P.c * P.c + one; M._34 = 0;
    M._41 = -two * P.a * P.d;       M._42 = -two * P.b * P.d;       M._43 = -two * P.c * P.d;       M._44 = one;
}

///////////////////////////////////////////////////////////////////////////////////////
// Creates Matrix That Projects Geometry Into A Plane
template <class T>
void CMMtx44Project(CMMatrix44<T> &M, const CMVector4D<T> &L, const CMPlane<T> &P)
{
    T d = CMVec4Dot((CMVector4D<T>)P, L);

    M._11 = P.a * L.x + d;  M._12 = P.a * L.y;       M._13 = P.a * L.z;      M._14 = P.a * L.w;  
    M._21 = P.b * L.x;      M._22 = P.b * L.y + d;   M._23 = P.b * L.z;      M._24 = P.b * L.w;  
    M._31 = P.c * L.x;      M._23 = P.c * L.y;       M._33 = P.c * L.z + d;  M._34 = P.c * L.w; 
    M._41 = P.d * L.x;      M._24 = P.d * L.y;       M._34 = P.d * L.z;      M._44 = P.d * L.w + d;
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
void CMMtx44Project(CMMatrix44<T> &M, const CMVector3D<T> &L, const CMPlane<T> &P)
{
    T d = CMVec4Dot((CMVector4D<T>)P, CMVector4D<T>(L));

    M._11 = P.a * L.x + d;  M._12 = P.a * L.y;       M._13 = P.a * L.z;      M._14 = P.a;  
    M._21 = P.b * L.x;      M._22 = P.b * L.y + d;   M._23 = P.b * L.z;      M._24 = P.b;  
    M._31 = P.c * L.x;      M._23 = P.c * L.y;       M._33 = P.c * L.z + d;  M._34 = P.c; 
    M._41 = P.d * L.x;      M._24 = P.d * L.y;       M._34 = P.d * L.z;      M._44 = P.d + d;
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 3x4 Matrix Non-Inline Functions
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template <class T>
void CMMtx34Add(CMMatrix34<T> &Out, const CMMatrix34<T> &M1, const CMMatrix34<T> &M2)
{
    T m[12];
    const T *p1 = (const T*)M1.M;
    const T *p2 = (const T*)M2.M;

    m[0] = p1[0] + p2[0];   
    m[1] = p1[1] + p2[1];
    m[2] = p1[2] + p2[2];

    m[3] = p1[3] + p2[3];   
    m[4] = p1[4] + p2[4];
    m[5] = p1[5] + p2[5];

    m[6] = p1[6] + p2[3];   
    m[7] = p1[7] + p2[4];
    m[8] = p1[8] + p2[5];

    m[9] = M1.V.x + M2.V.x;
    m[10]= M1.V.y + M2.V.y;
    m[11]= M1.V.z + M2.V.z;

    memcpy(&Out.M._11, &m[0], sizeof(T)*9);
    memcpy(&Out.V.x, &m[9], sizeof(T)*3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Matrices
template <class T>
void CMMtx34Sub(CMMatrix34<T> &Out, const CMMatrix34<T> &M1, const CMMatrix34<T> &M2)
{
    T m[12];
    const T *p1 = (const T*)M1.M;
    const T *p2 = (const T*)M2.M;

    m[0] = p1[0] - p2[0];   
    m[1] = p1[1] - p2[1];
    m[2] = p1[2] - p2[2];

    m[3] = p1[3] - p2[3];   
    m[4] = p1[4] - p2[4];
    m[5] = p1[5] - p2[5];

    m[6] = p1[6] - p2[3];   
    m[7] = p1[7] - p2[4];
    m[8] = p1[8] - p2[5];

    m[9] = M1.V.x - M2.V.x;
    m[10]= M1.V.y - M2.V.y;
    m[11]= M1.V.z - M2.V.z;

    memcpy(&Out.M._11, &m[0], sizeof(T)*9);
    memcpy(&Out.V.x, &m[9], sizeof(T)*3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two Matrices
template <class T>
void CMMtx34Mult(CMMatrix34<T> &Out, const CMMatrix34<T> &M1, const CMMatrix34<T> &M2)
{
    T m[12];
    const T* p1 = (const T*)M1.M;
    const T* p2 = (const T*)M2.M;

    m[0] = p1[0]*p2[0] + p1[1]*p2[3] + p1[2]*p2[6];
    m[1] = p1[0]*p2[1] + p1[1]*p2[4] + p1[2]*p2[7];
    m[2] = p1[0]*p2[2] + p1[1]*p2[5] + p1[2]*p2[8];

    m[3] = p1[3]*p2[0] + p1[4]*p2[3] + p1[5]*p2[6];
    m[4] = p1[3]*p2[1] + p1[4]*p2[4] + p1[5]*p2[7];
    m[5] = p1[3]*p2[2] + p1[4]*p2[5] + p1[5]*p2[8];

    m[6] = p1[6]*p2[0] + p1[7]*p2[3] + p1[8]*p2[6];
    m[7] = p1[6]*p2[1] + p1[7]*p2[4] + p1[8]*p2[7];
    m[8] = p1[6]*p2[2] + p1[7]*p2[5] + p1[8]*p2[8];

    m[9] = M1.V.x*p2[0] + M1.V.y*p2[3] + M1.V.z*p2[6] + M2.V.x;
    m[10]= M1.V.x*p2[1] + M1.V.y*p2[4] + M1.V.z*p2[7] + M2.V.y;
    m[11]= M1.V.x*p2[2] + M1.V.y*p2[5] + M1.V.z*p2[8] + M2.V.z;

    memcpy(&Out.M._11, &m[0], sizeof(T)*9);
    memcpy(&Out.V.x, &m[9], sizeof(T)*3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Translate Matrix
template <class T>
void CMMtx34Translate(CMMatrix34<T> &M, T x, T y, T z)
{
    CMMtx33Identity(&M.M);
    M.V.x = x;    M.V.y = y;    M.V.z = z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template <class T>
void CMMtx34Scale(CMMatrix34<T> &M, T x, T y, T z)
{
    CMMtx33Scale(&M.M, x,y,z);
    memset(&M.V,0,sizeof(T)*3);
}


///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template <class T>
void CMMtx34RotateX(CMMatrix34<T> &M, T rad)
{
    CMMtx33RotateX(M.M, rad);
    memset(&M.V,0,sizeof(T)*3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template <class T>
void CMMtx34RotateY(CMMatrix34<T> &M, T rad)
{
    CMMtx33RotateY(M.M, rad);
    memset(&M.V,0,sizeof(T)*3);
}


///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template <class T>
void CMMtx34RotateZ(CMMatrix34<T> &M, T rad)
{
    CMMtx33RotateZ(M.M, rad);
    memset(&M.V,0,sizeof(T)*3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x4 Matrix 
template <class T>
void CMVec3Mult(CMVector3D<T> &Out, const CMVector3D<T> &vec, const CMMatrix34<T> &M)
{
    T m[3];
    const T* p1 = (const T*)M.M;
    const T* p2 = (const T*)vec;

    m[0] = p2[0]*p1[0] + p2[1]*p1[3] + p2[2]*p1[6] + M.V.x;
    m[1] = p2[0]*p1[1] + p2[1]*p1[4] + p2[2]*p1[7] + M.V.y;
    m[2] = p2[0]*p1[2] + p2[1]*p1[5] + p2[2]*p1[8] + M.V.z;

    memcpy(&Out.v[0],&m[0],sizeof(T)*3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x3 Matrix Of 3x4 Matrix
template <class T>
void CMVec3MultMtx33(CMVector3D<T> &Out, const CMVector3D<T> &vec, const CMMatrix34<T> &M)
{
    T m[3];
    const T* p1 = (const T*)M.M;
    const T* p2 = (const T*)vec;

    m[0] = p2[0]*p1[0] + p2[1]*p1[3] + p2[2]*p1[6];
    m[1] = p2[0]*p1[1] + p2[1]*p1[4] + p2[2]*p1[7];
    m[2] = p2[0]*p1[2] + p2[1]*p1[5] + p2[2]*p1[8];

    memcpy(&Out.v[0],&m[0],sizeof(T)*3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x4 Matrix 
template <class T>
void CMVec4Mult(CMVector4D<T> &Out, const CMVector4D<T> &vec, const CMMatrix34<T> &M)
{
    T m[4];
    const T* p1 = (const T*)M.M;
    const T* p2 = (const T*)vec;

    m[0] = p2[0]*p1[0] + p2[1]*p1[3] + p2[2]*p1[6] + p2[3]*M.V.x;
    m[1] = p2[0]*p1[1] + p2[1]*p1[4] + p2[2]*p1[7] + p2[3]*M.V.y;
    m[2] = p2[0]*p1[2] + p2[1]*p1[5] + p2[2]*p1[8] + p2[3]*M.V.z;
    m[3] = p2[3];

    memcpy(&Out.v[0],&m[0],sizeof(T)*4);
}

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x3 Matrix Of 3x4 Matrix
template <class T>
void CMVec4MultMtx33(CMVector4D<T> &Out, const CMVector4D<T> &vec, const CMMatrix34<T> &M)
{
    T m[4];
    const T* p1 = (const T*)M.M;
    const T* p2 = (const T*)vec;

    m[0] = p2[0]*p1[0] + p2[1]*p1[3] + p2[2]*p1[6];
    m[1] = p2[0]*p1[1] + p2[1]*p1[4] + p2[2]*p1[7];
    m[2] = p2[0]*p1[2] + p2[1]*p1[5] + p2[2]*p1[8];
    m[3] = p2[3];

    memcpy(&Out.v[0],&m[0],sizeof(T)*4);
}
///////////////////////////////////////////////////////////////////////////////////////