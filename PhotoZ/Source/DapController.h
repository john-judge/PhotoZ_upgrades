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
	TaskHandle taskHandle_out; // Digital Output
	TaskHandle taskHandle_in; // Analog Input
	TaskHandle taskHandle_clk; // Chun: "M series don't have internal clock for output." -- X series though?
	TaskHandle taskHandle_led;

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


	// Live Feed 
	unsigned short* liveFeedFrame;
	Camera* liveFeedCam;
	bool* liveFeedFlags;

	// Ch2
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

	// NI-DAQmx
	int NI_openShutter(uInt8);

	void NI_stopTasks();
	void NI_clearTasks();

	// Flags
	void setStopFlag(char);
	char getStopFlag();

	void setScheduleFlag(char);
	void setScheduleRliFlag(char);
	char getScheduleFlag();
	char getScheduleRliFlag();

	// Buffers for digital output
	uInt32* outputs;

	// RLI
	int darkPts;
	int lightPts;
	int takeRli(unsigned short*);

	// NI Digital Output: create stimulation patterns
	void NI_fillOutputs();

	// Acquisition Control
	int acqui(unsigned short*, int16* fp_memory);
	int stop();
	void resetDAPs();
	void resetCamera();

	// Acquisition Duration
	void setAcquiOnset(float);
	float getAcquiOnset();
	float getAcquiDuration();
	size_t get_digital_output_size();

	void setNumPts(int);
	int getNumPts();

	int getCameraProgram();
	void setCameraProgram(int p);

	void setIntPts(double);
	double getIntPts();

	void setNumDarkRLI(int);
	int getNumDarkRLI();
	void setNumLightRLI(int);
	int getNumLightRLI();

	int getDisplayWidth();
	int getDisplayHeight();

	void setStimOnset(int ch, float v);
	void setShutterOnset(float v);
	float getShutterOnset();
	void setStimDuration(int ch, float v);
	float getStimOnset(int ch);
	float getStimDuration(int ch);


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

	// Live Feed
	void stopLiveFeed();
	void startLiveFeed(unsigned short* frame, bool* flags);
	void continueLiveFeed();

};

//=============================================================================
#endif
//=============================================================================