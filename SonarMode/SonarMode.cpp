#ifndef UNICODE
#define UNICODE
#endif 

#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <tchar.h>
#include <stdio.h>
#include <ctime>
#include <time.h>
#include <sstream>
#include "lib\BpStarFish1_Defs.h"
#include "sqlite3.h"

using namespace std;

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
		hSfLib = LoadLibrary(L"BpStarFish1.DLL");
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

void printLogInfo() {
	time_t currentTime = time(0);
	cout << "\n\n\n********************************************************************************" << endl;
	cout << "LOG TIMESTAMP: " << asctime(localtime(&currentTime)) << endl;
	cout << "********************************************************************************\n" << endl;
}

void runningUpdateDB(sqlite3* db, char* zErrMsg) {
	string sqlQuery = "UPDATE SFParameters SET value = 1 WHERE name = 'state'";
	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
	int res = sqlite3_exec(db, sqlQuery.c_str(), NULL, 0, &zErrMsg);
	if (res) {
		cout << "change not successfull" << endl;
	}
	else {
		cout << "change successfull" << endl;
		sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
	}
}

void crashedUpdateDB(sqlite3* db, char* zErrMsg) {
	string sqlQuery = "UPDATE SFParameters SET value = -1 WHERE name = 'state'";
	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
	int res = sqlite3_exec(db, sqlQuery.c_str(), NULL, 0, &zErrMsg);
	if (res) {
		cout << "change not successfull" << endl;
	}
	else {
		cout << "change successfull" << endl;
		sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
	}
}

int main(int argc, char* argv[])
{
	printLogInfo();

	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	string sqlQuery;
	rc = sqlite3_open("database.db", &db);

	runningUpdateDB(db, zErrMsg);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		crashedUpdateDB(db, zErrMsg);
		return(-2);
	}
	else {
		fprintf(stdout, "Opened database successfully\n");
	}

	//Chargement de la Librairie
	bool lib = SfLibLoad();
	if (lib) {
		cout << "Library successfully load" << endl;
	}
	else {
		cout << "Library loading failed" << endl;
		crashedUpdateDB(db, zErrMsg);
		// CODE ERROR : 1 -> Problem with the external Library
		return 1;
	}
	if(argc != 8) {
		cout << "Launch StarFishScanner.py, not this executable" << endl;
		crashedUpdateDB(db, zErrMsg);
		exit(-1);
	}

	int timeSeparator = atoi(argv[1]);
	Double range = stod(argv[2]);
	UInt32 samples = stod(argv[3]);
	Double vos = stod(argv[4]);
	UInt32 gain = stod(argv[5]);
	float contrast = stod(argv[6]);
	string outputFolder = argv[7];

	//Déclaration des variables
	SfHandle hdl;
	PSfHandle hSon = &hdl;
	SfResult resCreate, resOpen, resUpdate, resStart, resRead, resCount, resGetData, resClose;
	UInt32 msg, count;
	PUInt32 pmsg = &msg, pcount = &count;
	SfDataSonar data;
	PSfDataSonar pdata = &data;
	int val, nbmsg, nbdata = 0;

	//Check de la mise en route
	resCreate = SfCreate(SF_TYPE_990, hSon);
	if (resCreate == 0) {
		cout << "Creation done" << endl;
	}
	else {
		cout << "Creation of the SonarFish device failed" << endl;
		crashedUpdateDB(db, zErrMsg);
		// CODE ERROR : 2 -> Problem with the creation of the SonarFish Device
		return 2;
	}
	
	resOpen = SfOpen(hdl);
	if (resOpen == SF_RES_OK) {
		cout << "Opening done" << endl;
	}
	else if (resOpen == SF_RES_INVALID_HANDLE) {
		crashedUpdateDB(db, zErrMsg);
		cout << "Context of the handler is invalid while opening connexion with the sonar" << endl;
		// CODE ERROR : 10 -> The context of the handler is invalid
		return 10;
	}
	else if (resOpen == SF_RES_STATE_ERROR) {
		crashedUpdateDB(db, zErrMsg);
		cout << "The configuration of the sonar is invalid" << endl;
		// CODE ERROR : 4 -> Configuration of the sonar is invalid
		return 4;
	}
	else if (resOpen == SF_RES_COMMS_ERROR) {
		crashedUpdateDB(db, zErrMsg);
		cout << "Error with USB Connexion, Sonar is not powered or not plugged in to the computer" << endl;
		// CODE ERROR : 5 -> Error with USB connexion
		return 5;
	}
	else {
		crashedUpdateDB(db, zErrMsg);
		cout << "Unknown Error" << endl;
		// CODE ERROR : -2 -> Unknown Error
		return -2;
	}

	SfSetSonarRange(hdl, range);
	SfSetSonarSamples(hdl, samples);
	SfSetSonarVos(hdl, vos);

	resUpdate = SfUpdate(hdl);
	if (resUpdate == 0) {
		cout << "Configuration done" << endl;
	}
	else {
		crashedUpdateDB(db, zErrMsg);
		cout << "Configuration of the sonar failed" << endl;
		// CODE ERROR : 6 -> Error while updating sonar's configuration
		return 6;
	}

	resStart = SfStart(hdl);
	if (resStart == 0) {
		cout << "Start done" << endl;
	}
	else {
		crashedUpdateDB(db, zErrMsg);
		cout << "Sonar start failed " << resStart<< endl;
		// CODE ERROR : 7 -> Error while starting the sonar
		return 7;
	}

	//Ouverture du fichier(.csv) pour les données
	stringstream toWrite;

	//Collecte des données
	cout << "Data Collecting..." << endl;

	time_t start = time(0);

	int loopCount = 0;

	while (true) {
		if (time(0) - start > timeSeparator) {
			string filename = outputFolder + "/record-" + to_string(time(0)) + ".csv";
			cout << filename << endl;
			ofstream f(filename, ios::out, ios::binary);
			f << toWrite.str();
			f.close();
			toWrite.seekg(0, ios::end);
			cout << "Data log generated : " << filename << endl;
			toWrite.str("");
			start = time(0);
		}
		
		resRead = SfRead(hdl, pmsg);
		resCount = SfGetRxCountSonar(hdl, pcount);
		//Si des messages (data) sont en attentes
		if(count > 0) {
			nbmsg = 1;
			
			//Collecte de tous les messages 
			while(nbmsg <= msg) {
				resGetData = SfGetRxDataSonar(hdl, pdata);
				if (resGetData == SF_RES_OK) {
					if (data.Channel[0].Samples == samples)
					{
						toWrite << "P," << contrast << ",-" << gain << "," << data.Range << "," << data.Channel[0].Samples << ",";
						for (int i = 0; i < data.Channel[0].Samples; i++) {
							val = data.Channel[0].Data[i];
							toWrite << val << ",";
						}

						toWrite << "S," << contrast << ",-" << gain << "," << data.Range << "," << data.Channel[1].Samples;
						for (int i = 0; i < data.Channel[1].Samples; i++) {
							val = data.Channel[1].Data[i];
							toWrite << "," << val;
						}

						toWrite << endl;
					}
				}
				else if (resGetData == SF_RES_INVALID_HANDLE) {
					crashedUpdateDB(db, zErrMsg);
					cout << "Context of the handler is invalid while collecting data" << endl;
					// CODE ERROR : 10 -> The context of the handler is invalid
					return 10;
				}
				nbdata = nbdata + 1;
				nbmsg = nbmsg + 1;
			}
		}
		SfConfig conf;
		SfGetConfig(hdl, &conf);
		int isConnectionActive = (conf.Status & (1 << 0));
		if (isConnectionActive == 0) {
			crashedUpdateDB(db, zErrMsg);
			cout << "Error with USB Connexion, Sonar has been unplugged or unpowered" << endl;
			// CODE ERROR : 5 -> Error with USB connexion
			return 5;
		}
		loopCount++;
		Sleep(50);
	}
	resClose = SfClose(hdl);
	SfLibUnload();
	return 0;
} 
