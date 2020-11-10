//=============================================================================
// Filter.cpp
//=============================================================================
#include "Definitions.h"
#include "Filter.h"
#include "DapController.h"

//=============================================================================
Filter::Filter()
{
	type='G';
	width=1;
}

//=============================================================================
Filter::~Filter()
{
}

//=============================================================================
void Filter::setType(char input)
{
	type=input;
}

//=============================================================================
void Filter::process(double *input,double *output)
{
	if(type=='G')
	{
		gaussian(input,output);
	}
	else if(type=='M')
	{
		movAve(input,output);
	}
}

//=============================================================================
void Filter::movAve(double *input,double *output)
{
	int i,j,count;
	int numPts=dc->getNumPts();
	double sum;

	sum=0;
	for(j=0;j<=width;j++)
	{
		sum+=input[j];
	}
	count=1+width;
	output[0]=sum/count;

	for(i=1;i<=width;i++)
	{
		count++;
		sum+=input[i+width];
		output[i]=sum/count;
	}

	for(i=width+1;i<(numPts-width);i++)
	{
		sum+=input[i+width];
		sum-=input[i-width-1];
		output[i]=sum/count;
	}

	for(i=(numPts-width+1);i<numPts;i++)
	{
		count--;
		sum-=input[i-width-1];
		output[i]=sum/count;
	}
}

//=============================================================================
void Filter::gaussian(double *input,double *output)
{
	int i,j;
	int numPts=dc->getNumPts();

	double sum;

	for(i=width;i<numPts-width;i++)
	{
		sum=0;

		for(j=-width;j<=width;j++)
		{
			sum+=input[i+j]*gaussianWeights[j+width];
		}

		output[i]=sum;
	}
}

//=============================================================================
void Filter::setWidth(int input)
{
	width=input;
}

//=============================================================================
void Filter::setGaussianWeights()
{
	int i,j;
	int N=1+2*width;
	double sum=0;
	double tmp[101];

	// Initinization
	gaussianWeights[0]=tmp[0]=1;

	for(i=1;i<N;i++)
	{
		gaussianWeights[i]=tmp[i]=0;
	}

	// Pyramid
	for(i=2;i<=N;i++)
	{
		for(j=1;j<i;j++)
		{
			tmp[j]=gaussianWeights[j-1]+gaussianWeights[j];
		}

		for(j=1;j<i;j++)
		{
			gaussianWeights[j]=tmp[j];
		}
	}

	sum=0;
	for(i=0;i<N;i++)
	{
		sum+=gaussianWeights[i];
	}

	for(i=0;i<N;i++)
	{
		gaussianWeights[i]/=sum;
	}
}

//=============================================================================
