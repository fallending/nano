/*	NAME:
		NNSAutoReleasePool.h

	DESCRIPTION:
		Auto-release pool.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNSAUTORELEASEPOOL_HDR
#define NNSAUTORELEASEPOOL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUncopyable.h"
#include "NCFObject.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StAutoReleasePool : public NUncopyable {
public:
										StAutoReleasePool(void);
	virtual							   ~StAutoReleasePool(void);


private:
	NSAutoreleasePool					*mPool;
};






#endif // NNSAUTORELEASEPOOL_HDR





