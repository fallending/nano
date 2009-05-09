/*	NAME:
		NanoTypes.h

	DESCRIPTION:
		Nano data types.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_TYPES_HDR
#define NANO_TYPES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <map>

#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Primitives
typedef unsigned char												UInt8;
typedef unsigned short												UInt16;
typedef unsigned long												UInt32;
typedef unsigned long long											UInt64;

typedef signed char													SInt8;
typedef signed short												SInt16;
typedef signed long													SInt32;
typedef signed long long											SInt64;

typedef float														Float32;
typedef double														Float64;


// Characters
typedef UInt8														UTF8Char;
typedef UInt16														UTF16Char;
typedef UInt32														UTF32Char;


// Misc
typedef SInt32														NIndex;
typedef SInt32														NStatus;
typedef UInt32														NHashCode;
typedef UInt32														NBitfield;
typedef Float64														NTime;


// Lists
typedef std::vector<UInt8>											UInt8List;
typedef UInt8List::iterator											UInt8ListIterator;
typedef UInt8List::const_iterator									UInt8ListConstIterator;

typedef std::vector<UInt16>											UInt16List;
typedef UInt16List::iterator										UInt16ListIterator;
typedef UInt16List::const_iterator									UInt16ListConstIterator;

typedef std::vector<UInt32>											UInt32List;
typedef UInt32List::iterator										UInt32ListIterator;
typedef UInt32List::const_iterator									UInt32ListConstIterator;

typedef std::vector<UInt64>											UInt64List;
typedef UInt64List::iterator										UInt64ListIterator;
typedef UInt64List::const_iterator									UInt64ListConstIterator;


typedef std::vector<SInt8>											SInt8List;
typedef SInt8List::iterator											SInt8ListIterator;
typedef SInt8List::const_iterator									SInt8ListConstIterator;

typedef std::vector<SInt16>											SInt16List;
typedef SInt16List::iterator										SInt16ListIterator;
typedef SInt16List::const_iterator									SInt16ListConstIterator;

typedef std::vector<SInt32>											SInt32List;
typedef SInt32List::iterator										SInt32ListIterator;
typedef SInt32List::const_iterator									SInt32ListConstIterator;

typedef std::vector<SInt64>											SInt64List;
typedef SInt64List::iterator										SInt64ListIterator;
typedef SInt64List::const_iterator									SInt64ListConstIterator;


typedef std::vector<Float32>										Float32List;
typedef Float32List::iterator										Float32ListIterator;
typedef Float32List::const_iterator									Float32ListConstIterator;

typedef std::vector<Float64>										Float64List;
typedef Float64List::iterator										Float64ListIterator;
typedef Float64List::const_iterator									Float64ListConstIterator;


typedef std::vector<NIndex>											NIndexList;
typedef NIndexList::iterator										NIndexListIterator;
typedef NIndexList::const_iterator									NIndexListConstIterator;

typedef std::vector<NHashCode>										NHashCodeList;
typedef NHashCodeList::iterator										NHashCodeListIterator;
typedef NHashCodeList::const_iterator								NHashCodeListConstIterator;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Limits
static const UInt8  kUInt8Min										= 0;
static const UInt8  kUInt8Max										= UCHAR_MAX;
static const UInt16 kUInt16Min										= 0;
static const UInt16 kUInt16Max										= USHRT_MAX;
static const UInt32 kUInt32Min										= 0;
static const UInt32 kUInt32Max										= ULONG_MAX;
static const UInt64 kUInt64Min										= 0;
static const UInt64 kUInt64Max										= ULLONG_MAX;

static const SInt8  kSInt8Min										= SCHAR_MIN;
static const SInt8  kSInt8Max										= SCHAR_MAX;
static const SInt16 kSInt16Min										= SHRT_MIN;
static const SInt16 kSInt16Max										= SHRT_MAX;
static const SInt32 kSInt32Min										= LONG_MIN;
static const SInt32 kSInt32Max										= LONG_MAX;
static const SInt64 kSInt64Min										= LLONG_MIN;
static const SInt64 kSInt64Max										= LLONG_MAX;

static const Float32 kFloat32Min									= FLT_MIN;
static const Float32 kFloat32Max									= FLT_MAX;
static const Float64 kFloat64Min									= DBL_MIN;
static const Float64 kFloat64Max									= DBL_MAX;


// Misc
static const NIndex    kNIndexNone									= -1;
static const NHashCode kNHashCodeNone								= 0;


// Memory
static const UInt32 kKilobyte										= 1024;
static const UInt32 kMegabyte										= 1024 * kKilobyte;
static const UInt32 kGigabyte										= 1024 * kMegabyte;


// Times
static const NTime kNTimeNone										= 0.0;
static const NTime kNTimeNanosecond									= 1.0 / 1000000000.0;
static const NTime kNTimeMicrosecond								= 1.0 / 1000000.0;
static const NTime kNTimeMillisecond								= 1.0 / 1000.0;
static const NTime kNTimeSecond										= 1.0;
static const NTime kNTimeMinute										= 60.0 * kNTimeSecond;
static const NTime kNTimeHour										= 60.0 * kNTimeMinute;
static const NTime kNTimeDay										= 24.0 * kNTimeHour;
static const NTime kNTimeWeek										=  7.0 * kNTimeDay;
static const NTime kNTimeMonth										= 30.0 * kNTimeDay;
static const NTime kNTimeForever									= -1.0;


// Status
static const NStatus kNoErr											= 0;
static const NStatus kNErrParam										= -1;
static const NStatus kNErrTimeout									= -2;
static const NStatus kNErrMemory									= -3;
static const NStatus kNErrMalformed									= -4;
static const NStatus kNErrInternal									= -5;
static const NStatus kNErrPermission								= -7;
static const NStatus kNErrExhaustedSrc								= -8;
static const NStatus kNErrExhaustedDst								= -9;




#endif // NANO_TYPES_HDR

