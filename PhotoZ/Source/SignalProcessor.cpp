//=============================================================================
// SignalProcessor.cpp
//=============================================================================
#include <iostream>
#include <math.h>

#include "Definitions.h"
#include "SignalProcessor.h"
#include "DataArray.h"
#include "Data.h"
#include "DapController.h"
#include "Filter.h"
#include "UserInterface.h"
#include "PolynomialFitting.h"

using namespace std;
//=============================================================================
SignalProcessor::SignalProcessor()
{
	polynormialFitter=new PolynomialFitting();
	filter=new Filter();

	dataInverseFlag=0;
	fpInverseFlag=0;
	rliDivFlag=0;
	BLCType='P';
	correctionFlag=0;

	// Filter
	temporalFilterFlag=0;
	spatialFilterFlag=0;
	centerWeight=exp(2.0);

	clampPt=155;

	startWindow=90;
	widthWindow=150;
}

//=============================================================================
SignalProcessor::~SignalProcessor()
{
	delete polynormialFitter;
	delete filter;
}

//=============================================================================
double* SignalProcessor::get_proData(int i)
{
	if (i < 0)
		return fp_proData[i + NUM_FP_DIODES];
	return proData[i];
}

//=============================================================================
void SignalProcessor::changeNumDiodes()
{
	proData.assign(dataArray->num_binned_diodes(), NULL);
	ignoreFlag.assign(dataArray->num_binned_diodes(), 0);
	
	memset(fp_proData, 0, NUM_FP_DIODES * sizeof(double*));
	memset(fp_ignoreFlag, 0, NUM_FP_DIODES * sizeof(char));
}

//=============================================================================
void SignalProcessor::setRliDivFlag(char p)
{
	rliDivFlag=p;
}

//=============================================================================
char SignalProcessor::getRliDivFlag()
{
	return rliDivFlag;
}

//=============================================================================
void SignalProcessor::setTemporalFilterFlag(char p)
{
	temporalFilterFlag=p;
}

//=============================================================================
char SignalProcessor::getTemporalFilterFlag()
{
	return temporalFilterFlag;
}

//=============================================================================
void SignalProcessor::setSpatialFilterFlag(char p)
{
	spatialFilterFlag=p;
}

//=============================================================================
void SignalProcessor::setSpatialFilterSigma(double sigma)
{
	centerWeight=exp(0.5/sigma/sigma);
}

//=============================================================================
void SignalProcessor::setTemporalFilterWidth(int width)
{
	filter->setWidth(width);
}

//=============================================================================
void SignalProcessor::setDataInverseFlag(char p)
{
	dataInverseFlag=p;
}

//=============================================================================
char SignalProcessor::getDataInverseFlag()
{
	return dataInverseFlag;
}

//=============================================================================
void SignalProcessor::setFpInverseFlag(char p)
{
	fpInverseFlag=p;
}

//=============================================================================
char SignalProcessor::getFpInverseFlag()
{
	return fpInverseFlag;
}

//=============================================================================
void SignalProcessor::setStartWindow(int p)
{
	startWindow=p;
}

//=============================================================================
int SignalProcessor::getStartWindow()
{
	return startWindow;
}

//=============================================================================
void SignalProcessor::setWidthWindow(int p)
{
	widthWindow=p;
}

//=============================================================================
int SignalProcessor::getWidthWindow()
{
	return widthWindow;
}

//=============================================================================
void SignalProcessor::setBLCType(char type)
{
	BLCType=type;
}

//=============================================================================
void SignalProcessor::setClampPt(int pt)
{
	clampPt=pt;
}

//=============================================================================
int SignalProcessor::getClampPt()
{
	return clampPt;
}

//=============================================================================
char SignalProcessor::getBLCType()
{
	return BLCType;
}

//=============================================================================
void SignalProcessor::setPolynomialStartPt(int pt)
{
	polynormialFitter->setStartPt(pt);
}

//=============================================================================
int SignalProcessor::getPolynomialStartPt()
{
	return polynormialFitter->getStartPt();
}

//=============================================================================
void SignalProcessor::setPolynomialNumPts(int pt)
{
	polynormialFitter->setNumPts(pt);
}

//=============================================================================
int SignalProcessor::getPolynomialNumPts()
{
	return polynormialFitter->getNumPts();
}

//=============================================================================
void SignalProcessor::setCorrectionFlag(char flag)
{
	correctionFlag=flag;
}

//=============================================================================
char SignalProcessor::getCorrectionFlag()
{
	return correctionFlag;
}

//=============================================================================
void SignalProcessor::process()
{
	int i;

	for (i = -NUM_FP_DIODES; i < 0; i++)
	{
		fp_ignoreFlag[i + NUM_FP_DIODES] = dataArray->getIgnoredFlag(i);
		fp_proData[i + NUM_FP_DIODES] = dataArray->getProDataMem(i);
	}

	for (i = 0; i < dataArray->num_binned_diodes(); i++)
	{
		ignoreFlag[i] = dataArray->getIgnoredFlag(i);
		proData[i] = dataArray->getProDataMem(i);
	}
}

//=============================================================================
void SignalProcessor::spatialFilter()
{
	//---------------
	// Check flag
	//---------------
	if(!spatialFilterFlag)
	{
		return;
	}

	//---------------
	// Copy Data
	//---------------
	int i,j;
	int num_bdiodes = dataArray->num_binned_diodes();
	int numPts=dc->getNumPts();

	double** tmpBuf=new double*[num_bdiodes];

	for (i = 0; i < num_bdiodes; i++)
	{
		tmpBuf[i]=new double[numPts];
	}

	for (i = 0; i < num_bdiodes; i++)
	{
		if(!ignoreFlag[i])
			memcpy(tmpBuf[i], proData[i], sizeof(double) * numPts);
	}

	//---------------
	// Filter
	//---------------
	double sum;
	int x, y;
	int diode, center_diode;
	int array_height, array_width;

	array_width = dataArray->binned_width();
	array_height = dataArray->binned_height();

	for (center_diode = 0; center_diode < num_bdiodes; center_diode++)
	{
		sum = 0;
		x = center_diode % array_width;
		y = center_diode / array_width;

		// Center diode
		if (!ignoreFlag[center_diode])
		{
			sum+=centerWeight;
			for (i = 0; i < numPts; i++)
				proData[center_diode][i] = tmpBuf[center_diode][i] * centerWeight;
		}

		// Neighbouring 8 diodes
		for (j = 0; j < 9; j++)
		{
			int xoffset = (j % 3) - 1;
			int yoffset = (j / 3) - 1;
			int diode_x = x + xoffset;
			int diode_y = y + yoffset;

			if (diode_x >= array_width || diode_x < 0)
				continue;
			if (diode_y >= array_height || diode_y < 0)
				continue;
			if (j == 4)				// center handled seperately
				continue;

			diode = diode_x + (diode_y * array_width);

			if (!ignoreFlag[diode])
			{
				sum += 1;
				for (i = 0; i < numPts; i++)
					proData[center_diode][i] += tmpBuf[diode][i];
			}
		}

		// Averaging
		if(sum==0)
		{
			for(i=0;i<numPts;i++)
			{
				proData[center_diode][i] = 0;
			}
		}
		else {
			for (i = 0; i < numPts; i++)
			{
				proData[center_diode][i] /= sum;
			}
		}
	}

	// Release Memory
	for (i = 0; i < num_bdiodes; i++)
	{
		delete [] tmpBuf[i];
	}

	delete [] tmpBuf;
}

//=============================================================================
void SignalProcessor::baseLineCorrection()
{
	if(BLCType=='N')
	{
	}
	else if(BLCType=='S')
	{
		clampStart();
	}
	else if(BLCType=='E')
	{
		clampStartEnd();
	}
	else if(BLCType=='P')
	{
		polynomialFit();
	}
	else if(BLCType=='A')
	{
		clampArbitrary();
	}
}

//=============================================================================
void SignalProcessor::clampStart()
{
	int avePts=10;
	int numPts=dc->getNumPts();

	if(numPts<avePts)
	{
		std::cout<<"Number of Points is too small!\n";
		return;
	}

	int i,j;
	double ave;

	for (i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++)
	{
		ave=0;

		for (j = 0; j < avePts; j++)
		{
			ave += get_proData(i)[j];
		}

		ave/=avePts;

		for (j = 0; j < numPts; j++)
		{
			get_proData(i)[j] -= ave;
		}
	}
}

//=============================================================================
void SignalProcessor::calSlope()
{
	int numPts=dc->getNumPts();
	double intPts=dc->getIntPts();

	int i,j;

	for (i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++)
	{
		double* data=dataArray->getProDataMem(i);
		double* slope=dataArray->getSlopeMem(i);

		for(j=1;j<(numPts-1);j++)
		{
			slope[j] = (data[j+1] - data[j-1]) / (2.0 * intPts);
		}
	}
}

//=============================================================================
void SignalProcessor::clampStartEnd()
{
	int avePts=10;
	int numPts=dc->getNumPts();

	if(numPts<avePts)
	{
		std::cout<<"Number of Points is too small!\n";
		return;
	}

	int i,j;
	double aveStart,aveEnd,rate;

	for (i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++)
	{
		// Start
		aveStart=0;

		for (j = 0; j < avePts; j++)
		{
			aveStart += get_proData(i)[j];
		}

		aveStart /= avePts;

		// End
		aveEnd = 0;

		for (j = numPts - avePts; j < numPts; j++)
		{
			aveEnd += get_proData(i)[j];
		}

		aveEnd /= avePts;

		// Correction
		rate = (aveEnd - aveStart) / numPts;

		for (j = 0; j < numPts; j++)
		{
			get_proData(i)[j] = get_proData(i)[j] - aveStart - (rate*j);
		}
	}
}

//=============================================================================
void SignalProcessor::polynomialFit()
{
	int i;

	for (i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++)
	{
		polynormialFitter->process(get_proData(i));
	}
}

//=============================================================================
void SignalProcessor::clampArbitrary()
{
	int i,j;
	int numPts=dc->getNumPts();
	double ptValue;
	
	for (i = -NUM_FP_DIODES; i < dataArray->num_binned_diodes(); i++)
	{
		ptValue = get_proData(i)[clampPt];

		for (j = 0; j < numPts; j++)
		{
			get_proData(i)[j] -= ptValue;
		}
	}
}

//=============================================================================
void SignalProcessor::setTemporalFilterType(char input)
{
	filter->setType(input);

}

//=============================================================================
void SignalProcessor::temporalFiltering(double* source,double* target)
{
	filter->process(source,target);
}

//=============================================================================
void SignalProcessor::setGaussianWeights()
{
	filter->setGaussianWeights();
}

//=============================================================================
