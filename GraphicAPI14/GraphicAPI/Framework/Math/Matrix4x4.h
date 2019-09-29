////////////////////////////////////////////////////////////////////////////////////////////////
//	Matrix4x4.h 
//	Class declaration for a 4x4 matrix
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//Matrix4x4 structure:
//	|e11 e12 e13 e14|
//	|e21 e22 e23 e24|
//	|e31 e32 e33 e34|
//	|e41 e42 e43 e44|
//
//  elements eij: i->row, j->column
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MATRIX4X4_H_
#define _MATRIX4x4_H_

class Matrix4x4
{
public:
	float e11,e12,e13,e14,
		  e21,e22,e23,e24,
		  e31,e32,e33,e34,
		  e41,e42,e43,e44;

	Matrix4x4::Matrix4x4();
	Matrix4x4::Matrix4x4(float r1c1,float r1c2,float r1c3,float r1c4,
						 float r2c1,float r2c2,float r2c3,float r2c4,
						 float r3c1,float r3c2,float r3c3,float r3c4,
						 float r4c1,float r4c2,float r4c3,float r4c4);
	Matrix4x4::Matrix4x4(float Matrix[16]);
	Matrix4x4::Matrix4x4(const Matrix4x4 & copy);
#ifdef _MATRIX3X3_H_
	Matrix4x4::Matrix4x4(const Matrix3x3 & copy);
#endif

	Matrix4x4::~Matrix4x4();

	float       Matrix4x4::det();
	void        Matrix4x4::Transpose();
	void        Matrix4x4::Invert();
	void		Matrix4x4::InvertTranspose();
	void		Matrix4x4::InvertAffine();
	void        Matrix4x4::Identity();
	Matrix4x4   Matrix4x4::GetTransposed();
	Matrix4x4   Matrix4x4::GetInversed();
	Matrix4x4	Matrix4x4::GetInvertTransposed();
	Matrix4x4	Matrix4x4::GetInversedAffine();

	void		Matrix4x4::Translation(float x, float y, float z);
	void		Matrix4x4::RotationX(float RadAngle);
	void		Matrix4x4::RotationY(float RadAngle);
	void		Matrix4x4::RotationZ(float RadAngle);
	void		Matrix4x4::Scale(float ScaleX, float ScaleY, float ScaleZ);

	Matrix4x4&  Matrix4x4::operator=(const Matrix4x4 & M);
#ifdef _MATRIX3X3_H_
	Matrix4x4&  Matrix4x4::operator=(const Matrix3x3 & M);
#endif
	Matrix4x4   Matrix4x4::operator*(const Matrix4x4 & M);
	Matrix4x4   Matrix4x4::operator*(const float & scalar);
	Vector4D    Matrix4x4::operator*(const Vector4D & v);
	Vector3D	Matrix4x4::operator*(const Vector3D & v);
	Matrix4x4   Matrix4x4::operator/(const float & scalar);
	Matrix4x4   Matrix4x4::operator+(const Matrix4x4 & M);
	Matrix4x4   Matrix4x4::operator-(const Matrix4x4 & M);

	Matrix4x4&  Matrix4x4::operator*=(const Matrix4x4 & M);
	Matrix4x4&  Matrix4x4::operator*=(const float & scalar);
	Matrix4x4&  Matrix4x4::operator/=(const float & scalar);
	Matrix4x4&  Matrix4x4::operator+=(const Matrix4x4 & M);
	Matrix4x4&  Matrix4x4::operator-=(const Matrix4x4 & M);

	bool        Matrix4x4::operator==(const Matrix4x4 & M);
	bool        Matrix4x4::operator!=(const Matrix4x4 & M);

#ifdef _MATRIX3X3_H_
	operator	Matrix3x3();
#endif

	operator	float*() const {return (float*) this;}
	operator	const float* () const {return (const float*) this;}
};

#endif //_MATRIX4x4_H_

Matrix4x4 MatrixInvertAffine(Matrix4x4 &M);