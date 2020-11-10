//=============================================================================
// PolynomialFitting.cpp
//=============================================================================
#include <math.h>

#include "Definitions.h"
#include "PolynomialFitting.h"
#include "DapController.h"

//=============================================================================
PolynomialFitting::PolynomialFitting()
{
	degree=3;
	startPt=155;
	numPts=400;
}

//=============================================================================
PolynomialFitting::~PolynomialFitting()
{
}

//=============================================================================
void PolynomialFitting::setStartPt(int sp)
{
	startPt=sp;
}

//=============================================================================
int PolynomialFitting::getStartPt()
{
	return startPt;
}

//=============================================================================
void PolynomialFitting::setNumPts(int np)
{
	numPts=np;
}

//=============================================================================
int PolynomialFitting::getNumPts()
{
	return numPts;
}

//=============================================================================
void PolynomialFitting::process(double *data)
{
	this->data=data;

	setCoefficients();
	calA();
	fit();
}

//=============================================================================
void PolynomialFitting::setCoefficients()
{
	int i,j;

	// Clear the Matrix
	for(i=0;i<=degree;i++)
	{
		for(j=0;j<=degree;j++)
		{
			X[i][j]=0;
		}

		Y[i]=0;
	}

	// 
	double y;
	double x[7]={0};
	double cx[7]={0};
	double cy[4]={0};

	//
	int endPt=startPt+numPts;
	int length=dc->getNumPts();

	for(i=0;i<length;i++)
	{
		if(i>=startPt && i<endPt)
		{
			continue;
		}

		x[0]=1;
		y=data[i];

		for(j=1;j<7;j++)
		{
			x[j]=x[j-1]*i;
		}

		for(j=0;j<7;j++)
		{
			cx[j]+=x[j];
		}

		for(j=0;j<4;j++)
		{
			cy[j]+=y*x[j];
		}
	}

	//
	for(i=0;i<=degree;i++)
	{
		for(j=0;j<=degree;j++)
		{
			X[i][j]=cx[i+(3-j)];
		}

		Y[i]=cy[i];
	}

}

//=============================================================================
void PolynomialFitting::calA()
{
	int i;

	for(i=0;i<=degree;i++)
	{
		switchRow(i);
		pivot(i);
	}

	A[3]=Y[3]/X[3][3];
	A[2]=(Y[2]-X[2][3]*A[3])/X[2][2];
	A[1]=(Y[1]-X[1][3]*A[3]-X[1][2]*A[2])/X[1][1];
	A[0]=(Y[0]-X[0][3]*A[3]-X[0][2]*A[2]-X[0][1]*A[1])/X[0][0];
}

//=============================================================================
void PolynomialFitting::fit()
{
	int i;
	double value;
	int length=dc->getNumPts();

	for(i=0;i<length;i++)
	{
		value=((A[0]*i+A[1])*i+A[2])*i+A[3];
		data[i]-=value;
	}
}

//=============================================================================
void PolynomialFitting::switchRow(int index)
{
	if(index>degree || index<0)
	{
		return;
	}

	//
	int i;
	double maxV=0;
	int maxI;
	double tmp;

	for(i=index;i<=degree;i++)
	{
		tmp=fabs(X[i][index]);
		if(tmp>maxV)
		{
			maxV=tmp;
			maxI=i;
		}
	}

	if(maxI!=index)
	{
		for(i=index;i<=degree;i++)
		{
			tmp=X[index][i];
			X[index][i]=X[maxI][i];
			X[maxI][i]=tmp;
		}

		tmp=Y[index];
		Y[index]=Y[maxI];
		Y[maxI]=tmp;
	}
}

//=============================================================================
void PolynomialFitting::pivot(int index)
{
	if(index>degree || index<0)
	{
		return;
	}

	//
	int i,j;
	double m;

	for(i=index+1;i<=degree;i++)
	{
		m=X[i][index]/X[index][index];

		for(j=index+1;j<=degree;j++)
		{
			X[i][j]-=X[index][j]*m;
		}

		Y[i]-=Y[index]*m;
	}
}

//=============================================================================
