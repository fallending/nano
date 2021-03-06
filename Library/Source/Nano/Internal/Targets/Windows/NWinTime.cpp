/*	NAME:
		NWinTime.cpp

	DESCRIPTION:
		Windows time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <mmsystem.h>

#include "NDate.h"
#include "NWindows.h"
#include "NRegistry.h"
#include "NWinTarget.h"
#include "NTargetThread.h"
#include "NTargetTime.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// Timer info
typedef struct {
	UINT				timerID;
	NIndex				fireCount;
	NTimerFunctor		theFunctor;
	NTime				fireAfter;
	NTime				fireEvery;
} TimerInfo;


// Time zone info
//
// Documented at http://msdn.microsoft.com/en-us/library/ms725481(VS.85).aspx
typedef struct _REG_TZI_FORMAT {
    LONG				Bias;
    LONG				StandardBias;
    LONG				DaylightBias;
    SYSTEMTIME			StandardDate;
    SYSTEMTIME			DaylightDate;
} REG_TZI_FORMAT;


// Maps
typedef std::map<UINT_PTR, TimerInfo*>						TimerInfoMap;
typedef TimerInfoMap::iterator								TimerInfoMapIterator;
typedef TimerInfoMap::const_iterator						TimerInfoMapIteratorConstIterator;





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
static TimerInfoMap gTimers;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetTimeZoneName : Get a time zone name.
//----------------------------------------------------------------------------
static NString GetTimeZoneName(const NString &timeZone)
{	static NStringMap		sTimeZones;

	NStringMapConstIterator		theIter;
	NString						theName;



	// Populate the table
	//
	// Time zones are stored in the registry using their full names, and do not contain
	// an abbreviated name. This means we can't build a lookup table by scanning the
	// registry, but need to provide our own (incomplete) list of mappings.
	if (sTimeZones.empty())
		{
		sTimeZones["ADT"]	= "Atlantic Standard Time";
		sTimeZones["AKDT"]	= "Alaskan Standard Time";
		sTimeZones["AKST"]	= "Alaskan Standard Time";
		sTimeZones["ART"]	= "SA Eastern Standard Time";
		sTimeZones["AST"]	= "Atlantic Standard Time";
		sTimeZones["BDT"]	= "Central Asia Standard Time";
		sTimeZones["BRST"]	= "SA Eastern Standard Time";
		sTimeZones["BRT"]	= "SA Eastern Standard Time";
		sTimeZones["BST"]	= "GMT Standard Time";
		sTimeZones["CAT"]	= "South Africa Standard Time";
		sTimeZones["CDT"]	= "Central Standard Time";
		sTimeZones["CEST"]	= "Central Europe Standard Time";
		sTimeZones["CET"]	= "Central Europe Standard Time";
		sTimeZones["CLST"]	= "SA Western Standard Time";
		sTimeZones["CLT"]	= "SA Western Standard Time";
		sTimeZones["COT"]	= "Central Standard Time";
		sTimeZones["CST"]	= "Central Standard Time";
		sTimeZones["EAT"]	= "E. Africa Standard Time";
		sTimeZones["EDT"]	= "Eastern Standard Time";
		sTimeZones["EEST"]	= "E. Europe Standard Time";
		sTimeZones["EET"]	= "E. Europe Standard Time";
		sTimeZones["EST"]	= "Eastern Standard Time";
		sTimeZones["GMT"]	= "Greenwich Standard Time";
		sTimeZones["GST"]	= "Arabian Standard Time";
		sTimeZones["HKT"]	= "China Standard Time";
		sTimeZones["HST"]	= "Hawaiian Standard Time";
		sTimeZones["ICT"]	= "SE Asia Standard Time";
		sTimeZones["IRST"]	= "Iran Standard Time";
		sTimeZones["IST"]	= "India Standard Time";
		sTimeZones["JST"]	= "Tokyo Standard Time";
		sTimeZones["KST"]	= "Korea Standard Time";
		sTimeZones["MDT"]	= "Mountain Standard Time";
		sTimeZones["MSD"]	= "E. Europe Standard Time";
		sTimeZones["MSK"]	= "E. Europe Standard Time";
		sTimeZones["MST"]	= "Mountain Standard Time";
		sTimeZones["NZDT"]	= "New Zealand Standard Time";
		sTimeZones["NZST"]	= "New Zealand Standard Time";
		sTimeZones["PDT"]	= "Pacific Standard Time";
		sTimeZones["PET"]	= "SA Pacific Standard Time";
		sTimeZones["PHT"]	= "Taipei Standard Time";
		sTimeZones["PKT"]	= "West Asia Standard Time";
		sTimeZones["PST"]	= "Pacific Standard Time";
		sTimeZones["SGT"]	= "Singapore Standard Time";
		sTimeZones["UTC"]	= "Greenwich Standard Time";
		sTimeZones["WAT"]	= "W. Central Africa Standard Time";
		sTimeZones["WEST"]	= "W. Europe Standard Time";
		sTimeZones["WET"]	= "W. Europe Standard Time";
		sTimeZones["WIT"]	= "SE Asia Standard Time";
		}



	// Get the name
	//
	// Unknown names should be added to the above table.
	theIter = sTimeZones.find(timeZone);
	if (theIter == sTimeZones.end())
		{
		NN_LOG("Unknown time zone: '%@'", timeZone);
		theIter = sTimeZones.find("UTC");
		}

	NN_ASSERT(theIter != sTimeZones.end());
	theName = theIter->second;
	
	return(theName);
}





//============================================================================
//		GetTimeZone : Get a time zone.
//----------------------------------------------------------------------------
static TIME_ZONE_INFORMATION GetTimeZone(const NString &timeZone)
{	NString						zoneName, subKey;
	NRegistry					theRegistry;
	TIME_ZONE_INFORMATION		zoneInfo;
	REG_TZI_FORMAT				regInfo;
	NData						theData;
	NStatus						theErr;



	// Get the state we need
	memset(&zoneInfo, 0x00, sizeof(zoneInfo));



	// Get the default time zone
	if (timeZone == kNTimeZoneDefault)
		{
		theErr = GetTimeZoneInformation(&zoneInfo);
		NN_ASSERT(SUCCEEDED(theErr));
		}
	
	
	// Get a named time zone
	else
		{
		// Get the state we need
		zoneName = GetTimeZoneName(timeZone);
		subKey   = NString("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\") + zoneName;


		// Get the time zone info
		theErr = theRegistry.Open(HKEY_LOCAL_MACHINE, subKey);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == kNoErr)
			{
			theData = theRegistry.GetValueData("TZI");
			if (theData.GetSize() == sizeof(regInfo))
				{
				memcpy(&regInfo, theData.GetData(), sizeof(regInfo));
				
				zoneInfo.Bias         = regInfo.Bias;
				zoneInfo.StandardDate = regInfo.StandardDate;
				zoneInfo.StandardBias = regInfo.StandardBias;
				zoneInfo.DaylightDate = regInfo.DaylightDate;
				zoneInfo.DaylightBias = regInfo.DaylightBias;
				}
			}
		}

	return(zoneInfo);
}





//============================================================================
//		GetTimerMS : Get a timer time.
//----------------------------------------------------------------------------
static DWORD GetTimerMS(NTime theTime)
{	DWORD		timeMS;



	// Get the time
	timeMS = NWinTarget::ConvertTimeMS(theTime);
	timeMS = NN_CLAMP_VALUE(timeMS, (DWORD) USER_TIMER_MINIMUM, (DWORD) USER_TIMER_MAXIMUM);
	
	return(timeMS);
}





//============================================================================
//		TimerCallback : Timer callback.
//----------------------------------------------------------------------------
static void CALLBACK TimerCallback(HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR idEvent, DWORD /*dwTime*/)
{	TimerInfo				*timerInfo;
	TimerInfoMapIterator	theIter;
	NTimerID				timerID;
	UINT					newTime;



	// Get the state we need
	//
	// Timers have been seen to fire ever after KillTimer, so
	// we need to check it still exists before we process it.
	theIter = gTimers.find(idEvent);
	if (theIter == gTimers.end())
		return;

	timerInfo = theIter->second;
	NN_ASSERT(timerInfo != NULL);



	// Invoke the timer
	//
	// Since Windows timers only have a single fire time, even one-shot
	// NTimers must fire forever.
	//
	// To minimise overhead we adjust one-shot timers to have the maximum
	// fire time (currently about 25 days) after their initial pass, but
	// for correct behaviour we can only ever fire them once.
	//
	// Repeating NTimers can fire every time their Windows timer is invoked.
	timerInfo->fireCount++;

	if (timerInfo->fireCount == 1 || !NMathUtilities::IsZero(timerInfo->fireEvery))
		timerInfo->theFunctor(kNTimerFired);



	// Check our state
	//
	// The functor may have destroyed the timer, so we need to check it still exists.
	theIter = gTimers.find(idEvent);
	if (theIter == gTimers.end())
		return;



	// Reschedule the timer
	//
	// If we're a one-shot NTimer then we're done; we will never invoke
	// the functor again. However, the Windows timer will keep firing so
	// we increase the timeout to the maximum to reduce overhead.
	//
	// If we're a repeating NTimer then we need to adjust our Windows
	// fire time to match the NTimer interval.
	//
	// In both cases we only need to adjust the timer once, after it has
	// fired the first time.
	if (timerInfo->fireCount == 1)
		{
		if (NMathUtilities::IsZero(timerInfo->fireEvery))
			newTime = USER_TIMER_MAXIMUM;
		else
			newTime = GetTimerMS(timerInfo->fireEvery);

		timerID = SetTimer(NULL, timerInfo->timerID, newTime, TimerCallback);
		NN_ASSERT(timerID == idEvent);
		}
}





//============================================================================
//		DoTimerCreate : Create a timer.
//----------------------------------------------------------------------------
static void DoTimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery, NTimerID *theTimer)
{	TimerInfo		*timerInfo;



	// Validate our state
	NN_ASSERT(sizeof(UINT_PTR) <= sizeof(NTimerID));



	// Allocate the timer info
	timerInfo = new TimerInfo;
	*theTimer = (NTimerID) timerInfo;

	timerInfo->timerID    = 0;
	timerInfo->fireCount  = 0;
	timerInfo->theFunctor = theFunctor;
	timerInfo->fireAfter  = fireAfter;
	timerInfo->fireEvery  = fireEvery;



	// Create the timer
	timerInfo->timerID = SetTimer(NULL, 0, GetTimerMS(fireAfter), TimerCallback);
	NN_ASSERT(timerInfo->timerID != 0);

	if (timerInfo->timerID == 0)
		{
		delete timerInfo;
		return;
		}



	// Save the timer
	NN_ASSERT(gTimers.find(timerInfo->timerID) == gTimers.end());
	gTimers[timerInfo->timerID] = timerInfo;
}





//============================================================================
//		DoTimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
static void DoTimerDestroy(NTimerID theTimer)
{	TimerInfo		*timerInfo = (TimerInfo *) theTimer;
	BOOL			wasOK;



	// Remove the timer
	wasOK = KillTimer(NULL, timerInfo->timerID);
	NN_ASSERT(wasOK);



	// Forget the timer
	NN_ASSERT(gTimers.find(timerInfo->timerID) != gTimers.end());
	gTimers.erase(timerInfo->timerID);
	
	delete timerInfo;
}





//============================================================================
//		DoTimerReset : Reset a timer.
//----------------------------------------------------------------------------
static void DoTimerReset(NTimerID theTimer, NTime fireAfter)
{	TimerInfo		*timerInfo = (TimerInfo *) theTimer;
	UINT_PTR		timerID;



	// Reset the timer
	timerInfo->fireCount = 0;

	timerID = SetTimer(NULL, timerInfo->timerID, GetTimerMS(fireAfter), TimerCallback);
	NN_ASSERT(timerID == timerInfo->timerID);
}





#pragma mark NTargetTime
//============================================================================
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
NTime NTargetTime::GetTime(void)
{	FILETIME		fileTime;
	NTime			theTime;



	// Get the time
	GetSystemTimeAsFileTime(&fileTime);
	theTime = NWinTarget::ConvertFILETIME(fileTime);
	
	return(theTime);
}





//============================================================================
//		NTargetTime::GetUpTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTargetTime::GetUpTime(void)
{


	// Get the time
	return(((NTime) GetTickCount()) * kNTimeMillisecond);
}





//============================================================================
//		NTargetTime::TimerCreate : Create a timer.
//----------------------------------------------------------------------------
NTimerID NTargetTime::TimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery)
{	NTimerID	theTimer;



	// Create the timer
	//
	// Timers are per-thread on Windows, however Nano always executes timers on
	// the main thread and so we need to create the functor on the main thread.
	theTimer = kNTimerNone;
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerCreate, theFunctor, fireAfter, fireEvery, &theTimer));

	return(theTimer);
}





//============================================================================
//		NTargetTime::TimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerDestroy(NTimerID theTimer)
{


	// Destroy the timer
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerDestroy, theTimer));
}





//============================================================================
//		NTargetTime::TimerReset : Reset a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerReset(NTimerID theTimer, NTime fireAfter)
{


	// Reset the timer
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerReset, theTimer, fireAfter));
}





//============================================================================
//		NTargetTime::ConvertTimeToDate : Convert a UTC time to a date.
//----------------------------------------------------------------------------
NGregorianDate NTargetTime::ConvertTimeToDate(NTime theTime, const NString &timeZone)
{	SYSTEMTIME					timeUTC, timeLocal;
	TIME_ZONE_INFORMATION		zoneInfo;
	FILETIME					fileTime;
	NGregorianDate				theDate;



	// Get the state we need
	//
	// Since SYSTEMTIME uses integer milliseconds, we add 0.5 of a
	// millisecond to ensure times are consistently rounded upwards.
	zoneInfo = GetTimeZone(timeZone);
	fileTime = NWinTarget::ConvertTimeFile(theTime + (0.5 * kNTimeMillisecond));



	// Convert the time
	if (!FileTimeToSystemTime(&fileTime, &timeUTC))
		memset(&timeUTC, 0x00, sizeof(timeUTC));

	if (!SystemTimeToTzSpecificLocalTime(&zoneInfo, &timeUTC, &timeLocal))
		memset(&timeLocal, 0x00, sizeof(timeLocal));

	theDate.year     = (int32_t) timeLocal.wYear;
	theDate.month    = (int8_t)  timeLocal.wMonth;
	theDate.day      = (int8_t)  timeLocal.wDay;
	theDate.hour     = (int8_t)  timeLocal.wHour;
	theDate.minute   = (int8_t)  timeLocal.wMinute;
	theDate.second   = ((NTime)  timeLocal.wSecond) + (((NTime) timeLocal.wMilliseconds) * kNTimeMillisecond);
	theDate.timeZone = timeZone;

	return(theDate);
}





//============================================================================
//		NTargetTime::ConvertDateToTime : Convert a date to a UTC time.
//----------------------------------------------------------------------------
NTime NTargetTime::ConvertDateToTime(const NGregorianDate &theDate)
{	SYSTEMTIME					timeUTC, timeLocal;
	TIME_ZONE_INFORMATION		zoneInfo;
	FILETIME					fileTime;
	double						timeSecs;
	NTime						theTime;



	// Validate our parameters
	NN_ASSERT(theDate.year >= 1601 && theDate.year <= 30827);



	// Get the state we need
	zoneInfo = GetTimeZone(theDate.timeZone);
	timeSecs = floor(theDate.second);

	timeLocal.wYear         = (WORD) theDate.year;
	timeLocal.wMonth        = (WORD) theDate.month;
	timeLocal.wDayOfWeek    = (WORD) 0;
	timeLocal.wDay          = (WORD) theDate.day;
	timeLocal.wHour         = (WORD) theDate.hour;
	timeLocal.wMinute       = (WORD) theDate.minute;
	timeLocal.wSecond       = (WORD) timeSecs;
	timeLocal.wMilliseconds = (WORD) ((theDate.second - timeSecs) / kNTimeMillisecond);



	// Convert the time
	if (FAILED(TzSpecificLocalTimeToSystemTime(&zoneInfo, &timeLocal, &timeUTC)))
		return(0.0);

	if (!SystemTimeToFileTime(&timeUTC, &fileTime))
		return(0.0);
	
	theTime = NWinTarget::ConvertFILETIME(fileTime);

	return(theTime);
}





//============================================================================
//		NTargetTime::GetDayOfWeek : Get the day of the week.
//----------------------------------------------------------------------------
NIndex NTargetTime::GetDayOfWeek(const NGregorianDate &theDate)
{	SYSTEMTIME					timeUTC, timeLocal;
	TIME_ZONE_INFORMATION		zoneInfo;
	double						timeSecs;



	// Validate our parameters
	NN_ASSERT(theDate.year >= 1601 && theDate.year <= 30827);



	// Get the state we need
	zoneInfo = GetTimeZone(theDate.timeZone);
	timeSecs = floor(theDate.second);

	timeLocal.wYear         = (WORD) theDate.year;
	timeLocal.wMonth        = (WORD) theDate.month;
	timeLocal.wDayOfWeek    = (WORD) 0;
	timeLocal.wDay          = (WORD) theDate.day;
	timeLocal.wHour         = (WORD) theDate.hour;
	timeLocal.wMinute       = (WORD) theDate.minute;
	timeLocal.wSecond       = (WORD) timeSecs;
	timeLocal.wMilliseconds = (WORD) ((theDate.second - timeSecs) / kNTimeMillisecond);



	// Convert the time
	//
	// SYSTEMTIME identifies days as 0..6, from Sun..Sat.
	if (FAILED(TzSpecificLocalTimeToSystemTime(&zoneInfo, &timeLocal, &timeUTC)))
		return(1);

	if (timeUTC.wDayOfWeek == 0)
		timeUTC.wDayOfWeek = 7;
	
	return(timeUTC.wDayOfWeek);
}


