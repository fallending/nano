/*	NAME:
		�PROJECTNAME� Build.h

	DESCRIPTION:
		�PROJECTNAME� build header.
	
	COPYRIGHT:
		Copyright (c) �YEAR�, �ORGANIZATIONNAME�

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef �PROJECTNAMEASIDENTIFIER�_BUILD_HDR
#define �PROJECTNAMEASIDENTIFIER�_BUILD_HDR
//============================================================================
//		Macros
//----------------------------------------------------------------------------
#ifdef INFO_PLIST
	#define STRING(_x)											_x
#else
	#define STRING(_x)											# _x
#endif





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// NApplication
#define kAppNibFile												STRING(�PROJECTNAME�)
#define kAppName												STRING(�PROJECTNAME�)
#define kAppCreator												STRING(NAPP)
#define kAppVersion												STRING(1.0)
#define kAppCompany												STRING(�ORGANIZATIONNAME�)
#define kAppBundleIdentifier									STRING(com.mycompany.�PROJECTNAMEASIDENTIFIER�)
#define kAppHomePage											"http://www.mycompany.com/�PROJECTNAME�/"


// NDocument





//============================================================================
//      Bundle resources
//----------------------------------------------------------------------------
// Strings
#define kStringToolbarPrefsGeneral								"ToolbarPrefsGeneral"
#define kStringToolbarPrefsAdvanced								"ToolbarPrefsAdvanced"
#define kStringToolbarPrefsUpdate								"ToolbarPrefsUpdate"


// Images
#define kImageToolbarPrefsGeneral								"PrefsGeneral.png"
#define kImageToolbarPrefsAdvanced								"PrefsAdvanced.png"





#endif // �PROJECTNAMEASIDENTIFIER�_BUILD_HDR

