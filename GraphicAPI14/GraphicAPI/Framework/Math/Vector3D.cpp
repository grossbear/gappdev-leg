////////////////////////////////////////////////////////////////////////////////////////////////
//	Vector3D.cpp 
//	Function definitions for 3d vector class
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

#include "MathFunc.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

//constructors definition
Vector3D::Vector3D()
{
	x=0; y=0; z=0;
}

Vector3D::Vector3D(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

Vector3D::Vector3D(float Vec[3])
{
	x=Vec[0];
	y=Vec[1];
	z=Vec[2];
}

Vector3D::Vector3D(float u[3], float v[3])
{
	x=v[0]-u[0];
	y=v[1]-u[1];
	z=v[2]-u[2];
}

Vector3D::Vector3D(float ux, float uy, float uz, float vx, float vy, float vz)
{
	x=vx-ux;
	y=vy-uy;
	z=vz-uz;
}

/*Vector3D::Vector3D(const Vector4D & v)
{
	x=v.x;
	y=v.y;
	z=v.z;
}*/

Vector3D::Vector3D(const Vector3D & copy)
{
	x=copy.x;
	y=copy.y;
	z=copy.z;
}


#ifdef _VECTOR2D_H_
Vector3D::Vector3D(const Vector2D & copy)
{
	x=copy.x;
	y=copy.y;
	z=0;
}
#endif

//destructor
Vector3D::~Vector3D(){}

//class function definition
//Magnitude function calculate vector length
float Vector3D::Magnitude()
{
	return sqrtf(x*x+y*y+z*z);
}

//function normalize vector - makes vector length=1
void  Vector3D::Normalize()
{
	float length=this->Magnitude();
	if(length==0)
		return;

	x=x/length;
	y=y/length;
	z=z/length;
}

//dot product calculation 
float Vector3D::DotProduct(const Vector3D & v)
{
	return x*v.x + y*v.y+z*v.z;
}

//function calculates angle between two vectors
float Vector3D::GetAngle(Vector3D  v)
{
	if(v.Magnitude()==0)
		return 0;
	else
		return acos( (this->DotProduct(v)) / (this->Magnitude()/v.Magnitude()) );
		
}

//set up new values in Vector3D
void Vector3D::Set(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

//rotations 
//values in radians
Vector3D Vector3D::GetRotatedX(float angle) const
{
	if(angle==0.0f)
		return (*this);

	float sinAngle=sinf(angle);
	float cosAngle=cosf(angle);

	return Vector3D(	x,
							y*cosAngle - z*sinAngle,
							y*sinAngle + z*cosAngle);
}

void Vector3D::RotateX(float angle)
{
	(*this)=GetRotatedX(angle);
}


Vector3D Vector3D::GetRotatedY(float angle) const
{
	if(angle==0.0f)
		return (*this);

	float sinAngle=sinf(angle);
	float cosAngle=cosf(angle);

	return Vector3D(	x*cosAngle + z*sinAngle,
									y,
						-x*sinAngle + z*cosAngle);
}

void Vector3D::RotateY(float angle)
{
	(*this)=GetRotatedY(angle);
}

Vector3D Vector3D::GetRotatedZ(float angle) const
{
	if(angle==0.0f)
		return (*this);

	float sinAngle=sinf(angle);
	float cosAngle=cosf(angle);

	return Vector3D(x*cosAngle - y*sinAngle,
					x*sinAngle + y*cosAngle,
					z);

}

void Vector3D::RotateZ(float angle)
{
	(*this)=GetRotatedZ(angle);
}


//packing to 01
Vector3D Vector3D::GetPackedTo01() const
{
	Vector3D packvec(*this);
	packvec.Normalize();

	packvec=packvec*0.5f + Vector3D(0.5f, 0.5f, 0.5f);

	return packvec;
}

void Vector3D::PackTo01()
{
	(*this)=GetPackedTo01();
}


//function return new normalized vector based on this vector
Vector3D Vector3D::GetNormalized()
{
	Vector3D norm;

	norm.x=this->x;
	norm.y=this->y;
	norm.z=this->z;

	norm.Normalize();

	return norm;
}

//operators
Vector3D& Vector3D::operator=(const Vector3D & v)
{
	x=v.x;
	y=v.y;
	z=v.z;

	return *this;
}

#ifdef _VECTOR2D_H_
Vector3D& Vector3D::operator=(const Vector2D & v)
{
	x=v.x;
	y=v.y;
	z=0;

	return *this;
}
#endif

//add two vectors (you'll get middle vector between these two)
Vector3D  Vector3D::operator+(const Vector3D & v)
{
	return Vector3D(x+v.x, y+v.y, z+v.z);
}

//subtract two vectors
Vector3D  Vector3D::operator-(const Vector3D & v)
{
	return Vector3D(x-v.x, y-v.y, z-v.z);
}

//dot product between two vectors
float Vector3D::operator*(const Vector3D & v)
{
	return x*v.x + y*v.y + z*v.z;
}

//multiply this vector by scalar
Vector3D  Vector3D::operator*(const float & scalar)
{
	return Vector3D(x*scalar, y*scalar, z*scalar);
}

//divide this vector by scalar
Vector3D  Vector3D::operator/(const float & scalar)
{
	return (scalar==0.0f) ? Vector3D() : Vector3D(x/scalar, y/scalar, z/scalar);
}

//cross product
Vector3D Vector3D::operator^(const Vector3D & v)
{
	return Vector3D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

//negate vector
Vector3D  Vector3D::operator-()
{
	return Vector3D(-x,-y,-z);
}


Vector3D& Vector3D::operator+=(const Vector3D & v)
{
	x+=v.x;
	y+=v.y;
	z+=v.z;

	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D & v)
{
	x-=v.x;
	y-=v.y;
	z-=v.z;

	return *this;
}

Vector3D& Vector3D::operator*=(const float & scalar)
{
	x*=scalar;
	y*=scalar;
	z*=scalar;

	return *this;
}

Vector3D& Vector3D::operator/=(const float & scalar)
{
	if(scalar==0.0f)
	{
		this->x=0.0f;
		this->y=0.0f;
		this->z=0.0f;

		return *this;
	}
	else
	{
		x/=scalar;
		y/=scalar;
		z/=scalar;

		return *this;
	}
}

Vector3D& Vector3D::operator^=(const Vector3D & v)
{
	float x = this->y*v.z - this->z*v.y;
	float y = this->z*v.x - this->x*v.z;
	float z = this->x*v.y - this->y*v.x;
	
	this->x=x;
	this->y=y;
	this->z=z;
	
	return *this;
}

//check equality 
bool	  Vector3D::operator==(const Vector3D & v)
{
	if(x==v.x && y==v.y && z==v.z)
		return true;
	else
		return false;
}

//check unequality
bool	  Vector3D::operator!=(const Vector3D & v)
{
	if(x==v.x && y==v.y && z==v.z)
		return false;
	else
		return true;
}

#ifdef _VECTOR2D_H_
Vector3D::operator Vector2D()
{
	return Vector2D(x,y);
}
#endif

//other non-class function working on Vector3D class objects
//another function calculating dot product between two vectors
float DotProduct(const Vector3D & u, const Vector3D & v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

//another function calculating angle between two vectors
float GetAngle(Vector3D  u, Vector3D  v)
{
	if(v.Magnitude()==0)
		return 0;
	else
		return acos( DotProduct(u,v) / (u.Magnitude()/v.Magnitude()) );
}

//another function calculating cross product between two vectors
Vector3D CrossProduct(const Vector3D & u, const Vector3D & v)
{
	return Vector3D(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

//Clamping Values In Vector3D

void ClampValues(float Min, float Max, Vector3D &Vec)
{
	Vec.x = Clamp(Min,Max,Vec.x);
	Vec.y = Clamp(Min,Max,Vec.y);
	Vec.z = Clamp(Min,Max,Vec.z);
}
