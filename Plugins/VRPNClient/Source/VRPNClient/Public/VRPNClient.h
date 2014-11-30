#pragma once
 
#include "ModuleManager.h"
#include "VRPNClientPrivatePCH.h"
 
class VRPNCLIENT_API VRPNClientImpl : public IModuleInterface
{
private:
	vrpn_Tracker_Remote iRemote;
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
	void initializeTracker();
};