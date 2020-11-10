//=============================================================================
// MainControllerArrayWindow.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()
#include <fstream>

#include <FL/fl_file_chooser.h>

#include "MainController.h"
#include "ArrayWindow.h"
#include "UserInterface.h"
#include "DataArray.h"
#include "ColorWindow.h"
#include "Definitions.h"

using namespace std;

char* i2txt(int);
char* d2txt(double);

//=============================================================================
void MainController::setAwShowTrace(char p)
{
	aw->setShowTrace(p);
	aw->redraw();
}

//=============================================================================
void MainController::setAwShowRliValue(char p)
{
	aw->setShowRliValue(p);
	aw->redraw();
}

//=============================================================================
void MainController::setAwShowDiodeNum(char p)
{
	aw->setShowDiodeNum(p);
	aw->redraw();
}

//=============================================================================
void MainController::setAwYScale(double p)
{
	aw->setYScale(p);

	if(aw->getShowTrace())
	{
		aw->redraw();
	}

	ui->awYScaleTxt->value(d2txt(p));
}

//=============================================================================
void MainController::setAwFpYScale(double p)
{
	aw->setYScale2(p);

	if(aw->getShowTrace())
	{
		aw->redraw();
	}

	ui->awFpYScaleTxt->value(d2txt(p));
}

//=============================================================================
void MainController::setAwXScale(double p)
{
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
void MainController::saveSelected()
{
	int numSelectedDiodes=aw->getNumSelectedDiodes();
	int* diodeList=aw->getSelectedDiodes();

	if(numSelectedDiodes<=0)
	{
		return;
	}

	// Ask for the file name
	char *fileName=fl_file_chooser(
		"Please enter the name of the SEL file","*.sel","Diodes.sel");

	if(!fileName)
	{
		return;
	}

	// Open the file and save values
	int i;

	fstream file;
	file.open(fileName,ios::out);

	file<<numSelectedDiodes<<"\n";

	for(i=0;i<numSelectedDiodes;i++)
	{
		file<<diodeList[i]+1<<'\n';
	}

	file.close();
}

//=============================================================================
void MainController::loadSelected()
{
	// Ask for the file name
	char *fileName=fl_file_chooser("Please select one SEL file","*.sel","Diodes.sel");

	if(!fileName)
	{
		return;
	}

	// Load values
	int i;
	int numSelectedDiodes;
	int diodeNo;

	aw->clearSelected();

	fstream file;
	file.open(fileName,ios::in);

	file>>numSelectedDiodes;

	for(i=0;i<numSelectedDiodes;i++)
	{
		file>>diodeNo;
		aw->selectDiode(diodeNo-1);
	}

	file.close();

	// Redraw
	redraw();
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

//=============================================================================
