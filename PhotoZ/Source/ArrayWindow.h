//=============================================================================
// ArrayWindow.h
//=============================================================================
#ifndef _Array_Window_h
#define _Array_Window_h
//=============================================================================
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

// Diode definitions
#define Num_Diodes 472
#define Diode_Width 26
#define Diode_Height 28

// Background for Array Window
#define BG_None 0
#define BG_Image 1

#define BG_Rli 2
#define BG_Max_Amplitude 3

#define BG_Max_Amp_Latency 4
#define BG_Half_Amp_Latency 5

#define BG_Spike_Amp 6
#define BG_EPSP_Latency 7

//=============================================================================
class ArrayWindow:public Fl_Double_Window
{
private:
	//
	class Image *image;

	class Diode *diodes[Num_Diodes];
	void setDiodes();

	// Diode Selection
	int numSelectedDiodes;
	int selectedDiodes[Num_Diodes];

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
	void drawMaxAmpLatency();
	void drawHalfAmpLatency();
	void drawEPSPLatency();

	void drawTxt(char *txtBuf,Fl_Color fg,Fl_Color bg,int x,int y);

public:
	//-------------------------------------
	ArrayWindow(int X,int Y,int W,int H);
	~ArrayWindow();
	//-------------------------------------
	int diodeMap[51][27];
	int diodeX[Num_Diodes];
	int diodeY[Num_Diodes];

	//-------------------------------------
	int handle(int);

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
	void saveBGData();

	//-------------------------------------
	// Imaqge

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

	void addToSelectedList(int);
	void deleteFromSelectedList(int);
	void clearSelected();
	int getNumSelectedDiodes();
	int* getSelectedDiodes();
	void selectDiode(int);

	//-------------------------------------
	void setColorAsDiode(int);
};

//=============================================================================
#endif
//=============================================================================
