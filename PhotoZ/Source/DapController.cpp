//=============================================================================
// DapController.cpp
//=============================================================================
#include <iostream>	// cout
#include <stdlib.h>		// _gcvt()
#include <fstream>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <FL/fl_ask.h>

#include "dapio32.h"
#include "DapController.h"
#include "UserInterface.h"
#include "DapChannel.h"

using namespace std;

//=============================================================================
DapController::DapController()
{
	reset=new DapChannel(0,100);
	shutter=new DapChannel(0,1210);
	sti1=new DapChannel(300,1);
	sti2=new DapChannel(300,1);

	// Acquisition
	acquiOnset=200;

	// Number of points per trace
	intPts=1;
	numPts=1000;

	// Flags
	stopFlag=0;
	scheduleFlag=0;
	scheduleRliFlag=0;

	// Ch1
	numPulses1=1;
	intPulses1=10;

	numBursts1=5;
	intBursts1=200;

	// Ch2
	numPulses2=1;
	intPulses2=10;

	numBursts2=5;
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
void DapController::setAcquiOnset(int p)
{
	acquiOnset=p;
}

//=============================================================================
int DapController::getAcquiOnset()
{
	return acquiOnset;
}

//=============================================================================
// Acquisition Duration
//=============================================================================
double DapController::getAcquiDuration()
{
	return numPts*intPts;
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
void DapController::acqui(short *memory)
{
	DapInputFlush(dap5400Input);

	// Start Acquisition
	DapLinePut(dap5400Sys,"START Define_Input,Send_Data");
	DapLinePut(dap3200Sys,"START Send_Pipe_Output,Start_Output");

	// Get Binary Data
	int bufSize=512*numPts*sizeof(short);
	int numBytes=DapBufferGet(dap5400Input,bufSize,memory);
}

//=============================================================================
void DapController::pseudoAcqui()
{
	DapLinePut(dap3200Sys,"START Send_Pipe_Output,Start_Output");
}

//=============================================================================
void DapController::resetDAPs()
{
	DapLinePut(dap5400Sys,"RESET");
	DapLinePut(dap3200Sys,"RESET");
}

//=============================================================================
void DapController::stop()
{
	stopFlag=1;

	setDAPs();
	resetDAPs();
	int status=DapConfig(dap3200Sys,"\\PhotoZ\\Stop v5.dap");

	if(status==0)
	{
		fl_alert("Failed to send Stop v5.dap to DAP!");
	}

	releaseDAPs();
}

//=============================================================================
int DapController::sendFile2Dap(const char *fileName3200, const char *fileName5400)
{
	char fileName1[64]="\\PhotoZ\\";
	char fileName2[64]="\\PhotoZ\\";

	strcat_s(fileName1, 64, fileName3200);
	strcat_s(fileName2, 64, fileName5400);

	if(DapConfig(dap3200Sys,fileName1) && DapConfig(dap5400Sys,fileName2))
	{	// Success
		return 1;
	}
	else
	{
		char buf[64];
		DapLastErrorTextGet(buf,64);
		return 0;
	}
}

//=============================================================================
int DapController::sendFile2Dap(const char *fileName3200)
{
	char fileName1[64]="\\PhotoZ\\";

	strcat_s(fileName1, 64,fileName3200);

	if(DapConfig(dap3200Sys,fileName1))
	{	// Success
		return 1;
	}
	else
	{
		char buf[64];
		DapLastErrorTextGet(buf,64);
		return 0;
	}
}

//=============================================================================
void DapController::createAcquiDapFile()
{
	fstream file;

	//-------------------------------------------------------------------
	// Record-3200 v5.dap

	file.open("\\PhotoZ\\Record-3200 v5.dap",ios::out|ios::trunc);

	file<<";*****************************************\n";
	file<<";* Photoz5 Author:Payne Y. Chang  (c)2005*\n";
	file<<";*****************************************\n\n";

	file<<";*****************************************\n";
	file<<";* Fill Pipe_Output\n";
	file<<";*****************************************\n";
	file<<"PIPE Pipe_Output MAXSIZE=60000\n";
	fillPDOut(file,1);
	file<<"\n\n";

	file<<";*****************************************\n";
	file<<";* Send Pipe_Output to Output Pipe\n";
	file<<";*****************************************\n";
	file<<"PDEF Send_Pipe_Output\n";
	file<<" COPY(Pipe_Output,OPIPE0)\n";
	file<<"END\n\n";

	file<<";*****************************************\n";
	file<<";* Start Output\n";
	file<<";*****************************************\n";
	file<<"ODEF Start_Output 1\n";
	file<<" OUTPUTWAIT 10\n";
	file<<" UPDATE BURST\n";
	file<<" SET OPIPE0 B0\n";
	file<<" TIME 1000\n";
	file<<"END\n\n";

	file.close();

	//-------------------------------------------------------------------
	// PseudoRecord-3200 v5.dap

	file.open("\\PhotoZ\\PseudoRecord-3200 v5.dap",ios::out|ios::trunc);

	file<<";*****************************************\n";
	file<<";* Photoz5 Author:Payne Y. Chang  (c)2005*\n";
	file<<";*****************************************\n\n";

	file<<";*****************************************\n";
	file<<";* Fill Pipe_Output\n";
	file<<";*****************************************\n";
	file<<"PIPE Pipe_Output MAXSIZE=60000\n";
	fillPDOut(file,0);
	file<<"\n\n";

	file<<";*****************************************\n";
	file<<";* Send Pipe_Output to Output Pipe\n";
	file<<";*****************************************\n";
	file<<"PDEF Send_Pipe_Output\n";
	file<<" COPY(Pipe_Output,OPIPE0)\n";
	file<<"END\n\n";

	file<<";*****************************************\n";
	file<<";* Start Output\n";
	file<<";*****************************************\n";
	file<<"ODEF Start_Output 1\n";
	file<<" OUTPUTWAIT 10\n";
	file<<" UPDATE BURST\n";
	file<<" SET OPIPE0 B0\n";
	file<<" TIME 1000\n";
	file<<"END\n\n";

	file.close();

	//-------------------------------------------------------------------
	// Record-5400 v5.dap

	file.open("\\PhotoZ\\Record-5400 v5.dap",ios::out|ios::trunc);

	file<<";*****************************************\n";
	file<<";* Photoz5 Author:Payne Y. Chang  (c)2005*\n";
	file<<";*****************************************\n\n";
	file<<"RESET\n\n";		// Stop all procedures

	file<<";*****************************************\n";
	file<<";* Define Input\n";
	file<<";*****************************************\n";
	file<<"IDEF Define_Input 64\n";
	file<<" groupsize 8\n";
	file<<" vrange -10 10\n";

	// Set IPipe-SPG map
	int i;

	for(i=0;i<64;i++)
	{
		file<<" set IP("<<i*8<<".."<<i*8+7<<") SPG"<<i<<'\n';
	}

	file<<"\n TIME "<<intPts*15.625<<"\n";	// 1000/64
	file<<" HTrigger Oneshot\n";
	file<<" COUNT "<<512*numPts<<"\n";
	file<<"\nEND\n\n";

	file<<";*****************************************\n";
	file<<";* Send Data to PC\n";
	file<<";*****************************************\n";
	file<<"PDEF Send_Data\n";
	file<<" MERGE(IPIPES(0..511),$BINOUT)\n";
	file<<"END\n\n";

	file<<";*****************************************\n";
	file<<";* END\n";
	file<<";*****************************************\n";

	file.close();

}

//=============================================================================
void DapController::setDuration()
{
	int time;
		
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
		fl_alert("The total duration of the acquisition can not exceed 1 min! Please adjust DAP settings.");
		return;
	}
}

//=============================================================================
int DapController::getDuration()
{
	return duration;
}

//=============================================================================
void DapController::fillPDOut(fstream & file,char realFlag)
{
	int i,j,k;
	int start,end;
	static int pipe[60000];

	//--------------------------------------------------------------
	// Reset the array
	for(i=0;i<duration;i++)
	{
		pipe[i]=0;
	}

	//--------------------------------------------------------------
	// Reset : output[i]+=1
	start=reset->getOnset();
	end=start+reset->getDuration();

	for(i=start;i<end;i++)
	{
		pipe[i]+=1;
	}

	//--------------------------------------------------------------
	// Shutter : output[i]+=2
	if(realFlag)
	{
		start=shutter->getOnset();
		end=start+shutter->getDuration();

		for(i=start;i<end;i++)
		{
			pipe[i]+=2;
		}
	}

	//--------------------------------------------------------------
	// Stimulator #1 : output[i]+=4
	for(k=0;k<numBursts1;k++)
	{
		for(j=0;j<numPulses1;j++)
		{
			start=sti1->getOnset()+j*intPulses1+k*intBursts1;
			end=start+sti1->getDuration();
			for(i=start;i<end;i++)
			{
				pipe[i]+=4;
			}
		}
	}

	//--------------------------------------------------------------
	// Stimulator #2 : output[i]+=8
	for(k=0;k<numBursts2;k++)
	{
		for(j=0;j<numPulses2;j++)
		{
			start=sti2->getOnset()+j*intPulses2+k*intBursts2;
			end=start+sti2->getDuration();
			for(i=start;i<end;i++)
			{
				pipe[i]+=8;
			}
		}
	}


	//--------------------------------------------------------------
	// RLI : output[i]+=16

	//--------------------------------------------------------------
	// Acquisition (Hardware Trigger) : output[i]+=32
	if(realFlag)
	{
		start=acquiOnset;
		pipe[start]+=32;
	}

	//--------------------------------------------------------------
	// Reserved : output[i]+=64
	// Reserved : output[i]+=128

	//--------------------------------------------------------------
	for(i=0;i<duration;i++)
	{
		if((i%10)==0)
			file<<"\n Fill Pipe_Output";

		file<<" "<<pipe[i];
	}
}

//=============================================================================
char *DapController::getSamplingRateTxt()
{
	static char buf[32];
	double rate=1/intPts;

	sprintf_s(buf, 32, "%5.3f", rate);
	strcat_s(buf, 32," (kHz)");

	return buf;
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
void DapController::takeRli(short *memory)
{
	int status=DapConfig(dap3200Sys,"\\PhotoZ\\RLI-3200 v5.dap");

	if(status==0)
	{
		fl_alert("Failed to send RLI-3200 v5.dap to DAP!");
		return;
	}

	status=DapConfig(dap5400Sys,"\\PhotoZ\\RLI-5400 v5.dap");

	if(status==0)
	{
		fl_alert("Failed to send RLI-5400 v5.dap to DAP!");
		return;
	}

	//------------------------------------
	// Flush Buffer in DAP
	DapInputFlush(dap5400Input);

	// Start Acquisition
	DapLinePut(dap3200Sys,"STA Send_Pipe_Output,Start_Output");
	DapLinePut(dap5400Sys,"STA Define_Input,Send_Data\r");

	// Get Binary Data
	int bufSize=512*350*sizeof(short);
	int numBytes=DapBufferGet(dap5400Input,bufSize,memory);
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
void DapController::setDAPs()
{
	dap3200Sys=DapHandleOpen("\\\\.\\Dap1\\$SysIn", DAPOPEN_WRITE);
	dap5400Sys=DapHandleOpen("\\\\.\\Dap0\\$SysIn", DAPOPEN_WRITE);
	dap5400Input=DapHandleOpen("\\\\.\\Dap0\\$BinOut", DAPOPEN_READ);
}

//=============================================================================
void DapController::releaseDAPs()
{
	DapHandleClose(dap3200Sys);
	DapHandleClose(dap5400Sys);
	DapHandleClose(dap5400Input);
}

//=============================================================================
