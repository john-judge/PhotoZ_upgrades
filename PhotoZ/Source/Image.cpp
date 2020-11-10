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

#include <paintlib/plstdpch.h>		// Must have one for paintlib
#include <paintlib/planydec.h>
#include <paintlib/plwinbmp.h>
#include <paintlib/plfiltergrayscale.h>

#include "Image.h"

//=============================================================================
Image::Image()
{
	decoder=new PLAnyPicDecoder;
	winBmp=new PLWinBmp;
	rgbImage=0;

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
	delete decoder;
	delete winBmp;
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
	reAllocMem();
}

//=============================================================================
void Image::setYSize(int p)
{
	ySize=p;
	reAllocMem();
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

	// Tell the decoder to fill the bitmap object with data.
	try
	{
		decoder->MakeBmpFromFile(fileName,winBmp);
		fileLoaded=1;
	}
	catch (PLTextException e)
	{
		std::cerr << "Error "<<e.GetCode()<<" decoding : "<<fileName<<std::endl;
		std::cerr << e << std::endl;
		fileLoaded=0;
		return;
	}

	//
	ws=winBmp->GetWidth();
	hs=winBmp->GetHeight();

	reAllocMem();
}

//=============================================================================
void Image::reAllocMem()
{
	if(!fileLoaded)
	{
		return;
	}

	int xSize=this->xSize;
	int ySize=this->ySize;

	if(rgbImage!=0)
	{
		delete rgbImage;
	}

	imageBuf=new uchar[xSize*ySize];
	rgbImage=new Fl_RGB_Image(imageBuf,xSize,ySize,1,0);

	// 32 bpp version
	char bpp=winBmp->GetBitsPerPixel();

	if(bpp!=8)
	{
		PLFilterGrayscale filter;
		filter.ApplyInPlace(winBmp);
	}

	PLBYTE **pLineArray=winBmp->GetLineArray();

	double xRatio=double(ws-1)/double(xSize-1);
	double yRatio=double(hs-1)/double(ySize-1);

	int x,y;			// Destination
	double xs,ys;		// Source
	int xs1,ys1;		// The coordinates of the four vertices of the square in the source image
	uchar d1,d2,d3,d4;	// Intensities at four vertices
	double disX,disY;

	for(y=0;y<ySize;y++)
	{
		for(x=0;x<xSize;x++)
		{
			xs=xRatio*double(x);
			ys=yRatio*double(y);

			xs1=int(xs);
			ys1=int(ys);
			disX=xs-xs1;
			disY=ys-ys1;
			if(disX<1.0e-10)
				disX=0;
			if(disY<1.0e-10)
				disY=0;

			if(disX==0 && disY==0)
			{
				imageBuf[y*xSize+x]=pLineArray[ys1][xs1];
			}
			else if(disX==0)
			{
				d1=pLineArray[ys1][xs1];
				d4=pLineArray[ys1+1][xs1];
				imageBuf[y*xSize+x]=uchar((1.0-disY)*d1+disY*d4);
			}
			else if(disY==0)
			{
				d1=pLineArray[ys1][xs1];
				d2=pLineArray[ys1][xs1+1];
				imageBuf[y*xSize+x]=uchar((1.0-disX)*d1+disX*d2);
			}
			else
			{
				d1=pLineArray[ys1][xs1];
				d2=pLineArray[ys1][xs1+1];
				d3=pLineArray[ys1+1][xs1+1];
				d4=pLineArray[ys1+1][xs1];

				imageBuf[y*xSize+x]=uchar(
					(1.0-disX)*(1.0-disY)*d1
					+(disX)*(1.0-disY)*d2
					+(disX)*(disY)*d3
					+(1.0-disX)*(disY)*d4);
			}
		}
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
