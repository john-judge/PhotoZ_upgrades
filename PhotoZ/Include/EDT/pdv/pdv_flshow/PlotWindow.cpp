

#include  "PlotWindow.h"

#include <FL/fl_draw.H>

#include <stdio.h>
#include <math.h>

PlotWindow::PlotWindow(int X, int Y, int W, int H, const char *label) :
	Fl_Box(X,Y,W,H,label)

{
    nvectors = 0;
    v = NULL;


    title = NULL;
    x_label = NULL;
    y_label = NULL;

    min_y = 0;
    max_y = 1;

    min_x = 0;
    max_x = 1;

    xscale = 1.0;
    yscale = 1.0;

    max_x = 0;

    fixed_y_min = true;
    fixed_y_max = false;

    fixed_x_min = false;
    fixed_x_max = false;

    user_min_x = 0;
    data_min_x = 0;

    user_min_y = 0;
    data_min_y = 0;

    user_max_x = 0;
    data_max_x = 0;

    user_max_y = 0;
    data_max_y = 0;


    xticks = 4;
    yticks = 4;

    border = 4;
    
    left_margin = 2 * border;
    top_margin = 2 * border;
    right_margin = 2 * border;
    bottom_margin = 2 * border;

    mode = LinePlot;
    barwidth = 1;

    box_width = w() - 4 * border;
    box_height = h() - 4 * border;

    bottom = y() + box_height + top_margin;

    channel = 0;

    rounding = true;
    do_all_channels = true;

    imagetype = 0;

    channel_colors[1] = FL_GREEN;
    channel_colors[0] = FL_RED;
    channel_colors[2] = FL_BLUE;

    upper_margin = 1.1;

    cursor_line = true;
    point_box = false;
   
}

void PlotWindow::SetVector(int n, int type, void *data, int vecnumber)

{
    int i;

    if (vecnumber >= nvectors)
    {
	DataVector **newp = new DataVector *[vecnumber+1];

	
	if (v)
	{
	    for (i=0;i<nvectors;i++)
		newp[i] = v[i];

	    delete v;
	}
	
	v = newp;

	for (i=nvectors;i<vecnumber;i++)
	    v[i] = NULL;

	v[vecnumber] = new DataVector();

	nvectors = vecnumber + 1;

    }
    else
    {
	if (!v[vecnumber])
	    v[vecnumber] = new DataVector();
    }

    v[vecnumber]->SetValues(n, type, data);

    v[vecnumber]->GetRange();

    update();

}

bool PlotWindow::GetMaxVectorValues(void)

{
    bool started = false;

    for (int vv = 0; vv < nvectors; vv++)
    {

	if (v[vv])
	{
	    if (started)
	    {
		if (v[vv]->elements > data_max_x)
		{
		    data_max_x = v[vv]->elements;
        
    		}
		if (v[vv]->max_value > data_max_y)
		{
		    data_max_y = v[vv]->max_value;
        
    		}
		if (v[vv]->min_value > data_min_y)
		{
		    data_min_y = v[vv]->min_value;	
    		}
	    }
	    else
	    {
		data_max_x = v[vv]->elements;
		data_max_y = v[vv]->max_value;
 		data_min_y = v[vv]->min_value;	
		started = true;
	    }
	}
    }

    return started;

}

void PlotWindow::update()

{
    bool started = false;

    double start_max_value = max_y;
 
    started = GetMaxVectorValues();

    if (!started || (data_max_x == 0) || 
	(data_min_y == data_max_y))
	return;

    if (!fixed_y_max)
    {
	max_y = data_max_y * upper_margin;
    }
    else
    {
	max_y = user_max_y;
    }

    if (!fixed_y_min)
    {
	min_y = data_min_y;
    }
    else
    {
	min_y = user_min_y;
    }

    if (max_y <= 0)
	return;

    if (!fixed_x_min)
	min_x = data_min_x;
    else
	min_x = user_min_x;

   if (!fixed_x_max)
	max_x = data_max_x;
    else
	max_x = user_max_x;


    if (rounding)
    {
	int digits = (int) floor(log10(max_y))-1;
	double rounder = pow((float)10,digits);

	max_y = (((int) (max_y / rounder)+1)) * rounder;
   	
    }
    // max_y never gets smaller unless reset 

    if (growing_y_max)
    {
	if (max_y < start_max_value)
    	    max_y = start_max_value;
    }

    // figure out margins

    char buf[32];
    int fwidth, fheight;

    sprintf(buf,"%d",(int) max_y);
    fl_measure(buf, fwidth, fheight);

    left_margin = fwidth + 2 * border;
    right_margin = border + fwidth/2;
    top_margin = fheight + 2 * border;
    bottom_margin = fheight + 2 * border;

    box_width = w() - left_margin - right_margin;
    box_height = h() - top_margin - bottom_margin;

    double oldxscale = xscale;
    
    xscale = (double) box_width / (double) (max_x - min_x);

    yscale = box_height / (max_y - min_y);

    bottom = y() + top_margin + box_height;
 
}

void PlotWindow::SetYAxis(int minval, int maxval)

{
    user_max_y = maxval;
    user_min_y = minval;

    fixed_y_min = true;
    fixed_y_max = true;

}

void PlotWindow::draw_axes()

{
    fl_color(FL_BLACK);
    fl_rect(x() + left_margin-1,y() + top_margin-1, box_width+2, box_height+2);
    
    // put in axes

    // plot v

    if (v == NULL)
	return;

    int vec;


  
    // label y axis

    int i;
    int dtick = box_height / yticks;
    double dval = max_y / yticks;

    int ty = bottom;
    for (i=0; i<= yticks; i++)
    {
	char buf[32];

	sprintf(buf, "%d", (int) (dval * i));

	fl_draw(buf, border, ty);

	ty -= dtick;

    }

    dtick = box_width / xticks;
    dval = max_x / xticks;

    int tx = left_margin;
    ty = bottom + border + fl_height();
    for (i=0; i<= xticks; i++)
    {
	char buf[32];

	sprintf(buf, "%d", (int) (dval * i));
	int half = (int)fl_width(buf)/2;


	fl_draw(buf, tx-half,ty);

	tx += dtick;

    }
}

void PlotWindow::draw_bars()

{
    if (v == NULL)
	return;

    int i;

    fl_color(FL_BLUE);

    if (do_all_channels && nvectors == 3)
    {
	double dx = x() + left_margin;
	int Channel_colors[3];
	Channel_colors[1] = FL_GREEN;

	if (imagetype == TYPE_BGR || imagetype == TYPE_BGRA)
	{
	    Channel_colors[0] = FL_BLUE;
	    Channel_colors[2] = FL_RED;
	}
	else
	{
	    Channel_colors[0] = FL_RED;
	    Channel_colors[2] = FL_BLUE;
	}
	
	for (i=0;i<v[0]->elements;i++)
	{
	    int bx, bw, by, bh;

	    double val[3];
	    int    colors[3];
	    double maxval = 0;
	    double minval;
	    int chan;
    	
    	    val[0] = v[0]->GetDouble(i);

	    maxval = minval = val[0];
	    colors[2] = colors[0] = colors[1] = 0;
	
	    for (chan = 1; chan < 3; chan++)
	    {
		val[chan] = v[chan]->GetDouble(i);
		if (val[chan] > maxval)
		{
		    maxval = val[chan];
		    if (colors[0])
			colors[1] = colors[0];
		    colors[0] = chan;
		}
		else if (val[chan] < minval)
		{
		    minval = val[chan];
		    if (colors[2])
			colors[1] = colors[2];
		    colors[2] = chan;
		}
	    }

	    double nextdx = dx + xscale;

	    if (maxval > 0)
	    {
		bx = (int) (dx);
		bw = (int) nextdx - bx;
		if (bw==0) bw = 1;

		for (chan = 0; chan < 3; chan++)
		{
		    fl_color(Channel_colors[colors[chan]]);
		    bh = (int) ((val[colors[chan]] * yscale) + 0.5);	   
		    by = bottom - bh;
		    fl_rectf(bx,by,bw,bh);
		}
	    }

	    dx = nextdx;

	}
    }
    else
    {
	DataVector *vv = v[channel];
	double dx = x() + left_margin;

    
	for (i=0;i<vv->elements;i++)
	{
	    int bx, bw, by, bh;

	    double val;
	    double maxval = 0;
    	
    	
	    val= vv->GetDouble(i,channel);

	    double nextdx = dx + xscale;

	    if (val > 0)
	    {
		bx = (int) (dx);
		bw = (int) (nextdx - bx);
		if (bw==0) bw = 1;
		bh = (int) ((val * yscale) + 0.5);	   
		by = bottom - bh;
		fl_rectf(bx,by,bw,bh);
	    }

	    dx = nextdx;

	}
    
    }

}

void PlotWindow::draw_line(int chan, int chancolor)

{
    fl_color(chancolor);

    DataVector *vv = v[chan];
    double dx = x() + left_margin;

    int last_by = 0;
    int last_bx = 0;

    int i;

    for (i=0;i<vv->elements;i++)
    {
	int bx, bw, by, bh;

	double val;
	double maxval = 0;
    
    
	val= vv->GetDouble(i);

	double nextdx = dx + xscale;

	bx = (int) (dx);
	bw = (int) nextdx - bx;
	if (bw==0) bw = 1;
	bh = (int) ((val * yscale) + 0.5);	   
	by = bottom - bh;
	if (i > 0)
	    fl_line(bx,by,last_bx,last_by);

	dx = nextdx;

	last_by = by;
	last_bx = bx;
    }
}


void PlotWindow::draw_lines()

{
    if (v == NULL)
	return;

   
    int chan;

    if (do_all_channels && nvectors == 3)
    {
	for (chan = 0; chan < 3; chan++)
	    draw_line(chan, channel_colors[chan]);

    }
    else
    {
	draw_line(channel, channel_colors[channel]);
    }
}

void PlotWindow::draw()

{
    // compute scale

    update();

    fl_color(color());
    fl_rectf(x(),y(),w(),h());
    fl_rectf(x() + left_margin,y() + top_margin, box_width, box_height, 255,255,255);
 
    draw_axes();

    if (mode == BarPlot)
	draw_bars();
    else if (mode == LinePlot)
	draw_lines();

    if (cursor_line)
    {
	fl_color(FL_BLACK);
	fl_line(x() + left_margin + (int)(cursor_x * xscale), y() + top_margin, x() + left_margin + (int) (cursor_x * xscale), y() + top_margin + box_height);
    }

}

int PlotWindow::handle(int event)
{
    switch (event)
    {
 
    case FL_PUSH:
    case FL_DRAG:
    case FL_RELEASE:
	if (Fl::event_x() >= left_margin + x() && 
	    Fl::event_x() <= x() + left_margin + box_width &&
	    Fl::event_y() >= top_margin + y() && Fl::event_y() <= y() + top_margin + box_height)
	{
	    cursor_y = Fl::event_y();
	    cursor_x = (xscale != 0) ? (Fl::event_x() - (left_margin+x())) / xscale:0;
	    cursor_y = (yscale != 0) ? (box_height - (Fl::event_y() - (top_margin+y()))) / yscale:0; 
	    last_event = event;
	    do_callback();
	}

	return 1;
    }
    return Fl_Box::handle(event);
}

