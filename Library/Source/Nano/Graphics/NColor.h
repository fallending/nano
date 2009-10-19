/*	NAME:
		NColor.h

	DESCRIPTION:
		Color object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOLOR_HDR
#define NCOLOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Color formats
typedef enum {
	kNColorARGB,
	kNColorRGBA,
} NColorFormat;


// Colors
class NColor;

extern const NColor kColorBlack;
extern const NColor kColorBlue;
extern const NColor kColorBrown;
extern const NColor kColorClear;
extern const NColor kColorCyan;
extern const NColor kColorGray;
extern const NColor kColorGrayDark;
extern const NColor kColorGrayLight;
extern const NColor kColorGreen;
extern const NColor kColorMagenta;
extern const NColor kColorOrange;
extern const NColor kColorPurple;
extern const NColor kColorRed;
extern const NColor kColorWhite;
extern const NColor kColorYellow;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NColor :	public NEncodable,
				public NComparable<NColor> {
public:
										NENCODABLE_DECLARE(NColor);

										NColor(Float32 r, Float32 g, Float32 b, Float32 a=1.0f);
										NColor(      UInt32    theColor, NColorFormat theFormat=kNColorRGBA);
										NColor(const NString  &hexColor, NColorFormat theFormat=kNColorRGBA);
										
										NColor(void);
	virtual							   ~NColor(void);


	// Compare the value
	NComparison							Compare(const NColor &theValue) const;


	// Get/set the color
	void								GetColor(Float32 &r, Float32 &g, Float32 &b, Float32 &a) const;
	UInt32								GetColor(NColorFormat theFormat=kNColorRGBA)             const;

	void								SetColor(Float32 r, Float32 g, Float32 b, Float32 a);
	void								SetColor(UInt32 theColor, NColorFormat theFormat=kNColorRGBA);
	

	// Get/set the components
	Float32								GetRed(  void) const;
	Float32								GetGreen(void) const;
	Float32								GetBlue( void) const;
	Float32								GetAlpha(void) const;

	void								SetRed(  Float32 theValue);
	void								SetGreen(Float32 theValue);
	void								SetBlue( Float32 theValue);
	void								SetAlpha(Float32 theValue);


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	Float32								mRed;
	Float32								mGreen;
	Float32								mBlue;
	Float32								mAlpha;
};









#endif // NCOLOR_HDR

