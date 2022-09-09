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

	cam = new Camera();
	intPts = 1000.0 / cam->FREQ[program];

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
int DapController::acqui(unsigned short *memory, int16* fp_memory)
{
	cam->prepare_acqui();
	int superframe_factor = cam->get_superframe_factor();

	// Start image for cam FIRST, THEN start NI tasks
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {

		int loops = getNumPts() / superframe_factor; // superframing 

		// Start all images
		cam->start_images(ipdv, loops);
	}

	//-------------------------------------------
	// Initialize NI tasks
	//int16* tmp_fp_memory = new(std::nothrow) int16[numPts * NUM_BNC_CHANNELS];
	//memset(tmp_fp_memory, 0, numPts * NUM_BNC_CHANNELS * sizeof(int16));
	float64 samplingRate = 1000.0 / getIntPts();
	NI_fillOutputs();

	//-------------------------------------------
	// Initialize variables for camera data management
	int width = cam->width();
	int height = cam->height();
	int quadrantSize = width * height;

	int32 defaultSuccess = -1;
	int32* successfulSamples = &defaultSuccess;
	int32 defaultReadSuccess = -1;
	int32* successfulSamplesIn = &defaultReadSuccess;

	//-------------------------------------------
	// Configure NI outputs and trigger
				// config clock channel M series don't have internal clock for output.
	// clk frequency calculation: see SM's BNC_ratio, BNC_R_list, output_rate, and frame_interval
	//			output_rate = BNC_ratio*1000.0 / frame_interval;
	if (!taskHandle_clk) {
		DAQmxErrChk(DAQmxCreateTask("Clock", &taskHandle_clk));
		DAQmxErrChk(DAQmxCreateCOPulseChanTime(taskHandle_clk, "Dev1/ctr0", "",
			DAQmx_Val_Seconds, DAQmx_Val_Low, 0.00, 0.50 / getIntPts(), 0.50 / getIntPts()));
		DAQmxErrChk(DAQmxCfgImplicitTiming(taskHandle_clk, DAQmx_Val_ContSamps, get_digital_output_size()));
	}
	// Stimulator outputs (line2) and Clock for synchronizing tasks w camera (line0)
	if (!taskHandle_out) {
		DAQmxErrChk(DAQmxCreateTask("Stimulators", &taskHandle_out));

		// To write a clock to trigger camera, open line0 channel also: "Dev1/port0/line0,Dev1/port0/line2" (and see NI_fillOutputs)
		DAQmxErrChk(DAQmxCreateDOChan(taskHandle_out, "Dev1/port0/line1:2", "", DAQmx_Val_ChanForAllLines));

		// Change this to "/Dev1/PFI12" for external trigger. But for now, trigger DO tasks from camera clock (PFI0)
		DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle_out, "/Dev1/PFI0", getIntPts(),
			DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, get_digital_output_size()));
	}

	// External trigger:
	//DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(taskHandle_clk, "/Dev1/PFI1", DAQmx_Val_Rising));

	//-------------------------------------------
	// Configure NI inputs and trigger
	if (!taskHandle_in) {
		DAQmxErrChk(DAQmxCreateTask("FP Input", &taskHandle_in));
		DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle_in, "Dev1/ai0:3", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL));
		DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle_in, "/Dev1/PFI0", float64(1005.0) / getIntPts(), // sync (cam clock) to trigger input
			DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 2 * (float64)get_digital_output_size() - (float64)getAcquiOnset()));
		//DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(taskHandle_in, "/Dev1/PFI2", DAQmx_Val_Rising));
		DAQmxErrChk(DAQmxRegisterDoneEvent(taskHandle_clk, 0, DoneCallback, NULL));
	}
	//-------------------------------------------
	// Start NI tasks
	long total_written = 0, total_read = 0;

	DAQmxErrChk(DAQmxWriteDigitalU32(taskHandle_out, get_digital_output_size(), false, 0,
		DAQmx_Val_GroupByChannel, outputs, &total_written, NULL));

	DAQmxErrChk(DAQmxStartTask(taskHandle_in));
	DAQmxErrChk(DAQmxStartTask(taskHandle_out));
	DAQmxErrChk(DAQmxStartTask(taskHandle_clk));
	cout << "Total written per channel: " << total_written << "\n\t Size of output: " << get_digital_output_size() << "\n";

	/*
	DAQmxErrChk(DAQmxReadBinaryI16(taskHandle_in, numPts, 10, // timeout: 10 seconds to wait for samples?
		DAQmx_Val_GroupByScanNumber, tmp_fp_memory, 4 * numPts, successfulSamplesIn, NULL));
	*/


	//-------------------------------------------
	// Camera Acquisition loops
	//NI_openShutter(1);
	Sleep(100);
	int16* NI_ptr = fp_memory;
	omp_set_num_threads(NUM_PDV_CHANNELS);
#pragma omp parallel for	
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {

		unsigned char* image;
		int loops = getNumPts() / superframe_factor; // superframing 

		unsigned short* privateMem = memory + (ipdv * quadrantSize * getNumPts()); // pointer to this thread's section of MEMORY	
		for (int i = 0; i < loops; i++)
		{
			// acquire data for this image from the IPDVth channel	
			image = cam->wait_image(ipdv);

			// Save the image(s) to process later	
			memcpy(privateMem, image, quadrantSize * sizeof(short) * superframe_factor);
			privateMem += quadrantSize * superframe_factor; // stride to the next destination for this channel's memory	

			if (ipdv == 0) {
				long read;
				int samplesSoFar = superframe_factor * (i + 1);
				if (i == loops - 1) {
					DAQmxErrChk(DAQmxReadBinaryI16(taskHandle_in, superframe_factor * (i + 1) - total_read, 5.0,
						DAQmx_Val_GroupByScanNumber, NI_ptr, (samplesSoFar - total_read + 1) * NUM_BNC_CHANNELS, &read, NULL));
				}
				else {
					DAQmxErrChk(DAQmxReadBinaryI16(taskHandle_in, superframe_factor * (i + 1) - total_read, 0.01,
						DAQmx_Val_GroupByScanNumber, NI_ptr, (samplesSoFar - total_read) * NUM_BNC_CHANNELS, &read, NULL));
				}
				NI_ptr += read * NUM_BNC_CHANNELS;

				total_read += read;
			}
		}
		cam->end_images(ipdv);
	}
	//NI_openShutter(0);
	cout << "Total read: " << total_read << "\n";

	//=============================================================================	
	// NI clean up
	delete[] outputs;

	NI_stopTasks();
	NI_clearTasks();

	//=============================================================================	
	// Image reassembly	
	cam->reassembleImages(memory, numPts);

	//=============================================================================	
	// FP reassembly
	/*
	int16* dst_fp = fp_memory;
	for (int i_bnc = 0; i_bnc < NUM_BNC_CHANNELS; i_bnc++) {
		int16* src_fp = tmp_fp_memory + i_bnc;
		for (int m = 0; m < total_read; m++) {
			*dst_fp++ = *src_fp;
			src_fp += NUM_BNC_CHANNELS;
		}
		dst_fp += numPts - total_read; // skip to next FP trace start (nonzero iff missing read)
	}
	*/

	//delete[] tmp_fp_memory;

	cam->set_freerun_mode();

	return 0;
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
	int	sure = fl_ask("Are you sure you want to reset camera?");
	if (sure == 1) {
		for (int ipdv = 0; ipdv < 4; ipdv++) {
			cam->end_images(ipdv);
		}
		char command1[80];
		sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv0_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");	//	command sequence from Chun B 4/22/2020
		system(command1);
		sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv1_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
		system(command1);
		sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv0_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
		system(command1);
		sprintf(command1, "c:\\EDT\\pdv\\initcam -u pdv1_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
		system(command1);
		cout << " DapC resetCamera reset camera " << endl;
	}
	for (int ipdv = 0; ipdv < 4; ipdv++) {
		try {
			if (cam->open_channel(ipdv)) {
				fl_alert("DapC resetCamera Failed to open the channel!\n");
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

	// No longer needed now that numSkippedTrials is disabled
	//fillPDOut(pseudoOutputs, 1);
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
		fl_alert("DC setDuration The total duration of the acquisition can not exceed 1 min! Please adjust DAP settings.");
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
int DapController::takeRli(unsigned short *memory, Camera &cam, int rliPts)
{
	int halfwayPts = 200;

	if (rliPts < halfwayPts) {
		cout << "DapController::takeRli - Can't take less than " << halfwayPts << " RLI points \n";
		return 1;
	}

	int32       error = 0;
	TaskHandle  taskHandle = 0;
	uInt8       data[4] = { 0,1,0,0 };
	char        errBuff[2048] = { '\0' };

	uint8_t samplesForRLI[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
								0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	uInt8 data0[4] = { 0,0,0,0 };
	int32 defaultSuccess = -1; int32* successfulSamples = &defaultSuccess;

	unsigned char *image;
	int width = cam->width();
	int height = cam->height();
	int quadrantSize = width * height;	
	int array_diodes = dataArray->num_raw_array_diodes();

	int superframe_factor = cam->get_superframe_factor();
	/*
	if (width != dataArray->raw_width() || height != dataArray->raw_height() / 2)
	{
		fl_alert("Camera not set up properly. Reselect camera size & frequency settings");
		cout << " DapController line 619 - program  " << dc->getCameraProgram() << endl;
		cout << "line 620 - width & height " << width << "   " << height << endl;
		cout << "line 621 - raw values     " << dataArray->raw_width() << "   " << dataArray->raw_height() / 2 << endl;
		return 0;
	}*/

	//Sends the digital samples to port 0 line 0 (connected to LED)
	//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxwritedigitallines/
	DAQmxWriteDigitalLines(taskHandleRLI, 348, true, 0, DAQmx_Val_GroupByChannel, samplesForRLI, successfulSamples, NULL);
	
	//cam->serial_command("@SEQ 0\r\n@SEQ 1\r\n@TXC 1\r\n");

	omp_set_num_threads(NUM_PDV_CHANNELS);
	// acquire halfwayPts (200) dark frames with LED off	
	#pragma omp parallel for	
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		
		int loops = halfwayPts / superframe_factor; // superframing 

		// Start all images
		cam->start_images(ipdv, loops);

		unsigned short* privateMem = memory + (ipdv * quadrantSize * rliPts); // pointer to this thread's section of MEMORY	
		for (int i = 0; i < loops; i++)
		{
			// acquire data for this image from the IPDVth channel	
			image = cam->wait_image(ipdv);

			// Save the image(s) to process later	
			memcpy(privateMem, image, quadrantSize * sizeof(short) * superframe_factor);
			privateMem += quadrantSize * superframe_factor; // stride to the next destination for this channel's memory	
			
		}
	}

	// parallel section pauses, threads sync and close	
	NI_openShutter(1); 
	Sleep(100); 
	omp_set_num_threads(NUM_PDV_CHANNELS);
	// parallel acquisition resumes now that light is on	
	#pragma omp parallel for	
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {

		int loops = (rliPts - halfwayPts) / superframe_factor; // superframing 

		cam->start_images(ipdv, loops);

		unsigned short* privateMem = memory + (ipdv * quadrantSize * rliPts) // pointer to this thread's section of MEMORY	
						+ (quadrantSize * halfwayPts); // offset of where we left off	
			 
		for (int i = 0; i < loops; i++) 		// acquire rest of frames with LED on	
		{
			// acquire data for this image from the IPDVth channel	
			image = cam->wait_image(ipdv);

			// Save the image(s) to process later	
			memcpy(privateMem, image, quadrantSize * sizeof(short) * superframe_factor);
			privateMem += quadrantSize * superframe_factor; // stride to the next destination for this channel's memory	

		}
		cam->end_images(ipdv);
	}
	Sleep(100);
	NI_openShutter(0); // light off	
	//=============================================================================	
	// Debug: print raw images out
	unordered_map<int, std::string> framesToDebug;
	//framesToDebug[0] = "0";
	//framesToDebug[150] = "150";
	//framesToDebug[350] = "350";
	framesToDebug[355] = "355";
	framesToDebug[450] = "450";
	unsigned short* img = (unsigned short*)(memory);

	// Might not be useful to print out raw images.
	for (int i = 0; i < rliPts; i++) {
		bool debug = framesToDebug.find(i) != framesToDebug.end();
		if (debug) {
			std::string filename = "raw-full-out" + framesToDebug[i] + ".txt";
			cam->printFinishedImage(img, filename.c_str(), false);
			cout << "\t This full image was located in MEMORY at offset " <<
				(img - (unsigned short*)memory) / quadrantSize << " quadrant-sizes\n";
		}
		img += quadrantSize * NUM_PDV_CHANNELS; // stride to the full image 
	}
	

	//=============================================================================	
	// Image reassembly	
	cam->reassembleImages(memory, rliPts); // deinterleaves, CDS subtracts, and arranges quadrants	

	//=============================================================================	
	// Debug: print reassembled images out
	img = (unsigned short*)(memory);
	for (int i = 0; i < rliPts; i++) {
		bool debug = framesToDebug.find(i) != framesToDebug.end();

		if (debug) {
			std::string filename = "full-out" + framesToDebug[i] + ".txt";
			cam->printFinishedImage(img, filename.c_str(), true);
			cout << "\t This full image was located in MEMORY at offset " <<
				(img - (unsigned short*)memory) / quadrantSize << " quadrant-sizes\n";
		}
		img += quadrantSize * NUM_PDV_CHANNELS / 2; // stride to the full image (now 1/2 size due to CDS subtract)
	}
	return 0;
}

//=============================================================================
void DapController::setNumPulses(int ch, int p) {
	if (ch == 1) numPulses1 = p;
	else numPulses2 = p;
}

//=============================================================================
void DapController::setNumBursts(int ch, int num) {
	if (ch == 1) numBursts1 = num;
	else numBursts2 = num;
}

//=============================================================================
int DapController::getNumBursts(int ch) {
	if (ch == 1) return numBursts1;
	return numBursts2;
}

//=============================================================================
int DapController::getNumPulses(int ch) {
	if (ch == 1) return numPulses1;
	return numPulses2;
}

//=============================================================================
void DapController::setIntBursts(int ch, int p) {
	if (ch == 1) intBursts1 = p;
	else intBursts2 = p;
}

//=============================================================================
void DapController::setIntPulses(int ch, int p) {
	if (ch == 1) intPulses1 = p;
	else intPulses2 = p;
}

//=============================================================================
int DapController::getIntBursts(int ch) {
	if (ch == 1) return intBursts1;
	return intBursts2;
}

//=============================================================================
int DapController::getIntPulses(int ch) {
	if (ch == 1) return intPulses1;
	return intPulses2;
}

//=============================================================================
void DapController::setScheduleFlag(char p) {
	scheduleFlag = p;
}

//=============================================================================
void DapController::setScheduleRliFlag(char p) {
	scheduleRliFlag = p;
}

//=============================================================================
char DapController::getScheduleFlag() {
	return scheduleFlag;
}

//=============================================================================
char DapController::getScheduleRliFlag() {
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