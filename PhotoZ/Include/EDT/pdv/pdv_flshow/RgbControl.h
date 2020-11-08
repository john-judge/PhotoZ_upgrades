

#ifndef RgbControl_H
#define RgbControl_H

#include "EdtImage.h"

#include "RgbControlUI.h"

#include "PdvGlWindow.h"

#include "EdtLut.h"

#include "BayerConverter.h"

#include "PdvInput.h"

class RgbControl {

public:

    RgbControl(PdvGlWindow *pWin = NULL, PdvInput * dev = NULL);

    RgbControlUI *pUI;

    PdvGlWindow *pWin;

    EdtImage *m_image;
    EdtImage *m_raw_image;

    BayerConverter converter;
    BayerConverter initial;

    bool has_pdv;
    PdvInput *pdv;
   
    int channel;
    int nchannels;

    bool visible()
    {
	if (pUI && pUI->RgbControlWindow->visible())
	    return true;
	return false;
    }

    void refresh(PdvGlWindow *pWin = NULL);
    void update();

    void show(void)
    {
        GetInitial();

	pUI->show();
    }

    void GetInitial();

    void SetImages(EdtImage *pRaw, EdtImage *pTarget);

    void Setup();
};



#endif
