////////////////////////////////////////////////////////////////////////////////////////////////
//	Vector4D.h 
//	Class declaration for a 4d vector
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VECTOR4D_H_
#define _VECTOR4D_H_

class Vector4D
{
public:
	float x;
	float y;
	float z;
	float w;
	
	//constructors declarations
	Vector4D::Vector4D();
	Vector4D::Vector4D(float x, float y, float z);
	Vector4D::Vector4D(float x, float y, float z, float w);

	Vector4D::Vector4D(float Vec4[4]);
	Vector4D::Vector4D(float u[3],float v[3]);
	Vector4D::Vector4D(float ux, float uy, float uz, float vx, float vy, float vz);
	Vector4D::Vector4D(const Vector4D & copy);

	Vector4D::Vector4D(const Vector3D & copy);

#ifdef _VECTOR2D_H_
	Vector4D::Vector4D(const Vector2D & copy);
#endif

	Vector4D::~Vector4D();

	//class function declaration
	float Vector4D::Magnitude();
	void  Vector4D::Normalize();
	float Vector4D::DotProduct(const Vector4D & v);
	float Vector4D::GetAngle(Vector4D  v);
	void Vector4D::Set(float x,float y,float z,float w);
	
	Vector4D Vector4D::GetNormalized();

	//rotations
	//values in radians
	void Vector4D::RotateX(float angle);
	Vector4D Vector4D::GetRotatedX(float angle) const;
	void Vector4D::RotateY(float angle);
	Vector4D Vector4D::GetRotatedY(float angle) const;
	void Vector4D::RotateZ(float angle);
	Vector4D Vector4D::GetRotatedZ(float angle) const;

	//overloaded operators
	Vector4D& Vector4D::operator=(const Vector4D & v);

	Vector4D& Vector4D::operator=(const Vector3D & v);

#ifdef _VECTOR2D_H_
	Vector4D& Vector4D::operator=(const Vector2D & v);
#endif
	Vector4D  Vector4D::operator+(const Vector4D & v);
	Vector4D  Vector4D::operator-(const Vector4D & v);
	float	  Vector4D::operator*(const Vector4D & v);
	Vector4D  Vector4D::operator*(const float & scalar);
	Vector4D  Vector4D::operator/(const float & scalar);
	Vector4D  Vector4D::operator^(const Vector4D & v);
	Vector4D  Vector4D::operator-();

	Vector4D& Vector4D::operator+=(const Vector4D & v);
	Vector4D& Vector4D::operator-=(const Vector4D & v);
	Vector4D& Vector4D::operator*=(const float & scalar);
	Vector4D& Vector4D::operator/=(const float & scalar);
	Vector4D& Vector4D::operator^=(const Vector4D & v);
	
	bool	  Vector4D::operator==(const Vector4D & v);
	bool	  Vector4D::operator!=(const Vector4D & v);

	operator  Vector3D();

	operator	float*() const {return (float*) this;}
	operator	const float* () const {return (const float*) this;}
};

float DotProduct(const Vector4D & u, const Vector4D & v);
float GetAngle(Vector4D u, Vector4D v);
Vector4D CrossProduct(const Vector4D & u, const Vector4D & v); 
void ClampValues(float Min, float Max, Vector4D &Vec);

#endif //_VECTOR4D_H_