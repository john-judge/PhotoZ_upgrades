//=============================================================================
// WindowExporter.h: interface for the WindowExporter class.
//=============================================================================
#ifndef _WindowExporter_h
#define _WindowExporter_h
//=============================================================================
#include <windows.h>

//=============================================================================
class WindowExporter  
{
private:
	char imageType;

	class PLJPEGEncoder* jpegEncoder;
	class PLPNGEncoder* pngEncoder;
	class PLBmpEncoder* bmpEncoder;
	class PLTIFFEncoder* tiffEncoder;

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
