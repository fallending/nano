/*	NAME:
		NDictionary.h

	DESCRIPTION:
		CFDictionaryRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDICTIONARY_HDR
#define NDICTIONARY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NColor.h"
#include "NString.h"
#include "NArray.h"
#include "NData.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDictionary : public NCFObject {
public:
										NDictionary(CFDictionaryRef cfDictionary);
										
										NDictionary(void);
	virtual								~NDictionary(void);


	// Is the dictionary empty?
	bool								IsEmpty(   void) const;
	bool								IsNotEmpty(void) const;


	// Get the dictionary size
	UInt32								GetSize(void) const;


	// Clear the dictionary
	void								Clear(void);


	// Join two dictionaries
	//
	// Existing keys will have their values replaced.
	void								Join(const NDictionary &theValue);


	// Does a key exist?
	bool								HasKey(const NString &theKey) const;


	// Remove a key
	void								RemoveKey(const NString &theKey);


	// Get the keys
	NArray								GetKeys(void) const;
	

	// Get/set a value
	//
	// If the key does not exist, a 0'd/empty value is returned.
	bool								GetValueBoolean(   const NString &theKey) const;
	OSType								GetValueOSType(    const NString &theKey) const;
	UInt32								GetValueUInt32(    const NString &theKey) const;
	Float64								GetValueFloat64(   const NString &theKey) const;
	NString								GetValueString(    const NString &theKey) const;
	HIPoint								GetValuePoint(     const NString &theKey) const;
	HISize								GetValueSize(      const NString &theKey) const;
	HIRect								GetValueRect(      const NString &theKey) const;
	NColor								GetValueColor(     const NString &theKey) const;
	NDate								GetValueDate(      const NString &theKey) const;
	NArray								GetValueArray(     const NString &theKey) const;
	NDictionary							GetValueDictionary(const NString &theKey) const;
	NData								GetValueData(      const NString &theKey) const;

	void								SetValueBoolean(   const NString &theKey, bool                theValue);
	void								SetValueOSType(    const NString &theKey, OSType              theValue);
	void								SetValueUInt32(    const NString &theKey, UInt32              theValue);
	void								SetValueFloat64(   const NString &theKey, Float64             theValue);
	void								SetValueString(    const NString &theKey, const NString      &theValue);
	void								SetValuePoint(     const NString &theKey, const HIPoint      &theValue);
	void								SetValueSize(      const NString &theKey, const HISize       &theValue);
	void								SetValueRect(      const NString &theKey, const HIRect       &theValue);
	void								SetValueColor(     const NString &theKey, const NColor       &theValue);
	void								SetValueDate(      const NString &theKey, const NDate        &theValue);
	void								SetValueArray(     const NString &theKey, const NArray       &theValue);
	void								SetValueDictionary(const NString &theKey, const NDictionary  &theValue);
	void								SetValueData(      const NString &theKey, const NData        &theValue);


	// Operators
	CFTypeRef							operator [](const NString &theKey) const;


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);


private:
	void								InitializeSelf(CFDictionaryRef cfDictionary);
	
	CFTypeRef							GetValue(const NString &theKey) const;
	void								SetValue(const NString &theKey, CFTypeRef theValue);

	static void							JoinCallback(const void *theKey, const void *theValue, void *theContext);


private:


};





#endif // NDICTIONARY_HDR

