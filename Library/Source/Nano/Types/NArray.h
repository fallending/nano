/*	NAME:
		NArray.h

	DESCRIPTION:
		Array object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NARRAY_HDR
#define NARRAY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NSharedValue.h"
#include "NContainer.h"
#include "NVariant.h"
#include "NComparable.h"
#include "NDebuggable.h"
#include "NEncodable.h"
#include "NRange.h"
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NVector.h"
#include "NString.h"
#include "NColor.h"
#include "NDate.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NDictionary;


// Functors
typedef nfunctor<NComparison (const NVariant &value1, const NVariant &value2)>	NArrayCompareFunctor;
typedef nfunctor<void        (NIndex theIndex, const NVariant &theValue)>		NArrayForEachFunctor;


// Lists
typedef std::vector<NVariant>										NArrayValue;
typedef NArrayValue::iterator										NArrayValueIterator;
typedef NArrayValue::const_iterator									NArrayValueConstIterator;


// Value
typedef NSharedValue<NArrayValue>									NSharedValueArray;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NArray :	public NContainer,
				public NEncodable,
				public NDebuggable,
				public NComparable<NArray>,
				public NSharedValueArray {
public:
										NENCODABLE_DECLARE(NArray);

										NArray(const Int32List   &theValues);
										NArray(const Int64List   &theValues);
										NArray(const Float32List &theValues);
										NArray(const Float64List &theValues);
										NArray(const NStringList &theValues);

										NArray(void);
	virtual							   ~NArray(void);


	// Get the size
	NIndex								GetSize(void) const;


	// Compare the value
	NComparison							Compare(const NArray &theValue) const;


	// Join two arrays
	void								Join(const NArray &theValue);


	// Sort the array
	//
	// If no comparison functor is provided, values are sorted by NVariant::Compare.
	void								Sort(const NArrayCompareFunctor &theFunctor=NULL, const NRange &theRange=kNRangeAll);


	// Process each item
	void								ForEach(const NArrayForEachFunctor &theFunctor, const NRange &theRange=kNRangeAll) const;


	// Does a value exist?
	//
	// If no comparison functor is provided, values are compared by NVariant::Compare.
	bool								HasValue(const NVariant &theValue, const NArrayCompareFunctor &theFunctor=NULL) const;
	

	// Get/set a value
	NVariant							GetValue(NIndex theIndex) const;
	void								SetValue(NIndex theIndex, const NVariant &theValue);


	// Append a value
	void								AppendValue(const NVariant &theValue);


	// Get a type'd value
	//
	// If the value can not be returned as the specified type, 0/empty is returned.
	bool								GetValueBoolean(   NIndex theIndex) const;
	int32_t								GetValueInt32(     NIndex theIndex) const;
	int64_t								GetValueInt64(     NIndex theIndex) const;
	float32_t							GetValueFloat32(   NIndex theIndex) const;
	float64_t							GetValueFloat64(   NIndex theIndex) const;
	NString								GetValueString(    NIndex theIndex) const;
	NData								GetValueData(      NIndex theIndex) const;
	NDate								GetValueDate(      NIndex theIndex) const;
	NColor								GetValueColor(     NIndex theIndex) const;
	NPoint								GetValuePoint(     NIndex theIndex) const;
	NSize								GetValueSize(      NIndex theIndex) const;
	NRectangle							GetValueRectangle( NIndex theIndex) const;
	NVector								GetValueVector(    NIndex theIndex) const;
	NArray								GetValueArray(     NIndex theIndex) const;
	NDictionary							GetValueDictionary(NIndex theIndex) const;


	// Get/set an array of values
	Int32List							GetValuesInt32(  void) const;
	Int64List							GetValuesInt64(  void) const;
	Float32List							GetValuesFloat32(void) const;
	Float64List							GetValuesFloat64(void) const;
	NStringList							GetValuesString( void) const;

	void								SetValuesInt32(  const Int32List   &theValues);
	void								SetValuesInt64(  const Int64List   &theValues);
	void								SetValuesFloat32(const Float32List &theValues);
	void								SetValuesFloat64(const Float64List &theValues);
	void								SetValuesString( const NStringList &theValues);


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Get the null value
	const NArrayValue					*GetNullValue(void) const;


	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	void								ValueChanged(NArrayValue *theValue);

	NArrayCompareFunctor				GetCompareFunctor(const NArrayCompareFunctor &theFunctor) const;
	NString								GetDebugID(NIndex theIndex)                               const;


private:


};





#endif // NARRAY_HDR


