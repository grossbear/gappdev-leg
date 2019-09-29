////////////////////////////////////////////////////////////////////////////////////////////////
//	Matrix4x4.cpp 
//	function definitions for 4x4 matrix class
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

#include "MathFunc.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

#include "Matrix4x4.h"


Matrix4x4::Matrix4x4()
{
	e11=0; e12=0; e13=0; e14=0;
	e21=0; e22=0; e23=0; e24=0;
	e31=0; e32=0; e33=0; e34=0;
	e41=0; e42=0; e43=0; e44=0;
}


Matrix4x4::Matrix4x4(float r1c1,float r1c2,float r1c3,float r1c4,
					 float r2c1,float r2c2,float r2c3,float r2c4,
					 float r3c1,float r3c2,float r3c3,float r3c4,
					 float r4c1,float r4c2,float r4c3,float r4c4)

{
	e11=r1c1; e12=r1c2; e13=r1c3; e14=r1c4;
	e21=r2c1; e22=r2c2; e23=r2c3; e24=r2c4;
	e31=r3c1; e32=r3c2; e33=r3c3; e34=r3c4;
	e41=r4c1; e42=r4c2; e43=r4c3; e44=r4c4;
}

Matrix4x4::Matrix4x4(float Matrix[16])
{
	e11=Matrix[0]; e12=Matrix[1]; e13=Matrix[2]; e14=Matrix[3];
	e21=Matrix[4]; e22=Matrix[5]; e23=Matrix[6]; e24=Matrix[7];
	e31=Matrix[8]; e32=Matrix[9]; e33=Matrix[10]; e34=Matrix[11];
	e41=Matrix[12]; e42=Matrix[13]; e43=Matrix[14]; e44=Matrix[15];
}

Matrix4x4::Matrix4x4(const Matrix4x4 & copy)
{
	e11=copy.e11; e12=copy.e12; e13=copy.e13; e14=copy.e14;
	e21=copy.e21; e22=copy.e22; e23=copy.e23; e24=copy.e24;
	e31=copy.e31; e32=copy.e32; e33=copy.e33; e34=copy.e34;
	e41=copy.e41; e42=copy.e42; e43=copy.e43; e44=copy.e44;
}

#ifdef _MATRIX3X3_H_
Matrix4x4::Matrix4x4(const Matrix3x3 & M)
{
	e11=M.e11;	e12=M.e12;	e13=M.e13;	e14=0;
	e21=M.e21;	e22=M.e22;	e23=M.e23;	e24=0;
	e31=M.e31;	e32=M.e32;	e33=M.e33;	e34=0;
	e41=0;		e42=0;		e43=0;		e44=1;
}
#endif

Matrix4x4::~Matrix4x4(){}

float       Matrix4x4::det()
{
	float E1,E2,E3,E4,E5,E6;

	E1=e32*e44 - e42*e34;
	E2=e31*e42 - e41*e32;
	E3=e33*e44 - e43*e34;
	E4=e32*e43 - e42*e33;
	E5=e31*e43 - e41*e33;
	E6=e31*e44 - e41*e34;

	return e11*(e22*E3-e23*E1+e24*E4)-
		   e12*(e21*E3-e23*E6+e24*E5)+
		   e13*(e21*E1-e22*E6+e24*E2)-
		   e14*(e21*E4-e22*E5+e23*E2);
}

void        Matrix4x4::Transpose()
{
	float tmp;

	tmp=e12;
	e12=e21;
	e21=tmp;

	tmp=e13;
	e13=e31;
	e31=tmp;

	tmp=e14;
	e14=e41;
	e41=tmp;

	tmp=e23;
	e23=e32;
	e32=tmp;

	tmp=e24;
	e24=e42;
	e42=tmp;

	tmp=e34;
	e34=e43;
	e43=tmp;
}

void		Matrix4x4::InvertTranspose()
{
	*this = this->GetInvertTransposed();
}

void        Matrix4x4::Invert()
{
	*this = this->GetInvertTransposed();
	this->Transpose();
}

void	Matrix4x4::InvertAffine()
{
	(*this)=this->GetInversedAffine();
}

void	Matrix4x4::Identity()
{
	e11=1; e12=0; e13=0; e14=0;
	e21=0; e22=1; e23=0; e24=0;
	e31=0; e32=0; e33=1; e34=0;
	e41=0; e42=0; e43=0; e44=1;
}

Matrix4x4   Matrix4x4::GetTransposed()
{
	return Matrix4x4(e11,e21,e31,e41,
					 e12,e22,e32,e42,
					 e13,e23,e33,e43,
					 e14,e24,e34,e44);
}

Matrix4x4	Matrix4x4::GetInversed()
{
	Matrix4x4 res = this->GetInvertTransposed();
	res.Transpose();

	return res;
}

Matrix4x4   Matrix4x4::GetInvertTransposed()
{
	Matrix4x4 res;
	float tmp[12];
	float det; //determinant

	//calculate pairs for first 8 elements (cofactors)
	tmp[0] = e33 * e44;
	tmp[1] = e43 * e34;
	tmp[2] = e23 * e44;
	tmp[3] = e43 * e24;
	tmp[4] = e23 * e34;
	tmp[5] = e33 * e24;
	tmp[6] = e13 * e44;
	tmp[7] = e43 * e14;
	tmp[8] = e13 * e34;
	tmp[9] = e33 * e14;
	tmp[10] = e13 * e24;
	tmp[11] = e23 * e14;

	//calculate first 8 elements (cofactors)
	res.e11 = tmp[0]*e22 + tmp[3]*e32 + tmp[4]*e42 - tmp[1]*e22 - tmp[2]*e32 - tmp[5]*e42;
	res.e21 = tmp[1]*e12 + tmp[6]*e32 + tmp[9]*e42 - tmp[0]*e12 - tmp[7]*e32 - tmp[8]*e42;
	res.e31 = tmp[2]*e12 + tmp[7]*e22 + tmp[10]*e42 - tmp[3]*e12 - tmp[6]*e22 - tmp[11]*e42;
	res.e41 = tmp[5]*e12 + tmp[8]*e22 + tmp[11]*e32 - tmp[4]*e12 - tmp[9]*e22 - tmp[10]*e32;
	res.e12 = tmp[1]*e21 + tmp[2]*e31 + tmp[5]*e41 - tmp[0]*e21 - tmp[3]*e32 - tmp[4]*e41;
	res.e22 = tmp[0]*e11 + tmp[7]*e31 + tmp[8]*e41 - tmp[1]*e11 - tmp[6]*e31 - tmp[9]*e41;
	res.e32 = tmp[3]*e11 + tmp[6]*e21 + tmp[11]*e41 - tmp[2]*e11 - tmp[7]*e21 - tmp[10]*e41;
	res.e42 = tmp[4]*e11 + tmp[9]*e21 + tmp[10]*e31 - tmp[5]*e11 - tmp[8]*e21 - tmp[11]*e31;

	//calculate pairs for second 8 elements (cofactors)
	tmp[0] = e31*e42;
	tmp[1] = e41*e32;
	tmp[2] = e21*e42;
	tmp[3] = e41*e22;
	tmp[4] = e21*e32;
	tmp[5] = e31*e22;
	tmp[6] = e11*e42;
	tmp[7] = e41*e12;
	tmp[8] = e11*e32;
	tmp[9] = e31*e12;
	tmp[10] = e11*e22;
	tmp[11] = e21*e12;

	//calculate second 8 elements (cofactors)
	res.e13 = tmp[0]*e24 + tmp[3]*e34 + tmp[4]*e44 - tmp[1]*e24 - tmp[2]*e34 - tmp[5]*e44;
	res.e23 = tmp[1]*e14 + tmp[6]*e34 + tmp[9]*e44 - tmp[0]*e14 - tmp[7]*e34 - tmp[8]*e44;
	res.e33 = tmp[2]*e14 + tmp[7]*e24 + tmp[10]*e44 - tmp[3]*e14 - tmp[6]*e24 - tmp[11]*e44;
	res.e43 = tmp[5]*e14 + tmp[8]*e24 + tmp[11]*e34 - tmp[4]*e14 - tmp[9]*e24 - tmp[10]*e34;
	res.e14 = tmp[2]*e33 + tmp[5]*e43 + tmp[1]*e23 - tmp[4]*e43 - tmp[0]*e23 - tmp[3]*e33;
	res.e24 = tmp[8]*e43 + tmp[0]*e13 + tmp[7]*e33 - tmp[6]*e33 - tmp[9]*e43 - tmp[1]*e13;
	res.e34 = tmp[6]*e23 + tmp[11]*e43 + tmp[3]*e13 - tmp[10]*e43 - tmp[2]*e13 - tmp[7]*e23;
	res.e44 = tmp[10]*e33 + tmp[4]*e13 + tmp[9]*e23 - tmp[8]*e23 - tmp[11]*e33 - tmp[5]*e13;

	// calculate determinant
	det	= e11*res.e11 + e21*res.e21 + e31*res.e31 + e41*res.e41;

	if(det==0.0f)
	{
		Matrix4x4 id;
		return id;
	}	
	res=res/det;

	return res;
}

Matrix4x4 Matrix4x4::GetInversedAffine()
{
	return Matrix4x4(	e11, e21, e31, -(e14*e11)-(e24*e21)-(e34*e31),
						e12, e22, e32, -(e14*e12)-(e24*e22)-(e34*e32),
						e13, e23, e33, -(e14*e13)-(e24*e23)-(e34*e33),
						0.0f, 0.0f, 0.0f, 1.0f);						
}

void        Matrix4x4::Translation(float x, float y, float z)
{
	this->Identity();
	e14=x;
	e24=y;
	e34=z;
}

void		Matrix4x4::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
	this->Identity();
	e11=ScaleX;
	e22=ScaleY;
	e33=ScaleZ;
}

void		Matrix4x4::RotationX(float RadAngle)
{
	this->Identity();
	
	float cosTheta=cosf(RadAngle);
	float sinTheta=sinf(RadAngle);

	e22=cosTheta; e23=-sinTheta;
	e32=sinTheta; e33=cosTheta;
}

void		Matrix4x4::RotationY(float RadAngle)
{
	this->Identity();
	
	float cosTheta=cosf(RadAngle);
	float sinTheta=sinf(RadAngle);

	e11=cosTheta; e13=sinTheta;
	e31=-sinTheta; e33=cosTheta;
}

void		Matrix4x4::RotationZ(float RadAngle)
{
	this->Identity();
	
	float cosTheta=cosf(RadAngle);
	float sinTheta=sinf(RadAngle);

	e11=cosTheta; e12=-sinTheta;
	e21=sinTheta; e22=cosTheta;
}

Matrix4x4&  Matrix4x4::operator=(const Matrix4x4 & M)
{
	e11=M.e11; e12=M.e12; e13=M.e13; e14=M.e14;
	e21=M.e21; e22=M.e22; e23=M.e23; e24=M.e24;
	e31=M.e31; e32=M.e32; e33=M.e33; e34=M.e34;
	e41=M.e41; e42=M.e42; e43=M.e43; e44=M.e44;

	return *this;
}

#ifdef _MATRIX3X3_H_
Matrix4x4&  Matrix4x4::operator=(const Matrix3x3 & M)
{
	e11=M.e11;	e12=M.e12;	e13=M.e13;	e14=0;
	e21=M.e21;	e22=M.e22;	e23=M.e23;	e24=0;
	e31=M.e31;	e32=M.e32;	e33=M.e33;	e34=0;
	e41=0;		e42=0;		e43=0;		e44=1;

	return *this;
}
#endif

Matrix4x4   Matrix4x4::operator*(const Matrix4x4 & M)
{
	return Matrix4x4(e11*M.e11 + e12*M.e21 + e13*M.e31 + e14*M.e41,
					 e11*M.e12 + e12*M.e22 + e13*M.e32 + e14*M.e42,
					 e11*M.e13 + e12*M.e23 + e13*M.e33 + e14*M.e43,
					 e11*M.e14 + e12*M.e24 + e13*M.e34 + e14*M.e44,

					 e21*M.e11 + e22*M.e21 + e23*M.e31 + e24*M.e41,
					 e21*M.e12 + e22*M.e22 + e23*M.e32 + e24*M.e42,
					 e21*M.e13 + e22*M.e23 + e23*M.e33 + e24*M.e43,
					 e21*M.e14 + e22*M.e24 + e23*M.e34 + e24*M.e44,

					 e31*M.e11 + e32*M.e21 + e33*M.e31 + e34*M.e41,
					 e31*M.e12 + e32*M.e22 + e33*M.e32 + e34*M.e42,
					 e31*M.e13 + e32*M.e23 + e33*M.e33 + e34*M.e43,
					 e31*M.e14 + e32*M.e24 + e33*M.e34 + e34*M.e44,

					 e41*M.e11 + e42*M.e21 + e43*M.e31 + e44*M.e41,
					 e41*M.e12 + e42*M.e22 + e43*M.e32 + e44*M.e42,
					 e41*M.e13 + e42*M.e23 + e43*M.e33 + e44*M.e43,
					 e41*M.e14 + e42*M.e24 + e43*M.e34 + e44*M.e44);
}

Matrix4x4   Matrix4x4::operator*(const float & scalar)
{
	return Matrix4x4(e11*scalar,e12*scalar,e13*scalar,e14*scalar,
					 e21*scalar,e22*scalar,e23*scalar,e24*scalar,
					 e31*scalar,e32*scalar,e33*scalar,e34*scalar,
					 e41*scalar,e42*scalar,e43*scalar,e44*scalar);
}

Vector4D    Matrix4x4::operator*(const Vector4D & v)
{
	return Vector4D(e11*v.x + e12*v.y + e13*v.z + e14*v.w,
					e21*v.x + e22*v.y + e23*v.z + e24*v.w,
					e31*v.x + e32*v.y + e33*v.z + e34*v.w,
					e41*v.x + e42*v.y + e43*v.z + e44*v.w);
}

Vector3D	Matrix4x4::operator*(const Vector3D & v3d)
{
	Vector4D v(e11*v3d.x + e12*v3d.y + e13*v3d.z + e14*1.0f,
			   e21*v3d.x + e22*v3d.y + e23*v3d.z + e24*1.0f,
			   e31*v3d.x + e32*v3d.y + e33*v3d.z + e34*1.0f,
			   e41*v3d.x + e42*v3d.y + e43*v3d.z + e44*1.0f);
	
	return (Vector3D)v;
	
}

Matrix4x4   Matrix4x4::operator/(const float & scalar)
{
	return (scalar==0.0) ? Matrix4x4() : Matrix4x4(e11/scalar,e12/scalar,e13/scalar,e14/scalar,
												   e21/scalar,e22/scalar,e23/scalar,e24/scalar,
												   e31/scalar,e32/scalar,e33/scalar,e34/scalar,
												   e41/scalar,e42/scalar,e43/scalar,e44/scalar);
}

Matrix4x4   Matrix4x4::operator+(const Matrix4x4 & M)
{
	return Matrix4x4(e11+M.e11,e12+M.e12,e13+M.e13,e14+M.e14,
					 e21+M.e21,e22+M.e22,e23+M.e23,e24+M.e24,
					 e31+M.e31,e32+M.e32,e33+M.e33,e34+M.e34,
					 e41+M.e41,e42+M.e42,e43+M.e43,e44+M.e44);
}

Matrix4x4   Matrix4x4::operator-(const Matrix4x4 & M)
{
	return Matrix4x4(e11-M.e11,e12-M.e12,e13-M.e13,e14-M.e14,
					 e21-M.e21,e22-M.e22,e23-M.e23,e24-M.e24,
					 e31-M.e31,e32-M.e32,e33-M.e33,e34-M.e34,
					 e41-M.e41,e42-M.e42,e43-M.e43,e44-M.e44);
}

Matrix4x4&  Matrix4x4::operator*=(const Matrix4x4 & M)
{
	*this=*this*M;
	return *this;
}

Matrix4x4&  Matrix4x4::operator*=(const float & scalar)
{
	*this=*this*scalar;

	return *this;
}

Matrix4x4&  Matrix4x4::operator/=(const float & scalar)
{
	(scalar==0.0) ? *this=Matrix4x4() : *this=*this/scalar;

	return *this;
}

Matrix4x4&  Matrix4x4::operator+=(const Matrix4x4 & M)
{
	e11+=M.e11; e12+=M.e12; e13+=M.e13; e14+=M.e14;
	e21+=M.e21; e22+=M.e22; e23+=M.e23; e24+=M.e24;
	e31+=M.e31; e32+=M.e32; e33+=M.e33; e34+=M.e34;
	e41+=M.e41; e42+=M.e42; e43+=M.e43; e44+=M.e44;

	return *this;
}

Matrix4x4&  Matrix4x4::operator-=(const Matrix4x4 & M)
{
	e11-=M.e11; e12-=M.e12; e13-=M.e13; e14-=M.e14;
	e21-=M.e21; e22-=M.e22; e23-=M.e23; e24-=M.e24;
	e31-=M.e31; e32-=M.e32; e33-=M.e33; e34-=M.e34;
	e41-=M.e41; e42-=M.e42; e43-=M.e43; e44-=M.e44;

	return *this;
}

bool        Matrix4x4::operator==(const Matrix4x4 & M)
{
	if(e11==M.e11 && e12==M.e12 && e13==M.e13 && e14==M.e14 &&
	   e21==M.e21 && e22==M.e22 && e23==M.e23 && e24==M.e24 &&
	   e31==M.e31 && e32==M.e32 && e33==M.e33 && e34==M.e34 &&
	   e41==M.e41 && e42==M.e42 && e43==M.e43 && e44==M.e44)
	    return true;
	else
		return false;
}

bool        Matrix4x4::operator!=(const Matrix4x4 & M)
{
	if(e11==M.e11 && e12==M.e12 && e13==M.e13 && e14==M.e14 &&
	   e21==M.e21 && e22==M.e22 && e23==M.e23 && e24==M.e24 &&
	   e31==M.e31 && e32==M.e32 && e33==M.e33 && e34==M.e34 &&
	   e41==M.e41 && e42==M.e42 && e43==M.e43 && e44==M.e44)
	    return false;
	else
		return true;
}

#ifdef _MATRIX3X3_H_
Matrix4x4::operator Matrix3x3()
{
	return Matrix3x3(e11, e12, e13,
						e21, e22, e23,
						e31, e32, e33);
}
#endif

Matrix4x4 MatrixInvertAffine(Matrix4x4 &M)
{
	Matrix4x4 res;

	res.e11 = M.e11;
	res.e21 = M.e12;
	res.e31 = M.e13;
	res.e41 = 0.0f;

	res.e12 = M.e21;
	res.e22 = M.e22;
	res.e32 = M.e23;
	res.e42 = 0.0f;

	res.e13 = M.e31;
	res.e23 = M.e32;
	res.e33 = M.e33;
	res.e43 = 0.0f;

	res.e14 = -(M.e14*M.e11)-(M.e24*M.e21)-(M.e34*M.e31);
	res.e24 = -(M.e14*M.e12)-(M.e24*M.e22)-(M.e34*M.e32);
	res.e34 = -(M.e14*M.e13)-(M.e24*M.e23)-(M.e34*M.e33);
	res.e44 = 1.0f;

	return res;
}