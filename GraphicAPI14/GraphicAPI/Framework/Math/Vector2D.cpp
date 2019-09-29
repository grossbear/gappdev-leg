////////////////////////////////////////////////////////////////////////////////////////////////
//	Vector2D.cpp 
//	Function definitions for 2d vector class
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

#include "MathFunc.h"
#include "Vector2D.h"

//constructors definition
Vector2D::Vector2D() {x=0;y=0;}
Vector2D::Vector2D(float X, float Y) {x=X; y=Y;}
Vector2D::Vector2D(float Vec[2]) {x=Vec[0]; y=Vec[1];}
Vector2D::Vector2D(float u[2], float v[2]) {x=v[0]-u[0]; y=v[1]-u[1];}
Vector2D::Vector2D(float ux, float uy, float vx, float vy) {x=vx-ux; y=vy-uy;}
Vector2D::Vector2D(const Vector2D & copy) {x=copy.x; y=copy.y;}

//destructor
Vector2D::~Vector2D(){}

//class function definition
//Magnitude function calculate vector length
float Vector2D::Magnitude()
{
	return sqrtf(x*x+y*y);
}

//function normalize vector - makes vector length=1
void  Vector2D::Normalize()
{
	float length=this->Magnitude();
	if(length==0)
		return;

	x=x/length;
	y=y/length;
}

//dot product calculation 
float Vector2D::DotProduct(const Vector2D & v)
{
	return x*v.x + y*v.y;
}

//function calculates angle between two vectors
float Vector2D::GetAngle(Vector2D  v)
{
	if(v.Magnitude()==0)
		return 0;
	else
		return acos( (this->DotProduct(v)) / (this->Magnitude()/v.Magnitude()) );
		
}

void Vector2D::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

//function return new normalized vector based on this vector
Vector2D Vector2D::GetNormalized()
{
	Vector2D norm;

	norm.x=this->x;
	norm.y=this->y;

	norm.Normalize();

	return norm;
}

//operators
Vector2D& Vector2D::operator=(const Vector2D & v)
{
	x=v.x;
	y=v.y;

	return *this;
}

//add two vectors (you'll get middle vector between these two)
Vector2D  Vector2D::operator+(const Vector2D & v)
{
	return Vector2D(x+v.x, y+v.y);
}

//subtract two vectors
Vector2D  Vector2D::operator-(const Vector2D & v)
{
	return Vector2D(x-v.x, y-v.y);
}

float Vector2D::operator*(const Vector2D & v)
{
	return x*v.x + y*v.y;
}

//multiply this vector by scalar
Vector2D  Vector2D::operator*(const float & scalar)
{
	return Vector2D(x*scalar, y*scalar);
}

//divide this vector by scalar
Vector2D  Vector2D::operator/(const float & scalar)
{
	return (scalar==0.0f) ? Vector2D(0.0f,0.0f) : Vector2D(x/scalar, y/scalar);
}

//negate vector
Vector2D  Vector2D::operator-()
{
	return Vector2D(-x,-y);
}


Vector2D& Vector2D::operator+=(const Vector2D & v)
{
	x+=v.x;
	y+=v.y;

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D & v)
{
	x-=v.x;
	y-=v.y;

	return *this;
}

Vector2D& Vector2D::operator*=(const float & scalar)
{
	x*=scalar;
	y*=scalar;

	return *this;
}

Vector2D& Vector2D::operator/=(const float & scalar)
{
	if(scalar==0.0f)
	{
		this->x=0.0f;
		this->y=0.0f;

		return *this;
	}
	else
	{
		x/=scalar;
		y/=scalar;

		return *this;
	}
}

//check equality 
bool	  Vector2D::operator==(const Vector2D & v)
{
	if(x==v.x && y==v.y)
		return true;
	else
		return false;
}

//check unequality
bool	  Vector2D::operator!=(const Vector2D & v)
{
	if(x==v.x && y==v.y)
		return false;
	else
		return true;
}

//other non-class function working on Vector2D class objects
//another function calculating dot product between two vectors
float DotProduct(const Vector2D & u, const Vector2D & v)
{
	return u.x*v.x + u.y*v.y;
}

//another function calculating angle between two vectors
float GetAngle(Vector2D  u, Vector2D  v)
{
	if(v.Magnitude()==0)
		return 0;
	else
		return ArcCos( DotProduct(u,v) / (u.Magnitude()/v.Magnitude()) );
}

//Clamping Values In Vector2D
void ClampValues(float Min, float Max, Vector2D &Vec)
{
	Vec.x = Clamp(Min,Max,Vec.x);
	Vec.y = Clamp(Min,Max,Vec.y);
}


