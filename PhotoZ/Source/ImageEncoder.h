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
	void checkExtension(char* fileName, const char* ext);

	void saveImage(const char* fileName, HBITMAP hbm);

public:
	ImageEncoder();
	~ImageEncoder();
	void saveJpegFromBmp(char* fileName, HBITMAP hbm);
	void savePngFromBmp(char* fileName, HBITMAP hbm);
	void saveTiffFromBmp(char* fileName, HBITMAP hbm);
	void saveBmp(char* fileName, HBITMAP hbm, HDC hdc);
};

//=============================================================================
#endif
//=============================================================================
