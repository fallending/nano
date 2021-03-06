/*	NAME:
		NDBQuery.cpp

	DESCRIPTION:
		Database query.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDBQuery.h"





//============================================================================
//		NDBQuery::NDBQuery : Constructor.
//----------------------------------------------------------------------------
NDBQuery::NDBQuery(const NString &theValue)
{


	// Initialize ourselves
	SetValue(theValue);
}





//============================================================================
//		NDBQuery::NDBQuery : Constructor.
//----------------------------------------------------------------------------
NDBQuery::NDBQuery(const char *theValue)
{


	// Initialize ourselves
	SetValue(theValue);
}





//============================================================================
//		NDBQuery::NDBQuery : Constructor.
//----------------------------------------------------------------------------
NDBQuery::NDBQuery(void)
{
}





//============================================================================
//		NDBQuery::~NDBQuery : Destructor.
//----------------------------------------------------------------------------
NDBQuery::~NDBQuery(void)
{
}





//============================================================================
//		NDBQuery::GetValue : Get the value.
//----------------------------------------------------------------------------
NString NDBQuery::GetValue(void) const
{


	// Get the value
	return(mValue);
}





//============================================================================
//		NDBQuery::SetValue : Set the value.
//----------------------------------------------------------------------------
void NDBQuery::SetValue(const NString &theValue)
{


	// Validate our parameters
	NN_ASSERT(!theValue.IsEmpty());



	// Set the value
	mValue = theValue;
}





//============================================================================
//		NDBQuery::GetParameters : Get the parameters.
//----------------------------------------------------------------------------
NVariant NDBQuery::GetParameters(void) const
{


	// Get the parameters
	return(mParameters);
}





//============================================================================
//		NDBQuery::SetParameters : Set the parameters.
//----------------------------------------------------------------------------
void NDBQuery::SetParameters(const NVariant &theValue)
{


	// Validate our parameters
	if (NN_DEBUG && theValue.IsValid())
		NN_ASSERT(theValue.IsType(typeid(NArray)) || theValue.IsType(typeid(NDictionary)));



	// Set the parameters
	mParameters = theValue;
}


