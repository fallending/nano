/*	NAME:
		TArray.h

	DESCRIPTION:
		NArray tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef TARRAY_HDR
#define TARRAY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NVariant.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class TArray {
public:
	// Execute the tests
	static void							Execute(void);


private:
	static void							ForEach(const NVariant &theValue, NIndex *theCount);
};





#endif // TARRAY_HDR