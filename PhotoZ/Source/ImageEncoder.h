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
	HBITMAP *hbm;

	PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);
	void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi,
		HBITMAP hBMP, HDC hDC);

public:
	ImageEncoder();
	~ImageEncoder();
	void saveJpegFromBmp(const char* fileName, HBITMAP hbm);
	void savePngFromBmp(const char* fileName, HBITMAP hbm);
	void saveTiffFromBmp(const char* fileName, HBITMAP hbm);
	void saveBmp(const char* fileName, HBITMAP hbm, HDC hdc);
};

//=============================================================================
#endif
//=============================================================================
