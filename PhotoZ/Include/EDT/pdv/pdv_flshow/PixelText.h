

#ifndef PIXEL_TEXT_H
#define PIXEL_TEXT_H

#include "EdtImage.h"

#include "PixelTextUI.h"

class PixelText {

public:

    PixelText(EdtImage *pImage = NULL);

    PixelTextUI *pUI;

    bool IsOpen()
    {
	if (pUI && pUI->PixelTextWindow->visible())
	    return true;
	return false;
    }

    EdtImage *m_image;

    void update(EdtImage *pImage, const int x, const int y);

    bool follow_cursor;

    int channel;
};



#endif
