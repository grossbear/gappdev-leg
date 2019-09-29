///////////////////////////////////////////////////////////////////////////////////////
//  3DMathSpec.h
//
//  3D Math Functions Specification
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _3DMATHSPEC_H_
#define _3DMATHSPEC_H_

///////////////////////////////////////////////////////////////////////////////////////
// Line Triangle Intersection Functions

///////////////////////////////////////////////////////////////////////////////////////
template boolt CMIsPointInsideTriangle(const CMVector3D<float> &point, const CMVector3D<float> &vertex1,
                                        const CMVector3D<float> &vertex2, const CMVector3D<float> &vertex3);

///////////////////////////////////////////////////////////////////////////////////////
template boolt CMIsLineIntersectTriangle(const CMVector3D<float> &vertex1, 
                                       const CMVector3D<float> &vertex2, const CMVector3D<float> &vertex3,
                                       const CMVector3D<float> &point, const CMVector3D<float> &dir);
///////////////////////////////////////////////////////////////////////////////////////
template boolt CMLineTriangleIntersectPt(CMVector3D<float> &vOut, const CMVector3D<float> &vertex1, 
                                       const CMVector3D<float> &vertex2, const CMVector3D<float> &vertex3,
                                       const CMVector3D<float> &point, const CMVector3D<float> &dir);

///////////////////////////////////////////////////////////////////////////////////////
template boolt CMRayTriangleIntersectPt(CMVector3D<float> &vOut, const CMVector3D<float> &vertex1, 
                                      const CMVector3D<float> &vertex2, const CMVector3D<float> &vertex3, 
                                      const CMVector3D<float> &origin, const CMVector3D<float> &dir, 
                                        boolt twoSided);
///////////////////////////////////////////////////////////////////////////////////////
template boolt CMIsRayIntersectTriangle(const CMVector3D<float> &vertex1, 
                                      const CMVector3D<float> &vertex2, const CMVector3D<float> &vertex3, 
                                      const CMVector3D<float> &origin, const CMVector3D<float> &dir, 
                                        boolt twoSided);

///////////////////////////////////////////////////////////////////////////////////////
template 
boolt CMIsSegmentIntersectTriangle(const CMVector3D<float> &vertex1, const CMVector3D<float> &vertex2,
                                 const CMVector3D<float> &vertex3, const CMVector3D<float> &segPoint1, 
                                 const CMVector3D<float> &segPoint2);
///////////////////////////////////////////////////////////////////////////////////////
template 
boolt CMIsPointInsideTriangle(const CMVector3D<float> &point, const CMVector3D<float> &vertex1,
                              const CMVector3D<float> &vertex2, const CMVector3D<float> &vertex3);

///////////////////////////////////////////////////////////////////////////////////////
template void CMTriangleNormal(CMVector3D<float> &norm, const CMVector3D<float> &p0, 
                               const CMVector3D<float> &p1, const CMVector3D<float> &p2, boolt bNormalize);

///////////////////////////////////////////////////////////////////////////////////////
template void CMClosestPointOnLine(CMVector3D<float> &vOut, const CMVector3D<float> &A, const CMVector3D<float> &B,
                             const CMVector3D<float> &point, boolt SegmentClamp);

#endif //_3DMATHSPEC_H_