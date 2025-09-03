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
#include "Data.h"

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
				//if (mouseButton == 1)	// Left Button is Down				Note that FL_ALT does not appear to be anywhere so the ALT button is never used
				//{
				if (Fl::event_state(FL_CTRL)) {
//					cout << "line 177" << endl;
					select(1, 0, 1);
				}
				if (Fl::event_state(FL_SHIFT)) {
//					cout << "line 181" << endl;
					select(1, 1, 0);
				}
				else if (!Fl::event_state(FL_CTRL) && !Fl::event_state(FL_SHIFT))
//				else
				{
//					cout << "line 185" << endl;
					select(1);
				}
				redraw();
				return 1;
			}
			else {
				mouseButton = Fl::event_button();

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
void Diode::select(int altDown, int shiftDown, int ctrlDown)
{
	if (altDown)
	{
//		cout << "line 227 " << colorIndex << endl;
		if (!(colorIndex < 0))		//remove diode from region selection with shift
		{
			if (aw->deleteFromSelectedList(index, altDown))
			{
				//colorControl->releaseColorIndex(colorIndex);
				colorIndex = -1;
			} 
		}
//		else if (colorIndex < 0 && shiftDown==0 && ctrlDown==0)
		else if (shiftDown == 0 && ctrlDown == 0)
		{
			aw->addToSelectedList(index, altDown);
		}
//		else if (colorIndex < 0 && ctrlDown == 0)
//		{
//			aw->addToSelectedList(index, altDown);
//		}
/*		else if (colorIndex < 0)			// new feature shift-click selects 3x3 diodes, center and 8 neighbors
		{									// new code below replaces this one which was a simple 3x3 ROI
			if (shiftDown)
			{
				int width = dataArray->binned_width();

				aw->addToSelectedList(index + width, altDown); //center diode

				if (index%width != 0) aw->addToSelectedList(index - 1, altDown);			// center row left
				if ((index + 1) % width != 0) aw->addToSelectedList(index + 1, altDown);		//center row right

				if (index > width && index % width != 0) aw->addToSelectedList(index - (width + 1), altDown);	//top row left
				if (index > width) aw->addToSelectedList(index - width, altDown);						//top row center	
				if (index > width && (index + 1) % width != 0) aw->addToSelectedList(index - (width - 1), altDown); // top row right

				if (index < width*(width - 1) && index % width != 0) aw->addToSelectedList(index + (width - 1), altDown);				//bottom row left
				if (index < width*(width - 1)) aw->addToSelectedList(index + width, altDown);	//bottom row center
				if (index < width*(width - 1) && (index + 1) % width != 0) aw->addToSelectedList(index + width + 1, altDown);		//bottom row right

			}
		}*/
		if (shiftDown)					//	**new feature replaces above to select diodes from a 3x3 ROI to maximize SNR** 
			{
				Data* data;
				data = NULL;
				int width ;
				int regionIndex;
				double SNR_ROI=0, SNR_ROI_Temp=0;
				width = dataArray->binned_width();
				aw->addToSelectedList(index, altDown);						//center diode	
				regionIndex = aw->getCurrentRegionIndex();	
				dataArray->aveROIData(regionIndex);
				data = dataArray->getROIAve(regionIndex);
				data->measureProperties();
				SNR_ROI = data->getMaxAmp()/data->getSD();

				if (index%width!= 0)
				{
					aw->addToSelectedList(index-1, altDown);			// center row left	
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp= data->getMaxAmp()/data->getSD();
						if (SNR_ROI_Temp < SNR_ROI)
						{
							if (aw->deleteFromSelectedList(index-1, altDown));
							aw->get_diode(index - 1)->setColorIndex(-1);
//							cout << " SNR #2 skip " << SNR_ROI << " index " << index-1 << endl;
						}
						else
						{
							SNR_ROI = SNR_ROI_Temp;
//							cout << " SNR #2 " << SNR_ROI << " index " << index - 1 << endl;
						}
				}

				if((index+1)%width!=0) 
				{
					aw->addToSelectedList(index + 1, altDown);		//center row right
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp = data->getMaxAmp()/data->getSD();
						if (SNR_ROI_Temp < SNR_ROI)
						{
							if (aw->deleteFromSelectedList(index + 1, altDown));
							aw->get_diode(index + 1)->setColorIndex(-1);
						}
						else
						{
							SNR_ROI = SNR_ROI_Temp;
						}
					}

				if (index > width && index % width != 0)
				{
					aw->addToSelectedList(index - (width + 1), altDown);	//top row left
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp = data->getMaxAmp() / data->getSD();
					if (SNR_ROI_Temp < SNR_ROI)
					{
						if(aw->deleteFromSelectedList(index - (width + 1), altDown));
						aw->get_diode(index - (width + 1))->setColorIndex(-1);
					}
					else
					{
						SNR_ROI = SNR_ROI_Temp;
					}
				}

				if (index > width)
					{
					aw->addToSelectedList(index - width, altDown);						//top row center
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp = data->getMaxAmp() / data->getSD();
					if (SNR_ROI_Temp < SNR_ROI)
					{
						if(aw->deleteFromSelectedList(index - width, altDown));
						aw->get_diode(index - width)->setColorIndex(-1);
					}
					else
						{
							SNR_ROI = SNR_ROI_Temp;
						}
					}

				if (index > width && (index + 1) % width != 0)
					{
					aw->addToSelectedList(index - (width - 1), altDown); // top row right
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp = data->getMaxAmp() / data->getSD();
						if (SNR_ROI_Temp < SNR_ROI)
						{
							if(aw->deleteFromSelectedList(index - (width - 1), altDown));
							aw->get_diode(index - (width - 1))->setColorIndex(-1);
						}
						else
						{
							SNR_ROI = SNR_ROI_Temp;
						}
					}

				if (index < width*(width - 1) && index % width != 0)
					{
					aw->addToSelectedList(index + (width - 1), altDown);			//bottom row left
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp = data->getMaxAmp() / data->getSD();
						if (SNR_ROI_Temp < SNR_ROI)
						{
							if(aw->deleteFromSelectedList(index + (width - 1), altDown));
							aw->get_diode(index + (width - 1))->setColorIndex(-1);
						}
						else
						{
							SNR_ROI = SNR_ROI_Temp;
						}
					}

				if (index < width*(width-1)) 
					{
					aw->addToSelectedList(index + width, altDown);										//bottom row center
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp = data->getMaxAmp() / data->getSD();
						if (SNR_ROI_Temp < SNR_ROI)
						{
							if(aw->deleteFromSelectedList(index + width, altDown));
							aw->get_diode(index + width)->setColorIndex(-1);
						}
						else
						{
							SNR_ROI = SNR_ROI_Temp;
						}
					}

				if (index < width*(width-1) && (index + 1) % width != 0) 
					{
					aw->addToSelectedList(index + width + 1, altDown);		//bottom row right
					dataArray->aveROIData(regionIndex);
					data = dataArray->getROIAve(regionIndex);
					data->measureProperties();
					SNR_ROI_Temp = data->getMaxAmp() / data->getSD();
						if (SNR_ROI_Temp < SNR_ROI)
						{
						if(aw->deleteFromSelectedList(index + width + 1, altDown));
						aw->get_diode(index + width + 1)->setColorIndex(-1);
						}
						else
						{
							SNR_ROI = SNR_ROI_Temp;
						}
				}
			}
		else 
			{
			if (ctrlDown)		// for 5x5 square - select pixels that maximize SNR
				{
				Data* data;
				data = NULL;
				int width;
				int jjl = -2, jjh = 2, kkl = -2 ,kkh =2;			//used to remove pixels out of bounds
				int regionIndex;
				int index5=index;															//index used for 5x5 ROI optimization
				double SNR_ROI = 0, SNR_ROI_Temp = 0;
				width = dataArray->binned_width();
				
				if (((index+1)%width) == 0) kkh = 0;				//deal with out of bounds pixels
				if (((index+2)%width) == 0) kkh = 1;
				if (index%width == 0) kkl = 0;
				if (((index-1)%width) == 0) kkl = -1;
				if (index < 80) jjl = 0;
				if (index >=80 && index < 160) jjl = -1;
				if (index >= 6320) jjh = 0;
				if (index < 6320 && index > 6240) jjh = 1;
				
				regionIndex = aw->getCurrentRegionIndex();
				aw->addToSelectedList(index, altDown);						//center diode	
				dataArray->aveROIData(regionIndex);
				data = dataArray->getROIAve(regionIndex);
				data->measureProperties();
				SNR_ROI = 0;
				data->getMaxAmp() / data->getSD();
				for (int jj = jjl; jj <= jjh; jj++)										//row index
				{
					for (int kk = kkl; kk <= kkh; kk++)									//column index
					{
						index5 =index+jj*width+kk;
						if (index5!=index)
						{
							aw->addToSelectedList(index5, altDown);
							dataArray->aveROIData(regionIndex);
							data = dataArray->getROIAve(regionIndex);
							data->measureProperties();
							SNR_ROI_Temp = data->getMaxAmp() / data->getSD();
							if (SNR_ROI_Temp < SNR_ROI)
							{
								if (aw->deleteFromSelectedList(index5, altDown));
								aw->get_diode(index5)->setColorIndex(-1);
//								cout << " SNR 5x5 skip " << SNR_ROI << " index " << index5 << endl;
							}
							else
							{
								SNR_ROI = SNR_ROI_Temp;
//								cout << " SNR 5x5 " << SNR_ROI << " index " << index5 << endl;
							}
						}
					}
				}
			}
		}
	}
	else {
		if (colorIndex < 0)
		{
			colorIndex = colorControl->getColorIndex();		// selects diodes one at a time, not used when ROIs are being selected
			aw->addToSelectedList(index);
		}
		else
		{
			colorControl->releaseColorIndex(colorIndex);		//removes a previously selected diode
			colorIndex = -1;

			aw->deleteFromSelectedList(index);
		}
	}
}

//=============================================================================
/*int Diode::getIndex()
{
	return index;
}*/

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
