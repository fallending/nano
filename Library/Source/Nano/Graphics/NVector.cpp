/*	NAME:
		NVector.cpp

	DESCRIPTION:
		Vector object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NString.h"

#ifndef NVECTOR_CPP

#include "NEncoder.h"
#include "NVector.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NVector kNVectorNorth									= NVector( 0.0f,  1.0f);
const NVector kNVectorSouth									= NVector( 0.0f, -1.0f);
const NVector kNVectorEast									= NVector( 1.0f,  0.0f);
const NVector kNVectorWest									= NVector(-1.0f,  0.0f);





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNVectorXKey									= "x";
static const NString kNVectorYKey									= "y";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NVector);





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(const NVector32 &theVector)
		: NVector32(theVector.x, theVector.y)
{
}





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(const NVector64 &theVector)
		: NVector32(theVector.x, theVector.y)
{
}





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(Float32 x, Float32 y)
		: NVector32(x, y)
{
}





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(Float64 x, Float64 y)
		: NVector32(x, y)
{
}





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(const NPoint32 &point1, const NPoint32 &point2)
		: NVector32(point2.x - point1.x, point2.y - point1.y)
{
}





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(const NPoint64 &point1, const NPoint64 &point2)
		: NVector32(point2.x - point1.x, point2.y - point1.y)
{
}





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(void)
{
}





//============================================================================
//		NVector::~NVector : Destructor.
//----------------------------------------------------------------------------
NVector::~NVector(void)
{
}





//============================================================================
//		NVector::NVector64 : NVector64 operator.
//----------------------------------------------------------------------------
NVector::operator NVector64(void) const
{	NVector64		theResult;



	// Get the value
	theResult.x = x;
	theResult.y = y;
	
	return(theResult);
}





//============================================================================
//      NVector::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NVector::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNVectorXKey, x);
	theEncoder.EncodeNumber(kNVectorYKey, y);
}





//============================================================================
//      NVector::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NVector::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	x = theEncoder.DecodeNumber(kNVectorXKey).GetFloat32();
	y = theEncoder.DecodeNumber(kNVectorYKey).GetFloat32();
}





#else

//============================================================================
//		NVectorT::NVectorT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NVectorT<T>::NVectorT(T valX, T valY)
{


	// Initialize ourselves
	x = valX;
	y = valY;
}





//============================================================================
//		NVectorT::NVectorT : Constructor.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::NVectorT(const NPointT<T> &point1, const NPointT<T> &point2)
{


	// Initialize ourselves
	x = point2.x - point1.x;
	y = point2.y - point1.y;
}





//============================================================================
//		NVectorT::NVectorT : Constructor.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::NVectorT(void)
{


	// Initialize ourselves
	x = 0;
	y = 0;
}





//============================================================================
//		NVectorT::~NVectorT : Destructor.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::~NVectorT(void)
{
}





//============================================================================
//		NVectorT::Clear : Clear the Vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Clear(void)
{


	// Clear the vector
	x = 0;
	y = 0;
}





//============================================================================
//		NVectorT::IsZero : Is the vector zero?
//----------------------------------------------------------------------------
template<class T> bool NVectorT<T>::IsZero(void) const
{


	// Test the vector
	return(NMathUtilities::IsZero(x) && NMathUtilities::IsZero(y));
}





//============================================================================
//		NVectorT::IsNormalized : Is the vector normalized?
//----------------------------------------------------------------------------
template<class T> bool NVectorT<T>::IsNormalized(void) const
{


	// Test the vector
	return(NMathUtilities::AreEqual(GetLength(), 1.0));
}





//============================================================================
//		NVectorT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NVectorT<T>::Compare(const NVectorT<T> &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(x, theValue.x);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(y, theValue.y);
	
	return(theResult);
}





//============================================================================
//		NVectorT::Normalize : Normalize the vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Normalize(void)
{


	// Update our state
	*this = GetNormalized();
}





//============================================================================
//		NVectorT::Negate : Negate the vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Negate(void)
{


	// Update our state
	*this = GetNegated();
}





//============================================================================
//		NVectorT::Scale : Scale the vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Scale(T scaleBy)
{


	// Update our state
	*this = GetScaled(scaleBy);
}





//============================================================================
//		NVectorT::GetNormalized : Get the normalized vector.
//----------------------------------------------------------------------------
template<class T> NVectorT<T> NVectorT<T>::GetNormalized(void) const
{	NVectorT<T>		theResult;
	T				invLength;



	// Normalize the vector
	invLength = 1.0 / GetLength();

	theResult.x = x * invLength;
	theResult.y = y * invLength;

	return(theResult);
}





//============================================================================
//		NVectorT::GetNegated : Get the negated vector.
//----------------------------------------------------------------------------
template<class T> NVectorT<T> NVectorT<T>::GetNegated(void) const
{	NVectorT<T>		theResult;



	// Negate the vector
	theResult.x = -x;
	theResult.y = -y;
	
	return(theResult);
}





//============================================================================
//		NVectorT::GetScaled : Get the scaled vector.
//----------------------------------------------------------------------------
template<class T> NVectorT<T> NVectorT<T>::GetScaled(T scaleBy) const
{	NVectorT<T>		theResult;



	// Scale the vector
	theResult.x = x * scaleBy;
	theResult.y = y * scaleBy;
	
	return(theResult);
}





//============================================================================
//		NVectorT::GetLength : Get the length.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetLength(bool getApprox) const
{	T	theLength;



	// Get the length
	theLength = GetLength2();
	
	if (getApprox)
		theLength = NMathUtilities::FastRoot(theLength);
	else
		theLength = sqrt(theLength);
	
	return(theLength);
}





//============================================================================
//		NVectorT::GetLength2 : Get the length^2.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetLength2(void) const
{


	// Get the length^2
	return((x * x) + (y * y));
}





//============================================================================
//		NVectorT::GetDot : Get the dot product.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetDot(const NVectorT<T> &theVector) const
{


	// Get the dot product
	return((x * theVector.x) + (y * theVector.y));
}





//============================================================================
//		NVectorT::GetCross : Get the cross product.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetCross(const NVectorT<T> &theVector) const
{


	// Get the dot product
	return((x * theVector.y) - (y * theVector.x));
}





//============================================================================
//		NVectorT::GetAngle : Get the angle between two vectors.
//----------------------------------------------------------------------------
template<class T> NDegrees NVectorT<T>::GetAngle(const NVectorT<T> &theVector) const
{	T				dotProduct, crossProduct, angleRad;
	NVectorT<T>		normal1, normal2;



	// Get the state we need
	normal1 = *this;
	normal1.Normalize();
	
	normal2 = theVector;
	normal2.Normalize();



	// Calculate the angle
	dotProduct   = normal1.GetDot(  normal2);
	crossProduct = normal1.GetCross(normal2);
	angleRad     = atan2(crossProduct, dotProduct);
	
	return(NMathUtilities::GetDegrees(angleRad));
}





//============================================================================
//		NVectorT::+= : Addition operator.
//----------------------------------------------------------------------------
template<class T> const NVectorT<T>& NVectorT<T>::operator += (const NVectorT<T> &theVector)
{


	// Add the vector
	x += theVector.x;
	y += theVector.y;

	return(*this);
}





//============================================================================
//		NVectorT::-= : Subtraction operator.
//----------------------------------------------------------------------------
template<class T> const NVectorT<T>& NVectorT<T>::operator -= (const NVectorT<T> &theVector)
{


	// Subtract the vector
	x -= theVector.x;
	y -= theVector.y;

	return(*this);
}





//============================================================================
//		NVectorT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{x=%g, y=%g}", x, y);

	return(theResult);
}



#endif // NVECTOR_CPP



