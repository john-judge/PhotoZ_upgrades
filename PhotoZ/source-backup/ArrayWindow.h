//=============================================================================
// ArrayWindow.h
//=============================================================================
#ifndef _Array_Window_h
#define _Array_Window_h
//=============================================================================
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <vector>						// new

// Diode definitions
#define NUM_FP_DIODES 8
#define DEFAULT_ARRAY_WIDTH 256
#define DEFAULT_ARRAY_HEIGHT 40
#define NUM_BINNED_DIODES 10240

// Background for Array Window
#define BG_None 0
#define BG_Image 1

#define BG_Rli 2
#define BG_Max_Amplitude 3

#define BG_Max_Amp_Latency 4
#define BG_Half_Amp_Latency 5

#define BG_Spike_Amp 6
#define BG_EPSP_Latency 7
#define BG_SIGNAL_TO_NOISE 9

#define BG_Live_Feed 8

class Diode;
class LiveFeed;

//=============================================================================
class ArrayWindow:public Fl_Double_Window
{
private:
	//
	class Image *image;

	std::vector<Diode*> diodes;
	Diode *fp_diodes[NUM_FP_DIODES];
	void resizeDiodes();

	// Diode Selection
	std::vector<int> selectedDiodes;							//modified to use std::vector
	std::vector<std::vector<int>> selectedDiodesAverage;		//

	// Flags
	char showTrace;
	char showRliValue;
	char showDiodeNum;

	Fl_Color fg;
	Fl_Color bg;

	// Display control
	double yScale;
	double xScale;
	double xShift;
	double yScale2;

	// background
	int background;

	// Drawing methods
	void clear();
	void draw();
	void drawTrace();
	void drawRliValue();
	void drawDiodeNum();
	void drawScale();

	void drawBackground();
	void drawRli();
	void drawMaxAmp();
	void drawSpikeAmp();
	void drawSignalToNoise();
	void drawMaxAmpLatency();
	void drawHalfAmpLatency();
	void drawEPSPLatency();

	void drawTxt(char *txtBuf,Fl_Color fg,Fl_Color bg,int x,int y);

	// Helper methods
	Diode *get_diode(int);

	int numRegions = 0;
	int continuous;
	int currentRegionIndex = -1;
public:
	//-------------------------------------
	ArrayWindow(int X,int Y,int W,int H);
	~ArrayWindow();
	//-------------------------------------
	int diodeX(int);
	int diodeY(int);
	int diodeW();
	int diodeH();

	//-------------------------------------
	// Array resizing
	void changeNumDiodes();

	//-------------------------------------
	// Inherited from Fl_Double_Window
	int handle(int) override;
	void resize(int, int, int, int) override;

	//-------------------------------------
	// Flag related methods

	void setShowTrace(char);
	char getShowTrace();

	void setShowRliValue(char);
	char getShowRliValue();

	void setShowDiodeNum(char);
	char getShowDiodeNum();

	void setFgBgColor(Fl_Color foreGround,Fl_Color backGround);

	//-------------------------------------
	// Display control methods

	void setYScale(double);
	double getYScale();

	void setYScale2(double);
	double getYScale2();

	void setXScale(double);
	double getXScale();

	void setXShift(double);
	double getXShift();

	//-------------------------------------
	// Background

	void setBackground(int);
	int getBackground();
	void saveBGData();

	//-------------------------------------
	// Image

	void loadImage();
	void openImageFile(const char*);
	void setImageFileFormat(const char*);
	char* getImageFileFormat();
	void setImageXSize(int);
	void setImageYSize(int);
	void setImageX0(int);
	void setImageY0(int);
	int getImageX0();
	int getImageY0();
	int getImageXSize();
	int getImageYSize();

	//-------------------------------------
	// Diode Selection

	void addToSelectedList(int, int average = 0);
//	std::vector<int> averageSelectedDiodesROI();		//new  obsolete? see ArrayWindow.cpp 713
	int getNumRegions();								//new
	int getCurrentRegionIndex();						//new
	void setContinuous(int continuous);					//new

	int getContinuous();								//new

	int deleteFromSelectedList(int index, int average=0);
	void clearSelected(int selection);
	int getNumSelectedDiodes();
	int* getSelectedDiodes();
	int getNumSelectedDiodesAverage(int idx);			//new
	int* getSelectedDiodesAverage(int idx);				//new
	void selectDiode(int);
	void setSelectedDiodesAverageIndex(int regNo, int idx); //new for loadSelected in MainControllerArrayWindow

	//-------------------------------------
	void setColorAsDiode(int);
};

//=============================================================================
#endif
//=============================================================================
