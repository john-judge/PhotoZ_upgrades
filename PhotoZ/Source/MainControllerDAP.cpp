//=============================================================================
// MainControllerDAP.cpp
//=============================================================================
#include <stdlib.h>		// atof()
#include <stdio.h>
#include <iostream>

#include "MainController.h"
#include "Camera.h"
#include "UserInterface.h"
#include "DapController.h"
#include "DapChannel.h"
#include "DataArray.h"
#include "ColorWindow.h"
#include "SignalProcessor.h"
#include "Definitions.h"

using namespace std;
char* f2txt(float);
char* i2txt(int);

//=============================================================================
void MainController::setResetOnset(const char *txt)
{
	
	double value=atof(txt);
	//std::cout << txt;
	if(value<0) value=0;

	dc->reset->setOnset(value);
	dc->setDuration();
	ui->resetOnset->value(dc->reset->getOnsetTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setResetDuration(const char *txt)
{
	double value=atof(txt);//temporary might need to change back to atoi
	if(value<0) value=0;

	dc->reset->setDuration(value);
	dc->setDuration();
	ui->resetDuration->value(dc->reset->getDurationTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setShutterOnset(const char *txt)
{
	double value=atof(txt);
	if(value<0) value=0;

	dc->shutter->setOnset(value);
	dc->setDuration();
	ui->shutterOnset->value(dc->shutter->getOnsetTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setShutterDuration(const char *txt)
{
	double value=atof(txt);
	if(value<0) value=0;

	dc->shutter->setDuration(value);
	dc->setDuration();
	ui->shutterDuration->value(dc->shutter->getDurationTxt());
	dw->redraw();
}

//=============================================================================
void MainController::setStiOnset(int ch, const char *txt)
{
	double value=atof(txt);
	if(value<0) value=0;
	//value += 3.5*(1/(double)Camera::FREQ[dc->getCameraProgram()]);

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
	double value=atof(txt);
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
	double value=atof(txt);
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
//	numPts = numPts;
	// Check the length of duration
	if(numPts<10)
	{
		numPts=10;
	}
	cout << " mcdap line 139 " << numPts << endl;
	dapControl->setNumPts(numPts);
	dapControl->setDuration();

	dataArray->changeNumPts(numPts);
	dataArray->process();
	
	// Update userinterface and redraw
	sprintf_s(buff, 16,"%i",numPts);
	ui->numPts->value(buff);
	
	sprintf_s(buff, 16,"%5.0f",dapControl->getAcquiDuration());	
	ui->acquiDuration->value(buff);

	redraw();
}

//=============================================================================
void MainController::set_digital_binning(const char* txt)
{

	int dbinning = atoi(txt);
	set_digital_binning(dbinning);

}
void MainController::set_digital_binning(int dbinning)
{
	if (dbinning < 1) dbinning = 1;

	/*if (dbinning == 1) {
		ui->awFpYScale->value(4.65*log2(dbinning + 1));
		ui->awYScale->value(4.65*log2(dbinning + 1));
		setAwFpYScale(4.65*log2(dbinning + 1));
		setAwYScale(4.65*log2(dbinning + 1));
	}
	else {
	if ((4.65*log(dbinning + 1)) >= 10)
	{
		setAwFpYScale(10);
		setAwYScale(10);
		ui->awFpYScale->value(10);
		ui->awYScale->value(10);
	}
	else
	{
		ui->awFpYScale->value(4.65*log(dbinning + 1));
		ui->awYScale->value(4.65*log(dbinning + 1));
		setAwFpYScale(4.65*log(dbinning + 1));
		setAwYScale(4.65*log(dbinning + 1));
	}
	}*/


	aw->clearSelected(0);
	dataArray->binning(dbinning);
	aw->changeNumDiodes();
	cw->changeNumDiodes();
	cw->setPointXYZ();
	sp->changeNumDiodes();

	ui->digital_binning->value(i2txt(dbinning));
	int trialNo=recControl->getTrialNo();
	dataArray->loadTrialData(trialNo);
	dataArray->calRli();
	dataArray->setMaxRli();
	dataArray->process();
	redraw();
}

//=============================================================================
void MainController::setCameraProgram(int p)
{
	dc->setCameraProgram(p);
	Camera cam;
	cam.setCamProgram(p);
//	camera->setCamProgram(p);
	dc->setDuration();
	
	// Resize the array
	aw->clearSelected(0);
	dataArray->changeRawDataSize(Camera::WIDTH[p], Camera::HEIGHT[p]);
	aw->changeNumDiodes();
	cw->changeNumDiodes();
	cw->setPointXYZ();
	sp->changeNumDiodes();
	cout << "mcDAP line 216 setCamProg p WIDTH and HEIGHT "<< p<<"  "<< Camera::WIDTH[p]<<"  " << Camera::HEIGHT[p]<<"\n";
	// Redraw DAP window
	dw->redraw();

	// Update user interface
	char buff[16];
	sprintf_s(buff, 16,"%6.0f",dapControl->getAcquiDuration());
	ui->acquiDuration->value(buff);
	redraw();
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
/*void MainController::setNumSkippedTrials(const char* txt)
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
}*/

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

//		case Decrease_RLI_Gain:
//			recControl->decreaseRliGain();
//			break;

//		case Increase_RLI_Gain:
//			recControl->increaseRliGain();
//			break;
	}
	
	ui->acquiGain->value(i2txt(recControl->getAcquiGain()));
//	ui->rliGain->value(i2txt(recControl->getRliGain()));
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

void MainController::resetCamera()
{
	dc->resetCamera();
}
//=============================================================================
