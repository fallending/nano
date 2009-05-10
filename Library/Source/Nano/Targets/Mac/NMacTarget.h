/*	NAME:
		NMacTarget.h

	DESCRIPTION:
		Mac target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMAC_TARGET_HDR
#define NMAC_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMacTarget {
public:
	// Convert a status code
	static NStatus						ConvertOSStatus(OSStatus osErr);
	static NStatus						ConvertSysErr(int       sysErr);


	// Convert file permissions
	static const char					*ConvertPermission(NFilePermission thePermission);


	// Convert file positions
	static int							ConvertPosition(NFilePosition thePosition);
};





#endif // NMAC_TARGET_HDR
