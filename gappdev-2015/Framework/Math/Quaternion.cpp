///////////////////////////////////////////////////////////////////////////////////////
//  Quaternion.cpp
//
//  Quaternion Class Templates Methods Definitions
//
///////////////////////////////////////////////////////////////////////////////////////
#include "PlatformTypes.h"

#include <math.h>
#include <memory.h>

#include "MathConst.h"
#include "MathDefs.h"


#include "MathPrim.h"
#include "Trigonometry.h"

#include "MathLibDefs.h"

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
//#include "Plane.h"

#include "Algebra.h"

////////////////////////////////////////////////////////////////////////////
// Multiplying Two Quaternions
template <class T>
void CMQuatMult(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2)
{
    T q[4];

    q[0] = Q1.w*Q2.x + Q1.x*Q2.w + Q1.y*Q2.z - Q1.z*Q2.y;
    q[1] = Q1.w*Q2.y + Q1.y*Q2.w + Q1.z*Q2.x - Q1.x*Q2.z;
    q[2] = Q1.w*Q2.z + Q1.z*Q2.w + Q1.x*Q2.y - Q1.y*Q2.x;
    q[3] = Q1.w*Q2.w - Q1.x*Q2.x - Q1.y*Q2.y - Q1.z*Q2.z;

    memcpy(&qOut.x,&q[0],sizeof(T)*4);
}

////////////////////////////////////////////////////////////////////////////
// Multiplying 3D Vector And Quaternion Treating 3D Vector As Quaternion 
// With Zero Const Part
template <class T>
void CMQuatMult(CMQuaternion<T> &qOut, const CMVector3D<T> &V1, const CMQuaternion<T> &Q1)
{
    T q[4];

    q[0] = Q1.w*V1.x + Q1.z*V1.y - Q1.y*V1.z;
    q[1] = Q1.w*V1.y + Q1.x*V1.z - Q1.z*V1.x;
    q[2] = Q1.w*V1.z + Q1.y*V1.x - Q1.x*V1.y;
    q[3] = -(Q1.x*V1.x + Q1.y*V1.y + Q1.z*V1.z);

    memcpy(&qOut.x,&q[0],sizeof(T)*4);
}

////////////////////////////////////////////////////////////////////////////
// Multiplying Quaternion And 3D Vector Treating 3D Vector As Quaternion 
// With Zero Const Part
template <class T>
void CMQuatMult(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMVector3D<T> &V1)
{
    T q[4];
    
    q[0] = Q1.w*V1.x + Q1.y*V1.z - Q1.z*V1.y;
    q[1] = Q1.w*V1.y + Q1.z*V1.x - Q1.x*V1.z;
    q[2] = Q1.w*V1.z + Q1.x*V1.y - Q1.y*V1.x;
    q[3] = -(Q1.x*V1.x + Q1.y*V1.y + Q1.z*V1.z);

    memcpy(&qOut.x,&q[0],sizeof(T)*4);
}

////////////////////////////////////////////////////////////////////////////
// Getting Rotating Angle
template <class T>
T CMQuatAngle(const CMQuaternion<T> &Q)
{
    return 2*MQUATACOS(Q.w);
}


////////////////////////////////////////////////////////////////////////////
// Getting Normalised Vector Specified By Vector Part Of Quaternion
template <class T>
void CMQuatAxis(CMVector3D<T> &V, const CMQuaternion<T> &Q)
{
    V.x = Q.x;
    V.y = Q.y;
    V.z = Q.z;

    CMVec3Normalize(V,V);
}

////////////////////////////////////////////////////////////////////////////
// Returns The Length Of Quaternion 
template <class T>
T CMQuatLength(const CMQuaternion<T> &Q)
{
    return MQUATSQRT(Q.x, Q.y, Q.z, Q.w);
}

////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of Quaternion
template <class T>
T CMQuatLengthSq(const CMQuaternion<T> &Q)
{
    return Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w;
}


////////////////////////////////////////////////////////////////////////////
// Normalize A Quaternion
template <class T>
void CMQuatNormalize(CMQuaternion<T> &qOut, const CMQuaternion<T> &qIn)
{
    T revsqr = MQUATRSQRT(qIn.x, qIn.y, qIn.z, qIn.w);

    qOut.x = qIn.x*revsqr;
    qOut.y = qIn.y*revsqr;
    qOut.z = qIn.z*revsqr;
    qOut.w = qIn.w*revsqr;
}


////////////////////////////////////////////////////////////////////////////
// Rotate Quaternion Using Another Quaternion
template <class T>
void CMQuatRotate(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2)
{
    CMQuaternion<T> q3 = Q1;
    CMQuatConjugate(q3,q3);

    CMQuaternion<T> q12;
    CMQuatMult(q12,Q1,Q2);
    CMQuatMult(qOut,q12,q3);
}

////////////////////////////////////////////////////////////////////////////
// Rotating 3D Vector Using Quaternion
template <class T>
void CMQuatRotate(CMVector3D<T> &vOut, const CMQuaternion<T> &Q, const CMVector3D<T> &V)
{
    CMQuaternion<T> q1,q2,q3;

    CMQuatConjugate(q1,Q);
    CMQuatMult(q2,Q,V);
    CMQuatMult(q3,q2,q1);

    vOut.x = q3.x;
    vOut.y = q3.y;
    vOut.z = q3.z;
}

////////////////////////////////////////////////////////////////////////////
// Create Quaternion For Rotating From Vector V1 To Vector V2
// Vectors Must Be Normalized
template <class T>
void CMQuatRotateArc(CMQuaternion<T> &q, const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
    CMVector3D<T> c;
    CMVec3Cross(c, v1, v2);
    T d = CMVec3Dot(v1,v2);

    T one = T(1);
    T two = T(2);
    T s = MQUATSQRT((one+d)*two);

    q.x = c.x/s;
    q.y = c.y/s;
    q.z = c.z/s;
    q.w = s/two;
}

////////////////////////////////////////////////////////////////////////////
// Bulding Quaternion From Three Euler Angles
template <class T>
void CMQuatFromAxisAngles(CMQuaternion<T> &Q, T x, T y, T z)
{
    T two = T(2);

    T hroll = z/two;
    T hpitch = x/two;
    T hyaw = y/two;

    T cyaw, cpitch, croll, syaw, spitch, sroll;
    T cyaw_cpitch, syaw_spitch, cyaw_spitch, syaw_cpitch;

    MQUATSINCOS(hroll,sroll,croll);
    MQUATSINCOS(hpitch,spitch,cpitch);
    MQUATSINCOS(hyaw,syaw,cyaw);

    cyaw_cpitch = cyaw*cpitch;
	syaw_spitch = syaw*spitch;
	cyaw_spitch = cyaw*spitch;
	syaw_cpitch = syaw*cpitch;

    Q.x = cyaw_spitch * croll + syaw_cpitch * sroll;
    Q.y = syaw_cpitch * croll - cyaw_spitch * sroll;
    Q.z = cyaw_cpitch * sroll - syaw_spitch * croll;
    Q.w = cyaw_cpitch * croll + syaw_spitch * sroll;
}

////////////////////////////////////////////////////////////////////////////
// Getting Euler Angles From Quaternion
template <class T>
void CMQuatToAxisAngles(CMVector3D<T> &V, const CMQuaternion<T> &Q)
{
    T	r11, r21, r31, r32, r33, r12, r13;
	T	q00, q11, q22, q33;
	T	tmp;

	q00 = Q.w * Q.w;
	q11 = Q.x * Q.x;
	q22 = Q.y * Q.y;
	q33 = Q.z * Q.z;

	r11 = q00 + q11 - q22 - q33;
	r21 = (Q.x*Q.y + Q.w*Q.z) * T(2);
	r31 = (Q.x*Q.z - Q.w*Q.y) * T(2);
	r32 = (Q.y*Q.z + Q.w*Q.x) * T(2);
	r33 = q00 - q11 - q22 + q33;

	tmp = mabs(r31);
	if(tmp > T(0.999999))
	{
		r12 = (Q.x*Q.y - Q.w*Q.z) * T(2);
		r13 = (Q.x*Q.z + Q.w*Q.y) * T(2);

		V.x = mmultpi2(-r31/tmp);// pitch         //--0;  //roll
		V.y = MQUATATAN2(-r12, -r31*r13); // yaw  //--mmultpi2(-r31/tmp); // pitch
        V.z = 0; // roll                          //--MQUATATAN2(-r12, -r31*r13); // yaw
	}
    else
    {
	    V.x = MQUATASIN(-r31); // pitch           //--MQUATATAN2(r32, r33); // roll
	    V.y = MQUATATAN2(r21, r11); // yaw        //--MQUATASIN(-r31);		 // pitch
	    V.z = MQUATATAN2(r32, r33); // roll       //--MQUATATAN2(r21, r11); // yaw
    }
}

////////////////////////////////////////////////////////////////////////////
// Bulding Quaternion From Matix
template <class T>
void CMMtxToQuat(CMQuaternion<T> &Q, const CMMatrix44<T> &M)
{
    T one = T(1);
    T two = T(2);

    T trace = M._11 + M._22 + M._33 + one;
    T tracesqrt = MQUATSQRT(trace);

    Q.x = (M._23 - M._32)*tracesqrt*two;
    Q.y = (M._31 - M._13)*tracesqrt*two;
    Q.z = (M._12 - M._21)*tracesqrt*two;
    Q.w = tracesqrt/two;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
void CMMtxToQuat(CMQuaternion<T> &Q, const CMMatrix33<T> &M)
{
    T one = T(1);
    T two = T(2);

    T trace = M._11 + M._22 + M._33 + one;
    T tracesqrt = MQUATSQRT(trace);

    Q.x = (M._23 - M._32)*tracesqrt*two;
    Q.y = (M._31 - M._13)*tracesqrt*two;
    Q.z = (M._12 - M._21)*tracesqrt*two;
    Q.w = tracesqrt/two;
}

////////////////////////////////////////////////////////////////////////////
// Building Matrix From Quaternion
template <class T>
void CMQuatToMtx(CMMatrix44<T> &M, const CMQuaternion<T> &Q)
{
    T one = T(1);
    T two = T(2);

    memset(&M._11,0,sizeof(T)*16);

    T x2 = Q.x*Q.x;
    T y2 = Q.y*Q.y;
    T z2 = Q.z*Q.z;

    T xy = Q.x*Q.y;
    T wz = Q.w*Q.z;
    T wy = Q.w*Q.y;
    T xz = Q.x*Q.z;
    T wx = Q.w*Q.x;
    T yz = Q.y*Q.z;

    T *p = (T*)M;
    p[0] = one - two*(y2+z2);
    p[1] = two*(xy-wz);
    p[2] = two*(wy+xz);

    p[4] = two*(xy+wz);
    p[5] = one - two*(x2+z2);
    p[6] = two*(yz-wx);

    p[8] = two*(xz-wy);
    p[9] = two*(yz+wx);
    p[10]= one - two*(x2+y2);

    p[15]= one;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
void CMQuatToMtx(CMMatrix33<T> &M, const CMQuaternion<T> &Q)
{
    T one = T(1);
    T two = T(2);

    memset(&M._11,0,sizeof(T)*9);

    T x2 = Q.x*Q.x;
    T y2 = Q.y*Q.y;
    T z2 = Q.z*Q.z;

    T xy = Q.x*Q.y;
    T wz = Q.w*Q.z;
    T wy = Q.w*Q.y;
    T xz = Q.x*Q.z;
    T wx = Q.w*Q.x;
    T yz = Q.y*Q.z;

    T *p = (T*)M;
    p[0] = one - two*(y2+z2);
    p[1] = two*(xy-wz);
    p[2] = two*(wy+xz);

    p[3] = two*(xy+wz);
    p[4] = one - two*(x2+z2);
    p[5] = two*(yz-wx);

    p[6] = two*(xz-wy);
    p[7] = two*(yz+wx);
    p[8] = one - two*(x2+y2);
}

////////////////////////////////////////////////////////////////////////////
// Logarithm Of A Quaternion 
template <class T>
void CMQuatLog(CMQuaternion<T> &qOut, const CMQuaternion<T> &qIn)
{
    T a = MQUATACOS(qIn.w);
    T sina = MQUATSIN(a);

    qOut.w = 0;

    if(sina > 0)
    {
        qOut.x = a*qIn.x/sina;
        qOut.y = a*qIn.y/sina;
        qOut.z = a*qIn.z/sina;
    }
    else
    {
        qOut.x = qOut.y = qOut.z = 0;
    }
}


////////////////////////////////////////////////////////////////////////////
// Exponent Of A Quaternion
// Qexp(v*a) = [cos(a),vsin(a)]
template <class T>
void CMQuatExp(CMQuaternion<T> &qOut, const CMQuaternion<T> &qIn)
{
    T a = MQUATSQRT(qIn.x, qIn.y, qIn.z);
    T sina,cosa;

    MQUATSINCOS(a, sina, cosa);

    qOut.w = cosa;
    if(a > 0)
    {
        T inv = T(1)/a;
        qOut.x = sina * qIn.x*inv;
        qOut.y = sina * qIn.y*inv;
        qOut.z = sina * qIn.z*inv;
    }
    else
    {
        qOut.x = qOut.y = qOut.z = 0;
    }
}

////////////////////////////////////////////////////////////////////////////
// Linear Interpolation Between Two Quaternions
template <class T>
void CMQuatLerp(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2, T weight)
{
    qOut.x = Q1.x + ((Q2.x - Q1.x)*weight);
    qOut.y = Q1.y + ((Q2.y - Q1.y)*weight);
    qOut.z = Q1.z + ((Q2.z - Q1.z)*weight);
    qOut.w = Q1.w + ((Q2.w - Q1.w)*weight);
}


////////////////////////////////////////////////////////////////////////////
// Spherical Quaternion Interpolation
template <class T>
void CMQuatSLerp(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2, T weight)
{
    CMQuaternion<T> Q3;
    T dot;
    T one = T(1);

    // We Do A Dot Product To Check Angle Between Q1 And Q2
    dot = Q1.x*Q2.x + Q1.y*Q2.y + Q1.z*Q2.z + Q1.w*Q2.w;

    // If dot < 0 Then We Have To Invert One Quaternion To Reduce Spinning
    if(dot < 0)
    {
        dot = -dot;
		Q3 = -(Q2);
    }
    else
    {
        Q3 = Q2;
    }

    // Set The First And The Last Scale For The Interpolation
    T scale0 = one - weight;
    T scale1 = weight;

    // Check If The Angle Between Two Quaternions Was Big Enough To Make Such Calculation
    if (dot < T(0.95))
    {
        // Get The Angle Between The Two Quaternions, And Then Store Sinus Of That Angle
        T angle = MQUATACOS(dot);
        T sinA = MQUATSIN(angle);

        // Calculate The Scale For Q1 And Q2, According To The Angle And It's Sine Value
        scale0 = MQUATSIN((one - weight)*angle)/sinA;
        scale1 = MQUATSIN((weight*angle))/sinA;
    }

    // Calculate The x,y,z And w Values For The Quaternion By Using A Special Form Of Linear
    // Interpolation For The Quaternion
    qOut.x = (scale0*Q1.x) + (scale1*Q3.x);
    qOut.y = (scale0*Q1.y) + (scale1*Q3.y);
    qOut.z = (scale0*Q1.z) + (scale1*Q3.z);
    qOut.w = (scale0*Q1.w) + (scale1*Q3.w);
}

////////////////////////////////////////////////////////////////////////////
// This Version Of slerp, Used By squad, Does Not Check For Theta > 90
template <class T>
void CMQuatSLerpNoInv(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2, T weight)
{
    T dot = Q1.x*Q2.x + Q1.y*Q2.y + Q1.z*Q2.z + Q1.w*Q2.w;

    if (dot > T(-0.95) && dot < T(0.95))
    {
        T one = T(1);
        /////
        T angle = MQUATACOS(dot);
        T sinA = MQUATSIN(angle);
        /////
        T scale0 = MQUATSIN((one - weight)*angle)/sinA;
        T scale1 = MQUATSIN((weight*angle))/sinA;

        qOut.x = (scale0*Q1.x) + (scale1*Q2.x);
        qOut.y = (scale0*Q1.y) + (scale1*Q2.y);
        qOut.z = (scale0*Q1.z) + (scale1*Q2.z);
        qOut.w = (scale0*Q1.w) + (scale1*Q2.w);
    }
    else
    {
        CMQuatLerp(qOut,Q1,Q2,weight);
    }
}

////////////////////////////////////////////////////////////////////////////
// Spherical Cubic Interpolation
template <class T>
void CMQuatSQuad(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Q2, 
                 const CMQuaternion<T> &A, const CMQuaternion<T> &B, T weight)
{
    CMQuaternion<T> c,d;

    CMQuatSLerpNoInv(c,Q1,Q2,weight);
    CMQuatSLerpNoInv(d,A,B,weight);

    T one = T(1);
    T lrpweight = T(2)*weight*(one - weight);

    CMQuatSLerpNoInv(qOut,c,d,lrpweight);
}

////////////////////////////////////////////////////////////////////////////
// Spline Interpolation
template <class T>
void CMQuatSpline(CMQuaternion<T> &qOut, const CMQuaternion<T> &Q1, const CMQuaternion<T> &Qm, const CMQuaternion<T> &Q2)
{
    //qOut = qn*Qexp((Qlog(qni*qnm1)+Qlog(qni*qnp1))/-4);
    CMQuaternion<T> qni,qlog1,qlog2,q;

    CMQuatConjugate(qni,Qm);
    CMQuatMult(qlog1,qni,Q1);
    CMQuatMult(qlog2,qni,Q2);

    CMQuatLog(qlog1,qlog1);
    CMQuatLog(qlog2,qlog2);

    CMQuatAdd(q,qlog1,qlog2);

    q /= T(-4);

    CMQuatExp(q,q);
    CMQuatMult(qOut,Qm,q);
}