///////////////////////////////////////////////////////////////////////////////////////
//  3DMath.h
//
//  Functions Definitions That Operates In 3D Space
//  
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
#include "Plane.h"

#include "Algebra.h"

#include "3DMath.h"


///////////////////////////////////////////////////////////////////////////////////////
// Line Triangle Intersection Functions

/*
///////////////////////////////////////////////////////////////////////////////////////
// Calculates Points Of Intersection Of Line And The Triangle
template <class T>
boolt CMLineIntersectTriangle(CMVector3D<T> *pOut, CMVector3D<T> *p0, CMVector3D<T> *p1, CMVector3D<T> *p2,
                              CMVector3D<T> *point, CMVector3D<T> *dir)
{
    //ASSERT(pOut != NULL);
    ASSERT(p0 != NULL);
    ASSERT(p1 != NULL);
    ASSERT(p2 != NULL);
    ASSERT(point != NULL);
    ASSERT(dir != NULL);

    //Finds Vectors For Two Edges Sharing Vertex p0
    CMVector3D<T> edge1,edge2;
    CMVec3Sub(&edge1,p1,p0);
    CMVec3Sub(&edge2,p2,p0);

    //Begin Calculating Determinant - Also Used To Calculate U Parameter
    CMVector3D<T> pvec;
    CMVec3Cross(&pvec,dir,&edge2);

    //If Determinant Is Near Zero, Ray Lies On Triangle Plane
    T det = CMVec3Dot(&edge1,&pvec);

    if (mnear0(det))
        return false;

    T one = T(1);
    T invdet = one/det;

    // Calculate Vector From Vertex0 To Ray Origin
    CMVector3D<T> tvec;
    CMVec3Sub(&tvec,point,p0);

    // Calculate U Parameter And Test Bounds
    T u = CMVec3Dot(&tvec,&pvec)*invdet;
    if (mless0(u) || mgre0(u-one))
        return false;

    // Prepare To Test V Parameter
    CMVector3D<T> qvec;
    CMVec3Cross(&qvec,&tvec,&edge1);

    // Calculate v Parameter And Test Bounds
    T v = CMVec3Dot(dir,&qvec)*invdet;
    if(mless0(v) || mgre0(u+v-one))
       return false;

    // Calculate t, ray Intersects Triangle
    if (pOut != NULL)
    {
        T t = CMVec3Dot(&edge2,&qvec)*invdet;
        *pOut = *point + *dir*t;
    }

    return true;
}
*/
/*
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMRayIntersectTriangle(CMVector3D<T> *pOut, const CMVector3D<T> *p0, const CMVector3D<T> *p1,
                             const CMVector3D<T> *p2, const CMVector3D<T> *pOrigin, const CMVector3D<T> *pDir, 
                             boolt twoSided)
{
    ASSERT(p0 != NULL);
    ASSERT(p1 != NULL);
    ASSERT(p2 != NULL);
    ASSERT(pOrigin != NULL);
    ASSERT(pDir != NULL);


    //Finds Vectors For Two Edges Sharing Vertex p0
    CMVector3D<T> edge1,edge2;
    CMVec3Sub(&edge1,p1,p0);
    CMVec3Sub(&edge2,p2,p0);

    //Begin Calculating Determinant - Also Used To Calculate U Parameter
    CMVector3D<T> pvec;
    CMVec3Cross(&pvec,pDir,&edge2);

    //If Determinant Is Near Zero, Ray Lies On Triangle Plane
    T det = CMVec3Dot(&edge1,&pvec);

    if (mnear0(det))
        return false;

    CMVector3D<T> normal;
    CMVec3Cross(&normal,&edge1,&edge2);

    //det = mabs(det);
    T one = T(1);
    T invdet = one/det;

    det = mabs(det);

    // Calculate Vector From Vertex0 To Ray Origin
    CMVector3D<T> tvec;
    CMVec3Sub(&tvec,pOrigin,p0);

    //Checking In Which Side The Origin Is Situated
    T dp = CMVec3Dot(&normal,&tvec);
    if (mless0(dp))
    {
        if (twoSided)
        {
            if (mless0(CMVec3Dot(&normal,pDir)))
                return false;
        }
        else
            return false;
    }
    else
    {
        if (mgre0(CMVec3Dot(&normal,pDir)))
            return false;
    }


    // Calculate U Parameter And Test Bounds
    T u = CMVec3Dot(&tvec,&pvec)*invdet;
    if(mless0(u) || u > det)
        return false;

    // Prepare To Test V Parameter
    CMVector3D<T> qvec;
    CMVec3Cross(&qvec,&tvec,&edge1); //edge1

    // Calculate v Parameter And Test Bounds
    T v = CMVec3Dot(pDir,&qvec)*invdet;
    if(mless0(v) || u + v > det)
        return false;

    // Calculate t, ray Intersects Triangle
    if (pOut != NULL)
    {
        T t = CMVec3Dot(&edge2,&qvec)*invdet;
        *pOut = *pOrigin + *pDir*t;
    }

    return true;
}
*/

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMIsPointInsideTriangle(const CMVector3D<T> &point, const CMVector3D<T> &vertex1,
                              const CMVector3D<T> &vertex2, const CMVector3D<T> &vertex3)
{
    const CMVector3D<T> vtx[3] = {vertex1,vertex2,vertex3};

    CMVector3D<T> A,B;
    CMVec3Sub(A,vtx[1],vtx[0]);
    CMVec3Sub(B,vtx[2],vtx[1]);

    CMVector3D<T> vN;
    CMVec3Cross(vN,A,B);

    CMVector3D<T> vE,vH,vF;

    for(int32t j = 2, i = 0; i < 3; j = i, i++)
    {
        CMVec3Sub(vE,vtx[i],vtx[j]); // Edge Direction
        CMVec3Sub(vH,point,vtx[j]); // Edge Start Towards Point
        CMVec3Cross(vF,vE,vN); // Cross Product. Normal Of Edge's Half Plane
        T d = CMVec3Dot(vH,vF); // Dor Product. Sign Of Point From Edge's Half Plane.

        if (mless0(d)) // Point On Negative Half-Plane (Outside)
            return false;
    }
    

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
 

///////////////////////////////////////////////////////////////////////////////////////
/* // nie jest funkcja wydajna
template <class T>
boolt CMIsPointInsideTriangle(const CMVector3D<T> *pPoint, const CMVector3D<T> *pVertex1,
                              const CMVector3D<T> *pVertex2, const CMVector3D<T> *pVertex3)
{
    ASSERT(pPoint != NULL);
    ASSERT(pVertex1 != NULL);
    ASSERT(pVertex2 != NULL);
    ASSERT(pVertex3 != NULL);

    CMVector3D<T> vA,vB;
    T angle = 0;

    //////
    CMVec3Sub(&vA,pVertex1,pPoint);
    CMVec3Sub(&vB,pVertex2,pPoint);
    angle += CMVec3Angle(&vA,&vB);
    //////
    CMVec3Sub(&vA,pVertex2,pPoint);
    CMVec3Sub(&vB,pVertex3,pPoint);
    angle += CMVec3Angle(&vA,&vB);
    //////
    CMVec3Sub(&vA,pVertex3,pPoint);
    CMVec3Sub(&vB,pVertex1,pPoint);
    angle += CMVec3Angle(&vA,&vB);
    //////

    if( mgreq0(angle - mmult2pi(T(0.99)) ) ) //0.99 - Match Factor
    {
        return true;
    }

    return false;
}
*/
/*
//Poprawiac funkcje wyzej

---------------------
bool PointInTriangle(Vector P, Vector* V)
{
    Vector N = (V[1] - V[0]) ^ (V[2] - V[1]); // cross product
    for(int j = 2, i = 0; i < 3; j = i; i ++)
    {
        Vector E(V[i] - V[j]); // edge direction
        Vector H(P    - V[j]); // edge start towards point
        Vector F(E ^ N); // cross product. Normal of edge's half plane
        float  d(H * F); // dot   product. sign of point from edge's half plane

        if (d < 0.0f) // point on negative half-plane (outside)
        {
            return false;
        }
    }
    return true;
}
---------------------
D - is direction
bool PointInTriangle(Vector P, Vector D, Vector* V)
{
    for(int j = 2, i = 0; i < 3; j = i; i ++)
    {
        Vector E(V[i] - V[j]); // edge direction
        Vector H(P    - V[j]); // edge start towards point
        Vector F(E ^ D); // cross product. Normal of edge's half plane
        float  d(H * F); // dot   product. sign of point from edge's half plane

        // since D = -N from the Point in triangle function, 
        // we need to test for POSITIVE half planes.
        if (d > 0.0f) // point on positive half-plane (outside)
        {
            return false;
        }
    }
    return true;
}

---------------------

*/

///////////////////////////////////////////////////////////////////////////////////////
// Calculates Points Of Intersection Of Line And The Triangle
template <class T>
boolt CMLineTriangleIntersectPt(CMVector3D<T> &vOut, const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2, 
                              const CMVector3D<T> &vertex3, const CMVector3D<T> &point, const CMVector3D<T> &dir)
{
    CMPlane<T> plane;
    CMPlaneSet(plane,vertex1,vertex2,vertex3);

    CMVector3D<T> intersectionPoint; 
    boolt intersect = CMPlaneLineIntersect(intersectionPoint,plane,point,dir);

    if (intersect)
    {
        CMVector3D<T> vA,vB;
        T angle = 0;

        if (CMIsPointInsideTriangle(intersectionPoint,vertex1,vertex2,vertex3))
        {
            //if(pOut)
            //    *pOut = *intersectionPoint;
            vOut = intersectionPoint;

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// Calculate If Line Intersect Triangle
template <class T>
boolt CMIsLineIntersectTriangle(const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2, 
                              const CMVector3D<T> &vertex3, const CMVector3D<T> &point, const CMVector3D<T> &dir)
{
    CMPlane<T> plane;
    CMPlaneSet(plane,vertex1,vertex2,vertex3);

    CMVector3D<T> intersectionPoint; 
    boolt intersect = CMPlaneLineIntersect(intersectionPoint,plane,point,dir);

    if (intersect)
    {
        CMVector3D<T> vA,vB;
        T angle = 0;

        if (CMIsPointInsideTriangle(intersectionPoint,vertex1,vertex2,vertex3))
            return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMRayTriangleIntersectPt(CMVector3D<T> &vOut, const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                             const CMVector3D<T> &vertex3, const CMVector3D<T> &origin, const CMVector3D<T> &dir, 
                             boolt twoSided)
{
    CMPlane<T> plane; 
    CMPlaneSet(plane,vertex1,vertex2,vertex3);

    CMVector3D<T> tVec;
    CMVector3D<T> intersectionPoint; 

    boolt intersect = CMIsRayIntersectPlane(plane,origin,dir,twoSided);

    if (intersect)
    {
        CMVector3D<T> vA,vB;
        T angle = 0;

        if (CMIsPointInsideTriangle(intersectionPoint,vertex1,vertex2,vertex3))
        {
            vOut = intersectionPoint;

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMIsRayIntersectTriangle(const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                             const CMVector3D<T> &vertex3, const CMVector3D<T> &orig, const CMVector3D<T> &dir, 
                             boolt twoSided)
{
    CMPlane<T> plane; 
    CMPlaneSet(plane,vertex1,vertex2,vertex3);

    CMVector3D<T> intersectionPoint; 

    boolt intersect = CMIsRayIntersectPlane(plane,orig,dir,twoSided);

    if (intersect)
    {
        CMVector3D<T> vA,vB;
        T angle = 0;

        if (CMIsPointInsideTriangle(intersectionPoint,vertex1,vertex2,vertex3))
            return true;
    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMSegmentIntersectTriangle(CMVector3D<T> &vOut, const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                                 const CMVector3D<T> &vertex3, const CMVector3D<T> &segPoint1, 
                                 const CMVector3D<T> &segPoint2)
{
    CMPlane<T> plane; 
    CMPlaneSet(plane,vertex1,vertex2,vertex3);

    CMVector3D<T> tVec;
    CMVector3D<T> intersectionPoint;// = &tVec;

    boolt intersect = CMPlaneSegmentIntersect(vOut,plane,segPoint1,segPoint2); 

    if (intersect)
    {
        CMVector3D<T> vA,vB;
        T angle = 0;

        if (CMIsPointInsideTriangle(intersectionPoint,vertex1,vertex2,vertex3))
        {
            vOut = intersectionPoint;

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMIsSegmentIntersectTriangle(const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                                 const CMVector3D<T> &vertex3, const CMVector3D<T> &segPoint1, 
                                 const CMVector3D<T> &segPoint2)
{
    CMPlane<T> plane; 
    CMPlaneSet(plane,vertex1,vertex2,vertex3);

    CMVector3D<T> tVec;
    CMVector3D<T> intersectionPoint;

    boolt intersect = CMPlaneSegmentIntersect(tVec,plane,segPoint1,segPoint2); 

    if (intersect)
    {
        CMVector3D<T> vA,vB;
        T angle = 0;

        if (CMIsPointInsideTriangle(intersectionPoint,vertex1,vertex2,vertex3))
            return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
void CMTriangleNormal(CMVector3D<T> &vNorm, const CMVector3D<T> &p0, const CMVector3D<T> &p1,
                      const CMVector3D<T> &p2, boolt bNormalize)
{
    CMVector3D<T> dir1, dir2;

    CMVec3Sub(dir1,p1,p0);
    CMVec3Sub(dir2,p2,p0);

    CMVec3Cross(vNorm,dir1,dir2);

    if (bNormalize)
        CMVec3Normalize(vNorm,vNorm);
}

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
void CMClosestPointOnLine(CMVector3D<T> &vOut, const CMVector3D<T> &A, const CMVector3D<T> &B,
                             const CMVector3D<T> &point, boolt SegmentClamp)
{
    CMVector3D<T> AP = point - A;
    CMVector3D<T> AB = B - A;

    T ab2 = AB.x*AB.x + AB.y*AB.y;
    T ap_ab = AP.x*AB.x + AP.y*AB.y;
    T t = ap_ab / ab2;

    if (SegmentClamp)
    {
        T one = T(1);
        if(mless0(t))
            t = 0;
        else if (mgre0(t-one))
            t = one;
    }

    CMVector3D<T> Closest = A + AB * t;
    vOut = Closest;
}
