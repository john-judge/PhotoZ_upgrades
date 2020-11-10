//=============================================================================
// Data.h
//=============================================================================
#ifndef Data_H
#define Data_H

#define Max_Num_Files 500

//=============================================================================
class Data
{
private:
	static double perAmp;

	//=====================================================
	// Flag
	//
	char fpFlag;		// Extracellular Recording or Not
	char ignoredFlag;

	//=====================================================
	// Memory
	//
	double *rawData;		// Raw Data
	double *proData;		// Processed Data
	double *savData;		// Saved Traces
	double *slope;

	//=====================================================
	// RLI
	//
	short rliLow;			// RLI Low
	short rliHigh;			// RLI High
	short rliMax;			// RLI Maximum

	//=====================================================
	// Properties
	//
	double rli;				// RLI Value
	double maxAmp;			// Maximal Amplitude
	double maxAmpLatency;	// The Latency of the Maximal Amplitude Point
	double halfAmpLatency;
	int maxAmpLatencyPt;

	double correctionValue;

	//=====================================================
	// Time Course
	//
	double rliArray[Max_Num_Files];			// RLI Value
	double ampArray[Max_Num_Files];			// Amplitude
	double maxAmpArray[Max_Num_Files];		// Maximal Amplitude
	double maxAmpLatencyArray[Max_Num_Files];
	double halfAmpLatencyArray[Max_Num_Files];

	//=====================================================
	// Fitting
	//
	double fittingVar[5];
	int spikeStart,spikeEnd;

	bool autoDetectSpike;

	double maxSlope;
	double spikeAmp;

public:
	//=====================================================
	// Constructor and Destructor
	//
	Data(char ecFlag);
	~Data();

	void static setPerAmp(double);

	//=====================================================
	// Memory Manipulation
	//
	void allocMem();
	void releaseMem();
	void reset();
	void resetSavData();

	void saveTraces2();

	double *getRawDataMem();
	double *getProDataMem();
	double *getSavDataMem();
	double *getSlopeMem();

	//=====================================================
	// RLI
	//
	void setRliLow(short rliLow);
	void setRliHigh(short rliHigh);
	void setRliMax(short rliMax);

	short getRliLow();
	short getRliHigh();
	short getRliMax();
	double getRli();

	//=====================================================
	// Measure Properties
	//
	void measureProperties();
	void calRli();

	double getMaxAmp();
	double getAmp(int pt);
	double getSD();

	double getMaxAmpLatency();
	double getHalfAmpLatency();

	int getMaxAmpLatencyPt();

	//=====================================================
	// Signal Processing
	//
	void ignore();
	void setIgnoredFlag(char flag);
	char getIgnoredFlag();
	void clearIgnoredFlag();
	void setCorrectionValue(double);
	double getCorrectionValue();
	void measureAmp();
	void inverseData();
	void raw2pro();
	void rliDividing();
	void temporalFiltering(double* buf);
	void ampCorrecting();

	void normalization();

	//=====================================================
	// Time Course
	//
	double getRli(int);
	double getMaxAmp(int recordIndex);
	double getMaxAmpLatency(int);
	double getHalfAmpLatency(int);

	double* getRliArray();
	double* getAmpArray();
	double* getMaxAmpArray();
	double* getMaxAmpLatencyArray();
	double* getHalfAmpLatencyArray();

	void resetTimeCourse();

	//=====================================================
	// Field Potential Flag
	//
	char getFpFlag();

	//=====================================================
	// Fitting
	//
	void setAllFittingVar(double*,bool*);
	void setFittingVar(int index,double value);
	double* getFittingVar();

	int getAlphaSpikeStart();
	int getAlphaSpikeEnd();

	void setAlphaSpikeStart(int);
	void setAlphaSpikeEnd(int);

	void setAutoDetectSpike(bool);
	double getMaxSlope();

	void initializeFittingAmp();
	void setSpikeAmp(double);
	double getSpikeAmp();
};

//=============================================================================
#endif
//=============================================================================
