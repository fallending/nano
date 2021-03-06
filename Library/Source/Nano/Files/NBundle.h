/*	NAME:
		NBundle.h

	DESCRIPTION:
		Resource bundle.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NBUNDLE_HDR
#define NBUNDLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NString.h"
#include "NArray.h"
#include "NMutex.h"
#include "NImage.h"
#include "NFile.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Info.plist
static const NString kNBundleExecutableKey							= "CFBundleExecutable";
static const NString kNBundleIdentifierKey							= "CFBundleIdentifier";
static const NString kNBundleVersionKey								= "CFBundleVersion";
static const NString kNBundleNameKey								= "CFBundleName";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Bundle info
typedef struct {
	NDictionary			theInfo;
	NDictionary			theStrings;
} NBundleInfo;


// Lists
typedef std::map<NString, NBundleInfo, NStringHashCompare>			NBundleInfoMap;
typedef NBundleInfoMap::iterator									NBundleInfoMapIterator;
typedef NBundleInfoMap::const_iterator								NBundleInfoMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBundle {
public:
										NBundle(const NFile   &theFile);
										NBundle(const NString &bundleID="");
	virtual							   ~NBundle(void);
	
	
	// Is the bundle valid?
	bool								IsValid(void) const;


	// Get the bundle identifier
	NString								GetIdentifier(void) const;


	// Get the bundle directory
	NFile								GetFile(void) const;


	// Get the resources directory
	NFile								GetResources(void) const;


	// Get an Info.plist value
	//
	// If no key is supplied, GetInfoDictionary returns the Info.plist dictionary.
    bool								GetInfoBoolean(   const NString &theKey)    const;
	NString								GetInfoString(    const NString &theKey)    const;
	NArray								GetInfoArray(     const NString &theKey)    const;
	NDictionary							GetInfoDictionary(const NString &theKey="") const;


	// Get an executable
	//
	// If no name is supplied, returns the active executable.
	NFile								GetExecutable(const NString &theName="") const;


	// Get a resource
	NFile								GetResource(const NString &theName,
													const NString &theType = "",
													const NString &subDir  = "") const;


	// Get a string
	NString								GetString(const NString &theKey,
												  const NString &defaultValue = "",
												  const NString &tableName    = "") const;


private:
	NDictionary							GetBundleInfo(   void)                    const;
	NDictionary							GetBundleStrings(const NString &theTable) const;

	static NBundleInfo					*AcquireInfo(const NFile &theFile);
	static void							ReleaseInfo(void);
	static NMutex						&GetLock(void);


private:
	NFile								mFile;
	mutable NFile						mResources;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline NFile NBundleResource(const NString &theName, const NString &theType="", const NString &subDir="")
{	NBundle		appBundle;

	return(appBundle.GetResource(theName, theType, subDir));
}

inline NImage NBundleImage(const NString &theName, const NString &theType="", const NString &subDir="")
{	NBundle		appBundle;

	return(NImage(appBundle.GetResource(theName, theType, subDir)));
}

inline NString NBundleString(const NString &theKey, const NString &defaultValue="", const NString &tableName="")
{	NBundle		appBundle;

	return(appBundle.GetString(theKey, defaultValue, tableName));
}






#endif // NBUNDLE_HDR


