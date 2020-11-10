//=============================================================================
// DapController.h
//=============================================================================
#ifndef _DapController_h
#define _DapController_h
//=============================================================================
#include "dapio32.h"
#include <fstream>

class DapChannel;

//=============================================================================
class DapController  
{
private:
	HDAP dap3200Sys;				// DAP 3200e/214	;hdapSysPut,hdapBinGet
	HDAP dap5400Sys,dap5400Input;	// DAP 5400a/627

	int acquiOnset;
	int numPts;
	double intPts;
	int duration;

	// Ch1
	int numPulses1;
	int intPulses1;

	int numBursts1;
	int intBursts1;

	// Ch1
	int numPulses2;
	int intPulses2;

	int numBursts2;
	int intBursts2;

	// Flags
	char stopFlag;
	char ltpIndFlag;
	char scheduleFlag;
	char scheduleRliFlag;

public:
	// Constructors
	DapController();
	~DapController();

	DapChannel *reset;
	DapChannel *shutter;
	DapChannel *sti1;
	DapChannel *sti2;

	// Set DAP and release DAP
	void setDAPs();
	void releaseDAPs();
	

	// Flags
	void setStopFlag(char);
	char getStopFlag();
	
	void setScheduleFlag(char);
	void setScheduleRliFlag(char);
	char getScheduleFlag();
	char getScheduleRliFlag();

	// RLI
	void takeRli(short*);

	// Create DAP File for Acquisition
	void createAcquiDapFile();
	void fillPDOut(std::fstream &, char realFlag);

	// Acquisition Control
	int sendFile2Dap(const char *fileName3200, const char *fileName5400);
	int sendFile2Dap(const char *fileName3200);
	void acqui(short *memory);
	void pseudoAcqui();
	void stop();
	void resetDAPs();

	// Acquisition Duration
	void setAcquiOnset(int);
	int getAcquiOnset();
	double getAcquiDuration();

	void setNumPts(int);
	int getNumPts();

	void setIntPts(double);
	double getIntPts();

	char *getSamplingRateTxt();

	// Duration of the whole Process
	void setDuration();
	int getDuration();

	// Stimulator
	void setNumPulses(int ch,int num);
	int getNumPulses(int ch);
	void setIntPulses(int ch,int num);
	int getIntPulses(int ch);

	void setNumBursts(int ch,int num);
	int getNumBursts(int ch);
	void setIntBursts(int ch,int num);
	int getIntBursts(int ch);
};

//=============================================================================
#endif
//=============================================================================
