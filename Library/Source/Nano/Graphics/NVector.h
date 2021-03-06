/*	NAME:
		NVector.h

	DESCRIPTION:
		Vector object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NVECTOR_HDR
#define NVECTOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NVariant.h"
#include "NPoint.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
template <class T> class NVectorT;

typedef NVectorT<float64_t> NVector64;
typedef NVectorT<float32_t> NVector32;
typedef NVector32           NVector;


// Lists
typedef std::vector<NVector64>										NVector64List;
typedef NVector64List::iterator										NVector64ListIterator;
typedef NVector64List::const_iterator								NVector64ListConstIterator;

typedef std::vector<NVector32>										NVector32List;
typedef NVector32List::iterator										NVector32ListIterator;
typedef NVector32List::const_iterator								NVector32ListConstIterator;

typedef std::vector<NVector>										NVectorList;
typedef NVectorList::iterator										NVectorListIterator;
typedef NVectorList::const_iterator									NVectorListConstIterator;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
extern const NVector kNVectorNorth;
extern const NVector kNVectorSouth;
extern const NVector kNVectorEast;
extern const NVector kNVectorWest;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NVectorT {
public:
										NVectorT(const NVariant &theValue);

										NVectorT(const NPointT<T> &point1, const NPointT<T> &point2);
										NVectorT(T x, T y);
										NVectorT(void);


	// Clear the vector
	void								Clear(void);


	// Test the vector
	bool								IsZero      (void) const;
	bool								IsNormalized(void) const;


	// Compare the value
	NComparison							Compare(const NVectorT<T> &theValue) const;


	// Manipulate the vector
	void								Normalize(void);
	void								Negate(void);
	void								Scale(T scaleBy);

	NVectorT<T>							GetNormalized(void)  const;
	NVectorT<T>							GetNegated(void)     const;
	NVectorT<T>							GetScaled(T scaleBy) const;


	// Get the length
	T									GetLength (void) const;
	T									GetLength2(void) const;


	// Get the dot/cross products
	T									GetDot(  const NVectorT<T> &theVector) const;
	T									GetCross(const NVectorT<T> &theVector) const;


	// Get the angle between two vectors
	NDegrees							GetAngle(const NVectorT<T> &theVector) const;


	// Operators
	NCOMPARABLE_OPERATORS(NVectorT<T>)

	const NVectorT<T>&					operator +=(const NVectorT<T> &theVector);
	const NVectorT<T>&					operator -=(const NVectorT<T> &theVector);

										operator NEncodable(     void) const;
										operator NFormatArgument(void) const;


public:
	T									x;
	T									y;
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NVECTOR_CPP
#include "NVector.cpp"
#undef    NVECTOR_CPP









#endif // NVECTOR_HDR


