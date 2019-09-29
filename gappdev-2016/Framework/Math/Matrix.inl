//////////////////////////////////////////////////////////////////////////////
//
//  File:       Matrix.inl
//  Content:    Matrix inline functions
//
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 3x3 Matrix Class Inline Functions
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 3x3 Matrix Class Constructors

// Default Constructor
template <class T>
M_INLINE CMMatrix33<T>::CMMatrix33()
{
    memset(&_11,0,sizeof(T)*9);
}

// Getting Pointer To Array
template <class T>
M_INLINE CMMatrix33<T>::CMMatrix33(const T * const p)
{
    ASSERT(p != NULL);

    memcpy(&_11 ,p, sizeof(T)*9);
}

// Copy Constructor
template <class T>
M_INLINE CMMatrix33<T>::CMMatrix33(const CMMatrix33<T> &mtx)
{
    memcpy(&_11, &mtx._11, sizeof(T)*9);
}

// Constructing Matrix By Getting 3D Vectors
template <class T>
M_INLINE CMMatrix33<T>::CMMatrix33(const CMVector3D<T> &row1, const CMVector3D<T> &row2,
                                   const CMVector3D<T> &row3)
{
    memcpy(&_11, &row1, sizeof(T)*3);
    memcpy(&_21, &row2, sizeof(T)*3);
    memcpy(&_31, &row3, sizeof(T)*3);
}

// Building Matrix By Getting All Paremeters
template <class T>
M_INLINE CMMatrix33<T>::CMMatrix33(T _11, T _12, T _13, T _21, T _22, T _23, T _31, T _32, T _33)
{
    this->_11 = _11;    this->_12 = _12;    this->_13 = _13;
    this->_21 = _21;    this->_22 = _22;    this->_23 = _23;
    this->_31 = _31;    this->_32 = _32;    this->_33 = _33;
}

// Cast To Pointer  (T *)
template <class T>
M_INLINE CMMatrix33<T>::operator T* () const
{
    return (T*) &m[0][0];
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T>::operator const T* () const
{
    return (const T*) &m[0][0];
}

// Access Grants
template <class T>
M_INLINE CMVector3D<T>& CMMatrix33<T>::operator [] (int32t i)
{
    ASSERT(i >= 0 && i <= 2);

    return (CMVector3D<T> &) m[i][0];
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector3D<T>& CMMatrix33<T>::operator [] (int32t i) const
{
    ASSERT(i >= 0 && i <= 2);

    return (CMVector3D<T> &) m[i][0];
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T& CMMatrix33<T>::operator () (uint32t i, uint32t j)
{
    ASSERT(i>=0 && i<=2);
    ASSERT(j>=0 && j<=2);

    return m[i][j];
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T CMMatrix33<T>::operator () (uint32t i, uint32t j) const
{
    ASSERT(i>=0 && i<=2);
    ASSERT(j>=0 && j<=2);

    return m[i][j];
}


////////////////////////////////////////////////////////////////////////////
// Assignment Operators
template <class T>
M_INLINE CMMatrix33<T>& CMMatrix33<T>::operator = (const CMMatrix33<T> &mtx)
{
    memcpy(&_11, &mtx._11, sizeof(T)*9);
    
    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T>& CMMatrix33<T>::operator += (const CMMatrix33<T> &mtx)
{
    _11 += mtx._11; _12 += mtx._12; _13 += mtx._13;
    _21 += mtx._21; _22 += mtx._22; _23 += mtx._23;
    _31 += mtx._31; _32 += mtx._32; _33 += mtx._33;

    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T>& CMMatrix33<T>::operator -= (const CMMatrix33<T> &mtx)
{
    _11 -= mtx._11; _12 -= mtx._12; _13 -= mtx._13;
    _21 -= mtx._21; _22 -= mtx._22; _23 -= mtx._23;
    _31 -= mtx._31; _32 -= mtx._32; _33 -= mtx._33;

    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T>& CMMatrix33<T>::operator *= (const T & val)
{
    _11 *= val; _12 *= val; _13 *= val;
    _21 *= val; _22 *= val; _23 *= val;
    _31 *= val; _32 *= val; _33 *= val;

    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T>& CMMatrix33<T>::operator *= (const CMMatrix33<T>& mtx)
{
    CMMtx33Mult(*this,*this,mtx);

    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T>& CMMatrix33<T>::operator /= (const T & val)
{
    ASSERT(!mnear0(val));

    T inv = T(1)/val;

    _11 *= inv; _12 *= inv; _13 *= inv;
    _21 *= inv; _22 *= inv; _23 *= inv;
    _31 *= inv; _32 *= inv; _33 *= inv;

    return *this;
}



////////////////////////////////////////////////////////////////////////////
// Unary Operators
template <class T>
M_INLINE CMMatrix33<T> CMMatrix33<T>::operator + () const
{
    return *this;
}

// Negate
template <class T>
M_INLINE CMMatrix33<T> CMMatrix33<T>::operator - () const
{
    return CMMatrix33(  -_11, -_12, -_13,
                        -_21, -_22, -_23,
                        -_31, -_32, -_33);
}

// Binary Operators
template <class T>
M_INLINE CMMatrix33<T> CMMatrix33<T>::operator + ( const CMMatrix33 &mtx ) const
{
    return CMMatrix33(  _11 + mtx._11, _12 + mtx._12, _13 + mtx._13,
                        _21 + mtx._21, _22 + mtx._22, _23 + mtx._23,
                        _31 + mtx._31, _32 + mtx._32, _33 + mtx._33);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T> CMMatrix33<T>::operator - ( const CMMatrix33 &mtx) const
{
    return CMMatrix33(  _11 - mtx._11, _12 - mtx._12, _13 - mtx._13,
                        _21 - mtx._21, _21 - mtx._22, _23 - mtx._23,
                        _31 - mtx._31, _32 - mtx._32, _33 - mtx._33);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T> CMMatrix33<T>::operator * ( const T & val ) const
{
    return CMMatrix33(  _11*val, _12*val, _13*val,
                        _21*val, _22*val, _23*val,
                        _31*val, _32*val, _33*val);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T> CMMatrix33<T>::operator * ( const CMMatrix33<T> &mtx ) const
{
    CMMatrix33 mtxT;
    CMMtx33Mult(mtxT,*this,mtx);
    return mtxT;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix33<T> CMMatrix33<T>::operator / ( const T & val ) const
{
    ASSERT(!mnear0(val));
    T inv = T(1)/val;
    return CMMatrix33(  _11*val, _12*val, _13*val,
                        _21*val, _22*val, _23*val,
                        _31*val, _32*val, _33*val);
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMMatrix33<T>::operator == ( const CMMatrix33<T> & mtx ) const
{
    return 0 == memcmp(this, &mtx, sizeof(T)*9);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMMatrix33<T>::operator != ( const CMMatrix33<T> & mtx ) const
{
    return 0 != memcmp(this, &mtx, sizeof(T)*9);
}

///////////////////////////////////////////////////////////////////////////////////////
// Set Identity Matrix
template <class T>
void CMMtx33Identity(CMMatrix33<T> &M)
{
    memset(&pM->_11,0,sizeof(T)*9);

    pM._11 = pM.22 = pM.33 = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
boolt CMMtx33IsIdentity(const CMMatrix33<T> &M)
{
    return  pM->m[0][0] == T(1) && pM->m[0][1] == T(0) && pM->m[0][2] == T(0) &&
            pM->m[1][0] == T(0) && pM->m[1][1] == T(1) && pM->m[1][2] == T(0) &&
            pM->m[2][0] == T(0) && pM->m[2][1] == T(0) && pM->m[2][2] == T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// 4x4 Matrix Class Inline Functions
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 4x4 Matrix Class Constructors

// Default Constructor
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44()
{
    memset(&this->_11,0,sizeof(T)*16);
}

// Getting Pointer To Array
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(const T *const p)
{
    ASSERT(p != NULL);

    memcpy(&_11, p, sizeof(T)*16);
}

// Copy Constructor
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(const CMMatrix44<T> &mtx)
{
    memcpy(&_11, &mtx._11, sizeof(T)*16);
}

// Constructing Matrix From 3x3 Matrix
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(const CMMatrix33<T> &mtx)
{
    CMMatrix44();
    _44 = T(1);

    memcpy(&_11, &mtx._11, sizeof(T)*3);
    memcpy(&_21, &mtx._21, sizeof(T)*3);
    memcpy(&_31, &mtx._31, sizeof(T)*3);
}

// Constructing Matrix From 4D Vectors
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(const CMVector4D<T>& row1,const CMVector4D<T>& row2,const CMVector4D<T>& row3,
                                   const CMVector4D<T>& row4)
{
    memcpy(&_11, &row1.x, sizeof(T)*4);
    memcpy(&_21, &row2.x, sizeof(T)*4);
    memcpy(&_31, &row3.x, sizeof(T)*4);
    memcpy(&_41, &row4.x, sizeof(T)*4);
}

// Constructing Matrix From Four 3D Vectors
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(const CMVector3D<T>& row1, const CMVector3D<T>& row2, const CMVector3D<T>& row3,
                                   const CMVector3D<T>& row4)
{
    CMMatrix44();
    _44 = T(1);

    memcpy(&_11, &row1.x, sizeof(T)*3);
    memcpy(&_21, &row2.x, sizeof(T)*3);
    memcpy(&_31, &row3.x, sizeof(T)*3);
    memcpy(&_41, &row4.x, sizeof(T)*3);
}

// Constructing Matrix From 3D Vectors
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(const CMVector3D<T>& row1, const CMVector3D<T>& row2, const CMVector3D<T>& row3)
{
    CMMatrix44();
    _44 = T(1);

    memcpy(&_11, &row1.x, sizeof(T)*3);
    memcpy(&_21, &row2.x, sizeof(T)*3);
    memcpy(&_31, &row3.x, sizeof(T)*3);
}

// Building Matrix By Getting 9 Params Of 3x3 Matrix
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(T _11, T _12, T _13, T _21, T _22, T _23, T _31, T _32, T _33)
{
    CMMatrix44();
    m[3][3] = T(1);

    m[0][0] = _11;  m[0][1] = _12;  m[0][2] = _13;
    m[1][0] = _21;  m[1][1] = _22;  m[1][2] = _23;
    m[2][0] = _31;  m[2][1] = _32;  m[2][2] = _33;
}

// Building Matrix By Getting 16 Params Of 4x4 Matrix
template <class T>
M_INLINE CMMatrix44<T>::CMMatrix44(T _11, T _12, T _13, T _14, T _21, T _22, T _23, T _24, 
                                   T _31, T _32, T _33, T _34, T _41, T _42, T _43, T _44)
{
    m[0][0] = _11;  m[0][1] = _12;  m[0][2] = _13;  m[0][3] = _14;
    m[1][0] = _21;  m[1][1] = _22;  m[1][2] = _23;  m[1][3] = _24;
    m[2][0] = _31;  m[2][1] = _32;  m[2][2] = _33;  m[2][3] = _34;
    m[3][0] = _41;  m[3][1] = _42;  m[3][2] = _43;  m[3][3] = _44;
}

// Cast To Pointer  (T *)
template <class T>
M_INLINE CMMatrix44<T>::operator T* () const
{
    return (T*) &m[0][0];
}
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T>::operator const T* () const
{
    return (const T*) &m[0][0];
}

// Acces Grant
template <class T>
M_INLINE CMVector4D<T>& CMMatrix44<T>::operator [] (int32t i)
{
    ASSERT(i >= 0 && i <= 3);

    return (CMVector4D<T> &) m[i][0];
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMVector4D<T>& CMMatrix44<T>::operator [] (int32t i) const
{
    ASSERT(i >= 0 && i <= 3);

    return (CMVector4D<T> &) m[i][0];
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T& CMMatrix44<T>::operator () (int32t i, int32t j)
{
    ASSERT(i >= 0 && i <= 3);
    ASSERT(j >= 0 && j <= 3);

    return m[i][j];
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE T CMMatrix44<T>::operator () (int32t i, int32t j) const 
{
    ASSERT(i >= 0 && j <= 3);
    ASSERT(j >= 0 && j <= 3);

    return m[i][j];
}

////////////////////////////////////////////////////////////////////////////
// Assignment Operators
template <class T>
M_INLINE CMMatrix44<T>& CMMatrix44<T>::operator = (const CMMatrix44<T> &mtx)
{
    memcpy(&_11, &mtx._11, sizeof(T)*16);

    return *this;
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T>& CMMatrix44<T>::operator += (const CMMatrix44<T> &mtx)
{
    _11 += mtx._11; _12 += mtx._12; _13 += mtx._13; _14 += mtx._14;
    _21 += mtx._21; _22 += mtx._22; _23 += mtx._23; _24 += mtx._24;
    _31 += mtx._31; _32 += mtx._32; _33 += mtx._33; _34 += mtx._34;
    _41 += mtx._41; _42 += mtx._42; _43 += mtx._43; _44 += mtx._44;

    return *this;
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T>& CMMatrix44<T>::operator -= (const CMMatrix44<T> &mtx)
{
    _11 -= mtx._11; _12 -= mtx._12; _13 -= mtx._13; _14 -= mtx._14;
    _21 -= mtx._21; _22 -= mtx._22; _23 -= mtx._23; _24 -= mtx._24;
    _31 -= mtx._31; _32 -= mtx._32; _33 -= mtx._33; _34 -= mtx._34;
    _41 -= mtx._41; _42 -= mtx._42; _43 -= mtx._43; _44 -= mtx._44;

    return *this;
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T>& CMMatrix44<T>::operator *= (const T & val)
{
    _11 *= val; _12 *= val; _13 *= val; _14 *= val;
    _21 *= val; _22 *= val; _23 *= val; _24 *= val;
    _31 *= val; _32 *= val; _33 *= val; _34 *= val;
    _41 *= val; _42 *= val; _43 *= val; _44 *= val;

    return *this; 
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T>& CMMatrix44<T>::operator *= (const CMMatrix44<T>& mtx)
{   
    CMMtx44Mult(*this,*this,mtx);

    return *this;
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T>& CMMatrix44<T>::operator /= (const T & val)
{
    ASSERT(!mnear0(val));

    T inv = T(1)/val;

    _11 *= inv; _12 *= inv; _13 *= inv; _14 *= inv;
    _21 *= inv; _22 *= inv; _23 *= inv; _24 *= inv;
    _31 *= inv; _32 *= inv; _33 *= inv; _34 *= inv;
    _41 *= inv; _42 *= inv; _43 *= inv; _44 *= inv;

    return *this;
}

////////////////////////////////////////////////////////////////////////////
// Unary Operators
template <class T>
M_INLINE CMMatrix44<T> CMMatrix44<T>::operator + () const
{
    return *this;
}

// Negate
template <class T>
M_INLINE CMMatrix44<T> CMMatrix44<T>::operator - () const\
{
    return CMMatrix44(  -_11, -_12, -_13, -_14,
                        -_21, -_22, -_23, -_24,
                        -_31, -_32, -_33, -_34,
                        -_41, -_42, -_43, -_44  );
}

////////////////////////////////////////////////////////////////////////////
// Binary Operators
template <class T>
M_INLINE CMMatrix44<T> CMMatrix44<T>::operator + (const CMMatrix44 &mtx) const
{
    return CMMatrix44(  _11 + mtx._11,  _12 + mtx._12,  _13 + mtx._13,  _14 + mtx._14,
                        _21 + mtx._21,  _22 + mtx._22,  _23 + mtx._23,  _24 + mtx._24,
                        _31 + mtx._31,  _32 + mtx._32,  _33 + mtx._33,  _34 + mtx._34,
                        _41 + mtx._41,  _42 + mtx._42,  _43 + mtx._43,  _44 + mtx._44   );
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T> CMMatrix44<T>::operator - (const CMMatrix44 &mtx) const
{
    return CMMatrix44(  _11 - mtx._11,  _12 - mtx._12,  _13 - mtx._13,  _14 - mtx._14,
                        _21 - mtx._21,  _22 - mtx._22,  _23 - mtx._23,  _24 - mtx._24,
                        _31 - mtx._31,  _32 - mtx._32,  _33 - mtx._33,  _34 - mtx._34,
                        _41 - mtx._41,  _42 - mtx._42,  _43 - mtx._43,  _44 - mtx._44   );
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T> CMMatrix44<T>::operator * (const T & val) const
{
    return CMMatrix44(  _11*val, _12*val, _13*val, _14*val,
                        _21*val, _22*val, _23*val, _24*val,
                        _31*val, _32*val, _33*val, _34*val,
                        _41*val, _42*val, _43*val, _44*val  );
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T> CMMatrix44<T>::operator * (const CMMatrix44<T> &mtx) const
{
    CMMatrix44 mtxT;
    CMMtx44Mult(mtxT,*this,mtx);
    return mtxT;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix44<T> CMMatrix44<T>::operator / (const T & val) const
{
    ASSERT(!mnear0(val));
    T inv = T(1)/val;
    return CMMatrix44(  _11*inv, _12*inv, _13*inv, _14*inv,
                        _21*inv, _22*inv, _23*inv, _24*inv,
                        _31*inv, _32*inv, _33*inv, _34*inv,
                        _41*inv, _42*inv, _43*inv, _44*inv  );
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMMatrix44<T>::operator == (const CMMatrix44<T> &mtx) const
{
    return 0 == memcmp(this, &mtx, sizeof(T)*16);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMMatrix44<T>::operator != (const CMMatrix44<T> &mtx) const
{
    return 0 != memcmp(this, &mtx, sizeof(T)*16);
}

///////////////////////////////////////////////////////////////////////////////////////
// Set Identity Matrix
template <class T>
M_INLINE void CMMtx44Identity(CMMatrix44<T> &M)
{
    M.m[0][1] = M.m[0][2] = M.m[0][3] =   
    M.m[1][0] = M.m[1][2] = M.m[1][3] = 
    M.m[2][0] = M.m[2][1] = M.m[2][3] = 
    M.m[3][0] = M.m[3][1] = M.m[3][2] = T(0);

    M.m[0][0] = M.m[1][1] = M.m[2][2] = M.m[3][3] = T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
M_INLINE boolt CMMtx44IsIdentity(const CMMatrix44<T> &M)
{
    return  M._11 == T(1) && M._12 == T(0) && M._13 == T(0) && M._14 == T(0) &&
            M._21 == T(0) && M._22 == T(1) && M._23 == T(0) && M._24 == T(0) &&
            M._31 == T(0) && M._32 == T(0) && M._33 == T(1) && M._34 == T(0) &&
            M._41 == T(0) && M._42 == T(0) && M._43 == T(0) && M._44 == T(1);
}

///////////////////////////////////////////////////////////////////////////////////////
// Setting Matrix Row
template <class T>
M_INLINE void CMMtx44Set(int row, CMMatrix44<T> &M, const CMVector3D<T> &vec, T w)
{
    ASSERT(row >= 0 && row <= 3);
  
    M[row][0] = vec.x;
    M[row][1] = vec.y;
    M[row][2] = vec.z;
    M[row][3] = w;
}

///////////////////////////////////////////////////////////////////////////////////////
// Setting Matrix Row
template <class T>
M_INLINE void CMMtx44Set(int row, CMMatrix44<T> &M, const CMVector4D<T> &vec)
{
    ASSERT(row >= 0 && row <= 3);

    M[row][0] = vec.x;
    M[row][1] = vec.y;
    M[row][2] = vec.z;
    M[row][3] = vec.w
}

///////////////////////////////////////////////////////////////////////////////////////
// 3x4 Matrix Class Inline Functions
//////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 3x4 Matrix Class Constructors

// Default Constructor
template <class T>
M_INLINE CMMatrix34<T>::CMMatrix34(){}

// Bulding Matrix From 3x3 Matrix And 3D Vector
template <class T>
M_INLINE CMMatrix34<T>::CMMatrix34(const CMMatrix33<T> &mtx33, const CMVector3D<T> &vec)
{
    memcpy(&this->M._11,&mtx33._11, sizeof(T)*9);
    memcpy(&this->V.v[0], &vec.v[0], sizeof(T)*3);
}

// Copy Constructor
template <class T>
M_INLINE CMMatrix34<T>::CMMatrix34(const CMMatrix34<T>& mtx34)
{
    memcpy(&this->M._11,&mtx34.M._11, sizeof(T)*9);
    memcpy(&this->V.v[0], &mtx34.V.v[0], sizeof(T)*3);
}

// Building Matrix By Getting 9 Matrix Values And 3 Vector Values
template <class T>
M_INLINE CMMatrix34<T>::CMMatrix34(T m11, T m12, T m13, T m21, T m22, T m23, T m31, T m32, T m33, 
                                   T vx, T vy, T vz)
{
    M._11 = m11;    M._12 = m12;    M._13 = m13;
    M._21 = m21;    M._22 = m22;    M._23 = m23;
    M._31 = m31;    M._32 = m32;    M._33 = m33;

    V.x = vx;   V.y = vy;   V.z = vz;
}

// Access Grants
template <class T>
M_INLINE CMVector3D<T>& CMMatrix34<T>::operator [] (int32t i)
{
    ASSERT(i >= 0 && i <= 3);

    if(i < 3)
        return M[i];
    else
        return V;
}

template <class T>
M_INLINE CMVector3D<T>& CMMatrix34<T>::operator [] (int32t i) const
{
    ASSERT(i >= 0 && i <= 3);

    if(i < 3)
        return M[i];
    else
        return (CMVector3D<T> &)V;
}

////////////////////////////////////////////////////////////////////////////
// Assignment Operators
template <class T>
M_INLINE CMMatrix34<T>& CMMatrix34<T>::operator = (const CMMatrix34<T> &mtx34)
{
    M = mtx34.M;
    V = mtx34.V;

    return *this;
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T>& CMMatrix34<T>::operator += ( const CMMatrix34<T> &mtx34 )
{
    M += mtx34.M;
    V += mtx34.V;

    return *this;
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T>& CMMatrix34<T>::operator -= ( const CMMatrix34<T> &mtx34 )
{
    M -= mtx34.M;
    V -= mtx34.V;

    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T>& CMMatrix34<T>::operator *= ( const CMMatrix34<T> &mtx34 )
{
    CMMtx34Mult(*this,*this,mtx34);

    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T>& CMMatrix34<T>::operator *= ( const T &val )
{
    M *= val;
    V *= val;

    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T>& CMMatrix34<T>::operator /= ( const T &val )
{
    ASSERT(!mnear0(val));

    T inv = T(1)/val;

    M *= inv;
    V *= inv;

    return *this;
}

////////////////////////////////////////////////////////////////////////////
// Unary Operators
template <class T>
M_INLINE CMMatrix34<T> CMMatrix34<T>::operator + () const
{
    return *this;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T> CMMatrix34<T>::operator - () const
{
    return CMMatrix34(  -M._11, -M._12, -M._13,
                        -M._21, -M._22, -M._23,
                        -M._31, -M._32, -M._33,
                        -V.x, -V.y, -V.z);
}

////////////////////////////////////////////////////////////////////////////   
// Binary Operators
template <class T>
M_INLINE CMMatrix34<T> CMMatrix34<T>::operator + ( const CMMatrix34<T> &mtx34 ) const
{
    return CMMatrix34(  M._11 + mtx34.M._11, M._12 + mtx34.M._12, M._13 + mtx34.M._13,
                        M._21 + mtx34.M._21, M._22 + mtx34.M._22, M._23 + mtx34.M._23,
                        M._31 + mtx34.M._21, M._32 + mtx34.M._22, M._33 + mtx34.M._23,
                        V.x + mtx34.V.x,  V.y + mtx34.V.y,  V.z + mtx34.V.z);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T> CMMatrix34<T>::operator - ( const CMMatrix34<T> &mtx34 ) const
{
    return CMMatrix34(  M._11 - mtx34.M._11, M._12 - mtx34.M._12, M._13 - mtx34.M._13,
                        M._21 - mtx34.M._21, M._22 - mtx34.M._22, M._23 - mtx34.M._23,
                        M._31 - mtx34.M._21, M._32 - mtx34.M._22, M._33 - mtx34.M._23,
                        V.x - mtx34.V.x,  V.y - mtx34.V.y,  V.z - mtx34.V.z);
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T> CMMatrix34<T>::operator * ( const CMMatrix34<T> &mtx34 ) const
{
    CMMatrix34 mtx;
    CMMtx34Mult(mtx,*this,mtx34);
    return mtx;
}

////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T> CMMatrix34<T>::operator * ( const T &val ) const
{
    CMMatrix34 mtx;
    mtx.M._11 = M._11*val;  mtx.M._12 = M._12*val;  mtx.M._13 = M._13*val;
    mtx.M._21 = M._21*val;  mtx.M._22 = M._22*val;  mtx.M._23 = M._23*val;
    mtx.M._31 = M._31*val;  mtx.M._32 = M._32*val;  mtx.M._33 = M._33*val;
    mtx.V.x = V.x*val;  mtx.V.y = V.y*val;  mtx.V.z = V.z*val;

    return mtx;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE CMMatrix34<T> CMMatrix34<T>::operator / ( const T &val ) const
{
    ASSERT(!mnear0(val));
    T inv = T(1)/val;
    CMMatrix34 mtx;

    mtx.M._11 = M._11*inv;  mtx.M._12 = M._12*inv;  mtx.M._13 = M._13*inv;
    mtx.M._21 = M._21*inv;  mtx.M._22 = M._22*inv;  mtx.M._23 = M._23*inv;
    mtx.M._31 = M._31*inv;  mtx.M._32 = M._32*inv;  mtx.M._33 = M._33*inv;
    mtx.V.x = V.x*inv;  mtx.V.y = V.y*inv;  mtx.V.z = V.z*inv;

    return mtx;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMMatrix34<T>::operator == ( const CMMatrix34<T> &mtx34 ) const
{
    return M == mtx34.M && V == mtx34.V;
}
////////////////////////////////////////////////////////////////////////////
template <class T>
M_INLINE boolt CMMatrix34<T>::operator != ( const CMMatrix34<T> &mtx34 ) const
{
    return M != mtx34.M || V != mtx34.V;
}
  
///////////////////////////////////////////////////////////////////////////////////////
// Set Identity Matrix
template <class T>
M_INLINE void CMMtx34Identity(CMMatrix34<T> &M)
{
    CMMtx33Identity(&M.M);
}

///////////////////////////////////////////////////////////////////////////////////////
// Is Identity
template <class T>
M_INLINE boolt CMMtx34IsIdentity(const CMMatrix34<T> &M)
{
    return CMMtx33IsIdentity(M.M);
}

