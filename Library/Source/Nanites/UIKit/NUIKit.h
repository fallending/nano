/*	NAME:
		NUIKit.h

	DESCRIPTION:
		UIKit support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUIKIT_HDR
#define NUIKIT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NColor.h"





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to UIKit
inline UIColor *ToUI(const NColor &theColor)
{
	return([UIColor colorWithRed:theColor.GetRed() green:theColor.GetGreen() blue:theColor.GetBlue() alpha:theColor.GetAlpha()]);
}



// UIKit to Nano
inline NColor ToNN(UIColor *theColor)
{	CGColorSpaceModel	colorModel  = CGColorSpaceGetModel(CGColorGetColorSpace(theColor.CGColor));
	const CGFloat		*colorComps = CGColorGetComponents(        theColor.CGColor);
	NIndex				numComps    = CGColorGetNumberOfComponents(theColor.CGColor);
	NColor				nnColor;


	switch (numComps) {
		case 1:
			// Gray
			NN_ASSERT(colorModel == kCGColorSpaceModelMonochrome);
			nnColor.SetColor(colorComps[0], colorComps[0], colorComps[0]);
			break;

		case 2:
			// Gray + Alpha
			NN_ASSERT(colorModel == kCGColorSpaceModelMonochrome);
			nnColor.SetColor(colorComps[0], colorComps[0], colorComps[0], colorComps[1]);
			break;

		case 3:
			// RGB
			NN_ASSERT(colorModel == kCGColorSpaceModelRGB);
			nnColor.SetColor(colorComps[0], colorComps[1], colorComps[2]);
			break;

		case 4:
			// RGB + Alpha
			NN_ASSERT(colorModel == kCGColorSpaceModelRGB);
			nnColor.SetColor(colorComps[0], colorComps[1], colorComps[2], colorComps[3]);
			break;
		
		default:
			NN_LOG("Unknown colour model: %d (%d)", colorModel, numComps);
			break;
		}
	
	return(nnColor);
}





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// NSBundle
#define UIBundleImage(_name)								[[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@_name ofType:NULL]]




#endif // NUIKIT_HDR





