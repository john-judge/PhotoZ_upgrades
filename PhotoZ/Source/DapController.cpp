//=============================================================================
// DapController.cpp
//=============================================================================
#include <iostream>
#include <stdlib.h>		// _gcvt()
#include <fstream>
#include <time.h>
#include <string.h>
#include <unordered_map>
#include <stdio.h>
#include <exception>
#include <stdint.h>
#include <FL/fl_ask.H>
#include <omp.h> // parallelization

#include "dapio32.h"
#include "NIDAQmx.h"
#include "DapController.h"
#include "UserInterface.h"
#include "DapChannel.h"
#include "Camera.h"
#include "DataArray.h"
#include "Definitions.h"

// #pragma comment(lib,".\\lib\\NIDAQmx.lib")  Chun suggested it but turns out not to make any difference
using namespace std;

/* hacky way of synchronizing things, but it seems to work and nothing better
 * was found
 */
#define CAM_INPUT_OFFSET 10
#define DAQmxErrChk(functionCall)  if( DAQmxFailed(error=(functionCall)) ) NiErrorDump(); else

 //=============================================================================
DapController::DapController()
{
	error = 0;
	char errBuff[2048] = { '\0' };
	reset = new DapChannel(0, 100);
	shutter = new DapChannel(0, 1210);
	sti1 = new DapChannel(300, 1);
	sti2 = new DapChannel(300, 1);

	// Acquisition
	acquiOnset = float(50);

	// Number of points per trace
	program = 7;
	numPts = 2000;
	intPts = 1000.0 / (double)Camera::FREQ[program];

	// Flags
	stopFlag = 0;
	scheduleFlag = 0;
	scheduleRliFlag = 0;

	// Ch1
	numPulses1 = 1;
	intPulses1 = 10;

	numBursts1 = 1;
	intBursts1 = 200;

	// Ch2
	numPulses2 = 1;
	intPulses2 = 10;

	numBursts2 = 1;
	intBursts2 = 200;

	// Set Duration
	setDuration();
}


//=============================================================================
DapController::~DapController()
{
	delete reset;
	delete shutter;
	delete sti1;
	delete sti2;
	releaseDAPs();
}

void DapController::NiErrorDump() {
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	cout << errBuff;
}

//=============================================================================
// Number of Points per Trace
//=============================================================================
void DapController::setNumPts(int p)
{
	numPts = p;
}

//=============================================================================
int DapController::getNumPts()
{
	return numPts;
}

//=============================================================================
// Acquisition Onset
//=============================================================================
void DapController::setAcquiOnset(float p)
{
	acquiOnset = p;
}

//=============================================================================
float DapController::getAcquiOnset()
{
	return acquiOnset;
}

//=============================================================================
// Acquisition Duration
//=============================================================================
float DapController::getAcquiDuration()
{
	return (float)(numPts * intPts);
}

//=============================================================================
void DapController::setCameraProgram(int p)
{
	program = p;
	intPts = 1000.0 / (float)Camera::FREQ[program];
	//	int Frequency = Camera::FREQ[program];
}

//=============================================================================
int DapController::getCameraProgram()
{
	return program;
}

//=============================================================================
// Interval between Samples
//=============================================================================
void DapController::setIntPts(double p)
{
	intPts = p;
}

//=============================================================================
double DapController::getIntPts()
{
	return intPts;
}

//=============================================================================
// Acquisition
//=============================================================================
int DapController::acqui(short* memory, Camera& cam)
{
	int i;
	short* buf = new short[4 * numPts]; // There are 4 FP analog inputs for Lil Dave

	unsigned char* image;
	int width = cam.width();
	int height = cam.height();
	int quadrantSize = width * height;
	if (width != dataArray->raw_width() || height != dataArray->raw_height()) {
		fl_alert("Camera not set up properly. Reselect camera size & frequency settings");
		cout << " DapController::acqui() cam.width & cam.height " << width << "   " << height << endl;
		return 0;
	}
	int num_diodes = dataArray->num_raw_diodes();

	// Start Acquisition
	//joe->dave; might need to change it for dave cam
	//	cam.serial_write("@SEQ 0\@SEQ 1\r@TXC 1\r");
	//Sleep(100);
	omp_set_num_threads(4);

	#pragma omp parallel for 
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		cout << "Number of active threads: " << omp_get_num_threads() << "\n";
		cam.start_images(ipdv);

		int tos = 0;
		for (int ii = 0; ii < 7; ii++) image = cam.wait_image(0);		// throw away first seven frames to clear camera saturation
																	// be sure to add 7 to COUNT in lines 327 and 399
		for (i = 0; i < numPts; i++) {

			short* privateMem = memory + ipdv * quadrantSize; // pointer to this thread's section of MEMORY

			// acquire data for this image from the IPDVth channel
			image = cam.wait_image(ipdv);

			// Save the image to process later
			memcpy(privateMem + (quadrantSize * i), image, quadrantSize * sizeof(short));

			if (cam.num_timeouts(ipdv) != tos) {
				printf("DapController line 180 timeout on %d\n", i);
				tos = cam.num_timeouts(ipdv);
			}
			if (cam.num_timeouts(ipdv) > 20) {
				cam.end_images(ipdv);
				if(ipdv == 0) cam.serial_write("@TXC 0\r"); // only write to channel 0
				return cam.num_timeouts(ipdv);
			}
		}
		cam.end_images(ipdv);
		if (ipdv == 0) cam.serial_write("@TXC 0\r");
	}


	// Get Binary Data (digital outputs)
	/*int numBytes = DapBufferGet(dap820Get, 8 * numPts * sizeof(short), buf);
	for (i = 0; i < numPts; i++)
		memcpy(memory + (width * height) + (num_diodes * i), buf + (i * 8), 8 * sizeof(short));	// copy camera buffer into memory location set aside for raw data
	*/
	return 0;
}

//=============================================================================
void DapController::pseudoAcqui()
{
	//DapLinePut(dap820Put, "START Send_Pipe_Output,Start_Output");
}

//=============================================================================
void DapController::resetDAPs()
{
	//DapLinePut(dap820Put, "RESET");
	//don't need to do anything for NI since different tasks
	//might run stop or setDaps again to confirm clean tasks
}

void DapController::resetCamera()
{
	Camera cam;
	try {
		char command1[80];
		/*		if (cam.open_channel()) {
					fl_alert("DapC line 229 Failed to open the channel!\n");
				}*/
				//	if (getStopFlag() == 0) {
		int	sure = fl_ask("Are you sure you want to reset camera?");
		//	}
		if (sure == 1) {
			/*		cam.end_images();
					cout  << " DapC line 239 reset camera /n";
					sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv0_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");	//	command sequence from Chun B 4/22/2020
					system(command1);
					sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv1_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
					system(command1);
					sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv0_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
					system(command1);
					sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv1_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
					system(command1);*/
					//				cam.init_cam();			// replaced for LittleDave
			int program = dc->getCameraProgram();
			cam.program(program);

			//		}
		}
	}
	catch (exception& e) {
		cout << e.what() << '\n';
	}
}

//=============================================================================
int DapController::stop()
{
	stopFlag = 1;

	setDAPs();
	//resetDAPs();
	DAQmxErrChk(DAQmxStopTask(taskHandleGet));
	DAQmxErrChk(DAQmxStopTask(taskHandlePut));
	return  0;
}

//=============================================================================
int DapController::sendFile2Dap(const char* fileName820)
{
	/*
	char fileName1[64] = "\\PhotoZ\\";

	strcat_s(fileName1, 64, fileName820);
	
	if (!DapConfig(dap820Put, fileName1)) {
		char buf[64];
		DapLastErrorTextGet(buf, 64);
		printf("DAP ERROR: %s\n", buf);
		return 0;
	}
	Sleep(500);		// wait .5 second for dap file to reach dap	 ?? needed*/
	return 1;
}

//=============================================================================
void DapController::createAcquiDapFile()
{
	fstream file;
	int i;

	//-------------------------------------------------------------------
	// Record-820 v5.dap

	file.open("\\PhotoZ\\Record-820 v5.dap", ios::out | ios::trunc);

	file << ";*****************************************\n";
	file << ";* Photoz5 Author:Payne Y. Chang  (c)2005*\n";
	file << ";*****************************************\n\n";

	file << ";*****************************************\n";
	file << ";* Fill Pipe_Output\n";
	file << ";*****************************************\n";
	fillPDOut(file, 1);
	file << "\n\n";

	file << ";*****************************************\n";
	file << ";* Start Output\n";
	file << ";*****************************************\n";
	file << "ODEF Start_Output 3\n";
	file << " OUTPUTWAIT 100\n";
	file << " UPDATE BURST\n";
	file << " SET OPIPE0 B0\n";
	file << " SET OPIPE1 A0\n";
	file << " SET OPIPE2 A1\n";
	file << " TIME 333.33\n"; // 1000/3
	file << "END\n\n";

	//-------------------------------------------------------------------
	// Input procedures

	file << ";*****************************************\n";
	file << ";* Define Input\n";
	file << ";*****************************************\n";
	file << "IDEF Define_Input 8\n";
	file << " vrange -10 10\n";

	// Set IPipe-S map
	for (i = 0; i < 8; i++)
		file << " set IPIPE" << i << " S" << i << '\n';

	// need to translate the offset count for 1000ms interval to the current interval.
	// full calculation is 1.0ms * CAM_INPUT_OFFSET / intPts
	// also need to do so for acquiOnset
	int start_offset = (int)((double)(CAM_INPUT_OFFSET + acquiOnset) / intPts);
	file << "\n TIME " << intPts * 125 << "\n";	// 1000/8
	//file << " HTrigger Oneshot\n";	// might be able to sync with outputs
	file << " COUNT " << 8 * (numPts + 7 + start_offset) << "\n";			// added 5 to numPts to compensate for images skipped at beginning (line 171)
	file << "\nEND\n\n";

	file << ";*****************************************\n";
	file << ";* Send Data to PC\n";
	file << ";*****************************************\n";
	file << "PDEF Send_Data\n";
	file << " SKIP(IPIPE(0..7), " << 8 * start_offset << ", 1, 0, $BINOUT)\n";
	file << "END\n\n";

	file << ";*****************************************\n";
	file << ";* Send Pipe_Output to Output Pipe\n";
	file << ";*****************************************\n";
	file << "PDEF Send_Pipe_Output\n";
	file << " COPY(Pipe_Output,OPIPE0)\n";
	file << " COPY(PX,OPIPE1)\n";
	file << " COPY(PX1,OPIPE2)\n";
	file << "END\n\n";

	file << ";*****************************************\n";
	file << ";* END\n";
	file << ";*****************************************\n";

	file.close();

	//-------------------------------------------------------------------
	// PseudoRecord-820 v5.dap

	file.open("\\PhotoZ\\PseudoRecord-820 v5.dap", ios::out | ios::trunc);

	file << ";*****************************************\n";
	file << ";* Photoz5 Author:Payne Y. Chang  (c)2005*\n";
	file << ";*****************************************\n\n";

	file << ";*****************************************\n";
	file << ";* Fill Pipe_Output\n";
	file << ";*****************************************\n";
	fillPDOut(file, 0);
	file << "\n\n";

	file << ";*****************************************\n";
	file << ";* Send Pipe_Output to Output Pipe\n";
	file << ";*****************************************\n";
	file << "PDEF Send_Pipe_Output\n";
	file << " COPY(Pipe_Output,OPIPE0)\n";
	file << "END\n\n";

	file << ";*****************************************\n";
	file << ";* Start Output\n";
	file << ";*****************************************\n";
	file << "ODEF Start_Output 1\n";
	file << " OUTPUTWAIT 10\n";
	file << " UPDATE BURST\n";
	file << " SET OPIPE0 B0\n";
	file << " TIME 1000\n";
	file << "END\n\n";

	//-------------------------------------------------------------------
	// Input procedures

	file << ";*****************************************\n";
	file << ";* Define Input\n";
	file << ";*****************************************\n";
	file << "IDEF Define_Input 8\n";
	file << " vrange -10 10\n";

	// Set IPipe-S map
	for (i = 0; i < 8; i++)
		file << " set IPIPE" << i << " S" << i << '\n';

	file << "\n TIME " << intPts * 125 << "\n";	// 1000/8
	//file << " HTrigger Oneshot\n";	// might be able to sync with outputs
	file << " COUNT " << 8 * (numPts + 7) << "\n";
	file << "\nEND\n\n";

	file << ";*****************************************\n";
	file << ";* Send Data to PC\n";
	file << ";*****************************************\n";
	file << "PDEF Send_Data\n";
	file << " MERGE(IPIPE(0..7),$BINOUT)\n";
	file << "END\n\n";

	file << ";*****************************************\n";
	file << ";* END\n";
	file << ";*****************************************\n";

	file.close();
	// END OF PSEUDORECORD
}

//=============================================================================
void DapController::setDuration()
{
	float time;

	time = acquiOnset + int(getAcquiDuration()) + 1;
	duration = time;

	time = reset->getOnset() + reset->getDuration();
	if (time > duration)
		duration = time;

	time = shutter->getOnset() + shutter->getDuration();
	if (time > duration)
		duration = time;

	time = sti1->getOnset() + sti1->getDuration() + (numPulses1 - 1) * intPulses1 + (numBursts1 - 1) * intBursts1;
	if (time > duration)
		duration = time;

	time = sti2->getOnset() + sti2->getDuration() + (numPulses2 - 1) * intPulses2 + (numBursts2 - 1) * intBursts2;
	if (time > duration)
		duration = time;

	duration++;

	if (duration > 60000)
	{
		fl_alert("DC line 451 The total duration of the acquisition can not exceed 1 min! Please adjust DAP settings.");
		return;
	}
}

float DapController::getDuration() {
	return duration;
}

//=============================================================================
void DapController::fillPDOut(fstream& file, char realFlag)
{
	int i, j, k;
	float start, end;
	int* pipe = new int[60000];

	const int shutter_mask = (1 << 1);		// digital out 1
	const int sti1_mask = (1 << 2);			// digital out 2
	const int sti2_mask = (1 << 3);			// digital out 3
	const int cam_mask = (1 << 7);			// digital out 7

	//--------------------------------------------------------------
	// Reset the array
	memset(pipe, 0, sizeof(int) * 60000);

	//--------------------------------------------------------------
	// Reset : output[i]+=1
	/*start=reset->getOnset();
	end=start+reset->getDuration();

	for(i=start;i<end;i++)
	{
		pipe[i]+=1;
	}*/

	//--------------------------------------------------------------
	// Shutter
	if (realFlag) {
		start = shutter->getOnset();
		end = (start + shutter->getDuration());

		for (i = (int)start; i < end; i++)
			pipe[i] |= shutter_mask;
	}
	//--------------------------------------------------------------
	// Stimulator #1
	for (k = 0; k < numBursts1; k++)
	{
		for (j = 0; j < numPulses1; j++)
		{
			start = sti1->getOnset() + j * intPulses1 + k * intBursts1;
			end = (start + sti1->getDuration());
			for (i = (int)start; i < end; i++)
				pipe[i] |= sti1_mask;
		}
	}
	//--------------------------------------------------------------
	// Stimulator #2
	for (k = 0; k < numBursts2; k++)
	{
		for (j = 0; j < numPulses2; j++)
		{
			start = sti2->getOnset() + j * intPulses2 + k * intBursts2;
			end = (start + sti2->getDuration());
			for (i = (int)start; i < end; i++)
				pipe[i] |= sti2_mask;
		}
	}
	//--------------------------------------------------------------
	// Camera Acquire
	//for (i = acquiOnset; i < acquiOnset + getAcquiDuration() + 0.5; i++)
	for (i = (int)acquiOnset; i < duration; i++)
		pipe[i] |= cam_mask;

	//--------------------------------------------------------------
	file << "PIPE Pipe_Output MAXSIZE=60000\n";
	for (i = 0; i < duration; i++) {
		if ((i % 10) == 0)
			file << "\n Fill Pipe_Output";
		file << " " << pipe[i];
	}
	file << "\n Fill Pipe_Output 0 0 0 0 0 0 0 0 0 0";


	file << "\n\n\nPIPE PX MAXSIZE=60000";
	for (i = 0; i < duration; i++) {
		if ((i % 10) == 0)
			file << "\n FILL PX";
		if (pipe[i] & sti1_mask)
			file << " 32767";
		else
			file << " 0";
	}
	file << "\n Fill PX 0 0 0 0 0 0 0 0 0 0";

	file << "\n\n\nPIPE PX1 MAXSIZE=60000";
	for (i = 0; i < duration; i++) {
		if ((i % 10) == 0)
			file << "\n FILL PX1";
		if (pipe[i] & sti2_mask)
			file << " 32767";
		else
			file << " 0";
	}
	file << "\n Fill PX1 0 0 0 0 0 0 0 0 0 0";

	delete[] pipe;
}

//=============================================================================
void DapController::setStopFlag(char p)
{
	stopFlag = p;
}

//=============================================================================
char DapController::getStopFlag()
{
	return stopFlag;
}

//=============================================================================
int DapController::takeRli(short* memory, Camera& cam)
{
	/*
		DAQmxErrChk(DAQmxCreateTask("", &taskHandle));
		DAQmxErrChk(DAQmxCreateDOChan(taskHandle, "Dev1/port0/line0:1", "", DAQmx_Val_ChanForAllLines));
		DAQmxErrChk(DAQmxStartTask(taskHandle));
		DAQmxErrChk(DAQmxWriteDigitalLines(taskHandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, data, NULL, NULL));

	Error:
		if (DAQmxFailed(error))
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
		if (taskHandle != 0) {
			DAQmxStopTask(taskHandle);
			DAQmxClearTask(taskHandle);
		}
		if (DAQmxFailed(error))
			printf("DAQmx Error: %s\n", errBuff);			// code from Chun
		//got from dap file
		uInt8 samplesForRLI [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		uint8_t samplesForRLI[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		uInt8 data0[4] = { 0,0,0,0 };
		int32 defaultSuccess = -1; int32* successfulSamples=&defaultSuccess;
		*/
	int rliPts = 475;			//dark frame in 100-150 and LED on 325-375  see MainController::takeRli() in MainControllerAcqui.cpp
	unsigned char* image;
	cam.setCamProgram(dc->getCameraProgram());	//	*** necessary for changes in program - without this fl_alert is triggered	
	int width = cam.width();
	int height = cam.height();
	int quadrantSize = width * height;
	cout << " line 611 width " << width << " height " << height << " \n";
	int array_diodes = dataArray->num_raw_array_diodes();

	if (width != dataArray->raw_width() || height != dataArray->raw_height())
	{
		fl_alert("Camera not set up properly. Reselect camera size & frequency settings");
		cout << " DapController line 619 - program  " << dc->getCameraProgram() << endl;
		cout << "line 620 - width & height " << width << "   " << height << endl;
		cout << "line 621 - raw values     " << dataArray->raw_width() << "   " << dataArray->raw_height() << endl;
		return 0;
	}
	/*Sends the digital samples to port 0 line 0 (connected to LED)
		int32 DAQmxWriteDigitalLines (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, uInt8 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);
			http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxwritedigitallines/
		DAQmxWriteDigitalLines(taskHandlePut, 348, true, 0, DAQmx_Val_GroupByChannel, samplesForRLI, successfulSamples, NULL);

		bool32 isDone = false;
		while (!isDone) {
			DAQmxErrChk(DAQmxIsTaskDone(taskHandlePut, &isDone));
		}
		Sleep(100);
		cam.serial_write("@TXC 0\r");
		cam.serial_write("@SEQ 1\r");
		Sleep(100);
		cam.get_image_info();
		int bufferSize = cam.get_buffer_size();
		cout << " bufferSize " << bufferSize << " array_diodes " << array_diodes << "\n";
		image = cam.wait_image();
		short im_0 = image[0];
		short im_200 = image[200];
		cout << " array_diodes " <<array_diodes <<" image " << im_0 << "  " << im_200 << "\n";
		memcpy(memory, image, 8192);
		short mm_0 = memory[0];
		cout << "memory " << memory[0] << "  " << memory[200] << " mm_0"<< mm_0<<"\n";
		cout << "line 682 " << image[50] << "\n";
		cout << " DapC line 660 \n";*/
	//cam.init_cam();					//seems unnecessary

	/* JMJ 2/6/21 -- this shouldn't be needed since we call init_cam() from MainController
	if (cam.open_channel()) {
		fl_alert("DapC line 647 Failed to open the channel!\n");
	}
	*/
	//	cam.serial_write("@TXC 0\r");
	//	cam.serial_write("@SEQ 1\r");

	// acquire 200 dark frames with LED off
	#pragma omp parallel for
	{
		for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
			cam.start_images(ipdv);
			short* privateMem = memory + ipdv * quadrantSize; // pointer to this thread's section of MEMORY

			for (int i = 0; i < 200; i++)
			{
				// acquire data for this image from the IPDVth channel
				image = cam.wait_image(ipdv);

				// Save the image to process later
				memcpy(privateMem, image, quadrantSize * sizeof(short));
				privateMem += quadrantSize * NUM_PDV_CHANNELS; // stride to the next destination for this channel's memory
			}
		}
	}
	// parallel section pauses, threads sync and close

	NI_openShutter(1);

	Sleep(100);
	omp_set_num_threads(4);
	cout << "Number of active threads: " << omp_get_num_threads() << "\n";
	// parallel acquisition resumes now that light is on
	#pragma omp parallel for
	{
		for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
			cout << "Number of active threads: " << omp_get_num_threads() << "\n";
			// pointer to this thread's section of MEMORY
			short* privateMem = memory +
				ipdv * quadrantSize +
				quadrantSize * NUM_PDV_CHANNELS * 200; // offset of where we left off

			for (int i = 200; i < rliPts; i++) {			// acquire 275 frames with LED on
				// acquire data for this image from the IPDVth channel

				image = cam.wait_image(ipdv);

				// Save the image to process later
				memcpy(privateMem, image, quadrantSize * sizeof(short));
				privateMem += quadrantSize * NUM_PDV_CHANNELS; // stride to the next destination for this channel's memory
				/*cout << "Channel " << ipdv << " copied " << quadrantSize * sizeof(short) << " bytes to " <<
					" memory offset " << (privateMem - memory) / quadrantSize << " quadrant-sizes\n";*/
			}
			cam.end_images(ipdv);					// does not seem to matter
		}
	}
	Sleep(100);
	NI_openShutter(0); // light off



	//=============================================================================
	// Image reassembly
	unordered_map<int, std::string> framesToDebug;
	framesToDebug[300] = "300";
	framesToDebug[450] = "450";

	unsigned short* img = (unsigned short*)(memory);
	for (int i = 0; i < rliPts; i++) {

		bool debug = framesToDebug.find(i) != framesToDebug.end();

		if (debug) {
			std::string filename = "raw-full-out" + framesToDebug[i] + ".txt";
			cam.printFinishedImage(img, filename.c_str());
			cout << "\t This full image was located in MEMORY at offset " <<
				(img - (unsigned short*)memory) / quadrantSize << " quadrant-sizes\n";
		}

		cam.reassembleImage(img, false, (i == 450)); // deinterleaves, CDS subtracts, and arranges quadrants

		if (debug) {
			std::string filename = "full-out" + framesToDebug[i] + ".txt";
			cam.printFinishedImage(img, filename.c_str());
			cout << "\t This full image was located in MEMORY at offset " <<
				(img - (unsigned short*)memory) / quadrantSize << " quadrant-sizes\n";
		}

		img += quadrantSize * NUM_PDV_CHANNELS; // stride to the full image start	
	}

	//=============================================================================
	// Printing the raw data of images for debugging
	
	/*
	outFile.open("RLI-ALL.txt", std::ofstream::out | std::ofstream::trunc);
	int line = 0;
	for (int i = 200; i < rliPts-1; i++) {
		unsigned short* imgDebug = (unsigned short*)&memory [array_diodes * i];
		int w = cam.width();
		int h = cam.height();
		cam.deinterleave(imgDebug, h, w);
		cam.subtractCDS(imgDebug, h, w);
		for (int k = 0; k < array_diodes/ 2; k++) { // Is PDV's reported image height doubled for CDS subtraction?
			outFile << line << " " << imgDebug[k] << "\n";
			line++;
		}
	}
	outFile.close();


	// Save image raw data for debugging
	std::ofstream outFile;
	outFile.open("Output-300.txt", std::ofstream::out | std::ofstream::trunc);
	cam.deinterleave((unsigned short*)(memory + array_diodes * 300),cam.height(), cam.width());
	for (int k = 0; k < array_diodes; k++)
		outFile << k << " " << memory[array_diodes * 300 + k] << "\n";
	outFile.close();
	outFile.open("OutputCDS-300.txt", std::ofstream::out | std::ofstream::trunc);
	cam.subtractCDS((unsigned short*)(memory + array_diodes * 300), cam.height(), cam.width());
	for (int k = 0; k < array_diodes / 2; k++)
		outFile << k << " " << memory[array_diodes * 300 + k] << "\n";
	outFile.close();
	cout << "\nWrote 300th image's raw data to PhotoZ/: RLI-300.txt, Output-300.txt, and OutputCDS-300.txt\n";

	*/
	
	//=============================================================================

	//memcpy(memory, memory + array_diodes, array_diodes * 2);		//*sizeof(short)


	return 0;
}

//=============================================================================
void DapController::setNumPulses(int ch, int p)
{
	if (ch == 1)
	{
		numPulses1 = p;
	}
	else
	{
		numPulses2 = p;
	}
}

//=============================================================================
void DapController::setNumBursts(int ch, int num)
{
	if (ch == 1)
	{
		numBursts1 = num;
	}
	else
	{
		numBursts2 = num;
	}
}

//=============================================================================
int DapController::getNumBursts(int ch)
{
	int num;
	if (ch == 1)
	{
		num = numBursts1;
	}
	else
	{
		num = numBursts2;
	}

	return num;
}

//=============================================================================
int DapController::getNumPulses(int ch)
{
	int num;
	if (ch == 1)
	{
		return numPulses1;
	}
	else
	{
		return numPulses2;
	}

	return num;
}

//=============================================================================
void DapController::setIntBursts(int ch, int p)
{
	if (ch == 1)
	{
		intBursts1 = p;
	}
	else
	{
		intBursts2 = p;
	}
}

//=============================================================================
void DapController::setIntPulses(int ch, int p)
{
	if (ch == 1)
	{
		intPulses1 = p;
	}
	else
	{
		intPulses2 = p;
	}
}

//=============================================================================
int DapController::getIntBursts(int ch)
{
	int num;

	if (ch == 1)
	{
		num = intBursts1;
	}
	else
	{
		num = intBursts2;
	}

	return num;
}

//=============================================================================
int DapController::getIntPulses(int ch)
{
	int num;

	if (ch == 1)
	{
		num = intPulses1;
	}
	else
	{
		num = intPulses2;
	}

	return num;
}

//=============================================================================
void DapController::setScheduleFlag(char p)
{
	scheduleFlag = p;
}

//=============================================================================
void DapController::setScheduleRliFlag(char p)
{
	scheduleRliFlag = p;
}

//=============================================================================
char DapController::getScheduleFlag()
{
	return scheduleFlag;
}

//=============================================================================
char DapController::getScheduleRliFlag()
{
	return scheduleRliFlag;
}

//=============================================================================
int DapController::setDAPs(float64 SamplingRate)
{
	DAQmxErrChk(DAQmxCreateTask("  ", &taskHandleGet));
	DAQmxErrChk(DAQmxCreateTask("  ", &taskHandlePut));

	//int32 DAQmxCreateDOChan (TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcreatedochan/
		//Channel names: http://zone.ni.com/reference/en-XX/help/370466AH-01/mxcncpts/physchannames/

//	DAQmxErrChk(DAQmxCreateDOChan(taskHandlePut, "Dev1/port0/line0:1", "", DAQmx_Val_ChanForAllLines));	//			this one did not work and was changed to the line below
	DAQmxErrChk(DAQmxCreateDOChan(taskHandlePut, "Dev1/port0/line1", "ledOutP0L0", DAQmx_Val_ChanForAllLines));

	//Set timing.
	//int32 DAQmxCfgSampClkTiming (TaskHandle taskHandle, const char source[], float64 rate, int32 activeEdge, int32 sampleMode, uInt64 sampsPerChanToAcquire);
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcfgsampclktiming/
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandlePut, NULL, SamplingRate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 348));
	return 0;
}

//=============================================================================
void DapController::releaseDAPs()
{
	DAQmxClearTask(taskHandleGet);//	DapHandleClose(dap820Get); 
	DAQmxClearTask(taskHandlePut);//	DapHandleClose(dap820Put); 
}

int DapController::NI_openShutter(uInt8 on)
{
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	uInt8       data[4] = { 0,on,0,0 };
	char        errBuff[2048] = { '\0' };

	DAQmxErrChk(DAQmxCreateTask("", &taskHandle));
	DAQmxErrChk(DAQmxCreateDOChan(taskHandle, "Dev1/port0/line0:1", "", DAQmx_Val_ChanForAllLines));
	DAQmxErrChk(DAQmxStartTask(taskHandle));
	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, data, NULL, NULL));

Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
		printf("DAQmx Error: %s\n", errBuff);
	return 0;
}

//=============================================================================

//Concerns:
//Defining functions in files (like .dap files) which can send the signals to NI
//Dap820Put is used to send system commands. Figure out port equivalent to SYSin
//(or check if it's even needed as tasks can define and what needs to be done and
//  when executed will automatically send signals for niboards ports to the LED and STIMULATOR)
//Understand the code in .dap files.
//Burst mode usage

//Done (probably):
//Equivalent of dap handle to comm pipe is channel affiliated with a task
