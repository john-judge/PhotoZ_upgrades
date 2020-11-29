//=============================================================================
// WindowExporter.cpp: implementation of the WindowExporter class.
//=============================================================================
#include <windows.h>

#include <stdlib.h>
#include <string>
#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_File_Chooser.H>

/*
#include <paintlib/plstdpch.h>		// Must have one for paintlib
#include <paintlib/planydec.h>
#include <paintlib/plwinbmp.h>
#include <paintlib/plbmpenc.h>
#include <paintlib/pljpegenc.h>
#include <paintlib/plpngenc.h>
#include <paintlib/pltiffenc.h>
*/
#include "WindowExporter.h"
#include "MainController.h"

using namespace std;

//=============================================================================
WindowExporter::WindowExporter()
{
	imageType='P';

	/*
	jpegEncoder=new PLJPEGEncoder();
	pngEncoder=new PLPNGEncoder();
	bmpEncoder=new PLBmpEncoder();
	tiffEncoder=new PLTIFFEncoder();
	*/
	hdcMonitor=CreateDC("DISPLAY",NULL,NULL,NULL);	// Handle for DC
	hdcWindow=CreateCompatibleDC(hdcMonitor);
}

//=============================================================================
WindowExporter::~WindowExporter()
{
	/*
	delete jpegEncoder;
	delete pngEncoder;
	delete bmpEncoder;
	delete tiffEncoder;
	*/
	DeleteObject(hdcMonitor);
}

//=============================================================================
void WindowExporter::setImageType(char p)
{
	imageType=p;
}

//=============================================================================
void setBitmapInfo(HBITMAP hBmp,BITMAPINFO *pbmi)
{ 
	BITMAP bmp; 

	GetObject(hBmp,sizeof(BITMAP),&bmp); 

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pbmi->bmiHeader.biWidth = bmp.bmWidth; 
	pbmi->bmiHeader.biHeight = bmp.bmHeight; 
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
	pbmi->bmiHeader.biCompression = BI_RGB; 
	pbmi->bmiHeader.biSizeImage=pbmi->bmiHeader.biWidth*pbmi->bmiHeader.biHeight*32; 
	pbmi->bmiHeader.biClrImportant = 0;
}

//=============================================================================
void WindowExporter::export1(int x0, int y0, int w, int h)
{
	HBITMAP hbm=CreateCompatibleBitmap(hdcMonitor,w,h);
	setBitmapInfo(hbm,&bitMapInfo);
	SelectObject(hdcWindow,hbm);
//	cout << " line 78 x0  " << x0 << " y0 " << y0 << " w " << w << " h " << h << endl;
	BitBlt(hdcWindow,0,0,w,h,hdcMonitor,x0,y0,SRCCOPY);

	unsigned char *buf=new unsigned char[bitMapInfo.bmiHeader.biSizeImage];

	GetDIBits(
	  hdcWindow,				// handle to device context
	  hbm,						// handle to bitmap
	  0,						// first scan line to set in destination bitmap
	  bitMapInfo.bmiHeader.biHeight,	// number of scan lines to copy
	  buf,						// address of array for bitmap bits
	  &bitMapInfo,						// address of structure with bitmap data
	  DIB_RGB_COLORS			// RGB
	);
		
	//PLWinBmp *winBmp=new PLWinBmp;
	//winBmp->CreateFromHBitmap(hbm);

	//
	int     x,y;
	//PLPixel32 ** pLineArray = winBmp->GetLineArray32();
	//PLPixel32  * pLine;
	/*
	int width=winBmp->GetWidth();
	int height=winBmp->GetHeight();
	for (y=0;y<height;y++)
	{
		pLine=pLineArray[height-y-1];
		for(x=0;x<width;x++)
		{
			pLine[x].SetR(buf[4*(width*y+x)+2]);
			pLine[x].SetG(buf[4*(width*y+x)+1]);
			pLine[x].SetB(buf[4*(width*y+x)]);
			pLine[x].SetA(buf[4*(width*y+x)+3]);
		}
	}*/
	//
	delete [] buf;
		
	char *fileName;
	if(imageType=='B')
	{
		fileName=fl_file_chooser("BMP file name","*.bmp","Image.bmp");
		if(fileName) {}
			//bmpEncoder->MakeFileFromBmp(fileName,winBmp);
	}
	else if(imageType=='J')
	{
		fileName=fl_file_chooser("JPEG file name","*.jpg","Image.jpg");
		if (fileName) {}
			//jpegEncoder->MakeFileFromBmp(fileName,winBmp);
	}
	else if(imageType=='P')
	{
		fileName=fl_file_chooser("Enter PNG file name","*.png","Image.png");
		if(fileName) {}
			//pngEncoder->MakeFileFromBmp(fileName,winBmp);
	}
	else if(imageType=='T')
	{
		fileName=fl_file_chooser("TIFF file name","*.tif*","Image.tif");
		if(fileName) {}
			//tiffEncoder->MakeFileFromBmp(fileName,winBmp);
	}

	//
	//delete winBmp;
	DeleteObject(hbm);
}

//=============================================================================
void WindowExporter::export2(int x0, int y0, int w, int h, const char* fileName)
{
	HBITMAP hbm=CreateCompatibleBitmap(hdcMonitor,w,h);
	setBitmapInfo(hbm,&bitMapInfo);
	SelectObject(hdcWindow,hbm);
	BitBlt(hdcWindow,0,0,w,h,hdcMonitor,x0,y0,SRCCOPY);

	unsigned char *buf=new unsigned char[bitMapInfo.bmiHeader.biSizeImage];

	GetDIBits(
	  hdcWindow,				// handle to device context
	  hbm,						// handle to bitmap
	  0,						// first scan line to set in destination bitmap
	  bitMapInfo.bmiHeader.biHeight,	// number of scan lines to copy
	  buf,						// address of array for bitmap bits
	  &bitMapInfo,						// address of structure with bitmap data
	  DIB_RGB_COLORS			// RGB
	);

	//
	int     x, y;
	/*
	PLWinBmp *winBmp=new PLWinBmp;
	winBmp->CreateFromHBitmap(hbm);

	//
	
	PLPixel32 ** pLineArray = winBmp->GetLineArray32();
	PLPixel32  * pLine;
	int width=winBmp->GetWidth();
	int height=winBmp->GetHeight();

	for (y=0;y<height;y++)
	{
		pLine=pLineArray[height-y-1];
		for(x=0;x<width;x++)
		{
			pLine[x].SetR(buf[4*(width*y+x)+2]);
			pLine[x].SetG(buf[4*(width*y+x)+1]);
			pLine[x].SetB(buf[4*(width*y+x)]);
			pLine[x].SetA(buf[4*(width*y+x)+3]);
		}
	}*/

	//
	delete [] buf;

	//pngEncoder->MakeFileFromBmp(fileName,winBmp);

	//
	//delete winBmp;
	DeleteObject(hbm);
}

//=============================================================================
void WindowExporter::snapshot(int x0, int y0, int w, int h, int num)
{
	HBITMAP hbm=CreateCompatibleBitmap(hdcMonitor,w,h);		// Handle for Bitmap
	setBitmapInfo(hbm,&bitMapInfo);
	SelectObject(hdcWindow,hbm);
	BitBlt(hdcWindow,0,0,w,h,hdcMonitor,x0,y0,SRCCOPY);

	//
	unsigned char *buf=new unsigned char[bitMapInfo.bmiHeader.biSizeImage];

	GetDIBits(
	  hdcWindow,				// handle to device context
	  hbm,						// handle to bitmap
	  0,						// first scan line to set in destination bitmap
	  bitMapInfo.bmiHeader.biHeight,	// number of scan lines to copy
	  buf,						// address of array for bitmap bits
	  &bitMapInfo,						// address of structure with bitmap data
	  DIB_RGB_COLORS			// RGB
	);

	//
	int     x, y;
	/*
	PLWinBmp *winBmp=new PLWinBmp;
	winBmp->CreateFromHBitmap(hbm);

	//
	
	PLPixel32 ** pLineArray = winBmp->GetLineArray32();
	PLPixel32  * pLine;
	
	int width=winBmp->GetWidth();
	int height=winBmp->GetHeight();

	for (y=0;y<height;y++)
	{
		pLine=pLineArray[height-y-1];
		for(x=0;x<width;x++)
		{
			pLine[x].SetR(buf[4*(width*y+x)+2]);
			pLine[x].SetG(buf[4*(width*y+x)+1]);
			pLine[x].SetB(buf[4*(width*y+x)]);
			pLine[x].SetA(buf[4*(width*y+x)+3]);
		}
	}
	*/

	//
	delete [] buf;

	std::string fileName = std::to_string(num) + ".jpg";
	//jpegEncoder->MakeFileFromBmp(fileName.c_str(), winBmp);

	// delete winBmp;
	DeleteObject(hbm);
}

//=============================================================================
