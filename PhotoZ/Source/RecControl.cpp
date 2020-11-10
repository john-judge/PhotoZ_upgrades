//=============================================================================
// RecControl.cpp
//=============================================================================
#include <iostream>
#include <string.h>		// strcmp(),strcpy_s(),strcat_s()
#include <time.h>		// time_t
#include <stdlib.h>		// atoi(),_itoa_s()
#include <io.h>			// _access()

#include "Definitions.h"
#include "RecControl.h"
#include "UserInterface.h"

//=============================================================================
RecControl::RecControl()
{
	acquiGain=1000;
	rliGain=1;

	sliceNo=1;
	sliceNoMax=1;

	locationNo=1;
	locationNoMax=1;

	recordNo=1;
	recordNoMax=1;

	trialNo=0;
	trialNoMax=0;

	time_t time=0;

	numTrials=1;
	numSkippedTrials=0;
	intTrials=10;

	intRecords=120;
	numRecords=1000;
}

//=============================================================================
RecControl::~RecControl()
{
}

//=============================================================================
void RecControl::checkFileRange()
{
	char *fileName=createFileName();

	// Slice No
	sliceNoMax=sliceNo;
	fileName=createFileName(sliceNoMax+1,1,1);
	while(_access(fileName,0)==0)
	{
		sliceNoMax++;
		fileName=createFileName(sliceNoMax+1,1,1);
	}

	// Location No
	locationNoMax=locationNo;
	fileName=createFileName(sliceNo,locationNoMax+1,1);
	while(_access(fileName,0)==0)
	{
		locationNoMax++;
		fileName=createFileName(sliceNo,locationNoMax+1,1);
	}

	// Record No
	recordNoMax=recordNo;
	fileName=createFileName(sliceNo,locationNo,recordNoMax+1);
	while(_access(fileName,0)==0)
	{
		recordNoMax++;
		fileName=createFileName(sliceNo,locationNo,recordNoMax+1);
	}

	// Trial No
	trialNoMax=numTrials;
	if(trialNo>trialNoMax)
	{
		trialNo=trialNoMax;
	}
}

//=============================================================================
char *RecControl::createFileName()
{
	return createFileName(sliceNo,locationNo,recordNo);
}

//=============================================================================
char *RecControl::createFileName(short sn,short ln,short rn)
{
	static char fileName[32];
	char sBuf[64];

	// Slice No
	_itoa_s(sn,sBuf,10);
	if(sn<10)
	{
		strcpy_s(fileName, 32,"0");
		strcat_s(fileName, 32,sBuf);
	}
	else
	{
		strcpy_s(fileName,sBuf);
	}
	strcat_s(fileName, 32,"_");

	// Location No
	if(ln<10) strcat_s(fileName, 32,"0");
	_itoa_s(ln,sBuf,10);
	strcat_s(fileName, 32,sBuf);
	strcat_s(fileName, 32,"_");

	// Record No
	if(rn<10) strcat_s(fileName,"0");
	_itoa_s(rn,sBuf,10);
	strcat_s(fileName, 32,sBuf);
	strcat_s(fileName, 32,".zda");

	//
	return fileName;
}

//=============================================================================
char *RecControl::createImageFileName()
{
	return createImageFileName(sliceNo,locationNo);
}

//=============================================================================
char *RecControl::createImageFileName(short sn,short ln)
{
	static char fileName[32];
	char sBuf[64];

	// Slice No
	_itoa_s(sn,sBuf,10);
	strcpy_s(fileName,sBuf);
	strcat_s(fileName,"-");

	// Location No
	_itoa_s(ln,sBuf,10);
	strcat_s(fileName,sBuf);
	strcat_s(fileName,".");
	strcat_s(fileName,aw->getImageFileFormat());

	//
	return fileName;
}

//=============================================================================
void RecControl::reset()
{
	sliceNo=1;
	locationNo=1;
	recordNo=1;
	trialNo=0;
}

//=============================================================================
void RecControl::increaseNo(char type)
{
	if(type==SLICE)	// Slice
	{
		locationNo=1;
		recordNo=1;
		trialNo=0;

		if(_access(createFileName(),0)==0)
		{
			sliceNo++;
		}
		else
		{
			return;
		}
	}
	else if(type==LOCATION)	// Location
	{
		recordNo=1;
		trialNo=0;

		if(_access(createFileName(),0)==0)
		{
			locationNo++;
		}
		else
		{
			return;
		}
	}
	else if(type==RECORD)	// Record
	{
		trialNo=0;

		if(_access(createFileName(),0)==0)
		{
			recordNo++;
		}
		else
		{
			return;
		}
	}
	else if(type==TRIAL)	// Trial
	{
		if(trialNo<trialNoMax)
			trialNo++;
		else
			trialNo=0;
	}
	else
	{
		std::cout<<"Error! - RecControl::increaseNo()\n";
		return;
	}
}

//=============================================================================
void RecControl::decreaseNo(char type)
{
	if(type==0)			// Slice
	{
		locationNo=1;
		recordNo=1;
		trialNo=0;

		if(sliceNo>1) sliceNo--;
	}
	else if(type==1)	// Location
	{
		recordNo=1;
		trialNo=0;

		if(locationNo>1) locationNo--;
	}
	else if(type==2)	// Record
	{
		trialNo=0;

		if(recordNo>1) recordNo--;
	}
	else if(type==3)	// Trial
	{
		if(trialNo>0) trialNo--;
		else trialNo=trialNoMax;
	}
	else
	{
		std::cout<<"Error! - RecControl::decreaseNo()\n";
		return;
	}
}

//=============================================================================
// Slice Number
void RecControl::setSliceNo(short p)
{
	sliceNo=p;
}

//=============================================================================
short RecControl::getSliceNo()
{
	return sliceNo;
}

//=============================================================================
short RecControl::getSliceNoMax()
{
	return sliceNoMax;
}

//=============================================================================
// Location Number
void RecControl::setLocationNo(short p)
{
	locationNo=p;
}

//=============================================================================
short RecControl::getLocationNo()
{
	return locationNo;
}

//=============================================================================
short RecControl::getLocationNoMax()
{
	return locationNoMax;
}

//=============================================================================
// Record Number
void RecControl::setRecordNo(short p)
{
	recordNo=p;
}

//=============================================================================
short RecControl::getRecordNo()
{
	return recordNo;
}

//=============================================================================
short RecControl::getRecordNoMax()
{
	return recordNoMax;
}

//=============================================================================
// Trial Number
void RecControl::setTrialNo(short p)
{
	trialNo=p;
}

//=============================================================================
short RecControl::getTrialNo()
{
	return trialNo;
}

//=============================================================================
short RecControl::getTrialNoMax()
{
	return trialNoMax;
}

//=============================================================================
// Acquisition Gain
void RecControl::setAcquiGain(short p)
{
	acquiGain=p;
}

//=============================================================================
short RecControl::getAcquiGain()
{
	return acquiGain;
}

//=============================================================================
void RecControl::decreaseAcquiGain()
{
	if(acquiGain==1000)
		acquiGain=200;
	else if(acquiGain==200)
		acquiGain=50;
	else
		acquiGain=1;
}

//=============================================================================
void RecControl::increaseAcquiGain()
{
	if(acquiGain==1)
		acquiGain=50;
	else if(acquiGain==50)
		acquiGain=200;
	else
		acquiGain=1000;
}

//=============================================================================
// RLI Gain
void RecControl::setRliGain(short p)
{
	rliGain=p;
}

//=============================================================================
short RecControl::getRliGain()
{
	return rliGain;
}

//=============================================================================
void RecControl::decreaseRliGain()
{
	if(rliGain==1000)
		rliGain=200;
	else if(rliGain==200)
		rliGain=50;
	else
		rliGain=1;
}

//=============================================================================
void RecControl::increaseRliGain()
{
	if(rliGain==1)
		rliGain=50;
	else if(rliGain==50)
		rliGain=200;
	else
		rliGain=1000;
}

//=============================================================================
// Time
void RecControl::setTime(time_t p)
{
	time=p;
}

//=============================================================================
time_t RecControl::getTime()
{
	return time;
}

//=============================================================================
void RecControl::setNumTrials(int p)
{
	numTrials=p;
}

//=============================================================================
void RecControl::setNumSkippedTrials(int p)
{
	numSkippedTrials=p;
}

//=============================================================================
int RecControl::getNumTrials()
{
	return numTrials;
}

//=============================================================================
int RecControl::getNumSkippedTrials()
{
	return numSkippedTrials;
}

//=============================================================================
void RecControl::setIntTrials(int p)
{
	intTrials=p;
}

//=============================================================================
int RecControl::getIntTrials()
{
	return intTrials;
}

//=============================================================================
void RecControl::setIntRecords(int p)
{
	intRecords=p;
}

//=============================================================================
int RecControl::getIntRecords()
{
	return intRecords;
}

//=============================================================================
void RecControl::setNumRecords(int p)
{
	numRecords=p;
}

//=============================================================================
int RecControl::getNumRecords()
{
	return numRecords;
}

//=============================================================================
