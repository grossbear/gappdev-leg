///////////////////////////////////////////////////////////////////////////////////////
//  Plane.cpp
//
//  Plane Class Templates Methods Definitions
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
#include "Plane.h"
#include "Matrix.h"


#include "Algebra.h"


///////////////////////////////////////////////////////////////////////////////////////
// Builds Plane From Three Points
template <class T>
void CMPlaneSet(CMPlane<T> &plane, const CMVector3D<T> &p1, const CMVector3D<T> &p2,
                                    const CMVector3D<T> &p3)
{
    CMVector3D<T> vec1 = p2 - p1;
    CMVector3D<T> vec2 = p3 - p1;

    CMVector3D<T> *pvec = (CMVector3D<T>*)plane;

    CMVec3Cross(*pvec,vec1,vec2);
    CMVec3Normalize(*pvec,*pvec);

    plane.d = -CMVec3Dot(*pvec,p1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Builds Plane From Normal And Offset Point
template <class T>
void CMPlaneSet(CMPlane<T> &plane, const CMVector3D<T> &normal, const CMVector3D<T> &point)
{
    plane.a = normal.x;
    plane.b = normal.y;
    plane.c = normal.z;

    plane.d = -CMVec3Dot(point,normal);
}

///////////////////////////////////////////////////////////////////////////////////////
// Distance From Plane To Point Located In Space
template <class T>
T CMPlaneDistance(const CMPlane<T> &plane, const CMVector3D<T> &vec)
{
    return plane.a*vec.x + plane.b*vec.y + plane.c*vec.z + plane.d;
}

///////////////////////////////////////////////////////////////////////////////////////
// Normalizes The Plane Coefficients So That The Plane Normal Has Unit Length
template <class T>
void CMPlaneNormalize(CMPlane<T> &out, const CMPlane<T> &in)
{
    T revsq = MPLANEREVSQRT(in.a, in.b, in.c);

    out.a = in.a*revsq;
    out.b = in.b*revsq;
    out.c = in.c*revsq;
}

///////////////////////////////////////////////////////////////////////////////////////
// Angle Of Intersection Of The Two Planes
template <class T>
T CMPlaneIntersectionAngle(const CMPlane<T> &plane1, const CMPlane<T> &plane2)
{
    return CMVec3Angle((CMVector3D<T>&)plane1, (CMVector3D<T>&)plane2);
}


///////////////////////////////////////////////////////////////////////////////////////
// Finds Point Of Intersection Of Plane And Line
// Return False If Line Do Not Intersect A Plane (Is Parallel To Plane)
// dir Must Be Normalized Vector
// Result Vector Could Be NULL
template <class T>
boolt CMPlaneLineIntersect(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &point, 
                        const CMVector3D<T> &dir)
{
    T dot = CMVec3Dot((CMVector3D<T>&)plane, dir);

    if (mnear0(dot))
        return false;

    //if (pOut != NULL)
    {
        T t = -(point.x*plane.a + point.y*plane.b + point.z*plane.c + plane.d);

        CMVector3D<T> res = dir;
        res *= (t/dot);
        res += point;

        vOut = res;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculates Points Of Intersection Line With Plane
// Constructs Line From Given Two Points
// Result Vector Could Be NULL
template <class T>
boolt CMPlaneLineIntersectByPoints(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &point1,
                           const CMVector3D<T> &point2)
{
    CMVector3D<T> dir;
    CMVec3Sub(dir,point1,point2);

    T lenght = CMVec3Length(dir);
    if (mnear0(lenght))// Check If Points Are Not To Close To Each Other
    {
        ASSERT(false);
        return false;
    }
    
    T one = T(1);
    T inv = one/lenght;
    dir.x *= lenght;
    dir.y *= lenght;
    dir.z *= lenght;


    return CMPlaneLineIntersect(vOut,plane,point1,dir);
}


///////////////////////////////////////////////////////////////////////////////////////
// Finds Points Of Intersection Of Ray And Plane
// dir Must Be Normalized
template <class T>
boolt CMPlaneRayIntersectPt(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &orig,
                             const CMVector3D<T> &dir, boolt checkTwoSide)
{
    T dot = CMVec3Dot((CMVector3D<T>&)plane, dir);

    if (mnear0(dot))
        return false;


    T t = plane.a*orig.x + plane.b*orig.y + plane.c*orig.z + plane.d;

    if (mlesseq0(t))
    {
        if (!checkTwoSide)
        {
            return false;
        }
        else
        {
            if (mlesseq0(dot))
            {
                return false;
            }
        }
    }
    else
    {
        if (mgre0(dot))
            return false;
    }

    CMVector3D<T> res = dir;
    res *= (-t/dot);
    res += orig;

    vOut = res;
   
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Check If Ray Intersect Plane
template <class T>
boolt CMIsRayIntersectPlane(const CMPlane<T> &plane, const CMVector3D<T> &orig,
                             const CMVector3D<T> &dir, boolt checkTwoSide)
{
    T dot = CMVec3Dot((CMVector3D<T>&)plane, dir);

    if (mnear0(dot))
        return false;


    T t = plane.a*orig.x + plane.b*orig.y + plane.c*orig.z + plane.d;

    if (mlesseq0(t))
    {
        if (!checkTwoSide)
        {
            return false;
        }
        else
        {
            if (mlesseq0(dot))
            {
                return false;
            }
        }
    }
    else
    {
        if (mgre0(dot))
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Finds Point Of Intersection Of Line Segment And Plane
template <class T>
boolt CMPlaneSegmentIntersect(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &p1,
                                const CMVector3D<T> &p2)
{
    T distance1 = CMPlaneDistance(plane,p1);
    T distance2 = CMPlaneDistance(plane,p2);

    if (mgreq0(distance1*distance2))
        return false;

    CMVector3D<T> dir;
    CMVec3Sub(dir,p2,p1);
    CMVec3Normalize(dir,dir);

    return CMPlaneLineIntersectByPoints(vOut,plane,p1,p2);
}


///////////////////////////////////////////////////////////////////////////////////////
// Calculate 3D Point Of The Intersection Of Three Planes
// Returns False If There Were No Intersection Of Planes
template <class T>
boolt CMPlaneIntersect(CMVector3D<T> &vOut, const CMPlane<T> &plane1, const CMPlane<T> &plane2,
                       const CMPlane<T> &plane3)
{
    CMVector3D<T> crossVec;

    // Scalar Triple Product
    CMVec3Cross(crossVec,(CMVector3D<T>&)plane2,(CMVector3D<T>&)plane3);
    T denom = CMVec3Dot((CMVector3D<T>&)plane1,crossVec);

    //Check denomnator. If Zero No Intersection
    if(mnear0(denom))
        return false;

    CMVector3D<T> temp1,temp2,temp3;

    CMVec3Cross(temp1,(CMVector3D<T>&)plane2,(CMVector3D<T>&)plane3);
    temp1 *= plane1.d;

    CMVec3Cross(temp2,(CMVector3D<T>&)plane3,(CMVector3D<T>&)plane1);
    temp2 *= plane2.d;

    CMVec3Cross(temp3,(CMVector3D<T>&)plane1,(CMVector3D<T>&)plane2);
    temp3 *= plane3.d;

    CMVector3D<T> res,temp23;

    CMVec3Add(temp23,temp2,temp3);
    CMVec3Add(res,temp1,temp23);

    res /= minvert(denom);

    vOut = res;
    
    return true;
}
      

///////////////////////////////////////////////////////////////////////////////////////
// Orthogonal Projection 3D Vector On The Plane
template <class T>
void CMPlaneOrthoProjection(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &point)
{
    T len2 = CMVec3LengthSq(*(CMVector3D<T>*)plane);
    ASSERT(!mnear0(len2));

    T t = -(plane.a*point.x + plane.b*point.y + plane.c*point.z + plane.d)/len2;

    CMVector3D<T> temp(plane[0]);
    temp *= t;
    CMVec3Add(vOut,point,temp);
}
