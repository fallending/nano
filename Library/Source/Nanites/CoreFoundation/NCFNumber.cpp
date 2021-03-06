/*	NAME:
		NCFNumber.cpp

	DESCRIPTION:
		CFNumberRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFNumber.h"





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(const NVariant &theValue)
			: NNumber(theValue)
{
}





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(const NNumber &theNumber)
			: NNumber(theNumber)
{
}





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(CFNumberRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFNumber::NCFNumber : Constructor.
//----------------------------------------------------------------------------
NCFNumber::NCFNumber(void)
{
}





//============================================================================
//		NCFNumber::~NCFNumber : Destructor.
//----------------------------------------------------------------------------
NCFNumber::~NCFNumber(void)
{
}





//============================================================================
//		NCFNumber::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFNumber::GetObject(void) const
{	float64_t		valueFloat64;
	float32_t		valueFloat32;
	int64_t			valueInt64;
	NCFObject		theObject;



	// Get the object
	switch (GetPrecision()) {
		case kNPrecisionInt8:
		case kNPrecisionInt16:
		case kNPrecisionInt32:
		case kNPrecisionInt64:
			valueInt64 = GetInt64();
			theObject.SetObject(CFNumberCreate(kCFAllocatorNano, kCFNumberLongLongType, &valueInt64));
			break;
		
		case kNPrecisionFloat32:
			valueFloat32 = GetFloat32();
			theObject.SetObject(CFNumberCreate(kCFAllocatorNano, kCFNumberFloatType,    &valueFloat32));
			break;
		
		case kNPrecisionFloat64:
			valueFloat64 = GetFloat64();
			theObject.SetObject(CFNumberCreate(kCFAllocatorNano, kCFNumberDoubleType,   &valueFloat64));
			break;
		
		default:
			NN_LOG("Unable to convert '%@' to CFNumber", GetString());
			break;
		}

	return(theObject);
}





//============================================================================
//		NCFNumber::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFNumber::SetObject(CFNumberRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	float64_t		valueFloat64;
	float32_t		valueFloat32;
	int64_t			valueInt64;
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	SetInt8(0);



	// Set the object
	if (isValid)
		{
		if (CFNumberGetValue(     cfObject, kCFNumberLongLongType, &valueInt64))
			SetInt64(valueInt64);
		
		else if (CFNumberGetValue(cfObject, kCFNumberFloatType,    &valueFloat32))
			SetFloat32(valueFloat32);
		
		else if (CFNumberGetValue(cfObject, kCFNumberDoubleType,   &valueFloat64))
			SetFloat64(valueFloat64);

		else
			NN_LOG("Unable to convert CFNumber to NNumber");
		}

	return(isValid);
}


