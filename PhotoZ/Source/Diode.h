//=============================================================================
// Diode.h
//=============================================================================
#ifndef _Diode_h
#define _Diode_h
//=============================================================================
#include <FL/Fl.h>
#include <FL/Fl_Widget.h>

//=============================================================================
class Diode:public Fl_Widget  
{
private:
	int index;
	int colorIndex;
	char fpFlag;

	void draw();	// empty
	void drawTxt(char*,int,int,Fl_Color);

public:
	Diode(int x,int y,int w,int h,int index);
	~Diode();

	int handle(int);
	int getIndex();
	void clearSelected();
	void setColorAsDiode();
	void select();

	// Draw
	void drawTrace(double *proData);
	void drawRliValue(double);
	void drawDiodeNum();

	// Background
	void drawHexagon();
};

//=============================================================================
#endif
//=============================================================================
