

#ifndef DisplayContrast_H
#define DisplayContrast_H

#include "EdtImage.h"

#include "DisplayContrastUI.h"

#include "PdvGlWindow.h"

#include "EdtLut.h"

class DisplayContrast {

public:

    DisplayContrast(PdvGlWindow *pWin = NULL);

    DisplayContrastUI *pUI;

    PdvGlWindow *pWin;
    EdtImage *m_image;
   
    int channel;
    int nchannels;

    EdtLut::MapType m_mode;

    bool visible()
    {
	if (pUI && pUI->DisplayContrastWindow->visible())
	    return true;
	return false;
    }


    void update(PdvGlWindow *pWin = NULL);

 
    void show(void)
    {
	pUI->show();
    }

    void hide(void)
    {
        pUI->DisplayContrastWindow->hide();
    }

    void set_map_type(EdtLut::MapType type);

    void update_lut_values(void);
    void update_display(bool redraw_image);

    void reset();

};



#endif
