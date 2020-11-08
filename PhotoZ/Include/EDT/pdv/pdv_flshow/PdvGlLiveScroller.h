#ifndef PDV_GL_LIVE_SCROLLER_H
#define PDV_GL_LIVE_SCROLLER_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include "PdvGlWindow.h"
#include "PdvGlScroller.h"
#include "PdvGlLiveWindow.h"


class PdvGlLiveScroller  : public PdvGlScroller

{

public:
    

    PdvGlLiveScroller(int x, int y, int w, int h, const char *label=0)
	: PdvGlScroller(x,y,w,h,label,newLiveWindow)
    {

    }
 
    PdvGlLiveWindow *GetLiveWindow()
    {
	return (PdvGlLiveWindow *) pPdvWindow;
    }

 
    
};



#endif
