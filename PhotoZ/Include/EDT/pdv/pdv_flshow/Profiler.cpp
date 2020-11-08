
#include "EdtImage.h"

#include "PlotWindow.h"
#include "Profiler.h"

#include <stdio.h>


void Profiler::profile_button_cb(PlotWindow *w, Profiler *profile)

{
    // get the data value from the cursor 

    int index = (int) w->cursor_x;

    if (index < 0 || index >= profile->profile.GetNPixels())
	return;
    else
    {
	int x,y,values[4];

	profile->profile.GetPointValuesAtIndex(index, x, y, values);

	char buf[64];

	sprintf(buf,"%4d: (%d,%d)", index, x, y);

	profile->coordinate->value(buf);

	if (profile->profile.GetNColors() == 1)
	    sprintf(buf,"%d", values[0]);
	else if (profile->profile.GetNColors() == 3)
	    sprintf(buf,"%d, %d, %d", values[0], values[1], values[2]);

	profile->value->value(buf);

    }
    
}

Profiler::Profiler(EdtImage *pImage, const int width, const int height)

{
    m_image = pImage;

    channel = 0;

    frame = new Fl_Double_Window(width, height, "Profile");


    plot  = new PlotWindow(0,0, width, height-30);

 
   
    coordinate = new Fl_Output(110, height-30, 150, 25);
    coordinate->color(FL_DARK1);
    coordinate->copy_label("Coordinate");
    coordinate->align(FL_ALIGN_LEFT);
  
    value = new Fl_Output(300, height-30, 85, 25);
    value->color(FL_DARK1);
    value->copy_label("Value");
    value->align(FL_ALIGN_LEFT);
  
    frame->resizable(plot);
   
    frame->show();

    plot->callback((Fl_Callback *) Profiler::profile_button_cb, this);

    x1 = y1 = x2 = y2 = 0;

}


void Profiler::update(EdtImage *pImage, const int p1_x, const int p1_y, const int p2_x, const int p2_y)

{
    m_image = pImage;

 
    if (x1 != p1_x ||
	y1 != p1_y ||
	x2 != p2_x ||
	y2 != p2_y)
    {
	coordinate->value("");
	value->value("");
    }
 
    x1 = p1_x;
    y1 = p1_y;
    x2 = p2_x;
    y2 = p2_y;

    update();

}

void Profiler::update()

{
   if (m_image && (x1 != x2 || y1 != y2))
   {
       profile.Compute(m_image, x1, y1, x2, y2);
 	
       for (int chan = 0; chan < profile.GetNColors();chan++)
	   plot->SetVector(profile.GetNPixels(), TYPE_UINT, profile.GetProfile(chan), chan);

       plot->redraw();
   }
}


