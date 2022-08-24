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

#include <iostream>

using namespace std;
//=============================================================================
Fl_Color rliValueColor=fl_rgb_color(200,200,255);
Fl_Color diodeNumColor=fl_rgb_color(255,255,0);

//=============================================================================
Diode::Diode(int x,int y,int w,int h,int i):Fl_Widget(x,y,w,h)
{
	index=i;
	colorIndex=-1;

	if(index< 0)
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

	// Draw fewer points as the drawing space gets smaller
	int step = numPts / (w() * 4);
	step = max(1, step);

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
					if (fpFlag)
					{
						for(int i=0;i<numPts;i++)
						{
							fl_vertex(i*intPts,proData[i]);
						}
					}
					else
					{
						for(int i = 0; i < numPts; i += step)
						{
							fl_vertex(i*intPts,proData[i]);
						}
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
		//case FL_KEYUP:
			//if(event_key)

		case FL_PUSH:
			if (aw->getContinuous()) {
				//if (mouseButton == 1)	// Left Button is Down
				//{
				if (Fl::event_state(FL_SHIFT)) {
					select(1, 1);
				}
				else {
					select(1);
				}
				redraw();
				//tw->redraw();
				return 1;
				//}
			}
			else {
				mouseButton = Fl::event_button();

				//==============
				// Ignornance
				//==============
				if (Fl::event_state(FL_SHIFT))	// Shift key is down.
				{
					if (mouseButton == 1)			// Left Button is Down
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
					if (mouseButton == 1)	// Left Button is Down
					{
						select();
					}
				}
			}
			return 1;
		default:
			return Fl_Widget::handle(event);
	}
}

//=============================================================================
void Diode::select(int altDown, int shiftDown)
{
//	cout << " line 221 index " << index << endl;
	if (altDown)
	{			
		if (!(colorIndex < 0))		//remove diode from region selection with shift
		{
			if (aw->deleteFromSelectedList(index, altDown))
			{
				//colorControl->releaseColorIndex(colorIndex);
				colorIndex = -1;
//				cout << " diode line 230 " << altDown << " shiftDown " << shiftDown << endl;
			}
		}

		else if (colorIndex < 0 && shiftDown==0)
		{
			/*if (aw->getNumSelectedDiodesAverage(aw->getCurrentRegionIndex) == 0) {
				colorIndex = colorControl->getColorIndex();
			}
			else{}*/
			aw->addToSelectedList(index, altDown);
//			cout << " diode line 242 " << altDown << " shiftDown " << shiftDown << endl;
		}
		else if (colorIndex < 0)			// new feature shift-click selects 3x3 diodes, center and 8 neighbors
		{
			if (shiftDown)
			{
				int width = dataArray->binned_width();
//				cout << " diode line 248 width " << width << " shiftDown " << shiftDown << endl;
				aw->addToSelectedList(index, altDown);								//center diode
				if(index%width!=0) aw->addToSelectedList(index - 1, altDown);			// center row left
				if((index+1)%width!=0) aw->addToSelectedList(index + 1, altDown);		//center row right

				if (index > width && index % width != 0) aw->addToSelectedList(index - (width+1), altDown);	//top row left
				if (index > width) aw->addToSelectedList(index - width, altDown);						//top row center	
				if (index > width && (index + 1) % width != 0) aw->addToSelectedList(index - (width -1), altDown); // top row right

				if (index < width*(width-1) && index % width != 0) aw->addToSelectedList(index +(width -1), altDown);				//bottom row left
				if (index < width*(width-1)) aw->addToSelectedList(index + width, altDown);								//bottom row center
				if (index < width*(width-1) && (index + 1) % width != 0) aw->addToSelectedList(index + width+1, altDown);		//bottom row right
			}
		}
	}
	else {
		if (colorIndex < 0)
		{
			colorIndex = colorControl->getColorIndex();
			aw->addToSelectedList(index);
//			cout << " diode line 249 " << altDown << " shiftDown " << shiftDown << endl;
		}
		else
		{
			colorControl->releaseColorIndex(colorIndex);
			colorIndex = -1;
			aw->deleteFromSelectedList(index);
//			cout << " diode line 256 " << altDown << " shiftDown " << shiftDown << endl;
		}
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
void Diode::drawBackground()
{
	fl_push_matrix();
	{
		fl_translate(x(),y());
		{
			fl_begin_polygon();
				fl_vertex(0,0);
				fl_vertex(w(),0);
				fl_vertex(w(),h());
				fl_vertex(0,h());
			fl_end_polygon();
		}
	}
	fl_pop_matrix();
}

void Diode::setColorIndex(int i)
{
	this->colorIndex = i;
}

//=============================================================================
void Diode::setColorAsDiode()
{
	colorControl->setTraceColor(colorIndex);
}

//=============================================================================
