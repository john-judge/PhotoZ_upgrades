//=============================================================================
// Data.cpp
//=============================================================================
#include <math.h>

#include "Definitions.h"
#include "Data.h"
#include "DapController.h"
#include "RecControl.h"
#include "SignalProcessor.h"
#include "ArrayWindow.h"
#include "TraceWindow.h"

//=============================================================================
double Data::perAmp=0.5;

void Data::setPerAmp(double input)
{
	perAmp=input;
}

//=============================================================================
Data::Data(char flag)
{
	fpFlag=flag;
	ignoredFlag=0;
	correctionValue=1;
	maxAmpLatency=0;

	allocMem();
	reset();
	resetTimeCourse();

	fittingVar[0]=160;

	for(int i=1;i<5;i++)
	{
		fittingVar[i]=0;
	}

	spikeStart=spikeEnd=0;

	autoDetectSpike=1;
	maxSlope=0;
	spikeAmp=0;
}

//=============================================================================
Data::~Data()
{
	releaseMem();
}

//=============================================================================
void Data::resetTimeCourse()
{
	int i;
	for(i=0;i<Max_Num_Files;i++)
	{
		rliArray[i]=0;				// RLI Value
		ampArray[i]=0;				// Amplitude
		maxAmpArray[i]=0;			// Maximal Amplitude
		maxAmpLatencyArray[i]=0;	// Maximal Amplitude Latency
		halfAmpLatencyArray[i]=0;	// Half Amplitude Latency
	}
}

//=============================================================================
// Memory Manipulation
//
void Data::allocMem()
{
	int numPts=dc->getNumPts();

	rawData=new double[numPts];
	proData=new double[numPts];
	savData=new double[numPts];
	slope=new double[numPts];

	resetSavData();
}

//=============================================================================
void Data::releaseMem()
{
	delete [] rawData;
	delete [] proData;
	delete [] savData;
	delete [] slope;
}

//=============================================================================
// Reset Data and Properties
//
void Data::reset()
{
	int numPts=dc->getNumPts();

	for(int i=0;i<numPts;i++)
	{
		rawData[i]=0;
		proData[i]=0;
		slope[i]=0;
	}

	// Reset RLI and Properties
	rliLow=0;
	rliHigh=0;
	rliMax=0;
	rli=1.0e-10;

	maxAmp=0;
	maxAmpLatency=0;
	halfAmpLatency=0;
}

//=============================================================================
void Data::resetSavData()
{
	int i;
	int numPts=dc->getNumPts();

	for(i=0;i<numPts;i++)
	{
		savData[i]=0;
	}
}

//=============================================================================
double *Data::getRawDataMem()
{
	return rawData;
}

//=============================================================================
double *Data::getProDataMem()
{
	return proData;
}

//=============================================================================
double *Data::getSavDataMem()
{
	return savData;
}

//=============================================================================
double *Data::getSlopeMem()
{
	return slope;
}

//=============================================================================
void Data::setRliLow(short p)
{
	rliLow=p;
}

//=============================================================================
short Data::getRliLow()
{
	return rliLow;
}

//=============================================================================
void Data::setRliHigh(short p)
{
	rliHigh=p;
}

//=============================================================================
short Data::getRliHigh()
{
	return rliHigh;
}

//=============================================================================
void Data::setRliMax(short p)
{
	rliMax=p;
}

//=============================================================================
short Data::getRliMax()
{
	return rliMax;
}

//=============================================================================
void Data::calRli()
{
	int i;
	for(i=0;i<Num_Diodes;i++)
	{
		rli=double(rliLow-rliHigh)/3276.8;
	}

	if(rli<=0 || (rliMax-rliHigh)<-200)	// No RLI or Saturated
	{
		rli=-1;
	}
}

//=============================================================================
double Data::getRli()
{
	return rli;
}

//=============================================================================
// Measure Data Properties
//
void Data::measureProperties()
{
	int i;
	int start=sp->getStartWindow();
	int width=sp->getWidthWindow();
	double intPts=dc->getIntPts();
	int recordNo=recControl->getRecordNo()-1;
	int numPts=dc->getNumPts();

	//-------------------------------------------------------
	// 1. RLI
	rliArray[recordNo]=rli;

	//-------------------------------------------------------
	// 2. Max Amp
	// 3. Max Amp Latency

	maxAmp=0;
	maxAmpLatency=start+width;

	for(i=start;i<=start+width && i<numPts;i++)
	{
		if(proData[i]>maxAmp)
		{
			maxAmp=proData[i];
			maxAmpLatency=i;
		}
	}

	maxAmpArray[recordNo]=maxAmp;

	//-------------------------------------------------------
	// 4. Amplitude at the point line.
	int linePt=tw->getPointLinePt();
	if(linePt>=numPts)
	{
		linePt=numPts;
	}
	ampArray[recordNo]=proData[linePt];

	//-------------------------------------------------------
	// 5. Half Amp Latency
	if(maxAmp==0)
	{
		halfAmpLatency=start;
		return;
	}

	double halfAmp=maxAmp*perAmp;
	halfAmpLatency=start;

	for(i=start;i<=maxAmpLatency;i++)
	{
		if(proData[i]==halfAmp)
		{
			halfAmpLatency=i;
			break;
		}
		else if(proData[i]>halfAmp)
		{
			halfAmpLatency=i-1+(halfAmp-proData[i-1])/(proData[i]-proData[i-1]);
			break;
		}
	}

	//-------------------------------------------------------
	maxAmpLatencyPt=maxAmpLatency;
	maxAmpLatency=maxAmpLatency*intPts;
	maxAmpLatencyArray[recordNo]=maxAmpLatency;
	halfAmpLatency=halfAmpLatency*intPts;
	halfAmpLatencyArray[recordNo]=halfAmpLatency;

	//-------------------------------------------------------
	// Slope
	int maxSlopePt=maxAmpLatencyPt;
	maxSlope=slope[maxAmpLatencyPt];

	for(i=maxAmpLatencyPt-1;i>=start;i--)
	{
		if(slope[i]>maxSlope)
		{
			maxSlopePt=i;
			maxSlope=slope[i];
		}
	}

	int minSlopePt=maxAmpLatencyPt;
	double minSlope=slope[maxAmpLatencyPt];

	for(i=maxAmpLatencyPt+1;i<=start+width;i++)
	{
		if(slope[i]<minSlope)
		{
			minSlopePt=i;
			minSlope=slope[i];
		}
		else
		{
			break;
		}
	}

	if(autoDetectSpike)
	{
		for(i=maxSlopePt-1;i>=start;i--)
		{
			if(slope[i]<maxSlope/2)
			{
				spikeStart=i;
				break;
			}
		}

		for(i=minSlopePt+1;i<=start+width;i++)
		{
			if(slope[i]>minSlope/2)
			{
				spikeEnd=i;
				break;
			}
		}
	}
}

//=============================================================================
void Data::measureAmp()
{
	int recordNo=recControl->getRecordNo()-1;
	ampArray[recordNo]=proData[tw->getPointLinePt()];
}

//=============================================================================
double Data::getMaxAmp()
{
	return maxAmp;
}

//=============================================================================
double Data::getMaxAmp(int recordIndex)
{
	return maxAmpArray[recordIndex];
}

//=============================================================================
double Data::getMaxAmpLatency()
{
	return maxAmpLatency;
}

//=============================================================================
int Data::getMaxAmpLatencyPt()
{
	return maxAmpLatencyPt;
}

//=============================================================================
double Data::getMaxAmpLatency(int input)
{
	return maxAmpLatencyArray[input];
}

//=============================================================================
double Data::getHalfAmpLatency(int input)
{
	return halfAmpLatencyArray[input];
}

//=============================================================================
double Data::getHalfAmpLatency()
{
	return halfAmpLatency;
}

//=============================================================================
double Data::getAmp(int pt)
{
	return 	proData[pt];
}

//=============================================================================
double Data::getSD()
{
	int i;
	int num=50;
	int startPt=5;
	double sum2=0,sum1=0;
	double data;

	for(i=startPt;i<startPt+num;i++)
	{
		data=proData[i];
		sum1+=data;
		sum2+=data*data;
	}

	return sqrt((sum2-sum1*sum1/num)/(num-1));
}

//=============================================================================
double* Data::getRliArray()
{
	return rliArray;
}

//=============================================================================
double* Data::getAmpArray()
{
	return ampArray;
}

//=============================================================================
double* Data::getMaxAmpArray()
{
	return maxAmpArray;
}

//=============================================================================
double* Data::getMaxAmpLatencyArray()
{
	return maxAmpLatencyArray;
}

//=============================================================================
double* Data::getHalfAmpLatencyArray()
{
	return halfAmpLatencyArray;
}

//=============================================================================
double Data::getRli(int input)
{
	return rliArray[input];
}

//=============================================================================
void Data::ignore()
{
	if(ignoredFlag)
	{
		ignoredFlag=0;
	}
	else
	{
		ignoredFlag=1;
	}
}

//=============================================================================
void Data::setIgnoredFlag(char flag)
{
	ignoredFlag=flag;
}

//=============================================================================
char Data::getIgnoredFlag()
{
	return ignoredFlag;
}

//=============================================================================
void Data::clearIgnoredFlag()
{
	ignoredFlag=0;
}

//=============================================================================
void Data::saveTraces2()
{
	int i;
	int numPts=dc->getNumPts();

	for(i=0;i<numPts;i++)
	{
		savData[i]=proData[i];
	}
}

//=============================================================================
char Data::getFpFlag()
{
	return fpFlag;
}

//=============================================================================
void Data::setCorrectionValue(double value)
{
	correctionValue=value;
}

//=============================================================================
double Data::getCorrectionValue()
{
	return correctionValue;
}

//=============================================================================
void Data::normalization()
{
	int i;
	int numPts=dc->getNumPts();

	for(i=0;i<numPts;i++)
	{
		if(maxAmp>0)
		{
			proData[i]/=maxAmp;
		}
	}
}

//=============================================================================
void Data::inverseData()
{
	int i;
	int numPts=dc->getNumPts();

	if(ignoredFlag)
	{
		for(i=0;i<numPts;i++)
		{
			proData[i]=0;
		}
	}
	else
	{
		for(i=0;i<numPts;i++)
		{
			proData[i]=-rawData[i];
		}
	}
}

//=============================================================================
void Data::rliDividing()
{
	if(ignoredFlag)
	{
		return;
	}

	int i;
	int numPts=dc->getNumPts();

	for(i=0;i<numPts;i++)
	{
		proData[i]/=rli;
	}
}

//=============================================================================
void Data::raw2pro()
{
	int i;
	int numPts=dc->getNumPts();

	if(ignoredFlag)
	{
		for(i=0;i<numPts;i++)
		{
			proData[i]=0;
		}
	}
	else
	{
		for(i=0;i<numPts;i++)
		{
			proData[i]=rawData[i];
		}
	}
}

//=============================================================================
void Data::temporalFiltering(double* buf)
{
	if(ignoredFlag)
	{
		return;
	}

	int i;
	int numPts=dc->getNumPts();

	for(i=0;i<numPts;i++)
	{
		buf[i]=proData[i];
	}

	sp->temporalFiltering(buf,proData);
}

//=============================================================================
void Data::ampCorrecting()
{
	if(ignoredFlag)
	{
		return;
	}

	int i;
	int numPts=dc->getNumPts();

	for(i=0;i<numPts;i++)
	{
		proData[i]/=correctionValue;
	}
}

//=============================================================================
void Data::setAllFittingVar(double* input,bool* changeFlag)
{
	int i;

	for(i=0;i<5;i++)
	{
		if(changeFlag[i])
		{
			fittingVar[i]=input[i];
		}
	}
}

//=============================================================================
void Data::setFittingVar(int index,double value)
{
	fittingVar[index]=value;
}

//=============================================================================
double* Data::getFittingVar()
{
	return fittingVar;
}

//=============================================================================
int Data::getAlphaSpikeStart()
{
	return spikeStart;
}

//=============================================================================
int Data::getAlphaSpikeEnd()
{
	return spikeEnd;
}

//=============================================================================
void Data::setAlphaSpikeStart(int input)
{
	spikeStart=input;
}

//=============================================================================
void Data::setAlphaSpikeEnd(int input)
{
	spikeEnd=input;
}

//=============================================================================
void Data::setAutoDetectSpike(bool flag)
{
	autoDetectSpike=flag;

	if(flag)
	{
		measureProperties();
	}
}

//=============================================================================
double Data::getMaxSlope()
{
	return maxSlope;
}

//=============================================================================
void Data::initializeFittingAmp()
{
	fittingVar[1]=tw->revConvertFittingVar(1,maxAmp);
}

//=============================================================================
void Data::setSpikeAmp(double input)
{
	spikeAmp=input;
}

//=============================================================================
double Data::getSpikeAmp()
{
	return spikeAmp;
}

//=============================================================================
