/*	NAME:
		NSize.h

	DESCRIPTION:
		Size object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSIZE_HDR
#define NSIZE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NEncodable.h"
#include "NComparable.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
class NSize;

extern const NSize kNSizeZero;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSize :	public NEncodable,
				public NComparable<NSize> {
public:
										DECLARE_NENCODABLE(NSize);

										 NSize(Float32 width=0.0f, Float32 height=0.0f);
	virtual								~NSize(void);


	// Clear the size
	void								Clear(void);


	// Test the size
	bool								IsEmpty(void) const;


	// Compare the value
	NComparison							Compare(const NSize &theValue) const;


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


public:
	Float32								width;
	Float32								height;
};




#endif // NSIZE_HDR


