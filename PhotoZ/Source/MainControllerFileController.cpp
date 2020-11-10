//=============================================================================
// MainControllerFileController.cpp
//=============================================================================
#include <stdlib.h>		// atoi()
#include <direct.h>		// _chdir(),_mkdir()
#include <fstream>

#include <FL/Fl_File_Chooser.H>

#include "MainController.h"
#include "FileController.h"
#include "SignalProcessor.h"
#include "UserInterface.h"
#include "RecControl.h"
#include "DapController.h"
#include "DapChannel.h"
#include "DataArray.h"
#include "Definitions.h"

using namespace std;

char* i2txt(int);
char* d2txt(double);

//=============================================================================
void MainController::createDir()
{
	// Get The Name of the New Folder
	char *input=fl_file_chooser( "Pick enter a new folder",NULL, NULL);
	if(!input)
	{
		return;// Canceled
	}

	// Make Directory
	int status = _mkdir(input);
	if(status<0)
	{
		fl_alert("Can not create the folder!");
		return;
	}

	// Change Directory
	chdir(input);

	// Reset
	recControl->reset();
	fileController->setFileNameFromRecControl();

	ui->setValue();	// Checked

	dataArray->resetData();

	// Redraw
	redraw();
}

//=============================================================================
void MainController::savePre()
{
	// Get The Name of the Preference File
	char *input=fl_file_chooser("Enter the name of the preference file","*.pre", "filename.pre");
	if(!input)
	{
		return;// Canceled
	}

	//====================================================
	// Open the file
	fstream file;
	file.open(input,ios::out);

	//====================================================
	// Version
	file<<2629112<<endl;
	file<<version<<endl<<endl;

	//====================================================
	// Schedule Flag
	file<<(int)dc->getScheduleFlag()<<endl<<endl;

	//====================================================
	// Image
	file<<aw->getImageX0()<<endl;
	file<<aw->getImageY0()<<endl;
	file<<aw->getImageXSize()<<endl;
	file<<aw->getImageYSize()<<endl<<endl;

	//====================================================
	// DAP
	file<<dc->reset->getOnset()<<endl;
	file<<dc->reset->getDuration()<<endl;

	file<<dc->shutter->getOnset()<<endl;
	file<<dc->shutter->getDuration()<<endl;

	file<<dc->getAcquiOnset()<<endl;

	file<<dc->sti1->getOnset()<<endl;
	file<<dc->sti1->getDuration()<<endl;

	file<<dc->sti2->getOnset()<<endl;
	file<<dc->sti2->getDuration()<<endl;

	file<<dc->getNumPts()<<endl;
	file<<dc->getIntPts()<<endl;

	file<<dc->getNumPulses(1)<<endl;
	file<<dc->getIntPulses(1)<<endl;

	file<<dc->getNumBursts(1)<<endl;
	file<<dc->getIntBursts(1)<<endl<<endl;

	file<<dc->getNumPulses(2)<<endl;
	file<<dc->getIntPulses(2)<<endl;

	file<<dc->getNumBursts(2)<<endl;
	file<<dc->getIntBursts(2)<<endl<<endl;

	//====================================================
	// Record
	file<<recControl->getNumRecords()<<endl;
	file<<recControl->getIntRecords()<<endl<<endl;

	//====================================================
	// Trial
	file<<recControl->getNumTrials()<<endl;
	file<<recControl->getIntTrials()<<endl<<endl;

	//====================================================
	// Gain
	file<<recControl->getAcquiGain()<<endl;
	file<<recControl->getRliGain()<<endl<<endl;

	//====================================================
	// Examination window
	file<<sp->getStartWindow()<<endl;
	file<<sp->getWidthWindow()<<endl<<endl;

	//====================================================
	// Baseline skip window
	file<<sp->getPolynomialStartPt()<<endl;
	file<<sp->getPolynomialNumPts()<<endl<<endl;

	//====================================================
	// Close file
	file.close();
}

//=============================================================================
void MainController::loadPre()
{
	//====================================================
	// Get The Name of the Preference File
	char *input=fl_file_chooser("Enter the name of the preference file","*.pre", "filename.pre");
	if(!input)
	{
		return;// Canceled
	}

	//====================================================
	fstream file;
	file.open(input,ios::in);

	int nValue;
	float fValue;

	//====================================================
	// Identification Number
	file>>nValue;
	if(nValue!=2629112)
	{
		fl_alert("Not A Photoz Preferece File!");
		return;
	}

	//====================================================
	// Version
	file>>fValue;
	if(fValue!=version)
	{
		fl_alert("Pre file version did not match!");
		return;
	}

	//====================================================
	// Schedule Flag
	file>>nValue;
	char flag=char(nValue);
	ui->scheduleB->value(flag);
	dc->setScheduleFlag(flag);

	//====================================================
	// Image
	file>>nValue;	// X0
	aw->setImageX0(nValue);
	ui->awImageX0->value(nValue);
	ui->awImageX0Txt->value(i2txt(nValue));

	file>>nValue;	// Y0
	aw->setImageY0(nValue);
	ui->awImageY0->value(nValue);
	ui->awImageY0Txt->value(i2txt(nValue));	// Image Y0

	file>>nValue;	// X Size
	aw->setImageXSize(nValue);
	ui->awImageXSize->value(nValue);

	file>>nValue;	// Y Size
	aw->setImageYSize(nValue);
	ui->awImageYSize->value(nValue);

	//====================================================
	// DAP
	file>>nValue;	// Reset
	dc->reset->setOnset(nValue);

	file>>nValue;
	dc->reset->setDuration(nValue);

	file>>nValue;	// Shutter
	dc->shutter->setOnset(nValue);

	file>>nValue;
	dc->shutter->setDuration(nValue);

	file>>nValue;	// Acquisition Onset
	dc->setAcquiOnset(nValue);

	file>>nValue;	// Stimulator 1
	dc->sti1->setOnset(nValue);

	file>>nValue;
	dc->sti1->setDuration(nValue);

	file>>nValue;	// Stimulator 2
	dc->sti2->setOnset(nValue);

	file>>nValue;
	dc->sti2->setDuration(nValue);

	file>>nValue;	// Number of Points
	dc->setNumPts(nValue);
	dataArray->changeNumPts(nValue);

	file>>fValue;	// Interval between Points
	dc->setIntPts(fValue);

	// Ch1
	file>>nValue;	// Number of pulses
	dc->setNumPulses(1,nValue);
	ui->numPulses1->value(i2txt(dc->getNumPulses(1)));

	file>>nValue;	// Interval between pulses
	dc->setIntPulses(1,nValue);
	ui->intPulses1->value(i2txt(dc->getIntPulses(1)));

	file>>nValue;	// Number of bursts
	dc->setNumBursts(1,nValue);
	ui->numBursts1->value(i2txt(dc->getNumBursts(1)));

	file>>nValue;	// Interval between bursts
	dc->setIntBursts(1,nValue);
	ui->intBursts1->value(i2txt(dc->getIntBursts(1)));

	// Ch2
	file>>nValue;	// Number of pulses
	dc->setNumPulses(2,nValue);
	ui->numPulses2->value(i2txt(dc->getNumPulses(2)));

	file>>nValue;	// Interval between pulses
	dc->setIntPulses(2,nValue);
	ui->intPulses2->value(i2txt(dc->getIntPulses(2)));

	file>>nValue;	// Number of bursts
	dc->setNumBursts(2,nValue);
	ui->numBursts2->value(i2txt(dc->getNumBursts(2)));

	file>>nValue;	// Interval between bursts
	dc->setIntBursts(2,nValue);
	ui->intBursts2->value(i2txt(dc->getIntBursts(2)));

	dc->setDuration();	// Set duration

	//====================================================
	// Record
	file>>nValue;
	recControl->setNumRecords(nValue);

	file>>nValue;
	recControl->setIntRecords(nValue);

	//====================================================
	// Trial
	file>>nValue;
	recControl->setNumTrials(nValue);
	dataArray->changeNumTrials(nValue);

	file>>nValue;
	recControl->setIntTrials(nValue);

	//====================================================
	// Gain
	file>>nValue;
	recControl->setAcquiGain((short)nValue);

	file>>nValue;
	recControl->setRliGain((short)nValue);

	//====================================================
	// Examination Window
	file>>nValue;
	sp->setStartWindow(nValue);
	ui->startWindow->value(i2txt(nValue));

	file>>nValue;
	sp->setWidthWindow(nValue);
	ui->widthWindow->value(i2txt(nValue));

	//====================================================
	// Baseline skip window
	file>>nValue;
	sp->setPolynomialStartPt(nValue);
	ui->polynomialStartPtTxt->value(i2txt(nValue));

	file>>nValue;
	sp->setPolynomialNumPts(nValue);
	ui->polynomialNumPts->value(i2txt(nValue));

	//====================================================
	// Close file
	file.close();

	dataArray->process();
	ui->setValue();	// Checked
	redraw();
}

//=============================================================================
// Call back function. Called when the open menu item is clicked.
//
void MainController::openDataFile()
{
	int status=fileController->openFile();

	if(status==1)		// Existing
	{
		dataArray->process();
	}
	else if(status==0)	// New file
	{
		dataArray->resetData();
	}
	else if(status==-1)	// Cancel
	{
		return;
	}

	ui->setValue();	// Checked

	aw->openImageFile(recControl->createImageFileName());

	redraw();
}

//=============================================================================
void MainController::saveData2File()
{
	int status=fileController->saveDataFile();

	if(status!=-1)
	{
		Fl_Color cSaveButton=fl_rgb_color(0,150,0);
		ui->saveData2File->labelcolor(cSaveButton);
		ui->saveData2File->redraw();
	}
}

//=============================================================================
void MainController::saveProcessedData()
{
	fileController->saveProcessedData();
}

//=============================================================================
char MainController::openDataFileFromRecControl()
{
	char *fileName=recControl->createFileName();
	char success;
	
	if(fileController->openFileByName(fileName))
	{
		dataArray->process();
		success=1;
	}
	else
	{
		dataArray->resetData();// Not Existed
		success=0;
	}

	ui->setValue();	// Checked

	return success;
}

//=============================================================================
void MainController::openImageFileFromRecControl()
{
	char *fileName=recControl->createImageFileName();
	aw->openImageFile(fileName);
}

//=============================================================================
void MainController::increaseNo(char p)
{
	recControl->increaseNo(p);

	if(p==SLICE || p==LOCATION)// SliceNo || LocationNo
	{
		saveOrNot();

		openDataFileFromRecControl();
		openImageFileFromRecControl();

		redraw();
	}
	else if(p==RECORD)// RecordNo
	{
		saveOrNot();

		openDataFileFromRecControl();

		redraw();
	}
	else if(p==TRIAL)// TrialNo
	{
		int trialNo=recControl->getTrialNo();
		dataArray->loadTrialData(trialNo);
		dataArray->process();

		ui->trialNo->value(i2txt(recControl->getTrialNo()));
		redraw();
	}
}

//=============================================================================
void MainController::decreaseNo(char p)
{
	recControl->decreaseNo(p);

	if(p==0 || p==1)// SliceNo || LocationNo
	{
		saveOrNot();

		openDataFileFromRecControl();
		openImageFileFromRecControl();

		redraw();
	}
	else if(p==2)// RecordNo
	{
		saveOrNot();

		openDataFileFromRecControl();

		redraw();
	}
	else if(p==3)// TrialNo
	{
		int trialNo=recControl->getTrialNo();
		dataArray->loadTrialData(trialNo);
		dataArray->process();

		ui->trialNo->value(i2txt(recControl->getTrialNo()));
		redraw();
	}
}

//=============================================================================
void MainController::setSliceNo(const char *p)
{
	saveOrNot();

	short no=atoi(p);
	recControl->setSliceNo(no);

	openDataFileFromRecControl();
	openImageFileFromRecControl();

	redraw();
}

//=============================================================================
void MainController::setLocationNo(const char *p)
{
	saveOrNot();

	short no=atoi(p);
	recControl->setLocationNo(no);

	openDataFileFromRecControl();
	openImageFileFromRecControl();

	redraw();
}

//=============================================================================
void MainController::setRecordNo(const char *p)
{
	saveOrNot();

	short no=atoi(p);
	recControl->setRecordNo(no);

	openDataFileFromRecControl();

	redraw();
}

//=============================================================================
void MainController::setTrialNo(const char *p)
{
	short no=atoi(p);
	short trialNoMax=(short)recControl->getNumTrials();

	if(no>=trialNoMax)
	{
		no=trialNoMax;
	}
	else if(no<0)
	{
		no=0;
	}

	recControl->setTrialNo(no);
	ui->trialNo->value(i2txt(recControl->getTrialNo()));

	dataArray->loadTrialData(no);
	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::saveOrNot()
{
	if(ui->saveData2File->labelcolor()==FL_RED)
	{
		// Yes 1; No 0
		char decision=fl_ask("Data changed! Do you want to save it?");
		if(decision==1)
		{
			saveData2File();
		}
	}

	Fl_Color cSaveButton=fl_rgb_color(0,150,0);
	ui->saveData2File->labelcolor(cSaveButton);
	ui->saveData2File->redraw();
}

//=============================================================================
