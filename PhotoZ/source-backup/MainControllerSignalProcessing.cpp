//=============================================================================
// MainControllerSignalProcessing.cpp
//=============================================================================
#include <stdlib.h>
#include <fstream>

#include <FL/fl.h>
#include <FL/fl_ask.h>
#include <FL/fl_file_chooser.h>
#include <iostream>

#include "MainController.h"
#include "UserInterface.h"
#include "DataArray.h"
#include "FileController.h"
#include "SignalProcessor.h"
#include "Color.h"
#include "ColorWindow.h"
#include "Definitions.h"
#include "Data.h"
#include "DapWindow.h"
#include "DapController.h"
#include "WindowExporter.h"


using namespace std;

char* i2txt(int);

//=============================================================================
void MainController::setNormalization(char p)
{
	dataArray->setNormalizationFlag(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setRliDiv(char p)
{
	sp->setRliDivFlag(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setTemporalFilterFlag(char p)
{
	sp->setTemporalFilterFlag(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setSpatialFilterFlag(char p)
{
	sp->setSpatialFilterFlag(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setSpatialFilterSigma(double p)
{
	sp->setSpatialFilterSigma(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setTemporalFilterWidth(int width)
{
	sp->setTemporalFilterWidth(width);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setCorrectionFlag(char p)
{
	sp->setCorrectionFlag(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setDataInverseFlag(char p)
{
	sp->setDataInverseFlag(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setEcDataInverseFlag(char p)
{
	sp->setFpInverseFlag(p);

	dataArray->process();

	aw->redraw();
	tw->redraw();
}

//=============================================================================
void MainController::setBLCType(char p)
{
	sp->setBLCType(p);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setClampPt(int pt)
{
	int max=dc->getNumPts();
	if(pt>=max)
	{
		pt=max-1;
	}

	sp->setClampPt(pt);

	ui->clampPtTxt->value(i2txt(pt));

	if(sp->getBLCType()=='A')
	{
		dataArray->process();

		redraw();
	}
}

//=============================================================================
void MainController::setPolynomialStartPt(const char *p)
{
	int value=atoi(p);

	if(value<0)
	{
		value=0;
	}

	if(value>=dc->getNumPts()-sp->getWidthWindow())
	{
		value=dc->getNumPts()-sp->getWidthWindow()-1;
	}

	sp->setPolynomialStartPt(value);

	if(sp->getBLCType()=='P')
	{
		dataArray->process();

		redraw();
	}
}

//=============================================================================
void MainController::setPolynomialNumPts(const char *p)
{
	int value;
	value=atoi(p);

	if(value<0)
	{
		value=0;
	}

	sp->setPolynomialNumPts(value);

	if(sp->getBLCType()=='P')
	{
		dataArray->process();

		redraw();
	}
}

//=============================================================================
void MainController::processData()
{
	dataArray->process();
	redraw();
}

//=============================================================================
void MainController::setStartWindow(const char *p)
{
	int value=atoi(p);

	if(value<0)
	{
		value=0;
		ui->startWindow->value(i2txt(value));
		ui->startWindow->redraw();
	}

	int max=dc->getNumPts();
	int width=sp->getWidthWindow();
	
	if(value>=max-width)
	{
		value=max-width-1;
		ui->startWindow->value(i2txt(value));
		ui->startWindow->redraw();
	}

	sp->setStartWindow(value);
	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setWidthWindow(const char *p)
{
	int value=atoi(p);

	if(value<0)
	{
		value=0;
		ui->widthWindow->value(i2txt(value));
		ui->widthWindow->redraw();
	}

	int max=dc->getNumPts();

	int lowerBound=sp->getStartWindow();
	
	if(max-lowerBound < value)
	{
		value=max-lowerBound;
		ui->widthWindow->value(i2txt(value));
		ui->widthWindow->redraw();
	}

	sp->setWidthWindow(value);
	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::saveValues()
{
	int numSelectedDiodes=aw->getNumSelectedDiodes();
	int numRegions = aw->getNumRegions();
	char valueType=tw->getValueType();
	int *selectedDiodes = aw->getSelectedDiodes();
	if(numSelectedDiodes+numRegions<1 || valueType=='N')
	{
		return;
	}
	Data* data;
	data = NULL;

	// Get file name
	char *fileName=fl_file_chooser(
		"Please enter the name of the property file","*.dat","Values.dat");

	if(!fileName)
	{
		return;
	}

	// Open file
	fstream file;
	file.open(fileName,ios::out);

	//
	int i;
	double value;
	int index=0;

	for(i=0;i<numSelectedDiodes+numRegions;i++)
	{
		if (i < numSelectedDiodes) 
		{
			data = dataArray->getData(selectedDiodes[i]);
			index=selectedDiodes[i];
		}
		else
		{
			int j = i - numSelectedDiodes;
			dataArray->aveROIData(j);
			aveData = dataArray->getAveData();
			data = new Data;
			data = dataArray->getROIAve(j);
			data->measureProperties();
			index = j;
		}
	
		if(valueType=='M')
		{
			value=data->getMaxAmp();
		}
		else if(valueType=='L')
		{
			value=data->getMaxAmpLatency();
		}
		else if(valueType=='A')
		{
			value=data->getAmp(tw->getPointLinePt());
		}
		else if(valueType=='R')
		{
			value=data->getRli();
		}
		else if(valueType=='S')
		{
			value=data->getSD();
		}
		else if(valueType=='p')
		{
			value = data->getHalfAmpLatency();
		}
		
		else if (valueType == 'Q')
		{
			value = data->getMaxAmp() / data->getSD();
		}
		else if (valueType == '9')
		{
			value = data->getMaxSlope();
		} 
		else if (valueType == '8')
		{
			value = data->getMaxSlopeLatency();
		}
		file<<index+1<<'\t'<<value<<'\n';
	}
	// Close file
	file.close();
}

//=============================================================================
void MainController::saveProfileValues()
{
	int numSelectedDiodes=aw->getNumSelectedDiodes();

	if(numSelectedDiodes<1)
	{
		return;
	}

	// Get file name
	char *fileName=fl_file_chooser(
		"Please enter the name of the property file","*.dat","ProfileData.dat");

	if(!fileName)
	{
		return;
	}

	// Open file
	fstream file;
	file.open(fileName,ios::out);

	// Save labels
	file<<"DiodeNo\tAmp1\tAmp1SD\tAmp2\tAmp2SD\tAmpCha\tAmpChaSD\tAmpPerCha\tAmpPerChaSD\n";
	
	// Go through diodes
	int i;
	int *selectedDiodes=aw->getSelectedDiodes();
	int diodeIndex;

	for(i=0;i<numSelectedDiodes;i++)
	{
		// Diode No
		diodeIndex=selectedDiodes[i];
		file<<diodeIndex+1<<'\t';

		// Amp1
		file<<dataArray->getDataMaxAmp(diodeIndex,1)<<'\t';

		// Amp1SD		file<<dataArray->getDiodeMaxAmpSD(diodeIndex,1)<<'\t';

		// Amp2
		file<<dataArray->getDataMaxAmp(diodeIndex,2)<<'\t';

		// Amp2SD		file<<dataArray->getDiodeMaxAmpSD(diodeIndex,2)<<'\t';

		// AmpCha
		file<<dataArray->getDiodeMaxAmpCha(diodeIndex)<<'\t';

		// AmpChaSD		file<<dataArray->getDiodeMaxAmpSD(diodeIndex,2)<<'\t';

		// AmpPerCha
		file<<dataArray->getDiodeMaxAmpPerCha(diodeIndex)<<'\t';

		// AmpPerChaSD
		file<<dataArray->getDiodeMaxAmpPerChaSD(diodeIndex)<<'\n';
	}

	// Close file
	file.close();
}

//=============================================================================
// Thresholding
//=============================================================================
void MainController::doSelection(int property)
{
	dataArray->doSelection(property);

	aw->redraw();
	tw->redraw();
}

//=============================================================================
void MainController::setPerAmp(double p)
{
	Data::setPerAmp(p);
	dataArray->process();
	redraw();
}

//=============================================================================
void MainController::saveMapValues()
{
	int i;

	// Ask for the file name
	char *fileName=fl_file_chooser("Please enter the name of the map file mcsp line 424","*.map","Map.map");

	if(!fileName)
	{
		return;
	}

	// Open the file and save values
	fstream file;
	file.open(fileName,ios::out);
	char ignoreFlag;

	file << dataArray->binned_width() << '\n';
	file << dataArray->binned_height() << '\n';

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		file<<i+1;
		file<<'\t'<<dataArray->getCorrectionValue(i);

		if(dataArray->getIgnoredFlag(i))
		{
			ignoreFlag='1';
		}
		else
		{
			ignoreFlag='0';
		}

		file<<'\t'<<ignoreFlag<<'\n';
	}

	file.close();
}

//=============================================================================
void MainController::loadMapValues()
{
	int i;
	std::vector <double> value;
	std::vector <char> ignored;

	// Ask for the file name
	char *fileName=fl_file_chooser("Please select one map file","*.map","Map.map");

	if(!fileName)
	{
		return;
	}

	// Load values
	fstream file;
	file.open(fileName,ios::in);
	int diodeNum;
	char ignoreFlag;
	int bwidth, bheight, curr_bwidth, curr_bheight;

	curr_bwidth = dataArray->binned_width();
	curr_bheight = dataArray->binned_height();
	file >> bwidth;
	file >> bheight;

	if (bwidth != curr_bwidth || bheight != curr_bheight)
	{
		char msg[128];
		sprintf(msg, "Map file is for array size %dx%d, not %dx%d!\n",
			bwidth, bheight, curr_bwidth, curr_bheight);
		fl_alert(msg);
		file.close();
		return;
	}

	int bdiodes = dataArray->num_binned_diodes();
	value.assign(bdiodes, 0.0);
	ignored.assign(bdiodes, 0);

	for (i = 0; i < bdiodes; i++)
	{
		file>>diodeNum;
		file>>value[i];
		file>>ignoreFlag;

		if(ignoreFlag=='1')
		{
			ignored[i]=1;
		}
	}

	file.close();

	// Set Correction Value
	for (i = 0; i < bdiodes; i++)
	{
		// Set correction value
		dataArray->setCorrectionValue(i,value[i]);

		// Set ignorance
		dataArray->setIgnoredFlag(i,ignored[i]);
	}

	// Process Data
	dataArray->process();

	// Redraw
	redraw();
}

//=============================================================================
void MainController::rli2Map()
{
	int i;
	std::vector<double> rli;
	int bdiodes = dataArray->num_binned_diodes();

	rli.assign(bdiodes, 0.0);

	// Get RLI values
	for (i = 0; i < bdiodes; i++)
	{
		rli[i]=dataArray->getRli(i);
	}

	// Set Correction Value
	for (i = 0; i < bdiodes; i++)
	{
		dataArray->setCorrectionValue(i,rli[i]);
	}

	// Process Data
	dataArray->process();

	// Redraw
	redraw();
}

//=============================================================================
void MainController::makeCorrection()
{
	dataArray->makeCorrection();

	// Process Data
	dataArray->process();

	// Redraw
	redraw();
}

//=============================================================================
void MainController::setTemporalFilterType(char input)
{
	sp->setTemporalFilterType(input);

	dataArray->process();

	redraw();
}

//=============================================================================
void MainController::setNumAveRec(int num)
{
	dataArray->setNumAveRec(num);
	redraw();
}

//=============================================================================
