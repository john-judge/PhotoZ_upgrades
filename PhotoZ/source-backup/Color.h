//=============================================================================
// Color.h
//=============================================================================
#ifndef _Color_h
#define _Color_h
//=============================================================================
class RGB
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;

	void set(unsigned char rr,unsigned char gg,unsigned char bb);
};

//=============================================================================
class Color
{
private:
	RGB traceColor[28];
	RGB colorScale1[1276];
	RGB colorScale2[1022];
	RGB blueRedScale[1000];

	char used[28];
	unsigned char r,g,b;

	char colorMode;
	char controlMode;		// Bounds or Center-Amplitude
	double upperBound;
	double lowerBound;
	double colorCenter;
	double colorAmp;

	void setTraceColor();
	void indexExchange(int i,int j);

	void setColorScale1();
	void setColorScale2();
	void setBlueRedScale();

	void setColor1(int);
	void setColor2(int);
	void setBlueRedColor(int);


public:
	Color();
	~Color();

	int getColorIndex();			// Get Unused Color Index
	void releaseColorIndex(int);	// Release One Color
	void setTraceColor(int i);		// Set Forground Color
	void resetUsed();				// Reset All Colors to Unused

	void setColorMode(char);
	void setScaleColor(double);		// Not affected by color control

	void setColor(double);			// Modified by color control

	void setColor1(double);
	void setColor2(double);
	void setBlueRedColor(double);
	void setGrayScale(double);

	void getRGB(unsigned char*,unsigned char*,unsigned char*);

	void setColorControlMode(char);	// Bounds or Center-Amplitude
	void setColorUpperBound(double);
	void setColorLowerBound(double);
	void setColorCenter(double);
	void setColorAmp(double);
};

//=============================================================================
#endif
//=============================================================================
