//=============================================================================
// MainController.h
//=============================================================================

#ifndef _MainController_h
#define _MainController_h
//=============================================================================
#include "ArrayWindow.h"
//#include "TraceWindow.h"
#include "WindowExporter.h"

#define SLICE 0
#define LOCATION 1
#define RECORD 2
#define TRIAL 3

//=============================================================================
class MainController
{
private:
	//===========================================
	const float version;
	//===========================================
	void redraw();

public:
	//===========================================
	// Constructors
	MainController();
	//	Destructor
	~MainController();


	//===========================================
	void start();
	void quit();
	void setBgColor(char);

	// Signal processing
	void processData();
	void setDataInverseFlag(char);
	void setEcDataInverseFlag(char);
	void setRliDiv(char);
	void setCorrectionFlag(char);
	void setBLCType(char);
	void setClampPt(int pt);
	void setPolynomialStartPt(const char*);
	void setPolynomialNumPts(const char*);
	void setStartWindow(const char*);
	void setWidthWindow(const char*);
	void saveMapValues();
	void loadMapValues();
	void rli2Map();
	void setTemporalFilterFlag(char);
	void setTemporalFilterType(char);
	void setTemporalFilterWidth(int width);
	void setSpatialFilterFlag(char);
	void setSpatialFilterSigma(double sigma);
	void makeCorrection();
	void setNormalization(char);
	void setNumAveRec(int);

	void setLatencyStart(const char*);
	void setLatencyStart(double);
	void setLatencyWindow(double);

	void setRliScalar(const char*);
	void setRliScalar(double);

	// Acquisition & File Management
	void saveProcessedData();

	void takeRli();
	void acquiOneRecord();
	void record();
	void stop();
	void setScheduleFlag(char);
	void setScheduleRliFlag(char);

	void createDir();
	void openDataFile();
	void openDataFileNP();
	void saveData2File();
	void saveTraces2();
	void loadPre();
	void savePre();

	char openDataFileFromRecControl();
	void openImageFileFromRecControl();
	void setSliceNo(const char*);
	void setLocationNo(const char*);
	void setRecordNo(const char*);
	void setTrialNo(const char*);

	void increaseNo(char);
	void decreaseNo(char);

	void saveOrNot();

	void setFittingVarAllDiodes();
	void setAutoDetectSpike(bool);
	void setAlphaSpikeStart(const char*);
	void setAlphaSpikeEnd(const char*);
	void initializeFittingAmp();

	// DAP
	void setResetOnset(const char*);
	void setResetDuration(const char*);

	void setShutterOnset(const char*);
	void setShutterDuration(const char*);

	void setAcquiOnset(const char*);

	void setStiOnset(int, const char*);
	void setStiDuration(int, const char*);

	void setNumPulses(int,const char*);
	void setIntPulses(int,const char*);

	void setNumBursts(int,const char*);
	void setIntBursts(int,const char*);

	void setNumTrials(const char*);
	// void setNumSkippedTrials(const char*);
	void setIntTrials(const char*);

	void setAmplifierGain(int);
	void setNumPts(const char*);
	void setIntPts(const char*);

	void setCameraProgram(int);

	void set_digital_binning(const char*);
	void set_digital_binning(int);

	void setIntRecords(const char*);
	void setNumRecords(const char*);

	void resetCamera();

	// Array Window
	void setAwShowTrace(char);
	void setAwShowSlope(char);
	void setAwShowRliValue(char);
	void setAwShowDiodeNum(char);

	void setAwYScale(double);
	void setAwXScale(double);
	void setAwXShift(double);
	void setAwFpYScale(double);

	void setAwBackground(int);

	void loadAwImage();
	void setAwImageXSize(int);
	void setAwImageYSize(int);
	void setAwImageX0(int);
	void setAwImageY0(int);

	void saveSelected();
	void loadSelected();

	// Trace Window
	void setTwValueType(char);
	void setTwTimeCourseType(char);
	void setTwTimeCourseFlag(char);

	void saveValues();
	void saveProfileValues();

	void fitVm();
	void fitVmX10();
	void fitAllDiodes();

	// WindowExporter
	void setImageType(char);
	void exportAw();
	void exportAw_Jpeg();
	void exportTw();

	//
	void setPerAmp(double);
	void doSelection(int);
	void ignoreSelectedDiodes();

	void setContinuous(int continuous);

	// Compare
	void setRecordXNo(int,const char*);
	void setCompareFlag(char);
	void setNormalizationFlag(char);
	void setIncreaseFlag(char);
	void setAbsFlag(char);
	void setAverageFlag(char);

	void timeCourseMeasurement();

	// Color Control
	void setColorMode(char);
	void setColorControlMode(char);
	void setColorUpperBound(double);
	void setColorLowerBound(double);
	void setColorAmp(double);
	void setColorCenter(double);

	void setLiveFeed(char);

	// Do All
	void doAll();

	void saveLatestBackgroundImage();

	void sleep(double seconds);
	double *aveData;
};

//=============================================================================
#endif
//=============================================================================
