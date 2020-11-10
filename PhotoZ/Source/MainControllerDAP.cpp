//=============================================================================
// MainControllerDAP.cpp
//=============================================================================
#include <stdlib.h>		// atof()
#include <stdio.h>

#include "MainController.h"
#include "UserInterface.h"
#include "DapController.h"
#include "DapChannel.h"
#include "DataArray.h"
#include "ColorWindow.h"
#include "Definitions.h"

char* d2txt(double);
char* i2txt(int);

//=============================================================================
void MainController::setResetOnset(const char *txt)
{
	int value=atoi(txt);
	if(value<0) value=0;

	dc->reset->setOnset(value);
	dc->setDuration();
	ui->resetOnset->value(dc->reset->getOnsetTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setResetDuration(const char *txt)
{
	int value=atoi(txt);
	if(value<0) value=0;

	dc->reset->setDuration(value);
	dc->setDuration();
	ui->resetDuration->value(dc->reset->getDurationTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setShutterOnset(const char *txt)
{
	int value=atoi(txt);
	if(value<0) value=0;

	dc->shutter->setOnset(value);
	dc->setDuration();
	ui->shutterOnset->value(dc->shutter->getOnsetTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setShutterDuration(const char *txt)
{
	int value=atoi(txt);
	if(value<0) value=0;

	dc->shutter->setDuration(value);
	dc->setDuration();
	ui->shutterDuration->value(dc->shutter->getDurationTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setStiOnset(int ch, const char *txt)
{
	int value=atoi(txt);
	if(value<0) value=0;

	if(ch==1)
	{
		dc->sti1->setOnset(value);
		ui->sti1Onset->value(dc->sti1->getOnsetTxt());
	}
	else
	{
		dc->sti2->setOnset(value);
		ui->sti2Onset->value(dc->sti2->getOnsetTxt());
	}

	dc->setDuration();
	dw->redraw();
}

//=============================================================================
void MainController::setStiDuration(int ch, const char *txt)
{
	int value=atoi(txt);
	if(value<0) value=0;

	if(ch==1)
	{
		dc->sti1->setDuration(value);
		ui->sti1Duration->value(dc->sti1->getDurationTxt());
	}
	else
	{
		dc->sti2->setDuration(value);
		ui->sti2Duration->value(dc->sti2->getDurationTxt());
	}

	dc->setDuration();
	dw->redraw();
}

//=============================================================================
void MainController::setAcquiOnset(const char *txt)
{
	int value=atoi(txt);
	if(value<0) value=0;

	dc->setAcquiOnset(value);
	dc->setDuration();
	dw->redraw();
}

//=============================================================================
void MainController::setNumPts(const char *txt)
{
	char buff[16];

	int numPts=atoi(txt);
	double intPts=dapControl->getIntPts();

	// Chech the length of duration
	if(numPts<10)
	{
		numPts=10;
	}

	dapControl->setNumPts(numPts);
	dapControl->setDuration();

	dataArray->changeNumPts(numPts);

	// Update userinterface and redraw
	sprintf_s(buff, 16,"%i",numPts);
	ui->numPts->value(buff);

	sprintf_s(buff, 16,"%5.0f",dapControl->getAcquiDuration());
	ui->acquiDuration->value(buff);

	redraw();
}

//=============================================================================
void MainController::setIntPts(const char *txt)
{
	double intPts=atof(txt);
	if(intPts<0.06) intPts=0.06;
	if(intPts>10) intPts=10;

	// Set interval between points
	dapControl->setIntPts(intPts);
	dapControl->setDuration();

	// Redraw DAP window
	dw->redraw();

	// Update user interface
	char buff[16];

	// Interval between points
	sprintf_s(buff, 16,"%4.2f",intPts);
	ui->intPts->value(buff);

	// Acquisition duration
	sprintf_s(buff, 16,"%6.0f",dapControl->getAcquiDuration());
	ui->acquiDuration->value(buff);

	// Sampling rage
	ui->samplingRate->value(dapControl->getSamplingRateTxt());
}

//=============================================================================
void MainController::setNumTrials(const char* txt)
{
	int numTrials=atoi(txt);

	if(numTrials<1)
	{
		numTrials=1;
	}
	else if(numTrials>100)
	{
		numTrials=100;
	}

	recControl->setNumTrials(numTrials);
	dataArray->changeNumTrials(numTrials);

	ui->numTrials->value(i2txt(numTrials));

	redraw();
}

//=============================================================================
void MainController::setNumSkippedTrials(const char* txt)
{
	int numTrials=atoi(txt);

	if(numTrials<0)
	{
		numTrials=0;
	}
	else if(numTrials>100)
	{
		numTrials=100;
	}

	recControl->setNumSkippedTrials(numTrials);
	ui->numSkippedTrials->value(i2txt(numTrials));
}

//=============================================================================
void MainController::setIntTrials(const char* txt)
{
	int sec=atoi(txt);

	if(sec<1)
	{
		sec=1;
	}
	else if(sec>600)
	{
		sec=600;
	}

	recControl->setIntTrials(sec);
	ui->intTrials->value(i2txt(recControl->getIntTrials()));
}

//=============================================================================
void MainController::setAmplifierGain(int p)
{
	switch(p)
	{
		case Decrease_Acqui_Gain:
			recControl->decreaseAcquiGain();
			break;

		case Increase_Acqui_Gain:
			recControl->increaseAcquiGain();
			break;

		case Decrease_RLI_Gain:
			recControl->decreaseRliGain();
			break;

		case Increase_RLI_Gain:
			recControl->increaseRliGain();
			break;
	}
	
	ui->acquiGain->value(i2txt(recControl->getAcquiGain()));
	ui->rliGain->value(i2txt(recControl->getRliGain()));

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setNumPulses(int ch,const char *txt)
{
	int num=atoi(txt);
	if(num<0) num=0;

	dc->setNumPulses(ch,num);

	if(ch==1)
	{
		ui->numPulses1->value(i2txt(dc->getNumPulses(ch)));
	}
	else
	{
		ui->numPulses2->value(i2txt(dc->getNumPulses(ch)));
	}

	dc->setDuration();
	dw->redraw();
}

//=============================================================================
void MainController::setIntBursts(int ch,const char *txt)
{
	int interval=atoi(txt);
	if(interval<0) interval=0;

	dc->setIntBursts(ch,interval);
	dc->setDuration();

	if(ch==1)
	{
		ui->intBursts1->value(i2txt(dc->getIntBursts(ch)));
	}
	else
	{
		ui->intBursts2->value(i2txt(dc->getIntBursts(ch)));
	}

	dw->redraw();
}

//=============================================================================
void MainController::setIntPulses(int ch,const char *txt)
{
	int interval=atoi(txt);
	if(interval<0) interval=0;

	dc->setIntPulses(ch,interval);
	dc->setDuration();

	if(ch==1)
	{
		ui->intPulses1->value(i2txt(dc->getIntPulses(ch)));
	}
	else
	{
		ui->intPulses2->value(i2txt(dc->getIntPulses(ch)));
	}

	dw->redraw();
}

//=============================================================================
void MainController::setNumBursts(int ch,const char *txt)
{
	int num=atoi(txt);
	if(num<0) num=0;

	dc->setNumBursts(ch,num);
	dc->setDuration();

	if(ch==1)
	{
		ui->numBursts1->value(i2txt(dc->getNumBursts(ch)));
	}
	else
	{
		ui->numBursts2->value(i2txt(dc->getNumBursts(ch)));
	}

	dw->redraw();
}

//=============================================================================
void MainController::setIntRecords(const char *txt)
{
	int num=atoi(txt);
	if(num<1) num=1;

	recControl->setIntRecords(num);
	ui->intRecords->value(i2txt(recControl->getIntRecords()));
}

//=============================================================================
void MainController::setNumRecords(const char *txt)
{
	int num=atoi(txt);
	if(num<1) num=1;

	recControl->setNumRecords(num);
	ui->numRecords->value(i2txt(recControl->getNumRecords()));
}

//=============================================================================
