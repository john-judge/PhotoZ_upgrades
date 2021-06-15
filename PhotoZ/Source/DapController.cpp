//=============================================================================
// DapController.cpp
//=============================================================================
#include <iostream>
#include <stdlib.h>		// _gcvt()
#include <fstream>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <exception>
#include <stdint.h>
#include <FL/fl_ask.H>
#include <omp.h>
#include <unordered_map>

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
	return (float)(numPts*intPts);
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
int DapController::acqui(unsigned short *memory, Camera &cam)
{
	int i;
	short *buf = new short[4 * numPts]; // There are 4 FP analog inputs for Lil Dave
	unsigned char *image;
	int width = cam.width();
	int height = cam.height();
	int quadrantSize = width * height;
	if (width != dataArray->raw_width() || height != dataArray->raw_height())
	{
		fl_alert("Camera not set up properly. Reselect camera size & frequency settings");
		cout << " DapController::acqui() cam.width & cam.height " << width << "   " << height << endl;		return 0;
	}
	int num_diodes = dataArray->num_raw_diodes();

	// Start Acquisition
	//joe->dave; might need to change it for dave cam
	cam.serial_write("@SEQ 0\@SEQ 1\r@TXC 1\r");

  //DapLinePut(dap820Put,"START Send_Pipe_Output,Start_Output,Define_Input,Send_Data");
	//	int32 DAQmxWriteDigitalLines (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, uInt8 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxwritedigitallines/
	int32 defaultSuccess = -1; 
	int32* successfulSamples = &defaultSuccess;
	int32 defaultReadSuccess = -1; 
	int32* successfulSamplesIn = &defaultReadSuccess;

	DAQmxErrChk(DAQmxStartTask(taskHandleAcquiAI));

	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleAcquiDO, duration + 10, false, 0, DAQmx_Val_GroupByChannel, outputs, successfulSamples, NULL));
	int start_offset = (int)((double)(CAM_INPUT_OFFSET + acquiOnset) / intPts);
	//int32 DAQmxReadBinaryI16 (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, int16 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);	
	DAQmxErrChk(DAQmxReadBinaryI16(taskHandleAcquiAI, (numPts + 7 + start_offset), 0, DAQmx_Val_GroupByScanNumber, buf, 4 * numPts, successfulSamplesIn, NULL));
	DAQmxErrChk(DAQmxStartTask(taskHandleAcquiDO));
	int tos = 0;
  
  omp_set_num_threads(4);	
	#pragma omp parallel for 	
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {	
		cout << "Number of active threads: " << omp_get_num_threads() << "\n";	
		cam.start_images(ipdv);	
		int tos = 0;	
		for (int ii = 0; ii < 7; ii++) image = cam.wait_image(ipdv);		// throw away first seven frames to clear camera saturation	
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
				//return cam.num_timeouts(ipdv); Don't return from a parallel section	
			}	
		}	
		cam.end_images(ipdv);	
		if (ipdv == 0) cam.serial_write("@TXC 0\r");
  }
	free(buf);
	free(outputs);
	return 0;
}

//=============================================================================
void DapController::pseudoAcqui()
{
	int32 defaultSuccess = -1; int32* successfulSamples = &defaultSuccess;
	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleAcquiDO, duration + 10, true, 0, DAQmx_Val_GroupByChannel, pseudoOutputs, successfulSamples, NULL));
	//wait till complete
	DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandleAcquiDO, 30));

}

//=============================================================================
void DapController::resetDAPs()
{
	// just ensure stopped
	DAQmxErrChk(DAQmxStopTask(taskHandleAcquiAI));
	DAQmxErrChk(DAQmxStopTask(taskHandleAcquiDO));
	DAQmxErrChk(DAQmxStopTask(taskHandleRLI));
}

void DapController::resetCamera()
{
	for (int ipdv = 0; ipdv < 4; ipdv++) {
		try {
			Camera cam;
			char command1[80];
			if (cam.open_channel(ipdv)) {
				fl_alert("DapC line 229 Failed to open the channel!\n");
			}
			//	if (getStopFlag() == 0) {
			int	sure = fl_ask("Are you sure you want to reset camera?");
			//	}
			if (sure == 1) {
				//		if (stop()) {
				cam.end_images(ipdv);
				sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv0_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");	//	command sequence from Chun B 4/22/2020
				system(command1);
				sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv1_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
				system(command1);
				sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv0_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
				system(command1);
				sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv1_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
				system(command1);
				//				cam.init_cam();			// replaced for LittleDave
				//				int program = dc->getCameraProgram();
				//				cam.program(program);
				cout << " DapC line 251 reset camera " << endl;
				//		}
			}
		}
		catch (exception& e) {
			cout << e.what() << '\n';
		}
	}
}

//=============================================================================
int DapController::stop()
{
	stopFlag = 1;
	//resetDAPs();
	DAQmxErrChk(DAQmxStopTask(taskHandleAcquiAI));
	DAQmxErrChk(DAQmxStopTask(taskHandleAcquiDO));
	DAQmxErrChk(DAQmxStopTask(taskHandleRLI));
	return  0;
}

//=============================================================================
int DapController::sendFile2Dap(const char *fileName820)
{
	return 1;
}

//=============================================================================
void DapController::createAcquiDapFile()//set outputs samples array here//configure tasks here
{
	fillPDOut(outputs, 1);
	
  //Set timing for inputs
	//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcfgsampclktiming/
	// "" should drive both from Onboard Clock (internal)
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandleAcquiAI, "", Camera::FREQ[program], DAQmx_Val_RisingSlope, DAQmx_Val_FiniteSamps, duration + 10));
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandleAcquiDO, "", Camera::FREQ[program], DAQmx_Val_RisingSlope, DAQmx_Val_FiniteSamps, duration + 10));

	fillPDOut(pseudoOutputs, 1);
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

	time = sti1->getOnset() + sti1->getDuration() + (numPulses1 - 1)*intPulses1 + (numBursts1 - 1)*intBursts1;
	if (time > duration)
		duration = time;

	time = sti2->getOnset() + sti2->getDuration() + (numPulses2 - 1)*intPulses2 + (numBursts2 - 1)*intBursts2;
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
void DapController::fillPDOut(uint8_t *outputs, char realFlag)
{
	int i, j, k;
	float start, end;
	outputs = new uint8_t[duration + 10];
	const uint8_t shutter_mask = (1);		// digital out 0 based on virtual channel
	const uint8_t sti1_mask = (1 << 1);			// digital out 2
	const uint8_t sti2_mask = (1 << 2);			// digital out 3
	//--------------------------------------------------------------
	// Reset the array
	memset(outputs, 0, sizeof(uint8_t) * (duration + 10));
	//--------------------------------------------------------------
	// Shutter
	if (realFlag) {
		start = shutter->getOnset();
		end = (start + shutter->getDuration());
		for (i = (int)start; i < end; i++)
			outputs[i] |= shutter_mask;
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
				outputs[i] |= sti1_mask;
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
				outputs[i] |= sti2_mask;
		}
	}
	//include depending on professor meyer's reply
	// //--------------------------------------------------------------
	// // Camera Acquire
	// //for (i = acquiOnset; i < acquiOnset + getAcquiDuration() + 0.5; i++)
	// for (i = (int)acquiOnset; i < duration; i++)
	// 	pipe[i] |= cam_mask;

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
int DapController::takeRli(unsigned short *memory, Camera &cam)
{
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	uInt8       data[4] = { 0,1,0,0 };
	char        errBuff[2048] = { '\0' };

	uint8_t samplesForRLI[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	uInt8 data0[4] = { 0,0,0,0 };
	int32 defaultSuccess = -1; int32* successfulSamples = &defaultSuccess;
	int rliPts = 475; //default length of samples for RLI
	unsigned char *image;
	cam.setCamProgram(dc->getCameraProgram());
	int width = cam.width();
	int height = cam.height();
  int quadrantSize = width * height;	
	int array_diodes = dataArray->num_raw_array_diodes();
	short memory1[2560];

	if (width != dataArray->raw_width() || height != dataArray->raw_height())
	{
		fl_alert("Camera not set up properly. Reselect camera size & frequency settings");
		cout << " DapController line 619 - program  " << dc->getCameraProgram() << endl;
		cout << "line 620 - width & height " << width << "   " << height << endl;
		cout << "line 621 - raw values     " << dataArray->raw_width() << "   " << dataArray->raw_height() << endl;
		return 0;
	}

	//Sends the digital samples to port 0 line 0 (connected to LED)
	//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxwritedigitallines/
	DAQmxWriteDigitalLines(taskHandleRLI, 348, true, 0, DAQmx_Val_GroupByChannel, samplesForRLI, successfulSamples, NULL);

	// acquire 200 dark frames with LED off	
	#pragma omp parallel for	
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
		
	// parallel section pauses, threads sync and close	
	NI_openShutter(1);	
	Sleep(100);	
	omp_set_num_threads(4);	
	cout << "Number of active threads: " << omp_get_num_threads() << "\n";	
	// parallel acquisition resumes now that light is on	
	#pragma omp parallel for	
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {	
		cout << "Number of active threads: " << omp_get_num_threads() << "\n";	
		// pointer to this thread's section of MEMORY	
		short* privateMem = memory +	
			(ipdv + NUM_PDV_CHANNELS * 200) * quadrantSize; // offset of where we left off	
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
int DapController::setDAPs(float64 SamplingRate) //creates tasks
{
	DAQmxErrChk(DAQmxCreateTask("  ", &taskHandleRLI));
	//two tasks RLI and acqui
	//int32 DAQmxCreateDOChan (TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcreatedochan/
		//Channel names: http://zone.ni.com/reference/en-XX/help/370466AH-01/mxcncpts/physchannames/
	DAQmxErrChk(DAQmxCreateDOChan(taskHandleRLI, "Dev1/port0/line1", "ledOutP0L0", DAQmx_Val_ChanForAllLines));
	//Set timing.
	//int32 DAQmxCfgSampClkTiming (TaskHandle taskHandle, const char source[], float64 rate, int32 activeEdge, int32 sampleMode, uInt64 sampsPerChanToAcquire);
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcfgsampclktiming/
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandleRLI, NULL, SamplingRate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 348));


	DAQmxErrChk(DAQmxCreateTask("Acqui AI Task", &taskHandleAcquiAI));
	DAQmxErrChk(DAQmxCreateTask("Acqui DO Task", &taskHandleAcquiDO));
	
	DAQmxErrChk(DAQmxCreateDOChan(taskHandleAcquiDO, "Dev1/port0/line1", "led_St1_St2", DAQmx_Val_ChanForAllLines));

	//https://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcreateaovoltagechan/
	
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcquiAI, "Dev1/ai0", "acquiInput0", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcquiAI, "Dev1/ai1", "acquiInput1", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcquiAI, "Dev1/ai2", "acquiInput2", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcquiAI, "Dev1/ai3", "acquiInput3", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	//TO DO: configure elsewhere
	cout << "line 815  " << SamplingRate << "\n";
	return 0;
}

//=============================================================================
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
void DapController::releaseDAPs()
{
	DAQmxClearTask(taskHandleAcquiAI); 
	DAQmxClearTask(taskHandleAcquiDO);
	DAQmxClearTask(taskHandleRLI);
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