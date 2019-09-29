//////////////////////////////////////////////////////////////////////////////
//
//  File:       Vector.inl
//  Content:    Vector inline functions
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _VECTOR_INL_
#define _VECTOR_INL_

////////////////////////////////////////////////////////////////////////////
// 2D Vector Structure Constructors

// Default Constructor
template <class T>
M_INLINE CMVector2D<T>::CMVector2D()
{
	x = T(0);
	y = T(0);
}

// Getting Pointer To Array
template <class T>
M_INLINE CMVector2D<T>::CMVector2D(const T  * const p)
{
	CMVector2D();
	if(p)
	{
		x = p[0];
		y = p[1];
	}
}
// Getting Parameters
template <class T>
M_INLINE CMVector2D<T>::CMVector2D(T x, T y)
{
	this->x = x;
	this->y = y;
}
// Getting Another 2D Vector
template <class T>
M_INLINE CMVector2D<T>::CMVector2D(const CMVector2D & v)
{
	x = v.x;
	y = v.y;
}

////////////////////////////////////////////////////////////////////////////
//Cast To Pointer  (T *)
template <class T>
M_INLINE CMVector2D<T>::operator T* () const
{
    return (T*) this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T>::operator const T* () const
{
    return (const T*) this;
}

////////////////////////////////////////////////////////////////////////////
// Assignment Operators
template <class T>
M_INLINE CMVector2D<T>& CMVector2D<T>::operator =  ( const CMVector2D & v)
{
	x = v.x;
	y = v.y;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T>& CMVector2D<T>::operator += ( const CMVector2D & v)
{
	x += v.x;
	y += v.y;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T>& CMVector2D<T>::operator -= ( const CMVector2D & v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T>& CMVector2D<T>::operator *= ( const T & val)
{
	x *= val;
	y *= val;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T>& CMVector2D<T>::operator /= ( const T & val)
{	
    ASSERT(!mnear0(val));
    
    x /= val;
	y /= val;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T CMVector2D<T>::operator [](int32t i)
{
    ASSERT(i >= 0 && i <= 1);
    return v[i];
}

////////////////////////////////////////////////////////////////////////////
// Unary Operators
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T> CMVector2D<T>::operator + () const
{
	return *this;
}

// Negate
template <class T>
M_INLINE CMVector2D<T> CMVector2D<T>::operator - () const
{
	return CMVector2D(-x, -y);
}

////////////////////////////////////////////////////////////////////////////
// Binary Operators
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T> CMVector2D<T>::operator + ( const CMVector2D & v) const
{
	return CMVector2D(x + v.x, y + v.y);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T> CMVector2D<T>::operator - ( const CMVector2D & v) const
{
    return CMVector2D(x - v.x, y - v.y);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T> CMVector2D<T>::operator * ( const T & val) const
{
    return CMVector2D(x*val, y*val);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector2D<T> CMVector2D<T>::operator / ( const T & val) const
{
    T rev = T(1)/val;
    return CMVector2D(x*rev, y*rev);
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMVector2D<T>::operator == ( const CMVector2D & v) const
{
    return ((x == v.x) && (y == v.y)) ? (1) : (0);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMVector2D<T>::operator != ( const CMVector2D & v) const
{
	return ((x == v.x) && (y == v.y)) ? (0) : (1);
}


////////////////////////////////////////////////////////////////////////////
// Methods Operating On CMVector2D Class
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Adding Two Vectors
template <class T>
M_INLINE void CMVec2Add(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2)
{
    vOut.x = v1.x + v2.x;
    vOut.y = v1.y + v2.y;
    
    return ;
}
////////////////////////////////////////////////////////////////////////////
// Subtracting Two Vectors
template <class T>
M_INLINE void CMVec2Sub(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2)
{
    vOut.x = v1.x - v2.x;
    vOut.y = v1.y - v2.y;

    return ;
}
////////////////////////////////////////////////////////////////////////////
// Dot Product Between Two 2D Vectors
template <class T>
M_INLINE T CMVec2Dot(const CMVector2D<T> &v1, const CMVector2D<T> &v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
////////////////////////////////////////////////////////////////////////////
// Return Z Component By Taking Cross Product Of Two 2D Vectors
template <class T>
M_INLINE T CMVec2CCW(const CMVector2D<T> &v1, const CMVector2D<T> &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

////////////////////////////////////////////////////////////////////////////
// Returns A 2D Vector That Is Made Up Of The Largest Components Of Two 2D Vectors
template <class T>
M_INLINE void CMVec2Max(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2)
{
    vOut.x = (v1.x > v2.x) ? (v1.x) : (v2.x);
    vOut.y = (v1.y > v2.y) ? (v1.y) : (v2.y);

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 2D Vector That Is Made Up Of The Smallest Components Of Two 2D Vectors
template <class T>
M_INLINE void CMVec2Min(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2)
{
    vOut.x = (v1.x < v2.x) ? (v1.x) : (v2.x);
    vOut.y = (v1.y < v2.y) ? (v1.y) : (v2.y);

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Scales A 2D Vector
template <class T>
M_INLINE void CMVec2Scale(CMVector2D<T> &vOut, const CMVector2D<T> &vIn, T scale)
{
    vOut.x = vIn.x * scale;
    vOut.y = vIn.x * scale;

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Invert 2D Vector Scale
template <class T>
M_INLINE void CMVec2ScaleInv(CMVector2D<T> &vOut, const CMVector2D<T> &vIn, T scale)
{
    ASSERT(!mnear0(scale));

    T invScale = T(1)/scale;

    vOut.x = vIn.x*invScale;
    vOut.y = vIn.y*invScale;

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Linear Interpolation Between Two 2D Vectors
template <class T>
M_INLINE void CMVec2Lerp(CMVector2D<T> &vOut, const CMVector2D<T> &v1, const CMVector2D<T> &v2, T weight)
{
    T x = v1.x + ((v2.x - v1.x)*weight);
    T y = v1.y + ((v2.y - v1.y)*weight);

    vOut.x = x;
    vOut.y = y;

    return ;
}

///////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// 3D Vector Structure Constructors

// Default Constructor
template <class T>
M_INLINE CMVector3D<T>::CMVector3D()
{
	x = T(0);
	y = T(0);
    z = T(0);
}

// Getting Pointer To Array
template <class T>
M_INLINE CMVector3D<T>::CMVector3D(const T  * const p)
{
	CMVector3D();
	if(p)
	{
		x = p[0];
		y = p[1];
        z = p[2];
	}
}
// Getting Parameters
template <class T>
M_INLINE CMVector3D<T>::CMVector3D(T x, T y, T z)
{
	this->x = x;
	this->y = y;
    this->z = z;
}
// Copy Constructor
template <class T>
M_INLINE CMVector3D<T>::CMVector3D(const CMVector3D & v)
{
	x = v.x;
	y = v.y;
    z = v.z;
}

// Getting 2D Vector
template <class T>
M_INLINE CMVector3D<T>::CMVector3D(const CMVector2D<T> & v)

{
    x = v.x;
    y = v.y;
    z = T(0);
}

//Cast To Pointer  (T *)
template <class T>
M_INLINE CMVector3D<T>::operator T* () const
{
    return (T*) this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T>::operator const T* () const
{
    return (const T*) this;
}

//Cast To CMVector2D Type
template <class T>
M_INLINE CMVector3D<T>::operator CMVector2D<T>()
{
    return CMVector2D<T>(x,y);
}

////////////////////////////////////////////////////////////////////////////
// Assignment Operators
template <class T>
M_INLINE CMVector3D<T>& CMVector3D<T>::operator =  ( const CMVector3D & v)
{
	x = v.x;
	y = v.y;
    z = v.z;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T>& CMVector3D<T>::operator += ( const CMVector3D & v)
{
	x += v.x;
	y += v.y;
    z += v.z;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T>& CMVector3D<T>::operator -= ( const CMVector3D & v)
{
	x -= v.x;
	y -= v.y;
    z -= v.z;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T>& CMVector3D<T>::operator *= ( const T & val)
{
	x *= val;
	y *= val;
    z *= val;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T>& CMVector3D<T>::operator /= ( const T & val)
{	
    ASSERT(!mnear0(val));
    
    x /= val;
	y /= val;
    z /= val;

	return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T CMVector3D<T>::operator [](int32t i)
{
    ASSERT(i >= 0 && i <= 2);
    return v[i];
}

////////////////////////////////////////////////////////////////////////////
// Unary Operators
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T> CMVector3D<T>::operator + () const
{
	return CMVector3D(x, y, z);
}

// Negate
template <class T>
M_INLINE CMVector3D<T> CMVector3D<T>::operator - () const
{
	return CMVector3D(-x, -y, -z);
}

////////////////////////////////////////////////////////////////////////////
// Binary Operators
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T> CMVector3D<T>::operator + ( const CMVector3D & v) const
{
	return CMVector3D(x + v.x, y + v.y, z + v.z);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T> CMVector3D<T>::operator - ( const CMVector3D & v) const
{
    return CMVector3D(x - v.x, y - v.y, z - v.z);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T> CMVector3D<T>::operator * ( const T & val) const
{
    return CMVector3D(x*val, y*val, z*val);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T> CMVector3D<T>::operator / ( const T & val) const
{
    T inv = T(1)/val;
    ASSERT(!mnear0(inv));
    return CMVector3D(x*inv, y*inv, z*inv);
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMVector3D<T>::operator == ( const CMVector3D & v) const
{
    return ((x == v.x) && (y == v.y) && (z == v.z)) ? (1) : (0);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMVector3D<T>::operator != ( const CMVector3D & v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z)) ? (0) : (1);
}

////////////////////////////////////////////////////////////////////////////
// Methods Operating On CMVector3D Class
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Adding Two Vectors
template <class T>
M_INLINE void CMVec3Add(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
    vOut.x = v1.x + v2.x;
    vOut.y = v1.y + v2.y;
    vOut.z = v1.z + v2.z;
}
////////////////////////////////////////////////////////////////////////////
// Subtracting Two Vectors
template <class T>
M_INLINE void CMVec3Sub(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
    vOut.x = v1.x - v2.x;
    vOut.y = v1.y - v2.y;
    vOut.z = v1.z - v2.z;
}
////////////////////////////////////////////////////////////////////////////
// Dot Product Between Two 3D Vectors
template <class T>
M_INLINE T CMVec3Dot(const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
////////////////////////////////////////////////////////////////////////////
// Cross Product Of Two Vectors
template <class T>
M_INLINE void CMVec3Cross(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
    vOut.x = v1.y*v2.z - v1.z*v2.y;
    vOut.y = v1.z*v2.x - v1.x*v2.z; //v1.x*v2.z - v1.z*v2.x; 
    vOut.z = v1.x*v2.y - v1.y*v2.x;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Linear Interpolation Between Two 3D Vectors
template <class T>
M_INLINE void CMVec3Lerp(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2, T weight)
{
    T x = v1.x + ((v2.x - v1.x)*weight);
    T y = v1.y + ((v2.y - v1.y)*weight);
    T z = v1.z + ((v2.z - v1.z)*weight);

    vOut.x = x;
    vOut.y = y;
    vOut.z = z;
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 3D Vector That Is Made Up Of The Largest Components Of Two 3D Vectors
template <class T>
M_INLINE void CMVec3Max(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
    vOut.x = (v1.x > v2.x) ? (v1.x) : (v2.x);
    vOut.y = (v1.y > v2.y) ? (v1.y) : (v2.y);
    vOut.z = (v1.z > v2.z) ? (v1.z) : (v2.z);
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 3D Vector That Is Made Up Of The Smallest Components Of Two 3D Vectors
template <class T>
M_INLINE void CMVec3Min(CMVector3D<T> &vOut, const CMVector3D<T> &v1, const CMVector3D<T> &v2)
{
    vOut.x = (v1.x < v2.x) ? (v1.x) : (v2.x);
    vOut.y = (v1.y < v2.y) ? (v1.y) : (v2.y);
    vOut.z = (v1.z < v2.z) ? (v1.z) : (v2.z);
}

///////////////////////////////////////////////////////////////////////////////////////
// Scales A 3D Vector
template <class T>
M_INLINE void CMVec3Scale(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T scale)
{
    vOut.x = vIn.x*scale;
    vOut.y = vIn.y*scale;
    vOut.z = vIn.z*scale;
}

///////////////////////////////////////////////////////////////////////////////////////
// Invert Scale Of 3D Vector
template <class T>
M_INLINE void CMVec3ScaleInv(CMVector3D<T> &vOut, const CMVector3D<T> &vIn, T scale)
{
    T invscale = T(1)/scale;

    vOut.x = vIn.x*invscale;
    vOut.y = vIn.y*invscale;
    vOut.z = vIn.z*invscale;
}

///////////////////////////////////////////////////////////////////////////////////////
// Setting 3D Vectors Values
template <class T>
void CMVec3Set(CMVector3D<T> &v, T x, T y, T z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 4D Vector Structure Constructors

// Default Constructor
template <class T>
M_INLINE CMVector4D<T>::CMVector4D()
{
	x = T(0);
	y = T(0);
    z = T(0);
    w = T(0);
}

// Getting Pointer To Array
template <class T>
M_INLINE CMVector4D<T>::CMVector4D(const T  * const p)
{
	CMVector4D();
	if(p)
	{
		x = p[0];
		y = p[1];
        z = p[2];
        w = p[3];
	}
}
// Getting Parameters
template <class T>
M_INLINE CMVector4D<T>::CMVector4D(T x, T y, T z, T w)
{
	this->x = x;
	this->y = y;
    this->z = z;
    this->w = w;
}
// Copy Constructor
template <class T>
M_INLINE CMVector4D<T>::CMVector4D(const CMVector4D & v)
{
	x = v.x;
	y = v.y;
    z = v.z;
    w = v.w;
}

// Getting 3D Vector
template <class T>
M_INLINE CMVector4D<T>::CMVector4D(const CMVector3D<T> & v)

{
    x = v.x;
    y = v.y;
    z = v.z;
    w = T(0);
}

// Getting 2D Vector
template <class T>
M_INLINE CMVector4D<T>::CMVector4D(const CMVector2D<T> & v)

{
    x = v.x;
    y = v.y;
    z = T(0);
    w = T(0);
}

//Cast To Pointer  (T *)
template <class T>
M_INLINE CMVector4D<T>::operator T* () const
{
    return (T*) this;
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T>::operator const T* () const
{
    return (const T*) this;
}

//Cast To CMVector3D Type
template <class T>
M_INLINE CMVector4D<T>::operator CMVector3D<T>()
{
    return CMVector3D<T>(x,y,z);
}

//Cast To CMVector2D Type
template <class T>
M_INLINE CMVector4D<T>::operator CMVector2D<T>()
{
    return CMVector2D<T>(x,y);
}

///////////////////////////////////////////////////////////////////////////////////////
// Assignment Operators
template <class T>
M_INLINE CMVector4D<T>& CMVector4D<T>::operator =  ( const CMVector4D & v)
{
	x = v.x;
	y = v.y;
    z = v.z;
    w = v.w;

	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T>& CMVector4D<T>::operator += ( const CMVector4D & v)
{
	x += v.x;
	y += v.y;
    z += v.z;
    w += v.w;

	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T>& CMVector4D<T>::operator -= ( const CMVector4D & v)
{
	x -= v.x;
	y -= v.y;
    z -= v.z;
    w -= v.w;

	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T>& CMVector4D<T>::operator *= ( const T & val)
{
	x *= val;
	y *= val;
    z *= val;
    w *= val;

	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T>& CMVector4D<T>::operator /= ( const T & val)
{	
    ASSERT(!mnear0(val));
    
    x /= val;
	y /= val;
    z /= val;
    w /= val;

	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T CMVector4D<T>::operator [](int32t i)
{
    ASSERT(i >= 0 && i <= 3);
    return v[i];
}

///////////////////////////////////////////////////////////////////////////////////////
// Unary Operators
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T> CMVector4D<T>::operator + () const
{
	return *this;
}

// Negate
template <class T>
M_INLINE CMVector4D<T> CMVector4D<T>::operator - () const
{
	return CMVector4D(-x, -y, -z, -w);
}

///////////////////////////////////////////////////////////////////////////////////////
// Binary Operators
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T> CMVector4D<T>::operator + ( const CMVector4D & v) const
{
	return CMVector4D(x + v.x, y + v.y, z + v.z, w + v.w);
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T> CMVector4D<T>::operator - ( const CMVector4D & v) const
{
    return CMVector4D(x - v.x, y - v.y, z - v.z, w - v.w);
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T> CMVector4D<T>::operator * ( const T & val) const
{
    return CMVector4D(x*val, y*val, z*val, w*val);
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T> CMVector4D<T>::operator / ( const T & val) const
{
    T inv = T(1)/val;
    ASSERT(!mnear0(inv));
    return CMVector4D(x*inv, y*inv, z*inv, w*inv);
}

///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMVector4D<T>::operator == ( const CMVector4D & v) const
{
    return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)) ? (true) : (false);
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMVector4D<T>::operator != ( const CMVector4D & v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)) ? (false) : (true);
}

///////////////////////////////////////////////////////////////////////////////////////
// Methods Operating On CMVector4D Class
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Adding Two Vectors
template <class T>
M_INLINE void CMVec4Add(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
    vOut.x = v1.x + v2.x;
    vOut.y = v1.y + v2.y;
    vOut.z = v1.z + v2.z;
    vOut.w = v1.w + v2.w;
}
///////////////////////////////////////////////////////////////////////////////////////
// Subtracting Two Vectors
template <class T>
M_INLINE void CMVec4Sub(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
    vOut.x = v1.x - v2.x;
    vOut.y = v1.y - v2.y;
    vOut.z = v1.z - v2.z;
    vOut.w = v1.w - v2.w;
}
///////////////////////////////////////////////////////////////////////////////////////
// Dot Product Between Two 4D Vectors
template <class T>
M_INLINE T CMVec4Dot(const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w*v2.w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Performs A Linear Interpolation Between Two 4D Vectors
template <class T>
M_INLINE void CMVec4Lerp(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2, T weight)
{
    T x = v1.x + ((v2.x - v1.x)*weight);
    T y = v1.y + ((v2.y - v1.y)*weight);
    T z = v1.z + ((v2.z - v1.z)*weight);
    T w = v1.w + ((v2.w - v1.w)*weight);

    vOut.x = x;
    vOut.y = y;
    vOut.z = z;
    vOut.w = w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 4D Vector That Is Made Up Of The Largest Components Of Two 4D Vectors
template <class T>
M_INLINE void CMVec4Max(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
    vOut.x = (v1.x > v2.x) ? (v1.x) : (v2.x);
    vOut.y = (v1.y > v2.y) ? (v1.y) : (v2.y);
    vOut.z = (v1.z > v2.z) ? (v1.z) : (v2.z);
    vOut.w = (v1.w > v2.w) ? (v1.w) : (v2.w);
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns A 4D Vector That Is Made Up Of The Smallest Components Of Two 4D Vectors
template <class T>
M_INLINE void CMVec4Min(CMVector4D<T> &vOut, const CMVector4D<T> &v1, const CMVector4D<T> &v2)
{
    vOut.x = (v1.x < v2.x) ? (v1.x) : (v2.x);
    vOut.y = (v1.y < v2.y) ? (v1.y) : (v2.y);
    vOut.z = (v1.z < v2.z) ? (v1.z) : (v2.z);
    vOut.w = (v1.w < v2.w) ? (v1.w) : (v2.w);
}

///////////////////////////////////////////////////////////////////////////////////////
// Scales A 4D Vector
template <class T>
M_INLINE void CMVec4Scale(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T scale)
{
    vOut.x = vIn.x*scale;
    vOut.y = vIn.y*scale;
    vOut.z = vIn.z*scale;
    vOut.w = vIn.w*scale;
}

///////////////////////////////////////////////////////////////////////////////////////
// Invert Scale Of 4D Vector
template <class T>
M_INLINE void CMVec4ScaleInv(CMVector4D<T> &vOut, const CMVector4D<T> &vIn, T scale)
{
    T invscale = T(1)/scale;
    ASSERT(!mnear0(invscale));

    vOut.x = vIn.x*invscale;
    vOut.y = vIn.y*invscale;
    vOut.z = vIn.z*invscale;
    vOut.w = vIn.w*invscale;
}

///////////////////////////////////////////////////////////////////////////////////////



#endif //_VECTOR_INL_

////////////////////////////////////////////////////////////////////////////
