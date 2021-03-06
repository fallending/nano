/*	NAME:
		NEncodable.h

	DESCRIPTION:
		Mix-in class for encodable objects.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NENCODABLE_HDR
#define NENCODABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NEncoder;
class NString;
class NVariant;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Declare an NEncodable sub-class
//
// Example usage:
//
//		class NFoo : public NEncodable {
//		public:
//							NENCODABLE_DECLARE(NFoo);
//
//							NFoo(void);
//			virtual		   ~NFoo(void);
//			...
//		};
//
//		NENCODABLE_DEFINE(NFoo);
//
// Sub-clases of NEncodable must include an NENCODABLE_DECLARE in their class
// declaration, and an NENCODABLE_DEFINE in their class implementation.
//
//
// NENCODABLE_DEFINE automatically registers the class as encodable, however
// EncodableRegister can also be invoked explicitly to register classes whose
// static initializer has been discarded by the linker.
//
// This is necessary to support gcc pre-4.2, where __attribute__((used)) is
// not available.
//
// Older versions of gcc would incorrectly strip static initializers, unless
// PRESERVE_DEAD_CODE_INITS_AND_TERMS was enabled. However this flag has no
// effect unless DEAD_CODE_STRIPPING is also enabled, however that flag will
// increase link time so is off by default in debug builds.
//
// By calling EncodableRegister from a constructor or other reachable code,
// the class will be registered once on first use.
#define NENCODABLE_DECLARE(_class)																						\
																														\
	private:																											\
	static bool sEncodableRegistered;																					\
																														\
	public:																												\
	static bool							EncodableRegister(void);														\
	static bool							EncodableCanEncode(                               const NVariant &theValue);	\
	static void							EncodableEncodeObject(      NEncoder &theEncoder, const NVariant &theValue);	\
	static NVariant						EncodableDecodeObject(const NEncoder &theEncoder)


#define NENCODABLE_DEFINE_REGISTER(_class, _forClass)																	\
																														\
	bool _class::sEncodableRegistered = _class::EncodableRegister();													\
																														\
	bool _class::EncodableRegister(void)																				\
	{	static bool				sIsRegistered;																			\
																														\
		NEncoderClassInfo		classInfo;																				\
																														\
		if (!sIsRegistered)																								\
			{																											\
			classInfo.canEncode    = BindFunction(_class::EncodableCanEncode,    kNArg1);								\
			classInfo.encodeObject = BindFunction(_class::EncodableEncodeObject, kNArg1, kNArg2);						\
			classInfo.decodeObject = BindFunction(_class::EncodableDecodeObject, kNArg1);								\
																														\
			NEncoder::RegisterClass(#_forClass, classInfo);																\
			sIsRegistered = true;																						\
			}																											\
																														\
		return(sIsRegistered);																							\
	}																													\
																														\
	void *kEatLastSemiColonForPedanticWarning1 ## _class


#define NENCODABLE_DEFINE_CODER(_class)																					\
																														\
	bool _class::EncodableCanEncode(const NVariant &theValue)															\
	{	const _class	*theObject;																						\
																														\
		theObject = theValue.GetValue<_class>();																		\
		return(theObject != NULL);																						\
	}																													\
																														\
	void _class::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)									\
	{	const _class	*theObject;																						\
																														\
		theObject = theValue.GetValue<_class>();																		\
		theObject->EncodeSelf(theEncoder);																				\
	}																													\
																														\
	NVariant _class::EncodableDecodeObject(const NEncoder &theEncoder)													\
	{	_class		theObject;																							\
																														\
		theObject.DecodeSelf(theEncoder);																				\
		return(theObject);																								\
	}																													\
																														\
	void *kEatLastSemiColonForPedanticWarning2 ## _class


#define NENCODABLE_DEFINE(_class)																						\
	NENCODABLE_DEFINE_REGISTER(_class, _class);																			\
	NENCODABLE_DEFINE_CODER(   _class);																					\
																														\
	void *kEatLastSemiColonForPedanticWarning3 ## _class





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NEncodable {
friend class NEncoder;
public:
										NEncodable(void);
	virtual							   ~NEncodable(void);


protected:
	// Encode/decode the object
	//
	// Sub-classes must implement to encode/decode themselves.
	virtual void						EncodeSelf(      NEncoder &theEncoder) const;
	virtual void						DecodeSelf(const NEncoder &theEncoder);


private:


};





#endif // NENCODABLE_HDR
