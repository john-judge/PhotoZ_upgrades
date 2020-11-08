

#ifndef Profiler_H
#define Profiler_H

#include "edtdefines.h"
#include "PlotWindow.h"
#include "EdtImgProfile.h"

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Output.H>

class Profiler {

    Fl_Double_Window *frame;

    PlotWindow *plot;

    EdtImage *m_image;

    int channel;

    Fl_Output *coordinate;
    Fl_Output *value;


     EdtImgProfile profile;

public:

static void profile_button_cb(PlotWindow *w, Profiler *profile);


    int x1, y1, x2, y2;

    Profiler(EdtImage *pImage, const int width = 550, const int height = 360);

    void update(EdtImage *pImage, const int p1_x, const int p1_y, const int p2_x, const int p2_y);

    void update();

    void SelectChannel(const int newchannel)
    {
	channel = newchannel;
    }

    int Channel() const
    {
	return channel;
    }

    bool visible()

    {
	if (frame && frame->visible())
	    return true;
	return false;
    }

    void show()
    {
	if (frame)
	    frame->show();
    }

} ;


#endif
