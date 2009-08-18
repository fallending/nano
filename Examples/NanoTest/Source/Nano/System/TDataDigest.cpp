/*	NAME:
		TDataDigest.cpp

	DESCRIPTION:
		NDataDigest tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NDataDigest.h"

#include "TDataDigest.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt8 kData1[]											= { 0xAA, 0xBB, 0xCC, 0xDD };
static const UInt8 kData2[]											= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };





//============================================================================
//		TDataDigest::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDataDigest::Execute(void)
{	NData			data1, data2, data3;
	NDataDigest		theDigest;



	// Get the state we need
	data1 = NData(GET_ARRAY_SIZE(kData1), kData1);
	data2 = NData(GET_ARRAY_SIZE(kData2), kData2);



	// Execute the tests
	NN_ASSERT(theDigest.GetString(theDigest.GetInternet(data1)) == "00008866");
	NN_ASSERT(theDigest.GetString(theDigest.GetInternet(data2)) == "0000f9f6");
	NN_ASSERT(theDigest.GetString(theDigest.GetInternet(data3)) == "0000ffff");

	NN_ASSERT(theDigest.GetString(theDigest.GetDJB2(data1)) == "7cbd7e93");
	NN_ASSERT(theDigest.GetString(theDigest.GetDJB2(data2)) == "07f24354");
	NN_ASSERT(theDigest.GetString(theDigest.GetDJB2(data3)) == "00001505");

	NN_ASSERT(theDigest.GetString(theDigest.GetAdler32(data1)) == "074e030e");
	NN_ASSERT(theDigest.GetString(theDigest.GetAdler32(data2)) == "0023000f");
	NN_ASSERT(theDigest.GetString(theDigest.GetAdler32(data3)) == "00000001");
}



