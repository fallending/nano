/*	NAME:
		NTargetPOSIX.h

	DESCRIPTION:
		Thread-safe POSIX support.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_POSIX_HDR
#define NTARGET_POSIX_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetPOSIX {
public:
	// math.h
	static bool							is_nan(Float64 r);
	static bool							is_inf(Float64 r);

	
	// stdio.h
	static NIndex						snprintf( char *s, size_t n, const char *format, ...);
	static NIndex						vsnprintf(char *s, size_t n, const char *format, va_list argList);


	// time.h
	static struct tm					gmtime(time_t theTime);
	static time_t						timegm(const struct tm *tm);


	// unistd.h
	static char *						getcwd(char *buf, size_t size);
};





#endif // NTARGET_POSIX_HDR
