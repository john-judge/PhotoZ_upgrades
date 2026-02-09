//=============================================================================
// Diode.h
//=============================================================================
#ifndef _Diode_h
#define _Diode_h
//=============================================================================
#include <FL/Fl.h>
#include <FL/Fl_Widget.h>
#include <vector>

struct Point { int x, y; };

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
	void select(int altDown = 0, int shiftDown = 0, int crtlDown = 0, int tabDown = 0);

	// Draw
	void drawTrace(double *proData);
	void drawRliValue(double);
	void drawDiodeNum();

	std::vector<Point> dragPolygonPoints;

	// Background
	void drawBackground();
	void setColorIndex(int i);
	bool pointInPolygon(int x, int y, const std::vector<std::pair<int, int>>& pts);
};

//=============================================================================
#endif
//=============================================================================
