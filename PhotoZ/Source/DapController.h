//=============================================================================
// DapController.h
//=============================================================================
#ifndef _DapController_h
#define _DapController_h
//=============================================================================
#include "dapio32.h"
#include "edtinc.h"
#include <fstream>
#include "NIDAQmx.h"

class DapChannel;
class Camera;

//=============================================================================
class DapController
{
private:
	TaskHandle taskHandleGet = 0;
	TaskHandle taskHandlePut = 0;
	HDAP dap820Put;
	HDAP dap820Get;

	TaskHandle taskHandleRLI;
	TaskHandle taskHandleAcquiDO;
	TaskHandle taskHandleAcquiAI;

	int numPts;

	int32_t error = 0;
	char errBuff[2048];

	float acquiOnset;
	double intPts;
	float duration;
	int program;

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
	DapChannel* reset;
	DapChannel* shutter;
	DapChannel* sti1;
	DapChannel* sti2;

	void NiErrorDump();

	// Set DAP and release DAP
	int setDAPs(float64 SamplingRate = 2000);//setting default for testing purposes.
	int NI_openShutter(uInt8);
	void releaseDAPs();


	// Flags
	void setStopFlag(char);
	char getStopFlag();

	void setScheduleFlag(char);
	void setScheduleRliFlag(char);
	char getScheduleFlag();
	char getScheduleRliFlag();

	// Buffers for digital output
	uint8_t *outputs;
	uint8_t *pseudoOutputs;

	// RLI
	int takeRli(unsigned short*, Camera&);

	// Create DAP File for Acquisition
	void createAcquiDapFile();
	void fillPDOut(uint8_t *outputs, char realFlag);

	// Acquisition Control
	int sendFile2Dap(const char*);
	int acqui(unsigned short*, Camera&);
	void pseudoAcqui();
	int stop();
	void resetDAPs();
	void resetCamera();

	// Acquisition Duration
	void setAcquiOnset(float);
	float getAcquiOnset();
	float getAcquiDuration();

	void setNumPts(int);
	int getNumPts();

	void setCameraProgram(int);
	int getCameraProgram();
	void setIntPts(double);
	double getIntPts();

	// Duration of the whole Process
	void setDuration();
	float getDuration();

	// Stimulator
	void setNumPulses(int ch, int num);
	int getNumPulses(int ch);
	void setIntPulses(int ch, int num);
	int getIntPulses(int ch);

	void setNumBursts(int ch, int num);
	int getNumBursts(int ch);
	void setIntBursts(int ch, int num);
	int getIntBursts(int ch);
};

//=============================================================================
#endif
//=============================================================================