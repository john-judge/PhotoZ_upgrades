//=============================================================================
// DapWindow.cpp: implementation of the DapWindow class.
//=============================================================================
#include <FL/fl.h>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.h>

#include "DapWindow.h"
#include "DapController.h"
#include "DapChannel.h"

//=============================================================================
DapWindow::DapWindow(int X,int Y,int W,int H):Fl_Double_Window(X,Y,W,H)
{
	bg=FL_BLACK;
}

//=============================================================================
DapWindow::~DapWindow()
{
}

//=============================================================================
void DapWindow::init(DapController *dapController)
{
	this->dapController=dapController;
}

//=============================================================================
void DapWindow::draw()
{
	fl_color(bg);
	fl_rectf(0,0,w(),h());

	int i,j;
	int x0,y0;
	int width;
	int dh=25;
	int hight=20;

	double ratio=double(w()-4)/double(dapController->getDuration());
	y0=3;

	//-------------------------------------------------------
	// Reset
	/*fl_color(FL_YELLOW);

	x0=int(ratio*dapController->reset->getOnset()+2);
	width=int(ratio*dapController->reset->getDuration()+1);

	fl_rectf(x0,y0,width,hight);*/

	//-------------------------------------------------------
	// Shutter
	fl_color(FL_GRAY);

	x0=int(ratio*dapController->shutter->getOnset()+2);
	//y0+=dh;

	width=int(ratio*dapController->shutter->getDuration()+1);

	fl_rectf(x0,y0,width,hight);

	//-------------------------------------------------------
	// Acquisition
	fl_color(FL_BLUE);

	x0=int(ratio*dapController->getAcquiOnset()+2);
	y0+=dh;
	width=int(ratio*dapController->getAcquiDuration()+1);

	fl_rectf(x0,y0,width,hight);

	//-------------------------------------------------------
	// Stimulator #1
	int interval;

	int numPulses=dapController->getNumPulses(1);
	int intPulses=dapController->getIntPulses(1);
	int numBursts=dapController->getNumBursts(1);
	int intBursts=dapController->getIntBursts(1);

	fl_color(FL_GREEN);

	x0=int(ratio*dapController->sti1->getOnset()+2);
	y0+=dh;
	width=int(ratio*dapController->sti1->getDuration()+1);

	for(j=0;j<numBursts;j++)
	{
		for(i=0;i<numPulses;i++)
		{
			interval=int(i*intPulses*ratio);
			fl_rectf(x0+interval,y0,width,hight);
		}

		x0+=(int) (intBursts*ratio);
	}

	//-------------------------------------------------------
	// Stimulator #2
	numPulses=dapController->getNumPulses(2);
	intPulses=dapController->getIntPulses(2);
	numBursts=dapController->getNumBursts(2);
	intBursts=dapController->getIntBursts(2);

	fl_color(FL_MAGENTA);

	x0=int(ratio*dapController->sti2->getOnset()+2);
	y0+=dh;
	width=int(ratio*dapController->sti2->getDuration()+1);

	for(j=0;j<numBursts;j++)
	{
		for(i=0;i<numPulses;i++)
		{
			interval=int(i*intPulses*ratio);
			fl_rectf(x0+interval,y0,width,hight);
		}

		x0+=(int) (intBursts*ratio);
	}

	//-------------------------------------------------------
	fl_color(FL_RED);
	fl_color(FL_CYAN);

}

//=============================================================================
void DapWindow::setBgColor(Fl_Color backGroundColor)
{
	bg=backGroundColor;
}

//=============================================================================
