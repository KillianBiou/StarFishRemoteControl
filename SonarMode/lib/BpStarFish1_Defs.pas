unit SF.SonarDefs;
{*******************************************************************************
StarFish SDK Definitions Unit
********************************************************************************
Copyright (c), Blueprint Design Engineering Ltd. (www.bpde.co.uk)
V1.0.1 - August 2010. Initial version of the file
********************************************************************************
Delphi definitions unit for declaring constants, types and external functions
for use with "BpStarFish.dll" version 1.0.1 (or older).
The current library version can be obtained by calling "SfGetLibraryVersion".

For further details or technical support for using this header file please refer
to the "SDK Programmers Guide" documentation included in the StarFish SDK,
available from www.starfishsonar.com.
*******************************************************************************}
interface

const
    //Constants for the "SfChannel" type (specifying sonar channels)
    SF_CHANNEL_STBD = 0;
    SF_CHANNEL_PORT = 1;

    //Constants for the "SfCaptureMode" type (specifying the caputre module mode)
  	SF_CAPTURE_SONAR = 0;
    SF_CAPTURE_SCOPE = 1;

    //Constants for the "SfResult" type
    SF_RES_OK = 0;
    SF_RES_STATE_ERROR = 1;
    SF_RES_COMMS_ERROR = 2;
    SF_RES_LIBRARY_ERROR = 3;
    SF_RES_SYSTEM_ERROR = 4;
    SF_RES_INVALID_HANDLE = 16;		//The specified sonar handle is invalid
    SF_RES_INVALID_CHANNEL = 17;	//The specified channel number is invalid

    //Contants for the "SfRxMode" type (specifying the receiver mode)
	SF_RX_MANUAL = 0;
	SF_RX_STANDARD = 1;
	SF_RX_SUBSAMPLE = 2;

    //Constants for the "SfSource" type (specifying the capture source)
    SF_SOURCE_WAVE = 0;
    SF_SOURCE_FILTER_RAW = 1;
    SF_SOURCE_FILTER_GAIN = 2;
    SF_SOURCE_GAIN = 3;
    SF_SOURCE_RSL_LONG = 4;
    SF_SOURCE_RSL_SHORT = 5;

   	//Bit definitions for Status Flags for the config structure
	SF_STATUS_OPEN = 0;
    SF_STATUS_ACTIVE = 1;
	SF_STATUS_TXEN = 8;
	SF_STATUS_TXMUTE = 9;
	SF_STATUS_TVGEN = 10;
	SF_STATUS_RXEN_STBD = 11;
	SF_STATUS_RXEN_PORT = 12;

    //Copnstants for the "SfType" type (specifying sonar types)
    SF_TYPE_450 = 1;
	SF_TYPE_990 = 2;

type
    //Pointer types
    PInt32	= ^Int32;
    PUInt32 = ^UInt32;

    //Type to hold the capture mode of the sonar
    SfCaptureMode = UInt32;
	PSfCaptureMode = ^SfCaptureMode;

    //Type to hold a sonar channel specifing value
    SfChannel = UInt32;

	//Type to hold a library handle to the starfish sonar instance
	SfHandle = Int32;
    PSfHandle = ^SfHandle;

	//Type to hold a result code from StarFish functions
    SfResult = UInt32;

	//Type to hold the receiver mode of the sonar
    SfRxMode = UInt32;
	PSfRxMode = ^SfRxMode;

    //Type to hold the capture source of the sonar
    SfSource = UInt32;
    PSfSource = ^SfSource;

    //Type to hold status flags in
    SfStatus = UInt32;
    PSfStatus = ^SfStatus;

    //Ttype to hold a sonar type specifying value
    SfType = UInt32;
    PSfType = ^SfType;

	//Define a structure that is used to send a full Status
	SfConfig = packed record
		LibVersion : UInt32;
		SonarType : UInt32;
		//Control Settings
		Status : SfStatus;
		RxMode : SfRxMode;
		CaptureMode : SfCaptureMode;
		//Transmitter Settings
		TxFreqStart : Double;
		TxFreqEnd : Double;
		TxFreqCentre : Double;
		TxFreqBW : Double;
		TxFreqQ : Double;
		TxLength : Double;
		TxPower : UInt32;
		//Receiver Settings
		RxFreqStart : Double;
		RxFreqEnd : Double;
		RxFreqCentre : Double;
		RxFreqBW : Double;
		RxFreqQ : Double;
		RxLength : Double;
		RxGainStbd : Int32;
		RxGainPort : Int32;
		RxOffsetAdcStbd : Int32;
		RxOffsetAdcPort : Int32;
		RxOffsetRsl : Int32;
		RxOversample : Double;
		RxOversampleMin : Double;
		RxOversampleMax : Double;
		//TVG
    	TvgAlpha : Double;
        TvgAttnMax : Double;
        TvgRangeMax : Double;
        TvgSamples : UInt32;
        TvgSlope : Double;
		//Filter
		AdcFreq : Double;
		AdcLatch : UInt32;
		FltTaps : UInt32;
		FltTapsPerBlock : UInt32;
		FltTapsPerBlockMax : UInt32;
		//Ping
        PingDuration : Double;
		PingIntervalScalar : Double;
		PingIntervalMin : Double;
		PingInterval : Double;
		//Scope
		ScopeSource : SfSource;
		ScopeRange : Double;
		ScopeSamples : UInt32;
		ScopeDelay : Int32;
		//Sonar
		SonarRange : Double;
		SonarSamples : UInt32;
		SonarVos : Double;
		//Capture
		CaptureSource : SfSource;
		CaptureRange : Double;
		CaptureSamples : UInt32;
		CaptureDelay : Int32;
		CaptureFreq : Double;
		CaptureDuration : Double;
	end;
	PSfConfig = ^SfConfig;

	//Record used to describe a part information (part number and version)
    SfPartInfo = packed record
        Number : UInt16;
        Version : Byte
    end;

    //Record used to report product information
    SfProductInfo = packed record
        ProductPart : SfPartInfo;
        ProductSerial : UInt32;
        HardwarePart : SfPartInfo;
        FirmwarePart : SfPartInfo;
        FirmwareChecksum : UInt16;
    end;
	PSfProductInfo = ^SfProductInfo;

    //Record to hold sonar data for a single channel
	SfDataSonarEcho = packed record
    	Samples : UInt32;
    	Data : array[0..2047] of Byte;
    end;

    //Structure that holds the Sonar Data Record
	SfDataSonar = packed record
        Range : Double;
        Channel : array[0..1] of SfDataSonarEcho;
    end;
    PSfDataSonar = ^SfDataSonar;

    //Record that holds the entries for a single sampe of received scope data
    SfDataScopeSample = record
        Time : Single;
        Range : Single;
        Channel : array[0..1] of Single;
    end;

    //Record that holds the Scope Data
    SfDataScope = packed record
        Source : SfSource;
        Range : Double;
    	Period : Double;
        Offset : Double;
        Samples : UInt32;
        Signal : array[0..2047] of SfDataScopeSample;
    end;
    PSfDataScope = ^SfDataScope;

//Imported Library Funnctions...
//Standard Sonar creation and disposal functions
function SfClose(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfCreate(stype : SfType; son : PSfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfCreated() : UInt32; stdcall; external 'BpStarFish1.dll';
function SfFree(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetLibraryVersion() : UInt32; stdcall; external 'BpStarFish1.dll';
//Standard sonar control functions
function SfDefaults(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfOpen(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfRead(hSon : SfHandle; msgs : PUInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfStop(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfStartOnce(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfStart(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfUpdate(hSon : SfHandle) : SfResult; stdcall; external 'BpStarFish1.dll';
//Standard configuration and status functions
function SfGetChannelEn(hSon : SfHandle; channel : SfChannel; enable : PByte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetConfig(hSon : SfHandle; cfg : PSfConfig) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetOpen(hSon : SfHandle; value : PByte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetProductInfo(hSon : SfHandle; info : PSfProductInfo) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxCountScope(hSon : SfHandle; count : PUInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxCountSonar(hSon : SfHandle; count : PUInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxDataScope(hSon : SfHandle; data : PSfDataScope) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxDataSonar(hSon : SfHandle; data : PSfDataSonar) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetSonarRange(hSon : SfHandle; range : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetSonarSamples(hSon : SfHandle; samples : PUInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetSonarType(hSon : SfHandle; stype : PSfType) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetSonarVos(hSon : SfHandle; vos : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetChannelEn(hSon : SfHandle; channel : SfChannel; enable : Byte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetSonarRange(hSon : SfHandle; range : Double) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetSonarSamples(hSon : SfHandle; samples : UInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetSonarVos(hSon : SfHandle; vos : Double) : SfResult; stdcall; external 'BpStarFish1.dll';
//Advanced configuration and status functions
function SfGetAdcFreq(hSon : SfHandle; freq : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetAdcLatch(hSon : SfHandle; latch : PUInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetCapture(hSon : SfHandle; mode : PSfCaptureMode; source : PSfSource; samples : PUInt32; delay : PInt32; range : PDouble; freq : PDouble; duration : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetPingTiming(hSon : SfHandle; min : PDouble; scalar : PDouble; interval : PDouble; duration : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxMode(hSon : SfHandle; mode : PSfRxMode) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxOffsetAdc(hSon : SfHandle; channel : SfChannel; offset : PInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxOffsetRsl(hSon : SfHandle; rsl : PInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxOversample(hSon : SfHandle; ovs : PDouble; ovsMin : PDouble; ovsMax : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetRxPulse(hSon : SfHandle; fstart : PDouble; fend : PDouble; length : PDouble; q : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetScope(hSon : SfHandle; source : PSfSource; samples : PUInt32; delay : PInt32; range : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetTvgEnable(hSon : SfHandle; value : PByte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetTvgFunc(hSon : SfHandle; alpha : PDouble; attnMax : PDouble; rangeMax : PDouble; samples : PUInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetTvgSlope(hSon : SfHandle; slope : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetTxEnable(hSon : SfHandle; value : PByte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetTxMuteRx(hSon : SfHandle; value : PByte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetTxPower(hSon : SfHandle; power : PByte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfGetTxPulse(hSon : SfHandle; fstart : PDouble; fend : PDouble; length : PDouble; q : PDouble) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetAdcLatch(hSon : SfHandle; latch : UInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetCapture(hSon : SfHandle; mode : SfCaptureMode) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetPingTiming(hSon : SfHandle; min : Double; scalar : Double) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetRxMode(hSon : SfHandle; mode : SfRxMode) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetRxOffsetAdc(hSon : SfHandle; channel : SfChannel; offset : Int32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetRxOffsetRsl(hSon : SfHandle; rsl : Int32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetRxOversample(hSon : SfHandle; ovs : Double) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetRxPulse(hSon : SfHandle; fstart : Double; fend : Double; length : Double) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetScope(hSon : SfHandle; source : SfSource; samples : UInt32; delay : Int32; range : Double) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetTvgEnable(hSon : SfHandle; value : Byte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetTvgFunc(hSon : SfHandle; alpha : Double; attnMax : Double; rangeMax : Double; samples : UInt32) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetTvgSlope(hSon : SfHandle; slope : Double) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetTxEnable(hSon : SfHandle; value : Byte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetTxMuteRx(hSon : SfHandle; value : Byte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetTxPower(hSon : SfHandle; power : Byte) : SfResult; stdcall; external 'BpStarFish1.dll';
function SfSetTxPulse(hSon : SfHandle; fstart : Double; fend : Double; length : Double) : SfResult; stdcall; external 'BpStarFish1.dll';

//******************************************************************************
implementation

//******************************************************************************
end.
