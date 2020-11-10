//=============================================================================
// MainControllerAcqui.cpp
//=============================================================================
#include <time.h>
#include <sys/types.h>	// _stat
#include <sys/stat.h>	// _stat
#include <direct.h>		// _chdir(),_mkdir()

#include <FL/Fl_File_Chooser.H>

#include "MainController.h"
#include "SignalProcessor.h"
#include "UserInterface.h"
#include "RecControl.h"
#include "DapController.h"
#include "DataArray.h"
#include "Definitions.h"
#include "Data.h"

char* i2txt(int);

// IP - Channel Map
int Ch_IP[472]=
{
	57,61,49,53,41,45,33,219,223,211,215,203,37,25,29,17,21,9,13,207,
	195,199,250,254,242,246,1,5,56,60,48,52,40,234,238,226,230,218,222,210,
	214,44,32,36,24,28,16,20,8,202,206,194,198,313,317,305,309,12,0,4,
	59,63,51,55,43,47,297,301,289,293,281,285,273,277,35,39,27,31,19,23,
	11,15,3,265,269,257,261,312,316,304,308,296,7,58,62,50,54,42,46,34,
	30,300,288,292,280,284,272,276,264,268,256,26,38,18,22,10,14,2,6,121,
	125,260,315,319,307,311,299,303,291,295,283,113,117,105,109,97,101,89,93,81,
	85,73,287,275,279,267,271,259,263,314,318,306,77,65,69,120,124,112,116,104,
	108,96,100,310,298,302,290,294,282,286,274,278,266,270,88,92,80,84,72,76,
	64,68,123,127,115,258,262,377,381,369,373,361,365,353,357,345,349,119,107,111,
	99,103,91,95,83,87,75,79,67,337,341,329,333,321,325,376,380,368,372,360,
	364,71,122,126,114,118,106,110,98,102,90,94,82,352,356,344,348,336,340,328,
	332,320,324,379,86,74,78,66,70,185,189,177,181,169,173,161,383,371,375,363,
	367,355,359,347,351,339,343,331,165,153,157,145,149,137,141,129,133,184,188,335,
	323,327,378,382,370,374,362,366,354,358,346,176,180,168,172,160,164,152,156,144,
	148,136,350,338,342,330,334,322,326,441,445,433,437,140,128,132,187,191,179,183,
	171,175,163,167,425,429,417,421,409,413,401,405,393,397,155,159,147,151,139,143,
	131,135,186,190,385,389,440,444,432,436,424,428,416,420,178,182,170,174,162,166,
	154,158,146,408,412,400,404,392,396,384,388,443,447,150,138,142,130,134,249,253,
	241,245,435,439,427,431,419,423,411,415,403,233,237,225,229,217,221,209,213,201,
	407,395,399,387,391,442,446,434,205,193,197,248,252,240,244,232,438,426,430,418,
	422,410,414,402,236,224,228,216,220,208,212,406,394,398,386,390,505,497,489,200,
	204,192,196,251,255,243,481,473,465,457,449,504,496,247,235,239,227,231,488,480,
	472,464,456,448,
	452,460,468,476,484,492,500,508
};

//=============================================================================
void MainController::takeRli()
{
	int i,j;

	//-------------------------------------------
	// Allocate Memory
	//
	short *memory=new short[512*350];
	short **traceData=new short*[Num_Diodes];

	for(i=0;i<Num_Diodes;i++)
	{
		traceData[i]=new short[350];
	}

	//-------------------------------------------
	// Get Signal
	//
	dapControl->setDAPs();
	dapControl->resetDAPs();
	dapControl->takeRli(memory);
	dapControl->releaseDAPs();

	//-------------------------------------------
	// Arrange Data from Memory to traceData
	//
	for(i=0;i<Num_Diodes;i++)
	{
		for(j=0;j<350;j++)
		{
			traceData[i][j]=memory[Ch_IP[i]+j*512];
		}
	}

	//-------------------------------------------
	// Release memory
	//
	delete [] memory;

	//-------------------------------------------
	// Calculate RLI
	//
	long low,high;
	short max;

	for(i=0;i<Num_Diodes;i++)
	{
		for(low=0,j=100;j<150;j++)// Low : 100~150
		{
			low+=traceData[i][j];
		}
		low/=50;

		for(high=0,j=250;j<300;j++)// High : 250~300
		{
			high+=traceData[i][j];
		}
		high/=50;

		for(max=30000,j=175;j<225;j++)// Max : 175~225
		{
			if(traceData[i][j]<max)
			{
				max=traceData[i][j];
			}
		}

		dataArray->setRliLow(i,(short)low);
		dataArray->setRliHigh(i,(short)high);
		dataArray->setRliMax(i,max);
	}

	//-------------------------------------------
	// Release Memory
	//
	for(i=0;i<Num_Diodes;i++)
	{
		delete [] traceData[i];
	}
	delete [] traceData;

	//-------------------------------------------
	// Calculate RLI
	//
	dataArray->calRli();
	dataArray->setMaxRli();

	//-------------------------------------------
	// If RliDiv is set, process the data
	//
	if(sp->getRliDivFlag())
	{
		dataArray->process();
	}

	//-------------------------------------------
	// Redraw
	//
	redraw();
}

//=============================================================================
// Acquire one record
//
void MainController::acquiOneRecord()
{
	//-------------------------------------------
	int i,j;
	clock_t start,now;

	//-------------------------------------------
	// Get Number of Trials and the Interval between two Trials
	//
	int numTrials=recControl->getNumTrials();				// Number of Trials
	int numSkippedTrials=recControl->getNumSkippedTrials();	// Number of skipped trials
	int interval=recControl->getIntTrials()*1000;			// sec -> msec
	int numPts=dapControl->getNumPts();						// Number of Points per Trace

	//-------------------------------------------
	// Allocate Memory
	//
	short *memory=new short[512*numPts];

	//-------------------------------------------
	// Set Stop Flag to 0
	//
	dapControl->setStopFlag(0);


	int totalTrials=(1+numSkippedTrials)*numTrials;
	int trialCount=0;
	int trialNo=0;

	for(;trialCount<totalTrials && !dapControl->getStopFlag();trialCount++)
	{
		//-------------------------------------------
		// Time Management
		if(trialCount==0)// First Trial
		{
			start=clock();

			time_t t;
			time(&t);
			recControl->setTime(t);
		}
		else// Second Trial and so forth
		{
			now=clock();
			Fl::check();

			while((now-start)<(interval*trialCount))
			{
				now=clock();
				Fl::check();

				if(dapControl->getStopFlag())
				{
					break;
				}
			}
		}

		//-------------------------------------------
		if(dapControl->getStopFlag())
		{
			break;
		}

		//-------------------------------------------
		// Recording
		dapControl->setDAPs();
		dapControl->resetDAPs();
		dapControl->createAcquiDapFile();

		if(trialCount%(1+numSkippedTrials)==0)
		{
			// Send Dap File to the DAP board
			int status=dapControl->sendFile2Dap("Record-3200 v5.dap","Record-5400 v5.dap");

			if(status==0)// Failed to Send Dap File
			{
				delete [] memory;
				fl_alert("Failed to Send DAP files to DAPs!\n");
				dapControl->releaseDAPs();
				return;
			}

			dapControl->acqui(memory);

			// Arrange Data
			dataArray->arrangeData(trialNo,memory);
			dataArray->loadTrialData(trialNo+1);
			dataArray->process();

			// Set User Interface
			recControl->setTrialNo(trialNo+1);
			ui->trialNo->value(i2txt(recControl->getTrialNo()));

			// Redraw
			redraw();

			trialNo++;
		}
		else // Pseudo-Recording
		{
			// Send Dap File to the DAP board
			int status=dapControl->sendFile2Dap("PseudoRecord-3200 v5.dap");

			if(status==0)// Failed to Send Dap File
			{
				delete [] memory;
				fl_alert("Failed to Send DAP files to DAPs!\n");
				dapControl->releaseDAPs();
				return;
			}

			dapControl->pseudoAcqui();
		}

		dapControl->releaseDAPs();
	}

	// Release Memory
	delete [] memory;

	// Load Average
	dataArray->loadTrialData(0);
	dataArray->process();

	// Set User Interface
	recControl->setTrialNo(0);
	ui->trialNo->value(i2txt(recControl->getTrialNo()));
	ui->saveData2File->labelcolor(FL_RED);
	ui->saveData2File->redraw();

	// Redraw
	redraw();
}

//=============================================================================
void MainController::sleep(double seconds)
{
	clock_t start=clock();
	clock_t now=clock();

	while(now<(start+1000*seconds))
	{
		now=clock();
	}
}

//=============================================================================
// acqui() is executed after clicking Acquisition Button
//
void MainController::record()
{
	int i;

	if(!dc->getScheduleFlag())	// When Schedule is not checked
	{
		acquiOneRecord();
	}
	else	// When Schedule is checked
	{
		int interval=recControl->getIntRecords()*1000;	// sec -> m sec
		int numRecords=recControl->getNumRecords();
		int num=0;
		clock_t start;
		clock_t now;

		dc->setStopFlag(0);
		increaseNo(RECORD);

		// Do the first recording
		{
			start=clock();
			acquiOneRecord();

			if(dc->getScheduleRliFlag())
			{
				takeRli();
			}

			saveData2File();
		}

		// Do the rest recordings
		for(i=1;i<numRecords;i++)
		{
			now=clock();
			
			while((now-start)<(interval*i))
			{
				now=clock();
				Fl::check();

				if(!dc->getScheduleFlag() || dc->getStopFlag())
				{
					return;
				}
			}

			if(!dc->getScheduleFlag() || dc->getStopFlag())
			{
				return;
			}

			increaseNo(RECORD);

			{
				acquiOneRecord();

				if(dc->getScheduleRliFlag())
				{
					takeRli();
				}

				saveData2File();
			}
		}
	}
}

//=============================================================================
void MainController::stop()
{
	dapControl->stop();
}

//=============================================================================
void MainController::setScheduleFlag(char p)
{
	dapControl->setScheduleFlag(p);
}

//=============================================================================
void MainController::setScheduleRliFlag(char p)
{
	dapControl->setScheduleRliFlag(p);
}

//=============================================================================
void MainController::setLatencyStart(const char* input)
{
	double value;
	value=atof(input);

	dataArray->setLatencyStart(value);
	ui->latencyStartRoller->value(value);
	redraw();
}

//=============================================================================
void MainController::setLatencyStart(double value)
{
	char txt[32];
	dataArray->setLatencyStart(value);
	_gcvt_s(txt, 32, value, 6);
	ui->latencyStartTxt->value(txt);
	redraw();
}

//=============================================================================
void MainController::setLatencyWindow(double input)
{
	dataArray->setLatencyWindow(input);
	redraw();
}

//=============================================================================
void MainController::setFittingVarAllDiodes()
{
	int i;
	double fittingVar[5];
	bool changeFlag[5];

	fittingVar[0]=ui->alphaStartPointRoller->value();
	fittingVar[1]=ui->alphaAmplitudeSlider->value();
	fittingVar[2]=ui->alphaTauSlider->value();
	fittingVar[3]=ui->R2Slider->value();
	fittingVar[4]=ui->R1Slider->value();

	for(i=0;i<5;i++)
	{
		changeFlag[i]=tw->getChangeBits(i);
	}

	Data* data;

	for(i=0;i<464;i++)
	{
		data=dataArray->getData(i);
		data->setAllFittingVar(fittingVar,changeFlag);
	}
}

//=============================================================================
void MainController::setAutoDetectSpike(bool flag)
{
	int i;
	Data* data;

	for(i=0;i<464;i++)
	{
		data=dataArray->getData(i);
		data->setAutoDetectSpike(flag);
	}

	int diodeNo=tw->getLastDiodeNo();

	if(diodeNo>=0)
	{
		tw->setFittingGadgets(diodeNo);
	}
}

//=============================================================================
void MainController::setAlphaSpikeStart(const char* txt)
{
	int diodeNo=tw->getLastDiodeNo();

	if(diodeNo<0)
	{
		return;
	}

	Data* data=dataArray->getData(diodeNo);
	data->setAlphaSpikeStart(atoi(txt));
}

//=============================================================================
void MainController::setAlphaSpikeEnd(const char* txt)
{
	int diodeNo=tw->getLastDiodeNo();

	if(diodeNo<0)
	{
		return;
	}

	Data* data=dataArray->getData(diodeNo);
	data->setAlphaSpikeEnd(atoi(txt));
}

//=============================================================================
void MainController::initializeFittingAmp()
{
	int i;
	Data* data;

	for(i=0;i<464;i++)
	{
		data=dataArray->getData(i);
		data->initializeFittingAmp();
	}

	int diodeNo=tw->getLastDiodeNo();

	if(diodeNo>=0)
	{
		tw->setFittingGadgets(diodeNo);
	}
}

//=============================================================================
