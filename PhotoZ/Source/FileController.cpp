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

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>

#include <FL/Fl.H>
#include <FL/x.h>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.h>

#include "FileController.h"
#include "RecControl.h"
#include "ColorWindow.h"
#include "SignalProcessor.h"
#include "DapController.h"
#include "MainController.h"
extern MainController *mc;
#include "DapChannel.h"
#include "DataArray.h"
#include "Definitions.h"

bool NPFLAG = FALSE;				// Flag for opening neuroplex file instead of photoz file

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
	NPFLAG = FALSE;
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

int FileController::openNPFile()
{
	NPFLAG = TRUE;
	char dir[128];
	char filename[64];
	

	if (!getDirFileName(dir, filename))
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
		// validate 
		
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
	if(NPFLAG!=1&&!loadRecControl(file))
	{
		return 0;
	}
	if (NPFLAG==1&&!loadNPRecControl(file))
	{
		file.close();
		return 0;
	}

	//--------------------
	// Load Data
	if(NPFLAG!=1)	loadData(file);
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
	if(chBuf>=1 && chBuf<=3) {
		fl_alert("File Format is not correct! - Hexagonal data (use older version)\n");
		return 0;
	} else if(chBuf != 5 || chBuf != '5') {
		fl_alert("Allowing a possibly questionable file format!\n");
		cout << "Version number read from file: " << chBuf << "\n";
		// return 0;
	}

	// Slice Number
	file.read((char*)&shBuf, shSize);
	recControl->setSliceNo(shBuf);
	cout << "Slice Number: " << recControl->getSliceNo() << "\n";
	// Location Number
	file.read((char*)&shBuf, shSize);
	recControl->setLocationNo(shBuf);
	// Record Number
	file.read((char*)&shBuf, shSize);
	recControl->setRecordNo(shBuf);
	cout << "Record Number: " << recControl->getRecordNo() << "\n";
	// Camera Program
	file.read((char*)&nBuf, nSize);
	int cam_program = nBuf;
	dc->setCameraProgram(cam_program);
	cout << "Camera Program: " << nBuf << "\n";

	// Number of Trials
	file.read((char*)&chBuf, chSize);
	recControl->setNumTrials(chBuf);
	cout << "Number of Trials: " << chBuf << "\n";

	// Interval between Trials
	file.read((char*)&chBuf, chSize);
	recControl->setIntTrials(chBuf);
	cout << "Interval between Trials: " << chBuf << "\n";

	// Acquisition Gain
	file.read((char*)&shBuf, shSize);
	recControl->setAcquiGain(shBuf);
	cout << "Acquisition Gain: " << shBuf << "\n";

	// Number of Points per Trace
	file.read((char*)&nBuf,nSize);
	dc->setNumPts(nBuf);
	cout << "Number of Points per Trace: " << nBuf << "\n";

	// Time - RecControl
	file.read((char*)&tBuf,tSize);
	recControl->setTime(tBuf);

	// Reset Onset
	file.read((char*)&fBuf,fSize);
	dc->reset->setOnset(fBuf);

	// Reset Duration
	file.read((char*)&fBuf,fSize);
	dc->reset->setDuration(fBuf);

	// Shutter Onset
	file.read((char*)&fBuf,fSize);
	dc->shutter->setOnset(fBuf);

	// Shutter Duration
	file.read((char*)&fBuf,fSize);
	dc->shutter->setDuration(fBuf);

	// Stimulation 1 Onset
	file.read((char*)&fBuf,fSize);
	dc->sti1->setOnset(fBuf);

	// Stimulation 1 Duration
	file.read((char*)&fBuf,fSize);
	dc->sti1->setDuration(fBuf);

	// Stimulation 2 Onset
	file.read((char*)&fBuf, fSize);
	dc->sti2->setOnset(fBuf);

	// Stimulation 2 Duration
	file.read((char*)&fBuf, fSize);
	dc->sti2->setDuration(fBuf);

	// Acquisition Onset
	file.read((char*)&fBuf,fSize);
	dc->setAcquiOnset(fBuf);

	// Interval between Samples
	file.read((char*)&fBuf,fSize);
	if(fBuf<0.001f) {
		fBuf=0.614f;
	}
	dc->setIntPts(fBuf);
	cout << "Interval between Samples: " << fBuf << "\n";

	dapControl->setDuration();	// Set duration

	// Array Dimensions
	file.read((char*)&nBuf, nSize);
	int raw_width = nBuf;
	cout << "raw_width: " << nBuf << "\n";

	file.read((char*)&nBuf, nSize);
	int raw_height = nBuf;
	cout << "raw_height: " << nBuf << "\n";

	// Change Memory Size
	if (raw_width != dataArray->raw_width() || raw_height != dataArray->raw_height())
	{
		aw->clearSelected(0);
	}
		dataArray->changeMemSize(recControl->getNumTrials(), dc->getNumPts());
		dataArray->changeRawDataSize(raw_width, raw_height);
		dataArray->changeRawDataSize(raw_width, raw_height);
		aw->changeNumDiodes();
		cw->changeNumDiodes();
		cw->setPointXYZ();
		sp->changeNumDiodes();

	// depth set at a constant 2 bytes (hopefully) otherwise there is trouble
	//file.read((char*)&nBuf, nSize);
	// divide and round up
	//if ((nBuf + (8-1)) / 8 != 2)
	//	return 0;
//	else
//		int test = dataArray->depth();
	return 1;
}

int FileController::loadNPRecControl(fstream &file)						// reads header and data (also rli) in one module
{
	short NPHeader[2560];												// Read header as short array
	for (int i = 0; i < 2560; i++) {
		file.read((char*)&NPHeader[i], sizeof(NPHeader[i]));
	}
	recControl->setNumTrials(1);
	dc->setNumPts(NPHeader[4]);
	int numPts = NPHeader[4];
	double interval = 1000.0 / NPHeader[387];
	dc->setIntPts(interval);
	int raw_width = NPHeader[384];
	int raw_height = NPHeader[385];
	int numChannels = NPHeader[96];
	
	dataArray->changeMemSize(recControl->getNumTrials(), dc->getNumPts());
	dataArray->changeRawDataSize(raw_width, raw_height);
	dataArray->changeRawDataSize(raw_width, raw_height);
	aw->changeNumDiodes();
	cw->changeNumDiodes();
	cw->setPointXYZ();
	sp->changeNumDiodes();

	if (raw_width != dataArray->raw_width() || raw_height != dataArray->raw_height())
	{
		aw->clearSelected(0);
	}
	// read data as dataBuf and copy to raw_diode_data
	int dataSize = sizeof(short) * numPts; 
	int rliSize = sizeof(short) * numChannels;//  numChannels-8;
	signed short *rliBufLow = new short[rliSize];
	signed short *rliBufHigh = new short[rliSize];
	signed short *dataBuf = new short[numPts];
		for (int j = 0; j < numChannels; j++) {
			file.read((char*)dataBuf, dataSize);
			dataArray->assignTrialData(dataBuf, numPts, 0, j);
			rliBufHigh[j] = (dataBuf[0] + dataBuf[1] + dataBuf[2] + dataBuf[3] + dataBuf[4]) / 5;		//average first five points to get rliHigh
		}
	delete[] dataBuf;

	//  read dark from for rliLow
	file.read((char*)rliBufLow, rliSize);
	dataArray->setRliLow(rliBufLow);
	dataArray->setRliHigh(rliBufHigh);

	delete[] rliBufLow;
	delete[] rliBufHigh;
	dataArray->calRli();
	char* dbinning = i2txt(1);
	mc->set_digital_binning(dbinning);				// necessary to display background selection ??  
	return 1;
}

//=============================================================================
int FileController::loadData(fstream &file)
{
	file.seekp(1024);
	int i,j,k;
	int shSize=sizeof(short);
	int numDiodes = dataArray->num_raw_diodes();

	cout << "num fp pts: " << dataArray->num_diodes_fp();

	// RLI
	int rliSize=shSize*numDiodes;
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
	int dataSize = shSize; // * numPts;
	int numTrials=recControl->getNumTrials();

	short *dataBuf = new short[1];
	for (i = 0; i < numTrials; i++)
	{
		for (j = 0; j < dataArray->num_raw_diodes(); j++) {

			for (k = 0; k < numPts; k++) {
				file.read((char*)dataBuf, dataSize);
				
				dataArray->assignDataPoint(*dataBuf, k, i, j);
			}
			//cout << "read from file: " << *dataBuf << "\n";

		}
	}
	delete [] dataBuf;
	return 1;
}

//=============================================================================
// Return 1 if successful. Return 0 if failed.
//
int FileController::getDirFileName(char *dir,char *filename)
{
	char *input=NULL;

//	char *input = fl_file_chooser("Pick a data file", "*.zda", NULL);				previous format 
	if (NPFLAG==0) input = fl_file_chooser("Pick a data file", "*.zda", NULL);
	if (NPFLAG) input = fl_file_chooser("Pick a data file", "*.da", NULL);
	
	if(!input)
	{
		cout << " line 330 no input file " << endl;
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

int FileController::getDirNPFileName(char *dir, char *filename)
{
	char *input = fl_file_chooser("Pick a data file", "*.da", NULL);

	if (!input)
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
	const char *name=NULL;
	int existence = _access(fileName,0);	// O Existent; -1 Not existing
	while(existence==0|| strcmp(fileName, "None")==0)
	{
		int choice;
		if (strcmp(fileName, "None") == 0 && name==NULL) {
			choice = 1;
		}
		// 0:Cancel, 1:Save as, 2:Overwrite
		else {
			choice = fl_choice("The file already exists! Please choose one of following:", "Cancel",
				"Save as ", "Overwrite");
		}
		if(choice==0)// Cancel
		{
			return -1;
		}
		else if(choice==1)// Save as
		{
			name=fl_input("Please enter the file name.","filename.zda");
			if (!name)
				return -1;
			existence = _access(name,0);
			if(existence==-1 && name!=0)
			{
				strcpy_s(fileName, 64, name);
			}
		}
		else if(choice==2)// Overwrite
		{
			if(name!=NULL)
				strcpy_s(fileName, 64, name);
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
	//for (i = -NUM_FP_DIODES; i < dataArray->num_raw_array_diodes(); i++)
	for (i = -NUM_FP_DIODES; i < dataArray->binned_height()*dataArray->binned_width(); i++)
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
	chBuf=5;
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

	// Camera Program
	nBuf = dc->getCameraProgram();
	file->write((char*)&nBuf, nSize);

	// Number of Trials
	chBuf=char(recControl->getNumTrials());
	file->write((char*)&chBuf,chSize);

	// Interval between Trials
	chBuf=char(recControl->getIntTrials());
	file->write((char*)&chBuf,chSize);

	// Acquisition Gain
	shBuf=recControl->getAcquiGain();
	file->write((char*)&shBuf,shSize);

	// Number of Points per Trace
	nBuf=dc->getNumPts();
	file->write((char*)&nBuf,nSize);

	// Time - RecControl
	tBuf=recControl->getTime();
	file->write((char*)&tBuf,tSize);

	// Reset Onset
	fBuf=dc->reset->getOnset();
	file->write((char*)&fBuf,fSize);

	// Reset Duration
	fBuf=dc->reset->getDuration();
	file->write((char*)&fBuf,fSize);

	// Shutter Onset
	fBuf=dc->shutter->getOnset();
	file->write((char*)&fBuf,fSize);

	// Shutter Duration
	fBuf=dc->shutter->getDuration();
	file->write((char*)&fBuf,fSize);

	// Stimulation 1 Onset
	fBuf=dc->sti1->getOnset();
	file->write((char*)&fBuf,fSize);

	// Stimulation 1 Duration
	fBuf=dc->sti1->getDuration();
	file->write((char*)&fBuf,fSize);

	// Stimulation 2 Onset
	fBuf=dc->sti2->getOnset();
	file->write((char*)&fBuf,fSize);

	// Stimulation 2 Duration
	fBuf=dc->sti2->getDuration();
	file->write((char*)&fBuf,fSize);

	// Acquisition Onset
	fBuf=dc->getAcquiOnset();
	file->write((char*)&fBuf,fSize);

	// Interval between Samples
	fBuf=float(dc->getIntPts());
	file->write((char*)&fBuf,fSize);

	// Array Dimensions
	nBuf = dataArray->raw_width();
	file->write((char*)&nBuf, nSize);
	nBuf = dataArray->raw_height();
	file->write((char*)&nBuf, nSize);
	
	// depth set at a constant 2 bytes (hopefully) otherwise there is trouble
	nBuf = dataArray->depth();
	file->write((char*)&nBuf, nSize);
}

//=============================================================================
void FileController::saveData(fstream *file)
{
	int i,j;
	short shBuf;
	int shSize=sizeof(short);
	int arr_diodes = dataArray->num_raw_diodes();

	file->seekg(1024);

	// RLI
	for (i = 0; i < arr_diodes; i++)
	{
		shBuf=dataArray->getRliLow(i);
		file->write((char*)&shBuf,shSize);
	}

	for (i = 0; i < arr_diodes; i++)
	{
		shBuf=dataArray->getRliHigh(i);
		file->write((char*)&shBuf,shSize);
	}

	for (i = 0; i < arr_diodes; i++)
	{
		shBuf=dataArray->getRliMax(i);
		file->write((char*)&shBuf,shSize);
	}

	// Load Raw Data
	int numPts=dc->getNumPts();
	int dataSize=shSize*numPts;
	int numTrials=recControl->getNumTrials();

	for (i = 0; i < numTrials; i++)
	{
		for (j = 0; j < arr_diodes; j++)
		{
			file->write((const char*) dataArray->getTrialMem(i,j), dataSize);
		}
	}
}

//=============================================================================
