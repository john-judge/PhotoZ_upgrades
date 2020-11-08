//=============================================================================
// MainControllerLtp.cpp
//=============================================================================
#include <stdlib.h>
#include <iostream>

#include "MainController.h"
#include "RecControl.h"
#include "FileController.h"
#include "UserInterface.h"
#include "DataArray.h"
#include "Definitions.h"

using namespace std;
//=============================================================================
void MainController::timeCourseMeasurement()
{
	char success=1;
	char* fileName;
	recControl->setRecordNo(1);
	while(success)
	{
		fileName=recControl->createFileName();
		success=fileController->openFileByName(fileName);
		dataArray->process();
		recControl->increaseNo(RECORD);
	}

	recControl->decreaseNo(RECORD);
	ui->setValue();	// Checked
	redraw();
}

//=============================================================================
void MainController::setRecordXNo(int recordX,const char* recordXNoTxt)
{
	int maxRecordNo=recControl->getRecordNoMax();
	int recordXNo=atoi(recordXNoTxt);
	recordXNo--;

	if(recordXNo<0)
	{
		recordXNo=0;

		if(recordX==1)
		{
			ui->record1No->value("1");
		}
		else if(recordX==2)
		{
			ui->record2No->value("1");
		}
		else if(recordX==3)
		{
			ui->record2No->value("1");
		}
	}
	else if(recordXNo>=maxRecordNo)
	{
		recordXNo=maxRecordNo-1;

		if(recordX==1)
		{
			ui->record1No->value(i2txt(recordXNo+1));
		}
		else if(recordX==2)
		{
			ui->record2No->value(i2txt(recordXNo+1));
		}
		else if(recordX==3)
		{
			ui->record2No->value(i2txt(recordXNo+1));
		}
	}

	dataArray->setRecordXNo(recordX,recordXNo);

	redraw();
}

//=============================================================================
void MainController::setCompareFlag(char input)
{
	dataArray->setCompareFlag(input);
	redraw();
}

//=============================================================================
void MainController::setNormalizationFlag(char input)
{
	dataArray->setNor2ArrayMaxFlag(input);
	redraw();
}

//=============================================================================
void MainController::setAverageFlag(char input)
{
	dataArray->setAverageFlag(input);
	redraw();
}

//=============================================================================
void MainController::setIncreaseFlag(char input)
{
	dataArray->setIncreaseFlag(input);
	redraw();
}

//=============================================================================
void MainController::setAbsFlag(char input)
{
	dataArray->setAbsFlag(input);
	redraw();
}

//=============================================================================
