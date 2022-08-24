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
	else if(type=='B')
	{
		binomial8(input,output);
	}
	else if (type == 'C')
	{
		binomial6(input, output);
	}
	else if (type == 'D')
	{
		binomial4(input, output);
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
void Filter::binomial8(double *input,double *output)   //  new 9 point binomial for speed
{
	int i;
	int numPts = dc->getNumPts();

	double sum;
	output[0] = (input[4] + 8 * input[3] + 28 * input[2] + 56 * input[1]) / 93;
	output[1] = (input[5] + 8 * input[4] + 28 * input[3] + 56 * input[2] + 70 * input[1]) / 163;
	output[2] = (input[6] + 8 * input[5] + 28 * input[4] + 56 * (input[1] + input[3]) + 70 * input[2]) / 219;
	output[3] = (input[7] + 8 * input[6] + 28 * (input[1] + input[5]) + 56 * (input[2] + input[4]) + 70 * input[3]) / 247;
	
	for (i = 4; i < numPts - 4; i++)
	{
			output[i] = (input[i-4]+input[i+4] + 8*(input[i-3]+input[i+3])+ 28*(input[i-2]+input[i+2])+56*(input[i-1]+input[i+1])+70*input[i])/256;
	}
	output[numPts-4] = (input[numPts - 8] + 8 * (input[numPts - 7] + input[numPts -1 ]) + 28 * (input[numPts - 6] + input[numPts - 2]) + 56 * (input[numPts - 5] + input[numPts -3]) + 70 * input[numPts-4]) / 255;
	output[numPts-3] = (input[numPts - 7] + 8 * input[numPts - 6] + 28 * (input[numPts - 5] + input[numPts-1]) + 56 * (input[numPts - 4] + input[numPts -2]) + 70 * input[numPts-3]) / 247;
	output[numPts-2] = (input[numPts - 6] + 8 * input[numPts - 5] + 28 * input[numPts-4] + 56 * (input[numPts - 3] + input[numPts - 1]) + 70 * input[numPts-2]) / 219;
	output[numPts-1] = (input[numPts - 5] + 8 * input[numPts - 4] + 28 * input[numPts - 3] + 56 * input[numPts - 2] + 70 * input[numPts-1]) / 163;
}

void Filter::binomial6(double *input, double *output)   //   7 point binomial [from binomial8]
{
	int i;
	int numPts = dc->getNumPts();

	double sum;
	output[0] = (input[3] + 6 * input[2] + 15 * input[1] + 20 * input[0]) / 42;
	output[1] = (input[4] + 6 * input[3] + 15 * (input[0] + input[2]) + 20 * input[1]) / 57;
	output[2] = (input[5] + 6 * (input[0]+input[4]) + 15 * (input[1] + input[3]) + 20 * input[2]) /63;
	for (i = 3; i < numPts - 3; i++)
	{
		output[i] = (input[i - 3] + input[i + 3] + 6 * (input[i - 2] + input[i + 2]) + 15 * (input[i - 1] + input[i + 1]) + 20 * input[i]) / 64;
	}
	output[numPts - 3] = (input[numPts - 6] + 6 * (input[numPts - 5]+input[numPts-1]) + 15 * (input[numPts - 4] + input[numPts - 2]) + 20 * input[numPts - 3]) / 63;
	output[numPts - 2] = (input[numPts - 5] + 6 * input[numPts - 4] + 15 * (input[numPts - 3] + input[numPts - 1]) + 20 * input[numPts - 2]) / 57;
	output[numPts - 1] = (input[numPts - 4] + 6 * input[numPts - 3] + 15 * input[numPts - 2] + 20 * input[numPts - 1]) / 42;
}

void Filter::binomial4(double *input, double *output)   //   7 point binomial [from binomial8]
{
	int i;
	int numPts = dc->getNumPts();

	double sum;
	output[0] = (input[2]+4*input[1]+6*input[0])/11;
	output[1] = (input[3] + 4*(input[0]+input[2]) +6 * input[1]) / 15;
	for (i = 2; i < numPts - 2; i++)
	{
		output[i] = (input[i - 2] + input[i + 2] + 4 * (input[i - 1] + input[i + 1]) + 6 * input[i]) / 16;
	}
	output[numPts - 2] = (input[numPts-4] + 4 * (input[numPts-3] + input[numPts-1]) + 6 * input[numPts-2]) / 15;;
	output[numPts - 1] = (input[numPts - 3] + 4 * input[numPts - 2] + 6 * input[numPts - 1]) / 11;
}
void Filter::gaussian(double *input,double *output)
{
	int i, j;
	int k=width;
	int numPts=dc->getNumPts();

	double sum;
	if (width < 4) 		int k = 4;
	for(i=k;i<numPts-width;i++)						// modified to remove first few corrupt points
	{
		sum=0;

		for(j=-width;j<=width;j++)
		{
			sum+=input[i+j]*gaussianWeights[j+width];
		}

		output[i]=sum;
	}
	for (i = 0; i < width; i++) {
		output[i] = output[width+2*i];
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
