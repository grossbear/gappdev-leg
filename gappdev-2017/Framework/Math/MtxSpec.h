///////////////////////////////////////////////////////////////////////////////////////
//  MtxSpec.h
//
//  Matrix Classes Templates Specialization Declarations
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _MTXSPEC_H_
#define _MTXSPEC_H_

///////////////////////////////////////////////////////////////////////////////////////
// 3x3 Matrix Class Declaration

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template void CMMtx33Add(CMMatrix33<float> &Out, const CMMatrix33<float> &Mtx1, const CMMatrix33<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting 3x3 Matrices
template void CMMtx33Sub(CMMatrix33<float> &Out, const CMMatrix33<float> &Mtx1, const CMMatrix33<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two 3x3 Matrices
template void CMMtx33Mult(CMMatrix33<float> &Out, const CMMatrix33<float> &Mtx1, const CMMatrix33<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Inverse Matrix
template boolt CMMtx33Inverse(CMMatrix33<float> &Out, const CMMatrix33<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
template void CMMtx33Transpose(CMMatrix33<float> &Out, const CMMatrix33<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix In 2D Space
template void CMMtx33Scale2D(CMMatrix33<float> &M, float x, float y);

///////////////////////////////////////////////////////////////////////////////////////
// Create Inverse Scale Values Matrix In 2D Space
template void CMMtx33ScaleInv2D(CMMatrix33<float> &M, float x, float y);

///////////////////////////////////////////////////////////////////////////////////////
// Create Translation Matrix In 2D Space
template void CMMtx33Translate2D(CMMatrix33<float> &M, float x, float y);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template void CMMtx33Scale(CMMatrix33<float> &M, float x, float y, float z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Inverse Scale Values Matrix
template void CMMtx33ScaleInv(CMMatrix33<float> &M, float x, float y, float z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix In 2D Space
template void CMMtx33Rotate2D(CMMatrix33<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template void CMMtx33RotateX(CMMatrix33<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template void CMMtx33RotateY(CMMatrix33<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template void CMMtx33RotateZ(CMMatrix33<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Creates A Matrix That Rotates Around An Arbitrary Axis
template void CMMtx33Rotate(CMMatrix33<float> &M, const CMVector3D<float> &vNormVec, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x3 Matrix
template void CMVec3Mult(CMVector3D<float> &Out, const CMVector3D<float> &v, const CMMatrix33<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 2D Vector By 2x2 Matrix
template void CMVec2Transform(CMVector2D<float> &Out, const CMVector2D<float> &v, const CMMatrix33<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Decomposing Transformation 3x3 Matrix
template void CMMtx33Decompose(const CMMatrix33<float> &M, CMVector3D<float> &vScale, CMVector3D<float> &vRotation);


///////////////////////////////////////////////////////////////////////////////////////
// 4x4 Matrix Class Declaration
// Building Inctances

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template void CMMtx44Add(CMMatrix44<float> &Out, const CMMatrix44<float> &Mtx1, const CMMatrix44<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting 4x4 Matrices
template void CMMtx44Sub(CMMatrix44<float> &Out, const CMMatrix44<float> &Mtx1, const CMMatrix44<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two 4x4 Matrices
template void CMMtx44Mult(CMMatrix44<float> &Out, const CMMatrix44<float> &Mtx1, const CMMatrix44<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Inverse Matrix
template boolt CMMtx44Inverse(CMMatrix44<float> &Out, const CMMatrix44<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Affine Inverse Matrix
template boolt CMMtx44InverseAffine(CMMatrix44<float> &Out, const CMMatrix44<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
template void CMMtx44Transpose(CMMatrix44<float> &Out, const CMMatrix44<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Create Translate Matrix
template void CMMtx44Translate(CMMatrix44<float> &M, float x, float y, float z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template void CMMtx44Scale(CMMatrix44<float> &M, float x, float y, float z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template void CMMtx44RotateX(CMMatrix44<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template void CMMtx44RotateY(CMMatrix44<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template void CMMtx44RotateZ(CMMatrix44<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Creates A Matrix That Rotates Around An Arbitrary Axis
template void CMMtx44Rotate(CMMatrix44<float> &M, const CMVector3D<float> &vNormVec, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 4x4 Matrix
template void CMVec4Mult(CMVector4D<float> &Out, const CMVector4D<float> &v, const CMMatrix44<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 4x4 Matrix Multiplying Only By First Three Rows
template void CMVec4Mult3(CMVector4D<float> &Out, const CMVector4D<float> &v, const CMMatrix44<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x3 Matrix
template void CMVec4Mult(CMVector4D<float> &Out, const CMVector4D<float> &v, const CMMatrix33<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 4x4 Matrix 
template void CMVec3Mult(CMVector3D<float> &Out, const CMVector3D<float> &v, const CMMatrix44<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 4x4 Matrix Multiplying Only By First Three Rows
template void CMVec3Mult3(CMVector3D<float> &Out, const CMVector3D<float> &v, const CMMatrix44<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Builds A Transformation Matrix. NULL Arguments Are Treated As Default Transformations.
template void CMMtx44Transformation(CMMatrix44<float> &M, const CMVector3D<float> &vTranslation, 
                              const CMVector3D<float> &vRotation, const CMVector3D<float> &vScale, boolt israd);

///////////////////////////////////////////////////////////////////////////////////////
// Decomposing Transformation 4x4 Matrix
template void CMMtx44Decompose(const CMMatrix44<float> &M, CMVector3D<float> &vScale, CMVector3D<float> &vRotation,
                      CMVector3D<float> &vTranslation);

///////////////////////////////////////////////////////////////////////////////////////
// Create A LookAt Matrix (Right-Handed)
template void CMMtx44LookAtRH(CMMatrix44<float> &M, const CMVector3D<float> &vEye, const CMVector3D<float> &vAt,
                            const CMVector3D<float> &vUp);


///////////////////////////////////////////////////////////////////////////////////////
// Create A LookAt Matrix (Left-Handed)
template void CMMtx44LookAtLH(CMMatrix44<float> &M, const CMVector3D<float> &vEye, const CMVector3D<float> &vAt,
                            const CMVector3D<float> &vUp);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template void CMMtx44PerspectiveRH(CMMatrix44<float> &M, float w, float h, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template void CMMtx44PerspectiveLH(CMMatrix44<float> &M, float w, float h, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template void CMMtx44PerspectiveFOVRH(CMMatrix44<float> &M, float fov, float aspect, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template void CMMtx44PerspectiveFOVLH(CMMatrix44<float> &M, float fov, float aspect, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Right-Handed)
template void CMMtx44PerspectiveOffCenterRH(CMMatrix44<float> &M, float l, float r, float b, 
                                            float t, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create A Perspective Projection Matrix (Left-Handed)
template void CMMtx44PerspectiveOffCenterLH(CMMatrix44<float> &M, float l, float r, float b,
                                            float t, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Right-Handed)
template void CMMtx44OrthoRH(CMMatrix44<float> &M, float w, float h, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Left-Handed)
template void CMMtx44OrthoLH(CMMatrix44<float> &M, float w, float h, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Right-Handed)
template void CMMtx44OrthoOffCenterRH(CMMatrix44<float> &M, float l, float r, float b,
                                      float t, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Create An Ortho Projection Matrix (Left-Handed)
template void CMMtx44OrthoOffCenterLH(CMMatrix44<float> &M, float l, float r, float b,
                                      float t, float zn, float zf);

///////////////////////////////////////////////////////////////////////////////////////
// Creates Matrix That Reflects Coordinate System About The Plane
template void CMMtx44Reflect(CMMatrix44<float> &M, const CMPlane<float> &plane);

///////////////////////////////////////////////////////////////////////////////////////
// Creates Matrix That Projects Geometry Into A Plane
template void CMMtx44Project(CMMatrix44<float> &M, const CMVector4D<float> &light, 
                             const CMPlane<float> &plane);
///////////////////////////////////////////////////////////////////////////////////////
template void CMMtx44Project(CMMatrix44<float> &M, const CMVector3D<float> &light, 
                             const CMPlane<float> &plane);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 3x4 Matrix Class Declaration
// Building Inctances

//////////////////////////////////////////////////////////////////////////////////////
// Adding Two Matrices
template void CMMtx34Add(CMMatrix34<float> &Out, const CMMatrix34<float> &Mtx1, 
                         const CMMatrix34<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Matrices
template void CMMtx34Sub(CMMatrix34<float> &Out, const CMMatrix34<float> &Mtx1, 
                         const CMMatrix34<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Multiplying Two Matrices
template void CMMtx34Mult(CMMatrix34<float> &Out, const CMMatrix34<float> &Mtx1, 
                          const CMMatrix34<float> &Mtx2);

///////////////////////////////////////////////////////////////////////////////////////
// Create Scale Matrix
template void CMMtx34Scale(CMMatrix34<float> &M, float x, float y, float z);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The X Axis
template void CMMtx34RotateX(CMMatrix34<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Y Axis
template void CMMtx34RotateY(CMMatrix34<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Create Rotation Matrix Around The Z Axis
template void CMMtx34RotateZ(CMMatrix34<float> &M, float rad);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x4 Matrix 
template void CMVec3Mult(CMVector3D<float> &Out, const CMVector3D<float> &v, 
                         const CMMatrix34<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 3D Vector By 3x3 Matrix Of 3x4 Matrix
template void CMVec3MultMtx33(CMVector3D<float> &Out, const CMVector3D<float> &v, 
                              const CMMatrix34<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x4 Matrix 
template void CMVec4Mult(CMVector4D<float> &Out, const CMVector4D<float> &v, 
                         const CMMatrix34<float> &M);

///////////////////////////////////////////////////////////////////////////////////////
// Transform 4D Vector By 3x3 Matrix Of 3x4 Matrix
template void CMVec4MultMtx33(CMVector4D<float> &Out, const CMVector4D<float> &v, 
                              const CMMatrix34<float> &M);


#endif //_MTXSPEC_H_