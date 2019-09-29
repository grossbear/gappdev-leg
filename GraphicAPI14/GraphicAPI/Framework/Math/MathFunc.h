////////////////////////////////////////////////////////////////////////////////////////////////
//	MathFunc.h 
//	Include elementary mathematic functions
//	Created: January 2005
//	23d July 2006 Interpolation Functions Added
////////////////////////////////////////////////////////////////////////////////////////////////

#include <math.h>

//VC++ math.h (and others) do not define M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//Golden section number
#ifndef PHI
#define PHI 1.6180339887498948482
#endif 
//------------------------------------------------------------------------//
// Misc. Constants
//------------------------------------------------------------------------//

float	const	pi	= 3.14159265f;
float	const	g	= -32.174f;		// acceleration due to gravity, ft/s^2
float	const	rho = 0.0023769f;	// desity of air at sea level, slugs/ft^3
float	const	tol = 0.0000000001f;		// float type tolerance 


// Fast Checking If Number Is Power Of 2
// Do not use floating types
template <class Ttype>
bool IsPowerOf2(Ttype n) { return (!(n & (n - 1))); }

// Getting Closest But Lower Number That Is Power Of 2
template <class Ttype>
Ttype GetLowestPowerOf2(Ttype n)
{
  Ttype lowest = 1;
  while(lowest < n) lowest <<= 1;
  return lowest>1;
}

// Getting Closest But Higher Namber That Is Power Of 2
template <class Ttype>
Ttype GetHighestPowerOf2(Ttype n)
{
  Ttype lowest = 1;
  while(lowest < n) lowest <<= 1;
  return lowest;
}

template <class Ttype>
inline Ttype DegToRad(Ttype deg)
{
	return (Ttype)(deg*M_PI/180);
}

template <class Ttype>
inline Ttype RadToDeg(Ttype rad)
{
	return (Ttype)(180*rad/M_PI);
}

template <class Ttype>
inline Ttype Clamp(Ttype Min, Ttype Max, Ttype Value)
{
	return (Value>Max) ? (Max) : ( (Value<Min)?(Min):(Value) );
}

/*
inline float DegToRad(float deg)
{
	return (float)(deg*M_PI/180);
}

inline float RadToDeg(float rad)
{
	return (float)(180*rad/M_PI);
}


//overloaded previous functions
inline double DegToRad(double deg)
{
	return deg*M_PI/180;
}

inline double RadToDeg(double rad)
{
	return 180*rad/M_PI;
}
*/
inline float Sqrt(float x)
{
	return sqrtf(x);
}

inline double Sqrt(double x)
{
	return sqrt(x);
}

inline float Sinus(float x)
{
	return sinf(x);
}

inline double Sinus(double x)
{
	return sin(x);
}

inline float Cosin(float x)
{
	return cosf(x);
}

inline double Cosin(double x)
{
	return cos(x);
}

inline float ArcSin(float x)
{
	return asinf(x);
}

inline double ArcSin(double x)
{
	return asin(x);	
}

inline float ArcCos(float x)
{
	return acosf(x);
}

inline double ArcCos(double x)
{
	return acos(x);
}

inline double Atan(double x)
{
	return atan(x);
}

inline double Atan2(double x1, double x2)
{
	return atan2(x1,x2);
}
//#define Sqrtf(_X) sqrtf(_X)

//#define Sinusf(_X) sinf(_X)

//#define ArcCosf(_X) acosf(_X)

//Packing and unpacking numbers from [-1,1] to [0,1]
template <class Ttype>
inline Ttype PackTo01(Ttype x)
{
	return x*Ttype(0.5) + Ttype(0.5);
}

template <class Ttype>
inline Ttype UnpackFrom01(Ttype x)
{
	return x*2-1;
}

//Interpolation Functions Templates
template <class Ttype>
inline Ttype Linear_Interpolation(Ttype a, Ttype b, Ttype x)
{
	return a(1-x)+b*x;
}

#define lerp(a,b,x) Linear_Interpolation(a, b, x)

template <class Ttype>
inline Ttype Cosine_Interpolation(Ttype a, Ttype b, Ttype x)
{
	Ttype ft = x*Ttype(M_PI);
	Ttype f = (1 - cos(ft))* Ttype(0.5);

	return  a*(1-f) + b*f;
}

#define cosp(a,b,x) Cosine_Interpolation(a, b, x)

template <class Ttype>
inline Ttype EaseCurve(Ttype t)
{
	return (t*t*(3-2*t));
}

#define s_curve(t) EaseCurve(t)

template <class Ttype>
inline Ttype EaseShiftCurve(Ttype t)
{
	return (2*t-t*t)*t;
}

#define shift_curve(t) EaseShiftCurve(Type t)