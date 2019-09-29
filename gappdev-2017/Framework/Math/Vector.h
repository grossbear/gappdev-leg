///////////////////////////////////////////////////////////////////////////////////////
//  Vector.h
//
//  Vector Classes Templates Declarations
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMVECTOR2D_
#define _CMVECTOR2D_

template <class T> 
// 2D Vector Class Declaration
class CMVector2D
{
public:
    //Member Variables
    union {
        struct{
            T x, y;
        };
        T v[2];
    };

public:
    //Constructor Declarations
    CMVector2D();
    CMVector2D(const T  * const p);
    CMVector2D(T x, T y);
    CMVector2D(const CMVector2D & v);

    //Cast To Pointer  (T *)
    operator T* () const;
	operator const T* () const;

    // Assignment Operators
    CMVector2D& operator =  ( const CMVector2D & );
    CMVector2D& operator += ( const CMVector2D & );
    CMVector2D& operator -= ( const CMVector2D & );
    CMVector2D& operator *= ( const T & );
    CMVector2D& operator /= ( const T & );

    // Unary Operators
    CMVector2D operator + () const;
    CMVector2D operator - () const;

    T operator [] (int32t );

    // Binary Operators
    CMVector2D operator + ( const CMVector2D & ) const;
    CMVector2D operator - ( const CMVector2D & ) const;
    CMVector2D operator * ( const T & ) const;
    CMVector2D operator / ( const T & ) const;

    boolt operator == ( const CMVector2D & ) const;
    boolt operator != ( const CMVector2D & ) const;

};

//---------------------------
// Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two 2D Vectors
template <class T>
void CMVec2Add(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Two 2D Vectors
template <class T>
void CMVec2Sub(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Dot Product Between Two 2D Vectors
template <class T>
T CMVec2Dot(const CMVector2D<T> &v1, const CMVector2D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Returns the z-component by taking the cross product of two 2D vectors.
template <class T>
T CMVec2CCW(const CMVector2D<T> &v1, const CMVector2D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Linear Interpolation Between Two 2D Vectors
template <class T>
void CMVec2Lerp(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, T weight);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 2D Vector That Is Made Up Of The Largest Components Of Two 2D Vectors
template <class T>
void CMVec2Max(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 2D Vector That Is Made Up Of The Smallest Components Of Two 2D Vectors
template <class T>
void CMVec2Min(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Scales A 2D Vector
template <class T>
void CMVec2Scale(CMVector2D<T> &vOut, const CMVector2D<T> &vIn, T scale);

///////////////////////////////////////////////////////////////////////////////////////
// Invert 2D Vector Scale
template <class T>
void CMVec2ScaleInv(CMVector2D<T> &vOut, const CMVector2D<T> &vIn, T scale);

//---------------------------
// Non-Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 2D Vector, Using The Specified Angle In Radians
template <class T>
void CMVec2Rotate(CMVector2D<T> &vOut, const CMVector2D<T> &vIn, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Length Of A 2D Vector
template <class T>
T CMVec2Length(const CMVector2D<T> &v);

//template float CMVec2Length<float>(const CMVector2D<float> *pV);

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of A 2D Vector
template <class T>
T CMVec2LengthSq(const CMVector2D<T> &v);
  
///////////////////////////////////////////////////////////////////////////////////////
// Normalize A 2D Vector
template <class T>
void CMVec2Normalize(CMVector2D<T> &vOut, const CMVector2D<T> &vIn);

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Length Between Two 2D Points
template <class T>
T CMVec2Distance(const CMVector2D<T> &p0, const CMVector2D<T> &p1);

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Square Length Between Two 2D Points
template <class T>
T CMVec2DistanceSq(const CMVector2D<T> &p0, const CMVector2D<T> &p1);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Angle In Radians Between Two 2D Vectors
template <class T>
T CMVec2Angle(const CMVector2D<T> &v1, const CMVector2D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Reflected 2D Vector
template <class T>
void CMVec2Reflect(CMVector2D<T> &vR, const CMVector2D<T> &vL, const CMVector2D<T> &vN);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Refracted 2D Vector
template <class T>
void CMVec2Refract(CMVector2D<T> &vR, const CMVector2D<T> &vL, const CMVector2D<T> &vN, T etaRatio);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Catmull-Rom Interpolation, Using The Specified 2D Vectors
template <class T>
void CMVec2CatmullRom(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, 
                      const CMVector2D<T> &v3, const CMVector2D<T> &v4,  T weightingFactor);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A Point In Barycentric Coordinates, Using The Specified 2D Vectors
template <class T>
void CMVec2BaryCentric(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2,
                       const CMVector2D<T> &v3, T f, T g);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Hermite Spline Interpolation, Using The Specified 2D Vectors
template <class T>
void CMVec2Hermite(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &vT1, 
                   const CMVector2D<T> &v2, const CMVector2D<T> &vT2, T weight, T tension, T bias);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Quadratic Interpolation, Using The Specified 2D Vectors
template <class T>
void CMVec2Quadratic(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2,
                     const CMVector2D<T> &v3, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cubic Interpolation, Using The Specified 2D Vectors
template <class T>
void CMVec2Cubic(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2,
                 const CMVector2D<T> &v3, const CMVector2D<T> &v4, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Square Interpolation, Using Two 2D Vectors
template <class T>
void CMVec2Sqrp(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cosinus Interpolation, Using Two 2D Vectors
template <class T>
void CMVec2Cosrp(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, T factor);

#endif //_CMVECTOR2D_


///////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMVECTOR3D_
#define _CMVECTOR3D_

template <class T> 
// 3D Vector Class Declaration
class CMVector3D
{
public:
    //Member Variables
    union {
        struct{
            T x, y, z;
        };
        T v[3];
    };

public:
    //Constructor Declarations
    CMVector3D();
    CMVector3D(const T  * const p);
    CMVector3D(T x, T y, T z);
    CMVector3D(const CMVector3D & v);
    CMVector3D(const CMVector2D<T> & v);

    //Cast To Pointer  (T *)
    operator T* () const;
	operator const T* () const;

    //Cast To CMVector2D Type
    operator  CMVector2D<T>();
    
    // Assignment Operators
    CMVector3D& operator =  ( const CMVector3D & );
    CMVector3D& operator += ( const CMVector3D & );
    CMVector3D& operator -= ( const CMVector3D & );
    CMVector3D& operator *= ( const T & );
    CMVector3D& operator /= ( const T & );

    // Unary Operators
    CMVector3D operator + () const;
    CMVector3D operator - () const;

    T operator [] (int32t );

    // Binary Operators
    CMVector3D operator + ( const CMVector3D & ) const;
    CMVector3D operator - ( const CMVector3D & ) const;
    CMVector3D operator * ( const T & ) const;
    CMVector3D operator / ( const T & ) const;

    boolt operator == ( const CMVector3D & ) const;
    boolt operator != ( const CMVector3D & ) const;

};

//---------------------------
// Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two 3D Vectors
template <class T>
void CMVec3Add(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Two 3D Vectors
template <class T>
void CMVec3Sub(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Dot Product Between Two 3D Vectors
template <class T>
T CMVec3Dot(const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Cross Product Of Two 3D Vectors
template <class T>
void CMVec3Cross(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Linear Interpolation Between Two 3D Vectors
template <class T>
void CMVec3Lerp(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, T weight);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 3D Vector That Is Made Up Of The Largest Components Of Two 3D Vectors
template <class T>
void CMVec3Max(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 3D Vector That Is Made Up Of The Smallest Components Of Two 3D Vectors
template <class T>
void CMVec3Min(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Scales A 3D Vector
template <class T>
void CMVec3Scale(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T scale);

///////////////////////////////////////////////////////////////////////////////////////
// Invert Scale Of 3D Vector
template <class T>
void CMVec3ScaleInv(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T scale);

///////////////////////////////////////////////////////////////////////////////////////
// Setting 3D Vectors Values
template <class T>
void CMVec3Set(CMVector3D<T> &v, T x, T y, T z);

//---------------------------
// Non-Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 3D Vector In X Axis, Using The Specified Angle In Radians
template <class T>
void CMVec3RotateX(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 3D Vector In Y Axis, Using The Specified Angle In Radians
template <class T>
void CMVec3RotateY(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 3D Vector In Z Axis, Using The Specified Angle In Radians
template <class T>
void CMVec3RotateZ(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Length Of A 3D Vector
template <class T>
T CMVec3Length(const CMVector3D<T> &v);

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of A 3D Vector
template <class T>
T CMVec3LengthSq(const CMVector3D<T> &v);

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Length Between Two 3D Points
template <class T>
T CMVec3Distance(const CMVector3D<T> &p0, const CMVector3D<T> &p1);

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Square Length Between Two 3D Points
template <class T>
T CMVec3DistanceSq(const CMVector3D<T> &p0, const CMVector3D<T> &p1);
  
///////////////////////////////////////////////////////////////////////////////////////
// Normalize A 3D Vector
template <class T>
void CMVec3Normalize(CMVector3D<T> &vOut, const CMVector3D<T> &vIn);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Angle In Radians Between Two 3D Vectors
template <class T>
T CMVec3Angle(const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Reflected 3D Vector
template <class T>
void CMVec3Reflect(CMVector3D<T> &vR, const CMVector3D<T> &vL, const CMVector3D<T> &vN);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Refracted 3D Vector
template <class T>
void CMVec3Refract(CMVector3D<T> &vR, const CMVector3D<T> &vL, const CMVector3D<T> &vN, T etaRatio);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Catmull-Rom Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3CatmullRom(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, 
                      const CMVector3D<T> &v3, const CMVector3D<T> &v4,  T weightingFactor);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A Point In Barycentric Coordinates, Using The Specified 3D Vectors
template <class T>
void CMVec3BaryCentric(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2,
                       const CMVector3D<T> &v3, T f, T g);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Hermite Spline Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3Hermite(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &vT1, 
                   const CMVector3D<T> &v2, const CMVector3D<T> &vT2, T weight, T tension, T bias);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Quadratic Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3Quadratic(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2,
                     const CMVector3D<T> &v3, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cubic Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec3Cubic(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2,
                 const CMVector3D<T> &v3, const CMVector3D<T> &v4, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Square Interpolation, Using Two 3D Vectors
template <class T>
void CMVec3Sqrp(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cosinus Interpolation, Using Two 3D Vectors
template <class T>
void CMVec3Cosrp(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, T factor);


#endif //_CMVECTOR3D_
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMVECTOR4D_
#define _CMVECTOR4D_

template <class T> 
// 4D Vector Class Declaration
class CMVector4D
{
public:
    //Member Variables
    union {
        struct{
            T x, y, z, w;
        };
        T v[4];
    };

public:
    //Constructor Declarations
    CMVector4D();
    CMVector4D(const T  * const p);
    CMVector4D(T x, T y, T z, T w);
    CMVector4D(const CMVector4D &v);
    CMVector4D(const CMVector3D<T> &v);
    CMVector4D(const CMVector2D<T> &v);

    //Cast To Pointer  (T *)
    operator T* () const;
	operator const T* () const;

    //Cast To CMVector3D Type
    operator  CMVector3D<T>();

    //Cast To CMVector2D Type
    operator  CMVector2D<T>();
    
    // Assignment Operators
    CMVector4D& operator =  ( const CMVector4D & );
    CMVector4D& operator += ( const CMVector4D & );
    CMVector4D& operator -= ( const CMVector4D & );
    CMVector4D& operator *= ( const T & );
    CMVector4D& operator /= ( const T & );

    // Unary Operators
    CMVector4D operator + () const;
    CMVector4D operator - () const;

    T operator [] (int32t );

    // Binary Operators
    CMVector4D operator + ( const CMVector4D & ) const;
    CMVector4D operator - ( const CMVector4D & ) const;
    CMVector4D operator * ( const T & ) const;
    CMVector4D operator / ( const T & ) const;

    boolt operator == ( const CMVector4D & ) const;
    boolt operator != ( const CMVector4D & ) const;

};

//---------------------------
// Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two 4D Vectors
template <class T>
void CMVec4Add(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Two 4D Vectors
template <class T>
void CMVec4Sub(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Dot Product Between Two 4D Vectors
template <class T>
T CMVec4Dot(const CMVector3D<T> &v1, const CMVector3D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Linear Interpolation Between Two 4D Vectors
template <class T>
void CMVec4Lerp(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, T weight);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 4D Vector That Is Made Up Of The Largest Components Of Two 4D Vectors
template <class T>
void CMVec4Max(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 4D Vector That Is Made Up Of The Smallest Components Of Two 4D Vectors
template <class T>
void CMVec4Min(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Scales A 4D Vector
template <class T>
void CMVec4Scale(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T scale);

///////////////////////////////////////////////////////////////////////////////////////
// Invert Scale Of 4D Vector
template <class T>
void CMVec4ScaleInv(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T scale); 

//---------------------------
// Non-Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Cross Product Of Two 4D Vectors
template <class T>
void CMVec4Cross(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 4D Vector In X Axis, Using The Specified Angle In Radians
template <class T>
void CMVec4RotateX(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 4D Vector In Y Axis, Using The Specified Angle In Radians
template <class T>
void CMVec4RotateY(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Rotate A 4D Vector In Z Axis, Using The Specified Angle In Radians
template <class T>
void CMVec4RotateZ(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Length Of A 4D Vector
template <class T>
T CMVec4Length(const CMVector4D<T> &v);

///////////////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of A 4D Vector
template <class T>
T CMVec4LengthSq(const CMVector4D<T> &v);

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Length Between Two 4D Points
template <class T>
T CMVec4Distance(CMVector4D<T> &p0, CMVector4D<T> &p1);

///////////////////////////////////////////////////////////////////////////////////////
// Calculate Square Length Between Two 4D Points
template <class T>
T CMVec4DistanceSq(CMVector4D<T> &p0, CMVector4D<T> &p1);
  
///////////////////////////////////////////////////////////////////////////////////////
// Normalize A 4D Vector
template <class T>
void CMVec4Normalize(CMVector4D<T> &vOut, const CMVector4D<T> &vIn);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Angle In Radians Between Two 4D Vectors
template <class T>
T CMVec4Angle(const CMVector4D<T> &v1, const CMVector4D<T> &v2);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Reflected 4D Vector
template <class T>
void CMVec4Reflect(CMVector4D<T> &vR, const CMVector4D<T> &vL, const CMVector4D<T> &vN);

///////////////////////////////////////////////////////////////////////////////////////
// Getting Refracted 4D Vector
template <class T>
void CMVec4Refract(CMVector4D<T> &vR, const CMVector4D<T> &vL, const CMVector4D<T> &vN, T etaRatio);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Catmull-Rom Interpolation, Using The Specified 4D Vectors
template <class T>
void CMVec4CatmullRom(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, 
                      const CMVector4D<T> &v3, const CMVector4D<T> &v4,  T f);

///////////////////////////////////////////////////////////////////////////////////////
// Returns A Point In Barycentric Coordinates, Using The Specified 4D Vectors
template <class T>
void CMVec4BaryCentric(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2,
                       const CMVector4D<T> &v3, T f, T g);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Hermite Spline Interpolation, Using The Specified 3D Vectors
template <class T>
void CMVec4Hermite(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &vT1, 
                   const CMVector4D<T> &v2, const CMVector4D<T> &vT2, T weight, T tension, T bias);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Quadratic Interpolation, Using The Specified 4D Vectors
template <class T>
void CMVec4Quadratic(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2,
                     const CMVector4D<T> &v3, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cubic Interpolation, Using The Specified 4D Vectors
template <class T>
void CMVec4Cubic(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2,
                 const CMVector4D<T> &v3, const CMVector4D<T> &v4, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Square Interpolation, Using Two 4D Vectors
template <class T>
void CMVec4Sqrp(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, T factor);

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Cosinus Interpolation, Using Two 4D Vectors
template <class T>
void CMVec4Cosrp(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, T factor);

//#include "Vector.inl"
//#include "VectorSpec.h"

#endif //_CMVECTOR4D_

///////////////////////////////////////////////////////////////////////////////////////



#include "Vector.inl"
#include "VectorSpec.h"

///////////////////////////////////////////////////////////////////////////////////////