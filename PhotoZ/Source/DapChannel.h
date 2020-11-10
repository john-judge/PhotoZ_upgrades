//=============================================================================
// DapChannel.h
//=============================================================================
#ifndef _DapChannel_h
#define _DapChannel_h
//=============================================================================

class DapChannel  
{
private:
	int onset;
	int duration;

public:
	DapChannel(int,int);

	void setOnset(int);
	int getOnset();
	char *getOnsetTxt();

	void setDuration(int);
	int getDuration();
	char *getDurationTxt();
};

//=============================================================================
#endif
//=============================================================================
