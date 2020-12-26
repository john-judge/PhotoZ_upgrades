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

//#include "dapio32.h"

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
	error=0;
	char errBuff[2048] = { '\0' };
	reset=new DapChannel(0,100);
	shutter=new DapChannel(0,1210);
	sti1=new DapChannel(300,1);
	sti2=new DapChannel(300,1);

	// Acquisition
	acquiOnset=float(50);

	// Number of points per trace
	program = 7;
	numPts=2000;
	intPts=1000.0 / (double) Camera::FREQ[program];

	// Flags
	stopFlag=0;
	scheduleFlag=0;
	scheduleRliFlag=0;

	// Ch1
	numPulses1=1;
	intPulses1=10;

	numBursts1=1;
	intBursts1=200;

	// Ch2
	numPulses2=1;
	intPulses2=10;

	numBursts2=1;
	intBursts2=200;

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

void DapController::NiErrorDump(){
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		cout << errBuff;
}

//=============================================================================
// Number of Points per Trace
//=============================================================================
void DapController::setNumPts(int p)
{
	numPts=p;
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
	acquiOnset=p;
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
	return (float) (numPts*intPts);
}

//=============================================================================
void DapController::setCameraProgram(int p)
{
	program = p;
	intPts = 1000.0 / (float) Camera::FREQ[program];
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
	intPts=p;
}

//=============================================================================
double DapController::getIntPts()
{
	return intPts;
}

//=============================================================================
// Acquisition
//=============================================================================
int DapController::acqui(short *memory, Camera &cam) {

	short *buf = new short[4 * numPts];
	//DapInputFlush(dap820Get);

	unsigned char *image;
	int width = cam.width();
	int height = cam.height();
	if (width != dataArray->raw_width() || height != dataArray->raw_height()) {
		fl_alert("Camera not set up properly. Reselect camera size & frequency settings");
		cout << " line 158 width & height " << width << "   " << height << endl;
		return 0;
	}
	int num_diodes = dataArray->num_raw_diodes();

	// Start Acquisition
	//joe->dave; might need to change it for dave cam
	for (int ipdv = 0; ipdv < 4; ipdv++)
		cam.serial_write("@SEQ 0\@SEQ 1\r@TXC 1\r", ipdv);
	Sleep(100);

	cam.start_images();
	//DapLinePut(dap820Put,"START Send_Pipe_Output,Start_Output,Define_Input,Send_Data");
	//	int32 DAQmxWriteDigitalLines (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, uInt8 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxwritedigitallines/
	
	int32 defaultSuccess = -1; 
	int32* successfulSamples = &defaultSuccess;
	int32 defaultReadSuccess = -1; 
	int32* successfulSamplesIn = &defaultReadSuccess;

	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleAcqui, duration + 10, false, 0, DAQmx_Val_GroupByChannel, outputs, successfulSamples, NULL));
	int start_offset = (int)((double)(CAM_INPUT_OFFSET + acquiOnset) / intPts);
	//int32 DAQmxReadBinaryI16 (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, int16 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);	
	DAQmxErrChk(DAQmxReadBinaryI16(taskHandleAcqui, (numPts + 7 + start_offset), 0, DAQmx_Val_GroupByScanNumber, buf, 4 * numPts, successfulSamplesIn, NULL));
	DAQmxErrChk(DAQmxStartTask(taskHandleAcqui));

	// throw away first seven frames to clear camera saturation
	// be sure to add 7 to COUNT in lines 327 and 399
	for (int ii = 0; ii < 7; ii++) 
		image = cam.wait_image(ii % 4);

	#pragma omp parallel for 
	for (int ipdv = 0; ipdv < 4; ipdv++) {
		for (int i = 0; i < numPts; i++) {

			int tos = 0; // private to this thread.
			short* privateMem = memory + ipdv * numPts; // pointer to this thread's section of MEMORY

			// acquire data for this image from the IPDVth channel
			image = cam.wait_image(ipdv); 

			// Save the image to process later
			memcpy(privateMem + (num_diodes * i), image, width * height * sizeof(short));

			if (cam.num_timeouts(ipdv) != tos) {
				printf("DapController line 180 timeout on %d for thread %d\n", i, ipdv);
				tos = cam.num_timeouts(ipdv);
			}
			if (cam.num_timeouts(ipdv) > 20) {
				cam.end_images(ipdv);
				cam.serial_write("@TXC 0\r", ipdv);
				return cam.num_timeouts(ipdv);
			}
		}
	}

	cam.end_images();
	for (int ipdv = 0; ipdv < 4; ipdv++) 
		cam.serial_write("@TXC 0\r", ipdv);

	// Deinterleave memeory
	cam.deinterleave(memory);

	// Get Binary Data (digital outputs)
	//int numBytes=DapBufferGet(dap820Get,8*numPts*sizeof(short),buf);
	//DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandleAcqui),30);
	//cout << "Successful samples written: " << successfulSamples<<"\n";
	//cout << "Successful samples received: " << successfulSamplesIn<<"\n";
	//for (i = 0; i < numPts*4; i++)
	//	*(memory + (width * height) + (num_diodes*(int)(i/4)))= (short)(*(buf + i));	// copy camera buffer into memory location set aside for raw data
	return 0;
	//free(buf);
	free(outputs);
	return 0;
}


//=============================================================================
void DapController::pseudoAcqui()
{
	int32 defaultSuccess = -1; int32* successfulSamples=&defaultSuccess;
	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleAcqui, duration+10, true, 0, DAQmx_Val_GroupByChannel, pseudoOutputs, successfulSamples, NULL));
	//wait till complete
	DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandleAcqui,30));

}

//=============================================================================
void DapController::resetDAPs()
{
	//pass
	//DapLinePut(dap820Put,"RESET");
	//don't need to do anything for NI since different tasks
	//might run stop or setDaps again to confirm clean tasks
	//maybe just ensure stopped
	DAQmxErrChk(DAQmxStopTask(taskHandleAcqui));
	DAQmxErrChk(DAQmxStopTask(taskHandleRLI));
}

void DapController::resetCamera()
{
	try {
		Camera cam;
		char command1[80];
		if (cam.open_channel()) {
			fl_alert("DapC line 229 Failed to open the channel!\n");
		}
		//	if (getStopFlag() == 0) {
		int	sure = fl_ask("Are you sure you want to reset camera?");
		//	}
		if (sure == 1) {
	//		if (stop()) {
				cam.end_images();
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
				cout  << " DapC line 251 reset camera " << endl;
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
	stopFlag=1;
	//resetDAPs();
	DAQmxErrChk(DAQmxStopTask(taskHandleAcqui));
	DAQmxErrChk(DAQmxStopTask(taskHandleRLI));
	return  0;
}

//=============================================================================
int DapController::sendFile2Dap(const char *fileName820)
{
	//probably don't need this function
	// char fileName1[64]="\\PhotoZ\\";

	// strcat_s(fileName1, 64,fileName820);

	// if(!DapConfig(dap820Put,fileName1)) {
	// 	char buf[64];
	// 	DapLastErrorTextGet(buf,64);
	// 	printf("DAP ERROR: %s\n", buf);
	// 	return 0;
	// }
	// Sleep(500);		// wait .5 second for dap file to reach dap	 ?? needed
	return 1;
}

//=============================================================================
void DapController::createAcquiDapFile()//set outputs samples array here//configure tasks here
{

								//-------------------------------------------------------------------
								// Record-820 v5.dap

								// file.open("\\PhotoZ\\Record-820 v5.dap",ios::out|ios::trunc);

								// file<<";*****************************************\n";
								// file<<";* Photoz5 Author:Payne Y. Chang  (c)2005*\n";
								// file<<";*****************************************\n\n";

								// file<<";*****************************************\n";
								// file<<";* Fill Pipe_Output\n";
								// file<<";*****************************************\n";
	//outputs = new int*[3]; //old idea
	fillPDOut(outputs,1);
								// file<<"\n\n";

								// file<<";*****************************************\n";
								// file<<";* Start Output\n";
								// file<<";*****************************************\n";
								// file<<"ODEF Start_Output 3\n";
								// file<<" OUTPUTWAIT 100\n";
								// file<<" UPDATE BURST\n";
								// file<<" SET OPIPE0 B0\n";
								// file<<" SET OPIPE1 A0\n";
								// file<<" SET OPIPE2 A1\n";
								// file<<" TIME 333.33\n"; // 1000/3
								// file<<"END\n\n";

								// //-------------------------------------------------------------------
								// // Input procedures

								// // file<<";*****************************************\n";
								// // file<<";* Define Input\n";
								// // file<<";*****************************************\n";
								// // file<<"IDEF Define_Input 8\n";
								// // file<<" vrange -10 10\n";

								// // Set IPipe-S map
								// for(i=0;i<8;i++)
								// 	file << " set IPIPE" << i << " S" << i << '\n';

								// // need to translate the offset count for 1000ms interval to the current interval.
								// // full calculation is 1.0ms * CAM_INPUT_OFFSET / intPts
								// // also need to do so for acquiOnset

								// file << "\n TIME " << intPts * 125 << "\n";	// 1000/8
								// //file << " HTrigger Oneshot\n";	// might be able to sync with outputs
								// file << " COUNT " << 8 * (numPts+7 + start_offset) << "\n";			// added 5 to numPts to compensate for images skipped at beginning (line 171)
								// file << "\nEND\n\n";

	// int start_offset = (int)((double) (CAM_INPUT_OFFSET + acquiOnset) / intPts);(numPts+7+start_offset)
	//Set timing for inputs
		//DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandleAcquiIn,NULL,Camera::FREQ[program],DAQmx_Val_Rising,DAQmx_Val_Rising,8*(numPts+7+start_offset)));
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcfgsampclktiming/
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandleAcqui,NULL,Camera::FREQ[program],DAQmx_Val_Rising,DAQmx_Val_Rising, duration+10));
	// PseudoRecord-820 v5.dap
	//pseudoOutputs = new int*[3]; //old idea
	fillPDOut(pseudoOutputs,1);
								// file.open("\\PhotoZ\\PseudoRecord-820 v5.dap",ios::out|ios::trunc);

								// file<<";*****************************************\n";
								// file<<";* Photoz5 Author:Payne Y. Chang  (c)2005*\n";
								// file<<";*****************************************\n\n";

								// file<<";*****************************************\n";
								// file<<";* Fill Pipe_Output\n";
								// file<<";*****************************************\n";
								// fillPDOut(file,0);
								// file<<"\n\n";

								// file<<";*****************************************\n";
								// file<<";* Send Pipe_Output to Output Pipe\n";
								// file<<";*****************************************\n";
								// file<<"PDEF Send_Pipe_Output\n";
								// file<<" COPY(Pipe_Output,OPIPE0)\n";
								// file<<"END\n\n";

								// file<<";*****************************************\n";
								// file<<";* Start Output\n";
								// file<<";*****************************************\n";
								// file<<"ODEF Start_Output 1\n";
								// file<<" OUTPUTWAIT 10\n";
								// file<<" UPDATE BURST\n";
								// file<<" SET OPIPE0 B0\n";
								// file<<" TIME 1000\n";
								// file<<"END\n\n";
						//input definitions are not required for pseudo acqui 
								// //-------------------------------------------------------------------
								// // Input procedures

								// file<<";*****************************************\n";
								// file<<";* Define Input\n";
								// file<<";*****************************************\n";
								// file<<"IDEF Define_Input 8\n";
								// file<<" vrange -10 10\n";

								// // Set IPipe-S map
								// for(i=0;i<8;i++)
								// 	file << " set IPIPE" << i << " S" << i << '\n';

								// file << "\n TIME " << intPts * 125 << "\n";	// 1000/8
								// //file << " HTrigger Oneshot\n";	// might be able to sync with outputs
								// file << " COUNT " << 8 * (numPts+7) << "\n";
								// file << "\nEND\n\n";

								// file << ";*****************************************\n";
								// file << ";* Send Data to PC\n";
								// file << ";*****************************************\n";
								// file << "PDEF Send_Data\n";
								// file << " MERGE(IPIPE(0..7),$BINOUT)\n";
								// file << "END\n\n";

								// file << ";*****************************************\n";
								// file << ";* END\n";
								// file << ";*****************************************\n";

								// file.close();
								// // END OF PSEUDORECORD
}

//=============================================================================
void DapController::setDuration()
{
	float time;

	time=acquiOnset+int(getAcquiDuration())+1;
	duration=time;

	time=reset->getOnset()+reset->getDuration();
	if(time>duration)
		duration=time;

	time=shutter->getOnset()+shutter->getDuration();
	if(time>duration)
		duration=time;

	time=sti1->getOnset()+sti1->getDuration()+(numPulses1-1)*intPulses1+(numBursts1-1)*intBursts1;
	if(time>duration)
		duration=time;

	time=sti2->getOnset()+sti2->getDuration()+(numPulses2-1)*intPulses2+(numBursts2-1)*intBursts2;
	if(time>duration)
		duration=time;

	duration++;

	if(duration>60000)
	{
		fl_alert("DC line 451 The total duration of the acquisition can not exceed 1 min! Please adjust DAP settings.");
		return;
	}
}

float DapController::getDuration() {
	return duration;
}

//=============================================================================
void DapController::fillPDOut(uint8_t *outputs,char realFlag)
{
	int i, j, k;
	float start,end;
	// int **outputs = new int*[3];
	//don't need 60000 ints for pipe
	// outputs[0] = new int[duration+10];
	// outputs[1] = new int[duration+10];
	// outputs[2] = new int[duration+10];
	// int *pipe = outputs[0];
	// int *pipe_st1 = outputs[1];
	// int *pipe_st2 = outputs[2];
	outputs = new uint8_t[duration+10];
	const uint8_t shutter_mask = (1);		// digital out 0 based on virtual channel
	const uint8_t sti1_mask = (1 << 1);			// digital out 2
	const uint8_t sti2_mask = (1 << 2);			// digital out 3
	// const int cam_mask = (1 << 7);			// digital out 7

	//--------------------------------------------------------------
	// Reset the array
//	memset(pipe, 0, sizeof(int) * 60000);
	//don't need 60000
	memset(outputs, 0, sizeof(uint8_t) * (duration+10));
	// memset(pipe_st1, 0, sizeof(int) * duration+10);
	// memset(pipe_st2, 0, sizeof(int) * duration+10);

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
	if(realFlag) {
		start=shutter->getOnset();
		end=(start+shutter->getDuration());

		for(i=(int)start;i<end;i++)
			outputs[i] |= shutter_mask;
	}
	//--------------------------------------------------------------
	// Stimulator #1
	for(k=0;k<numBursts1;k++)
	{
		for(j=0;j<numPulses1;j++)
		{
			start=sti1->getOnset()+j*intPulses1+k*intBursts1;
			end=(start+sti1->getDuration());
			for(i=(int)start;i<end;i++)
				outputs[i] |= sti1_mask;
		}
	}
	//--------------------------------------------------------------
	// Stimulator #2
	for(k=0;k<numBursts2;k++)
	{
		for(j=0;j<numPulses2;j++)
		{
			start=sti2->getOnset()+j*intPulses2+k*intBursts2;
			end=(start+sti2->getDuration());
			for(i=(int)start;i<end;i++)
				outputs[i] |= sti2_mask;
		}
	}
			//include depending on professor meyer's reply
			// //--------------------------------------------------------------
			// // Camera Acquire
			// //for (i = acquiOnset; i < acquiOnset + getAcquiDuration() + 0.5; i++)
			// for (i = (int)acquiOnset; i < duration; i++)
			// 	pipe[i] |= cam_mask;
	
	//--------------------------------------------------------------
	// file << "PIPE Pipe_Output MAXSIZE=60000\n";
	// for (i = 0; i < duration; i++) {
	// 	if( (i%10) == 0)
	// 		file << "\n Fill Pipe_Output";
	// 	file << " " << pipe[i];
	// }
	//file << "\n Fill Pipe_Output 0 0 0 0 0 0 0 0 0 0";

			// include depending on professor meyer's reply
			// for (i = 0; i < duration; i++) {
			// 	if (pipe[i] & sti1_mask)
			// 		pipe_st1 = 32767;
			// }

			// for (i = 0; i < duration; i++) {
			// 	if (pipe[i] & sti2_mask)
			// 			pipe_st1 = 32767;
			// }

	// delete [] pipe;
	//deleted after data is sent now
}

//=============================================================================
void DapController::setStopFlag(char p)
{
	stopFlag=p;
}

//=============================================================================
char DapController::getStopFlag()
{
	return stopFlag;
}

//=============================================================================
int DapController::takeRli(short *memory, Camera &cam)
{
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	uInt8       data[4] = { 0,1,0,0 };
	char        errBuff[2048] = { '\0' };
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
		printf("DAQmx Error: %s\n", errBuff);*/			// code from Chun 

	//got from dap file
//	uInt8 samplesForRLI [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t samplesForRLI[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	uInt8 data0[4] = { 0,0,0,0 };
	int32 defaultSuccess = -1; int32* successfulSamples=&defaultSuccess;	
	int rliPts = 475;//where is this number coming from?//dafault length of samples, i think.
	unsigned char *image;
	cam.setCamProgram(dc->getCameraProgram());
	int width = cam.width();
	int height = cam.height();
	int array_diodes = dataArray->num_raw_array_diodes();
	short memory1[2560];

	if (width != dataArray->raw_width() || height != dataArray->raw_height())
	{
		fl_alert("Camera not set up properly. Reselect camera size & frequency settings");
		cout << " DapController line 619 - program  " << dc->getCameraProgram()<< endl;
		cout << "line 620 - width & height " << width << "   "  << height << endl;
		cout << "line 621 - raw values     " << dataArray->raw_width() << "   " << dataArray->raw_height() << endl;
		return 0;
	}

	//DapLinePut(dap820Put,"STA Send_Pipe_Output,Start_Output");

	//Sends the digital samples to port 0 line 0 (connected to LED)
//	int32 DAQmxWriteDigitalLines (TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, uInt8 writeArray[], int32 *sampsPerChanWritten, bool32 *reserved);
		//http://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxwritedigitallines/
	DAQmxWriteDigitalLines(taskHandleRLI, 348, true, 0, DAQmx_Val_GroupByChannel, samplesForRLI, successfulSamples, NULL);
//	DAQmxErrChk(DAQmxCreateTask("", &taskHandleRLI));															code adapted from Chun's above
//	DAQmxErrChk(DAQmxCreateDOChan(taskHandleRLI, "Dev1/port0/line0:1", "", DAQmx_Val_ChanForAllLines));
//	DAQmxErrChk(DAQmxStartTask(taskHandleRLI));
//	DAQmxWriteDigitalLines(taskHandleRLI, 1, 1, 10, DAQmx_Val_GroupByChannel, data, NULL, NULL);
//	cout << "DapController line 626 LED on \n";
//	Sleep(1000);
	//Number of samples is 348 as seen in dap file.
	//int32 DAQmxWaitUntilTaskDone (TaskHandle taskHandle, float64 timeToWait);
//    DAQmxWaitUntilTaskDone(taskHandleRLI,-1);
//can't wait because light will be off by the time camera captures



	cam.init_cam();
//	cam.serial_write("@TXC 0\r");
	/*
	if (cam.open_channel()) {
		fl_alert("DapC line 647 Failed to open the channel!\n");
	}*/
	for (int ipdv = 0; ipdv < 4; ipdv++) {
		cam.serial_write("@TXC 0\r", ipdv);
		cam.serial_write("@SEQ 1\r", ipdv);
	}
	//cam.get_image_info();
	int bufferSize = cam.get_buffer_size(0);
	cout << " bufferSize " << bufferSize << " array_diodes " << array_diodes << "\n";
	cam.start_images();
//	cout<<"\n";
	NI_openShutter(1);

	#pragma omp parallel for private(buf)
	{
		for (int ipdv = 0; ipdv < 4; ipdv++) {
			for (int i = 0; i < rliPts; i++)
			{
				image = cam.wait_image(ipdv);
				memcpy(memory + array_diodes * i, image, array_diodes);
				/*		if (i % 50 == 0)
							{
							memcpy(memory1, image, array_diodes * sizeof(image[1]));
							cout << "line 658 i " << i << " 50 " << memory1[50] << " 100 " << memory1[100] << " 500  " << memory1[500] << " \n";
							}*/
							/*		if (i == 0) cout << "line 660 i=  " << i <<" memory[100]     "<< memory[100] << " 200 " <<memory[200]<<" 1000 " << memory[1000]<<" \n";
									if (i == 1) cout << "line 660 i=  " << i << " memory[100]     " << memory[100 + array_diodes] << " 200 " << memory[200 + array_diodes] << " 1000 " << memory[1000 + array_diodes] << " \n";
									if (i % 100 == 0) cout << "line 661 i=  " << i << " memory[100+ad]  " << memory[100+array_diodes*i] << " 200 " << memory[200+array_diodes*i] << " 1000 " << memory[1000+array_diodes*i] << " \n";*/
				if (cam.num_timeouts(ipdv) > 10) {
					cout << "DapC line 663 timeouts for thread " << ipdv << " \n";
					return cam.num_timeouts(ipdv);
				}
			}
		}
	}

	cam.end_images();
	memcpy(memory, memory + array_diodes, array_diodes* sizeof(image[1]));		//*sizeof(short)
	DAQmxWriteDigitalLines(taskHandleRLI, 1, 1, 10, DAQmx_Val_GroupByChannel, data0, NULL, NULL);			//turn off LED
	NI_openShutter(0);
	return 0;
}

//=============================================================================
void DapController::setNumPulses(int ch,int p)
{
	if(ch==1)
	{
		numPulses1=p;
	}
	else
	{
		numPulses2=p;
	}
}

//=============================================================================
void DapController::setNumBursts(int ch,int num)
{
	if(ch==1)
	{
		numBursts1=num;
	}
	else
	{
		numBursts2=num;
	}
}

//=============================================================================
int DapController::getNumBursts(int ch)
{
	int num;
	if(ch==1)
	{
		num=numBursts1;
	}
	else
	{
		num=numBursts2;
	}

	return num;
}

//=============================================================================
int DapController::getNumPulses(int ch)
{
	int num;
	if(ch==1)
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
void DapController::setIntBursts(int ch,int p)
{
	if(ch==1)
	{
		intBursts1=p;
	}
	else
	{
		intBursts2=p;
	}
}

//=============================================================================
void DapController::setIntPulses(int ch,int p)
{
	if(ch==1)
	{
		intPulses1=p;
	}
	else
	{
		intPulses2=p;
	}
}

//=============================================================================
int DapController::getIntBursts(int ch)
{
	int num;

	if(ch==1)
	{
		num=intBursts1;
	}
	else
	{
		num=intBursts2;
	}

	return num;
}

//=============================================================================
int DapController::getIntPulses(int ch)
{
	int num;

	if(ch==1)
	{
		num=intPulses1;
	}
	else
	{
		num=intPulses2;
	}

	return num;
}

//=============================================================================
void DapController::setScheduleFlag(char p)
{
	scheduleFlag=p;
}

//=============================================================================
void DapController::setScheduleRliFlag(char p)
{
	scheduleRliFlag=p;
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
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandleRLI, NULL,SamplingRate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps, 348));


	DAQmxErrChk(DAQmxCreateTask("  ", &taskHandleAcqui));
			//old idea
			// DAQmxErrChk(DAQmxCreateDOChan(taskHandleAcqui, "Dev1/port0/line1", "ledOutP0L0", DAQmx_Val_ChanForAllLines));	
			// DAQmxErrChk(DAQmxCreateDOChan(taskHandleAcqui, "Dev1/port1/line1", "ledOutSt1", DAQmx_Val_ChanForAllLines));	
			// DAQmxErrChk(DAQmxCreateDOChan(taskHandleAcqui, "Dev1/port1/line3", "ledOutSt1", DAQmx_Val_ChanForAllLines));	
	
	// Commenting out the following line resolves the multiple channel types per task error. JMJ 12/6/2020
	//DAQmxErrChk(DAQmxCreateDOChan(taskHandleAcqui, "Dev1/port0/line1, Dev1/port1/line1, Dev1/port1/line3", "led_St1_St2", DAQmx_Val_ChanForAllLines));//new idea//might not work

	//int32 DAQmxCreateAOVoltageChan (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);
		//https://zone.ni.com/reference/en-XX/help/370471AM-01/daqmxcfunc/daqmxcreateaovoltagechan/
	// DAQmxErrChk(DAQmxCreateAOVoltageChan((taskHandleAcqui, "Dev1/port1/line1", "ledOutSt1", -10, 10, DAQmx_Val_Volts, NULL));
	// DAQmxErrChk(DAQmxCreateAOVoltageChan((taskHandleAcqui, "Dev1/port1/line3", "ledOutSt2", -10, 10, DAQmx_Val_Volts, NULL));
	//configure in acqui

//	DAQmxErrChk(DAQmxCreateTask("  ", &taskHandleAcquiIn));
	//1, 4, 7, and 10
	//int32 DAQmxCreateAIVoltageChan (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], int32 terminalConfig, float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);

	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcqui, "Dev1/ai0", "acquiInput0", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcqui, "Dev1/ai1", "acquiInput1", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcqui, "Dev1/ai2", "acquiInput2", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleAcqui, "Dev1/ai3", "acquiInput3", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	//TO DO: configure elsewhere
	cout << "line 815  " << SamplingRate<< "\n";
	return 0;
}

//=============================================================================
int DapController::NI_openShutter(int on)
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
	DAQmxClearTask(taskHandleAcqui);//	DapHandleClose(dap820Get); 
	DAQmxClearTask(taskHandleRLI);//	DapHandleClose(dap820Put); 
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
