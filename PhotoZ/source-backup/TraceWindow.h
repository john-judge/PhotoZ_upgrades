//=============================================================================
// TraceWindow.h
//=============================================================================
#ifndef _TraceWindow_h
#define _TraceWindow_h
//=============================================================================
#include <FL/Fl.h>
#include <FL/Fl_Double_Window.H>

//=============================================================================
class TraceWindow:public Fl_Double_Window
{
private:
	//=================
	// Status
	//=================
	char valueType;
	char timeCourseType;
	char timeCourseFlag;
	char showPointLine;
	char showFileLine;
	char superimpose;
	char showTrace;
	char showRef;
	char showSavedTraces;
	char showSlope;
	char timeCourseNormalizationFlag;
	char save2FileType;

	char handleType;

	//=================
	// Save to File
	//=================
	int saveTracesStartPt;
	int saveTracesEndPt;

	//=================
	// Alpha function control
	//=================
	bool calWholeVm;
	char showVm;
	char showAlpha;

	bool changeBits[5];

	double spikeAmp;
	double* alpha;
	double* Vm;

	double spikeSlopeThreshold;
	double spikeAmpThreshold;

	float onsetBounds[2];

	//=================
	// Display control
	//=================
	double yScale;
	double fpYScale;
	double xScale;
	double xShift;
	double ltpXScale;
	double ltpXShift;

	int pointLinePt;

	Fl_Color fg;
	Fl_Color bg;

	//===================
	// Drawing methods
	//===================
	void clear();
	void draw();
	/*void draw(int average=0);*/
	void drawTrace(int index, int currentRegion=-1);
	void drawSlope(int index,int region=-1);
	void drawVm(int index);
	void drawTimeCourse(int index, int region);
	void drawRef(int index);
	void drawValue(int numRegions=0);
	void drawScale();
	void drawClampLine();
	void drawPointLine();
	void drawFileLine();
	
	void drawTxt(char *txtBuf,Fl_Color fg,Fl_Color bg,int x,int y);

public:
	// Constructors
	TraceWindow(int X,int Y,int W,int H);
	~TraceWindow();

	int handle(int);
	void setHandleType(char);

	// Value Type
	void setValueType(char);
	char getValueType();
	void setTimeCourseType(char);
	void setTimeCourseFlag(char);

	// Display control
	void setPointLinePt(int pt);
	int getPointLinePt();

	// Flag related methods
	void setSuperimpose(char);

	void setShowTrace(char);
	char getShowTrace();

	void setShowSlope(char);
	
	void setShowRef(char);
	char getShowRef();

	void setShowSavedTraces(char);
	char getShowSavedTraces();

	void setShowPointLine(char);
	char getShowPointLine();

	void setShowFileLine(char);
	char getShowFileLine();

	void setTimeCourseNormalizationFlag(char flag);

	void setSave2FileType(char type);

	// Color
	void setFgBgColor(Fl_Color foreGround,Fl_Color backGround);

	// Save Data
	void saveTraces();
	void setSaveTracesPt(char tag,const char* ptTxt);
	void saveTimeCourse();

	//----------------------------
	// Alpha function control
	void setChangeBits(int index,bool);
	bool getChangeBits(int index);

	void setShowAlpha(char);
	void setShowVm(char);

	void setFittingVar(int,double);

	double convertFittingVar(int,double);
	double revConvertFittingVar(int,double);

	double calFitError(int dataNo);
	void calVm(int dataNo);
	double fitVm(int dataNo);
	void saveAlphaVm();
	void saveEPSP();
	int getLastDiodeNo();
	void setFittingGadgets(int dataNo);
	void setFittingMemory();
	void setCalWholeVm(bool);

	void setSpikeSlopeThreshold(double);
	void setSpikeAmpThreshold(double);

	void saveFittingVar();
	void loadFittingVar();

	void setOnsetBounds(int index,const char* txt);
	double *aveData;
};

//=============================================================================
#endif
//=============================================================================
