//=============================================================================
// Filter.h
//=============================================================================
#ifndef _Filter_h
#define _Filter_h
//=============================================================================

class Filter
{
private:
	char type;
	int width;
	double gaussianWeights[101];

	void movAve(double* input,double* output);
	void gaussian(double* input,double* output);

public:
	Filter();
	~Filter();

	void setType(char);
	void process(double *input,double *output);
	void setWidth(int);
	void setGaussianWeights();
};

//=============================================================================
#endif
//=============================================================================
