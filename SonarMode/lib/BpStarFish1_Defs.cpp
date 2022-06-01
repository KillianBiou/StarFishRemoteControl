/*******************************************************************************
StarFish SDK Interface File
********************************************************************************
Copyright (c), Blueprint Design Engineering Ltd. (www.bpde.co.uk)
V1.1 - August 2010. Initial version of the file.
V1.2 - September 2010. Fixed some syntax bugs in function declatations.
********************************************************************************
C (& C++) header file for declaring constants, types and external functions
for use with "BpStarFish.dll" version 1.0.1 (or older).
The current library version can be obtained by calling "SfGetLibraryVersion".

For further details or technical support for using this header file please refer
to the "SDK Programmers Guide" documentation included in the StarFish SDK,
available from www.starfishsonar.com.

Please note that this code is provided "as is", as refer to the StarFish SDK
license file for further details about use, liability and restrictions.
********************************************************************************
DIRECTIONS:
Include this file in your MFC based C++ project and when the application
initalises, make a call to "SfLibLoad" to load the StarFish DLL into memory and
initalise a series of 'delegate' functions that will point to the DLL functions.

When closing your application, call "SfLibUnload" to remove the DLL from memory

The "hSfLib" pointer will be Null (0) when the DLL isn't loaded, and a non-zero
value when it is. Likewise, all the functions (i.e. "SfCreate", "SfOpen",
"SfClose", etc) will be null when the library isn't loaded, and the address of
the function to call when the library is.
*******************************************************************************/
#include "stdafx.h"
#include "StarFishDefs.h"

//Declare a variable to hold a pointer to the library object in
HINSTANCE				hSfLib = NULL;

//Alocate storage for the function pointers
//Standard Sonar creation and disposal functions
TSfCreate				SfCreate = NULL;
TSfCreated				SfCreated = NULL;
TSfFree					SfFree = NULL;
TSfGetLibraryVersion	SfGetLibraryVersion = NULL;
//Standard sonar control functions
TSfClose				SfClose = NULL;
TSfDefaults				SfDefaults = NULL;
TSfOpen					SfOpen = NULL;
TSfRead					SfRead = NULL;
TSfStop					SfStop = NULL;
TSfStartOnce			SfStartOnce = NULL;
TSfStart				SfStart = NULL;
TSfUpdate				SfUpdate = NULL;
//Standard configuration and status functions
TSfGetChannelEn			SfGetChannelEn = NULL;
TSfGetConfig			SfGetConfig = NULL;
TSfGetOpen				SfGetOpen = NULL;
TSfGetProductInfo		SfGetProductInfo = NULL;
TSfGetRxCountScope		SfGetRxCountScope = NULL;
TSfGetRxCountSonar		SfGetRxCountSonar = NULL;
TSfGetRxDataScope		SfGetRxDataScope = NULL;
TSfGetRxDataSonar		SfGetRxDataSonar = NULL;
TSfGetSonarRange		SfGetSonarRange = NULL;
TSfGetSonarSamples		SfGetSonarSamples = NULL;
TSfGetSonarType			SfGetSonarType = NULL;
TSfGetSonarVos			SfGetSonarVos = NULL;
TSfSetChannelEn			SfSetChannelEn = NULL;
TSfSetSonarRange		SfSetSonarRange = NULL;
TSfSetSonarSamples		SfSetSonarSamples = NULL;
TSfSetSonarVos			SfSetSonarVos = NULL;
//Advanced configuration and status functions
TSfGetAdcFreq			SfGetAdcFreq = NULL;
TSfGetAdcLatch			SfGetAdcLatch = NULL;
TSfGetCapture			SfGetCapture = NULL;
TSfGetPingTiming		SfGetPingTiming = NULL;
TSfGetRxMode			SfGetRxMode = NULL;
TSfGetRxOffsetAdc		SfGetRxOffsetAdc = NULL;
TSfGetRxOffsetRsl		SfGetRxOffsetRsl = NULL;
TSfGetRxOversample		SfGetRxOversample = NULL;
TSfGetRxPulse			SfGetRxPulse = NULL;
TSfGetScope				SfGetScope = NULL;
TSfGetTvgEnable			SfGetTvgEnable = NULL;
TSfGetTvgFunc			SfGetTvgFunc = NULL;
TSfGetTvgSlope			SfGetTvgSlope = NULL;
TSfGetTxEnable			SfGetTxEnable = NULL;
TSfGetTxMuteRx			SfGetTxMuteRx = NULL;
TSfGetTxPower			SfGetTxPower = NULL;
TSfGetTxPulse			SfGetTxPulse = NULL;
TSfSetAdcLatch			SfSetAdcLatch = NULL;
TSfSetCapture			SfSetCapture = NULL;
TSfSetPingTiming		SfSetPingTiming = NULL;
TSfSetRxMode			SfSetRxMode = NULL;
TSfSetRxOffsetAdc		SfSetRxOffsetAdc = NULL;
TSfSetRxOffsetRsl		SfSetRxOffsetRsl = NULL;
TSfSetRxOversample		SfSetRxOversample = NULL;
TSfSetRxPulse			SfSetRxPulse = NULL;
TSfSetScope				SfSetScope = NULL;
TSfSetTvgEnable			SfSetTvgEnable = NULL;
TSfSetTvgFunc			SfSetTvgFunc = NULL;
TSfSetTvgSlope			SfSetTvgSlope = NULL;
TSfSetTxEnable			SfSetTxEnable = NULL;
TSfSetTxMuteRx			SfSetTxMuteRx = NULL;
TSfSetTxPower			SfSetTxPower = NULL;
TSfSetTxPulse			SfSetTxPulse = NULL;

//-----------------------------------------------------------------------------
//Function that dynamically loads the StarFish library and initalises
//pointers for calling the library functions
//	returns	: true if the library was loaded correctly, false if the library failed
bool SfLibLoad(void)
{
	if (hSfLib == NULL)
	{
		hSfLib = AfxLoadLibrary(_T("BpStarFish1"));
		if (hSfLib)
		{
			//If the library has loaded the initalise function pointers
			//Standard Sonar creation and disposal functions
			SfCreate = (TSfCreate)GetProcAddress(hSfLib, "SfCreate");
			SfCreated = (TSfCreated)GetProcAddress(hSfLib, "SfCreated");
			SfFree = (TSfFree)GetProcAddress(hSfLib, "SfFree");
			SfGetLibraryVersion = (TSfGetLibraryVersion)GetProcAddress(hSfLib, "SfGetLibraryVersion");
			//Standard sonar control functions
			SfClose = (TSfClose)GetProcAddress(hSfLib, "SfClose");
			SfDefaults = (TSfDefaults)GetProcAddress(hSfLib, "SfDefaults");
			SfOpen = (TSfOpen)GetProcAddress(hSfLib, "SfOpen");
			SfRead = (TSfRead)GetProcAddress(hSfLib, "SfRead");
			SfStop = (TSfStop)GetProcAddress(hSfLib, "SfStop");
			SfStartOnce = (TSfStartOnce)GetProcAddress(hSfLib, "SfStartOnce");
			SfStart = (TSfStart)GetProcAddress(hSfLib, "SfStart");
			SfUpdate = (TSfUpdate)GetProcAddress(hSfLib, "SfUpdate");
			//Standard configuration and status functions
			SfGetChannelEn = (TSfGetChannelEn)GetProcAddress(hSfLib, "SfGetChannelEn");
			SfGetConfig = (TSfGetConfig)GetProcAddress(hSfLib, "SfGetConfig");
			SfGetOpen = (TSfGetOpen)GetProcAddress(hSfLib, "SfGetOpen");
			SfGetProductInfo = (TSfGetProductInfo)GetProcAddress(hSfLib, "SfGetProductInfo");
			SfGetRxCountScope = (TSfGetRxCountScope)GetProcAddress(hSfLib, "SfGetRxCountScope");
			SfGetRxCountSonar = (TSfGetRxCountSonar)GetProcAddress(hSfLib, "SfGetRxCountSonar");
			SfGetRxDataScope = (TSfGetRxDataScope)GetProcAddress(hSfLib, "SfGetRxDataScope");
			SfGetRxDataSonar = (TSfGetRxDataSonar)GetProcAddress(hSfLib, "SfGetRxDataSonar");
			SfGetSonarRange = (TSfGetSonarRange)GetProcAddress(hSfLib, "SfGetSonarRange");
			SfGetSonarSamples = (TSfGetSonarSamples)GetProcAddress(hSfLib, "SfGetSonarSamples");
			SfGetSonarType = (TSfGetSonarType)GetProcAddress(hSfLib, "SfGetSonarType");
			SfGetSonarVos = (TSfGetSonarVos)GetProcAddress(hSfLib, "SfGetSonarVos");
			SfSetChannelEn = (TSfSetChannelEn)GetProcAddress(hSfLib, "SfSetChannelEn");
			SfSetSonarRange = (TSfSetSonarRange)GetProcAddress(hSfLib, "SfSetSonarRange");
			SfSetSonarSamples = (TSfSetSonarSamples)GetProcAddress(hSfLib, "SfSetSonarSamples");
			SfSetSonarVos = (TSfSetSonarVos)GetProcAddress(hSfLib, "SfSetSonarVos");
			//Advanced configuration and status functions
			SfGetAdcFreq = (TSfGetAdcFreq)GetProcAddress(hSfLib, "SfGetAdcFreq");
			SfGetAdcLatch = (TSfGetAdcLatch)GetProcAddress(hSfLib, "SfGetAdcLatch");
			SfGetCapture = (TSfGetCapture)GetProcAddress(hSfLib, "SfGetCapture");
			SfGetPingTiming = (TSfGetPingTiming)GetProcAddress(hSfLib, "SfGetPingTiming");
			SfGetRxMode = (TSfGetRxMode)GetProcAddress(hSfLib, "SfGetRxMode");
			SfGetRxOffsetAdc = (TSfGetRxOffsetAdc)GetProcAddress(hSfLib, "SfGetRxOffsetAdc");
			SfGetRxOffsetRsl = (TSfGetRxOffsetRsl)GetProcAddress(hSfLib, "SfGetRxOffsetRsl");
			SfGetRxOversample = (TSfGetRxOversample)GetProcAddress(hSfLib, "SfGetRxOversample");
			SfGetRxPulse = (TSfGetRxPulse)GetProcAddress(hSfLib, "SfGetRxPulse");
			SfGetScope = (TSfGetScope)GetProcAddress(hSfLib, "SfGetScope");
			SfGetTvgEnable = (TSfGetTvgEnable)GetProcAddress(hSfLib, "SfGetTvgEnable");
			SfGetTvgFunc = (TSfGetTvgFunc)GetProcAddress(hSfLib, "SfGetTvgFunc");
			SfGetTvgSlope = (TSfGetTvgSlope)GetProcAddress(hSfLib, "SfGetTvgSlope");
			SfGetTxEnable = (TSfGetTxEnable)GetProcAddress(hSfLib, "SfGetTxEnable");
			SfGetTxMuteRx = (TSfGetTxMuteRx)GetProcAddress(hSfLib, "SfGetTxMuteRx");
			SfGetTxPower = (TSfGetTxPower)GetProcAddress(hSfLib, "SfGetTxPower");
			SfGetTxPulse = (TSfGetTxPulse)GetProcAddress(hSfLib, "SfGetTxPulse");
			SfSetAdcLatch = (TSfSetAdcLatch)GetProcAddress(hSfLib, "SfSetAdcLatch");
			SfSetCapture = (TSfSetCapture)GetProcAddress(hSfLib, "SfSetCapture");
			SfSetPingTiming = (TSfSetPingTiming)GetProcAddress(hSfLib, "SfSetPingTiming");
			SfSetRxMode = (TSfSetRxMode)GetProcAddress(hSfLib, "SfSetRxMode");
			SfSetRxOffsetAdc = (TSfSetRxOffsetAdc)GetProcAddress(hSfLib, "SfSetRxOffsetAdc");
			SfSetRxOffsetRsl = (TSfSetRxOffsetRsl)GetProcAddress(hSfLib, "SfSetRxOffsetRsl");
			SfSetRxOversample = (TSfSetRxOversample)GetProcAddress(hSfLib, "SfSetRxOversample");
			SfSetRxPulse = (TSfSetRxPulse)GetProcAddress(hSfLib, "SfSetRxPulse");
			SfSetScope = (TSfSetScope)GetProcAddress(hSfLib, "SfSetScope");
			SfSetTvgEnable = (TSfSetTvgEnable)GetProcAddress(hSfLib, "SfSetTvgEnable");
			SfSetTvgFunc = (TSfSetTvgFunc)GetProcAddress(hSfLib, "SfSetTvgFunc");
			SfSetTvgSlope = (TSfSetTvgSlope)GetProcAddress(hSfLib, "SfSetTvgSlope");
			SfSetTxEnable = (TSfSetTxEnable)GetProcAddress(hSfLib, "SfSetTxEnable");
			SfSetTxMuteRx = (TSfSetTxMuteRx)GetProcAddress(hSfLib, "SfSetTxMuteRx");
			SfSetTxPower = (TSfSetTxPower)GetProcAddress(hSfLib, "SfSetTxPower");
			SfSetTxPulse = (TSfSetTxPulse)GetProcAddress(hSfLib, "SfSetTxPulse");

			//Indicate the library sucessfully loaded
			return TRUE;
		}
		else {
			//Reset function pointers to nulls
			SfLibUnload();
		}
	}
	//Indicate the library failed to load
	return FALSE;
}

//-----------------------------------------------------------------------------
//Function that removes the StarFish library from memory can cleans up afterwards
void SfLibUnload(void)
{
	if (hSfLib)
	{
		FreeLibrary(hSfLib);
	}
	//Standard Sonar creation and disposal functions
	SfCreate = NULL;
	SfCreated = NULL;
	SfFree = NULL;
	SfGetLibraryVersion = NULL;
	//Standard sonar control functions
	SfClose = NULL;
	SfDefaults = NULL;
	SfOpen = NULL;
	SfRead = NULL;
	SfStop = NULL;
	SfStartOnce = NULL;
	SfStart = NULL;
	SfUpdate = NULL;
	//Standard configuration and status functions
	SfGetChannelEn = NULL;
	SfGetConfig = NULL;
	SfGetOpen = NULL;
	SfGetProductInfo = NULL;
	SfGetRxCountScope = NULL;
	SfGetRxCountSonar = NULL;
	SfGetRxDataScope = NULL;
	SfGetRxDataSonar = NULL;
	SfGetSonarRange = NULL;
	SfGetSonarSamples = NULL;
	SfGetSonarType = NULL;
	SfGetSonarVos = NULL;
	SfSetChannelEn = NULL;
	SfSetSonarRange = NULL;
	SfSetSonarSamples = NULL;
	SfSetSonarVos = NULL;
	//Advanced configuration and status functions
	SfGetAdcFreq = NULL;
	SfGetAdcLatch = NULL;
	SfGetCapture = NULL;
	SfGetPingTiming = NULL;
	SfGetRxMode = NULL;
	SfGetRxOffsetAdc = NULL;
	SfGetRxOffsetRsl = NULL;
	SfGetRxOversample = NULL;
	SfGetRxPulse = NULL;
	SfGetScope = NULL;
	SfGetTvgEnable = NULL;
	SfGetTvgFunc = NULL;
	SfGetTvgSlope = NULL;
	SfGetTxEnable = NULL;
	SfGetTxMuteRx = NULL;
	SfGetTxPower = NULL;
	SfGetTxPulse = NULL;
	SfSetAdcLatch = NULL;
	SfSetCapture = NULL;
	SfSetPingTiming = NULL;
	SfSetRxMode = NULL;
	SfSetRxOffsetAdc = NULL;
	SfSetRxOffsetRsl = NULL;
	SfSetRxOversample = NULL;
	SfSetRxPulse = NULL;
	SfSetScope = NULL;
	SfSetTvgEnable = NULL;
	SfSetTvgFunc = NULL;
	SfSetTvgSlope = NULL;
	SfSetTxEnable = NULL;
	SfSetTxMuteRx = NULL;
	SfSetTxPower = NULL;
	SfSetTxPulse = NULL;
}

//*****************************************************************************
//End Of File