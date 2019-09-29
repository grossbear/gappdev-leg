///////////////////////////////////////////////////////////////////////////////////////
//  Plane.h
//
//  Plane Class Template Declaration
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLANE_H_
#define _PLANE_H_


///////////////////////////////////////////////////////////////////////////////////////
// Plane Class Declaration
template <class T>
class CMPlane
{
public:
    union{
        struct{
            T a,b,c,d;
        };
        T p[4];
    };

public:
    // Constructors
    CMPlane();
    CMPlane(const CMPlane<T> &plane);
    CMPlane(const CMVector3D<T> &normal, T offset);
    CMPlane(const CMVector4D<T> &v);
    CMPlane(const T* const ptr);
    CMPlane(T a, T b, T c, T d);
    CMPlane(const CMVector3D<T> &p1, const CMVector3D<T> &p2, const CMVector3D<T> &p3);
    CMPlane(const CMVector3D<T> &normal, const CMVector3D<T> &point);
    
    

    // Cast To Pointer  (T *)
    operator T* () const;
	operator const T* () const;

    T operator [] (int32t );
 
    // Cast To CMVector3D Type
    operator  CMVector3D<T>();
    operator  CMVector3D<T>&();
    operator  CMVector3D<T>*();

    // Cast To CMVector4D Type
    operator  CMVector4D<T>();
    operator  CMVector4D<T>*();

    // Assignment Operators
    CMPlane& operator =  ( const CMPlane &plane );

    // Unary Operators
    CMPlane operator + () const;
    CMPlane operator - () const;
};

// Inline Function

///////////////////////////////////////////////////////////////////////////////////////
// Invert Plane
template <class T>
void CMPlaneInvert(CMPlane<T> &vOut, const CMPlane<T> &vIn);


// Non Inline Functions

///////////////////////////////////////////////////////////////////////////////////////
// Builds Plane From Three Points
template <class T>
void CMPlaneSet(CMPlane<T> &plane, const CMVector3D<T> &p1, const CMVector3D<T> &p2,
                                    const CMVector3D<T> &p3);

///////////////////////////////////////////////////////////////////////////////////////
// Builds Plane From Normal And Offset Point
template <class T>
void CMPlaneSet(CMPlane<T> &plane, const CMVector3D<T> &normal, const CMVector3D<T> &point);

///////////////////////////////////////////////////////////////////////////////////////
// Distance From Plane To Point Located In Space
template <class T>
T CMPlaneDistance(const CMPlane<T> &plane, const CMVector3D<T> &point);

///////////////////////////////////////////////////////////////////////////////////////
// Normalizes The Plane Coefficients So That The Plane Normal Has Unit Length
template <class T>
void CMPlaneNormalize(CMPlane<T> &out, const CMPlane<T> &in);

///////////////////////////////////////////////////////////////////////////////////////
// Angle Of Intersection Of The Two Planes
template <class T>
T CMPlaneIntersectionAngle(const CMPlane<T> &plane1, const CMPlane<T> &plane2);

///////////////////////////////////////////////////////////////////////////////////////
// Finds Point Of Intersection Of Plane And Line
// Return False If Line Do Not Intersect A Plane (Is Parallel To Plane)
// dir Must Be Normalized Vector
template <class T>
boolt CMPlaneLineIntersect(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &point, 
                        const CMVector3D<T> &dir);

///////////////////////////////////////////////////////////////////////////////////////
// Calculates Points Of Intersection Line With Plane
// Constructs Line From Given Two Points
template <class T>
boolt CMPlaneLineIntersectByPoints(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &point1,
                           const CMVector3D<T> &point2);

///////////////////////////////////////////////////////////////////////////////////////
// Finds Points Of Intersection Of Ray And Plane
// dir Must Be Normalized
template <class T>
boolt CMPlaneRayIntersectPt(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &orig,
                             const CMVector3D<T> &dir, boolt checkTwoSide = false);

///////////////////////////////////////////////////////////////////////////////////////
// Check If Ray Intersect Plane
template <class T>
boolt CMIsRayIntersectPlane(const CMPlane<T> &plane, const CMVector3D<T> &orig,
                             const CMVector3D<T> &dir, boolt checkTwoSide = false);

///////////////////////////////////////////////////////////////////////////////////////
// Finds Point Of Intersection Of Line Segment And Plane
template <class T>
boolt CMPlaneSegmentIntersect(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &p1,
                                const CMVector3D<T> &p2);

///////////////////////////////////////////////////////////////////////////////////////
// Calculate 3D Point Of The Intersection Of Three Planes
// Returns False If There Were No Intersection Of Planes
template <class T>
boolt CMPlaneIntersect(CMVector3D<T> &vOut, const CMPlane<T> &plane1, const CMPlane<T> &plane2,
                       const CMPlane<T> &plane3);

///////////////////////////////////////////////////////////////////////////////////////
// Orthogonal Projection 3D Vector On The Plane
template <class T>
void CMPlaneOrthoProject(CMVector3D<T> &vOut, const CMPlane<T> &plane, const CMVector3D<T> &point);


#include "Plane.inl"
#include "PlaneSpec.h"

#endif //_PLANE_H_