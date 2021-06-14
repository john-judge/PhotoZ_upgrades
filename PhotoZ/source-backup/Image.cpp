//=============================================================================
// Image.cpp
//=============================================================================
#include <iostream>
#include <stdlib.h>
#include <io.h>			// access()

#include <FL/Fl.H>
#include <FL/Fl_Image.H>	// Fl_RGB_Image
#include <FL/fl_draw.h>
#include <FL/fl_file_chooser.h>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_BMP_Image.H>

/*
#include <paintlib/plstdpch.h>		// Must have one for paintlib
#include <paintlib/planydec.h>
#include <paintlib/plwinbmp.h>
#include <paintlib/plfiltergrayscale.h>
*/
#include "Image.h"

using namespace std;
//=============================================================================
Image::Image()
{

	//decoder=new PLAnyPicDecoder;
	//winBmp=new PLWinBmp;
	rgbImage = NULL;
	
	fileLoaded=0;

	x0=0;
	y0=0;
	xSize=640;
	ySize=640;

	strcpy_s(imageFormat, 8,"BMP");
}

//=============================================================================
Image::~Image()
{
	//delete decoder;
	//delete bmp;
	delete rgbImage;
}

//=============================================================================
void Image::draw()
{
	if(!fileLoaded)
		return;

	rgbImage->draw(x0,y0);
}

//=============================================================================
void Image::setX0(int x)
{
	x0=x;
}

//=============================================================================
void Image::setY0(int y)
{
	y0=y;
}

//=============================================================================
void Image::setXSize(int p)
{
	xSize=p;
}

//=============================================================================
void Image::setYSize(int p)
{
	ySize=p;
}

//=============================================================================
void Image::setFileFormat(const char *type)
{
	strcpy_s(imageFormat, 8,type);
}

//=============================================================================
void Image::openImageFile()
{
	char fileFilter[8];
	strcpy_s(fileFilter, 8,"*.");
	strcat_s(fileFilter, 8,imageFormat);
	char *fileName=fl_file_chooser("Pick one image file",fileFilter,NULL);
	openImageFile(fileName);
}

//=============================================================================
void Image::openImageFile(const char *fileName)
{
	if(!fileName)
	{
		return;
	}

	if(_access(fileName,0)!=0)
	{
		fileLoaded=0;
		return;
	}
	if (rgbImage) delete rgbImage;
	// Tell the decoder to fill the bitmap object with data.
	//imageBuf = new uchar[xSize*ySize];
	rgbImage = new Fl_BMP_Image(fileName);//new Fl_RGB_Image(imageBuf, xSize, ySize, 1, 0);
	fileLoaded = true;
	if (rgbImage->fail()) {
		fl_alert(fileName);
		fileLoaded = false;
		return;
	}

	ws= rgbImage->w();
	hs= rgbImage->h();
	
	if (fileLoaded)
	{
		draw();
	}

}
//=============================================================================
int Image::getX0()
{
	return x0;
}

//=============================================================================
int Image::getY0()
{
	return y0;
}

//=============================================================================
int Image::getXSize()
{
	return xSize;
}

//=============================================================================
int Image::getYSize()
{
	return ySize;
}

//=============================================================================
char* Image::getFileFormat()
{
	return imageFormat;
}

//=============================================================================
