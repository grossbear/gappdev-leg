///////////////////////////////////////////////////////////////////////////////////////
//  3DMath.h
//
//  Functions That Operates In 3D Space
//  
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _3DMATH_H_
#define _3DMATH_H_

///////////////////////////////////////////////////////////////////////////////////////
// Line Triangle Intersection Functions

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMIsLineIntersectTriangle(const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2, 
                              const CMVector3D<T> &vertex3, const CMVector3D<T> &point, const CMVector3D<T> &dir);
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMLineTriangleIntersectPt(CMVector3D<T> &vOut, const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2, 
                              const CMVector3D<T> &vertex3, const CMVector3D<T> &point, const CMVector3D<T> &dir);

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMRayTriangleIntersectPt(CMVector3D<T> &vOut, const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                             const CMVector3D<T> &vertex3, const CMVector3D<T> &orig, const CMVector3D<T> &dir, 
                             boolt twoSided = false);
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMIsRayIntersectTriangle(const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                             const CMVector3D<T> &vertex3, const CMVector3D<T> &orig, const CMVector3D<T> &dir, 
                             boolt twoSided = false);

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMSegmentTriangleIntersectPt(CMVector3D<T> &vOut, const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                                 const CMVector3D<T> &vertex3, const CMVector3D<T> &segPoint1, 
                                 const CMVector3D<T> &segPoint2);

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMIsSegmentIntersectTriangle(const CMVector3D<T> &vertex1, const CMVector3D<T> &vertex2,
                                 const CMVector3D<T> &vertex3, const CMVector3D<T> &segPoint1, 
                                 const CMVector3D<T> &segPoint2);

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
boolt CMIsPointInsideTriangle(const CMVector3D<T> &point, const CMVector3D<T> &vertex1,
                              const CMVector3D<T> &vertex2, const CMVector3D<T> &vertex3);

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
void CMTriangleNormal(CMVector3D<T> &norm, const CMVector3D<T> &p1, const CMVector3D<T> &p2,
                      const CMVector3D<T> &p3, boolt bNormalize = true);

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
void CMClosestPointOnLine(CMVector3D<T> &vOut, const CMVector3D<T> &A, const CMVector3D<T> &B,
                             const CMVector3D<T> &point, boolt SegmentClamp = true);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

#include "3DMathSpec.h"


#endif //_3DMATH_H_
