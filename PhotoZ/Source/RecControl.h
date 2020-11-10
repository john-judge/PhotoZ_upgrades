//=============================================================================
// RecControl.h
//=============================================================================
#ifndef RecControl_H
#define RecControl_H
//=============================================================================
#include <time.h>

// RecControl
#define Decrease_Acqui_Gain 0
#define Increase_Acqui_Gain 1
#define Decrease_RLI_Gain 2
#define Increase_RLI_Gain 3

//=============================================================================
class RecControl  
{
private:
	short sliceNo;
	short sliceNoMax;

	short locationNo;
	short locationNoMax;

	short recordNo;
	short recordNoMax;

	short trialNo;
	short trialNoMax;

	int numTrials;
	int numSkippedTrials;
	int intTrials;

	short acquiGain;
	short rliGain;

	time_t time;

	int intRecords;
	int numRecords;

public:
	RecControl();
	~RecControl();

	void reset();

	void checkFileRange();
	char *createFileName();
	char *createFileName(short,short,short);
	char *createImageFileName();
	char *createImageFileName(short,short);

	//==============
	// File Control
	//==============
	void increaseNo(char);
	void decreaseNo(char);

	// Slice No
	void setSliceNo(short);
	short getSliceNo();
	short getSliceNoMax();

	// Location No
	void setLocationNo(short);
	short getLocationNo();
	short getLocationNoMax();

	// Record No
	void setRecordNo(short);
	short getRecordNo();
	short getRecordNoMax();

	// Trial No
	void setTrialNo(short);
	short getTrialNo();
	short getTrialNoMax();

	// Trials
	void setNumTrials(int);
	void setNumSkippedTrials(int);
	int getNumTrials();
	int getNumSkippedTrials();

	void setIntTrials(int);
	int getIntTrials();

	// Gain
	void setAcquiGain(short);
	short getAcquiGain();
	void decreaseAcquiGain();
	void increaseAcquiGain();

	void setRliGain(short);
	short getRliGain();
	void decreaseRliGain();
	void increaseRliGain();

	// Time
	void setTime(time_t);
	time_t getTime();

	void setIntRecords(int);
	int getIntRecords();

	void setNumRecords(int);
	int getNumRecords();
};

//=============================================================================
#endif
//=============================================================================
