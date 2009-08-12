/*	NAME:
		NNSSingleton.h

	DESCRIPTION:
		Objective-C singleton.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNSSINGLETON_HDR
#define NNSSINGLETON_HDR
//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Declare an Objective-C singleton
//
// Example usage:
//
//		@interface MyClass : NSObject {
//			...
//		}
//		...
//		DECLARE_NNSSINGLETON(MyClass);
//		@end
//
//
//		@implementation  MyClass
//		DEFINE_NNSSINGLETON(MyClass);
//		...
//		@end
//
// Follows Apple's standard pattern from:
//
//		http://developer.apple.com/documentation/Cocoa/Conceptual/CocoaFundamentals/CocoaObjects/CocoaObjects.html
//
#define DECLARE_NNSSINGLETON(_class)										\
																			\
	+ (_class *) sharedInstance


#define DEFINE_NNSSINGLETON(_class)											\
																			\
	static _class *gSharedInstance = nil;									\
																			\
	+ (_class *) sharedInstance												\
	{																		\
		@synchronized(self)													\
		{																	\
			if (gSharedInstance == nil)										\
				[[self alloc] init];										\
		}																	\
																			\
		return(gSharedInstance);											\
	}																		\
																			\
	+ (id) allocWithZone:(NSZone *)zone										\
	{																		\
		@synchronized(self)													\
		{																	\
			if (gSharedInstance == nil)										\
				{															\
				gSharedInstance = [super allocWithZone:zone];				\
				return(gSharedInstance);									\
				}															\
		}																	\
																			\
		return(nil);														\
	}																		\
																			\
	- (id) copyWithZone:(NSZone *)zone										\
	{																		\
		(void) zone;														\
																			\
		return(self);														\
	}																		\
																			\
	- (id) retain															\
	{																		\
		return(self);														\
	}																		\
																			\
	- (unsigned) retainCount												\
	{																		\
		return(UINT_MAX);													\
	}																		\
																			\
	- (void) release														\
	{																		\
	}																		\
																			\
	- (id) autorelease														\
	{																		\
		return(self);														\
	}																		\
																			\
	void *kEatLastSemiColonForPedanticWarning ## _class





#endif // NNSSINGLETON_HDR




