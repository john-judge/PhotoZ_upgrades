//=============================================================================
// TraceWindow.cpp
//=============================================================================
#include <string.h>
#include <stdlib.h>		// _gcvt_s()
#include <fstream>
#include <math.h>
#include <io.h>
#include <iostream>

#include <FL/Fl.h>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.h>
#include <FL/fl_file_chooser.h>

#include "Definitions.h"
#include "UserInterface.h"
#include "TraceWindow.h"
#include "DataArray.h"
#include "Data.h"
#include "DapController.h"
#include "SignalProcessor.h"
#include <memory.h>

using namespace std;

//=============================================================================
TraceWindow::TraceWindow(int xx,int yy,int ww,int hh)
:Fl_Double_Window(xx,yy,ww,hh)
{
	fg=FL_WHITE;
	bg=FL_BLACK;

	pointLinePt=160;
	valueType='N';
	timeCourseType='M';

	// Scale Control
	yScale=3;
	fpYScale=3;
	xScale=1;
	xShift=0;
	ltpXScale=1;
	ltpXShift=0;

	// Flags
	timeCourseFlag=0;
	showPointLine=0;
	showFileLine=0;
	superimpose=0;
	showTrace=1;
	showSlope=0;
	showRef=0;
	showSavedTraces=0;
	timeCourseNormalizationFlag=0;
	save2FileType='p';

	handleType='o';

	// Save to File
	saveTracesStartPt=0;
	saveTracesEndPt=1000;

	//=================
	// Alpha control
	//=================
	showAlpha=0;
	showVm=0;
	calWholeVm=0;

	for(int i=0;i<4;i++)
	{
		changeBits[i]=1;
	}

	changeBits[4]=0;

	spikeAmp=0;

	alpha=0;	// Memory
	Vm=0;

	spikeSlopeThreshold=0;
	spikeAmpThreshold=0;

	onsetBounds[0]=float(160);
	onsetBounds[1]=float(180);
	aveData = NULL;
}

//=============================================================================
TraceWindow::~TraceWindow()
{
	delete [] alpha;
	delete [] Vm;
}

//=============================================================================
void TraceWindow::setPointLinePt(int pt)
{
	pointLinePt=pt;
}

//=============================================================================
int TraceWindow::getPointLinePt()
{
	return pointLinePt;
}

//=============================================================================
void TraceWindow::setShowFileLine(char p)
{
	showFileLine=p;
}

//=============================================================================
char TraceWindow::getShowFileLine()
{
	return showFileLine;
}

//=============================================================================
void TraceWindow::setShowPointLine(char p)
{
	showPointLine=p;
}

//=============================================================================
char TraceWindow::getShowPointLine()
{
	return showPointLine;
}

//=============================================================================
void TraceWindow::setSuperimpose(char p)
{
	superimpose=p;
}

//=============================================================================
void TraceWindow::setShowTrace(char p)
{
	showTrace=p;
}

//=============================================================================
void TraceWindow::setShowSlope(char p)
{
	showSlope=p;
}

//=============================================================================
char TraceWindow::getShowTrace()
{
	return showTrace;
}

//=============================================================================
void TraceWindow::setShowRef(char p)
{
	showRef=p;
}

//=============================================================================
void TraceWindow::setShowSavedTraces(char p)
{
	showSavedTraces=p;
}

//=============================================================================
char TraceWindow::getShowSavedTraces()
{
	return showSavedTraces;
}

//=============================================================================
void TraceWindow::setValueType(char p)
{
	valueType=p;
}

//=============================================================================
void TraceWindow::setTimeCourseType(char p)
{
	timeCourseType=p;
}

//=============================================================================
void TraceWindow::setTimeCourseFlag(char p)
{
	timeCourseFlag=p;
}

//=============================================================================
char TraceWindow::getValueType()
{
	return valueType;
}

//=============================================================================
void TraceWindow::setTimeCourseNormalizationFlag(char flag)
{
	timeCourseNormalizationFlag=flag;
}

//=============================================================================
void TraceWindow::saveTraces()
{
	//==================================================
	// Get Number of Selected Diodes
	int numSelectedDiodes = aw->getNumSelectedDiodes();
	int numRegions = aw->getNumRegions();
	if (numSelectedDiodes < 1 && numRegions <1)
	{
		return;
	}

	//==================================================
	// Get file name
	char *fileName = fl_file_chooser("Please enter the name of the Dat file",
		"*.dat", "Traces.dat");

	if (!fileName)
	{
		return;
	}

	//==================================================
	// Open file
	fstream file;
	file.open(fileName, ios::out);

	//==================================================
	// Get Selected Diode Array
	int *selectedDiodes = aw->getSelectedDiodes();
	int i, j;
	int numPts = dc->getNumPts();

	if (saveTracesStartPt >= numPts)
	{
		saveTracesStartPt = 0;
	}

	if (saveTracesEndPt >= numPts)
	{
		saveTracesEndPt = numPts - 1;
	}

	//==================================================
	// pt
	if (save2FileType == 'p')
	{
		file << "Pt";

		for (i = 0; i < numSelectedDiodes + numRegions; i++)		//save diode #  and ROI index 
		{
			if (i < numSelectedDiodes) file << "\tD" << selectedDiodes[i] + 1;
			if (i >= numSelectedDiodes && i < numSelectedDiodes + numRegions)	file << "\tROI" << i - numSelectedDiodes + 1;
		}
		file << "\n";
		for (i = saveTracesStartPt; i <= saveTracesEndPt; i++)
		{
			file << i;
			for (j = 0; j < numSelectedDiodes + numRegions; j++)
			{
				if (j < numSelectedDiodes)						// do the diodes
				{
					file << "\t" << (dataArray->getProDataMem(selectedDiodes[j]))[i];
				}
				if (j >= numSelectedDiodes && j < numSelectedDiodes + numRegions)   // do the ROIs
				{
					int numSelectedDiodesAverage = aw->getNumSelectedDiodesAverage(j - numSelectedDiodes) - 1;
					int *selectedDiodesAverage = aw->getSelectedDiodesAverage(j - numSelectedDiodes);
					double **proData = (double**)malloc(sizeof(double*)*numSelectedDiodesAverage);
					if (proData == NULL) 		return;
					double sum = 0;
					for (int ii = 0; ii < numSelectedDiodesAverage; ii++)
					{
						proData[ii] = dataArray->getProDataMem(selectedDiodesAverage[ii + 1]);
						sum += proData[ii][i];
					}
					sum = sum / numSelectedDiodesAverage;
					file << "\t" << sum;
				}
			}
		file << "\n";			//new line after adding data to all columns 
		}
	}
file.close();
}

//=============================================================================
void TraceWindow::saveTimeCourse()
{
	// If Time Course Type is None, return.
	if(timeCourseType=='N')
	{
		return;
	}

	//===========================================
	 
	int numSelectedDiodes=aw->getNumSelectedDiodes();				// Get Number of Selected Diodes and number of ROIs.
	int numRegions = aw->getNumRegions();
	if(numSelectedDiodes<1&&numRegions<1)							// If no diodes AND no ROIs selected, return.
	{
		return;
	}
	//===========================================
	// Get file name.
	char *fileName=fl_file_chooser("Enter the name of a Dat file","*.dat","TimeCourses.dat");
	if(!fileName)
	{
		return;
	}

	//===========================================
	// For loop control
	int i,j;

	//===========================================
	// Open file
	fstream file;
	file.open(fileName,ios::out);

	//===========================================
	// Get Selected Diode Array
	int *selectedDiodes=aw->getSelectedDiodes();

	//===========================================
	// Write titles to the file
	for(i=0;i<numSelectedDiodes+numRegions;i++)
	{
	if (i<numSelectedDiodes)	file<<"\tdio"<<selectedDiodes[i]+1;												// first write column heading for diodes
	if (i >=numSelectedDiodes && i< numSelectedDiodes+numRegions)	file << "\tROI" << i-numSelectedDiodes + 1;	// then regions
	}
	file<<"\n";

	//===========================================
	// Write data points
	Data *data;
	double *timeCourse = nullptr;
	double scale=1.0;
	int dataIndex=0;
	int refRecordNo=dataArray->getRecordXNo(1);
	int numRecord=recControl->getRecordNoMax();
	int startIndex=0;
	int endIndex=numRecord-1;

	//===========================================
	for(i=startIndex;i<=endIndex;i++)												// Go through every record.
	{
		file<<i+1;

		for(j=0;j<numSelectedDiodes+numRegions;j++)
		{
			if (j < numSelectedDiodes) {											// get data address for selected diodes
				dataIndex = selectedDiodes[j];
				data = dataArray->getData(dataIndex);
			}
			else  {
				dataArray->aveROIData(j-numSelectedDiodes);							// get data address for ROIs
				aveData = dataArray->getAveData();
				data = dataArray->getROIAve(j-numSelectedDiodes);
			}

			if(timeCourseType=='R')
			{
				timeCourse=data->getRliArray();
			}
			else if(timeCourseType=='A')
			{
				timeCourse=data->getAmpArray();
			}
			else if(timeCourseType=='M') // Max Amp
			{
				timeCourse=data->getMaxAmpArray();
			}
			else if(timeCourseType=='L')
			{
				timeCourse=data->getMaxAmpLatencyArray();
			}
			else if(timeCourseType=='H')
			{
				timeCourse=data->getHalfAmpLatencyArray();
			}
			else if (timeCourseType == 'J')
			{
				timeCourse = data->getHalfWidthArray();
			}
			
			//===================
			// Normalization
			if(timeCourseNormalizationFlag)
			{
				if(timeCourseType=='M')
				{
					scale=1.0/dataArray->getDataMaxAmp(dataIndex,1);
				}
				else
				{
					scale=1.0/timeCourse[refRecordNo];
				}
			}
			else
			{
				scale=1.0;
			}
			file<<"\t"<<timeCourse[i]*scale;
		}
		file<<"\n";
	}
	file.close();
}

//=============================================================================
void TraceWindow::setFgBgColor(Fl_Color foreGround,Fl_Color backGround)
{
	fg=foreGround;
	bg=backGround;
}

//=============================================================================
void TraceWindow::draw(/*int average*/)
{
	int numRegions = aw->getNumRegions();
	clear();

	int numSelectedDiodes=aw->getNumSelectedDiodes();

	if (numSelectedDiodes + numRegions > 0)
	{
		double height = h() / (numSelectedDiodes + numRegions + 1);
		int *selectedDiodes = aw->getSelectedDiodes();
		int i;
		for (i = 0; i < numSelectedDiodes; i++)
		{
			fl_push_matrix();
			{
				// Translation
				if (superimpose)
					fl_translate(0, h() / 2);
				else
					fl_translate(0, height*double(i + 1));

				//=================
				// Draw Base Line
				//=================
				fl_color(100, 100, 100);

				fl_begin_line();
				fl_vertex(0, 0);
				fl_vertex(w(), 0);
				fl_end_line();

				//=================
				// Draw Trace
				//=================
				if (showTrace)
				{
					drawTrace(selectedDiodes[i]);
				}

				//=================
				// Draw Slope
				//=================
				if (showSlope)
				{
					drawSlope(selectedDiodes[i]);
				}

				//=================
				// Draw Vm
				//=================
				if (i == (numSelectedDiodes - 1))
				{
					drawVm(selectedDiodes[i]);
				}

				//=================
				// Draw Time Course
				//=================
				if (timeCourseFlag)
				{
					drawTimeCourse(selectedDiodes[i],0);
				}

				//=====================
				// Draw Reference
				//=====================
				if (showRef)
				{
					drawRef(selectedDiodes[i]);
				}
			}
			fl_pop_matrix();
		}

		for (i = 0; i < numRegions; i++){
			fl_push_matrix();
			{
				// Translation
				if (superimpose)
					fl_translate(0, h() / 2);
				else
					fl_translate(0, height*(i+numSelectedDiodes+1));

				//=================
				// Draw Base Line
				//=================
				fl_color(100, 100, 100);

				fl_begin_line();
				fl_vertex(0, 0);
				fl_vertex(w(), 0);
				fl_end_line();

				//=================
				// Draw Trace
				//=================
				if (showTrace)
				{
					drawTrace(1, i);
				}

				//=================
				// Draw Slope
				//=================
				if (showSlope)
				{
					drawSlope(1/*selectedDiodes[i]*/,i); //1(dataNo) doesn't matter for ROIs 
				}

				//=================
				// Draw Vm
				//=================
				if (i == (numRegions-1))
				{
					//drawVm(0/*selectedDiodes[i]*/);
				}

				//=================
				// Draw Time Course
				//=================
				if (timeCourseFlag)
				{
					drawTimeCourse(0,i);							//activated for ROIs
				}

				//=====================
				// Draw Reference
				//=====================
				if (showRef)
				{
					drawRef(0/*selectedDiodes[i]*/);
				}
			}
			fl_pop_matrix();
		}
		drawValue(numRegions);
	}

	if(sp->getBLCType()=='A')
	{
		drawClampLine();
	}

	if(showPointLine)
	{
		drawPointLine();
	}

	if(showFileLine)
	{
		drawFileLine();
	}

	drawScale();
}

//=============================================================================
void TraceWindow::drawTrace(int dataNo, int currentRegion)
{
	if (currentRegion != -1) {
		int* selectedDiodesAverage = aw->getSelectedDiodesAverage(currentRegion);
		dataNo = selectedDiodesAverage[currentRegion];
	}
	int numPts=dc->getNumPts();
	double pixPts=double(w())/double(numPts);	// Pixels between two points

	// Push The Current Matrix
	fl_push_matrix();
	{
		// Translation & Scaling

		if (dataArray->getFpFlag(dataNo) && currentRegion == -1)		// Field Potential? && currentRegion added to correct bug in scaling of ROI traces 9/2/2020
		{
			fl_scale(xScale,-fpYScale);
		}
		else
		{
			fl_scale(xScale,-yScale);
		}

		fl_translate(xShift,0);

		// Draw Saved Traces
		if(showSavedTraces)
		{
			fl_color(200,200,200);

			double *savData=dataArray->getSavDataMem(dataNo);

			fl_begin_line();
				for(int i=0;i<numPts;i++)
				{
					fl_vertex(i*pixPts,savData[i]);
				}
			fl_end_line();
		}

		// Draw Processed Traces
		if (currentRegion != -1)
		{
/*			double value;
			dataArray->aveROIData(currentRegion, aveData);
			for (int i = 0; i < numPts; i++)
			{
				value = aveData[i];
				fl_vertex(i*pixPts, value);
			}
			fl_end_line();*/

			int numSelectedDiodesAverage = aw->getNumSelectedDiodesAverage(currentRegion)-1;
			int *selectedDiodesAverage = aw->getSelectedDiodesAverage(currentRegion);
			//dataNo = selectedDiodesAverage[0];
			aw->setColorAsDiode(selectedDiodesAverage[1]);
			double **proData = (double**)malloc(sizeof(double*)*numSelectedDiodesAverage);
			if (proData == NULL) {
				return;
			}
			for (int i = 0; i < numSelectedDiodesAverage; i++) {					//get average diodes
				proData[i] = dataArray->getProDataMem(selectedDiodesAverage[i+1]);
			}
			aveData = new double[numPts];
			fl_begin_line();
			for (int i = 0; i < numPts; i++)
			{
				double sum = 0;
				for (int j = 0; j < numSelectedDiodesAverage; j++) {
					sum += proData[j][i];
				}
				sum = sum / numSelectedDiodesAverage;				// sum is the trace averaged over the selected diodes
				aveData[i] = sum;									//aveData set
				fl_vertex(i*pixPts, sum);
			}
			fl_end_line();
			free(proData);	//don't forget to free aveData
		}
		else {
			aw->setColorAsDiode(dataNo);
			double *proData = dataArray->getProDataMem(dataNo);
			//get average diodes
			fl_begin_line();
			for (int i = 0; i < numPts; i++)
			{
				fl_vertex(i*pixPts, proData[i]);
			}
			fl_end_line();
		}
	}
	fl_pop_matrix();
}

//=============================================================================
void TraceWindow::drawSlope(int dataNo,int region)
{
	if (region != -1) {
		int* selectedDiodesAverage = aw->getSelectedDiodesAverage(region);
		dataNo = selectedDiodesAverage[1];
	}
	int numPts=dc->getNumPts();
	double pixPts=double(w())/double(numPts);	// Pixels between two points

	// Push The Current Matrix
	fl_push_matrix();
	{
		// Translation & Scaling
		if(dataArray->getFpFlag(dataNo))	// Field Potential?
		{
			fl_scale(xScale,-fpYScale);
		}
		else
		{
			fl_scale(xScale,-yScale);
		}

		fl_translate(xShift,0);

		// Draw Slope
		fl_color(200,200,200);
		if (region != -1) {
			int numSelectedDiodesAverage = aw->getNumSelectedDiodesAverage(region) - 1;
			int* selectedDiodesAverage = aw->getSelectedDiodesAverage(region);
			double* slope = (double*)malloc(sizeof(double)*numPts);
			for (int j = 0; j < numPts; j++) {
				double slopeSum = 0;
				for (int i = 1; i <= numSelectedDiodesAverage; i++) {
					double* slopeSingle = dataArray->getSlopeMem(selectedDiodesAverage[i]);
					slopeSum += slopeSingle[j];
				}
				slope[j] = slopeSum;
			}
			fl_begin_line();
			for (int i = 0; i < numPts; i++)
			{
				fl_vertex(i * pixPts, slope[i]);
			}
			fl_end_line();
			free(slope);
		}
		else {
			double* slope = dataArray->getSlopeMem(dataNo);
			fl_begin_line();
			for (int i = 0; i < numPts; i++)
			{
				fl_vertex(i * pixPts, slope[i]);
			}
			fl_end_line();
		}
	}
	fl_pop_matrix();
}

//=============================================================================
void TraceWindow::drawTimeCourse(int dataIndex, int regionIndex)
{
	int i;			// i indexes records
	int numDiodes = aw->getNumSelectedDiodes();
	int numRegions = aw->getNumRegions();
	int numTraces = numDiodes + numRegions;
	if (numTraces < 1)		// If no diode is selected, end and return.
	{
		return;
	}
	// Number of Records
	Data* data;
	data = NULL;
	int numRecord = recControl->getRecordNoMax();

	// Pixels between two points
	double pixPts = double(w()) / double(numRecord - 1);
	double *timeCourse = nullptr;
	double fpYScaleTmp=fpYScale;
	double yScaleTmp=yScale;

	// Get Data
	if (dataIndex > 0) data = dataArray->getData(dataIndex);
	if (regionIndex>=0&&numRegions>0&&dataIndex==0) {
		dataArray->aveROIData(regionIndex);
		aveData = dataArray->getAveData();
		data = dataArray->getROIAve(regionIndex);
		data->measureProperties();
		}
	// Get Data Array
	if(timeCourseType=='R')
	{
		timeCourse=data->getRliArray();
	}
	else if(timeCourseType=='A')		// Amplitude
	{
		timeCourse=data->getAmpArray();
	}
	else if(timeCourseType=='M')
	{
		timeCourse=data->getMaxAmpArray();
//		cout << " line 791 dataindex " << regionIndex << " value " << d2txt(*timeCourse) << endl;
	}
	else if(timeCourseType=='L')	// 30 msec -> 1 mV
	{
		timeCourse=data->getMaxAmpLatencyArray();

		if(!timeCourseNormalizationFlag)
		{
			fpYScaleTmp/=30;
			yScaleTmp/=30;
		}
	}
	else if(timeCourseType=='H')	// 30 msec -> 1 mV
	{
	timeCourse=data->getHalfAmpLatencyArray();

		if(!timeCourseNormalizationFlag)
		{
			fpYScaleTmp/=30;
			yScaleTmp/=30;
		}
	}
	else if (timeCourseType == 'J')	// 30 msec -> 1 mV
	{
		timeCourse = data->getHalfWidthArray();

		if (!timeCourseNormalizationFlag)
		{
			fpYScaleTmp /= 30;
			yScaleTmp /= 30;
		}
	}

	double latencyStart=dataArray->getLatencyStart();
	// Set Color
	if(dataIndex>0) aw->setColorAsDiode(dataIndex);
	if (regionIndex>0) {
		int *selectedDiodesAverage = aw->getSelectedDiodesAverage(regionIndex);
		aw->setColorAsDiode(selectedDiodesAverage[1]);
	}

	// Push The Current Matrix
	fl_push_matrix();
	{
		// Translation & Scaling
		double barLength;
		if (dataIndex < 0)		// fp diode
		{
			fl_scale(ltpXScale, -fpYScaleTmp);
			barLength = 4 / fpYScaleTmp;
		}
		else
		{
			fl_scale(ltpXScale, -yScaleTmp);
			barLength = 4 / yScaleTmp;
		}
		fl_translate(ltpXShift, 0);
		//==========================================================
		// Normalization
		double scale = 1.0;

		if (timeCourseNormalizationFlag)
		{
			if (timeCourseType == 'M')
			{
				scale = 1.0 / dataArray->getDataMaxAmp(dataIndex, 1);
			}
			else
			{
				scale = 1.0 / timeCourse[dataArray->getRecordXNo(1)];
			}
		}
		//==========================================================
		// Draw the trace
		fl_begin_line();
		for (i = 0; i < numRecord; i++)
		{
			fl_vertex(i*pixPts, timeCourse[i] * scale);
		}
		fl_end_line();

		// Draw the bar
		for (i = 0; i < numRecord; i++)
		{
			for (i = 0; i < numRecord; i++)
			{
				fl_begin_line();
				fl_vertex(i*pixPts, timeCourse[i] * scale + barLength);
				fl_vertex(i*pixPts, timeCourse[i] * scale - barLength);
				fl_end_line();
			}
		}
	}
	fl_pop_matrix();
}

//=============================================================================
void TraceWindow::drawFileLine()
{
	int numRecord=recControl->getRecordNoMax();
	double pixPts=double(w())/double(numRecord-1);
	int recordNo=recControl->getRecordNo()-1;

	// Color
	fl_color(50,150,50);

	// Push The Current Matrix
	fl_push_matrix();
	{
		// Translation & Scaling
		fl_scale(ltpXScale,1);
		fl_translate(ltpXShift,0);

		// Line
		fl_begin_line();
			fl_vertex(recordNo*pixPts,0);
			fl_vertex(recordNo*pixPts,h());
		fl_end_line();
	}
	fl_pop_matrix();
}

//=============================================================================
void TraceWindow::drawRef(int dataIndex)
{
	if(timeCourseType=='N')
	{
		return;
	}

	Data *data=dataArray->getData(dataIndex);
	double fpYScaleTmp=fpYScale;
	double yScaleTmp=yScale;
	double ref=0;
	
	if(timeCourseType=='R')
	{
		ref=(data->getRliArray())[dataArray->getRecordXNo(1)];
	}
	else if(timeCourseType=='A')
	{
		ref=(data->getAmpArray())[dataArray->getRecordXNo(1)];
	}
	// Max Amp
	else if(timeCourseType=='M')
	{
		ref=dataArray->getDataMaxAmp(dataIndex,1);
	}
	else if(timeCourseType=='L')	// 30 msec -> 1 mV
	{
		ref=(data->getMaxAmpLatencyArray())[dataArray->getRecordXNo(1)];

		if(!timeCourseNormalizationFlag)
		{
			fpYScaleTmp/=30;
			yScaleTmp/=30;
		}
	}
	else if(timeCourseType=='H')
	{
		ref=(data->getHalfAmpLatencyArray())[dataArray->getRecordXNo(1)];

		if(!timeCourseNormalizationFlag)
		{
			fpYScaleTmp/=30;
			yScaleTmp/=30;
		}
	}
	else if (timeCourseType == 'J')
	{
		ref = (data->getHalfWidthArray())[dataArray->getRecordXNo(1)];

		if (!timeCourseNormalizationFlag)
		{
			fpYScaleTmp /= 30;
			yScaleTmp /= 30;
		}
	}

	// Color
	fl_color(150,150,250);

	// Push The Current Matrix
	fl_push_matrix();
	{
		// Translation & Scaling
		if(dataIndex < 0)		// fp diode
		{
			fl_scale(1,-fpYScaleTmp);
		}
		else
		{
			fl_scale(1,-yScaleTmp);
		}

		// Line
		if(timeCourseNormalizationFlag)
		{
			ref=1;
		}

		fl_begin_line();
		{
			fl_vertex(0,ref);
			fl_vertex(w(),ref);
		}
		fl_end_line();
	}
	fl_pop_matrix();
}

//=============================================================================
void TraceWindow::clear()
{
	fl_color(bg);
	fl_rectf(0,0,w(),h());
}

//=============================================================================
void TraceWindow::drawClampLine()
{
	fl_push_matrix();
	{
		// Translation & Scaling
		fl_scale(xScale,1);
		fl_translate(xShift,0);

		// Draw The Line
		fl_color(100,100,100);
		int numPts=dc->getNumPts();
		double pixPts=double(w())/double(numPts);
		int clampPt=sp->getClampPt();

		fl_begin_line();
		{
			fl_vertex(clampPt*pixPts,0);
			fl_vertex(clampPt*pixPts,h());
		}
		fl_end_line();
	}
	fl_pop_matrix();
}

//=============================================================================
void TraceWindow::drawPointLine()
{
	fl_push_matrix();
	{
		// Translation & Scaling
		fl_scale(xScale,1);
		fl_translate(xShift,0);

		// Draw The Line
		fl_color(100,100,100);
		int numPts=dc->getNumPts();
		double pixPts=double(w())/double(numPts);

		fl_begin_line();
		{
			fl_vertex(pointLinePt*pixPts,0);
			fl_vertex(pointLinePt*pixPts,h());
		}
		fl_end_line();
	}
	fl_pop_matrix();

	// Draw Time
	double time=pointLinePt*dc->getIntPts();
	char timeTxt[64];
	char txtBuf[16];
	strcpy_s(timeTxt, 64,"Point=");
	_gcvt_s(txtBuf, 16, pointLinePt, 5);
	strcat_s(timeTxt, 64,txtBuf);
	strcat_s(timeTxt, 64,"  Time=");
	_gcvt_s(txtBuf, 16, time, 5);
	strcat_s(timeTxt, 64,txtBuf);
	strcat_s(timeTxt, 64," (m sec)");
	drawTxt(timeTxt,fg,bg,5,h()-10);
}

//=============================================================================
void TraceWindow::drawScale()
{
	char txtBuf[32];
	double xScaleBarValue,yScaleBarValue;

	int X=w()-100;
	int Y=600;

	//
	fl_color(bg);
	fl_rectf(X-1,Y-1,w()/10+3,3);				// Horizantal
	fl_rectf(X-1+w()/10,Y-h()/10-1,3,h()/10+3);	// Vertical
	fl_color(fg);
	fl_line(X,Y,X+w()/10,Y);					// Horizantal
	fl_line(X+w()/10,Y,X+w()/10,Y-h()/10);		// Vertical

	//
	int numPts=dc->getNumPts();
	double intPts=dc->getIntPts();	// Temporal interval between two points

	xScaleBarValue=numPts*intPts/xScale/10.0;// 10e3 usec->msec

	_gcvt_s(txtBuf, 32, xScaleBarValue, 3);
	strcat_s(txtBuf," (m sec)");
	drawTxt(txtBuf,fg,bg,X,Y+15);

	// Y Scale
	yScaleBarValue=h()/yScale/10;

	_gcvt_s(txtBuf, yScaleBarValue, 3);
	if(sp->getRliDivFlag())
	{
		strcat_s(txtBuf," (mV/V)");
	}
	else
	{
		strcat_s(txtBuf," (mV)");
	}
	drawTxt(txtBuf,fg,bg,X,Y-h()/10-5);
}

//=============================================================================
void TraceWindow::drawValue(int numRegions)
{
	int numDiodes = aw->getNumSelectedDiodes();
	int numTraces=numDiodes+numRegions;		// Get the number of selected diodes from the array window.
	if(numTraces<1)		// If no diode is selected, end and return.
	{
		return;
	}

	// Set position parameters.
	char* txtBuf = nullptr;
	int height=h()/(numTraces+1);
	int x0,y0;
	int *selectedDiodes=aw->getSelectedDiodes();
	Data* data;
	data = NULL;

	// Go through selected diodes.
	for (int i = 0; i < numTraces; i++)
	{
		if (i<numDiodes) {
			x0 = 10;
			y0 = height * (i + 1);								// sets height of each trace in window
			fl_color(bg);										// draw boxes around region with index
			fl_rectf(x0 - 3, y0 - 22, 35, 15);
			fl_color(fg);
			fl_rect(x0 - 3, y0 - 22, 35, 15);

			aw->setColorAsDiode(selectedDiodes[i]);
//			data = new Data();
			data = dataArray->getData(selectedDiodes[i]);

			fl_font(FL_HELVETICA, 12);
			fl_draw(i2txt(selectedDiodes[i] + 1), x0, y0 - 10);
		}
		if (i>=numDiodes&&i<numTraces) {
			int j = i - numDiodes;								// j is the region number
			int *selectedDiodesAverage = aw->getSelectedDiodesAverage(j);
			x0 = 10;
			y0 = height * (i + 1);
			fl_color(bg);
			fl_rectf(x0 - 3, y0 - 22, 35, 15);
			fl_color(fg);
			fl_rect(x0 - 3, y0 - 22, 35, 15);

			aw->setColorAsDiode(selectedDiodesAverage[1]);
			
			dataArray->aveROIData(j);
			aveData = dataArray->getAveData();
			data = new Data;		//			memcpy(data->getProDataMem(), aveData, sizeof(double) * dc->getNumPts());
			data = dataArray->getROIAve(j);
			data->measureProperties();

			fl_font(FL_HELVETICA, 12);
			fl_draw(i2txt(j + 1), x0, y0 - 10);
		}
		// RLI
		if(valueType=='R') 
		{
			txtBuf=d2txt(data->getRli());
		}
		// Max Amp
		else if(valueType=='M')	
		{
			txtBuf=d2txt(data->getMaxAmp());
		}
		// Max Amp Change
		else if(valueType=='5')	
		{
			txtBuf=d2txt(dataArray->getDiodeMaxAmpCha(selectedDiodes[i]));
		}
		// Max Amp Change Standard Error
		else if(valueType=='6')	
		{
			txtBuf=d2txt(dataArray->getDiodeMaxAmpChaSD(selectedDiodes[i]));
		}
		// Max Amp Percentage Change
		else if(valueType=='7')	
		{
			txtBuf=d2txt(dataArray->getDiodeMaxAmpPerCha(selectedDiodes[i]));
		}
		// Max Slope latency
		else if(valueType=='8')	
		{
			txtBuf=d2txt(data->getMaxSlopeLatency());
		}

		// Max Slope latency
		else if (valueType == '8')
		{
			txtBuf = d2txt(data->getMaxSlopeLatency());
		}

		//Max Slope Latency
		else if (valueType == 'p')
		{
			txtBuf = d2txt(data->getHalfAmpLatency());
		}
		// Signal-to-noise
		else if(valueType=='Q')	
		{
			txtBuf = d2txt(data->getMaxAmp()/data->getSD());
		}
		// Max Amp Latency
		else if(valueType=='L')	
		{
			txtBuf=d2txt(data->getMaxAmpLatency());
		}
		// Amp at Point Line
		else if(valueType=='A')	
		{
			txtBuf=d2txt(data->getAmp(pointLinePt));
		}
		// Standard Deviation
		else if(valueType=='S')	
		{
			txtBuf=d2txt(data->getSD());
		}
		// Half Amp Latency
		else if(valueType=='H')	
		{
			txtBuf=d2txt(data->getHalfAmpLatency());
		}

		// Width at half-height    **NEW**
		else if (valueType == 'J')
		{
			txtBuf = d2txt(data->getHalfWidth());
		}

		else if (valueType == 'K')
		{
			txtBuf = d2txt(data->getHalfRiseTime());
		}
		else if (valueType == 'k')
		{
			txtBuf = d2txt(data->getHalfDecayTime());
		}

		if (valueType != 'N')
		{
			drawTxt(txtBuf, fg, bg, x0 + 40, y0 - 10);
		}
	}
}

//=============================================================================
void TraceWindow::drawTxt(char *txtBuf,Fl_Color fg,Fl_Color bg,int x,int y)
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
void TraceWindow::setShowAlpha(char input)
{
	showAlpha=input;
}

//=============================================================================
void TraceWindow::setFittingVar(int index,double value)
{
	int dataNo=getLastDiodeNo();

	if(dataNo>=0)
	{
		Data* data=dataArray->getData(dataNo);
		data->setFittingVar(index,value);
	}
}

//=============================================================================
void TraceWindow::saveEPSP()
{
	int numSelectedDiodes=aw->getNumSelectedDiodes();

	if(numSelectedDiodes<1)
	{
		return;
	}

	int *selectedDiodes=aw->getSelectedDiodes();
	double* fittingVar=dataArray->getData(selectedDiodes[numSelectedDiodes-1])->getFittingVar();

	// Create file name
	fstream file;
	char fileName[64];
	char txtBuf[16];

	int sliceNo=recControl->getSliceNo();
	int locationNo=recControl->getLocationNo();
	int recordNo=recControl->getRecordNo();

	_itoa_s(sliceNo,txtBuf,10);
	strcpy_s(fileName,txtBuf);
	strcat_s(fileName,"-");

	_itoa_s(locationNo,txtBuf,10);
	strcat_s(fileName,txtBuf);
	strcat_s(fileName,"-EPSP.dat");

	// Check the existance of the file
	char fExist=(_access(fileName,0) != -1);

	// Open file
	file.open(fileName,ios::out|ios::app);

	// Write title
	if(!fExist)
	{
		file<<"RecordNumber\tDiodeNumber\tEPSPAmp\tSpikeAmp_Vm\t";
		file<<"Tau\tCm/Gpas\tGsyn/Gpas\n";
	}

	double intPts=dc->getIntPts();

	// Write data
	file<<"R"<<recordNo						// Record number
		<<"\tD"<<selectedDiodes[0]+1<<'\t'	// Diode number
		<<fittingVar[0]<<'\t'				// Amplitude of alpha and Vm
		<<fittingVar[1]<<'\t'				// Spike amplitude proData[i]-Vm[i]
		<<fittingVar[2]*intPts<<'\t'		// Tau for alpha
		<<fittingVar[3]<<'\t'				// Cm/Gpas
		<<fittingVar[4]<<'\n';				// Gsyn/Gpas

	// Close file
	file.close();
}

//=============================================================================
void TraceWindow::drawVm(int dataNo)
{
	int i;
	int numPts=dc->getNumPts();
	double pixPts=double(w())/double(numPts);
	double *proData=dataArray->getProDataMem(dataNo);

	setFittingMemory();
	calVm(dataNo);
	setFittingGadgets(dataNo);

	//=================
	// Push The Current Matrix
	//=================
	fl_push_matrix();
	{
		//=================
		// Translation & Scaling
		//=================
		if(dataArray->getFpFlag(dataNo))	// Field Potential?
		{
			fl_scale(xScale,-fpYScale);
		}
		else
		{
			fl_scale(xScale,-yScale);
		}
		
		fl_translate(xShift,0);

		//=================
		// Draw alpha function
		//=================
		if(showAlpha)
		{
			fl_color(200,200,200);

			fl_begin_line();
				for(i=0;i<numPts;i++)
				{
					fl_vertex(i*pixPts,alpha[i]);
				}
			fl_end_line();

			//=================
			// Draw spike line
			//=================
			i=dataArray->getDataMaxAmpLatencyPt(dataNo);
			if(i<0 || i>=numPts)
			{
				i=0;
			}

			fl_color(255,255,0);

			fl_begin_line();
					fl_vertex(i*pixPts,proData[i]);
					fl_vertex(i*pixPts,alpha[i]);
			fl_end_line();
		}

		//=================
		// Draw Vm
		//=================
		if(showVm)
		{
			fl_color(50,255,50);

			fl_begin_line();
				for(i=0;i<numPts;i++)
				{
					fl_vertex(i*pixPts,Vm[i]);
				}
			fl_end_line();

			//=================
			// Draw Spike Line
			//=================
			i=dataArray->getDataMaxAmpLatencyPt(dataNo);
			if(i<0 || i>=numPts)
			{
				i=0;
			}

			fl_color(0,0,255);
			fl_begin_line();
					fl_vertex(i*pixPts,proData[i]);
					fl_vertex(i*pixPts,Vm[i]);
			fl_end_line();

			fl_color(255,255,255);
			fl_begin_line();
					fl_vertex(i*pixPts,0);
					fl_vertex(i*pixPts,Vm[i]);
			fl_end_line();

			char buf[32];
			spikeAmp=proData[i]-Vm[i];
			sprintf_s(buf, 32,"Spike Amp : %g",spikeAmp);

			fl_font(FL_HELVETICA,12);
			fl_draw(buf,5,h()-25);
		}
	}
	fl_pop_matrix();
}

//=============================================================================
void TraceWindow::calVm(int dataNo)
{
	int i;

	double intPts=dc->getIntPts();	// Temporal interval between two points
	int numPts=dc->getNumPts();
	int window=atoi(ui->alphaWindow->value());

	Data* data=dataArray->getData(dataNo);
	double* fittingVar=data->getFittingVar();

	double alphaStartPoint=convertFittingVar(0,fittingVar[0]);
	double alphaAmp=convertFittingVar(1,fittingVar[1]);
	double alphaTau=convertFittingVar(2,fittingVar[2]);
	double R2=convertFittingVar(3,fittingVar[3]);
	double R1=convertFittingVar(4,fittingVar[4]);

	int end=numPts;

	if(!calWholeVm)
	{
		if((int)(alphaStartPoint+window+1) < numPts)
		{
			end=(int) (alphaStartPoint+window+1);
		}
	}

	//=================
	// Calculate alpha function
	//=================
	double maxAlpha=1.0e-10;

	for(i=0;i<numPts;i++)
	{
		alpha[i]=0;
	}

	// Calculate Alpha
	for(i=(int)alphaStartPoint;i<end;i++)
	{
		alpha[i]=(i-alphaStartPoint)/alphaTau*exp(-(i-alphaStartPoint)/alphaTau);

		if(alpha[i] > maxAlpha)
		{
			maxAlpha=alpha[i];
		}
	}

	// Normalize Alpha
	for(i=(int)alphaStartPoint;i<end;i++)
	{
		alpha[i]=alphaAmp*alpha[i]/maxAlpha;
	}

	//=================
	// Calculate Vm
	//=================
	double Vrest=-0.07;	// resting membrane potential
	double V0=Vrest;	// membrane potential (t-dt)
	double Vequ;	// target membrane potential
	double tau;	// time constant for RC circuit
	double Alpha2;	// alpha(t+dt/2)
	double maxVm=-10000;

	for(i=0;i<numPts;i++)
	{
		Vm[i]=Vrest;
	}

	// Calculate Vm
	for(i=(int)alphaStartPoint+1;i<end;i++)
	{
		Alpha2=(alpha[i]+alpha[i-1])/2.0;
		
		Vequ=Vrest/(1+R1*Alpha2);
		tau=R2/(1+R1*Alpha2);
		Vm[i]=Vequ+(V0-Vequ)*exp(-intPts*(1.0e-3)/tau);
		V0=Vm[i];

		if(Vm[i]>maxVm)
		{
			maxVm=Vm[i];
		}
	}

	// Normalize Vm
	for(i=0;i<numPts;i++)
	{
		Vm[i]=alphaAmp*(Vm[i]-Vrest)/(maxVm-Vrest);
	}

	// Set spike amp
	int maxAmpPt=data->getMaxAmpLatencyPt();
	double* proData=data->getProDataMem();

	if(maxAmpPt<0)
	{
		maxAmpPt=0;
	}

	data->setSpikeAmp(proData[maxAmpPt]-Vm[maxAmpPt]);
}

//=============================================================================
void TraceWindow::setFittingGadgets(int dataNo)
{
	if(dataNo < -NUM_FP_DIODES || dataNo >= dataArray->num_raw_array_diodes())
	{
		return;
	}

	Data* data=dataArray->getData(dataNo);
	double* fittingVar=data->getFittingVar();

	ui->alphaStartPointRoller->value(fittingVar[0]);
	ui->alphaStartPointRoller->do_callback();

	ui->alphaAmplitudeSlider->value(fittingVar[1]);
	ui->alphaAmplitudeSlider->do_callback();

	ui->alphaTauSlider->value(fittingVar[2]);
	ui->alphaTauSlider->do_callback();

	ui->R2Slider->value(fittingVar[3]);
	ui->R2Slider->do_callback();

	ui->R1Slider->value(fittingVar[4]);
	ui->R1Slider->do_callback();

	char buf[16];
	_itoa_s(data->getAlphaSpikeStart(), buf, 10);
	ui->alphaSpikeStart->value(buf);
	_itoa_s(data->getAlphaSpikeEnd(), buf, 10);
	ui->alphaSpikeEnd->value(buf);
}

//=============================================================================
void TraceWindow::setShowVm(char input)
{
	showVm=input;
}

//=============================================================================
void TraceWindow::saveAlphaVm()
{
	// Check the status
	if(aw->getNumSelectedDiodes()<1 || !(showAlpha||showVm))
	{
		return;
	}

	// Start here
	int i;
	int numPts=dc->getNumPts();

	// Open the output file
	fstream file;
	file.open("Alpha_Vm_Traces.dat",ios::out);
	
	// Write the title
	file<<"Pt\tAlpha\tVm\n";

	// Write the data
	for(i=0;i<numPts;i++)
	{
		file<<i<<'\t'<<alpha[i]<<'\t'<<Vm[i]<<'\n';
	}

	// Close the file
	file.close();
}

//=============================================================================
double TraceWindow::calFitError(int dataNo)
{
	int i;

	// Get data address
	Data* data=dataArray->getData(dataNo);
	double *proData=data->getProDataMem();
	double *fittingVar=data->getFittingVar();
	double maxAmp=data->getMaxAmp();

	// Fitting window width
	int width=atoi(ui->alphaWindow->value());

	// Initialize error
	double error=0;

	// Start point
	int alphaStartPt=(int) fittingVar[0];

	// Ignore axon signal
	int axonStart=atoi(ui->alphaAxonStart->value());
	int axonEnd=atoi(ui->alphaAxonEnd->value());

	// Ignore spike
	int spikeStart=data->getAlphaSpikeStart();
	int spikeEnd=data->getAlphaSpikeEnd();
	double maxSlope=data->getMaxSlope();
	
	// Main loop
	for(i=alphaStartPt-10;i<=(alphaStartPt+width);i++)
	{
		if(i>=axonStart && i<=axonEnd)
		{
			continue;
		}

		if(maxSlope>spikeSlopeThreshold)
		{
			if(i>=spikeStart && i<=spikeEnd)
			{
				continue;
			}
		}

		if(maxAmp>spikeAmpThreshold)
		{
			if(i>=spikeStart && i<=spikeEnd)
			{
				continue;
			}
		}

		error+=(proData[i]-Vm[i])*(proData[i]-Vm[i]);
	}

	// Return error
	return error;
}

//=============================================================================
void TraceWindow::setSave2FileType(char type)
{
	save2FileType=type;
}

//=============================================================================
void TraceWindow::setSaveTracesPt(char tag,const char* txt)
{
	int pt=atoi(txt);

	if(tag=='s')
	{
		saveTracesStartPt=pt;
	}
	else if(tag=='e')
	{
		saveTracesEndPt=pt;
	}
}

//=============================================================================
void TraceWindow::setChangeBits(int index,bool input)
{
	changeBits[index]=input;
}

//=============================================================================
bool TraceWindow::getChangeBits(int index)
{
	return changeBits[index];
}

//=============================================================================
void TraceWindow::setHandleType(char type)
{
	handleType=type;
}

//=============================================================================
int TraceWindow::handle(int event)
{
	static int x0,y0,xShift0,ltpXShift0;
	static double xScale0,ltpXScale0,yScale0,fpYScale0;
	int x1,y1;
	int yH;
	
	if(superimpose)
	{
		yH=h()/2;
	}
	else
	{
		yH=h()-h()/(aw->getNumSelectedDiodes()+1);	// Baseline of the last trace
	}

	switch(event)
	{
		// Push =========================================
		case FL_PUSH:

			x0=Fl::event_x();
			y0=Fl::event_y();

			if(y0==yH)
			{
				y0=yH+1;
			}

			xScale0=xScale;
			ltpXScale0=ltpXScale;
			yScale0=yScale;
			fpYScale0=fpYScale;

			xShift0= (int) xShift;
			ltpXShift0= (int) ltpXShift;

			if(Fl::event_button()==2)
			{
				int numPts=dc->getNumPts();

				pointLinePt=(int) ((x0/xScale-xShift)*numPts/w()+0.5);

				if(pointLinePt<0)
				{
					pointLinePt=0;
				}
				else if(pointLinePt>=numPts)
				{
					pointLinePt=numPts-1;
				}

				dataArray->measureAmp();
				redraw();
			}
			
			if(Fl::event_button()==3)
			{
				if(Fl::event_key(FL_Shift_L))
				{
					if(handleType=='o')		// Optical signal
					{
						xScale=1;
						xShift=0;
					}
					else if(handleType=='f')	// Field potential
					{
						xScale=1;
						xShift=0;
					}
					else if(handleType=='l')	// LTP
					{
						ltpXScale=1;
						ltpXShift=0;
					}

					redraw();
				}
			}
			
			return 1;

		// Drag =========================================
		case FL_DRAG:
			x1=Fl::event_x();
			y1=Fl::event_y();

			// Left button
			if(Fl::event_button()==1)
			{
				if(handleType=='o')		// Optical signal
				{
					xScale=xScale0*x1/x0;
					yScale=yScale0*(y1-yH)/(y0-yH);
				}
				else if(handleType=='f')	// Field potential
				{
					xScale=xScale0*x1/x0;
					fpYScale=fpYScale0*(y1-yH)/(y0-yH);
				}
				else if(handleType=='l')	// LTP
				{
					ltpXScale=ltpXScale0*x1/x0;
					yScale=yScale0*(y1-yH)/(y0-yH);
				}
			}
			// Middle button
			else if(Fl::event_button()==2)
			{
				int numPts=dc->getNumPts();

				pointLinePt=(int) ((x1/xScale-xShift)*numPts/w()+0.5);

				if(pointLinePt<0)
				{
					pointLinePt=0;
				}
				else if(pointLinePt>=numPts)
				{
					pointLinePt=numPts-1;
				}

				dataArray->measureAmp();
			}
			// Right button
			else if(Fl::event_button()==3)
			{
				if(handleType=='o')
				{
					xShift=xShift0+(x1-x0)/xScale;
				}
				else if(handleType=='f')
				{
					xShift=xShift0+(x1-x0)/xScale;
				}
				else if(handleType=='l')
				{
					ltpXShift=ltpXShift0+(x1-x0)/ltpXScale;
				}
			}

			redraw();

			return 1;

		// Lease =========================================
		case FL_RELEASE:
			return 1;

		// Default =========================================
		default:
			return Fl_Double_Window::handle(event);
	}
}

//=============================================================================
double TraceWindow::convertFittingVar(int index,double value)
{
	if(index==0)	// Start Point
	{
		return value;
	}
	else if(index==1)	// Amp
	{
		return 0.3*pow(1.02,value);
	}
	else if(index==2)	// Tau
	{
		return 4.0*pow(1.02,value);
	}
	else if(index==3)	// R2
	{
		return 0.01*pow(1.05,value);
	}
	else if(index==4)	// R1
	{
		return 1.0*pow(1.05,value);
	}
	
	return -1;
}

//=============================================================================
double TraceWindow::revConvertFittingVar(int index,double value)
{
	if(index==0)	// Start Point
	{
		return value;
	}
	else if(index==1)	// Amp
	{
		if(value>0)
		{
			return (log(value)-log(0.3))/log(1.02);
		}
		else
		{
			return -100;
		}
	}
	else if(index==2)	// Tau
	{
		return 4.0*pow(1.02,value);
	}
	else if(index==3)	// R2
	{
		return 0.01*pow(1.05,value);
	}
	else if(index==4)	// R1
	{
		return 1.0*pow(1.05,value);
	}
	
	return -1;
}

//=============================================================================
double TraceWindow::fitVm(int dataNo)
{
	Data* data=dataArray->getData(dataNo);
	double* fittingVar0=data->getFittingVar();
	double fittingVar[5];

	int i,step[5];
	int min[5];
	double minError=1000;
	double error;

	int fittingRange=(int) ui->fittingRange->value();
	int fittingStepSize=(int) ui->fittingStepSize->value();

	int low[5];
	int high[5];

	// Set the range and initial value
	for(i=0;i<5;i++)
	{
		fittingVar[i]=fittingVar0[i];
		low[i]=-fittingRange*fittingStepSize;
		high[i]=fittingRange*fittingStepSize;
	}

	// Set the range
	if(getChangeBits(0))//
	{
		if(fittingVar[0]<onsetBounds[0])
		{
			low[0]=0;
			high[0]=1;
		}
		else if(fittingVar[0]>onsetBounds[1])
		{
			low[0]=-1;
			high[0]=0;
		}
		else
		{
			low[0]=-1;
			high[0]=1;
		}
	}
	else
	{
		low[0]=high[0]=0;
	}
	
	for(i=1;i<5;i++)//
	{
		if(getChangeBits(i))
		{
			if(fittingVar[i]<=-100)
			{
				low[i]=0;
			}
			else if(fittingVar[i]>=100)
			{
				high[i]=0;
			}
		}
		else
		{
			low[i]=high[i]=0;
		}
	}

	// Set memory for alpha and Vm
	setFittingMemory();

	// Main loops
	for(step[0]=low[0];step[0]<=high[0];step[0]++)	// Start point
	{
		for(step[1]=low[1];step[1]<=high[1];step[1]+=fittingStepSize)
		{
			for(step[2]=low[2];step[2]<=high[2];step[2]+=fittingStepSize)
			{
				for(step[3]=low[3];step[3]<=high[3];step[3]+=fittingStepSize)
				{
					for(step[4]=low[4];step[4]<=high[4];step[4]+=fittingStepSize)
					{
						for(i=0;i<5;i++)
						{
							if(tw->getChangeBits(i))
							{
								data->setFittingVar(i,fittingVar[i]+step[i]);
							}
						}

						// Calculate Vm
						tw->calVm(dataNo);

						// Calculate Error
						error=tw->calFitError(dataNo);

						// Check for minimum
						if(error<minError)
						{
							minError=error;

							for(i=0;i<5;i++)
							{
								min[i]=(int) (fittingVar[i]+step[i]);
							}
						}
					}
				}
			}
		}
	}

	// Save variables
	for(i=0;i<5;i++)
	{
		fittingVar0[i]=min[i];
	}

	// Return minimum value
	return minError;
}

//=============================================================================
int TraceWindow::getLastDiodeNo()
{
	int numSelectedDiodes=aw->getNumSelectedDiodes();

	if(numSelectedDiodes>0)
	{
		int *selectedDiodes=aw->getSelectedDiodes();
		return selectedDiodes[numSelectedDiodes-1];
	}
	else
	{
		return -NUM_FP_DIODES - 1;
	}
}

//=============================================================================
void TraceWindow::setFittingMemory()
{
	int numPts=dc->getNumPts();

	delete [] alpha;
	alpha=new double[numPts];

	delete [] Vm;
	Vm=new double[numPts];
}

//=============================================================================
void TraceWindow::setCalWholeVm(bool input)
{
	calWholeVm=input;
}

//=============================================================================
void TraceWindow::setSpikeSlopeThreshold(double input)
{
	spikeSlopeThreshold=input;
}

//=============================================================================
void TraceWindow::setSpikeAmpThreshold(double input)
{
	spikeAmpThreshold=input;
}

//=============================================================================
void TraceWindow::saveFittingVar()
{
	//==================================================
	// Get file name
	char *fileName=fl_file_chooser("Please enter the name of the file",
		"*.fit","Data.fit");

	if(!fileName)
	{
		return;
	}

	//==================================================
	// Open file
	fstream file;
	file.open(fileName,ios::out);

	//
	int i,j;
	Data* data;
	double* fittingVar;

	file << dataArray->binned_width() << '\n';
	file << dataArray->binned_height() << '\n';

	for(i=0;i< dataArray->num_binned_diodes();i++)
	{
		data=dataArray->getData(i);
		fittingVar=data->getFittingVar();

		file<<i+1;

		for(j=0;j<5;j++)
		{
			file<<'\t'<<fittingVar[j];
		}

		file<<'\n';
	}

	//==================================================
	// Open file
	file.close();
}

//=============================================================================
void TraceWindow::loadFittingVar()
{
	//==================================================
	// Get file name
	char *fileName=fl_file_chooser("Please enter the name of the file",
		"*.fit","Data.fit");

	if(!fileName)
	{
		return;
	}

	//==================================================
	// Open file
	fstream file;
	file.open(fileName,ios::in);

	//
	int i,j;
	Data* data;
	double* fittingVar;
	int diodeNo;
	int bwidth, bheight, curr_bwidth, curr_bheight;

	curr_bwidth = dataArray->binned_width();
	curr_bheight = dataArray->binned_height();
	file >> bwidth;
	file >> bheight;

	if (bwidth != curr_bwidth || bheight != curr_bheight)
	{
		char msg[128];
		sprintf(msg, "Fitting file is for array size %dx%d, not %dx%d!\n",
			bwidth, bheight, curr_bwidth, curr_bheight);
		fl_alert(msg);
		file.close();
		return;
	}

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		data=dataArray->getData(i);
		fittingVar=data->getFittingVar();

		file>>diodeNo;

		for(j=0;j<5;j++)
		{
			file>>fittingVar[j];
		}
	}

	//==================================================
	// Open file
	file.close();

	//
	diodeNo=tw->getLastDiodeNo();
	tw->setFittingGadgets(diodeNo);
	aw->redraw();
}

//=============================================================================
void TraceWindow::setOnsetBounds(int index,const char* txt)
{
	float bound=atof(txt);
	int numPts=dc->getNumPts();

	if(bound<0)
	{
		bound=0;
	}
	else if(bound>=numPts)
	{
		bound=numPts-1;
	}

	onsetBounds[index]=bound;
}

//=============================================================================
