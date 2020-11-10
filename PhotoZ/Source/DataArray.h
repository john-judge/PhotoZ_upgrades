//=============================================================================
// DataArray.h
//=============================================================================
#ifndef DataArray_H
#define DataArray_H
//=============================================================================
#include "ArrayWindow.h"

#define Max_Trials 100

//=============================================================================
class DataArray  
{
private:
	//-----------------------------------------------------
	// 10 maximal number of trials
	//
	short *trialData[Num_Diodes][Max_Trials];	// Memory for raw data
	class Data *data[Num_Diodes];

	int numTrials;
	int numPts;
	int numAveRec;

	double maxRli;

	// Compare
	char compareFlag;
	char nor2ArrayMaxFlag;
	char increaseFlag;
	char absFlag;
	char averageFlag;
	char normalizationFlag;

	int record1No;
	int record2No;

	double dataFeature[Num_Diodes - 8];
	void resetDataFeature();

	//-----------------------------------------------------
	void allocMem();
	void releaseMem();

	// Latency
	double latencyStart;
	double latencyWindow;

public:
	//-----------------------------------------------------
	DataArray(int numPts);
	~DataArray();

	// Memory Manipulation
	void changeMemSize(int numTrials,int numPts);
	void changeNumTrials(int numTrials);
	void changeNumPts(int numPts);

	// Data Manipulation
	void loadTrialData(int trialNo);
	void average();
	void resetData();

 	void arrangeData(int,short*);

	// RLI
	void calRli();
	void setMaxRli();

	void setRliLow(short* dataBlock);
	void setRliHigh(short* dataBlock);
	void setRliMax(short* dataBlock);

	void setRliLow(int diodeNo,short rli);
	void setRliHigh(int diodeNo,short rli);
	void setRliMax(int diodeNo,short rli);

	// Signal Processing
	void process();
	void setCorrectionValue(int dataIndex,double value);
	double getCorrectionValue(int dataIndex);
	void makeCorrection();
	void measureAmp();
	void setNormalizationFlag(char);
	void setAverageFlag(char);
	void setNumAveRec(int);
	char getAverageFlag();
	int getNumAveRec();
	void ampCorrecting();

	// Ignorance
	void doSelection(int);
	void clearIgnoredFlag();
	char getIgnoredFlag(int dataNo);
	void setIgnoredFlag(int dataNo,char flag);
	void ignore(int dataNo);

	//
	void dataInversing();
	void rliDividing();
	void temporalFiltering();

	// Trace 2
	void saveTraces2();

	class Data* getData(int);
	short *getMem(int trialNo,int diodeNo);
	double *getRawDataMem(int diodeNo);
	double *getProDataMem(int diodeNo);
	double *getSavDataMem(int diodeNo);
	double *getSlopeMem(int diodeNo);

	// RLI Processing
	short getRliLow(int diodeNo);
	short getRliHigh(int diodeNo);
	short getRliMax(int diodeNo);
	double getRli(int diodeNo);
	double getRliRatio(int diodeNo);

	// Get Diode Properties
	char getFpFlag(int dataNo);
	double getAmp(int diodeNo,int ptNo);
	double getDataSD(int dataNo);
	double getDataMaxAmpLatency(int dataNo);
	double getDataHalfAmpLatency(int dataNo);
	int getDataMaxAmpLatencyPt(int dataNo);

	double getDataMaxAmp(int diodeNo,int recordNoChoice=0);
	double getDiodeMaxAmpSD(int diodeIndex,int recordNoChoice=0);
	double getDiodeMaxAmpCha(int diodeIndex,int choice=0);
	double getDiodeMaxAmpChaSD(int diodeIndex);
	double getDiodeMaxAmpPerCha(int diodeIndex);
	double getDiodeMaxAmpPerChaSD(int diodeIndex);

	// Compare
	void setRecordXNo(int recordX,int recordXNo);
	int getRecordXNo(int recordX);
	void setCompareFlag(char);
	char getCompareFlag();
	void setNor2ArrayMaxFlag(char);
	void setIncreaseFlag(char);
	char getIncreaseFlag();
	void setAbsFlag(char);
	char getAbsFlag();

	void setRli2DataFeature();
	void setMaxAmp2DataFeature();
	void setSpikeAmp2DataFeature();
	void setMaxAmpLatency2DataFeature();
	void setHalfAmpLatency2DataFeature();
	void setHalfAmpLatencyRatio2DataFeature();
	void setEPSPLatency2DataFeature();

	double getDataFeature(int);

	// Latency
	void setLatencyStart(double);
	double getLatencyStart();
	void setLatencyWindow(double);
};

//=============================================================================
#endif
//=============================================================================
