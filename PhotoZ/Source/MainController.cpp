//=============================================================================
// MainController.cpp
//=============================================================================
#include <stdlib.h>
#include <stdio.h>

#include <FL/fl_ask.h>

#include "MainController.h"
#include "UserInterface.h"
#include "DataArray.h"
#include "FileController.h"
#include "SignalProcessor.h"
#include "Color.h"
#include "Data.h"
#include "DapWindow.h"
#include "DapController.h"
#include "WindowExporter.h"

//=============================================================================
Color *colorControl=new Color();
UserInterface *ui=new UserInterface();
ArrayWindow *aw=ui->aw;
TraceWindow *tw=ui->tw;
ColorWindow *cw=ui->cw;
DapWindow *dw=ui->dapWindow;
RecControl *recControl=new RecControl();

DapController *dc=new DapController();
DapController *dapControl=dc;

DataArray *dataArray=new DataArray(dc->getNumPts());
SignalProcessor *sp=new SignalProcessor();
FileController *fileController=new FileController();
WindowExporter *we=new WindowExporter();

char txtBuf[32];

//=============================================================================
MainController::MainController():version(5.16)
{
}

//=============================================================================
//	Start the program
//=============================================================================
void MainController::start()
{
	ui->init();
	cw->setPointXYZ();

	// Open 01_01_01.zda
	if(fileController->openFileByName("01_01_01.zda"))
	{
		dataArray->process();
	}

	ui->setValue();	// Checked

	dw->init(dc);

	aw->openImageFile("01-01.bmp");

	//
	ui->show();
	Fl::run();
}

//=============================================================================
void MainController::redraw()
{
	aw->redraw();
	tw->redraw();
	dw->redraw();

	if(ui->mapGroup->visible())
	{
		cw->redraw();
	}
}

//=============================================================================
void MainController::quit()
{
	char choice=fl_ask("Do you really want to quit Photoz3?");

	if(choice)
	{
		exit(0);
	}
}

//=============================================================================
void MainController::setBgColor(char p)	// 1 indicate : FG:Black , BG:White
{
	if(p)
	{
		aw->setFgBgColor(FL_BLACK,FL_WHITE);
		tw->setFgBgColor(FL_BLACK,FL_WHITE);
		dw->setBgColor(FL_WHITE);
	}
	else
	{
		aw->setFgBgColor(FL_WHITE,FL_BLACK);
		tw->setFgBgColor(FL_WHITE,FL_BLACK);
		dw->setBgColor(FL_BLACK);
	}

	redraw();
}

//=============================================================================
void MainController::setImageType(char p)
{
	we->setImageType(p);
}

//=============================================================================
void MainController::exportAw()
{
	we->export1(aw->x(),aw->y(),aw->w(),aw->h());
}

//=============================================================================
void MainController::exportTw()
{
	we->export1(tw->x(),tw->y(),tw->w(),tw->h());
}

//=============================================================================
void MainController::setColorMode(char input)
{
	colorControl->setColorMode(input);
	redraw();
}

//=============================================================================
void MainController::setColorControlMode(char input)
{
	colorControl->setColorControlMode(input);
	redraw();
}

//=============================================================================
void MainController::setColorUpperBound(double input)
{
	colorControl->setColorUpperBound(input);
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->colorUpperBound->value(txtBuf);
	redraw();
}

//=============================================================================
void MainController::setColorLowerBound(double input)
{
	colorControl->setColorLowerBound(input);
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->colorLowerBound->value(txtBuf);
	redraw();
}

//=============================================================================
void MainController::setColorAmp(double input)
{
	colorControl->setColorAmp(input);
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->colorAmp->value(txtBuf);
	redraw();
}

//=============================================================================
void MainController::setColorCenter(double input)
{
	colorControl->setColorCenter(input);
	sprintf_s(txtBuf, 32," % .2f",input);
	ui->colorCenter->value(txtBuf);
	redraw();
}

//=============================================================================
void MainController::doAll()
{
	// Save original state
	short oldRecordNo=recControl->getRecordNo();
	char oldCompareFlag=dataArray->getCompareFlag();
	char oldIncreaseFlag=dataArray->getIncreaseFlag();

	// Amp1
	dataArray->setCompareFlag(0);
	recControl->setRecordNo(dataArray->getRecordXNo(1)+1);
	openDataFileFromRecControl();
	cw->redraw();
	Fl::flush();
	we->export2(tw->x(),tw->y(),tw->w(),tw->h(),"Amp1.png");

	// Amp2
	recControl->setRecordNo(dataArray->getRecordXNo(2)+1);
	openDataFileFromRecControl();
	cw->redraw();
	Fl::flush();
	we->export2(tw->x(),tw->y(),tw->w(),tw->h(),"Amp2.png");

	// Restore original state
	recControl->setRecordNo(oldRecordNo);
	openDataFileFromRecControl();

	// DAmp12
	dataArray->setCompareFlag(1);
	dataArray->setIncreaseFlag(1);
	cw->redraw();
	Fl::flush();
	we->export2(tw->x(),tw->y(),tw->w(),tw->h(),"DAmp12.png");

	// -DAmp12
	dataArray->setIncreaseFlag(0);
	cw->redraw();
	Fl::flush();
	we->export2(tw->x(),tw->y(),tw->w(),tw->h(),"DAmp12-.png");

	// Restore original state
	dataArray->setCompareFlag(oldCompareFlag);
	dataArray->setIncreaseFlag(oldIncreaseFlag);
	cw->redraw();
}

//=============================================================================
