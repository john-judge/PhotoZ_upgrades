//=============================================================================
// SignalProcessor.h
//=============================================================================
#ifndef _SignalProcessor_h
#define _SignalProcessor_h
//=============================================================================
#include "ArrayWindow.h"

//=============================================================================
class SignalProcessor  
{
private:

	class PolynomialFitting *polynormialFitter;
	class Filter *filter;

	// Flags
	char dataInverseFlag;		// Data Inverse
	char fpInverseFlag;		// Extracellular Data Inverse
	char rliDivFlag;			// RLI Dividing
	char temporalFilterFlag;	// Temporal Filter
	char spatialFilterFlag;		// Spatial Filter
	char correctionFlag;		// Amplitude Correction

	// For Spatial Filter
	double centerWeight;
	double* proData[Num_Diodes];

	char diodeFlag[Num_Diodes];
	char ignoreFlag[Num_Diodes];

	// Baseline Correction
	char BLCType;				// BaseLine Correction Type
	int clampPt;

	// Baseline Correction
	void clampStart();
	void clampStartEnd();
	void clampArbitrary();
	void polynomialFit();

	// Settings for the analysis window
	int startWindow;
	int widthWindow;

public:
	// Constructor and Destructor
	SignalProcessor();
	~SignalProcessor();

	// Processing
	void process();
	void temporalFiltering(double*,double*);
	void spatialFilter();
	void calSlope();

	// Data Inverse
	void setDataInverseFlag(char);
	void setFpInverseFlag(char);
	char getDataInverseFlag();
	char getFpInverseFlag();

	// RLI Dividing
	void setRliDivFlag(char);
	char getRliDivFlag();

	// Amplitude Correction
	void setCorrectionFlag(char);
	char getCorrectionFlag();

	// Filter
	void setTemporalFilterFlag(char);
	char getTemporalFilterFlag();
	void setSpatialFilterFlag(char);
	void setTemporalFilterType(char);
	void setTemporalFilterWidth(int);
	void setSpatialFilterSigma(double);
	void setGaussianWeights();

	// Base Line Correction
	void setBLCType(char);
	char getBLCType();
	void setClampPt(int pt);
	int getClampPt();
	void baseLineCorrection();

	// Polynomial Fitting
	void setPolynomialStartPt(int pt);
	int getPolynomialStartPt();
	void setPolynomialNumPts(int pt);
	int getPolynomialNumPts();

	// Analysis Window Settings
	void setStartWindow(int);
	int getStartWindow();
	void setWidthWindow(int);
	int getWidthWindow();
};

//=============================================================================
#endif
//=============================================================================
