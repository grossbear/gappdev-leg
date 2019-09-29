////////////////////////////////////////////////////////////////////////////////////////////////
//	Vector4D.cpp 
//	Function definitions for 4d vector class
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

#include "MathFunc.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

//constructors definition
Vector4D::Vector4D()
{
	x=0;
	y=0;
	z=0;
	w=1;
}


Vector4D::Vector4D(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
	w=1.0f;
}

Vector4D::Vector4D(float x, float y, float z, float w)
{
	this->x=x;
	this->y=y;
	this->z=z;
	this->w=w;
}



Vector4D::Vector4D(float Vec4[4])
{
	x=Vec4[0];
	y=Vec4[1];
	z=Vec4[2];
	w=Vec4[3];
}

Vector4D::Vector4D(float u[3],float v[3])
{
	x=v[0]-u[0];
	y=v[1]-u[1];
	z=v[2]-u[2];
	w=1.0f;
}

Vector4D::Vector4D(float ux, float uy, float uz, float vx, float vy, float vz)
{
	x=vx-ux;
	y=vy-uy;
	z=vz-uz;
	w=1.0f;
}

Vector4D::Vector4D(const Vector4D & copy)
{
	x=copy.x;
	y=copy.y;
	z=copy.z;
	w=copy.w;
}

#ifdef _VECTOR3D_H_
Vector4D::Vector4D(const Vector3D & copy)
{
	x=copy.x;
	y=copy.y;
	z=copy.z;
	w=1.0f;
}
#endif

#ifdef _VECTOR2D_H_
Vector4D::Vector4D(const Vector2D & copy)
{
	x=copy.x;
	y=copy.y;
	z=0;
	w=1;
}
#endif

Vector4D::~Vector4D(){}

//class functions definitions
float Vector4D::Magnitude()
{
	return sqrtf(x*x + y*y + z*z);
}

void  Vector4D::Normalize()
{
	float length=this->Magnitude();
	if(length==0)
		return;

	x=x/length;
	y=y/length;
	z=z/length;
}

float Vector4D::DotProduct(const Vector4D & v)
{
	return x*v.x + y*v.y + z*v.z;
}

float Vector4D::GetAngle(Vector4D  v)
{
	if(v.Magnitude()==0)
		return 0;
	else
		return acos( (this->DotProduct(v)) / (this->Magnitude()/v.Magnitude()) );
}
	
Vector4D Vector4D::GetNormalized()
{
	Vector4D norm;

	norm.x=this->x;
	norm.y=this->y;
	norm.z=this->z;

	norm.Normalize();

	return norm;
}

//set up new values in Vector3D
void Vector4D::Set(float x, float y, float z, float w)
{
	this->x=x;
	this->y=y;
	this->z=z;
	this->w=w;
}

void Vector4D::RotateX(float angle)
{
	(*this)=GetRotatedX(angle);
}

Vector4D Vector4D::GetRotatedX(float angle) const
{
	Vector3D v3d(x, y, z);

	v3d.RotateX(angle);

	return Vector4D(v3d.x, v3d.y, v3d.z, w);
}

void Vector4D::RotateY(float angle)
{
	(*this)=GetRotatedY(angle);
}

Vector4D Vector4D::GetRotatedY(float angle) const
{
	Vector3D v3d(x, y, z);

	v3d.RotateY(angle);

	return Vector4D(v3d.x, v3d.y, v3d.z, w);
}

void Vector4D::RotateZ(float angle)
{
	(*this)=GetRotatedZ(angle);
}

Vector4D Vector4D::GetRotatedZ(float angle) const
{
	Vector3D v3d(x, y, z);

	v3d.RotateZ(angle);

	return Vector4D(v3d.x, v3d.y, v3d.z, w);
}


//overloaded operators
Vector4D& Vector4D::operator=(const Vector4D & v)
{
	x=v.x;
	y=v.y;
	z=v.z;
	w=v.w;

	return *this;
}

Vector4D& Vector4D::operator=(const Vector3D & v)
{
	x=v.x;
	y=v.y;
	z=v.z;
	w=1;

	return *this;
}

#ifdef _VECTOR2D_H_
Vector4D& Vector4D::operator=(const Vector2D & v)
{
	x=v.x;
	y=v.y;
	z=0;
	w=1;

	return *this;
}
#endif

Vector4D  Vector4D::operator+(const Vector4D & v)
{
	return Vector4D(x+v.x, y+v.y, z+v.z);
}

Vector4D  Vector4D::operator-(const Vector4D & v)
{
	return Vector4D(x-v.x, y-v.y, z-v.z);
}

float	  Vector4D::operator*(const Vector4D & v)
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

Vector4D  Vector4D::operator*(const float & scalar)
{
	return Vector4D(x*scalar, y*scalar, z*scalar);
}

Vector4D  Vector4D::operator/(const float & scalar)
{
	return (scalar==0.0f) ? Vector4D() : Vector4D(x/scalar, y/scalar, z/scalar);
}

Vector4D  Vector4D::operator^(const Vector4D & v)
{
	return Vector4D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

Vector4D  Vector4D::operator-()
{
	return Vector4D(-x,-y,-z);
}

Vector4D& Vector4D::operator+=(const Vector4D & v)
{
	x+=v.x;
	y+=v.y;
	z+=v.z;

	return *this;
}

Vector4D& Vector4D::operator-=(const Vector4D & v)
{
	x-=v.x;
	y-=v.y;
	z-=v.z;

	return *this;
}

Vector4D& Vector4D::operator*=(const float & scalar)
{
	x*=scalar;
	y*=scalar;
	z*=scalar;

	return *this;
}

Vector4D& Vector4D::operator/=(const float & scalar)
{
	x/=scalar;
	y/=scalar;
	z/=scalar;

	return *this;
}

Vector4D& Vector4D::operator^=(const Vector4D & v)
{
	float x = this->y*v.z - this->z*v.y;
	float y = this->z*v.x - this->x*v.z;
	float z = this->x*v.y - this->y*v.x;
	
	this->x=x;
	this->y=y;
	this->z=z;
	
	return *this;
}
	
bool	  Vector4D::operator==(const Vector4D & v)
{
	if(x==v.x && y==v.y && z==v.z && w==v.w)
		return true;
	else
		return false;
}


bool	  Vector4D::operator!=(const Vector4D & v)
{
	if(x==v.x && y==v.y && z==v.z && x==v.w)
		return false;
	else
		return true;
}

Vector4D::operator Vector3D()
{
	return Vector3D(x,y,z);
}

//other non-class function working on Vector2D class objects
//another function calculating dot product between two vectors
float DotProduct(const Vector4D & u, const Vector4D & v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

//another function calculating angle between two vectors
float GetAngle(Vector4D  u, Vector4D  v)
{
	if(v.Magnitude()==0)
		return 0;
	else
		return acos( DotProduct(u,v) / (u.Magnitude()/v.Magnitude()) );
}

//another function calculating cross product between two vectors
Vector4D CrossProduct(const Vector4D & u, const Vector4D & v)
{
	return Vector4D(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

//Clamping Values In Vector4D
void ClampValues(float Min, float Max, Vector4D &Vec)
{
	Vec.x = Clamp(Min,Max,Vec.x);
	Vec.y = Clamp(Min,Max,Vec.y);
	Vec.z = Clamp(Min,Max,Vec.z);
	Vec.w = Clamp(Min,Max,Vec.w);
}