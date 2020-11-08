
#ifndef EDT_PLOTWINDOW_H
#define EDT_PLOTWINDOW_H


#include <FL/Fl.H>

#include <FL/Fl_Box.H>

#include "edtdefines.h"

class DataVector
{
public:

    int size;
    int elements;
    int elementsize;
    int type;

    void *data;

    double min_value;
    double max_value;


    int channels;

    DataVector()
    {
	data = NULL;
	elements = 0;
	size = 0;
	channels = 1;
    }

    DataVector(int newelements, int newtype, void *newdata = NULL)
    {
	data = NULL;
	elements = 0;
	size = 0;
	
	SetValues(newelements, newtype, newdata);

    }
   
    void SetValues(int newelements, int newtype, void *newdata = NULL)
    {
	elements = newelements;
	type = newtype;
	elementsize = (int) TypeSize(newtype);

	int newsize = elementsize * elements;

	if (size != newsize)
	{
	    size = newsize;
	    if (data)
		delete ((char *) data);
	    data = (void *) new unsigned char[size];
	}
	    
	if (newdata && data)
	    memcpy(data,newdata,size);
	else
	    if (data)
		memset(data,0, size);

	if (type >= TYPE_RGB && type <= TYPE_BGRA)
	    channels = 3;
	else
	    channels = 1;

	GetRange();
    }

    int GetInt(int index, int channel = 0)
    {
	if (index < 0 || index >= elements)
	    return 0;

	switch (type)
	{
	case TYPE_BYTE:
	    return ((uchar *) data)[index];
	    break;
	case TYPE_CHAR:
	    return ((char *) data)[index];
	    break;
	case TYPE_USHORT:
	    return ((unsigned short *) data)[index];
	    break;
	case TYPE_SHORT:
	    return ((short *) data)[index];
	    break;
	case TYPE_INT:
	    return ((int *) data)[index];
	    break;
	case TYPE_UINT:
	    return ((unsigned int *) data)[index];
	    break;
	case TYPE_FLOAT:
	    return (int) ((float *) data)[index];
	    break;
	case TYPE_DOUBLE:
	    return (int) ((double *) data)[index];
	    break;
	case TYPE_RGB:
	case TYPE_BGR:
	    return ((uchar *) data)[index*3 + channel];
	    break;
	case TYPE_RGBA:
	case TYPE_BGRA:
	    return ((uchar *) data)[index*4 + channel];
	    break;

	}

	return 0;
    }

    double GetDouble(int index, int channel = 0)

    {
	if (index < 0 || index >= elements)
	    return 0;

	switch (type)
	{
	case TYPE_BYTE:
	    return ((uchar *) data)[index];
	    break;
	case TYPE_CHAR:
	    return ((char *) data)[index];
	    break;
	case TYPE_USHORT:
	    return ((unsigned short *) data)[index];
	    break;
	case TYPE_SHORT:
	    return ((short *) data)[index];
	    break;
	case TYPE_INT:
	    return ((int *) data)[index];
	    break;
	case TYPE_UINT:
	    return ((unsigned int *) data)[index];
	    break;
	case TYPE_FLOAT:
	    return ((float *) data)[index];
	    break;
	case TYPE_DOUBLE:
	    return ((double *) data)[index];
	    break;
	case TYPE_RGB:
	case TYPE_BGR:
	    return ((uchar *) data)[index*3 + channel];
	    break;
	case TYPE_RGBA:
	case TYPE_BGRA:
	    return ((uchar *) data)[index*4 + channel];
	    break;

	}

	return 0;

    }

    void Set(int index, int value, int channel = 0)

    {
	if (index < 0 || index >= elements)
	    return;

	switch (type)
	{
	case TYPE_BYTE:
	     ((uchar *) data)[index] = value;
	    break;
	case TYPE_CHAR:
	     ((char *) data)[index] = value;
	    break;
	case TYPE_USHORT:
	     ((unsigned short *) data)[index] = value;
	    break;
	case TYPE_SHORT:
	     ((short *) data)[index] = value;
	    break;
	case TYPE_INT:
	     ((int *) data)[index] = value;
	    break;
	case TYPE_UINT:
	     ((unsigned int *) data)[index] = value;
	    break;
	case TYPE_FLOAT:
	     ((float *) data)[index] = value;
	    break;
	case TYPE_DOUBLE:
	     ((double *) data)[index] = value;
	    break;
	case TYPE_RGB:
	case TYPE_BGR:
	     ((uchar *) data)[index*3 + channel] = value;
	    break;
	case TYPE_RGBA:
	case TYPE_BGRA:
	     ((uchar *) data)[index*4 + channel] = value;
	    break;

	}
    }

    void Set(int index, double value, int channel = 0)
    {
	if (index < 0 || index >= elements)
	    return;

	switch (type)
	{
	case TYPE_BYTE:
	     ((uchar *) data)[index] = (unsigned char)value;
	    break;
	case TYPE_CHAR:
	     ((char *) data)[index] = (char)value;
	    break;
	case TYPE_USHORT:
	     ((unsigned short *) data)[index] = (unsigned short) value;
	    break;
	case TYPE_SHORT:
	     ((short *) data)[index] = (short)value;
	    break;
	case TYPE_INT:
	     ((int *) data)[index] = (int) value;
	    break;
	case TYPE_UINT:
	     ((unsigned int *) data)[index] = (unsigned int) value;
	    break;
	case TYPE_FLOAT:
	     ((float *) data)[index] = (float)value;
	    break;
	case TYPE_DOUBLE:
	     ((double *) data)[index] = value;
	    break;
	case TYPE_RGB:
	case TYPE_BGR:
	     ((uchar *) data)[index*3 + channel] = (uchar) value;
	    break;
	case TYPE_RGBA:
	case TYPE_BGRA:
	     ((uchar *) data)[index*4 + channel] = (uchar) value;
	    break;

	}
    }

    void GetRange()
    {
	int i;

	if (!data)
	    return;

	min_value = max_value = 
	    GetDouble(0);
	
	int chan;

	for (chan = 0; chan < channels; chan++)

	{
	    for (i=1;i<elements;i++)
	    {
		double v = GetDouble(i, chan);
    	    
		if (v > max_value)
		    max_value = v;
		else if (v < min_value)
		    min_value = v;

	    }
	}
    }

};

class PlotWindow : public Fl_Box

{

public:

    enum PlotType {
	BarPlot,
	LinePlot
    };

private:

    DataVector **v;
    int nvectors;
    double upper_margin;

    int max_size;

    double min_x, max_x;
    double min_y, max_y;

    double user_min_x, user_max_x; // if user sets min/max
    double user_min_y, user_max_y; // if user sets min/max

    double data_min_x, data_max_x; // if user sets min/max
    double data_min_y, data_max_y; // if user sets min/max

    double xscale, yscale;

    char *x_label;
    char *y_label;

    int  xticks;
    int	 yticks;
    
    int left_margin;
    int top_margin;
    int right_margin;
    int bottom_margin;

    int box_width;
    int box_height;

    int border;

    int bottom;

    char *title;

    double barwidth;

    bool fixed_y_min;
    bool fixed_y_max;
    bool fixed_x_min;
    bool fixed_x_max;

    bool growing_y_max;
    
    PlotType mode;

    int channel;

    bool rounding;
    bool do_all_channels;

    int imagetype; // for image histograms;

    void draw_axes();

    void draw_bars();

    void draw_lines();

    void draw_line(int chan,int chancolor);

    int channel_colors[3];

    bool point_box;

    bool cursor_line;

    int last_event;

public:

    PlotWindow(int X, int Y, int W, int H, const char *label = NULL);

    void SetVector(int n, int type, void *data, int vecnumber = 0);

    void clear();

    void update();

    void draw();

    int handle(int event);

    float cursor_x, cursor_y;

    void SetMode(PlotType newmode)
    {
	mode = newmode;
	redraw();
    }

    void SetChannel(int newchannel)
    {
	channel = newchannel;
    }
    int Channel() const
    {
	return channel;
    }

    void SetImageType(int newtype)
    {
	imagetype = newtype;
	channel_colors[1] = FL_GREEN;

	if (imagetype == TYPE_BGR || imagetype == TYPE_BGRA)
	{
	    channel_colors[0] = FL_BLUE;
	    channel_colors[2] = FL_RED;
	}
	else
	{
	    channel_colors[0] = FL_RED;
	    channel_colors[2] = FL_BLUE;
	}  

    }

    int ImageType() const
    {
	return imagetype;
    }


    void ResetY() 
    {
	max_y = 0;
	min_y = 0;

	SetYFixed(false);

	update();
    }

    void SetYAxis(const int ylow, const int yhigh);
    void SetXAxis(const int xlow, const int xhigh);

    void SetXFixed(const bool state)
    {
	fixed_x_min = fixed_x_max = state;
    }

    void SetYFixed(const bool state)
    {
	fixed_y_min = fixed_y_max = state;
    }

    bool GetMaxVectorValues(void);

    // cursor_line determines whether to draw vertical line at mouse

    void CursorLine(const bool state)
    {
	cursor_line = state;
    }
    bool CursorLine(void) const
    {
	return cursor_line;
    }

    void PointBox(const bool state)
    {
	point_box = state;
    }
    bool PointBox(void) const
    {
	return point_box;
    }

    int LastEvent()
    {
	return last_event;
    }

};




#endif
