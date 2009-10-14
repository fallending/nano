/*	NAME:
		NLevelIndicator.cpp

	DESCRIPTION:
		Level indicator view.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NLevelIndicator.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagLevelContinuous								= 'cont';
static const OSType kTagLevelWarning								= 'warn';
static const OSType kTagLevelCritical								= 'crit';


// Colors
static const NColor			kNormalEdgeDark   =						 NColor(0.431, 0.651, 0.239);
static const NColor			kNormalEdgeLight  =						 NColor(0.569, 0.812, 0.353);
static const NShadingSample	kNormalGradient[] =			{	{ 0.00f, NColor(0.608, 0.796, 0.424) }, 
															{ 0.16f, NColor(0.831, 0.941, 0.729) }, 
															{ 0.54f, NColor(0.435, 0.729, 0.176) }, 
															{ 1.00f, NColor(0.651, 0.902, 0.439) } };


static const NColor			kWarningEdgeDark   =					 NColor(0.741, 0.671, 0.282);
static const NColor			kWarningEdgeLight  =					 NColor(0.890, 0.824, 0.392);
static const NShadingSample	kWarningGradient[] =		{	{ 0.00f, NColor(0.894, 0.820, 0.482) }, 
															{ 0.16f, NColor(1.000, 1.000, 0.804) }, 
															{ 0.54f, NColor(0.827, 0.718, 0.200) }, 
															{ 1.00f, NColor(0.980, 0.898, 0.471) } };


static const NColor			kCriticalEdgeDark   =					 NColor(0.741, 0.290, 0.282);
static const NColor			kCriticalEdgeLight  =					 NColor(0.890, 0.416, 0.392);
static const NShadingSample	kCriticalGradient[] =		{	{ 0.00f, NColor(0.898, 0.459, 0.459) }, 
															{ 0.16f, NColor(1.000, 0.808, 0.808) }, 
															{ 0.54f, NColor(0.827, 0.200, 0.200) }, 
															{ 1.00f, NColor(0.980, 0.471, 0.471) } };

static const NColor			kBackgroundEdgeDark   =					 NColor(0.710, 0.710, 0.710);
static const NColor			kBackgroundEdgeLight  =					 NColor(0.839, 0.839, 0.839);
static const NColor			kBackgroundEdgeShadow =					 NColor(0.400, 0.400, 0.400, 0.3);
static const NShadingSample	kBackgroundGradient[] =		{	{ 0.00f, NColor(0.843, 0.843, 0.843) }, 
															{ 0.16f, NColor(0.929, 0.929, 0.929) }, 
															{ 0.54f, NColor(0.800, 0.800, 0.800) }, 
															{ 1.00f, NColor(0.933, 0.933, 0.933) } };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NLevelIndicator, "com.nano.nlevelindicator", kHIViewClassID);





//============================================================================
//		NLevelIndicator::NLevelIndicator : Constructor.
//----------------------------------------------------------------------------
NLevelIndicator::NLevelIndicator(HIWindowRef		theWindow,
									const HIRect	&theRect,
									bool			isContinuous,
									SInt32			valueInitial,
									SInt32			valueMinimum,
									SInt32			valueMaximum,
									SInt32			valueWarning,
									SInt32			valueCritical)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetContinuous(isContinuous);
	
	SetMinimumValue( valueMinimum);
	SetMaximumValue( valueMaximum);
	SetCurrentValue( valueInitial);
	SetWarningValue( valueWarning);
	SetCriticalValue(valueCritical);
}





//============================================================================
//		NLevelIndicator::~NLevelIndicator : Destructor.
//----------------------------------------------------------------------------
NLevelIndicator::~NLevelIndicator(void)
{
}





//============================================================================
//		NLevelIndicator::IsContinuous : Is the level indicator continuous?
//----------------------------------------------------------------------------
bool NLevelIndicator::IsContinuous(void) const
{


	// Get our state
	return(mIsContinuous);
}





//============================================================================
//		NLevelIndicator::SetContinuous : Set our continuous state.
//----------------------------------------------------------------------------
void NLevelIndicator::SetContinuous(bool isContinuous)
{


	// Set our state
	mIsContinuous = isContinuous;
	
	SetNeedsDisplay();
}





//============================================================================
//		NLevelIndicator::GetWarningValue : Get the warning level.
//----------------------------------------------------------------------------
SInt32 NLevelIndicator::GetWarningValue(void) const
{


	// Get our state
	return(mValueWarning);
}





//============================================================================
//		NLevelIndicator::SetWarningValue : Set the warning value.
//----------------------------------------------------------------------------
void NLevelIndicator::SetWarningValue(SInt32 theValue)
{


	// Set our state
	mValueWarning = theValue;
	
	SetNeedsDisplay();
}





//============================================================================
//		NLevelIndicator::GetCriticalValue : Get the critical level.
//----------------------------------------------------------------------------
SInt32 NLevelIndicator::GetCriticalValue(void) const
{


	// Get our state
	return(mValueCritical);
}





//============================================================================
//		NLevelIndicator::SetCriticalValue : Set the critical value.
//----------------------------------------------------------------------------
void NLevelIndicator::SetCriticalValue(SInt32 theValue)
{


	// Set our state
	mValueCritical = theValue;
	
	SetNeedsDisplay();
}





//============================================================================
//		NLevelIndicator::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NLevelIndicator::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw				},
										{ kEventClassControl, kEventControlHitTest			},
										{ kEventClassControl, kEventControlTrack			},
										{ kEventClassNone,    kEventKindNone				} };



	// Initialize ourselves
	mIsContinuous  = true;
	mValueWarning  = 0;
	mValueCritical = 0;



	// Disable tracking
	//
	// Although level indicators do support mouse tracking, they are typically
	// used to display information rather than collect input.
	//
	// As such, we disable tracking by default - this bit can be cleared later
	// if the view should track the mouse.
	SetFeatures(kHIViewFeatureIgnoresClicks);



	// Apply our properties
	if (HasProperty(                     kPropertyNano, kTagLevelContinuous))
		SetContinuous(GetPropertyBoolean(kPropertyNano, kTagLevelContinuous));

	if (HasProperty(                                kPropertyNano, kTagLevelWarning))
		SetWarningValue( (SInt32) GetPropertyUInt32(kPropertyNano, kTagLevelWarning));

	if (HasProperty(                                kPropertyNano, kTagLevelCritical))
		SetCriticalValue((SInt32) GetPropertyUInt32(kPropertyNano, kTagLevelCritical));



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NLevelIndicator::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NLevelIndicator::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);



	// Prepare to draw
	UpdateShading();

	cgContext.SetLineWidth(1.0f);
	cgContext.SetAntiAlias(false);

	if (!IsEnabled())
		cgContext.SetAlpha(kHIViewDisabledAlpha);



	// Draw the level indicator
	if (mIsContinuous)
		DrawContinuous(cgContext);
	else
		DrawDiscrete(cgContext);

	return(noErr);
}





//============================================================================
//		NLevelIndicator::DoTrackMouse : Track the mouse.
//----------------------------------------------------------------------------
bool NLevelIndicator::DoTrackMouse(NCarbonEvent &theEvent, const HIPoint &theMouse, MouseTrackingResult theTrack)
{	float		theValue;
	HIRect		theRect;



	// Track the mouse
	switch (theTrack) {
		case kMouseTrackingMouseDown:
		case kMouseTrackingMouseDragged:
		case kMouseTrackingTimedOut:
			// Update our value
			theRect  = GetBounds();
			theValue = (theMouse.x - theRect.origin.x) / theRect.size.width;

			SetPercentValue(GET_CLAMPED_VALUE(theValue, 0.0f, 1.0f));
			break;
		}



	// Inherit default behaviour
	return(NHIView::DoTrackMouse(theEvent, theMouse, theTrack));
}





//============================================================================
//		NLevelIndicator::UpdateShading : Update the shadings.
//----------------------------------------------------------------------------
#pragma mark -
void NLevelIndicator::UpdateShading(void)
{	float		minY, maxY;
	HIRect		theRect;



	// Get the state we need
	theRect = GetBounds();
	minY    = CGRectGetMinY(theRect) + 1;
	maxY    = CGRectGetMaxY(theRect) - 1;



	// Updating the shadings
	mShadingNormal.SetSamples(    kNormalGradient);
	mShadingWarning.SetSamples(   kWarningGradient);
	mShadingCritical.SetSamples(  kCriticalGradient);
	mShadingBackground.SetSamples(kBackgroundGradient);

	mShadingNormal.ApplyLinearShading(    kLinearTopBottom, minY, maxY);
	mShadingWarning.ApplyLinearShading(   kLinearTopBottom, minY, maxY);
	mShadingCritical.ApplyLinearShading(  kLinearTopBottom, minY, maxY);
	mShadingBackground.ApplyLinearShading(kLinearTopBottom, minY, maxY);
}





//============================================================================
//		NLevelIndicator::DrawContinuous : Draw a continuous level indicator.
//----------------------------------------------------------------------------
void NLevelIndicator::DrawContinuous(NCGContext &cgContext)
{	NColor				colorTop, colorBottom;
	float				minX, minY, maxX, maxY;
	const NCGShading	*theShading;
	SInt32				theValue;
	HIRect				theRect;



	// Get the state we need
	theRect  = GetBounds();
	theValue = GetCurrentValue();
	
	minX = CGRectGetMinX(theRect);
	minY = CGRectGetMinY(theRect) + 1;
	maxX = CGRectGetMaxX(theRect);
	maxY = CGRectGetMaxY(theRect);

	if (theValue >= mValueCritical)
		{
		colorTop    = kCriticalEdgeDark;
		colorBottom = kCriticalEdgeLight;
		theShading  = &mShadingCritical;
		}
	
	else if (theValue >= mValueWarning)
		{
		colorTop    = kWarningEdgeDark;
		colorBottom = kWarningEdgeLight;
		theShading  = &mShadingWarning;
		}
	
	else
		{
		colorTop    = kNormalEdgeDark;
		colorBottom = kNormalEdgeLight;
		theShading  = &mShadingNormal;
		}



	// Draw the background
	cgContext.DrawShading(mShadingBackground);

	cgContext.SetStrokeColor(kBackgroundEdgeDark);
	cgContext.MoveTo(   minX, minY);
	cgContext.AddLineTo(maxX, minY);
	cgContext.StrokePath();

	cgContext.SetStrokeColor(kBackgroundEdgeLight);
	cgContext.MoveTo(   minX, maxY);
	cgContext.AddLineTo(maxX, maxY);
	cgContext.StrokePath();



	// Draw the level
	theRect.size.width *= GetPercentValue();
	maxX = CGRectGetMaxX(theRect);

	cgContext.SetStrokeColor(colorTop);
	cgContext.MoveTo(   minX, minY);
	cgContext.AddLineTo(maxX, minY);
	cgContext.StrokePath();

	cgContext.SetStrokeColor(colorBottom);
	cgContext.MoveTo(   minX, maxY);
	cgContext.AddLineTo(maxX, maxY);
	cgContext.StrokePath();

	cgContext.ClipToRect(  theRect);
	cgContext.DrawShading(*theShading);
}





//============================================================================
//		NLevelIndicator::DrawDiscrete : Draw a discrete level indicator.
//----------------------------------------------------------------------------
void NLevelIndicator::DrawDiscrete(NCGContext &cgContext)
{	UInt32				n, numCells, cellWidth, cellValue;
	HIRect				theRect, rectShadow, rectCell;
	const NCGShading	*theShading, *valueShading;
	NColor				theColor, valueColor;
	SInt32				theValue;



	// Get the state we need
	theRect  = GetBounds();
	theValue = GetCurrentValue();

	numCells  = GetMaximumValue() - GetMinimumValue();
	cellWidth = (UInt32) (CGRectGetWidth(theRect) / numCells);
	cellValue = (UInt32) (numCells * GetPercentValue());

	rectShadow = theRect;
	rectShadow.origin.y    += 2.0f;
	rectShadow.size.width   = (cellWidth * numCells) - 1.0f;
	rectShadow.size.height -= 2.0f;

	rectCell = theRect;
	rectCell.size.width   = cellWidth;
	rectCell.size.height -= 2.0f;

	if (theValue >= mValueCritical)
		{
		valueColor   = kCriticalEdgeDark;
		valueShading = &mShadingCritical;
		}
	else if (theValue >= mValueWarning)
		{
		valueColor   = kWarningEdgeDark;
		valueShading = &mShadingWarning;
		}
	else
		{
		valueColor   = kNormalEdgeDark;
		valueShading = &mShadingNormal;
		}



	// Draw the background
	cgContext.SetFillColor(kBackgroundEdgeShadow);
	cgContext.FillRect(rectShadow);



	// Draw the cells
	for (n = 0; n < numCells; n++)
		{
		// Select the color
		if (n > cellValue)
			{
			theColor   = kBackgroundEdgeDark;
			theShading = &mShadingBackground;
			}
	
		else
			{
			theColor   = valueColor;
			theShading = valueShading;
			}


		// Draw the cell
		theRect = rectCell;
		theRect.origin.y    += 1.0f;
		theRect.size.width  -= 2.0f;
		theRect.size.height -= 1.0f;

		cgContext.SaveState();
		cgContext.ClipToRect(  theRect);
		cgContext.DrawShading(*theShading);
		cgContext.RestoreState();

		cgContext.SetStrokeColor(theColor);
		cgContext.StrokeRect(    theRect);


		// Move on to the next cell
		rectCell.origin.x += cellWidth;
		}
}





