////////////////////////////////////////////////////////////////////////////////////////////////
//	Vector2D.h 
//	Class declaration for a 2d vector
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

  
class Vector2D 
{
public:
	
	float x;
	float y;

	//constructors declarations
	Vector2D::Vector2D();
	Vector2D::Vector2D(float X, float Y);
	Vector2D::Vector2D(float Vec[2]);
	Vector2D::Vector2D(float u[2],float v[2]);
	Vector2D::Vector2D(float ux, float uy, float vx, float vy);
	Vector2D::Vector2D(const Vector2D & copy);

	Vector2D::~Vector2D();

	//class function declaration
	float Vector2D::Magnitude();
	void  Vector2D::Normalize();
	float Vector2D::DotProduct(const Vector2D & v);
	float Vector2D::GetAngle(Vector2D  v);
	void Vector2D::Set(float x,float y);
	
	Vector2D Vector2D::GetNormalized();
	
	
	//overloaded operators
	Vector2D& Vector2D::operator=(const Vector2D & v);
	Vector2D  Vector2D::operator+(const Vector2D & v);
	Vector2D  Vector2D::operator-(const Vector2D & v);
	float     Vector2D::operator*(const Vector2D & v);
	Vector2D  Vector2D::operator*(const float & scalar);
	Vector2D  Vector2D::operator/(const float & scalar);
	Vector2D  Vector2D::operator-();

	Vector2D& Vector2D::operator+=(const Vector2D & v);
	Vector2D& Vector2D::operator-=(const Vector2D & v);
	Vector2D& Vector2D::operator*=(const float & scalar);
	Vector2D& Vector2D::operator/=(const float & scalar);
	
	bool	  Vector2D::operator==(const Vector2D & v);
	bool	  Vector2D::operator!=(const Vector2D & v);

};

float DotProduct(const Vector2D & u, const Vector2D & v);
float GetAngle(Vector2D u, Vector2D v);
void ClampValues(float Min, float Max, Vector2D &Vec);


#endif //_VECTOR2D_H_