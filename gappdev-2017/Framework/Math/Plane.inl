//////////////////////////////////////////////////////////////////////////////
//
//  File:       Plane.inl
//  Content:    Plane inline functions
//
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Plane Structure Constructors

// Default Constructor
template <class T>
M_INLINE CMPlane<T>::CMPlane()
{
    a = b = c = d = T(0);
}

// Copy Constructor
template <class T>
M_INLINE CMPlane<T>::CMPlane(const CMPlane<T> &plane)
{
    a = plane.a;
    b = plane.b;
    c = plane.c;
    d = plane.d;
}

// Building Plane From Three Points
template <class T>
M_INLINE CMPlane<T>::CMPlane(const CMVector3D<T> &p1, const CMVector3D<T> &p2, const CMVector3D<T> &p3)
{
    CMPlaneSet(*this,p1,p2,p3);
}

// Building Plane From Normal And Offsert Vector
template <class T>
M_INLINE CMPlane<T>::CMPlane(const CMVector3D<T> &normal, const CMVector3D<T> &point)
{
    CMPlaneSet(*this,normal,point);
}

// Building From Normal And Offset
template <class T>
M_INLINE CMPlane<T>::CMPlane(const CMVector3D<T> &v, T offset)
{
    a = v.x;
    b = v.y;
    c = v.z;
    d = offset;
}

// Bulding From 4D Vector By Treating 4D Vector As Plane Parameters
template <class T>
M_INLINE CMPlane<T>::CMPlane(const CMVector4D<T> &v)
{
    a = v.x;
    b = v.y;
    c = v.z;
    d = v.w;
}

// Getting Pointer To Array
template <class T>
M_INLINE CMPlane<T>::CMPlane(const T* const ptr)
{
    CMPlane();
    if(ptr)
    {
        a = ptr[0];
        b = ptr[1];
        c = ptr[2];
        d = ptr[3];
    }
}

// Getting Parameters Separatly
template <class T>
M_INLINE CMPlane<T>::CMPlane(T _a, T _b, T _c, T _d):a(_a),b(_b),c(_c),d(_d)
{
    
}

////////////////////////////////////////////////////////////////////////////
// Assignment Operators
template <class T>
M_INLINE CMPlane<T>& CMPlane<T>::operator = ( const CMPlane &plane )
{
    a = plane.a;
    b = plane.b;
    c = plane.c;
    d = plane.d;

    return *this;
}
    
////////////////////////////////////////////////////////////////////////////
//Cast To Pointer  (T *)
template <class T>
M_INLINE CMPlane<T>::operator T* () const
{
    return (T*) this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMPlane<T>::operator const T*() const
{
    return (const T*) this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T CMPlane<T>:: operator [] (int32t i)
{
    ASSERT(i >= 0 && i <= 3);

    return p[i];
}

////////////////////////////////////////////////////////////////////////////
// Cast To CMVector3D Type
template <class T>
M_INLINE CMPlane<T>::operator  CMVector3D<T>()
{
    return (CMVector3D<T>&)p[0];//return CMVector3D<T>(x,y,z);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMPlane<T>::operator  CMVector3D<T>&()
{
    return (CMVector3D<T>&)p[0];
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMPlane<T>::operator CMVector3D<T>*()
{
    return (CMVector3D<T>*)&p[0];
}

////////////////////////////////////////////////////////////////////////////
// Cast To CMVector4D Type
template <class T>
M_INLINE CMPlane<T>::operator CMVector4D<T>()
{
    return CMVector4D<T>(&p[0]);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMPlane<T>::operator  CMVector4D<T>*()
{
    return (CMVector4D<T>*)&p[0];
}

////////////////////////////////////////////////////////////////////////////
// Unary Operators
template <class T>
M_INLINE CMPlane<T>  CMPlane<T>::operator + () const
{
    return CMPlane(a,b,c,d);
}
////////////////////////////////////////////////////////////////////////////
// Negate
template <class T>
M_INLINE CMPlane<T> CMPlane<T>::operator - () const
{
    return CMPlane<T>(-a,-b,-c,-d);
}
    