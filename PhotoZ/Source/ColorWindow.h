//=============================================================================
// ColorWindow.h
//=============================================================================
#ifndef ColorWindow_H
#define ColorWindow_H
//=============================================================================
#include <FL/fl.h>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.h>
#include "ArrayWindow.h"
#include <vector>

#define	stripeLength 1024

#define CW_RLI 0
#define CW_Max_Amp 1
#define CW_Spike_Amp 2
#define CW_Per_Amp_Latency 3
#define CW_Max_Amp_Latency 4

void playAmpMovieCB(void *);

//=============================================================================
class GridPoint
{
public:
	float x;
	float y;
	float z;

	void setXYZ(float xx,float yy,float zz){x=xx;y=yy;z=zz;}
	void setZ(float zz){z=zz;}
};

//=============================================================================
class ColorWindow:public Fl_Gl_Window
{
public:
	enum MovieMode
	{
		AmpNor2ArrayMax,
		AmpNor2DiodeMax,
	};

private:
	char mapFlag;
	int mapMode;
	int movieMode;
	char blackBGFlag;

	char colorControlMode;	// Bounds or Center-Amplitude
	double upperBound;
	double lowerBound;
	double colorCenter;
	double colorAmp;

	// Texture Creation
	int textureIndex;
	GLuint textureHandle;
	char contourFlag;
	unsigned char stripeImage[4][stripeLength*4];
	unsigned char contourImage[4][stripeLength*4];
	void setStripe(unsigned char* stripe,int i,uchar r,unsigned char g,unsigned char b);
	void makeStripeImage();
	void makeContourImage();

	// 3D Model Creation
	GLfloat plane[4];
	std::vector<GridPoint> point;
	void build3DModel();
	void setZ_AmpNor2ArrayMax();
	void setZ_AmpNor2DiodeMax();

	// Draw
	void draw();
	void init();
	void reshape();
	void drawScale();
	void display();
	void setMapZ();
	void setMovieZ();

	//------------------------------------------
	// Movie
	int startPt;
	int endPt;
	int currentPt;
	char stopFlag;
	double speed;
	char repeatFlag;
	char makeMovieFlag;

public:
	ColorWindow(int x,int y,int w,int h,const char* label);

	void setPointXYZ();

	void changeNumDiodes();

	void setMapFlag(char);
	void setMapMode(int);
	void setMovieMode(int);

	void setZ();

	void setBlackBGFlag(char);

	// Color Scale
	void setColorControlMode(char);
	void setTextureIndex(int);
	void setContourFlag(char);

	void setColorUpperBound(double);
	void setColorLowerBound(double);
	void setColorCenter(double);
	void setColorAmp(double);

	//------------------------------------------
	// Movie
	void setMakeMovieFlag(char);
	void setStopFlag(char);
	void setRepeatFlag(char);

	void playMovie();
	void playAmpMovie();

	void increasePt();
	void resetCurrentPt();

	char getEndFlag();
	char getStopFlag();
	char getRepeatFlag();
	char getMakeMovie();

	double getSpeed();
	void setSpeed(double noFrame);

	void setCurrentPt(int no);
	void setStartPt(const char*);
	void setEndPt(const char*);

	int getCurrentPt();
	int getStartPt();
	int getEndPt();
};

//=============================================================================
#endif
//=============================================================================
