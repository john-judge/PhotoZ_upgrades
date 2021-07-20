//=============================================================================
// MainController.cpp
//=============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

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
#include "LiveFeed.h"

using namespace std;
//=============================================================================
Color *colorControl;
UserInterface *ui;
ArrayWindow *aw;
TraceWindow *tw;
ColorWindow *cw;
DapWindow *dw;
RecControl *recControl;

DapController *dc;
DapController *dapControl;

LiveFeed *lf;
DataArray *dataArray;
SignalProcessor *sp;
FileController *fileController;
WindowExporter *we;

char txtBuf[32];

//=============================================================================
MainController::MainController():version(5.32f)
{
	// these need to be changed after dataArray has been initialized
	colorControl = new Color();
	ui = new UserInterface();
	aw = ui->aw;
	tw = ui->tw;
	cw = ui->cw;
	dw = ui->dapWindow;
	recControl = new RecControl();

	dc = new DapController();
	dapControl = dc;

	lf = new LiveFeed();
	dataArray = new DataArray(dc->getNumPts());
	sp = new SignalProcessor();
	fileController = new FileController();
	we = new WindowExporter();
	aw->changeNumDiodes();
	cw->changeNumDiodes();
	sp->changeNumDiodes();
}

MainController::~MainController() {
	delete colorControl;
	ui->killUI();
	delete ui;
	delete recControl;
	delete dc;
	delete lf;
	delete dataArray;
	delete sp;
	delete fileController;
	delete we;
	
}

//=============================================================================
//	Start the program
//=============================================================================
void MainController::start()					//this module runs at startup and the initcam calls give console messages indicating commands were executed correcly
{
	//char command[80];
	ui->init();
	cw->setPointXYZ();
	ui->setValue();	
	/* 
	JMJ 12/26/2020 - replaced for little Dave; initcam is now called in Camera::init_cam

	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_0 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_1 -f c:\\EDT\\pdv\\camera_config\\DM2K_1024x20.cfg");
	system(command);
	*/
	//dc->resetCamera();			// replaced for LittleDave works but disable while developing program
	dw->init(dc);
	aw->openImageFile("01-01.bmp");
	setCameraProgram(7); // Default setting
	//cout << "mc line 103  initialized camera " << endl;
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
	char choice=fl_ask("Do you really want to quit Photoz-LilDave?");
	if(choice)
	{
		this->~MainController();
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
	if (aw->h() < 631) 	fl_alert("Make sure you are in full screen!\n");
	we->setImageType('P');
	we->export1(aw->x(),aw->y()+18,aw->w(),aw->h());		// 18 added to correct for centering by code in 
}

void MainController::exportAw_Jpeg()						//export array window as jpeg
{
	if (aw->h() < 631) fl_alert("Make sure you are in full screen!\n");
	we->setImageType('J');
	we->export1(201, 124, 500, 500);
	}

//=============================================================================
void MainController::exportTw()
{
	if (aw->h() < 631) 	fl_alert("Make sure you are in full screen!\n");
	we->setImageType('P');
	we->export1(tw->x(), tw->y() + 18, tw->w(), tw->h());
}

//=============================================================================
void MainController::setLiveFeed(char input)
{
	try {
		if (input)
		{
			dapControl->releaseDAPs();
			if (!lf->begin_livefeed())
				ui->lfRun->value(0);
		}
		else
		{
			lf->stop_livefeed();
			lf->~LiveFeed();
		}
	}
	catch (...) {
		std::cout << "Some exception \n";
		std::exception_ptr p = std::current_exception();
		std::cout << (p._Current_exception) << std::endl;
	}
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

void MainController::saveLatestBackgroundImage()
{
	std::string filename = "Check.bmp";
	lf->getImage(filename);
}

//=============================================================================
