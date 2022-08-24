//=============================================================================
// MainControllerArrayWindow.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()
#include <fstream>
#include <math.h> 
#include <FL/fl_file_chooser.h>
#include <iostream>

#include "MainController.h"
#include "ArrayWindow.h"
#include "UserInterface.h"
#include "DataArray.h"
#include "ColorWindow.h"
#include "Color.h"
#include "Definitions.h"

using namespace std;

char* i2txt(int);
char* d2txt(double);

//=============================================================================
void MainController::setAwShowTrace(char p)
{
	/*if (aw->getBackground() == BG_Live_Feed&&!(ui->lfRun->value()))
	{
		int choice = fl_choice("The file already exists! Please choose one of following:", "Cancel",
			"Save as ", "Overwrite");
		
		fl_alert("Doing this w.");
		return;
	}*/
	aw->setShowTrace(p);
	aw->redraw();
}

//=============================================================================
void MainController::setAwShowRliValue(char p)
{

		/*if (aw->getBackground() == BG_Live_Feed && !(ui->lfRun->value()))
		{
			fl_alert("Please toggle or change this during or before live feed.");
			return;
		}*/
	aw->setShowRliValue(p);
	aw->redraw();
}

//=============================================================================
void MainController::setAwShowDiodeNum(char p)
{
	/*if (aw->getBackground() == BG_Live_Feed && !(ui->lfRun->value))
	{
		fl_alert("Please toggle or change this during or before live feed.");
		return;
	}*/
	aw->setShowDiodeNum(p);
	aw->redraw();
}

//=============================================================================
void MainController::setAwYScale(double p)
{	
	/*if (aw->getBackground() == BG_Live_Feed && !(ui->lfRun->value))
	{
		fl_alert("Please toggle or change this during or before live feed.");
		return;
	}*/
	if (p >=0) {
		p = 0.1*pow(p,4); //changed 1 to 4 to increase range of scale in arraw window
	}
	else {
		p = -1*0.1*pow(p,4);
	}
	aw->setYScale2(p);

	if(aw->getShowTrace())
	{
		aw->redraw();
	}

	ui->awYScaleTxt->value(d2txt(p));
}

//=============================================================================
void MainController::setAwFpYScale(double p)
{
	/*if (aw->getBackground() == BG_Live_Feed && !(ui->lfRun->value))
	{
		fl_alert("Please toggle or change or change this during or before live feed.");
		return;
	}*/
	if (p >= 0) {
		p = 0.1*pow(p, 1);
	}
	else {
		p = -1 *0.1* pow(p, 1);
	}
	aw->setYScale(p);

	if(aw->getShowTrace())
	{
		aw->redraw();
	}

	ui->awFpYScaleTxt->value(d2txt(p));
}

//=============================================================================
void MainController::setAwXScale(double p)
{
	/*if (aw->getBackground() == BG_Live_Feed && !(ui->lfRun->value))
	{
		fl_alert("Please toggle or change or change this during or before live feed.");
		return;
	}*/
	aw->setXScale(p);

	if(aw->getShowTrace())
	{
		aw->redraw();
	}

	ui->awXScaleTxt->value(d2txt(p));
}

//=============================================================================
void MainController::setAwXShift(double p)
{
	/*if (aw->getBackground() == BG_Live_Feed && !(ui->lfRun->value))
	{
		fl_alert("Please toggle or change or change this during or before live feed.");
		return;
	}*/
	aw->setXShift(p);

	if(aw->getShowTrace())
	{
		aw->redraw();
	}

	ui->awXShiftTxt->value(d2txt(p));
}

//=============================================================================
void MainController::setAwBackground(int p)
{
	aw->setBackground(p);
	aw->redraw();
}

//=============================================================================
// Image
//=============================================================================
void MainController::loadAwImage()
{
	aw->loadImage();
	aw->redraw();
}

//=============================================================================
void MainController::setAwImageX0(int value)
{
	aw->setImageX0(value);

	ui->awImageX0Txt->value(i2txt(value));

	aw->redraw();
}

//=============================================================================
void MainController::setAwImageY0(int value)
{
	aw->setImageY0(value);

	ui->awImageY0Txt->value(i2txt(value));

	aw->redraw();
}

//=============================================================================
void MainController::setAwImageXSize(int size)
{
	aw->setImageXSize(size);
	aw->redraw();
}

//=============================================================================
void MainController::setAwImageYSize(int size)
{
	aw->setImageYSize(size);
	aw->redraw();
}

//=============================================================================
void MainController::saveSelected()		// repurpose to save ROIs. It previously saved diodes with no ROI selection.
{
	int numROIs = aw->getNumRegions();
	if(numROIs<=0)
	{
		printf_s(" \a");
		fl_alert(" no ROIs selected ");
		return;
	}
	
	// Ask for the file name
	char *fileName=fl_file_chooser("Enter the name of the Selected ROI file to save","*.dat","ROIs-.dat");
		if(!fileName)
	{
		return;
	}
	// Open the file and save values
	int i;
	fstream file;
	file.open(fileName,ios::out);
	file << numROIs<< "\n";
	for (int RegionIndex = 0; RegionIndex < numROIs; RegionIndex++)
	{
		int* ROIList = aw->getSelectedDiodesAverage(RegionIndex);
		file << RegionIndex << "\n";
		int RegionSize = aw->getNumSelectedDiodesAverage(RegionIndex);
		file << RegionSize << "\n";
			for (i = 0; i < RegionSize; i++)
			{
				file << ROIList[i] << '\n';
			}
	}
	file.close();
}

//=============================================================================
void MainController::loadSelected()		// repurposed to load saved ROIs
{
	// Ask for the file name
	char *fileName = fl_file_chooser("Please select one .dat file", "*.dat", "Diodes.dat");
	if (!fileName)
	{
		return;
	}
	int i;
	int j;
	int numROIs;
	int RegionSize;
	int RegionIndex;

	aw->clearSelected(0);
	
	fstream file;
	file.open(fileName, ios::in);
	file >> numROIs;
		
	for (int i = 0; i < numROIs; i++)
	{
		int diodeIndex;
		file >> RegionIndex;
		file >> RegionSize;
		aw->setContinuous(1);
		for (j = 0; j < RegionSize; j++)
		{
			file >> diodeIndex;
			if (j > 0)
			{
				cout << "mcaw line 267 regionindex " << RegionIndex << " pixel " << diodeIndex << endl;
				aw->addToSelectedList(diodeIndex, 1);
			}
		}
		aw->setContinuous(0);
	}
file.close();
}

//=============================================================================
void MainController::ignoreSelectedDiodes()
{
	int i;

	int numDiodes=aw->getNumSelectedDiodes();
	int* index=aw->getSelectedDiodes();
	for(i=0;i<numDiodes;i++)
	{
		dataArray->setIgnoredFlag(index[i],1);
	}

	dataArray->process();
	redraw();
}

void MainController::setContinuous(int continuous)
{
	aw->setContinuous(continuous);
}
//=============================================================================
