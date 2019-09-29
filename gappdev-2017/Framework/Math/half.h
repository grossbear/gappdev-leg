///////////////////////////////////////////////////////////////////////////////////////
//  half.h
//
//  Half Precision Floating Point Number
//  
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _HALF_H_
#define _HALF_H_

//-------------------------------------------------------------------------
// Limits
//
// Visual C++ will complain if HALF_MIN, HALF_NRM_MIN etc. are not float
// constants, but at least one other compiler (gcc 2.96) produces incorrect
// results if they are.
//-------------------------------------------------------------------------
/*
#if (defined _WIN32 || defined _WIN64) && defined _MSC_VER

  #define HALF_MIN	5.96046448e-08f	// Smallest positive half

  #define HALF_NRM_MIN	6.10351562e-05f	// Smallest positive normalized half

  #define HALF_MAX	65504.0f	// Largest positive half

  #define HALF_EPSILON	0.00097656f	// Smallest positive e for which
					// half (1.0 + e) != half (1.0)
#else

  #define HALF_MIN	5.96046448e-08	// Smallest positive half

  #define HALF_NRM_MIN	6.10351562e-05	// Smallest positive normalized half

  #define HALF_MAX	65504.0		// Largest positive half

  #define HALF_EPSILON	0.00097656	// Smallest positive e for which
					// half (1.0 + e) != half (1.0)
#endif


#define HALF_MANT_DIG	11		// Number of digits in mantissa
					// (significand + hidden leading 1)

#define HALF_DIG	2		// Number of base 10 digits that
					// can be represented without change

#define HALF_RADIX	2		// Base of the exponent

#define HALF_MIN_EXP	-13		// Minimum negative integer such that
					// HALF_RADIX raised to the power of
					// one less than that integer is a
					// normalized half

#define HALF_MAX_EXP	16		// Maximum positive integer such that
					// HALF_RADIX raised to the power of
					// one less than that integer is a
					// normalized half

#define HALF_MIN_10_EXP	-4		// Minimum positive integer such
					// that 10 raised to that power is
					// a normalized half

#define HALF_MAX_10_EXP	4		// Maximum positive integer such
					// that 10 raised to that power is
					// a normalized half

*/
//---------------------------------------------------------------------------
//
// Implementation --
//
// Representation of a float:
//
//	We assume that a float, f, is an IEEE 754 single-precision
//	floating point number, whose bits are arranged as follows:
//
//	    31 (msb)
//	    | 
//	    | 30     23
//	    | |      | 
//	    | |      | 22                    0 (lsb)
//	    | |      | |                     |
//	    X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
//
//	    s e        m
//
//	S is the sign-bit, e is the exponent and m is the significand.
//
//	If e is between 1 and 254, f is a normalized number:
//
//	            s    e-127
//	    f = (-1)  * 2      * 1.m
//
//	If e is 0, and m is not zero, f is a denormalized number:
//
//	            s    -126
//	    f = (-1)  * 2      * 0.m
//
//	If e and m are both zero, f is zero:
//
//	    f = 0.0
//
//	If e is 255, f is an "infinity" or "not a number" (NAN),
//	depending on whether m is zero or not.
//
//	Examples:
//
//	    0 00000000 00000000000000000000000 = 0.0
//	    0 01111110 00000000000000000000000 = 0.5
//	    0 01111111 00000000000000000000000 = 1.0
//	    0 10000000 00000000000000000000000 = 2.0
//	    0 10000000 10000000000000000000000 = 3.0
//	    1 10000101 11110000010000000000000 = -124.0625
//	    0 11111111 00000000000000000000000 = +infinity
//	    1 11111111 00000000000000000000000 = -infinity
//	    0 11111111 10000000000000000000000 = NAN
//	    1 11111111 11111111111111111111111 = NAN
//
// Representation of a half:
//
//	Here is the bit-layout for a half number, h:
//
//	    15 (msb)
//	    | 
//	    | 14  10
//	    | |   |
//	    | |   | 9        0 (lsb)
//	    | |   | |        |
//	    X XXXXX XXXXXXXXXX
//
//	    s e     m
//
//	S is the sign-bit, e is the exponent and m is the significand.
//
//	If e is between 1 and 30, h is a normalized number:
//
//	            s    e-15
//	    h = (-1)  * 2     * 1.m
//
//	If e is 0, and m is not zero, h is a denormalized number:
//
//	            S    -14
//	    h = (-1)  * 2     * 0.m
//
//	If e and m are both zero, h is zero:
//
//	    h = 0.0
//
//	If e is 31, h is an "infinity" or "not a number" (NAN),
//	depending on whether m is zero or not.
//
//	Examples:
//
//	    0 00000 0000000000 = 0.0
//	    0 01110 0000000000 = 0.5
//	    0 01111 0000000000 = 1.0
//	    0 10000 0000000000 = 2.0
//	    0 10000 1000000000 = 3.0
//	    1 10101 1111000001 = -124.0625
//	    0 11111 0000000000 = +infinity
//	    1 11111 0000000000 = -infinity
//	    0 11111 1000000000 = NAN
//	    1 11111 1111111111 = NAN
//
// Conversion:
//
//	Converting from a float to a half requires some non-trivial bit
//	manipulations.  In some cases, this makes conversion relatively
//	slow, but the most common case is accelerated via table lookups.
//
//	Converting back from a half to a float is easier because we don't
//	have to do any rounding.  In addition, there are only 65536
//	different half numbers; we can convert each of those numbers once
//	and store the results in a table.  Later, all conversions can be
//	done using only simple table lookups.
//
//---------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////////////
// Converting Float Value To Half Precision Value
uint16t mftoh(float f);
///////////////////////////////////////////////////////////////////////////////////////
uint16t mftoh_f(float f);
///////////////////////////////////////////////////////////////////////////////////////
// Converting Array Of Float Values To Half Values
void mftoh(uint16t *h, const float *f, int32t num);

///////////////////////////////////////////////////////////////////////////////////////
// Converting Half Value To Float Value
float mhtof(uint16t h);
///////////////////////////////////////////////////////////////////////////////////////
float mhtof2(uint16t h);
///////////////////////////////////////////////////////////////////////////////////////
// Converting Array Of Half Values To Float Values
void mhtof(float *f, const uint16t *h, int32t num);

///////////////////////////////////////////////////////////////////////////////////////
// Making Half Number
M_API uint16t   mmakeh(int32t sign, int32t exp, int32t mant);
///////////////////////////////////////////////////////////////////////////////////////
// Splitting Half Number
M_API void      msplith(uint16t h, int32t &sign, int32t &exp, int32t &mant);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Testing Value Functions
///////////////////////////////////////////////////////////////////////////////////////
// Test If Not A Number
boolt misnan(uint16t h);
///////////////////////////////////////////////////////////////////////////////////////
boolt mqnan(uint16t h);
///////////////////////////////////////////////////////////////////////////////////////
boolt msnan(uint16t h);

///////////////////////////////////////////////////////////////////////////////////////
// Is Infinity
boolt misinf(uint16t h);
///////////////////////////////////////////////////////////////////////////////////////
// Is Positive Infinity
boolt mpinf(uint16t h);
///////////////////////////////////////////////////////////////////////////////////////
// Is Negative Infinity
boolt mninf(uint16t h);


#endif //_HALF_H_