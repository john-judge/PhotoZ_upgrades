//=============================================================================
// ArrayWindow.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()
#include <string.h>		// strcat()
#include <math.h>
#include <fstream>

#include <windows.h>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.h>
#include <FL/fl_ask.h>
#include <FL/fl_file_chooser.h>
#include <FL/Fl_Output.H>

#include "ArrayWindow.h"
#include "UserInterface.h"
#include "Diode.h"
#include "Image.h"
#include "Color.h"
#include "DAPController.h"
#include "DataArray.h"
#include "SignalProcessor.h"
#include "LiveFeed.h"
#include "Definitions.h"
#include "OpenCVImage.h"

#include <iostream>

using namespace std;
extern char txtBuf[];

//=============================================================================
ArrayWindow::ArrayWindow(int X,int Y,int W,int H)
:Fl_Double_Window(X,Y,W,H)
{
	image = new Image();

	bg_image = new Fl_OpenCV(0, 0, 1, 1);

	// indexes of the FP diodes will be negative
	for (int i = 0; i < NUM_FP_DIODES; i++)
		fp_diodes[i] = new Diode(0, 0, 1, 1, i - NUM_FP_DIODES);

	fg=FL_WHITE;
	bg=FL_BLACK;
	background = BG_SIGNAL_TO_NOISE; // JMJ updated default to SNR background
	showTrace = 0; // JMJ 7/6/21 - Disable trace in AW by default. It's not that useful for lil Dave, and GUI performance is now a concern.
	showRliValue = 0;
	showDiodeNum = 0;

	yScale  = pow((0.2*log2(1 + 1)),1); // yScale and yScale2 = 100 in photoz 3.0
	yScale2 = pow((0.2*log2(1 + 1)),1);
	xScale=1;
	xShift=0;
	continuous = 0;							//new

	// 12/12/2020 JMJ +6 lines
	zoomFactor = 1;
	xPan = 0;
	yPan = 0;
	xDragLast = 0;
	yDragLast = 0;
	isDragActive = false;
}

//=============================================================================
ArrayWindow::~ArrayWindow()
{
	int i;

	delete image;

	for (i = 0; i < NUM_FP_DIODES; i++)
		delete fp_diodes[i];
	for (auto it = diodes.begin(); it != diodes.end(); it++)
		delete *it;
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
int ArrayWindow::getBackground()
{
	return background;
}

//=============================================================================
void ArrayWindow::setBackground(int p)
{
	background=p;
}

//=============================================================================
void ArrayWindow::changeNumDiodes()			// for binning
{
	int num_bdiodes = dataArray->num_binned_diodes();
	int change = num_bdiodes - (int) diodes.size();

	if (change > 0)							// increase the number of diodes in the ROI
	{
		diodes.reserve(num_bdiodes);
		int oldsize = diodes.size();
		for (int i = 0; i < change; i++)
		{
			diodes.push_back(new Diode(0, 0, 1, 1, oldsize + i));
			add(diodes.back());			// add the widget to this group (so it gets events)
		}
	}
	else {
		while (change < 0)					// decrease the number of diodes in the ROI
		{
			remove(diodes.back());
			delete diodes.back();
			diodes.pop_back();
			change++;
		}
	}

	resizeDiodes();
}

void ArrayWindow::set_drag_active() {
	isDragActive = true;
}

void ArrayWindow::set_drag_inactive() {
	isDragActive = false;
}

bool ArrayWindow::get_drag_active() {
	return isDragActive;
}

void ArrayWindow::release_drag() {

	// clear drag location
	set_drag_inactive();

	// drag buffers get cleared in scheduled AW redraw
	xPan += (Fl::event_x() - xDragLast);
	yPan += (Fl::event_y() - yDragLast);

	resizeDiodes(); // updated xPan and yPan in Diode widget properties
	redraw();
}

//=============================================================================
int ArrayWindow::handle(int event)
{
	int mouseButton;
	double dBinning;
	double scrollAmt, tmpZoomFactor;

	bool verbose = false; // Set to true to receive debugging output to stdout

	switch(event)
	{
	case FL_RELEASE: // JMJ 12/12/2020
		if (Fl::event_button() == 1) {
			if (!get_drag_active()) return Fl_Double_Window::handle(event);
			release_drag();
			if (verbose) cout << "dragged to new center: (" << xPan << ", " << yPan << ")\n";
			return Fl_Double_Window::handle(event);
		}
	case FL_DRAG: // JMJ 12/12/2020
		if (Fl::event_button() == 1) {

			// save the location where the drag starts
			if(verbose && !isDragActive) cout << "activated drag\n";
			set_drag_active();

			return Fl_Double_Window::handle(event);
		}
	case FL_PUSH:
			
		xDragLast = Fl::event_x();
		yDragLast = Fl::event_y();

		mouseButton=Fl::event_button();
		//cout << " aw line 211 aw   "<< mouseButton << " event " << event << endl;	//test showed left = 1; middle = 2; right = 3; event always = 1
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
			else if(mouseButton==3)	// Clear ignored list with shift right click
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
		else if (Fl::event_state(FL_ALT))	// Alt Key is Down: reset view
		{
			cout << "resetting array window \n";
			zoomFactor = 1;
			xPan = 0;
			yPan = 0;
			xDragLast = 0;
			yDragLast = 0;

			redraw();
			Fl_Double_Window::handle(event);
			return 1;
		}
		//==============
		// Selection
		//==============
		if(mouseButton==1) // left-click
		{
			redraw();
			tw->redraw();
			Fl_Double_Window::handle(event);
			return 1;
		}

		else if(mouseButton==3)		// right-click: Clear selected
		{
			if(!Fl::event_state(FL_CTRL))	clearSelected(0);
			if (Fl::event_state(FL_CTRL))	clearSelected(1);
			redraw();
			tw->redraw();
			return 1;
		}
		return Fl_Double_Window::handle(event);
	case FL_MOUSEWHEEL: // JMJ 12/12/2020
		scrollAmt = Fl::event_dy() > 0 ? -0.2 : 0.2;
		tmpZoomFactor = zoomFactor;
		zoomFactor = std::min((const double)50, max(0.8, zoomFactor + scrollAmt));

		if (zoomFactor != tmpZoomFactor) {
			// As we zoom, automatically adjust binning ~ # raw / zoom
			dBinning = (int)std::max((const double)1, dataArray->raw_height() * dataArray->raw_width() / (zoomFactor * DEFAULT_BINNING_FACTOR));

			mc->set_digital_binning(dBinning);

			// clear selected. TO DO: save selected, but separate at each zoom level
			if (!Fl::event_state(FL_CTRL))	clearSelected(0);
			if (Fl::event_state(FL_CTRL))	clearSelected(1);

			if (verbose) {
				cout << "zooming to factor: " << zoomFactor \
					<< "\tbinning: " << dBinning << "\n";
			}
			resizeDiodes();
			redraw();
				
		}
		return Fl_Double_Window::handle(event);
	default:
		return Fl_Double_Window::handle(event);
	}
}

//=============================================================================
void ArrayWindow::resize(int X, int Y, int W, int H) {			// new to change diode size 
	Fl_Double_Window::resize(X, Y, W, H);
	resizeDiodes();
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
	
	if(showTrace) {
		drawTrace();
	}

	if(showRliValue) {
		drawRliValue();
	}

	if(showDiodeNum) {
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
	switch (background) {
		case BG_None:
			return;
		case BG_Image:
			return image->draw();
		case BG_Rli:
			return drawRli();
		case BG_Max_Amplitude:
			return drawMaxAmp();
		case BG_Spike_Amp:
			return drawSpikeAmp();
		case BG_Max_Amp_Latency:
			return drawMaxAmpLatency();
		case BG_Half_Amp_Latency:
			return drawHalfAmpLatency();
		case BG_SIGNAL_TO_NOISE:
			return drawSignalToNoise();
		case BG_EPSP_Latency:
			return drawEPSPLatency();
		case BG_Live_Feed:
			return lf->drawBackground();
		default:
			return;
	}
}

//=============================================================================
void ArrayWindow::drawDiodeNum()
{
	int i;

	fl_font(FL_COURIER_BOLD,11);

	for (i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++)
	{
		get_diode(i)->drawDiodeNum();
	}

	fl_font(FL_HELVETICA,11);
}

//=============================================================================
void ArrayWindow::drawRli()
{
	int i;
	double ratio;

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		ratio=dataArray->getRliRatio(i);

		if(ratio<0 || ratio>1) {
			fl_color(char(155),0,0);
		} else {
			colorControl->setGrayScale(ratio);
		}
		get_diode(i)->drawBackground();
	}
}

//=============================================================================
void ArrayWindow::drawMaxAmp()
{
	int i;

	dataArray->setMaxAmp2DataFeature();

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		get_diode(i)->drawBackground();
	}
}

//=============================================================================
void ArrayWindow::drawSpikeAmp()
{
	int i;

	dataArray->setSpikeAmp2DataFeature();

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		get_diode(i)->drawBackground();
	}
}

//=============================================================================
void ArrayWindow::drawSignalToNoise()
{
	int i;

	dataArray->setSignalToNoise2DataFeature();

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		if (dataArray->getIgnoredFlag(i))
		{
			fl_color(50, 50, 50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		get_diode(i)->drawBackground();
	}
}

//=============================================================================
void ArrayWindow::drawMaxAmpLatency()
{
	int i;

	dataArray->setMaxAmpLatency2DataFeature();

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		get_diode(i)->drawBackground();
	}
}

//=============================================================================
void ArrayWindow::drawHalfAmpLatency()
{
	int i;

	dataArray->setHalfAmpLatencyRatio2DataFeature();

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		get_diode(i)->drawBackground();
	}
}

//=============================================================================
void ArrayWindow::drawEPSPLatency()
{
	int i;

	dataArray->setEPSPLatency2DataFeature();

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		if(dataArray->getIgnoredFlag(i))
		{
			fl_color(50,50,50);
		}
		else
		{
			colorControl->setColor(dataArray->getDataFeature(i));
		}

		get_diode(i)->drawBackground();
	}
}

//=============================================================================
void ArrayWindow::drawRliValue()
{
	int i;

	fl_font(FL_HELVETICA_BOLD,10);

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		get_diode(i)->drawRliValue(dataArray->getRli(i));
	}

	fl_font(FL_HELVETICA,11);
}

//=============================================================================
void ArrayWindow::drawScale()
{
	if (diodes.size() <= 0)
		return;

	double xScaleBarValue,yScaleBarValue;
	int X=w()-100;
	int Y=h()-30;;
	int diodeW=diodes[0]->w();
	int diodeH=diodes[0]->h();

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
void ArrayWindow::drawTrace() {
	int drawnCount = 0;
	for (int i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++) {		//  modified to vary diode number with binning
		if (get_diode(i)->drawTrace(dataArray->getProDataMem(i)))
			drawnCount++;
	}
	//cout << "Number of bin traces rendered in array window: " << drawnCount - NUM_FP_DIODES << " of " << dataArray->num_binned_diodes() << "\n";
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
Diode* ArrayWindow::get_diode(int diodeNo)			//new ; mostly new until final saveBGdata action
{		
	if (diodeNo < 0) 
		return fp_diodes[diodeNo + NUM_FP_DIODES];
	return diodes[diodeNo];
}

//=============================================================================
void ArrayWindow::addToSelectedList(int index, int average)	// index is the diode #; average is a flag 0 or 1 to indicate average is performed 
{												
	if (!average) {
		selectedDiodes.push_back(index);
		tw->redraw();
	}
	else {
	if(currentRegionIndex<0) currentRegionIndex=0;			// with some sequences currentRegionsIndex was -1 and that caused crashes due to vector overflow
//		cout << "aw line 698 currentRegionIndex " << currentRegionIndex << endl;
		if (selectedDiodesAverage[currentRegionIndex].size() == 0) {
			int cI = colorControl->getColorIndex();
			get_diode(index)->setColorIndex(cI);
			selectedDiodesAverage[currentRegionIndex].push_back(cI);
/*			char regionStr[14];
			sprintf(regionStr , "Region %d", currentRegionIndex);
			Fl_Output(1051, 950 + numRegions * 30, 50, 65, regionStr);*/
//			ui->allRegionsDisplay->add(newRegion);
			this->numRegions += 1;
		}
		else {
			get_diode(index)->setColorIndex(selectedDiodesAverage[currentRegionIndex][0]);
		}
		selectedDiodesAverage[currentRegionIndex].push_back(index);
		tw->redraw();
	redraw();
	}
}

int ArrayWindow::getNumRegions()
{
	return numRegions;
}

int ArrayWindow::getCurrentRegionIndex()
{
	return currentRegionIndex;
}

void ArrayWindow::setContinuous(int continuous)			// "new region" button in GUI calls mc->setContinuous(o->value());
{
	if (continuous) 
		{
//		cout << "line 737 continuous " << continuous << " currentRegionIndex " << currentRegionIndex << endl;
		selectedDiodesAverage.push_back(std::vector<int> {});
		currentRegionIndex++;	
		}
	else 
		{
//		cout << "line 744 continuous " << continuous << " currentRegionIndex " << currentRegionIndex << endl;
		if (currentRegionIndex < 0) currentRegionIndex = 0;		// might crash with negative value see line 697
		if (selectedDiodesAverage[currentRegionIndex].size() == 0) 
			{
			//possibly add numRegions which keep tracks of number of Regions and update currentRegion for true purpose like name suggests
			selectedDiodesAverage.erase(selectedDiodesAverage.begin() + currentRegionIndex);
			currentRegionIndex--;
			}
		}
	this->continuous = continuous;
}

int ArrayWindow::getContinuous() {
	return continuous;
}

//=============================================================================
int ArrayWindow::deleteFromSelectedList(int index, int average)
{
//	cout << "line 763"  << endl;
	if (!average) {
//		cout << "line 765" << endl;
		int i;
		for (i = 0; i < (int)selectedDiodes.size(); i++)
		{
			if (index == selectedDiodes[i])
			{
				selectedDiodes.erase(selectedDiodes.begin() + i);
				tw->redraw();
				return 1;
			}
		}
	}
	else {
		int i;
//		cout << "line 780" << endl;
		for (i = 1; i < (int)selectedDiodesAverage[currentRegionIndex].size(); i++)
		{
			if (index == selectedDiodesAverage[currentRegionIndex][i])
			{
				selectedDiodesAverage[currentRegionIndex].erase(selectedDiodesAverage[currentRegionIndex].begin() + i);
				if (selectedDiodesAverage[currentRegionIndex].size() == 1) {
					selectedDiodesAverage[currentRegionIndex].erase(selectedDiodesAverage[currentRegionIndex].begin(), selectedDiodesAverage[currentRegionIndex].end());	
					this->numRegions -= 1;
				}
				tw->redraw();
				return 1;
			}
		}
	}
	return 0;
}

//=============================================================================
void ArrayWindow::clearSelected(int selection)
{
	int i;	
		if (selection == 0)
		{
//			cout << "aw line 823 " << selectedDiodes.size()<< endl;
		for (i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++)
			{
				get_diode(i)->clearSelected();
			}
		selectedDiodes.clear();
		colorControl->resetUsed();
//		cout << "aw line 828 " << selectedDiodes.size() << endl;
		/*while(numRegions!=0){
	selectedDiodesAverage[numRegions - 1].erase(selectedDiodesAverage[numRegions - 1].begin(), selectedDiodesAverage[numRegions - 1].end());
		this->numRegions -= 1;
		if (selectedDiodesAverage[currentRegionIndex].size() == 0) {
			//possibly add numRegions which keep tracks of number of Regions and update currentRegion for true purpose like name suggests
			selectedDiodesAverage.erase(selectedDiodesAverage.begin() + currentRegionIndex);
		currentRegionIndex--;
		} //blocking the above out and replacing with for loop below elliminated crashes after clearing with right click
		}
		cout << " line 819 numReg " << numRegions << " currentRegInd " << currentRegionIndex<< " contin " << continuous << endl;*/
		
		for (i = 0; i < numRegions; i++)
			{
				selectedDiodesAverage[i].clear();
			}
			currentRegionIndex = -1;
			numRegions = 0;
		}
		else if (selection == 1&&numRegions>0)					// when selection=1 just current ROI is cleared and user sent back to lower region number
		{
			int* ROIList = aw->getSelectedDiodesAverage(numRegions-1);	
			int RegionSize = aw->getNumSelectedDiodesAverage(numRegions-1);
			for (i = RegionSize - 1; i > 0; i--)
			{
				get_diode(ROIList[i])->setColorIndex(-1);		// reset colors to unselected
			}
			selectedDiodesAverage[numRegions - 1].clear();
			currentRegionIndex--;
			numRegions--;
		}	
}

//=============================================================================
int ArrayWindow::getNumSelectedDiodes()
{
	return selectedDiodes.size();
}

//=============================================================================
int ArrayWindow::getNumSelectedDiodesAverage(int idx)
{
	return selectedDiodesAverage[idx].size();
}

//=============================================================================
int *ArrayWindow::getSelectedDiodes()
{
	return selectedDiodes.data();
}

//=============================================================================
int *ArrayWindow::getSelectedDiodesAverage(int idx)
{
	return selectedDiodesAverage[idx].data();
}

//=============================================================================
void ArrayWindow::setColorAsDiode(int index)
{
	get_diode(index)->setColorAsDiode();
}

//=============================================================================
void ArrayWindow::selectDiode(int input)
{
	get_diode(input)->select();
}

void ArrayWindow::setSelectedDiodesAverageIndex(int regNo, int dioNo)
{
	return selectedDiodesAverage[regNo].push_back(dioNo);
}

//=============================================================================
void ArrayWindow::resizeDiodes()
{
	int i;

	if (diodes.size() <= 0)
		return;

	/* the fraction of the screen the array will take up. running into problems
	 * where an increase of one pixel means the array get much larger - diode
	 * width is only around 8 pixels
	 */
	const int num = 7;		//these numbers control the size of the array window
	const int den = 8;

	/* digital output placing */
	const int do_width = 30;		//these numbers only appear in the FP display statement line 895
	const int do_height = 30;
	const int do_yoffset = 0;

	int array_width = dataArray->binned_width();
	int array_height = dataArray->binned_height();
	int num_bdiodes = dataArray->num_binned_diodes();

	int diode_width = std::min(int(w() * num * zoomFactor / (den * array_width)), 
							   int(h() * num / (den * array_height)));
	int diode_height = diode_width * zoomFactor;

	// Center the array
	int array_xoffset = (int)((w() - diode_width*array_width) / 2 + xPan);
	int array_yoffset = (int)((h() - diode_height*array_height) / 2 + yPan);
	if (array_yoffset + array_height * diode_height > h())
		array_yoffset = do_yoffset + do_height;

	// Calculate the positions for each diode and resize
	for (i = 0; i < num_bdiodes; i++)
	{
		int x = array_xoffset + diode_width * (i % array_width);
		int y = array_yoffset + diode_height * (i / array_width);
		diodes[i]->resize(x, y, diode_width, diode_height);
	}
	for (i = 0; i < NUM_FP_DIODES; i++)
	{
		int x = (w()/2) - (NUM_FP_DIODES * do_width / 2) + (do_width * i);
		int y = do_yoffset;
		fp_diodes[i]->resize(x, y, do_width, do_height);
	}
}

//=============================================================================
int ArrayWindow::diodeX(int diodeNo)
{
	return get_diode(diodeNo)->x();
}

//=============================================================================
int ArrayWindow::diodeY(int diodeNo)
{
	return get_diode(diodeNo)->y();
}

//=============================================================================
int ArrayWindow::diodeW()
{
	return diodes[0]->w();
}

//=============================================================================
int ArrayWindow::diodeH()
{
	return diodes[0]->h();
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
	else if (background == BG_Rli)
	{
		dataArray->setRli2DataFeature();
	}

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
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
