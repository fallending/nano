/*	NAME:
		NCFBundle.cpp

	DESCRIPTION:
		CoreFoundation bundle object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kRoleEditor									= "editor";
static const NString kRoleViewer									= "viewer";





//============================================================================
//		NCFBundle::NCFBundle : Constructor.
//----------------------------------------------------------------------------
NCFBundle::NCFBundle(const NFile &theFile)
{	CFBundleRef		cfBundle;



	// Create the bundle
	cfBundle = CFBundleCreate(kCFAllocatorNano, theFile.GetURL());
	if (cfBundle != NULL)
		Set(cfBundle);
}





//============================================================================
//		NCFBundle::NCFBundle : Constructor.
//----------------------------------------------------------------------------
NCFBundle::NCFBundle(const NString &bundleID)
{	bool			shouldRelease;
	CFBundleRef     cfBundle;
	OSStatus		theErr;
	CFURLRef		cfURL;



	// Find the bundle
	//
	// If a name is supplied, we first check the currently open bundles
	// before falling back to a more expensive search using LaunchServices.
	//
	// Note that LSFindApplicationForInfo, despite the name, expects us to
	// release the URL reference that it returns.
	shouldRelease = false;

	if (bundleID.IsEmpty())
		cfBundle = CFBundleGetMainBundle();
	else
		{
		cfBundle = CFBundleGetBundleWithIdentifier(bundleID);
		if (cfBundle == NULL)
			{
			theErr = LSFindApplicationForInfo(kLSUnknownCreator, bundleID, NULL, NULL, &cfURL);
			if (theErr == noErr && cfURL != NULL)
				{
				cfBundle      = CFBundleCreate(kCFAllocatorNano, cfURL);
				shouldRelease = (cfBundle != NULL);
				
				CFSafeRelease(cfURL);
				}
			}
		}



	// Initialize ourselves
	NN_ASSERT(cfBundle != NULL);
    Set(cfBundle, shouldRelease);
}





//============================================================================
//		NCFBundle::~NCFBundle : Destructor.
//----------------------------------------------------------------------------
NCFBundle::~NCFBundle(void)
{
}





//============================================================================
//		NCFBundle::GetIdentifier : Get the identifier.
//----------------------------------------------------------------------------
NString NCFBundle::GetIdentifier(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the identifier
	return(CFBundleGetIdentifier(*this));
}





//============================================================================
//		NCFBundle::GetFile : Get the file.
//----------------------------------------------------------------------------
NFile NCFBundle::GetFile(void) const
{	NCFURL	theURL;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the file
	theURL.Set(CFBundleCopyBundleURL(*this));
	
	return(theURL.GetFile());
}





//============================================================================
//      NCFBundle::GetInfoBoolean : Get an Info.plist boolean.
//----------------------------------------------------------------------------
bool NCFBundle::GetInfoBoolean(const NString &theKey) const
{ 


    // Validate our state
    NN_ASSERT(IsValid());



    // Get the value
    return(CFBooleanGetValue((CFBooleanRef) CFBundleGetValueForInfoDictionaryKey(*this, theKey)));
}





//============================================================================
//		NCFBundle::GetInfoString : Get an Info.plist string.
//----------------------------------------------------------------------------
NString NCFBundle::GetInfoString(const NString &theKey) const
{	NString		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	theValue.Set(CFBundleGetValueForInfoDictionaryKey(*this, theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFBundle::GetInfoArray : Get an Info.plist array.
//----------------------------------------------------------------------------
NArray NCFBundle::GetInfoArray(const NString &theKey) const
{	NArray	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	theValue.Set(CFBundleGetValueForInfoDictionaryKey(*this, theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFBundle::GetInfoDictionary : Get an Info.plist dictionary.
//----------------------------------------------------------------------------
NDictionary NCFBundle::GetInfoDictionary(const NString &theKey) const
{	NDictionary		theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the value
	if (theKey.IsEmpty())
		theValue.Set(CFBundleGetLocalInfoDictionary(*this), false);
	else
		theValue.Set(CFBundleGetValueForInfoDictionaryKey(*this, theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFBundle::GetInfoUTIs : Get the Info.plist UTI values.
//----------------------------------------------------------------------------
NArray NCFBundle::GetInfoUTIs(LSRolesMask theRoles)
{	NArray			theTypes, theUTIs, theResult;
	bool			isMatch, isEditor, isViewer;
	UInt32			n, numItems;
	NDictionary		typeDict;
	NString			theRole;



	// Extract the UTIs
	theTypes = GetInfoArray(kCFBundleDocumentTypesKey);
	numItems = theTypes.GetSize();
	
	for (n = 0; n < numItems; n++)
		{
		// Get the document type info
		typeDict = theTypes.GetValueDictionary(n);
		theRole  = typeDict.GetValueString(kCFBundleTypeRoleKey);

		isEditor = theRole.EqualTo(kRoleEditor, true);
		isViewer = theRole.EqualTo(kRoleViewer, true) || isEditor;



		// Add the UTIs that match
		isMatch = ( (isEditor && (theRoles & kLSRolesEditor)) ||
					(isViewer && (theRoles & kLSRolesViewer)) );

		if (isMatch)
			{
			theUTIs = typeDict.GetValueArray(kLSItemContentTypesKey);
			theResult.Join(theUTIs);
			}
		}
	
	return(theResult);
}





//============================================================================
//		NCFBundle::GetResources : Get the resources directory.
//----------------------------------------------------------------------------
NFile NCFBundle::GetResources(void) const
{	NCFURL		theURL;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the URL
	theURL.Set(CFBundleCopyResourcesDirectoryURL(*this));
	
	return(theURL.GetFile());
}





//============================================================================
//		NCFBundle::GetResource : Get a resource from the bundle.
//----------------------------------------------------------------------------
NFile NCFBundle::GetResource(const NString &theName, const NString &theType, const NString &subDir) const
{	CFStringRef		cfType, cfSubDir;
	NFile			theFile;
	NCFURL			theURL;



	// Check our parameters
	if (theName.IsEmpty())
		return(theFile);



	// Get the state we need
	cfType   = theType.IsEmpty() ? NULL : (CFStringRef) theType;
	cfSubDir =  subDir.IsEmpty() ? NULL : (CFStringRef) subDir;



	// Get the file
	//
	// We allow bundle resources to be specified with an absolute path, since
	// this lets us use absolute paths in .nib files and treat those items as
	// if they were part of the bundle.
	if (theName.StartsWith("/"))
		theFile = NFile(theName);
	else
		{
		if (theURL.Set(CFBundleCopyResourceURL(*this, theName, cfType, cfSubDir)))
			theFile = theURL.GetFile();
		}
	
	return(theFile);
}





//============================================================================
//		NCFBundle::GetString : Get a string from the bundle.
//----------------------------------------------------------------------------
NString NCFBundle::GetString(const NString &theKey, const NString &defaultValue, const NString &tableName) const
{	CFStringRef		cfDefaultValue, cfTableName;
	NString			theValue;



	// Check our parameters
	if (theKey.IsEmpty())
		return(theValue);



	// Get the state we need
	cfDefaultValue = defaultValue.IsEmpty() ? NULL : ((CFStringRef) defaultValue);
	cfTableName    =    tableName.IsEmpty() ? NULL : ((CFStringRef) tableName);



	// Get the string
	theValue.Set(CFBundleCopyLocalizedString(*this, theKey, cfDefaultValue, cfTableName));
	
	return(theValue);
}

