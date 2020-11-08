//=============================================================================
// MainControllerTrace.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()

#include "Definitions.h"
#include "MainController.h"
#include "UserInterface.h"
#include "FileController.h"
#include "SignalProcessor.h"
#include "DataArray.h"
#include "DapController.h"

char* i2txt(int);
char* d2txt(double);

//=============================================================================
void MainController::saveTraces2()
{
	// Save Traces to Buffer
	dataArray->saveTraces2();

	// Copy File Name
	char *fileName=fileController->getFileName();
	ui->secondFileName->value(fileName);

	// Copy Trial No
	ui->savedTracesTrialNoTxt->value(i2txt(recControl->getTrialNo()));

	// Redraw
	if(tw->getShowSavedTraces())
	{
		tw->redraw();
	}
}

//=============================================================================
void MainController::setTwValueType(char p)
{
	tw->setValueType(p);
	tw->redraw();
}

//=============================================================================
void MainController::setTwTimeCourseType(char p)
{
	tw->setTimeCourseType(p);
	tw->redraw();
}

//=============================================================================
void MainController::setTwTimeCourseFlag(char p)
{
	tw->setTimeCourseFlag(p);
	tw->redraw();
}

//=============================================================================
void MainController::fitVm()
{
	int diodeNo=tw->getLastDiodeNo();

	if(diodeNo<-NUM_FP_DIODES)
	{
		return;
	}
	else
	{
		tw->fitVm(diodeNo);
		tw->setFittingGadgets(diodeNo);
		aw->redraw();
	}
}

//=============================================================================
void MainController::fitVmX10()
{
	int diodeNo=tw->getLastDiodeNo();

	if(diodeNo<-NUM_FP_DIODES)
	{
		return;
	}

	//
	int i;

	double minValue=1000;
	double error;

	for(i=0;i<10;i++)
	{
		error=tw->fitVm(diodeNo);

		if(error==minValue)
		{
			break;
		}
		else
		{
			minValue=error;
		}
	}

	tw->setFittingGadgets(diodeNo);
	aw->redraw();
}

//=============================================================================
void MainController::fitAllDiodes()
{
	int diodeNo;

	double minValue,error;

	for (diodeNo = 0; diodeNo < dataArray->num_binned_diodes(); diodeNo++)
	{
		// Find the minimum
		minValue=1000;
		error=999;

		while(error<minValue)
		{
			minValue=error;
			error=tw->fitVm(diodeNo);
		}

		// Set spike amp
		tw->calVm(diodeNo);
	}

	diodeNo=tw->getLastDiodeNo();
	tw->setFittingGadgets(diodeNo);
	aw->redraw();
}

//=============================================================================
