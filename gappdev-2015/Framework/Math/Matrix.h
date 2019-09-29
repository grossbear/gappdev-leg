///////////////////////////////////////////////////////////////////////////////////////
//  Matrix.h
//
//  Matrix Classes Templates Declarations
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _MATRIX_H_
#define _MATRIX_H_

///////////////////////////////////////////////////////////////////////////////////////
// 2x2 Matrix Class Declaration
template <class T>
class CMMatrix22
{
public:
    union{
        struct{
            T _11, _12;
            T _21, _22;
        };
        T m[2][2];
    };

public:
    //Constructors
    CMMatrix22();
    CMMatrix22(const T* const p);
    CMMatrix22(const CMMatrix22& mtx);
    CMMatrix22(CMVector2D<T> row1, CMVector2D<T> row2);
    CMMatrix22(T _11, T _12, 
               T _21, T _22);

    //Cast Operators  
    operator T* () const;
	operator const T* () const;

    // Access Grants
    CMVector2D<T> operator [] (int32t );
    CMVector2D<T> operator [] (int32t ) const;

    // Assignment Operators
    CMMatrix22& operator =  ( const CMMatrix22 & );
    CMMatrix22& operator += ( const CMMatrix22 & );
    CMMatrix22& operator -= ( const CMMatrix22 & );
    CMMatrix22& operator *= ( const CMMatrix22 & );
    CMMatrix22& operator *= ( const T & );
    CMMatrix22& operator /= ( const T & );

    // Unary Operators
    CMMatrix22 operator + () const;
    CMMatrix22 operator - () const;

    // Binary Operators
    CMMatrix22 operator + ( const CMMatrix22 & ) const;
    CMMatrix22 operator - ( const CMMatrix22 & ) const;
    CMMatrix22 operator * ( const CMMatrix22 & ) const;
    CMMatrix22 operator * ( const T & ) const;
    CMMatrix22 operator / ( const T & ) const;

    boolt operator == ( const CMMatrix22 & ) const;
    boolt operator != ( const CMMatrix22 & ) const;
};

//---------------------------
// Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Set Identity Matrix
template <class T>
void CMMtx22Identity(CMMatrix22<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
boolt CMMtx22IsIdentity(const CMMatrix22<T> &M);

//---------------------------
// Non-Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template <class T>
void CMMtx22Add(CMMatrix22<T> &Out, const CMMatrix22<T> &Mtx1, const CMMatrix22<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting 2x2 Matrices
template <class T>
void CMMtx22Sub(CMMatrix22<T> &Out, const CMMatrix22<T> &Mtx1, const CMMatrix22<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Inverse Matrix
template <class T>
void CMMtx22Inverse(CMMatrix22<T> &Out, const CMMatrix22<T> &vM);

///////////////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
template <class T>
void CMMtx22Transpose(CMMatrix22<T> &Out, const CMMatrix22<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template <class T>
void CMMtx22Scale(CMMatrix22<T> &M, T x, T y);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix
template <class T>
void CMMtx22Rotate(CMMatrix22<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 2D Vector By 2x2 Matrix
template <class T>
void CMVec2Transform(CMVector2D<T> &Out, const CMVector2D<T> &v, const CMMatrix22<T> &M);


///////////////////////////////////////////////////////////////////////////////////////
// 3x3 Matrix Class Declaration
template <class T>
class CMMatrix33
{
public:
    union{
        struct{
            T _11, _12, _13;
            T _21, _22, _23;
            T _31, _32, _33;
        };
        T m[3][3];
    };

public:
    //Constructors
    CMMatrix33();
    CMMatrix33(const T* const p);
    CMMatrix33(const CMMatrix33& mtx);
    CMMatrix33(const CMVector3D<T> &row1, const CMVector3D<T> &row2, const CMVector3D<T> &row3);
    CMMatrix33(T e11, T e12, T e13, 
               T e21, T e22, T e23,
               T e31, T e32, T e33);

    // Cast Operators  
    operator T* () const;
	operator const T* () const;

    // Access Grants
    CMVector3D<T>& operator [] (int32t );
    CMVector3D<T>& operator [] (int32t ) const;

    T& operator () (uint32t i, uint32t j);
    T  operator () (uint32t i, uint32t j) const;

    // Assignment Operators
    CMMatrix33& operator =  ( const CMMatrix33 & );
    CMMatrix33& operator += ( const CMMatrix33 & );
    CMMatrix33& operator -= ( const CMMatrix33 & );
    CMMatrix33& operator *= ( const CMMatrix33 & );
    CMMatrix33& operator *= ( const T & );
    CMMatrix33& operator /= ( const T & );

    // Unary Operators
    CMMatrix33 operator + () const;
    CMMatrix33 operator - () const;

    // Binary Operators
    CMMatrix33 operator + ( const CMMatrix33 & ) const;
    CMMatrix33 operator - ( const CMMatrix33 & ) const;
    CMMatrix33 operator * ( const CMMatrix33 & ) const;
    CMMatrix33 operator * ( const T & ) const;
    CMMatrix33 operator / ( const T & ) const;

    boolt operator == ( const CMMatrix33 & ) const;
    boolt operator != ( const CMMatrix33 & ) const;
};

//---------------------------
// Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Set Identity Matrix
template <class T>
void CMMtx33Identity(CMMatrix33<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
boolt CMMtx33IsIdentity(const CMMatrix33<T> &M);

//---------------------------
// Non-Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template <class T>
void CMMtx33Add(CMMatrix33<T> &Out, const CMMatrix33<T> &Mtx1, const CMMatrix33<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting 3x3 Matrices
template <class T>
void CMMtx33Sub(CMMatrix33<T> &Out, const CMMatrix33<T> &Mtx1, const CMMatrix33<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two 3x3 Matrices
template <class T>
void CMMtx33Mult(CMMatrix33<T> &Out, const CMMatrix33<T> &Mtx1, const CMMatrix33<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Inverse Matrix
template <class T>
boolt CMMtx33Inverse(CMMatrix33<T> &Out, const CMMatrix33<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
template <class T>
void CMMtx33Transpose(CMMatrix33<T> &Out, const CMMatrix33<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix In 2D Space
template <class T>
void CMMtx33Scale2D(CMMatrix33<T> &M, T x, T y);

///////////////////////////////////////////////////////////////////////////////////////
// Create Inverse Scale Values Matrix In 2D Space
template <class T>
void CMMtx33ScaleInv2D(CMMatrix33<T> &M, T x, T y);

///////////////////////////////////////////////////////////////////////////////////////
// Create Translation Matrix In 2D Space
template <class T>
void CMMtx33Translate2D(CMMatrix33<T> &M, T x, T y);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template <class T>
void CMMtx33Scale(CMMatrix33<T> &M, T x, T y, T z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Inverse Scale Values Matrix
template <class T>
void CMMtx33ScaleInv(CMMatrix33<T> &M, T x, T y, T z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix In 2D Space
template <class T>
void CMMtx33Rotate2D(CMMatrix33<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template <class T>
void CMMtx33RotateX(CMMatrix33<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template <class T>
void CMMtx33RotateY(CMMatrix33<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template <class T>
void CMMtx33RotateZ(CMMatrix33<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Creates A Matrix That Rotates Around An Arbitrary Axis
template <class T>
void CMMtx33Rotate(CMMatrix33<T> &M, const CMVector3D<T> &vNormVec, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x3 Matrix
template <class T>
void CMVec3Mult(CMVector3D<T> &Out, const CMVector3D<T> &V, const CMMatrix33<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 2D Vector By 2x2 Matrix
template <class T>
void CMVec2Transform(CMVector2D<T> &Out, const CMVector2D<T> &v, const CMMatrix33<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Decomposing Transformation 3x3 Matrix
template <class T>
void CMMtx33Decompose(const CMMatrix33<T> &M, CMVector3D<T> &vScale, CMVector3D<T> &vRotation);


///////////////////////////////////////////////////////////////////////////////////////
// 4x4 Matrix Class Declaration
template <class T>
class CMMatrix44
{
public:
	union{
		struct{
            T        _11, _12, _13, _14;
            T        _21, _22, _23, _24;
            T        _31, _32, _33, _34;
            T        _41, _42, _43, _44;
		};
		T m[4][4];
	};	

public:
    //Constructors
    CMMatrix44();
    CMMatrix44(const T* const p);
    CMMatrix44(const CMMatrix44& mtx);
    CMMatrix44(const CMMatrix33<T>& mtx);
    CMMatrix44(const CMVector4D<T>& row1,const CMVector4D<T>& row2,const CMVector4D<T>& row3,const CMVector4D<T>& row4);
    CMMatrix44(const CMVector3D<T>& row1,const CMVector3D<T>& row2,const CMVector3D<T>& row3,const CMVector3D<T>& row4);
    CMMatrix44(const CMVector3D<T>& row1,const CMVector3D<T>& row2,const CMVector3D<T>& row3);
    CMMatrix44(T _11, T _12, T _13, T _14, 
               T _21, T _22, T _23, T _24,
               T _31, T _32, T _33, T _34,
               T _41, T _42, T _43, T _44 );

    CMMatrix44(T _11, T _12, T _13,
               T _21, T _22, T _23,
               T _31, T _32, T _33 );

    //Cast Operators  
    operator T* () const;
	operator const T* () const;

    // Access Grants
    CMVector4D<T>& operator [] (int32t );
    CMVector4D<T>& operator [] (int32t ) const;

    T& operator () (int32t i, int32t j);
    T  operator () (int32t i, int32t j) const;


    // Assignment Operators
    CMMatrix44& operator =  ( const CMMatrix44 & );
    CMMatrix44& operator += ( const CMMatrix44 & );
    CMMatrix44& operator -= ( const CMMatrix44 & );
    CMMatrix44& operator *= ( const CMMatrix44 & );
    CMMatrix44& operator *= ( const T & );
    CMMatrix44& operator /= ( const T & );

    // Unary Operators
    CMMatrix44 operator + () const;
    CMMatrix44 operator - () const;

    // Binary Operators
    CMMatrix44 operator + ( const CMMatrix44 & ) const;
    CMMatrix44 operator - ( const CMMatrix44 & ) const;
    CMMatrix44 operator * ( const CMMatrix44 & ) const;
    CMMatrix44 operator * ( const T & ) const;
    CMMatrix44 operator / ( const T & ) const;

    boolt operator == ( const CMMatrix44 & ) const;
    boolt operator != ( const CMMatrix44 & ) const;
};

//---------------------------
// Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Set Identity Matrix
template <class T>
void CMMtx44Identity(CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
boolt CMMtx44IsIdentity(const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Setting Matrix Row
template <class T>
void CMMtx44Set(int row, CMMatrix44<T> &M, const CMVector3D<T> &vec, T w = T(1));

///////////////////////////////////////////////////////////////////////////////////////
// Setting Matrix Row
template <class T>
void CMMtx44Set(int row, CMMatrix44<T> &M, const CMVector4D<T> &vec);

//---------------------------
// Non-Inline 
//---------------------------

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template <class T>
void CMMtx44Add(CMMatrix44<T> &Out, const CMMatrix44<T> &Mtx1, const CMMatrix44<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting 4x4 Matrices
template <class T>
void CMMtx44Sub(CMMatrix44<T> &Out, const CMMatrix44<T> &Mtx1, const CMMatrix44<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two 4x4 Matrices
template <class T>
void CMMtx44Mult(CMMatrix44<T> &Out, const CMMatrix44<T> &Mtx1, const CMMatrix44<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Inverse Matrix
template <class T>
boolt CMMtx44Inverse(CMMatrix44<T> &Out, const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Affine Inverse Matrix
template <class T>
boolt CMMtx44InverseAffine(CMMatrix44<T> &Out, const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
template <class T>
void CMMtx44Transpose(CMMatrix44<T> &Out, const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Create Translate Matrix
template <class T>
void CMMtx44Translate(CMMatrix44<T> &M, T x, T y, T z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template <class T>
void CMMtx44Scale(CMMatrix44<T> &M, T x, T y, T z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template <class T>
void CMMtx44RotateX(CMMatrix44<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template <class T>
void CMMtx44RotateY(CMMatrix44<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template <class T>
void CMMtx44RotateZ(CMMatrix44<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Creates A Matrix That Rotates Around An Arbitrary Axis
template <class T>
void CMMtx44Rotate(CMMatrix44<T> &M, const CMVector3D<T> &vNormVec, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 4x4 Matrix
template <class T>
void CMVec4Mult(CMVector4D<T> &Out, const CMVector4D<T> &v, const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 4x4 Matrix Multiplying Only By First Three Rows
template <class T>
void CMVec4Mult3(CMVector4D<T> &Out, const CMVector4D<T> &v, const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x3 Matrix
template <class T>
void CMVec4Mult(CMVector4D<T> &Out, const CMVector4D<T> &v, const CMMatrix33<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 4x4 Matrix 
template <class T>
void CMVec3Mult(CMVector3D<T> &Out, const CMVector3D<T> &v, const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 4x4 Matrix Multiplying Only By First Three Rows
template <class T>
void CMVec3Mult3(CMVector3D<T> &Out, const CMVector3D<T> &v, const CMMatrix44<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Builds A Transformation Matrix. NULL Arguments Are Treated As Default Transformations.
template <class T>
void CMMtx44Transformation(CMMatrix44<T> &M, const CMVector3D<T> &vTranslation, const CMVector3D<T> &vRotation,
                     const CMVector3D<T> &vScale, boolt israd = true);

///////////////////////////////////////////////////////////////////////////////////////
// Decomposing Transformation 4x4 Matrix
template <class T>
void CMMtx44Decompose(const CMMatrix44<T> &M, CMVector3D<T> &vScale, CMVector3D<T> &vRotation,
                      CMVector3D<T> &vTranslation);

///////////////////////////////////////////////////////////////////////////////////////
// Create A LookAt Matrix (Right-Handed)
template <class T>
void CMMtx44LookAtRH(CMMatrix44<T> &M, const CMVector3D<T> &vEye, const CMVector3D<T> &vAt,
                     const CMVector3D<T> &vUp);

///////////////////////////////////////////////////////////////////////////////////////
// Create A LookAt Matrix (Left-Handed)
template <class T>
void CMMtx44LookAtLH(CMMatrix44<T> &M, const CMVector3D<T> &vEye, const CMVector3D<T> &vAt,
                     const CMVector3D<T> &vUp);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template <class T>
void CMMtx44PerspectiveRH(CMMatrix44<T> &M, T w, T h, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template <class T>
void CMMtx44PerspectiveLH(CMMatrix44<T> &M, T w, T h, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template <class T>
void CMMtx44PerspectiveFOVRH(CMMatrix44<T> &M, T fov, T aspect, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template <class T>
void CMMtx44PerspectiveFOVLH(CMMatrix44<T> &M, T fov, T aspect, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template <class T>
void CMMtx44PerspectiveOffCenterRH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template <class T>
void CMMtx44PerspectiveOffCenterLH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Right-Handed)
template <class T>
void CMMtx44OrthoRH(CMMatrix44<T> &M, T w, T h, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Left-Handed)
template <class T>
void CMMtx44OrthoLH(CMMatrix44<T> &M, T w, T h, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Right-Handed)
template <class T>
void CMMtx44OrthoOffCenterRH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Left-Handed)
template <class T>
void CMMtx44OrthoOffCenterLH(CMMatrix44<T> &M, T l, T r, T b, T t, T zn, T zf);

///////////////////////////////////////////////////////////////////////////////////////
// Creates Matrix That Reflects Coordinate System About The Plane
// Plane Must Be Normalized
template <class T>
void CMMtx44Reflect(CMMatrix44<T> &M, const CMPlane<T> &plane);

///////////////////////////////////////////////////////////////////////////////////////
// Creates Matrix That Projects Geometry Into A Plane
// Plane Must Be Normalized
template <class T>
void CMMtx44Project(CMMatrix44<T> &M, const CMVector4D<T> &light, const CMPlane<T> &plane);
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
void CMMtx44Project(CMMatrix44<T> &M, const CMVector3D<T> &light, const CMPlane<T> &plane);

///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// A Combination Of 3x3 Rotation Matrix And Translation 3D Vector
template <class T>
class CMMatrix34 
{
public:
    CMMatrix33<T> M;
    CMVector3D<T> V;

public:
    CMMatrix34();
    CMMatrix34(const CMMatrix34<T>& mtx34);
    CMMatrix34(const CMMatrix33<T>& mtx33, const CMVector3D<T>& vec3 );
    CMMatrix34( T m11, T m12, T m13,
                T m21, T m22, T m23,
                T m31, T m32, T m33,
                T vx,  T vy,  T vz);

    /*
    //Cast Operators  
    operator T* () const;
	operator const T* () const;
    */

    // Access Grants
    CMVector3D<T>& operator [] (int32t );
    CMVector3D<T>& operator [] (int32t ) const;


    // Assignment Operators
    CMMatrix34& operator =  ( const CMMatrix34 & );
    CMMatrix34& operator += ( const CMMatrix34 & );
    CMMatrix34& operator -= ( const CMMatrix34 & );
    CMMatrix34& operator *= ( const CMMatrix34 & );
    CMMatrix34& operator *= ( const T & );
    CMMatrix34& operator /= ( const T & );

    // Unary Operators
    CMMatrix34 operator + () const;
    CMMatrix34 operator - () const;

    // Binary Operators
    CMMatrix34 operator + ( const CMMatrix34 & ) const;
    CMMatrix34 operator - ( const CMMatrix34 & ) const;
    CMMatrix34 operator * ( const CMMatrix34 & ) const;
    CMMatrix34 operator * ( const T & ) const;
    CMMatrix34 operator / ( const T & ) const;

    boolt operator == ( const CMMatrix34 & ) const;
    boolt operator != ( const CMMatrix34 & ) const;

};

//////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template <class T>
void CMMtx34Add(CMMatrix34<T> &Out, const CMMatrix34<T> &Mtx1, const CMMatrix34<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Matrices
template <class T>
void CMMtx34Sub(CMMatrix34<T> &Out, const CMMatrix34<T> &Mtx1, const CMMatrix34<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two Matrices
template <class T>
void CMMtx34Mult(CMMatrix34<T> &Out, const CMMatrix34<T> &Mtx1, const CMMatrix34<T> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Set Identity Matrix
template <class T>
void CMMtx34Identity(CMMatrix34<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
boolt CMMtx34IsIdentity(const CMMatrix34<T> &M);

/*
///////////////////////////////////////////////////////////////////////////////////////
// Inverse Matrix
template <class T>
void CMMtx34Inverse(CMMatrix44<T> *pOut, const CMMatrix44<T> *pM);
*/



///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template <class T>
void CMMtx34Scale(CMMatrix34<T> &M, T x, T y, T z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template <class T>
void CMMtx34RotateX(CMMatrix34<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template <class T>
void CMMtx34RotateY(CMMatrix34<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template <class T>
void CMMtx34RotateZ(CMMatrix34<T> &M, T rad);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x4 Matrix 
template <class T>
void CMVec3Mult(CMVector3D<T> &Out, const CMVector3D<T> &v, const CMMatrix34<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x3 Matrix Of 3x4 Matrix
template <class T>
void CMVec3MultMtx33(CMVector3D<T> &Out, const CMVector3D<T> &v, const CMMatrix34<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x4 Matrix 
template <class T>
void CMVec4Mult(CMVector4D<T> &Out, const CMVector4D<T> &v, const CMMatrix34<T> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x3 Matrix Of 3x4 Matrix
template <class T>
void CMVec4MultMtx33(CMVector4D<T> &Out, const CMVector4D<T> &V, const CMMatrix34<T> &M);


#include "Matrix.inl"
#include "MtxSpec.h"

#endif //_MATRIX_H_