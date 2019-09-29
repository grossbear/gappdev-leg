///////////////////////////////////////////////////////////////////////////////////////
//  Algebra.h
//
//  Algebra Classes Instances
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __ALGEBRA_H__
#define __ALGEBRA_H__


//-------------------------------------------------------------------------------------
typedef CMVector2D<float> cvec2f;
typedef CMVector2D<float> CMVector2Df;

#define mvec2add    CMVec2Add
#define mvec2sub    CMVec2Sub 
#define mvec2dot    CMVec2Dot 
#define mvec2ccw    CMVec2CCW 
#define mvec2rot    CMVec2Rotate
#define mvec2len    CMVec2Length
#define mvec2len2   CMVec2LengthSq
#define mvec2nrm    CMVec2Normalize
#define mvec2max    CMVec2Max
#define mvec2min    CMVec2Min
#define mvec2scl    CMVec2Scale
#define mvec2scli   CMVec2ScaleInv
#define mvec2agl    CMVec2Angle
#define mvec2lrp    CMVec2Lerp
#define mvec2sqrp   CMVec2Sqrp
#define mvec2cosp   CMVec2Cosrp
#define mvec2bary   CMVec2BaryCentric
#define mvec2quad   CMVec2Quadratic
#define mvec2cubic  CMVec2Cubic
#define mvec2catm   CMVec2CatmullRom 
#define mvec2herm   CMVec2Hermite

//-------------------------------------------------------------------------------------
typedef CMVector3D<float> cvec3f;
typedef CMVector3D<float> CMVector3Df;

#define mvec3add    CMVec3Add
#define mvec3sub    CMVec3Sub
#define mvec3dot    CMVec3Dot
#define mvec3cross  CMVec3Cross
#define mvec3rotx   CMVec3RotateX
#define mvec3roty   CMVec3RotateY
#define mvec3rotz   CMVec3RotateZ
#define mvec3len    CMVec3Length
#define mvec3len2   CMVec3LengthSq
#define mvec3nrm    CMVec3Normalize
#define mvec3max    CMVec3Max
#define mvec3min    CMVec3Min
#define mvec3scl    CMVec3Scale
#define mvec3scli   CMVec3ScaleInv
#define mvec3agl    CMVec3Angle
#define mvec3lrp    CMVec3Lerp
#define mvec3sqrp   CMVec3Sqrp
#define mvec3cosp   CMVec3Cosrp
#define mvec3bary   CMVec3BaryCentric
#define mvec3quad   CMVec3Quadratic
#define mvec3cubic  CMVec3Cubic
#define mvec3catm   CMVec3CatmullRom 
#define mvec3herm   CMVec3Hermite

//-------------------------------------------------------------------------------------
typedef CMVector4D<float> cvec4f;
typedef CMVector4D<float> CMVector4Df;

#define mvec4add    CMVec4Add
#define mvec4sub    CMVec4Sub
#define mvec4dot    CMVec4Dot
#define mvec4cross  CMVec4Cross
#define mvec4rotx   CMVec4RotateX
#define mvec4roty   CMVec4RotateY
#define mvec4rotz   CMVec4RotateZ
#define mvec4len    CMVec4Length
#define mvec4len2   CMVec4LengthSq
#define mvec4nrm    CMVec4Normalize
#define mvec4max    CMVec4Max
#define mvec4min    CMVec4Min
#define mvec4scl    CMVec4Scale
#define mvec4scli   CMVec4ScaleInv
#define mvec4agl    CMVec4Angle
#define mvec4lrp    CMVec4Lerp
#define mvec4sqrp   CMVec4Sqrp
#define mvec4cosp   CMVec4Cosrp
#define mvec4bary   CMVec4BaryCentric
#define mvec4quad   CMVec4Quadratic
#define mvec4cubic  CMVec4Cubic
#define mvec4catm   CMVec4CatmullRom 
#define mvec4herm   CMVec4Hermite


//-------------------------------------------------------------------------------------
#endif //__ALGEBRA_H__