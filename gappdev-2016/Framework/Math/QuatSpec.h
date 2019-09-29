///////////////////////////////////////////////////////////////////////////////////////
//  QuatSpec.h
//
//  Quaternion Class Template Specialization Declarations
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _QUATSPEC_H_
#define _QUATSPEC_H_

////////////////////////////////////////////////////////////////////////////
// Multiplying Two Quaternions
template void CMQuatMult(CMQuaternion<float> &qOut, const CMQuaternion<float> &Q1, const CMQuaternion<float> &Q2);

////////////////////////////////////////////////////////////////////////////
// Multiplying 3D Vector And Quaternion Treating 3D Vector As Quaternion 
// With Zero Const Part
template void CMQuatMult(CMQuaternion<float> &qOut, const CMVector3D<float> &V1, const CMQuaternion<float> &Q1);

////////////////////////////////////////////////////////////////////////////
// Multiplying Quaternion And 3D Vector Treating 3D Vector As Quaternion 
// With Zero Const Part
template void CMQuatMult(CMQuaternion<float> &qOut, const CMQuaternion<float> &Q1, const CMVector3D<float> &V1);

////////////////////////////////////////////////////////////////////////////
// Getting Rotating Angle
template float CMQuatAngle(const CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Getting Normalised Vector Specified By Vector Part Of Quaternion
template void CMQuatAxis(CMVector3D<float> &V, const CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Returns The Length Of Quaternion 
template float CMQuatLength(const CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Returns The Square Of The Length Of Quaternion
template float CMQuatLengthSq(const CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Normalize A Quaternion
template void CMQuatNormalize(CMQuaternion<float> &qOut, const CMQuaternion<float> &qIn);

////////////////////////////////////////////////////////////////////////////
// Rotate Quaternion Using Another Quaternion
template void CMQuatRotate(CMQuaternion<float> &qOut, const CMQuaternion<float> &Q1, const CMQuaternion<float> &Q2);

////////////////////////////////////////////////////////////////////////////
// Rotating 3D Vector Using Quaternion
template void CMQuatRotate(CMVector3D<float> &vOut, const CMQuaternion<float> &Q, const CMVector3D<float> &V);

////////////////////////////////////////////////////////////////////////////
// Create Quaternion For Rotating From Vector V1 To Vector V2
template void CMQuatRotateArc(CMQuaternion<float> &q, const CMVector3D<float> &v1, const CMVector3D<float> &v2);

////////////////////////////////////////////////////////////////////////////
// Bulding Quaternion From Three Euler Angles
template void CMQuatFromAxisAngles(CMQuaternion<float> &Q, float x, float y, float z);

////////////////////////////////////////////////////////////////////////////
// Getting Euler Angles From Quaternion
template void CMQuatToAxisAngles(CMVector3D<float> &vOut, const CMQuaternion<float> &qIn);

////////////////////////////////////////////////////////////////////////////
// Building Matrix From Quaternion
template void CMQuatToMtx(CMMatrix44<float> &M, const CMQuaternion<float> &Q);
////////////////////////////////////////////////////////////////////////////
template void CMMtxToQuat(CMQuaternion<float> &Q, const CMMatrix33<float> &M);

////////////////////////////////////////////////////////////////////////////
// Bulding Quaternion From Matix
template void CMMtxToQuat(CMQuaternion<float> &Q, const CMMatrix44<float> &M);
////////////////////////////////////////////////////////////////////////////
template void CMQuatToMtx(CMMatrix33<float> &M, const CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Logarithm Of A Quaternion 
template void CMQuatLog(CMQuaternion<float> &qOut, const CMQuaternion<float> &qIn);

////////////////////////////////////////////////////////////////////////////
// Exponent Of A Quaternion
template void CMQuatExp(CMQuaternion<float> &qOut, const CMQuaternion<float> &qIn);

////////////////////////////////////////////////////////////////////////////
// Linear Interpolation Between Two Quaternions
template void CMQuatLerp(CMQuaternion<float> &qOut, const CMQuaternion<float> &Q1, 
                         const CMQuaternion<float> &Q2, float weight);

////////////////////////////////////////////////////////////////////////////
// Spherical Quaternion Interpolation
template void CMQuatSLerp(CMQuaternion<float> &qOut, const CMQuaternion<float> &Q1, 
                          const CMQuaternion<float> &Q2, float weight);

////////////////////////////////////////////////////////////////////////////
// Spherical Cubic Interpolation
template void CMQuatSQuad(CMQuaternion<float> &qOut, const CMQuaternion<float> &Q1, const CMQuaternion<float> &Q2, 
                 const CMQuaternion<float> &A, const CMQuaternion<float> &B, float weight);

////////////////////////////////////////////////////////////////////////////
// Spline Interpolation
template void CMQuatSpline(CMQuaternion<float> &Q, const CMQuaternion<float> &Q1, 
                           const CMQuaternion<float> &Qm, const CMQuaternion<float> &Q2);

////////////////////////////////////////////////////////////////////////////
// Quaternion Compression
// Quaternion Must Be Normalized
int32t CMQuatCompress(CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Quaternion Decompression
void CMQuatDecmpress(CMQuaternion<float> &Q, int32t ciquat);

////////////////////////////////////////////////////////////////////////////
// Quaternion Compression Polar Coding Method
// Quaternion Must Be Normalized
int32t CMQuatPolarCoding(CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Quaternion Polar Coding Decompression
void CMQuatPolarDecoding(CMQuaternion<float> &Q, int32t ciquat);

////////////////////////////////////////////////////////////////////////////
// Quaternion Compression Polar Coding Method Saving Two Angles
// Quaternion Must Be Normalized
int32t CMQuatPolarAngleCoding(CMQuaternion<float> &Q);

////////////////////////////////////////////////////////////////////////////
// Quaternion Polar Coding Decompression Decoding Two Angles
void CMQuatPolarAngleDecoding(CMQuaternion<float> &Q, int32t ciquat);

////////////////////////////////////////////////////////////////////////////
// Quaternion Polar Coding Decompression Decoding Two Angles
void CMQuatPolarAngleDecoding(CMQuaternion<float> &Q, int32t ciquat);

#endif  //_QUATSPEC_H_