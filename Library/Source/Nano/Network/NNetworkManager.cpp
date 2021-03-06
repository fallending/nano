/*	NAME:
		NNetworkManager.cpp

	DESCRIPTION:
		Network manager.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NThreadUtilities.h"
#include "NBundle.h"
#include "NTargetNetwork.h"
#include "NNetworkManager.h"





//============================================================================
//		NNetworkManager::NNetworkManager : Constructor.
//----------------------------------------------------------------------------
NNetworkManager::NNetworkManager(void)
{


	// Initialise ourselves
	mUserAgent = GetDefaultUserAgent();
}





//============================================================================
//		NNetworkManager::~NNetworkManager : Destructor.
//----------------------------------------------------------------------------
NNetworkManager::~NNetworkManager(void)
{
}





//============================================================================
//		NNetworkManager::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
bool NNetworkManager::IsReachable(const NURL &theURL)
{


	// Check the URL
	return(NTargetNetwork::IsReachable(theURL));
}





//============================================================================
//		NNetworkManager::GetUserAgent : Get the user agent.
//----------------------------------------------------------------------------
NString NNetworkManager::GetUserAgent(void) const
{


	// Get the user agent
	return(mUserAgent);
}





//============================================================================
//		NNetworkManager::SetUserAgent : Set the user agent.
//----------------------------------------------------------------------------
void NNetworkManager::SetUserAgent(const NString &userAgent)
{


	// Set the user agent
	mUserAgent = userAgent;
}





//============================================================================
//		NNetworkManager::Get : Get the instance.
//----------------------------------------------------------------------------
NNetworkManager *NNetworkManager::Get(void)
{	static NNetworkManager		*sInstance = NULL;



	// Get the instance
	return(CreateInstance<NNetworkManager>(&sInstance));
}





#pragma mark private
//============================================================================
//		NNetworkManager::GetDefaultUserAgent : Get the default user agent.
//----------------------------------------------------------------------------
NString NNetworkManager::GetDefaultUserAgent(void)
{	NString		appName, appVers, osName, sysArch;
	uint64_t		numCPUs, sysCPU, sysRAM;
	NBundle		appBundle;
	NString		theResult;



	// Get the state we needsys
	appName = appBundle.GetInfoString(kNBundleExecutableKey);
	appVers = appBundle.GetInfoString(kNBundleVersionKey);
	
	numCPUs = NThreadUtilities::GetCPUCount();
	osName  = NSystemUtilities::GetOSName();
	sysCPU  = NSystemUtilities::GetSystemCPU() / 1000000;
	sysRAM  = NSystemUtilities::GetSystemRAM() / kNMegabyte;
	sysArch = NSystemUtilities::GetSystemArch();



	// Get the user agent
	theResult.Format("%@/%@ (%@; %lldx%lldMhz; %lldMb; %@)",
						appName, appVers,
						osName,
						numCPUs, sysCPU,
						sysRAM,
						sysArch);

	return(theResult);
}



