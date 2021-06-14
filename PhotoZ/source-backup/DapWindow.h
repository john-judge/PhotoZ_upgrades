//=============================================================================
// DapWindow.h: interface for the DapWindow class.
//=============================================================================
#ifndef _DapWindow_h
#define _DapWindow_h
//=============================================================================
#include <FL/fl.h>
#include <FL/Fl_Double_Window.H>

class DapController;

//=============================================================================
class DapWindow:public Fl_Double_Window
{
private:
	DapController *dapController;
	Fl_Color bg;
	
	void draw();

public:
	DapWindow(int X,int Y,int W,int H);
	~DapWindow();

	void init(DapController*);
	void setBgColor(Fl_Color backGroundColor);
};

//=============================================================================
#endif
//=============================================================================
