

#ifndef HISTOGRAMMER_H
#define HISTOGRAMMER_H

#include "edtdefines.h"
#include "PlotWindow.h"
#include "EdtHistogram.h"

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>

class Histogrammer {

    Fl_Double_Window *frame;

    PlotWindow *plot;

    Fl_Group *ChannelButtons;
    Fl_Button *green, *red, *blue;

    EdtImage *m_image;

    int channel;

    EdtHistogram histogram;
    EdtImgStats  stats[4];

    Fl_Box *NPixels;
    Fl_Box *MinValue[3];
    Fl_Box *MaxValue[3];
    Fl_Box *Mean[3];
    Fl_Box *StDev[3];
    Fl_Box *Median[3];

    Fl_Box *IndexValue;
    Fl_Box *IndexFreq;

    EdtRect box;

    static void histo_button_cb(PlotWindow *w, Histogrammer *p);

    static void histo_reset_y_cb(Fl_Button *w, Histogrammer *p);

    Fl_Button * ColorButton(const int x, const int y, const int width, const int height, 
			  const char *label, const int color, int data);


public:

    Histogrammer(EdtImage *pImage, const int width = 400, const int height = 550);

    void update(EdtImage *pImage, EdtRect *pBox = NULL);

    void update();
    void update_stats(const int chan);

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

};


#endif
