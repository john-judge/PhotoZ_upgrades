//=============================================================================
// UserInterfaceMethods.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()
#include <stdio.h>		// fprintf()

#include "UserInterface.h"
#include "ArrayWindow.h"
#include "TraceWindow.h"
#include "DapController.h"
#include "DapChannel.h"
#include "RecControl.h"
#include "FileController.h"
#include "SignalProcessor.h"
#include "ColorWindow.h"
#include "Definitions.h"

//=============================================================================
void UserInterface::init()
{
	mainWindow->fullscreen();
	BLCType->value(3);
	record1No->value("1");
	record2No->value("1");
	mapFeatureType->value(1);
	timeCourseType->value(2);

	//=====================================
	// Array
	//=====================================
	awYScaleTxt->value("100");	// Y Scale
	awFpYScaleTxt->value("100");	// FP Y Scale
	awXScaleTxt->value("1");	// X Scale
	awXShiftTxt->value("0");	// X Shift

	//=====================================
	// Trace
	//=====================================
	
	// Alpha function
	onsetLowerBoundTxt->value("160");
	onsetHigherBoundTxt->value("180");
	alphaStartPointTxt->value("160");
	alphaWindow->value("40");
	alphaAmpTxt->value("0.3");
	alphaTauTxt->value("4");
	R2Txt->value("0.01");
	R1Txt->value("1.0");
	alphaAxonStart->value("0");
	alphaAxonEnd->value("0");
	alphaSpikeStart->value("0");
	alphaSpikeEnd->value("0");
	spikeSlopeThreshold->value("0");
	spikeAmpThreshold->value("0");

	// Save to File
	twSaveTracesStartPt->value("0");
	twSaveTracesEndPt->value("1000");

	//=====================================
	// Signal Processing
	//=====================================
	startWindow->value("160");
	widthWindow->value("200");

	latencyStartTxt->value("100");
	thresholdTxt->value("1.0");

	//=====================================
	// Baseline
	//=====================================
	polynomialStartPtTxt->value("155");
	polynomialNumPts->value("400");
	clampPtTxt->value("155");

	//=====================================
	// Image
	//=====================================
	awImageX0Txt->value("0");	// Image X0
	awImageY0Txt->value("0");	// Image Y0

	//=====================================
	// Color Control
	//=====================================
	colorUpperBound->value("  1.00");
	colorLowerBound->value("  0.00");
	cwColorUpperBound->value("  1.00");
	cwColorLowerBound->value("  0.00");
	colorAmp->value("  1.00");
	colorCenter->value("  0.00");
	cwColorAmp->value("  1.00");
	cwColorCenter->value("  0.00");

	//=====================================
	// Map & Movie
	//=====================================
	cwCurrentPtText->value("160");
	cwStartPt->value("160");
	cwEndPt->value("200");

	// Trial Control
	numSkippedTrials->value("0");
}

//=============================================================================
void UserInterface::setValue()
{
	char buff[32];

	//=====================================
	// File
	//=====================================
	fileName->value(fileController->getFileName());

	recControl->checkFileRange();

	sliceNo->value(i2txt(recControl->getSliceNo()));
	sliceNoMax->value(i2txt(recControl->getSliceNoMax()));

	locationNo->value(i2txt(recControl->getLocationNo()));
	locationNoMax->value(i2txt(recControl->getLocationNoMax()));

	recordNo->value(i2txt(recControl->getRecordNo()));
	recordNoMax->value(i2txt(recControl->getRecordNoMax()));

	trialNo->value(i2txt(recControl->getTrialNo()));
	trialNoMax->value(i2txt(recControl->getTrialNoMax()));

	//=====================================
	// DAP
	//=====================================
	samplingRate->value(dc->getSamplingRateTxt());

	resetOnset->value(dc->reset->getOnsetTxt());
	resetDuration->value(dc->reset->getDurationTxt());

	shutterOnset->value(dc->shutter->getOnsetTxt());
	shutterDuration->value(dc->shutter->getDurationTxt());

	sti1Onset->value(dc->sti1->getOnsetTxt());
	sti1Duration->value(dc->sti1->getDurationTxt());

	sti2Onset->value(dc->sti2->getOnsetTxt());
	sti2Duration->value(dc->sti2->getDurationTxt());

	acquiOnset->value(i2txt(dc->getAcquiOnset()));

	// Acquisition duration
	sprintf_s(buff, 32,"%5.0f",dc->getAcquiDuration());
	acquiDuration->value(buff);

	//===================
	numPts->value(i2txt(dc->getNumPts()));

	sprintf_s(buff, 32,"%4.2f",dc->getIntPts());
	intPts->value(buff);

	//===================
	intBursts1->value(i2txt(dc->getIntBursts(1)));
	numBursts1->value(i2txt(dc->getNumBursts(1)));
	intPulses1->value(i2txt(dc->getIntPulses(1)));
	numPulses1->value(i2txt(dc->getNumPulses(1)));

	intBursts2->value(i2txt(dc->getIntBursts(2)));
	numBursts2->value(i2txt(dc->getNumBursts(2)));
	intPulses2->value(i2txt(dc->getIntPulses(2)));
	numPulses2->value(i2txt(dc->getNumPulses(2)));

	//===================
	intRecords->value(i2txt(recControl->getIntRecords()));
	numRecords->value(i2txt(recControl->getNumRecords()));

	//===================
	numTrials->value(i2txt(recControl->getNumTrials()));
	intTrials->value(i2txt(recControl->getIntTrials()));

	acquiGain->value(i2txt(recControl->getAcquiGain()));
	rliGain->value(i2txt(recControl->getRliGain()));
}

//=============================================================================
