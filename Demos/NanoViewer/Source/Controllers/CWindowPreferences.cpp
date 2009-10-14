/*	NAME:
		CWindowPreferences.cpp

	DESCRIPTION:
		NanoViewer preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NApplication.h"
#include "NCFBundle.h"

#include "NanoViewer Build.h"
#include "CApplication.h"
#include "CWindowPreferences.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const PanelInfo kPanelGeneral			= { "PrefsGeneral",  kImageToolbarPrefsGeneral,  kStringToolbarPrefsGeneral  };
static const PanelInfo kPanelUpdate				= { "PrefsUpdate",   kBundleIDSoftwareUpdate,    kStringToolbarPrefsUpdate   };





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(CWindowPreferences, kAppNibFile, "Preferences");





//============================================================================
//		CWindowPreferences::CWindowPreferences : Constructor.
//----------------------------------------------------------------------------
CWindowPreferences::CWindowPreferences(void)
{
}





//============================================================================
//		CWindowPreferences::~CWindowPreferences : Destructor.
//----------------------------------------------------------------------------
CWindowPreferences::~CWindowPreferences(void)
{
}





//============================================================================
//		CWindowPreferences::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowPreferences::InitializeSelf(void)
{	PanelInfoList	theList;



	// Build the panels
	theList.push_back(kPanelGeneral);
	theList.push_back(kPanelUpdate);



	// Initialize ourselves
	SetPanelNib(NApplication::GetApp()->GetNib());
	SetPanels(theList);
	
	return(noErr);
}





//============================================================================
//		CWindowPreferences::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CWindowPreferences::DoMessage(BroadcastMsg theMsg, void *msgData)
{


	// Handle the message
	switch (theMsg) {
		case kMsgColorPickerChanged:
			NCFPreferences::GetPrefs()->SetValueColor(kPrefWindowBackgroundKey, mWindowBackground->GetColor());
			break;
		
		default:
			NWindowPreferences::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		CWindowPreferences::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowPreferences::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	bool	changedAll;



	// Update the UI
	changedAll = (theKey == kPrefChangedAllKey);

	if (mWindowBackground != NULL)
		{
		if (changedAll || theKey == kPrefWindowBackgroundKey)
			mWindowBackground->SetColor(thePrefs->GetValueColor(kPrefWindowBackgroundKey));
		}
	
	if (mSoftwareUpdateLastCheck != NULL)
		{
		if ((changedAll || theKey == kSULastCheckedKey) && thePrefs->HasKey(kSULastCheckedKey, false))
			mSoftwareUpdateLastCheck->SetTextValue(thePrefs->GetValueDate(kSULastCheckedKey).GetAsString(kCFDateFormatterFullStyle));
		}
}


