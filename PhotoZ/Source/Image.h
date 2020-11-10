//=============================================================================
// Image.h
//=============================================================================
#ifndef _Image_h
#define _Image_h
//=============================================================================
class Image  
{
private:
	//
	class PLAnyPicDecoder *decoder;
	class PLWinBmp *winBmp;
	class Fl_RGB_Image *rgbImage;

	int ws;	// width of source image
	int hs;	// height of source image

	int xSize;
	int ySize;

	int x0;
	int y0;

	uchar *imageBuf;
	char imageFormat[8];
	char fileLoaded;

	//
	void reAllocMem();

public:
	Image();
	~Image();

	void draw();

	void setX0(int p);
	void setY0(int p);
	void setXSize(int p);
	void setYSize(int p);

	int getX0();
	int getY0();
	int getXSize();
	int getYSize();

	void openImageFile();
	void openImageFile(const char*);
	void setFileFormat(const char*);
	char* getFileFormat();
};

//=============================================================================
#endif
//=============================================================================
