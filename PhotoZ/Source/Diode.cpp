//=============================================================================
// Diode.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()

#include <FL/Fl.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.h>
#include <FL/Fl_Group.H>	// Event handle

#include "Definitions.h"
#include "Diode.h"
#include "Color.h"
#include "DataArray.h"
#include "ArrayWindow.h"
#include "DapController.h"

//=============================================================================
Fl_Color rliValueColor=fl_rgb_color(200,200,255);
Fl_Color diodeNumColor=fl_rgb_color(255,255,0);

//=============================================================================
Diode::Diode(int x,int y,int w,int h,int i):Fl_Widget(x,y,w,h)
{
	index=i;
	colorIndex=-1;

	if(index<464)
	{
		fpFlag=0;
	}
	else
	{
		fpFlag=1;
	}
}

//=============================================================================
Diode::~Diode()
{
}

//=============================================================================
void Diode::draw()
{
}

//=============================================================================
void Diode::drawTrace(double *proData)
{
	int numPts=dc->getNumPts();
	double intPts=double(w())/numPts;

	// Clip the drawing area
	fl_clip(x(),y()-h()/2,w()-1,2*h());
	{
		// Push The Current Matrix
		fl_push_matrix();
		{
			//-----------------------
			// Translation & Scaling
			//-----------------------
			fl_translate(x(),y()+0.7*h());

			if(!fpFlag)
			{
				fl_scale(aw->getXScale(),-aw->getYScale());
			}
			else
			{
				fl_scale(aw->getXScale(),-aw->getYScale2());
			}

			fl_translate(aw->getXShift(),0);

			//-----------------------
			// Set Color
			//-----------------------
			if(colorIndex<0)
			{
				fl_color(0,0,255);
			}
			else
			{
				colorControl->setTraceColor(colorIndex);
			}

			//-----------------------
			// Draw The Trace
			//-----------------------
			if(dataArray->getIgnoredFlag(index))
			{
				fl_begin_line();
					fl_vertex(0,0);
					fl_vertex(w(),0);
				fl_end_line();
			}
			else
			{
				fl_begin_line();
					for(int i=0;i<numPts;i++)
					{
						fl_vertex(i*intPts,proData[i]);
					}
				fl_end_line();
			}
		}
		fl_pop_matrix();
	}
	fl_pop_clip();
}

//=============================================================================
void Diode::drawRliValue(double rli)
{
	if(rli<0.1)
	{
		return;
	}

	char txtBuf[8];
	_gcvt_s(txtBuf, 8, rli, 2);

	drawTxt(txtBuf,x(),y()+25,rliValueColor);
}

//=============================================================================
void Diode::drawDiodeNum()
{
	drawTxt(i2txt(index+1),x(),y()+15,diodeNumColor);
}

//=============================================================================
void Diode::drawTxt(char *txt,int X,int Y,Fl_Color color)
{
	fl_color(FL_BLACK);	// Background
		fl_draw(txt,X,Y+1);
		fl_draw(txt,X,Y-1);
		fl_draw(txt,X+1,Y);
		fl_draw(txt,X-1,Y);
	fl_color(color);	// Number
		fl_draw(txt,X,Y);
}

//=============================================================================
int Diode::handle(int event)
{
	int mouseButton;

	switch(event)
	{
		case FL_PUSH:
			mouseButton=Fl::event_button();

			//==============
			// Ignornance
			//==============
			if(Fl::event_state(FL_SHIFT))	// Shift key is down.
			{
				if(mouseButton==1)			// Left Button is Down
				{
					dataArray->ignore(index);
				}
					
				dataArray->process();
			}
			//==============
			// Selection
			//==============
			else
			{
				if(mouseButton==1)	// Left Button is Down
				{
					select();
				}
			}
			return 1;

		default:
			return Fl_Widget::handle(event);
	}
}

//=============================================================================
void Diode::select()
{
	if(colorIndex<0)
	{
		colorIndex=colorControl->getColorIndex();
		aw->addToSelectedList(index);
	}
	else
	{
		colorControl->releaseColorIndex(colorIndex);
		colorIndex=-1;
		aw->deleteFromSelectedList(index);
	}
}

//=============================================================================
int Diode::getIndex()
{
	return index;
}

//=============================================================================
void Diode::clearSelected()
{
	colorIndex=-1;
}

//=============================================================================
void Diode::drawHexagon()
{
	fl_push_matrix();
	{
		fl_translate(x(),y());
		{
			double dW=0.5*w();
			double dH=0.25*h();
			fl_begin_polygon();
				fl_vertex(0,dH);
				fl_vertex(dW,0);
				fl_vertex(w(),dH);
				fl_vertex(w(),h()-dH);
				fl_vertex(dW,h());
				fl_vertex(0,h()-dH);
			fl_end_polygon();
		}
	}
	fl_pop_matrix();
}

//=============================================================================
void Diode::setColorAsDiode()
{
	colorControl->setTraceColor(colorIndex);
}

//=============================================================================
