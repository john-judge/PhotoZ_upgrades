#ifndef PDV_GL_SCROLLER_H
#define PDV_GL_SCROLLER_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include "PdvGlWindow.h"

class PdvGlWindow;

class PdvGlScroller  : public Fl_Group

{

public:
    
    void resize(int w, int h);
    void resize(int x, int y, int w, int h);

    PdvGlScroller(int x, int y, int w, int h, const char *l=0, PdvGlWindowFactory f = NULL);
 
    PdvGlWindow *pPdvWindow;

    Fl_Scrollbar *right;
    Fl_Scrollbar *bottom;

    int scroll_width;

    void UpdateScrollbars();

    
};



#endif
