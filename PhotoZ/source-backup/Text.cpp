//=============================================================================
// Text.cpp
//=============================================================================
#include <stdlib.h>		// atoi(),itoa()
#include <stdio.h>		// sprintf()

//=============================================================================
char *i2txt(int value)
{
	static char txt[16];
	_itoa_s(value, txt, 10);
	//int ret = snprintf(txt, sizeof txt, "%f", value);
	return txt;
}

//=============================================================================
char *d2txt(double value)
{
	static char txt[16];

//	int ret = snprintf(txt, sizeof txt, "%f", value);
	sprintf_s(txt, 16,"%g",value);
	return txt;
}

//=============================================================================
char *d2txt(double value,int digit)
{
	static char txt[16];
	_gcvt_s(txt, 16, value, digit);
	return txt;
}

char *f2txt(float value)
{
	static char txt[7];

	int ret = snprintf(txt, sizeof txt, "%f", value);
	//	sprintf_s(txt, 16,"%g",value);
	return txt;
}

//=============================================================================
