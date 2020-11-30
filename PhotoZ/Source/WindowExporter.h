//=============================================================================
// WindowExporter.h: interface for the WindowExporter class.
//=============================================================================
#ifndef _WindowExporter_h
#define _WindowExporter_h

#include <Windows.h>
#include "ImageEncoder.h"
//=============================================================================
class WindowExporter  
{
private:
	char imageType;

	ImageEncoder* encoder; // 11/29/20 - replace Paintlib encoders

	HDC hdcMonitor;
	HDC hdcWindow;
	BITMAPINFO bitMapInfo;

public:
	WindowExporter();
	~WindowExporter();
	void setImageType(char);
	void export1(int x0, int y0, int w, int h);
	void export2(int x0, int y0, int w, int h, const char* fileName);
	void snapshot(int x0, int y0, int w, int h, int num);
};

//=============================================================================
#endif
//=============================================================================
