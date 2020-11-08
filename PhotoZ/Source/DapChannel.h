//=============================================================================
// DapChannel.h
//=============================================================================
#ifndef _DapChannel_h
#define _DapChannel_h
//=============================================================================

class DapChannel  
{
private:
	float onset;
	float duration;

public:
	DapChannel(float,float);

	void setOnset(float);
	float getOnset();
	char *getOnsetTxt();

	void setDuration(float);
	float getDuration();
	char *getDurationTxt();
};

//=============================================================================
#endif
//=============================================================================
