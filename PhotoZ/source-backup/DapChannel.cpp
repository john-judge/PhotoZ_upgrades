//=============================================================================
// DapChannel.cpp
//=============================================================================
#include "DapChannel.h"

char* i2txt(int);
char* d2txt(double);
char* f2txt(float);
//=============================================================================
DapChannel::DapChannel(float onset,float duration)
{
	this->onset=onset;
	this->duration=duration;
}

//=============================================================================
void DapChannel::setOnset(float p)
{
	onset=p;
}

//=============================================================================
float DapChannel::getOnset()
{
	return onset;
}

//=============================================================================
char *DapChannel::getOnsetTxt()
{
	return f2txt(onset);
}

//=============================================================================
void DapChannel::setDuration(float p)
{
	duration=p;
}

//=============================================================================
float DapChannel::getDuration()
{
	return duration;
}

//=============================================================================
char* DapChannel::getDurationTxt()
{
	return f2txt(duration);
}

//=============================================================================
