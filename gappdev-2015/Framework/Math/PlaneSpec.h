///////////////////////////////////////////////////////////////////////////////////////
//  PlaneSpec.h
//
//  Plane Classes Templates Specialization Declarations
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLANESPEC_H_
#define _PLANESPEC_H_

///////////////////////////////////////////////////////////////////////////////////////
// Builds Plane From Three Points
template void CMPlaneSet(CMPlane<float> &plane, const CMVector3D<float> &p1, const CMVector3D<float> &p2,
                                    const CMVector3D<float> &p3);

///////////////////////////////////////////////////////////////////////////////////////
// Builds Plane From Normal And Offset Point
template void CMPlaneSet(CMPlane<float> &plane, const CMVector3D<float> &normal, const CMVector3D<float> &point);

///////////////////////////////////////////////////////////////////////////////////////
// Distance From Plane To Point Located In Space
template float CMPlaneDistance(const CMPlane<float> &plane, const CMVector3D<float> &vec);

///////////////////////////////////////////////////////////////////////////////////////
// Normalizes The Plane Coefficients So That The Plane Normal Has Unit Length
template void CMPlaneNormalize(CMPlane<float> &out, const CMPlane<float> &in);

///////////////////////////////////////////////////////////////////////////////////////
// Angle Of Intersection Of The Two Planes
template float CMPlaneIntersectionAngle(const CMPlane<float> &plane1, const CMPlane<float> &plane2);

///////////////////////////////////////////////////////////////////////////////////////
template boolt CMPlaneLineIntersect(CMVector3D<float> &vOut, const CMPlane<float> &plane, const CMVector3D<float> &point, 
                        const CMVector3D<float> &dir);

///////////////////////////////////////////////////////////////////////////////////////
template boolt CMPlaneLineIntersectByPoints(CMVector3D<float> &vOut, const CMPlane<float> &plane, 
                                            const CMVector3D<float> &point1, const CMVector3D<float> &point2);

///////////////////////////////////////////////////////////////////////////////////////
template 
boolt CMPlaneRayIntersectPt(CMVector3D<float> &vOut, const CMPlane<float> &plane, const CMVector3D<float> &orig,
                             const CMVector3D<float> &dir, boolt checkTwoSide);
///////////////////////////////////////////////////////////////////////////////////////
template 
boolt CMIsRayIntersectPlane(const CMPlane<float> &plane, const CMVector3D<float> &orig,
                             const CMVector3D<float> &dir, boolt checkTwoSide);

///////////////////////////////////////////////////////////////////////////////////////
template boolt CMPlaneIntersect(CMVector3D<float> &vOut, const CMPlane<float> &plane1, const CMPlane<float> &plane2,
                       const CMPlane<float> &plane3);

///////////////////////////////////////////////////////////////////////////////////////
template boolt CMPlaneSegmentIntersect(CMVector3D<float> &vOut, const CMPlane<float> &plane, const CMVector3D<float> &p1,
                                        const CMVector3D<float> &p2);

///////////////////////////////////////////////////////////////////////////////////////
// Orthogonal Projection 3D Vector On The Plane
template void CMPlaneOrthoProject(CMVector3D<float> &vOut, const CMPlane<float> &plane, const CMVector3D<float> &point);

#endif //_PLANESPEC_H_
