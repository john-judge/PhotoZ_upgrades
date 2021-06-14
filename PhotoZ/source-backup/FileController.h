//=============================================================================
// FileController.h
//=============================================================================
#ifndef _FileController_h
#define _FileController_h
//=============================================================================
#include <fstream>

//=============================================================================
class FileController  
{
private:
	// File Names for the two Data Array
	char fileName[64] = "None";

	// Private functions
	int getDirFileName(char* dir,char* filename);
	int getDirNPFileName(char* dir, char* filename);
	void changeDir(char*);

	int loadWholeFile();
	int loadRecControl(std::fstream&);
	int loadNPRecControl(std::fstream&);
	int loadData(std::fstream&);

	void saveRecControl(std::fstream*);
	void saveData(std::fstream*);

public:
	FileController();
	~FileController();

	int openFile();
	int openNPFile();
	int openFileByName(const char* fileName);
	int saveDataFile();
	void saveProcessedData();
	char *getFileName();
	void setFileNameFromRecControl();
};

//=============================================================================
#endif
//=============================================================================
