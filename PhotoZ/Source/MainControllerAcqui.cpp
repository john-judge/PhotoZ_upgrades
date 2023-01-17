//=============================================================================
// MainControllerAcqui.cpp
//=============================================================================
#include <time.h>
#include <sys/types.h>	// _stat
#include <sys/stat.h>	// _stat
#include <direct.h>		// _chdir(),_mkdir()
#include <string>
#include <FL/Fl_File_Chooser.H>
#include <iostream>

#include "MainController.h"
#include "SignalProcessor.h"
#include "UserInterface.h"
#include "RecControl.h"
#include "DapController.h"
#include "DataArray.h"
#include "Definitions.h"
#include "Data.h"
#include "Camera.h"

using namespace std;
char* i2txt(int);

//=============================================================================
void MainController::takeRli()
{
	int i, j;		// i is array index; j = time index


	//cam->program(dc->getCameraProgram()); // instead of setCamProgram, this can change PDV dim settings by calling pdv_setsize.

	int array_diodes = dataArray->num_raw_array_diodes();
	int num_RLI_pts = 475;
	

	//-------------------------------------------
	// Allocate image memory 
	Camera cam;
	unsigned short* memory = cam.allocateImageMemory(array_diodes, num_RLI_pts+1);

	//-------------------------------------------
	// Allocate trace data
	short** traceData = new short*[array_diodes];
	for (i = 0; i < array_diodes; i++) {
		traceData[i] = new short[num_RLI_pts];
	}

	if (dapControl->takeRli(memory))
		fl_alert(" Main Controller::takeRli Timeouts ocurred! Check camera and connections.");

	// Arrange Data from Memory to traceData

	for (i = 0; i < array_diodes; i++) {
		for (j = 0; j < num_RLI_pts; j++) {
			traceData[i][j] = memory[i + j * array_diodes];
			//if(memory[i + j * array_diodes] != 2048) 
			//	cout << "j=" << j << "\t Value:" << memory[i + j * array_diodes] << "\n";
		}
	}
	// Release memory
	delete[] memory;
	//	for (j = 0; j < 16; j++) cout << j << "  " << traceData[j][350]<< "  " <<traceData[j][351] << "\n";
		//-------------------------------------------
		// Calculate RLI
		//
	long low, high;
	short max;

	for (i = 0; i < array_diodes; i++) {				//	high and low refer to early and late in the acquisition time - must be before the light is on!!
		// Low : 100~150
		for (low = 0, j = 100; j < 150; j++) {
			low += traceData[i][j];
		}
		low /= 50;

		// High : 325~375
		for (high = 0, j = 325; j < 375; j++) {
			high += traceData[i][j];
		}
		high /= 50;
		//	if(i<515)	cout << " MainConA line 102 " << i << "   " << double(low) / 3276.8<< "   " << double(high) / 3276.8 << "\n";
				// Max : 325~375
		for (max = 0, j = 325; j < 375; j++) {
			max = max(max, traceData[i][j]);
		}
		//	if (i % 100 == 1) cout << "MainCA line 112 " << i << "  " << low << "  " << high << "\n";
		dataArray->setRliLow(i, (short)low);
		dataArray->setRliHigh(i, (short)high);
		dataArray->setRliMax(i, max);
		if (i%500==1) cout << " MainContrAcqui line 113 diode " << i << "low " << low <<" high " << high << endl; //test showed that low is a dark frame for subtraction
	}

	// Release Memory
	//
	for (i = 0; i < array_diodes; i++) {
		delete[] traceData[i];
	}
	delete[] traceData;

	// Calculate RLI
	//
	dataArray->calRli();
	dataArray->setMaxRli();

	//-------------------------------------------
	// If RliDiv is set, process the data
	//
	if (sp->getRliDivFlag())
		dataArray->process();

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
	clock_t start, now;

	//-------------------------------------------
	// Get Number of Trials and the Interval between two Trials
	int numTrials = recControl->getNumTrials();				// Number of Trials
	int interval = recControl->getIntTrials() * 1000;			// sec -> msec
	int numPts = dapControl->getNumPts();						// Number of Points per Trace
	int num_diodes = dataArray->num_raw_diodes();
	// Note: previously, OG PhotoZ had a feature for number of "skipped trials"

	//-------------------------------------------
	// Allocate Memory
	Camera cam;
	unsigned short* memory = cam.allocateImageMemory(num_diodes, numPts);
	int16 *fp_memory = new(std::nothrow) int16[(size_t)((numPts + 1) * NUM_PDV_CHANNELS)];

	//-------------------------------------------
	// Trial Acquisition Loop
	dapControl->setStopFlag(0); // Set Stop Flag to 0

	int totalTrials = numTrials;
	int trialCount = 0;
	int trialNo = 0;

	clock_t fr_st, fr_end;

	for (; trialCount < totalTrials && !dapControl->getStopFlag(); trialCount++)
	{
		printf("Trial %d\n", trialCount);

		//-------------------------------------------
		// Time Management
		if (trialCount == 0)// start clock on first Trial
		{
			start = clock();

			time_t t;
			time(&t);
			recControl->setTime(t);
		}
		else // Check time on second trial and so forth
		{
			now = clock();
			Fl::check();

			while ((now - start) < (interval * trialCount))
			{
				now = clock();
				Fl::check();

				if (dapControl->getStopFlag())
				{
					break;
				}
			}
		}

		if (dapControl->getStopFlag()) break;

		fr_st = clock();
		int timeouts = dapControl->acqui(memory, fp_memory);
		fr_end = clock();

		double runtime = (double)(fr_end - fr_st) / CLOCKS_PER_SEC;
		printf("runtime: %f\n", runtime);
		if (timeouts)
			fl_alert("Main Cont Acq line 244 Timeouts ocurred! Check camera and connections.");

		// Arrange Data
		dataArray->arrangeData(trialNo, memory);
		dataArray->loadTrialData(trialNo + 1);
		dataArray->process();

		// Set User Interface
		recControl->setTrialNo(trialNo + 1);
		ui->trialNo->value(i2txt(recControl->getTrialNo()));

		// Redraw
		redraw();

		trialNo++;

		/* Now that numSkippedTrials is disabled, "Pseudo-Recording" is also disabled 
		else // Pseudo-Recording
		{
			// Send Dap File to the DAP board
			int status = dapControl->sendFile2Dap("PseudoRecord-820 v5.dap");

			if (status == 0)// Failed to Send Dap File
			{
				fl_alert("MCA line 267 Failed to Send DAP files to DAPs!\n");
				goto error;
			}

			dapControl->pseudoAcqui();
		}*/

		// dapControl->releaseDAPs();
	}

	// Release Memory
	delete[] memory;

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
	return;

error:
	delete[] memory;
	// dapControl->releaseDAPs();
	return;
}

//=============================================================================
void MainController::sleep(double seconds)
{
	clock_t start = clock();
	clock_t now = clock();

	while (now < (start + 1000 * seconds))
	{
		now = clock();
	}
}

//=============================================================================
// acqui() is executed after clicking Acquisition Button
//
void MainController::record()
{
	int i;

	int interval = recControl->getIntRecords() * 1000;	// sec -> m sec
	int numRecords = recControl->getNumRecords();
	int num = 0;
	clock_t start;
	clock_t now;

	dc->setStopFlag(0);
	increaseNo(RECORD);

	// Do the first recording
	
	start = clock();
	acquiOneRecord();

	if (dc->getScheduleRliFlag())
	{
		takeRli();
	}

	saveData2File();


	// Do the rest recordings
	for (i = 1; i < numRecords; i++)
	{
		now = clock();

		while ((now - start) < (interval * i))
		{
			now = clock();
			Fl::check();

			if (!dc->getScheduleFlag() || dc->getStopFlag())
			{
				return;
			}
		}

		if (!dc->getScheduleFlag() || dc->getStopFlag())
		{
			return;
		}

		increaseNo(RECORD);

		{
			acquiOneRecord();

			if (dc->getScheduleRliFlag())
			{
				takeRli();
			}

			saveData2File();
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
	value = atof(input);

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

void MainController::setRliScalar(const char* input)
{
	double value;
	value = atof(input);

	dataArray->setRliScalar(value);
	ui->rliScalarRoller->value(value);
	redraw();
}
void MainController::setRliScalar(double value)
{
	char txt[32];
	dataArray->setRliScalar(value);
	_gcvt_s(txt, 32, value, 6);
	ui->rliScalarTxt->value(txt);
	redraw();
}

//=============================================================================
void MainController::setFittingVarAllDiodes()
{
	int i;
	double fittingVar[5];
	bool changeFlag[5];

	fittingVar[0] = ui->alphaStartPointRoller->value();
	fittingVar[1] = ui->alphaAmplitudeSlider->value();
	fittingVar[2] = ui->alphaTauSlider->value();
	fittingVar[3] = ui->R2Slider->value();
	fittingVar[4] = ui->R1Slider->value();

	for (i = 0; i < 5; i++)
	{
		changeFlag[i] = tw->getChangeBits(i);
	}

	Data* data;

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		data = dataArray->getData(i);
		data->setAllFittingVar(fittingVar, changeFlag);
	}
}

//=============================================================================
void MainController::setAutoDetectSpike(bool flag)
{
	int i;
	Data* data;

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		data = dataArray->getData(i);
		data->setAutoDetectSpike(flag);
	}

	int diodeNo = tw->getLastDiodeNo();

	if (diodeNo >= -NUM_FP_DIODES)
	{
		tw->setFittingGadgets(diodeNo);
	}
}

//=============================================================================
void MainController::setAlphaSpikeStart(const char* txt)
{
	int diodeNo = tw->getLastDiodeNo();

	if (diodeNo < -NUM_FP_DIODES)
	{
		return;
	}

	Data* data = dataArray->getData(diodeNo);
	data->setAlphaSpikeStart(atoi(txt));
}

//=============================================================================
void MainController::setAlphaSpikeEnd(const char* txt)
{
	int diodeNo = tw->getLastDiodeNo();

	if (diodeNo < -NUM_FP_DIODES)
	{
		return;
	}

	Data* data = dataArray->getData(diodeNo);
	data->setAlphaSpikeEnd(atoi(txt));
}

//=============================================================================
void MainController::initializeFittingAmp()
{
	int i;
	Data* data;

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		data = dataArray->getData(i);
		data->initializeFittingAmp();
	}

	int diodeNo = tw->getLastDiodeNo();

	if (diodeNo >= -NUM_FP_DIODES)
	{
		tw->setFittingGadgets(diodeNo);
	}
}

//=============================================================================
