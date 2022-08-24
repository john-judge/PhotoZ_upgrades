#ifndef PIXEL_TABLE_H
#define PIXEL_TABLE_H

#include "EdtImage.h"

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

class PixelTable : public Fl_Group

{

    Fl_Box *** cells;
    int rows, columns;

    int startrow;
    int startcol;

    EdtImage *m_image;

    int cell_width;
    int cell_height;

    bool use_hex;

    int pixel_x;
    int pixel_y;

    int last_pixel_x;
    int last_pixel_y;

    int xmax;
    int ymax;

    char pixelformat[32];

    int channel;
    int nchannels;

        
    void set_box_labelcolor(Fl_Color);

    bool firsttime;

    bool show_all_colors;

public:
 

    PixelTable(int X, int Y, int W, int H, const char*L=0);

    void update(const int pX, const int pY);

    void update();

    void set_image(EdtImage *pImage)
    {
	m_image = pImage;
	if (pImage)
	{
	    xmax = pImage->GetWidth()-1;
	    ymax = pImage->GetHeight()-1;
	}
	else
	{
	    xmax = 0;
	    ymax = 0;
	}

    }

	void set_channel(const int chan);
	
	
	int get_channel() const
	{
		return channel;
	}
	
    void set_hex(bool state);

    void set_n_colors(const int n)
    {
	if (n != nchannels)
	{
	    nchannels = n;
	    firsttime = true;
	    if (nchannels <= channel)
		set_channel(0);
	}

    }

};

#endif
