/*	NAME:
		NClockView.h

	DESCRIPTION:
		Clock view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCLOCKVIEW_HDR
#define NCLOCKVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NDate.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NClockView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NClockView);

										NClockView(HIWindowRef			theWindow,
													const HIRect		&theRect,
													ControlClockType	clockType   = kControlClockTypeHourMinuteSecond,
													ControlClockFlags	clockFlags  = kControlClockFlagStandard,
													bool				isAnimating = true);

	virtual								~NClockView(void);


	// Get/set the date
	NDate								GetDate(void) const;
	void								SetDate(const NDate &theDate);


	// Get/set the animation state
	bool								IsAnimating(void) const;
	void								SetAnimating(bool isAnimating);


private:


};





#endif // NCLOCKVIEW_HDR

