/*******************************************************************************
StarFish SDK Header File
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
*******************************************************************************/
//Compiler Constants...
//******************************************************************************
//Ensure this header isn't included twice
#pragma once
#ifndef STARFISH_DEFS_H
#define STARFISH_DEFS_H

#ifndef WINAPI
#define WINAPI					__stdcall
#endif

//******************************************************************************
//Declare Constants...
//******************************************************************************
//Constants for the "SfChannel" type (specifying sonar channels)
#define SF_CHANNEL_STBD			0
#define SF_CHANNEL_PORT			1

//Constants for the "SfCaptureMode" type (specifying the caputre module mode)
#define SF_CAPTURE_SONAR		0
#define SF_CAPTURE_SCOPE		1

//Constants for the "SfResult" type
#define SF_RES_OK				0
#define SF_RES_STATE_ERROR		1
#define SF_RES_COMMS_ERROR		2
#define SF_RES_LIBRARY_ERROR	3
#define SF_RES_SYSTEM_ERROR		4
#define SF_RES_INVALID_HANDLE	16
#define SF_RES_INVALID_CHANNEL	17

//Contants for the "SfRxMode" type (specifying the receiver mode)
#define SF_RX_MANUAL			0
#define SF_RX_STANDARD			1
#define SF_RX_SUBSAMPLE			2

//Constants for the "SfSource" type (specifying the capture source)
#define SF_SOURCE_WAVE			0
#define SF_SOURCE_FILTER_RAW	1
#define SF_SOURCE_FILTER_GAIN	2
#define SF_SOURCE_GAIN			3
#define SF_SOURCE_RSL_LONG		4
#define SF_SOURCE_RSL_SHORT		5

//Bit definitions for Status Flags for the config structure
#define SF_STATUS_OPEN			0
#define SF_STATUS_ACTIVE		1
#define SF_STATUS_TXEN			8
#define SF_STATUS_TXMUTE		9
#define SF_STATUS_TVGEN			10
#define SF_STATUS_RXEN_STBD		11
#define SF_STATUS_RXEN_PORT		12

//Copnstants for the "SfType" type (specifying sonar types)
#define SF_TYPE_450				1
#define SF_TYPE_990				2

//******************************************************************************
//Declare Data Types...
//******************************************************************************
typedef unsigned char	Byte;
typedef short			Int16;
typedef unsigned short	UInt16;
typedef long			Int32;
typedef unsigned long	UInt32;
typedef float			Single;
typedef double			Double;

typedef Byte* PByte;
typedef Int16* PInt16;
typedef UInt16* PUInt16;
typedef Int32* PInt32;
typedef UInt32* PUInt32;
typedef float* PSingle;
typedef double* PDouble;

//Type to hold the capture mode of the sonar
typedef UInt32			SfCaptureMode;
typedef SfCaptureMode* PSfCaptureMode;

//Type to hold a sonar channel specifing value
typedef UInt32			SfChannel;

//Type to hold a library handle to the starfish sonar instance
typedef Int32			SfHandle;
typedef SfHandle* PSfHandle;

//Type to hold a result code from StarFish functions
typedef UInt32			SfResult;

//Type to hold the receiver mode of the sonar
typedef UInt32			SfRxMode;
typedef SfRxMode* PSfRxMode;

//Type to hold the capture source of the sonar
typedef UInt32			SfSource;
typedef SfSource* PSfSource;

//Type to hold status flags in
typedef UInt32			SfStatus;
typedef SfStatus* PSfStatus;

//Ttype to hold a sonar type specifying value
typedef UInt32			SfType;
typedef SfType* PSfType;

//Define a structure that is used to send a full Status
struct SfConfig {
	UInt32				LibVersion;
	UInt32				SonarType;
	//Control Settings
	SfStatus			Status;
	SfRxMode			RxMode;
	SfCaptureMode		CaptureMode;
	//Transmitter Settings
	Double				TxFreqStart;
	Double				TxFreqEnd;
	Double				TxFreqCentre;
	Double				TxFreqBW;
	Double				TxFreqQ;
	Double				TxLength;
	UInt32				TxPower;
	//Receiver Settings
	Double				RxFreqStart;
	Double				RxFreqEnd;
	Double				RxFreqCentre;
	Double				RxFreqBW;
	Double				RxFreqQ;
	Double				RxLength;
	Int32				RxGainStbd;
	Int32				RxGainPort;
	Int32				RxOffsetAdcStbd;
	Int32				RxOffsetAdcPort;
	Int32				RxOffsetRsl;
	Double				RxOversample;
	Double				RxOversampleMin;
	Double				RxOversampleMax;
	//TVG
	Double				TvgAlpha;
	Double				TvgAttnMax;
	Double				TvgRangeMax;
	UInt32				TvgSamples;
	Double				TvgSlope;
	//Filter
	Double				AdcFreq;
	UInt32				AdcLatch;
	UInt32				FltTaps;
	UInt32				FltTapsPerBlock;
	UInt32				FltTapsPerBlockMax;
	//Ping
	Double				PingDuration;
	Double				PingIntervalScalar;
	Double				PingIntervalMin;
	Double				PingInterval;
	//Scope
	SfSource			ScopeSource;
	Double				ScopeRange;
	UInt32				ScopeSamples;
	Int32				ScopeDelay;
	//Sonar
	Double				SonarRange;
	UInt32				SonarSamples;
	Double				SonarVos;
	//Capture	
	SfSource			CaptureSource;
	Double				CaptureRange;
	UInt32				CaptureSamples;
	Int32				CaptureDelay;
	Double				CaptureFreq;
	Double				CaptureDuration;
};
typedef SfConfig* PSfConfig;

//Record used to describe a part information (part number and version)
struct SfPartInfo {
	UInt16				Number;
	Byte				Version;
};

//Record used to report product information
struct SfProductInfo {
	SfPartInfo			ProductPart;
	UInt32				ProductSerial;
	SfPartInfo			HardwarePart;
	SfPartInfo			FirmwarePart;
	UInt16				FirmwareChecksum;
};
typedef SfProductInfo* PSfProductInfo;

//Record to hold sonar data for a single channel
struct SfDataSonarEcho {
	UInt32				Samples;
	Byte				Data[2048];
};

//Structure that holds the Sonar Data Record
struct SfDataSonar {
	Double				Range;
	SfDataSonarEcho		Channel[2];
};
typedef SfDataSonar* PSfDataSonar;

//Record that holds the entries for a single sampe of received scope data
struct SfDataScopeSample {
	Single				Time;
	Single				Range;
	Single				Channel[2];
};

//Record that holds the Scope Data
struct SfDataScope {
	SfSource			Source;
	Double				Range;
	Double				Period;
	Double				Offset;
	UInt32				Samples;
	SfDataScopeSample	Signal[2048];
};
typedef SfDataScope* PSfDataScope;

//Imported Function Prototypes...
//Standard Sonar creation and disposal functions
typedef SfResult(WINAPI* TSfCreate)(SfType stype, PSfHandle hSon);
typedef UInt32(WINAPI* TSfCreated)(void);
typedef SfResult(WINAPI* TSfFree)(SfHandle hSon);
typedef UInt32(WINAPI* TSfGetLibraryVersion)(void);
//Standard sonar control functions
typedef SfResult(WINAPI* TSfClose)(SfHandle hSon);
typedef SfResult(WINAPI* TSfDefaults)(SfHandle hSon);
typedef SfResult(WINAPI* TSfOpen)(SfHandle hSon);
typedef SfResult(WINAPI* TSfRead)(SfHandle hSon, PUInt32 msgs);
typedef SfResult(WINAPI* TSfStop)(SfHandle hSon);
typedef SfResult(WINAPI* TSfStartOnce)(SfHandle hSon);
typedef SfResult(WINAPI* TSfStart)(SfHandle hSon);
typedef SfResult(WINAPI* TSfUpdate)(SfHandle hSon);
//Standard configuration and status functions
typedef SfResult(WINAPI* TSfGetChannelEn)(SfHandle hSon, SfChannel Channel, PByte enable);
typedef SfResult(WINAPI* TSfGetConfig)(SfHandle hSon, PSfConfig cfg);
typedef SfResult(WINAPI* TSfGetOpen)(SfHandle hSon, PByte value);
typedef SfResult(WINAPI* TSfGetProductInfo)(SfHandle hSon, PSfProductInfo info);
typedef SfResult(WINAPI* TSfGetRxCountScope)(SfHandle hSon, PUInt32 count);
typedef SfResult(WINAPI* TSfGetRxCountSonar)(SfHandle hSon, PUInt32 count);
typedef SfResult(WINAPI* TSfGetRxDataScope)(SfHandle hSon, PSfDataScope Data);
typedef SfResult(WINAPI* TSfGetRxDataSonar)(SfHandle hSon, PSfDataSonar Data);
typedef SfResult(WINAPI* TSfGetSonarRange)(SfHandle hSon, PDouble Range);
typedef SfResult(WINAPI* TSfGetSonarSamples)(SfHandle hSon, PUInt32 Samples);
typedef SfResult(WINAPI* TSfGetSonarType)(SfHandle hSon, PSfType stype);
typedef SfResult(WINAPI* TSfGetSonarVos)(SfHandle hSon, PDouble vos);
typedef SfResult(WINAPI* TSfSetChannelEn)(SfHandle hSon, SfChannel Channel, Byte enable);
typedef SfResult(WINAPI* TSfSetSonarRange)(SfHandle hSon, Double Range);
typedef SfResult(WINAPI* TSfSetSonarSamples)(SfHandle hSon, UInt32 Samples);
typedef SfResult(WINAPI* TSfSetSonarVos)(SfHandle hSon, Double vos);
//Advanced configuration and status functions
typedef SfResult(WINAPI* TSfGetAdcFreq)(SfHandle hSon, PDouble freq);
typedef SfResult(WINAPI* TSfGetAdcLatch)(SfHandle hSon, PUInt32 latch);
typedef SfResult(WINAPI* TSfGetCapture)(SfHandle hSon, PSfCaptureMode mode, PSfSource source, PUInt32 Samples, PInt32 delay, PDouble range, PDouble freq, PDouble duration);
typedef SfResult(WINAPI* TSfGetPingTiming)(SfHandle hSon, PDouble min, PDouble scalar, PDouble interval, PDouble duration);
typedef SfResult(WINAPI* TSfGetRxMode)(SfHandle hSon, PSfRxMode mode);
typedef SfResult(WINAPI* TSfGetRxOffsetAdc)(SfHandle hSon, SfChannel Channel, PInt32 Offset);
typedef SfResult(WINAPI* TSfGetRxOffsetRsl)(SfHandle hSon, PInt32 rsl);
typedef SfResult(WINAPI* TSfGetRxOversample)(SfHandle hSon, PDouble ovs, PDouble ovsMin, PDouble ovsMax);
typedef SfResult(WINAPI* TSfGetRxPulse)(SfHandle hSon, PDouble fstart, PDouble fend, PDouble length, PDouble q);
typedef SfResult(WINAPI* TSfGetScope)(SfHandle hSon, PSfSource Source, PUInt32 Samples, PInt32 delay, PDouble Range);
typedef SfResult(WINAPI* TSfGetTvgEnable)(SfHandle hSon, PByte value);
typedef SfResult(WINAPI* TSfGetTvgFunc)(SfHandle hSon, PDouble alpha, PDouble attnMax, PDouble rangeMax, PUInt32 Samples);
typedef SfResult(WINAPI* TSfGetTvgSlope)(SfHandle hSon, PDouble slope);
typedef SfResult(WINAPI* TSfGetTxEnable)(SfHandle hSon, PByte value);
typedef SfResult(WINAPI* TSfGetTxMuteRx)(SfHandle hSon, PByte value);
typedef SfResult(WINAPI* TSfGetTxPower)(SfHandle hSon, PByte power);
typedef SfResult(WINAPI* TSfGetTxPulse)(SfHandle hSon, PDouble fstart, PDouble fend, PDouble length, PDouble q);
typedef SfResult(WINAPI* TSfSetAdcLatch)(SfHandle hSon, UInt32 latch);
typedef SfResult(WINAPI* TSfSetCapture)(SfHandle hSon, SfCaptureMode mode);
typedef SfResult(WINAPI* TSfSetPingTiming)(SfHandle hSon, Double min, Double scalar);
typedef SfResult(WINAPI* TSfSetRxMode)(SfHandle hSon, SfRxMode mode);
typedef SfResult(WINAPI* TSfSetRxOffsetAdc)(SfHandle hSon, SfChannel Channel, Int32 Offset);
typedef SfResult(WINAPI* TSfSetRxOffsetRsl)(SfHandle hSon, Int32 rsl);
typedef SfResult(WINAPI* TSfSetRxOversample)(SfHandle hSon, Double ovs);
typedef SfResult(WINAPI* TSfSetRxPulse)(SfHandle hSon, Double fstart, Double fend, Double length);
typedef SfResult(WINAPI* TSfSetScope)(SfHandle hSon, SfSource Source, UInt32 Samples, Int32 delay, Double Range);
typedef SfResult(WINAPI* TSfSetTvgEnable)(SfHandle hSon, Byte value);
typedef SfResult(WINAPI* TSfSetTvgFunc)(SfHandle hSon, Double alpha, Double attnMax, Double rangeMax, UInt32 Samples);
typedef SfResult(WINAPI* TSfSetTvgSlope)(SfHandle hSon, Double slope);
typedef SfResult(WINAPI* TSfSetTxEnable)(SfHandle hSon, Byte value);
typedef SfResult(WINAPI* TSfSetTxMuteRx)(SfHandle hSon, Byte value);
typedef SfResult(WINAPI* TSfSetTxPower)(SfHandle hSon, Byte power);
typedef SfResult(WINAPI* TSfSetTxPulse)(SfHandle hSon, Double fstart, Double fend, Double length);

//******************************************************************************
//Declare Allocated Storage For Variables & Function Pointers
//******************************************************************************
extern HINSTANCE hSfLib;

//Declare indirect function call variables
//Standard Sonar creation and disposal functions
extern TSfCreate			SfCreate;
extern TSfCreated			SfCreated;
extern TSfFree				SfFree;
extern TSfGetLibraryVersion	SfGetLibraryVersion;
//Standard sonar control functions
extern TSfClose				SfClose;
extern TSfDefaults			SfDefaults;
extern TSfOpen				SfOpen;
extern TSfRead				SfRead;
extern TSfStop				SfStop;
extern TSfStartOnce			SfStartOnce;
extern TSfStart				SfStart;
extern TSfUpdate			SfUpdate;
//Standard configuration and status functions
extern TSfGetChannelEn		SfGetChannelEn;
extern TSfGetConfig			SfGetConfig;
extern TSfGetOpen			SfGetOpen;
extern TSfGetProductInfo	SfGetProductInfo;
extern TSfGetRxCountScope	SfGetRxCountScope;
extern TSfGetRxCountSonar	SfGetRxCountSonar;
extern TSfGetRxDataScope	SfGetRxDataScope;
extern TSfGetRxDataSonar	SfGetRxDataSonar;
extern TSfGetSonarRange		SfGetSonarRange;
extern TSfGetSonarSamples	SfGetSonarSamples;
extern TSfGetSonarType		SfGetSonarType;
extern TSfGetSonarVos		SfGetSonarVos;
extern TSfSetChannelEn		SfSetChannelEn;
extern TSfSetSonarRange		SfSetSonarRange;
extern TSfSetSonarSamples	SfSetSonarSamples;
extern TSfSetSonarVos		SfSetSonarVos;
//Advanced configuration and status functions
extern TSfGetAdcFreq		SfGetAdcFreq;
extern TSfGetAdcLatch		SfGetAdcLatch;
extern TSfGetCapture		SfGetCapture;
extern TSfGetPingTiming		SfGetPingTiming;
extern TSfGetRxMode			SfGetRxMode;
extern TSfGetRxOffsetAdc	SfGetRxOffsetAdc;
extern TSfGetRxOffsetRsl	SfGetRxOffsetRsl;
extern TSfGetRxOversample	SfGetRxOversample;
extern TSfGetRxPulse		SfGetRxPulse;
extern TSfGetScope			SfGetScope;
extern TSfGetTvgEnable		SfGetTvgEnable;
extern TSfGetTvgFunc		SfGetTvgFunc;
extern TSfGetTvgSlope		SfGetTvgSlope;
extern TSfGetTxEnable		SfGetTxEnable;
extern TSfGetTxMuteRx		SfGetTxMuteRx;
extern TSfGetTxPower		SfGetTxPower;
extern TSfGetTxPulse		SfGetTxPulse;
extern TSfSetAdcLatch		SfSetAdcLatch;
extern TSfSetCapture		SfSetCapture;
extern TSfSetPingTiming		SfSetPingTiming;
extern TSfSetRxMode			SfSetRxMode;
extern TSfSetRxOffsetAdc	SfSetRxOffsetAdc;
extern TSfSetRxOffsetRsl	SfSetRxOffsetRsl;
extern TSfSetRxOversample	SfSetRxOversample;
extern TSfSetRxPulse		SfSetRxPulse;
extern TSfSetScope			SfSetScope;
extern TSfSetTvgEnable		SfSetTvgEnable;
extern TSfSetTvgFunc		SfSetTvgFunc;
extern TSfSetTvgSlope		SfSetTvgSlope;
extern TSfSetTxEnable		SfSetTxEnable;
extern TSfSetTxMuteRx		SfSetTxMuteRx;
extern TSfSetTxPower		SfSetTxPower;
extern TSfSetTxPulse		SfSetTxPulse;

//******************************************************************************
//Helper Function Prototypes
//******************************************************************************
bool SfLibLoad(void);
void SfLibUnload(void);

//******************************************************************************
#endif	/*STARFISH_DEFS_H*/