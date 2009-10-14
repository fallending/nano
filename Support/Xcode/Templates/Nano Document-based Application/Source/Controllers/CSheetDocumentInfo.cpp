/*	NAME:
		CSheetDocumentInfo.cpp

	DESCRIPTION:
		�PROJECTNAME� document info sheet.
	
	COPYRIGHT:
		Copyright (c) �YEAR�, �ORGANIZATIONNAME�

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "�PROJECTNAME� Build.h"
#include "CSheetDocumentInfo.h"





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(CSheetDocumentInfo, kAppNibFile, "Document Info");





//============================================================================
//		CSheetDocumentInfo::CSheetDocumentInfo : Constructor.
//----------------------------------------------------------------------------
CSheetDocumentInfo::CSheetDocumentInfo(void)
{


	// Initialise ourselves
	mDocument = NULL;
}





//============================================================================
//		CSheetDocumentInfo::~CSheetDocumentInfo : Destructor.
//----------------------------------------------------------------------------
CSheetDocumentInfo::~CSheetDocumentInfo(void)
{
}





//============================================================================
//		CSheetDocumentInfo::CommandSheet : Handle a sheet command.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CSheetDocumentInfo::CommandSheet(UInt32 theCmd)
{


	// Process the command
	if (theCmd == kHICommandOK)
		{
		mDocument->SetAuthor(     mAuthor->GetTextValue());
		mDocument->SetDescription(mDescription->GetTextValue());
		}



	// Invoke the base class
	return(NWindowController::CommandSheet(theCmd));
}





//============================================================================
//		CSheetDocumentInfo::DoSheetOpening : A sheet is opening.
//----------------------------------------------------------------------------
void CSheetDocumentInfo::DoSheetOpening(NWindowController *theParent, NWindowController */*theSheet*/)
{


	// Obtain our document
	mDocument = dynamic_cast<CWindowDocument*>(theParent);
	NN_ASSERT(mDocument != NULL);



	// Initialize the sheet
	GetWindow()->SetKeyboardFocus(mAuthor);

	     mAuthor->SetTextValue(mDocument->GetAuthor());
	mDescription->SetTextValue(mDocument->GetDescription());
}
