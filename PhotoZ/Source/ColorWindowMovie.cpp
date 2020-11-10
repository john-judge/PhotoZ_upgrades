//=============================================================================
// ColorWindow.cpp
//=============================================================================
#include <stdlib.h>
#include <math.h>

#include <FL/fl.h>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.h>
#include <FL/fl_draw.h>

#include "Definitions.h"
#include "ColorWindow.h"
#include "DataArray.h"
#include "UserInterface.h"
#include "WindowExporter.h"
#include "DapController.h"
#include "Color.h"

char* i2txt(int);

//=============================================================================
void playAmpMovieCB(void *)
{
	if(cw->getStopFlag())
	{
		return;
	}
	else if(cw->getEndFlag())
	{
		if(cw->getRepeatFlag())
		{
			cw->resetCurrentPt();
		}
		else
		{
			return;
		}
	}

	cw->setZ();
	cw->redraw();
	Fl::flush();
	ui->cwCurrentPtText->value(i2txt(cw->getCurrentPt()));
	ui->cwCurrentPtRoller->value(cw->getCurrentPt());

	// Movie to images
	if(cw->getMakeMovie())
	{
		we->snapshot(tw->x(),tw->y(),tw->w(),tw->h(),cw->getCurrentPt());
	}

	// Add an time out
	cw->increasePt();
	Fl::repeat_timeout(cw->getSpeed(),playAmpMovieCB);
}

//=============================================================================
void ColorWindow::playMovie()
{
	playAmpMovie();
}

//=============================================================================
void ColorWindow::playAmpMovie()
{
	if(currentPt<startPt)
	{
		currentPt=startPt;
	}
	else if(currentPt>=endPt)
	{
		currentPt=startPt;
	}

	setZ();
	redraw();
	Fl::flush();
	ui->cwCurrentPtText->value(i2txt(getCurrentPt()));
	ui->cwCurrentPtRoller->value(currentPt);

	// Movie to images
	if(makeMovieFlag)
	{
		we->snapshot(tw->x(),tw->y(),tw->w(),tw->h(),currentPt);
	}
	
	// Add an time out
	increasePt();
	Fl::add_timeout(speed,playAmpMovieCB);
}

//=============================================================================
char ColorWindow::getMakeMovie()
{
	return makeMovieFlag;
}

//=============================================================================
char ColorWindow::getEndFlag()
{
	if(currentPt>endPt)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//=============================================================================
void ColorWindow::increasePt()
{
	currentPt++;
}

//=============================================================================
void ColorWindow::setCurrentPt(int no)
{
	if(no<0)
	{
		currentPt=0;
	}
	else if(no>=dc->getNumPts())
	{
		currentPt=dc->getNumPts()-1;
	}
	else
	{
		currentPt=no;
	}

	ui->cwCurrentPtRoller->value(currentPt);
	ui->cwCurrentPtText->value(i2txt(getCurrentPt()));
}

//=============================================================================
void ColorWindow::setStartPt(const char* input)
{
	int no=atoi(input);

	if(no<0)
	{
		startPt=0;
	}
	else if(no>=dc->getNumPts())
	{
		startPt=dc->getNumPts()-1;
	}
	else
	{
		startPt=no;
	}

	ui->cwStartPt->value(i2txt(getStartPt()));
}

//=============================================================================
void ColorWindow::setEndPt(const char* input)
{
	int no=atoi(input);

	if(no<0)
	{
		endPt=0;
	}
	else if(no>=dc->getNumPts())
	{
		endPt=dc->getNumPts()-1;
	}
	else
	{
		endPt=no;
	}

	ui->cwEndPt->value(i2txt(getEndPt()));
}

//=============================================================================
void ColorWindow::resetCurrentPt()
{
	currentPt=startPt;
	ui->cwCurrentPtText->value(i2txt(getCurrentPt()));
	ui->cwCurrentPtRoller->value(currentPt);
}

//=============================================================================
void ColorWindow::setStopFlag(char flag)
{
	stopFlag=flag;
}

//=============================================================================
char ColorWindow::getStopFlag()
{
	return stopFlag;
}

//=============================================================================
double ColorWindow::getSpeed()
{
	return speed;
}

//=============================================================================
void ColorWindow::setSpeed(double noFrame)
{
	speed=1.0/noFrame;
}

//=============================================================================
int ColorWindow::getEndPt()
{
	return endPt;
}

//=============================================================================
int ColorWindow::getCurrentPt()
{
	return currentPt;
}

//=============================================================================
int ColorWindow::getStartPt()
{
	return startPt;
}

//=============================================================================
char ColorWindow::getRepeatFlag()
{
	return repeatFlag;
}

//=============================================================================
void ColorWindow::setRepeatFlag(char flag)
{
	repeatFlag=flag;
}

//=============================================================================
void ColorWindow::setMakeMovieFlag(char flag)
{
	makeMovieFlag=flag;
}

//=============================================================================
