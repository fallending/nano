/*	NAME:
		NHIViewUtilities.cpp

	DESCRIPTION:
		HIView utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIViewUtilities.h"
#include "NHIWindow.h"





//============================================================================
//		NHIViewUtilities::GetView : Get an NHIView from an HIViewRef.
//----------------------------------------------------------------------------
NHIView *NHIViewUtilities::GetView(HIViewRef hiView)
{	NHIView		*theView;



	// Check our parameters
	if (hiView == NULL)
		return(NULL);



	// Get the NHIView object for the view
	theView = dynamic_cast<NHIView*>(NHIObject::GetNHIObject((HIObjectRef) hiView));
	if (theView != NULL)
		return(theView);



	// Or create a new object
	theView = dynamic_cast<NHIView*>(NHIObjectFactory::CreateObject((HIObjectRef) hiView));
	
	return(theView);
}





//============================================================================
//		NHIViewUtilities::CreateHIView : Create an NHIView.
//----------------------------------------------------------------------------
//		Note :	Although this code could be placed in NHIView, so that apps
//				coudl instantiate a view with 'new NHIView', it has been kept
//				in this method to avoid any confusion within sub-classes of
//				NHIView.
//
//				A subclass of NHIView should use ConstructView to finish its
//				construction when it is created as a wrapper, and will be
//				configured automatically when it is created as a subclass via
//				NHIObjectFactory.
//
//				Sub-classes should never call NHIView's constructor, however
//				if NHIView provided a window/rect/visible-based constructor
//				then apps may think that path is the correct way to initialize
//				an NHIView subclass.
//
//				To ensure this isn't a temptation, we require that creating a
//				plain NHIView object comes through here rather than new.
//----------------------------------------------------------------------------
NHIView *NHIViewUtilities::CreateHIView(HIWindowRef	theWindow, const HIRect &theRect, bool isVisible)
{	HIViewRef	hiView, contentView;
	NHIView		*theView;
	OSStatus	theErr;



	// Create the view
	theErr = HIObjectCreate(kHIViewClassID, NULL, (HIObjectRef *) &hiView);
	if (theErr != noErr)
		return(NULL);



	// Create the wrapper
	theView = NHIViewUtilities::GetView(hiView);
	if (theView == NULL)
		return(NULL);



	// Configure the view
	if (theWindow != NULL)
		{
		contentView = NHIWindow(theWindow).GetHIView(kHIViewWindowContentID);
		NN_ASSERT(contentView != NULL);
		
		theErr = HIViewAddSubview(contentView, hiView);
		NN_ASSERT_NOERR(theErr);
		}
	
	theView->SetFrame(theRect);
	theView->SetVisible(isVisible);
	
	return(theView);
}





//============================================================================
//		NHIViewUtilities::GetLayoutInfo : Get an HILayoutInfo.
//----------------------------------------------------------------------------
HILayoutInfo NHIViewUtilities::GetLayoutInfo(OptionBits bindTo)
{	HILayoutInfo	theInfo;



	// Construct the layout
	theInfo = kHILayoutNone;
	
	if (bindTo & kBindTop)
		theInfo.binding.top.kind    = kHILayoutBindTop;
	
	if (bindTo & kBindLeft)
		theInfo.binding.left.kind   = kHILayoutBindLeft;
	
	if (bindTo & kBindBottom)
		theInfo.binding.bottom.kind = kHILayoutBindBottom;
	
	if (bindTo & kBindRight)
		theInfo.binding.right.kind  = kHILayoutBindRight;
	
	return(theInfo);
}





//============================================================================
//      NHIViewUtilities::GetClickablePart : Get the "clickable" part code.
//----------------------------------------------------------------------------
HIViewPartCode NHIViewUtilities::GetClickablePart(NHIView *theView)
{	HIViewPartCode	partCode;



	// Find a suitable part code
	//
	// Although some views have no "clickable part", or have multiple parts,
	// many views only have a single part that responds to mouse clicks.
	if (theView->IsOfClass(kHIPushButtonClassID)  ||
		theView->IsOfClass(kHIBevelButtonClassID) ||
		theView->IsOfClass(kHIRoundButtonClassID) ||
		theView->IsOfClass(kHIDisclosureButtonClassID))
		partCode = kControlButtonPart;

	else if (theView->IsOfClass(kHICheckBoxClassID) ||
			 theView->IsOfClass(kHICheckBoxGroupClassID))
		partCode = kControlCheckBoxPart;

	else if (theView->IsOfClass(kHIRadioButtonClassID))
		partCode = kControlRadioButtonPart;

	else if (theView->IsOfClass(kHIRadioGroupClassID))
		partCode = kControlRadioGroupPart;

	else if (theView->IsOfClass(kHIDisclosureTriangleClassID))
		partCode = kControlTrianglePart;

	else if (theView->IsOfClass(kHIPopupButtonClassID))
		partCode = kControlMenuPart;

	else if (theView->IsOfClass(kHIIconViewClassID))
		partCode = kControlIconPart;

	else if (theView->IsOfClass(kHIImageWellClassID))
		partCode = kControlImageWellPart;
	
	else
		partCode = kHIViewIndicatorPart;
	
	return(partCode);
}





//============================================================================
//		NHIViewUtilities::EmbedInScrollView : Embed a view into a scroll view.
//----------------------------------------------------------------------------
void NHIViewUtilities::EmbedInScrollView(NHIView *theView, OptionBits theOptions)
{	NHIScrollView		*scrollView;
	NHIView				*superView;



	// Validate our parameters
	NN_ASSERT(theView != NULL);



	// Create the scroll view
	scrollView = new NHIScrollView(theView->GetHIWindow(), theView->GetFrame(), theOptions);
	if (scrollView == NULL)
		return;



	// Embed the view
	//
	// Unfortunately IB has very limited supported for HIScrollView, and does
	// not support placing a custom view inside an scroll view (rdar://3982065).
	//
	// This function allows an arbitrary view to be placed inside a scroll view.
	//
	// Typically the view would be a subclass of NScrollableView, since this
	// can automatically embed the view based on a property assigned in IB.
	superView = theView->GetSuperView();

	superView->RemoveSubView(theView, false);
	superView->AddSubView(scrollView);

	scrollView->AddSubView(   theView);
	scrollView->SetLayoutInfo(theView->GetLayoutInfo());
	scrollView->SetScrollBarAutoHide(true);
}





//============================================================================
//		NHIViewUtilities::ReplaceTextValue : Replace part of a view's text value.
//----------------------------------------------------------------------------
void NHIViewUtilities::ReplaceTextValue(NHIView *theView, const NString &theString, const NString &replaceWith)
{	NString		theValue;



	// Replace the text
	theValue = theView->GetTextValue();
	theValue.ReplaceAll(theString, replaceWith);
	theView->SetTextValue(theValue);
}

