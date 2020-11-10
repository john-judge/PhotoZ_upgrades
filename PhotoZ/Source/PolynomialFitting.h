//=============================================================================
// PolynomialFitting.h
//=============================================================================
#ifndef _PolynomialFitting_h
#define _PolynomialFitting_h
//=============================================================================

class PolynomialFitting  
{
private:
	int degree;
	double *data;

	double X[4][4];
	double Y[4];
	double A[4];	// Coefficients of the function f(x)

	// Window
	int startPt;
	int numPts;

	void switchRow(int index);
	void pivot(int index);
	void setCoefficients();
	void calA();
	void fit();

public:
	PolynomialFitting();
	~PolynomialFitting();

	void setStartPt(int pts);
	int getStartPt();

	void setNumPts(int pts);
	int getNumPts();

	void process(double *iData);
};

//=============================================================================
#endif
//=============================================================================
