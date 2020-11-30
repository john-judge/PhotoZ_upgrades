#pragma once
//=============================================================================
// ImageEncoder.h: interface for the ImageEncoder class.
//=============================================================================
#ifndef _ImageEncoder_h
#define _ImageEncoder_h

//=============================================================================
class ImageEncoder
{
private:

	/*
	class PLJPEGEncoder* jpegEncoder;
	class PLPNGEncoder* pngEncoder;
	class PLBmpEncoder* bmpEncoder;
	class PLTIFFEncoder* tiffEncoder;
	*/


public:
	ImageEncoder();
	~ImageEncoder();
	void saveJpegFromBmp(const char* fileName, HBITMAP *hbm);
	void savePngFromBmp(const char* fileName, HBITMAP *hbm);
	void saveTiffFromBmp(const char* fileName, HBITMAP *hbm);
	void saveBmp(const char* fileName, HBITMAP *hbm);
};

//=============================================================================
#endif
//=============================================================================
