//=============================================================================
// FileController.cpp
//=============================================================================
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>		// _stat
#include <sys/stat.h>		// _stat
#include <direct.h>			// _chdir(),_mkdir()
#include <io.h>				// access()
#include <windows.h>

#include <FL/Fl.H>
#include <FL/x.h>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.h>

#include "FileController.h"
#include "RecControl.h"
#include "DapController.h"
#include "DapChannel.h"
#include "DataArray.h"
#include "Definitions.h"

using namespace std;

//=============================================================================
FileController::FileController()
{
}

FileController::~FileController()
{
}

//=============================================================================
int FileController::openFile()
{
	char dir[128];
	char filename[64];

	if(!getDirFileName(dir,filename))
	{
		return -1;	// Canceled
	}

	// Change directory
	changeDir(dir);

	//
	return openFileByName(filename);
}

//=============================================================================
int FileController::openFileByName(const char *name)
{
	strcpy_s(fileName, 64, name);

	// Load data
	if(loadWholeFile())
	{
		recControl->setTrialNo(char(0));
		dataArray->average();
		return 1;
	}
	else
	{
		return 0;
	}
}

//=============================================================================
int FileController::loadWholeFile()
{
	// Open File
	fstream file;
	file.open(fileName,ios::in|ios::binary|ios::_Nocreate);
	if(!file)
	{
		file.close();
		return 0;
	}

	// Load RecControl
	if(!loadRecControl(file))
	{
		file.close();
		return 0;
	}

	//--------------------
	// Load Data
	loadData(file);
	file.close();

	//
	return 1;
}

//=============================================================================
int FileController::loadRecControl(fstream &file)
{
	char chBuf;
	short shBuf;
	int nBuf;
	float fBuf;
	time_t tBuf;
	int chSize=sizeof(char);
	int shSize=sizeof(short);
	int nSize=sizeof(int);
	int tSize=sizeof(time_t);
	int fSize=sizeof(float);

	// Version
	file.read((char*)&chBuf, chSize);
	if(chBuf<1 || chBuf>3)
	{
		fl_alert("File Format is not correct! - FileController::loadRecControl()\n");
		return 0;
	}

	// Slice Number
	file.read((char*)&shBuf, shSize);
	recControl->setSliceNo(shBuf);

	// Location Number
	file.read((char*)&shBuf, shSize);
	recControl->setLocationNo(shBuf);

	// Record Number
	file.read((char*)&shBuf, shSize);
	recControl->setRecordNo(shBuf);

	// Number of Trials
	file.read((char*)&chBuf, chSize);
	recControl->setNumTrials(chBuf);

	// Interval between Trials
	file.read((char*)&chBuf, chSize);
	recControl->setIntTrials(chBuf);

	// Acquisition Gain
	file.read((char*)&shBuf, shSize);
	recControl->setAcquiGain(shBuf);

	// RLI Gain
	file.read((char*)&shBuf, shSize);
	recControl->setRliGain(shBuf);

	// Number of Points per Trace
	file.read((char*)&nBuf,nSize);
	dc->setNumPts(nBuf);

	// Time - RecControl
	file.read((char*)&tBuf,tSize);
	recControl->setTime(tBuf);

	// Reset Onset
	file.read((char*)&nBuf,nSize);
	dc->reset->setOnset(nBuf);

	// Reset Duration
	file.read((char*)&nBuf,nSize);
	dc->reset->setDuration(nBuf);

	// Shutter Onset
	file.read((char*)&nBuf,nSize);
	dc->shutter->setOnset(nBuf);

	// Shutter Duration
	file.read((char*)&nBuf,nSize);
	dc->shutter->setDuration(nBuf);

	// Stimulation Onset
	file.read((char*)&nBuf,nSize);
	dc->sti1->setOnset(nBuf);

	// Stimulation Duration
	file.read((char*)&nBuf,nSize);
	dc->sti1->setDuration(nBuf);

	// Acquisition Onset
	file.read((char*)&nBuf,nSize);
	dc->setAcquiOnset(nBuf);

	// Interval between Samples
	file.read((char*)&fBuf,fSize);
	if(fBuf<0.001)
	{
		fBuf=0.614;
	}
	dc->setIntPts(fBuf);

	dapControl->setDuration();	// Set duration

	// Change Memory Size
	dataArray->changeMemSize(recControl->getNumTrials(),dc->getNumPts());

	//
	return 1;
}

//=============================================================================
int FileController::loadData(fstream &file)
{
	file.seekp(1024);
	int i,j;
	int shSize=sizeof(short);

	// RLI
	int rliSize=shSize*Num_Diodes;
	short *rliBuf=new short[rliSize];

	file.read((char*)rliBuf,rliSize);// RLI Low
	dataArray->setRliLow(rliBuf);

	file.read((char*)rliBuf,rliSize);// RLI High
	dataArray->setRliHigh(rliBuf);

	file.read((char*)rliBuf,rliSize);// RLI Max
	dataArray->setRliMax(rliBuf);

	delete [] rliBuf;

	dataArray->calRli();
	dataArray->setMaxRli();

	// Load Raw Data
	int numPts=dc->getNumPts();
	int dataSize=shSize*numPts;
	int numTrials=recControl->getNumTrials();

	for(i=0;i<numTrials;i++)
	{
		for(j=0;j<Num_Diodes;j++)
		{
			file.read((char*)dataArray->getMem(i,j),dataSize);
		}
	}

	//
	return 1;
}

//=============================================================================
// Return 1 if successful. Return 0 if failed.
//
int FileController::getDirFileName(char *dir,char *filename)
{
	char *input=fl_file_chooser( "Pick a data file", "*.zda", NULL);
	
	if(!input)
	{
		return 0;// Canceled
	}

	//---------------------------------------
	// One file have been picked or entered.
	char driveBuf[128];
	char dirBuf[128];
	char extBuf[128];

	_splitpath_s(input, driveBuf, 128, dirBuf, 128, filename, 64, extBuf, 128);

	strcat_s(filename, 64, extBuf);
	strcpy_s(dir, 128, dirBuf);
	return 1;
}

//=============================================================================
void FileController::changeDir(char *dir)
{
	_chdir(dir);
}

//=============================================================================
char *FileController::getFileName()
{
	return fileName;
}

//=============================================================================
void FileController::setFileNameFromRecControl()
{
	char *name=recControl->createFileName();
	strcpy_s(fileName, 64, name);
}

//=============================================================================
int FileController::saveDataFile()
{
	int existence = _access(fileName,0);	// O Existent; -1 Not existing
	while(existence==0)
	{
		// 0:Cancel, 1:Save as, 2:Overwrite
		int choice=fl_choice("The file already exists! Please choose one of following:","Cancel",
			"Save as ","Overwrite");

		if(choice==0)// Cancel
		{
			return -1;
		}
		else if(choice==1)// Save as
		{
			const char *name=fl_input("Please enter the file name.","filename.zda");
			existence = _access(name,0);
			if(existence==-1 && name!=0)
			{
				strcpy_s(fileName, 64, name);
			}
		}
		else if(choice==2)// Overwrite
		{
			existence=-1;
		}
	}

	//
	fstream file;
	file.open(fileName,ios::out|ios::binary|ios::trunc);

	saveRecControl(&file);
	saveData(&file);

	file.close();

	//
	return 1;
}

//=============================================================================
void FileController::saveProcessedData()
{
	int i;
	int doubleSize=sizeof(double);

	//=============================================
	// Open file
	fstream file;
	file.open("ProcessedData.dat",ios::out|ios::binary|ios::trunc);

	//=============================================
	// Save Processed Data
	int numPts=dc->getNumPts();
	int dataSize=doubleSize*numPts;

	for(i=0;i<Num_Diodes;i++)
	{
		file.write((char*)dataArray->getProDataMem(i),dataSize);
	}

	//=============================================
	// Close file
	file.close();
}

//=============================================================================
void FileController::saveRecControl(fstream *file)
{
	char chBuf;
	short shBuf;
	int nBuf;
	float fBuf;
	time_t tBuf;
	int chSize=sizeof(char);
	int shSize=sizeof(short);
	int nSize=sizeof(int);
	int fSize=sizeof(float);
	int tSize=sizeof(time_t);

	// Version
	chBuf=3;
	file->write((char*)&chBuf,chSize);

	// Slice Number
	shBuf=recControl->getSliceNo();
	file->write((char*)&shBuf,shSize);

	// Location Number
	shBuf=recControl->getLocationNo();
	file->write((char*)&shBuf,shSize);

	// Record Number
	shBuf=recControl->getRecordNo();
	file->write((char*)&shBuf,shSize);

	// Number of Trials
	chBuf=char(recControl->getNumTrials());
	file->write((char*)&chBuf,chSize);

	// Interval between Trials
	chBuf=char(recControl->getIntTrials());
	file->write((char*)&chBuf,chSize);

	// Acquisition Gain
	shBuf=recControl->getAcquiGain();
	file->write((char*)&shBuf,shSize);

	// RLI Gain
	shBuf=recControl->getRliGain();
	file->write((char*)&shBuf,shSize);

	// Number of Points per Trace
	nBuf=dc->getNumPts();
	file->write((char*)&nBuf,nSize);

	// Time - RecControl
	tBuf=recControl->getTime();
	file->write((char*)&tBuf,tSize);

	// Reset Onset
	nBuf=dc->reset->getOnset();
	file->write((char*)&nBuf,nSize);

	// Reset Duration
	nBuf=dc->reset->getDuration();
	file->write((char*)&nBuf,nSize);

	// Shutter Onset
	nBuf=dc->shutter->getOnset();
	file->write((char*)&nBuf,nSize);

	// Shutter Duration
	nBuf=dc->shutter->getDuration();
	file->write((char*)&nBuf,nSize);

	// Stimulation Onset
	nBuf=dc->sti1->getOnset();
	file->write((char*)&nBuf,nSize);

	// Stimulation Duration
	nBuf=dc->sti1->getDuration();
	file->write((char*)&nBuf,nSize);

	// Acquisition Onset
	nBuf=dc->getAcquiOnset();
	file->write((char*)&nBuf,nSize);

	// Interval between Samples
	fBuf=float(dc->getIntPts());
	file->write((char*)&fBuf,fSize);
}

//=============================================================================
void FileController::saveData(fstream *file)
{
	int i,j;
	short shBuf;
	int shSize=sizeof(short);

	file->seekg(1024);

	// RLI
	for(i=0;i<Num_Diodes;i++)
	{
		shBuf=dataArray->getRliLow(i);
		file->write((char*)&shBuf,shSize);
	}

	for(i=0;i<Num_Diodes;i++)
	{
		shBuf=dataArray->getRliHigh(i);
		file->write((char*)&shBuf,shSize);
	}

	for(i=0;i<Num_Diodes;i++)
	{
		shBuf=dataArray->getRliMax(i);
		file->write((char*)&shBuf,shSize);
	}

	// Load Raw Data
	int numPts=dc->getNumPts();
	int dataSize=shSize*numPts;
	int numTrials=recControl->getNumTrials();

	for(i=0;i<numTrials;i++)
	{
		for(j=0;j<Num_Diodes;j++)
		{
			file->write((char*)dataArray->getMem(i,j),dataSize);
		}
	}
}

//=============================================================================
