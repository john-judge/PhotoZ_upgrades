//=============================================================================
// DataArray.cpp
//=============================================================================
#include <stdlib.h>		// _gcvt()
#include <stdio.h>
#include <math.h>
#include <fstream>

#include <FL/Fl_File_Chooser.H>

#include "Definitions.h"
#include "DataArray.h"
#include "DapController.h"
#include "Data.h"
#include "SignalProcessor.h"
#include "RecControl.h"
#include "ArrayWindow.h"
#include "TraceWindow.h"
#include "Diode.h"
#include "UserInterface.h"

extern int Ch_IP[];

//=============================================================================
// Constructor and Destructor
//=============================================================================
DataArray::DataArray(int input)
{
	int i;

	compareFlag=0;
	nor2ArrayMaxFlag=1;
	increaseFlag=1;
	absFlag=1;
	normalizationFlag=0;
	averageFlag=0;
	numAveRec=5;

	record1No=0;
	record2No=0;

	numPts=input;
	numTrials=1;
	maxRli=0;

	// Latency
	latencyStart=100;
	latencyWindow=5;

	for(i=0;i<Num_Diodes;i++)
	{
		trialData[i][0]=new short[numPts];
	}

	for(i=0;i<Num_Diodes-8;i++)
	{
		data[i]=new Data(0);
	}

	for(i=Num_Diodes-8;i<Num_Diodes;i++)
	{
		data[i]=new Data(1);
	}
}

//=============================================================================
DataArray::~DataArray()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		delete data[i];
	}

	releaseMem();
}

//=============================================================================
// rawData Management
//=============================================================================
void DataArray::allocMem()
{
	int i,j,k;

	for(j=0;j<Num_Diodes;j++)
	{
		for(i=0;i<numTrials;i++)
		{
			trialData[j][i]=new short[numPts];

			for(k=0;k<numPts;k++)
			{
				trialData[j][i][k]=0;
			}
		}
	}
}

//=============================================================================
void DataArray::releaseMem()
{
	int i,j;
	for(j=0;j<Num_Diodes;j++)
	{
		for(i=0;i<numTrials;i++)
		{
			delete [] trialData[j][i];
		}
	}
}

//=============================================================================
void DataArray::changeMemSize(int numTrials,int numPts)
{	
	if(this->numTrials==numTrials && this->numPts==numPts)
	{
		return;
	}

	//----------------------------
	// Change rawData size of data
	if(this->numPts!=numPts)
	{
		for(int i=0;i<Num_Diodes;i++)
		{
			data[i]->releaseMem();
			data[i]->allocMem();
		}
	}

	//----------------------------
	releaseMem();

	this->numTrials=numTrials;
	this->numPts=numPts;

	allocMem();
	resetData();
}

//=============================================================================
void DataArray::changeNumTrials(int numTrials)
{	
	if(this->numTrials==numTrials)
	{
		return;
	}

	releaseMem();
	this->numTrials=numTrials;
	allocMem();
	resetData();
}

//=============================================================================
void DataArray::changeNumPts(int numPts)
{	
	if(this->numPts==numPts)
	{
		return;
	}

	releaseMem();
	this->numPts=numPts;
	allocMem();

	//----------------------------
	// Change rawData size of data
	for(int i=0;i<Num_Diodes;i++)
	{
		data[i]->releaseMem();
		data[i]->allocMem();
	}

	resetData();
}

//=============================================================================
void DataArray::resetData()
{
	int i,j,k;

	for(j=0;j<Num_Diodes;j++)
	{
		for(i=0;i<numTrials;i++)// Reset rawData
		{
			for(k=0;k<numPts;k++)
			{
				trialData[j][i][k]=0;
			}
		}
	}

	for(i=0;i<Num_Diodes;i++)// Reset Data
	{
		data[i]->reset();
	}

	// Reset Properties
	maxRli=0;
}

//=============================================================================
Data *DataArray::getData(int index)
{
	return data[index];
}

//=============================================================================
short *DataArray::getMem(int trialNo,int diodeNo)
{
	return trialData[diodeNo][trialNo];
}

//=============================================================================
double *DataArray::getRawDataMem(int diodeNo)
{
	return data[diodeNo]->getRawDataMem();
}

//=============================================================================
double *DataArray::getProDataMem(int p)
{
	return data[p]->getProDataMem();
}

//=============================================================================
double *DataArray::getSavDataMem(int p)
{
	return data[p]->getSavDataMem();
}

//=============================================================================
double *DataArray::getSlopeMem(int p)
{
	return data[p]->getSlopeMem();
}

//=============================================================================
// RLI Processing
//=============================================================================
void DataArray::setRliLow(short *dataBlock)
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->setRliLow(dataBlock[i]);
	}
}

//=============================================================================
void DataArray::setRliHigh(short *dataBlock)
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->setRliHigh(dataBlock[i]);
	}
}

//=============================================================================
void DataArray::setRliMax(short *dataBlock)
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->setRliMax(dataBlock[i]);
	}
}

//=============================================================================
void DataArray::setRliLow(int diodeNo,short rli)
{
	data[diodeNo]->setRliLow(rli);
}

//=============================================================================
void DataArray::setRliHigh(int diodeNo,short rli)
{
	data[diodeNo]->setRliHigh(rli);
}

//=============================================================================
void DataArray::setRliMax(int diodeNo,short rli)
{
	data[diodeNo]->setRliMax(rli);
}

//=============================================================================
short DataArray::getRliLow(int index)
{
	return data[index]->getRliLow();
}

//=============================================================================
short DataArray::getRliHigh(int index)
{
	return data[index]->getRliHigh();
}

//=============================================================================
short DataArray::getRliMax(int index)
{
	return data[index]->getRliMax();
}

//=============================================================================
void DataArray::calRli()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->calRli();
	}
}

//=============================================================================
void DataArray::setMaxRli()
{
	int i;
	double tmp;
	maxRli=0;

	for(i=0;i<Num_Diodes;i++)
	{
		tmp=data[i]->getRli();
		if(tmp>maxRli)
		{
			maxRli=tmp;
		}
	}
}

//=============================================================================
double DataArray::getRli(int diodeNo)
{
	return data[diodeNo]->getRli();
}

//=============================================================================
// Output the ratio of RLI to maximum RLI
//=============================================================================
double DataArray::getRliRatio(int p)
{
	return data[p]->getRli()/maxRli;
}

//=============================================================================
double DataArray::getDataMaxAmp(int diodeIndex,int recordNoChoice)
{
	// Determine which record is going to processed.
	int recordIndex;

	if(recordNoChoice==0)
	{
		recordIndex=recControl->getRecordNo()-1;
	}
	else if(recordNoChoice==1)
	{
		recordIndex=record1No;
	}
	else if(recordNoChoice==2)
	{
		recordIndex=record2No;
	}

	// Calculation
	double maxAmp=0;

	if(averageFlag && recordIndex>=(numAveRec-1))
	{
		int i;
		double sum=0;

		for(i=0;i<numAveRec;i++)
		{
			sum+=data[diodeIndex]->getMaxAmp(recordIndex-i);
		}

		maxAmp=sum/numAveRec;
	}
	else
	{
		maxAmp=data[diodeIndex]->getMaxAmp(recordIndex);
	}

	return maxAmp;
}

//=============================================================================
double DataArray::getDiodeMaxAmpSD(int diodeIndex,int recordNoChoice)
{
	// Determine which record is going to processed.
	int recordIndex;

	if(recordNoChoice==0)
	{
		recordIndex=recControl->getRecordNo()-1;
	}
	else if(recordNoChoice==1)
	{
		recordIndex=record1No;
	}
	else if(recordNoChoice==2)
	{
		recordIndex=record2No;
	}
	else if(recordNoChoice==3)
	{
		recordIndex=record2No;
	}

	// Calculation
	double sd=0;

	if(recordIndex>=(numAveRec-1))
	{
		int i;
		double sum=0;

		// Calculate the mean
		for(i=0;i<numAveRec;i++)
		{
			sum+=data[diodeIndex]->getMaxAmp(recordIndex-i);
		}

		double mean=sum/numAveRec;

		// Calculate the sum of squares
		for(i=0,sum=0;i<numAveRec;i++)
		{
			sum+=pow(data[diodeIndex]->getMaxAmp(recordIndex-i)-mean,2);
		}

		// Calculate SD (Standard Deviation)
		sd=sqrt(sum/numAveRec);
	}

	return sd;
}

//=============================================================================
// Max Amp Change
double DataArray::getDiodeMaxAmpCha(int diodeIndex,int choice)
{
	double maxAmpCha=0;

	if(choice==2)
	{
		maxAmpCha=getDataMaxAmp(diodeIndex,3) - getDataMaxAmp(diodeIndex,2);
	}
	else
	{
		maxAmpCha=getDataMaxAmp(diodeIndex,2) - getDataMaxAmp(diodeIndex,1);
	}
	return maxAmpCha;
}

//=============================================================================
double DataArray::getDiodeMaxAmpChaSD(int diodeIndex)
{
	double sd=0;

	if(record2No>=(numAveRec-1))
	{
		int i;
		double sum=0;

		// Calculate the mean
		for(i=0;i<numAveRec;i++)
		{
			sum+=data[diodeIndex]->getMaxAmp(record2No-i);
		}

		double mean=sum/numAveRec;

		// Calculate the sum of squares
		for(i=0,sum=0;i<numAveRec;i++)
		{
			sum+=pow(data[diodeIndex]->getMaxAmp(record2No-i)-mean,2);
		}

		// Calculate SD (Standard Deviation)
		sd=sqrt(sum/numAveRec);
	}

	return sd;
}

//=============================================================================
// Max Amp Percentage Change
double DataArray::getDiodeMaxAmpPerCha(int diodeIndex)
{
	double maxAmp1=getDataMaxAmp(diodeIndex,1);
	double maxAmp2=getDataMaxAmp(diodeIndex,2);

	if(maxAmp1==0)
	{
		return -1000;
	}
	else
	{
		return 100.0*(maxAmp2-maxAmp1)/maxAmp1;
	}
}

//=============================================================================
// Max Amp Percentage Change Standard Error
double DataArray::getDiodeMaxAmpPerChaSD(int diodeIndex)
{
	double sd=0;

	if(record2No>=(numAveRec-1))
	{
		int i;
		double sum;
		double per[10];
		double maxAmp1=getDataMaxAmp(diodeIndex,1);

		// Calculate the mean
		for(i=0,sum=0;i<numAveRec;i++)
		{
			per[i]=100.0*(data[diodeIndex]->getMaxAmp(record2No-i)-maxAmp1)/maxAmp1;
			sum+=per[i];
		}

		double mean=sum/numAveRec;

		// Calculate the sum of squares
		for(i=0,sum=0;i<numAveRec;i++)
		{
			sum+=pow(per[i]-mean,2);
		}

		// Calculate SD (Standard Deviation)
		sd=sqrt(sum/numAveRec);
	}

	return sd;
}

//=============================================================================
double DataArray::getDataMaxAmpLatency(int index)
{
	return data[index]->getMaxAmpLatency();
}

//=============================================================================
int DataArray::getDataMaxAmpLatencyPt(int index)
{
	return data[index]->getMaxAmpLatencyPt();
}

//=============================================================================
double DataArray::getDataHalfAmpLatency(int index)
{
	return data[index]->getHalfAmpLatency();
}

//=============================================================================
double DataArray::getAmp(int diodeNo,int ptNo)
{
	return data[diodeNo]->getAmp(ptNo);
}

//=============================================================================
double DataArray::getDataSD(int diodeNo)
{
	return data[diodeNo]->getSD();
}

//=============================================================================
char DataArray::getFpFlag(int diodeNo)
{
	return data[diodeNo]->getFpFlag();
}

//=============================================================================
void DataArray::average()
{
	int i,j,k;
	double gain=recControl->getAcquiGain();
	double scale=1/(gain*3.2768);

	if(numTrials==1)
	{
		for(i=0;i<Num_Diodes;i++)
		{
			double *rawData=data[i]->getRawDataMem();

			for(j=0;j<numPts;j++)
			{
				rawData[j]=trialData[i][0][j]*scale;
			}
		}
	}
	else
	{
		scale/=numTrials;

		for(i=0;i<Num_Diodes;i++)
		{
			double *rawData=data[i]->getRawDataMem();

			for(j=0;j<numPts;j++)
			{
				rawData[j]=0;

				for(k=0;k<numTrials;k++)
				{
					rawData[j]+=trialData[i][k][j];
				}

				rawData[j]*=scale;
			}
		}
	}
}

//=============================================================================
void DataArray::loadTrialData(int trialNo)
{
	if(trialNo==0)
	{
		average();
	}
	else
	{
		int i,j;
		double gain=recControl->getAcquiGain();
		double scale=1/(gain*3.2768);

		trialNo-=1;

		for(i=0;i<Num_Diodes;i++)
		{
			double *rawData=data[i]->getRawDataMem();

			for(j=0;j<numPts;j++)
			{
				rawData[j]=trialData[i][trialNo][j]*scale;
			}
		}
	}
}

//=============================================================================
void DataArray::arrangeData(int trialNo,short* input)
{
	int i,j;

	for(i=0;i<Num_Diodes;i++)
	{
		for(j=0;j<numPts;j++)
		{
			trialData[i][trialNo][j]=input[Ch_IP[i]+j*512];
		}
	}
}

//=============================================================================
void DataArray::process()
{
	int i;

	sp->process();

	dataInversing();
	rliDividing();
	temporalFiltering();
	ampCorrecting();
	sp->spatialFilter();
	sp->baseLineCorrection();
	sp->calSlope();

	// Measure Properties
	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->measureProperties();
	}

	// Normalization
	if(normalizationFlag)
	{
		for(i=0;i<Num_Diodes;i++)
		{
			data[i]->normalization();
		}
	}
}

//=============================================================================
void DataArray::dataInversing()
{
	int i;

	if(sp->getDataInverseFlag())
	{
		for(i=0;i<464;i++)
		{
			data[i]->inverseData();
		}
	}
	else
	{
		for(i=0;i<464;i++)
		{
			data[i]->raw2pro();
		}
	}

	if(sp->getFpInverseFlag())
	{
		for(i=464;i<472;i++)
		{
			data[i]->inverseData();
		}
	}
	else
	{
		for(i=464;i<472;i++)
		{
			data[i]->raw2pro();
		}
	}

}

//=============================================================================
void DataArray::rliDividing()
{
	if(!sp->getRliDivFlag())
	{
		return;
	}

	int i;

	for(i=0;i<464;i++)
	{
		data[i]->rliDividing();
	}
}

//=============================================================================
void DataArray::temporalFiltering()
{
	if(!sp->getTemporalFilterFlag())
	{
		return;
	}

	int i;
	double* buf=new double[numPts];

	sp->setGaussianWeights();

	for(i=0;i<472;i++)
	{
		data[i]->temporalFiltering(buf);
	}

	delete [] buf;
}

//=============================================================================
void DataArray::ampCorrecting()
{
	if(!sp->getCorrectionFlag())
	{
		return;
	}

	int i;

	for(i=0;i<464;i++)
	{
		data[i]->ampCorrecting();
	}
}

//=============================================================================
void DataArray::doSelection(int property)
{
	int i;
	double threshold=atof(ui->thresholdTxt->value());

	aw->clearSelected();

	if(property==-1)	// Max Amp < Threshold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getDataMaxAmp(i)<threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
	else if(property==1)	// Max Amp > Threshold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getDataMaxAmp(i)>threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
	else if(property==-2)	// % Amp Latency < threshold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getDataHalfAmpLatency(i)<threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
	else if(property==2)	// % Amp Latency > threshold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getDataHalfAmpLatency(i)>threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
	else if(property==-3)	// SD < thredhold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getDataSD(i)<threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
	else if(property==3)	// SD > thredhold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getDataSD(i)>threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
	else if(property==-4)	// RLI < thredhold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getRli(i)<threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
	else if(property==4)	// RLI > thredhold
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			if(getRli(i)>threshold)
			{
				aw->selectDiode(i);
			}
		}
	}
}

//=============================================================================
void DataArray::clearIgnoredFlag()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->clearIgnoredFlag();
	}
}

//=============================================================================
char DataArray::getIgnoredFlag(int dataNo)
{
	return data[dataNo]->getIgnoredFlag();
}

//=============================================================================
void DataArray::setIgnoredFlag(int dataNo,char flag)
{
	data[dataNo]->setIgnoredFlag(flag);
}

//=============================================================================
void DataArray::ignore(int dataNo)
{
	data[dataNo]->ignore();
}

//=============================================================================
void DataArray::saveTraces2()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->saveTraces2();
	}
}

//=============================================================================
void DataArray::setCorrectionValue(int dataIndex,double value)
{
	data[dataIndex]->setCorrectionValue(value);
}

//=============================================================================
void DataArray::makeCorrection()
{
	int numSelectedDiodes=aw->getNumSelectedDiodes();
	if(numSelectedDiodes<2)
	{
		return;
	}

	int i;
	int* selectedDiodes=aw->getSelectedDiodes();
	int diodeStd=selectedDiodes[0];

	if(getDataMaxAmp(diodeStd)<=0)
	{
		return;
	}

	int diodeX;
	double correctionValue;

	for(i=1;i<numSelectedDiodes;i++)
	{
		diodeX=selectedDiodes[i];
		correctionValue=getCorrectionValue(diodeX)*getDataMaxAmp(diodeX)/getDataMaxAmp(diodeStd);
		setCorrectionValue(diodeX,correctionValue);
	}
}

//=============================================================================
double DataArray::getCorrectionValue(int index)
{
	return data[index]->getCorrectionValue();
}

//=============================================================================
void DataArray::setRecordXNo(int recordX,int recordXNo)
{
	if(recordX==1)
	{
		record1No=recordXNo;
	}
	else if(recordX==2)
	{
		record2No=recordXNo;
	}
}

//=============================================================================
int DataArray::getRecordXNo(int recordX)
{
	if(recordX==1)
	{
		return record1No;
	}
	else if(recordX==2)
	{
		return record2No;
	}

	return 1;
}

//=============================================================================
void DataArray::setCompareFlag(char input)
{
	compareFlag=input;
}

//=============================================================================
char DataArray::getCompareFlag()
{
	return compareFlag;
}

//=============================================================================
void DataArray::setNor2ArrayMaxFlag(char input)
{
	nor2ArrayMaxFlag=input;
}

//=============================================================================
void DataArray::setIncreaseFlag(char input)
{
	increaseFlag=input;
}

//=============================================================================
char DataArray::getIncreaseFlag()
{
	return increaseFlag;
}

//=============================================================================
void DataArray::setAbsFlag(char input)
{
	absFlag=input;
}

//=============================================================================
char DataArray::getAbsFlag()
{
	return absFlag;
}

//=============================================================================
void DataArray::setRli2DataFeature()
{
	int i;
	double maxValue=1.0e-9;
	double scale;

	if(!compareFlag)
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=getRli(i);

			if(dataFeature[i] > maxValue)
			{
				maxValue=dataFeature[i];
			}
		}

		if(nor2ArrayMaxFlag)
		{
			scale=1/maxValue;

			for(i=0;i<Num_Diodes-8;i++)
			{
				dataFeature[i]*=scale;
			}
		}
	}
	else
	{
		if(record1No==record2No)
		{
			resetDataFeature();
			return;
		}

		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=data[i]->getRli(record2No) - data[i]->getRli(record1No);

			dataFeature[i]=-dataFeature[i];

			if(!absFlag)
			{
				if(data[i]->getRli(record1No)==0)
				{
					dataFeature[i]/=1.0e-9;
				}
				else
				{
					dataFeature[i]/=data[i]->getRli(record1No);
				}
			}

			if(dataFeature[i] > maxValue)
			{
				maxValue=dataFeature[i];
			}
		}

		if(nor2ArrayMaxFlag)
		{
			scale=1/maxValue;

			for(i=0;i<Num_Diodes-8;i++)
			{
				dataFeature[i]*=scale;
			}
		}
	}
}

//=============================================================================
void DataArray::setAverageFlag(char input)
{
	averageFlag=input;
}

//=============================================================================
char DataArray::getAverageFlag()
{
	return averageFlag;
}

//=============================================================================
void DataArray::setMaxAmp2DataFeature()
{
	int i;

	double scale;
	double maxValue=1.0e-9;

	// Amplitude
	if(!compareFlag)
	{
		// Get Max Amp
		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=getDataMaxAmp(i);

			if(dataFeature[i] > maxValue)
			{
				maxValue=dataFeature[i];
			}
		}

		// Normalization if feature is on.
		if(nor2ArrayMaxFlag)
		{
			scale=1/maxValue;

			for(i=0;i<Num_Diodes-8;i++)
			{
				dataFeature[i]*=scale;
			}
		}
	}
	// Compare two records
	else
	{
		// Set Max Amp Change
		for(i=0;i<Num_Diodes-8;i++)
		{
			double maxAmp1;
			double maxAmp2;

			maxAmp1=getDataMaxAmp(i,1);
			maxAmp2=getDataMaxAmp(i,2);

			dataFeature[i]=maxAmp2-maxAmp1;

			if(!increaseFlag)
			{
				dataFeature[i]=-dataFeature[i];
			}

			if(!absFlag)
			{
				if(maxAmp1==0)
				{
					dataFeature[i]/=1.0e-9;
				}
				else
				{
					dataFeature[i]/=maxAmp1;
				}
			}

			if(dataFeature[i] > maxValue)
			{
				maxValue=dataFeature[i];
			}
		}

		if(nor2ArrayMaxFlag)
		{
			scale=1/maxValue;

			for(i=0;i<Num_Diodes-8;i++)
			{
				dataFeature[i]*=scale;
			}
		}
	}
}

//=============================================================================
void DataArray::setSpikeAmp2DataFeature()
{
	int i;

	double maxValue=1.0e-9;
	double scale;

	// Get Max Amp
	for(i=0;i<Num_Diodes-8;i++)
	{
		dataFeature[i]=data[i]->getSpikeAmp();

		if(dataFeature[i] > maxValue)
		{
			maxValue=dataFeature[i];
		}
	}

	// Normalization if feature is on.
	if(nor2ArrayMaxFlag)
	{
		scale=1/maxValue;

		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]*=scale;
		}
	}
}

//=============================================================================
void DataArray::setMaxAmpLatency2DataFeature()
{
	int i;

	if(!compareFlag)
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=(latencyStart+latencyWindow-getDataMaxAmpLatency(i))/latencyWindow;
		}
	}
	else // Compare
	{
		if(record1No==record2No)
		{
			resetDataFeature();
			return;
		}

		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=data[i]->getMaxAmpLatency(record2No)
				- data[i]->getMaxAmpLatency(record1No);

			if(!increaseFlag)
			{
				dataFeature[i]=-dataFeature[i];
			}

			dataFeature[i]/=latencyWindow;
		}
	}
}

//=============================================================================
void DataArray::setHalfAmpLatency2DataFeature()
{
	int i;

	if(!compareFlag)
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=getDataHalfAmpLatency(i);
		}
	}
	else // Compare
	{
		if(record1No==record2No)
		{
			resetDataFeature();
			return;
		}

		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=(data[i]->getHalfAmpLatency(record2No)
				- data[i]->getHalfAmpLatency(record1No));

			if(!increaseFlag)
			{
				dataFeature[i]=-dataFeature[i];
			}
		}
	}
}

//=============================================================================
void DataArray::setEPSPLatency2DataFeature()
{
	int i;
	double* fittingVar;
	double first=1000;
	double last=0;

	// Find first pt and last pt
	for(i=0;i<Num_Diodes-8;i++)
	{
		fittingVar=data[i]->getFittingVar();
		dataFeature[i]=fittingVar[0];

		if(dataFeature[i]>last)
		{
			last=dataFeature[i];
		}

		if(dataFeature[i]<first)
		{
			first=dataFeature[i];
		}
	}

	// Normalize
	double range=last-first;

	if(range==0)
	{
		range=20;
	}

	for(i=0;i<Num_Diodes-8;i++)
	{
		dataFeature[i]=(last-dataFeature[i])/range;
	}
}

//=============================================================================
double DataArray::getDataFeature(int input)
{
	return dataFeature[input];
}

//=============================================================================
void DataArray::resetDataFeature()
{
	int i;

	for(i=0;i<Num_Diodes-8;i++)
	{
		dataFeature[i]=0;
	}
}

//=============================================================================
void DataArray::measureAmp()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		data[i]->measureAmp();
	}
}

//=============================================================================
void DataArray::setNormalizationFlag(char input)
{
	normalizationFlag=input;
}

//=============================================================================
void DataArray::setNumAveRec(int num)
{
	numAveRec=num;
}

//=============================================================================
int DataArray::getNumAveRec()
{
	return numAveRec;
}

//=============================================================================
void DataArray::setLatencyStart(double input)
{
	latencyStart=input;

	setMaxAmpLatency2DataFeature();
	setHalfAmpLatency2DataFeature();
	setHalfAmpLatencyRatio2DataFeature();
}

//=============================================================================
double DataArray::getLatencyStart()
{
	return latencyStart;
}

//=============================================================================
void DataArray::setLatencyWindow(double input)
{
	latencyWindow=input;

	setMaxAmpLatency2DataFeature();
	setHalfAmpLatency2DataFeature();
	setHalfAmpLatencyRatio2DataFeature();
}

//=============================================================================
void DataArray::setHalfAmpLatencyRatio2DataFeature()
{
	int i;

	if(!compareFlag)
	{
		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=(latencyStart+latencyWindow-getDataHalfAmpLatency(i))/latencyWindow;
		}
	}
	else // Compare
	{
		if(record1No==record2No)
		{
			resetDataFeature();
			return;
		}

		for(i=0;i<Num_Diodes-8;i++)
		{
			dataFeature[i]=(data[i]->getHalfAmpLatency(record2No)
				- data[i]->getHalfAmpLatency(record1No))/latencyWindow;

			if(!increaseFlag)
			{
				dataFeature[i]=-dataFeature[i];
			}
		}
	}
}

//=============================================================================
