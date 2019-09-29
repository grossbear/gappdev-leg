////////////////////////////////////////////////////////////////////////////////////////////////
//	Vector3D.h 
//	Class declaration for a 3d vector
//	Created: January 2005
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

class Vector3D
{
public:
	float x;
	float y;
	float z;

	//constructors declarations
	Vector3D::Vector3D();
	Vector3D::Vector3D(float x, float y, float z);
	Vector3D::Vector3D(float Vec[3]);
	Vector3D::Vector3D(float u[3],float v[3]);
	Vector3D::Vector3D(float ux, float uy, float uz, float vx, float vy, float vz);
	
	Vector3D::Vector3D(const Vector3D & copy);
#ifdef _VECTOR2D_H_
	Vector3D::Vector3D(const Vector2D & copy);
#endif
	
	Vector3D::~Vector3D();

	//class function declaration
	float Vector3D::Magnitude();
	void  Vector3D::Normalize();
	float Vector3D::DotProduct(const Vector3D & v);
	float Vector3D::GetAngle(Vector3D  v);//returning angle value in radians
	void  Vector3D::Set(float x, float y, float z);
	
	Vector3D Vector3D::GetNormalized();
	
	//rotations
	//values in radians
	void Vector3D::RotateX(float angle);
	Vector3D Vector3D::GetRotatedX(float angle) const;
	void Vector3D::RotateY(float angle);
	Vector3D Vector3D::GetRotatedY(float angle) const;
	void Vector3D::RotateZ(float angle);
	Vector3D Vector3D::GetRotatedZ(float angle) const;

	void Vector3D::PackTo01();
	Vector3D Vector3D::GetPackedTo01() const;
	
	//overloaded operators
	Vector3D& Vector3D::operator=(const Vector3D & v);
	Vector3D& Vector3D::operator=(const Vector2D & v);
	Vector3D  Vector3D::operator+(const Vector3D & v);
	Vector3D  Vector3D::operator-(const Vector3D & v);
	float	  Vector3D::operator*(const Vector3D & v);
	Vector3D  Vector3D::operator*(const float & scalar);
	Vector3D  Vector3D::operator/(const float & scalar);
	Vector3D  Vector3D::operator^(const Vector3D & v);
	Vector3D  Vector3D::operator-();

	Vector3D& Vector3D::operator+=(const Vector3D & v);
	Vector3D& Vector3D::operator-=(const Vector3D & v);
	Vector3D& Vector3D::operator*=(const float & scalar);
	Vector3D& Vector3D::operator/=(const float & scalar);
	Vector3D& Vector3D::operator^=(const Vector3D & v);
	
	bool	  Vector3D::operator==(const Vector3D & v);
	bool	  Vector3D::operator!=(const Vector3D & v);

#ifdef _VECTOR2D_H_
	operator  Vector2D();
#endif

	operator	float*() const {return (float*) this;}
	operator	const float* () const {return (const float*) this;}
	
};

float DotProduct(const Vector3D & u, const Vector3D & v);
float GetAngle(Vector3D u, Vector3D v);
Vector3D CrossProduct(const Vector3D & u, const Vector3D & v); 
void ClampValues(float Min, float Max, Vector3D &Vec);


#endif //_VECTOR3D_H_