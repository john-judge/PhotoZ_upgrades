//=============================================================================
// ArrayWindow.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()
#include <string.h>		// strcat()
#include <math.h>
#include <fstream>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.h>
#include <FL/fl_ask.h>
#include <FL/fl_file_chooser.h>

#include "ArrayWindow.h"
#include "UserInterface.h"
#include "Diode.h"
#include "Image.h"
#include "Color.h"
#include "DAPController.h"
#include "DataArray.h"
#include "SignalProcessor.h"
#include "Definitions.h"

extern char txtBuf[];

//=============================================================================
ArrayWindow::ArrayWindow(int X,int Y,int W,int H)
:Fl_Double_Window(X,Y,W,H)
{
	int i,j;
	for(i=0;i<51;i++)
	{
		for(j=0;j<27;j++)
		{
			diodeMap[i][j]=-1;
		}
	}

	image=new Image();
	setDiodes();

	fg=FL_WHITE;
	bg=FL_BLACK;
	background=BG_None;
	showTrace=1;
	showRliValue=0;
	showDiodeNum=0;

	yScale=100;
	yScale2=100;
	xScale=1;
	xShift=0;
}

//=============================================================================
ArrayWindow::~ArrayWindow()
{
	int i;

	delete image;

	for(i=0;i<Num_Diodes;i++)
	{
		delete diodes[i];
	}
}

//=============================================================================
void ArrayWindow::setShowTrace(char p)
{
	showTrace=p;
}

//=============================================================================
char ArrayWindow::getShowTrace()
{
	return showTrace;
}

//=============================================================================
void ArrayWindow::setShowRliValue(char p)
{
	showRliValue=p;
}

//=============================================================================
char ArrayWindow::getShowRliValue()
{
	return showRliValue;
}

//=============================================================================
void ArrayWindow::setShowDiodeNum(char p)
{
	showDiodeNum=p;
}

//=============================================================================
char ArrayWindow::getShowDiodeNum()
{
	return showDiodeNum;
}

//=============================================================================
void ArrayWindow::setFgBgColor(Fl_Color c1,Fl_Color c2)
{
	fg=c1;
	bg=c2;
}

//=============================================================================
void ArrayWindow::setYScale(double p)
{
	yScale=p;
}

//=============================================================================
double ArrayWindow::getYScale()
{
  return yScale;
}

//=============================================================================
void ArrayWindow::setYScale2(double p)
{
  yScale2=p;
}

//=============================================================================
double ArrayWindow::getYScale2()
{
  return yScale2;
}

//=============================================================================
void ArrayWindow::setXScale(double p)
{
  xScale=p;
}

//=============================================================================
double ArrayWindow::getXScale()
{
	return xScale;
}

//=============================================================================
void ArrayWindow::setXShift(double p)
{
  xShift=p;
}

//=============================================================================
double ArrayWindow::getXShift()
{
	return xShift;
}

//=============================================================================
void ArrayWindow::setBackground(int p)
{
	background=p;
}

//=============================================================================
int ArrayWindow::handle(int event)
{
	int mouseButton;

	switch(event)
	{
		case FL_PUSH:
			mouseButton=Fl::event_button();

			//==============
			// Ignornance
			//==============
			if(Fl::event_state(FL_SHIFT))	// Shift Key is Down
			{
				if(mouseButton==1)		// Ignore
				{
					redraw();
					tw->redraw();
					if(ui->mapGroup->visible())
					{
						cw->redraw();
					}
					return Fl_Double_Window::handle(event);
				}
				else if(mouseButton==3)	// Clear ignorance
				{
					dataArray->clearIgnoredFlag();
					dataArray->process();
					redraw();
					tw->redraw();
					if(ui->mapGroup->visible())
					{
						cw->redraw();
					}
					return 1;
				}				 
			}
			
			//==============
			// Selection
			//==============
			if(mouseButton==1)
			{
					redraw();
					tw->redraw();
					return Fl_Double_Window::handle(event);
			}
			else if(mouseButton==3)		// Clear selected
			{
				clearSelected();
				redraw();
				tw->redraw();
				return 1;
			}
			
			return Fl_Double_Window::handle(event);

		default:
			return Fl_Double_Window::handle(event);
	}
}

//=============================================================================
// Image Management
//=============================================================================
void ArrayWindow::setImageFileFormat(const char *type)
{
	image->setFileFormat(type);
}

//=============================================================================
char* ArrayWindow::getImageFileFormat()
{
	return image->getFileFormat();
}

//=============================================================================
void ArrayWindow::loadImage()
{
	image->openImageFile();
}

//=============================================================================
void ArrayWindow::openImageFile(const char *imageFileName)
{
	image->openImageFile(imageFileName);
}

//=============================================================================
void ArrayWindow::setImageXSize(int size)
{
	image->setXSize(size);
}

//=============================================================================
void ArrayWindow::setImageYSize(int size)
{
	image->setYSize(size);
}

//=============================================================================
void ArrayWindow::setImageX0(int p)
{
	image->setX0(p);
}

//=============================================================================
void ArrayWindow::setImageY0(int p)
{
	image->setY0(p);
}

//=============================================================================
int ArrayWindow::getImageX0()
{
	return image->getX0();
}

//=============================================================================
int ArrayWindow::getImageY0()
{
	return image->getY0();
}

//=============================================================================
int ArrayWindow::getImageXSize()
{
	return image->getXSize();
}

//=============================================================================
int ArrayWindow::getImageYSize()
{
	return image->getYSize();
}

//=============================================================================
void ArrayWindow::draw()
{
	clear();

	drawBackground();

	if(showTrace)
	{
		drawTrace();
	}
	
	if(showRliValue)
	{
		drawRliValue();
	}

	if(showDiodeNum)
	{
		drawDiodeNum();
	}

	drawScale();
}

//=============================================================================
void ArrayWindow::clear()
{
	fl_color(bg);
	fl_rectf(0,0,w(),h());
}

//=============================================================================
void ArrayWindow::drawBackground()
{
	if(background==BG_None)
	{
		return;
	}
	else if(background==BG_Image)
	{
		image->draw();
	}
	else if(background==BG_Rli)
	{
		drawRli();
	}
	else if(background==BG_Max_Amplitude)
	{
		drawMaxAmp();
	}
	else if(background==BG_Spike_Amp)
	{
		drawSpikeAmp();
	}
	else if(background==BG_Max_Amp_Latency)
	{
		drawMaxAmpLatency();
	}
	else if(background==BG_Half_Amp_Latency)
	{
		drawHalfAmpLatency();
	}
	else if(background==BG_EPSP_Latency)
	{
		drawEPSPLatency();
	}
}

//=============================================================================
void ArrayWindow::drawDiodeNum()
{
	int i;

	fl_font(FL_COURIER_BOLD,11);

	for(i=0;i<Num_Diodes;i++)
	{
		diodes[i]->drawDiodeNum();
	}

	fl_font(FL_HELVETICA,11);
}

//=============================================================================
void ArrayWindow::drawRli()
{
	int i;
	double ratio;

	for(i=0;i<464;i++)
	{
		ratio=dataArray->getRliRatio(i);

		if(ratio<0 || ratio>1)
		{
			fl_color(char(155),0,0);
		}
		else
		{
			colorControl->setGrayScale(ratio);
		}

		diodes[i]->drawHexagon();
	}
}

//=============================================================================
void ArrayWindow::drawMaxAmp()
{
	int i;

	dataArray->setMaxAmp2DataFeature();

	for(i=0;i<Num_Diodes-8;i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		diodes[i]->drawHexagon();
	}
}

//=============================================================================
void ArrayWindow::drawSpikeAmp()
{
	int i;

	dataArray->setSpikeAmp2DataFeature();

	for(i=0;i<Num_Diodes-8;i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		diodes[i]->drawHexagon();
	}
}

//=============================================================================
void ArrayWindow::drawMaxAmpLatency()
{
	int i;

	dataArray->setMaxAmpLatency2DataFeature();

	for(i=0;i<Num_Diodes-8;i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		diodes[i]->drawHexagon();
	}
}

//=============================================================================
void ArrayWindow::drawHalfAmpLatency()
{
	int i;

	dataArray->setHalfAmpLatencyRatio2DataFeature();

	for(i=0;i<Num_Diodes-8;i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		diodes[i]->drawHexagon();
	}
}

//=============================================================================
void ArrayWindow::drawEPSPLatency()
{
	int i;

	dataArray->setEPSPLatency2DataFeature();

	for(i=0;i<Num_Diodes-8;i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		diodes[i]->drawHexagon();
	}
}

//=============================================================================
void ArrayWindow::drawRliValue()
{
	int i;

	fl_font(FL_HELVETICA_BOLD,10);

	for(i=0;i<Num_Diodes-8;i++)
	{
		diodes[i]->drawRliValue(dataArray->getRli(i));
	}

	fl_font(FL_HELVETICA,11);
}

//=============================================================================
void ArrayWindow::drawScale()
{
	double xScaleBarValue,yScaleBarValue;
	int X=w()-100;
	int Y=h()-30;;
	int diodeW=Diode_Width;
	int diodeH=Diode_Height;

	// Draw Lines
	fl_color(bg);
	fl_rectf(X-1,Y-1,diodeW+3,3);				// Horizantal
	fl_rectf(X-1+diodeW,Y-diodeH-1,3,diodeH+3);	// Vertical
	fl_color(fg);
	fl_line(X,Y,X+diodeW,Y);
	fl_line(X+diodeW,Y,X+diodeW,Y-diodeH);

	// Time axis
	int numPts=dc->getNumPts();
	double intPts=dc->getIntPts();

	xScaleBarValue=numPts*intPts/xScale/1000.0;

	_gcvt_s(txtBuf, 32, xScaleBarValue, 3);
	strcat_s(txtBuf, 32," (sec)");
	drawTxt(txtBuf,fg,bg,X,Y+20);

	// Amp axis
	yScaleBarValue=diodeH/yScale;

	_gcvt_s(txtBuf, 32, yScaleBarValue, 3);

	if(sp->getRliDivFlag())
	{
		strcat_s(txtBuf, 32, " (mV/V)");
	}
	else
	{
		strcat_s(txtBuf, 32, " (mV)");
	}

	drawTxt(txtBuf,fg,bg,X,Y-diodeH-5);

	// Draw Pseudo Color Scale
	int i;
	for(i=-1;i<=101;i++)
	{
		colorControl->setScaleColor(double(i)/100.0);
		fl_line(10,h()-10-i,30,h()-10-i);
	}

	colorControl->setGrayScale(0.5);
	fl_line(9,h()-10,35,h()-10);
	fl_line(9,h()-60,35,h()-60);
	fl_line(9,h()-110,35,h()-110);

	fl_line(9,h()-8,31,h()-8);
	fl_line(9,h()-112,31,h()-112);

	fl_line(9,h()-8,9,h()-112);
	fl_line(31,h()-8,31,h()-112);
}

//=============================================================================
void ArrayWindow::drawTrace()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		diodes[i]->drawTrace(dataArray->getProDataMem(i));
	}
}

//=============================================================================
void ArrayWindow::drawTxt(char *txtBuf,Fl_Color fg,Fl_Color bg,int x,int y)
{
	fl_font(FL_HELVETICA,12);

	fl_color(bg);
	fl_draw(txtBuf,x+1,y);
	fl_draw(txtBuf,x-1,y);
	fl_draw(txtBuf,x,y+1);
	fl_draw(txtBuf,x,y-1);
	fl_color(fg);
	fl_draw(txtBuf,x,y);
}

//=============================================================================
void ArrayWindow::addToSelectedList(int index)
{
	selectedDiodes[numSelectedDiodes]=index;
	numSelectedDiodes++;
}

//=============================================================================
void ArrayWindow::deleteFromSelectedList(int index)
{
	int i;
	int iList;

	for(i=0;i<numSelectedDiodes;i++)
	{
		if(index==selectedDiodes[i])
		{
			iList=i;
			break;
		}
	}

	numSelectedDiodes--;

	for(i=iList;i<numSelectedDiodes;i++)
	{
		selectedDiodes[i]=selectedDiodes[i+1];
	}
}

//=============================================================================
void ArrayWindow::clearSelected()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		diodes[i]->clearSelected();
	}

	numSelectedDiodes=0;

	colorControl->resetUsed();
}

//=============================================================================
int ArrayWindow::getNumSelectedDiodes()
{
	return numSelectedDiodes;
}

//=============================================================================
int *ArrayWindow::getSelectedDiodes()
{
	return selectedDiodes;
}

//=============================================================================
void ArrayWindow::setColorAsDiode(int index)
{
	diodes[index]->setColorAsDiode();
}

//=============================================================================
void ArrayWindow::selectDiode(int input)
{
	diodes[input]->select();
}

//=============================================================================
void ArrayWindow::setDiodes()
{
	int i,j,index;

	//==============================================
	int width;
	int start;
	int yPosition;
	int x[472];
	int y[472];

	// Row 0
	index=0;

	for(i=0;i<12;i++)
	{
		x[index]=11+2*i;
		y[index]=0;
		index++;
	}

	// Row 1~11
	width=14;
	start=10;
	yPosition=1;

	for(j=0;j<11;j++)
	{
		for(i=0;i<width;i++)
		{
			x[index]=start+2*i;
			y[index]=yPosition;
			index++;
		}

		width++;
		start--;
		yPosition++;
	}

	// Row 12
	for(i=0;i<23;i++)
	{
		x[index]=1+2*i;
		y[index]=12;
		index++;
	}

	// Row 13~23
	width=24;
	start=0;
	yPosition=13;

	for(j=0;j<11;j++)
	{
		for(i=0;i<width;i++)
		{
			x[index]=start+2*i;
			y[index]=yPosition;
			index++;
		}

		width--;
		start++;
		yPosition++;
	}

	// Row 24
	for(i=0;i<11;i++)
	{
		x[index]=13+2*i;
		y[index]=24;
		index++;
	}

	// Row FP
	for(i=0;i<4;i++)
	{
		x[index]=2*i;
		y[index]=-1;
		index++;
	}
	for(i=0;i<4;i++)
	{
		x[index]=40+2*i;
		y[index]=-1;
		index++;
	}

	//==============================================
	int pixel_x;
	int pixel_y;

	for(i=0;i<Num_Diodes;i++)
	{
		diodeMap[x[i]+2][y[i]+1]=i; // Shift map

		pixel_x=x[i]*13+7;
		pixel_y=y[i]*22+35;
		diodeX[i]=pixel_x;
		diodeY[i]=pixel_y;

		diodes[i]=new Diode(pixel_x,pixel_y,Diode_Width,Diode_Height,i);
	}

	numSelectedDiodes=0;
}

//=============================================================================
void ArrayWindow::saveBGData()
{
	//==================================================
	// Get file name
	char *fileName=fl_file_chooser("Save background data (all diodes) to a file",
		"*.dat","Data.dat");

	if(!fileName)
	{
		return;
	}

	//==================================================
	// Open file
	std::fstream file;
	file.open(fileName,std::ios::out);

	//
	int i;

	if(background==BG_Max_Amplitude)
	{
		dataArray->setMaxAmp2DataFeature();
	}
	else if(background==BG_Spike_Amp)
	{
		dataArray->setSpikeAmp2DataFeature();
	}
	else if(background==BG_Half_Amp_Latency)
	{
		dataArray->setHalfAmpLatency2DataFeature();
	}

	for(i=0;i<Num_Diodes-8;i++)
	{
		file<<i+1<<'\t';

		if(dataArray->getIgnoredFlag(i))
		{
			file<<-1<<'\n';
		}
		else
		{
			file<<dataArray->getDataFeature(i)<<'\n';
		}
	}

	//==================================================
	// Open file
	file.close();
}

//=============================================================================
