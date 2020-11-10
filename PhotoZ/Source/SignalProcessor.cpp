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

	startWindow=160;
	widthWindow=200;
}

//=============================================================================
SignalProcessor::~SignalProcessor()
{
	delete polynormialFitter;
	delete filter;
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

	for(i=0;i<Num_Diodes;i++)
	{
		diodeFlag[i]=!dataArray->getFpFlag(i);
		ignoreFlag[i]=dataArray->getIgnoredFlag(i);
		proData[i]=dataArray->getProDataMem(i);
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
	double** tmpBuf=new double*[Num_Diodes];
	int numPts=dc->getNumPts();

	for(i=0;i<Num_Diodes;i++)
	{
		tmpBuf[i]=new double[numPts];
	}

	for(i=0;i<Num_Diodes;i++)
	{
		if(!ignoreFlag[i])
		{
			for(j=0;j<numPts;j++)
			{
				tmpBuf[i][j]=proData[i][j];
			}
		}
	}

	//---------------
	// Filter
	//---------------
	int iCenter,iNeighbor,k;
	double sum;

	for(i=2;i<49;i++)
	{
		for(j=1;j<26;j++)
		{
			iCenter=aw->diodeMap[i][j];

			if(iCenter>=0)
			{
				if(diodeFlag[iCenter])
				{
					sum=0;

					// Center Diode
					if(!ignoreFlag[iCenter])
					{
						sum+=centerWeight;
						for(k=0;k<numPts;k++)
						{
							proData[iCenter][k]=tmpBuf[iCenter][k]*centerWeight;
						}
					}

					// Neighbor 1
					iNeighbor=aw->diodeMap[i-1][j-1];
					if(iNeighbor>=0)
					{
						if(!ignoreFlag[iNeighbor])
						{
							sum+=1;
							for(k=0;k<numPts;k++)
							{
								proData[iCenter][k]+=tmpBuf[iNeighbor][k];
							}
						}
					}

					// Neighbor 2
					iNeighbor=aw->diodeMap[i+1][j-1];
					if(iNeighbor>=0)
					{
						if(!ignoreFlag[iNeighbor])
						{
							sum+=1;
							for(k=0;k<numPts;k++)
							{
								proData[iCenter][k]+=tmpBuf[iNeighbor][k];
							}
						}
					}

					// Neighbor 3
					iNeighbor=aw->diodeMap[i-2][j];
					if(iNeighbor>=0)
					{
						if(!ignoreFlag[iNeighbor])
						{
							sum+=1;
							for(k=0;k<numPts;k++)
							{
								proData[iCenter][k]+=tmpBuf[iNeighbor][k];
							}
						}
					}

					// Neighbor 4
					iNeighbor=aw->diodeMap[i+2][j];
					if(iNeighbor>=0)
					{
						if(!ignoreFlag[iNeighbor])
						{
							sum+=1;
							for(k=0;k<numPts;k++)
							{
								proData[iCenter][k]+=tmpBuf[iNeighbor][k];
							}
						}
					}

					// Neighbor 5
					iNeighbor=aw->diodeMap[i-1][j+1];
					if(iNeighbor>=0)
					{
						if(!ignoreFlag[iNeighbor])
						{
							sum+=1;
							for(k=0;k<numPts;k++)
							{
								proData[iCenter][k]+=tmpBuf[iNeighbor][k];
							}
						}
					}

					// Neighbor 6
					iNeighbor=aw->diodeMap[i+1][j+1];
					if(iNeighbor>=0)
					{
						if(!ignoreFlag[iNeighbor])
						{
							sum+=1;
							for(k=0;k<numPts;k++)
							{
								proData[iCenter][k]+=tmpBuf[iNeighbor][k];
							}
						}
					}

					// Averaging
					if(sum==0)
					{
						for(k=0;k<numPts;k++)
						{
							proData[iCenter][k]=0;
						}
					}
					else
					{
						for(k=0;k<numPts;k++)
						{
							proData[iCenter][k]/=sum;
						}
					}
				}
			}
		}
	}

	// Release Memory
	for(i=0;i<Num_Diodes;i++)
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

	for(i=0;i<Num_Diodes;i++)
	{
		ave=0;

		for(j=0;j<avePts;j++)
		{
			ave+=proData[i][j];
		}

		ave/=avePts;

		for(j=0;j<numPts;j++)
		{
			proData[i][j]-=ave;
		}
	}
}

//=============================================================================
void SignalProcessor::calSlope()
{
	int numPts=dc->getNumPts();
	double intPts=dc->getIntPts();

	int i,j;

	for(i=0;i<Num_Diodes;i++)
	{
		double* data=dataArray->getProDataMem(i);
		double* slope=dataArray->getSlopeMem(i);

		for(j=1;j<(numPts-1);j++)
		{
			slope[j]=(data[j+1]-data[j-1])/(2.0*intPts);
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

	for(i=0;i<Num_Diodes;i++)
	{
		// Start
		aveStart=0;

		for(j=0;j<avePts;j++)
		{
			aveStart+=proData[i][j];
		}

		aveStart/=avePts;

		// End
		aveEnd=0;

		for(j=numPts-avePts;j<numPts;j++)
		{
			aveEnd+=proData[i][j];
		}

		aveEnd/=avePts;

		// Correction
		rate=(aveEnd-aveStart)/numPts;

		for(j=0;j<numPts;j++)
		{
			proData[i][j]=proData[i][j]-aveStart-rate*j;
		}
	}
}

//=============================================================================
void SignalProcessor::polynomialFit()
{
	int i;

	for(i=0;i<Num_Diodes;i++)
	{
		polynormialFitter->process(proData[i]);
	}
}

//=============================================================================
void SignalProcessor::clampArbitrary()
{
	int i,j;
	int numPts=dc->getNumPts();
	double ptValue;
	
	for(i=0;i<Num_Diodes;i++)
	{
		ptValue=proData[i][clampPt];

		for(j=0;j<numPts;j++)
		{
			proData[i][j]-=ptValue;
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
