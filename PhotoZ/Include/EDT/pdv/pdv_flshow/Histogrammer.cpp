
#include "EdtImage.h"

#include "PlotWindow.h"
#include "Histogrammer.h"

#include <FL/Fl_Button.H>

#include <stdio.h>


void histogrammer_channel_cb(Fl_Button *w, void *data)

{
    Histogrammer *pUI = (Histogrammer *) data;

    int index;

    const char *p = w->label();

    if (!strcmp(p,"Red"))
	index = 0;
    else if  (!strcmp(p,"Green"))
	index = 1;
    else if  (!strcmp(p,"Blue"))
	index = 2;

    pUI->SelectChannel(index);    
    pUI->update();


}

void Histogrammer::histo_button_cb(PlotWindow *w, Histogrammer *p)

{
    // get the data value from the cursor 

    int index = (int) w->cursor_x;

    if (index < 0 || index >= p->histogram.GetNBins())
	return;
    else
    {
	int values[4];
	
	int *data = p->histogram.GetHistogram(p->channel);

	char buf[64];

	sprintf(buf,"%d", index);

	p->IndexValue->copy_label(buf);

	sprintf(buf, "%d", data[index]);

	p->IndexFreq->copy_label(buf);

    }

}

void Histogrammer::histo_reset_y_cb(Fl_Button *w, Histogrammer *p)

{
    p->plot->ResetY();

}

static Fl_Box *
histo_val_box(int x, int y, int width, int height)

{
    Fl_Box *o = new Fl_Box(x, y, width, height);
    o->color(FL_DARK1);
    o->box(FL_DOWN_BOX);
    o->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
    return o;
}

Fl_Button *
Histogrammer::ColorButton(const int x, const int y, const int width, const int height, 
			  const char *label, const int color, int data)

{
    Fl_Button * o = new Fl_Button(x, y, width, height, label);
    o->type(102);
    o->down_box(FL_DOWN_BOX);
    o->color(color);
    o->selection_color(color);
    o->labelcolor(FL_BACKGROUND2_COLOR);
    o->callback((Fl_Callback*)histogrammer_channel_cb, this);
    o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    //o->user_data((void *)data);

    return o;
}


Histogrammer::Histogrammer(EdtImage *pImage, const int width, const int height)

{
    m_image = pImage;

    channel = 0;

    frame = new Fl_Double_Window(width, height, "Histogram");

    int statline = height - 200;

    plot  = new PlotWindow(0,0, width, statline);
    plot->callback((Fl_Callback *) Histogrammer ::histo_button_cb, this);

    frame->resizable(plot);

    { 
	Fl_Group* o = new Fl_Group(0, statline, 515, 165, "");
	o->box(FL_ENGRAVED_BOX);
	o->align(FL_ALIGN_TOP_LEFT);
	{ Fl_Box* o = new Fl_Box(5, statline + 10, 80, 25, "N Pixels");
	o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	}
	{ Fl_Box* o = new Fl_Box(5, statline + 35, 80, 25, "Min Pixel Value");
	o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	}
	{ Fl_Box* o = new Fl_Box(5, statline + 60, 80, 25, "Max Pixel Value");
	o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	}
	{ Fl_Box* o = new Fl_Box(5, statline + 85, 80, 25, "Mean");
	o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	}
	{ Fl_Box* o = new Fl_Box(5, statline + 110, 80, 25, "Standard Deviation");
	o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	}
	{ Fl_Box* o = new Fl_Box(5, statline + 135, 80, 25, "Median");
	o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	}
	{ 
	    Fl_Group* o = new Fl_Group(140, statline, 115, 165);
	    o->box(FL_ENGRAVED_BOX);
	    NPixels = histo_val_box(150, statline + 10, 85, 25);

	    MinValue[0] = histo_val_box(150,statline + 35, 85, 25);

	    MaxValue[0] = histo_val_box(150, statline + 60, 85, 25);
	    Mean[0] = histo_val_box(150, statline + 85, 85, 25);
	    StDev[0] = histo_val_box(150, statline + 110, 85, 25);
	    Median[0] = histo_val_box(150, statline + 135, 85, 25);
	    o->end();
	}
	o->end();

    }

    { 
	Fl_Group* o = ChannelButtons = new Fl_Group(60, statline + 170, 315, 25, "Color");
	o->align(FL_ALIGN_LEFT);
	red = ColorButton(95, statline + 170, 65, 20, "Red", FL_DARK_RED, 0);
	green = ColorButton(180, statline + 170, 65, 20, "Green", FL_DARK_GREEN, 1);
	blue = ColorButton(265, statline + 170, 65, 20, "Blue",FL_DARK_BLUE,2);
	o->end();
    }

    {
	Fl_Button *o = new Fl_Button(265,statline + 10, 65, 20,"Reset Y");
	o->user_data(this);
	o->callback((Fl_Callback *) histo_reset_y_cb, this);
    }
    {
	Fl_Box *o = new Fl_Box(265, statline + 35, 40, 20, "Index");
	IndexValue = new Fl_Box(305, statline + 35, 65, 20);
	IndexValue->box(FL_DOWN_BOX);
    }
    {
	Fl_Box *o = new Fl_Box(265, statline + 60, 40, 20, "Freq.");
	IndexFreq = new Fl_Box(305, statline + 60, 65, 20);
	IndexFreq->box(FL_DOWN_BOX);
    }


    frame->end();

    box.height = box.width = box.x = box.y = 0;

    frame->show();

}


void Histogrammer::update(EdtImage *pImage, EdtRect *pBox)

{
    m_image = pImage;

    if (pBox)
	box = *pBox;

    if (pImage->GetNColors() > 1)	
	ChannelButtons->show();
    else
	ChannelButtons->hide();

    update();

}

void Histogrammer::update_stats(int chan)

{
    char buf[80];

    sprintf(buf,"%d", stats[chan].m_nN);
    NPixels->copy_label(buf);

    sprintf(buf,"%d", stats[chan].m_nMin);
    MinValue[0]->copy_label(buf);

    sprintf(buf,"%d", stats[chan].m_nMax);
    MaxValue[0]->copy_label(buf);

    sprintf(buf,"%f", stats[chan].m_dMean);
    Mean[0]->copy_label(buf);

    sprintf(buf,"%f", stats[chan].m_dSD);
    StDev[0]->copy_label(buf);

    sprintf(buf,"%d", stats[chan].m_nMedian);
    Median[0]->copy_label(buf);

}


void Histogrammer::update()

{
    if (m_image)
    {
	if (box.width)
	{
	    EdtImage workimage;
	    workimage.AttachToParent(m_image, box.x, box.y, box.width, box.height);
	    histogram.Compute(&workimage);

	}
	else
	{
	    histogram.Compute(m_image);
	}

	histogram.ComputeStatistics(stats);

	update_stats(channel);

	for (int band = 0; band < histogram.GetNColors();band++)
	    plot->SetVector(histogram.GetNBins(), TYPE_UINT, 
		histogram.GetHistogram(band), band);

	plot->SetChannel(channel);

	plot->redraw();
    }


}


