
//#include "config.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include "PdvGlWindow.h"
#include "PdvGlScroller.h"



void
PdvGlScroller::resize(int w, int h)

{
    // printf("resize scroll %d %d\n",w,h);
    Fl_Group::resize(x(), y(), w,h);
     UpdateScrollbars();
}

void 
PdvGlScroller::resize(int x, int y, int w, int h)

{
    // printf("resize scroll %d %d %d %d\n", x,y,w,h);
    Fl_Group::resize(x,y,w,h);
    UpdateScrollbars();

}

void PdvGlScroller::UpdateScrollbars()

{
    if (!pPdvWindow->GetImage())
    {

	// No Image - don't worry about it
	bottom->hide();
	right->hide();

	return;

    }

    right->resize(x()+w() - scroll_width, y(), scroll_width, h()-scroll_width);
	
    bottom->resize(x(),y()+h() - scroll_width, w()-scroll_width, scroll_width);

    pPdvWindow->show_width =   pPdvWindow->GetImage()->GetWidth();
    pPdvWindow->show_height =  pPdvWindow->GetImage()->GetHeight();

    pPdvWindow->scaled_width =   (int) (pPdvWindow->GetImage()->GetWidth() * pPdvWindow->zoom.GetZoomX());
    pPdvWindow->scaled_height =  (int) (pPdvWindow->GetImage()->GetHeight() * pPdvWindow->zoom.GetZoomY());

    pPdvWindow->scalex = (float) (w()-scroll_width) / ( pPdvWindow->scaled_width);
    pPdvWindow->scaley = (float) (h()-scroll_width) / ( pPdvWindow->scaled_height);


    int gl_w = w();
    int gl_h = h();

    if (pPdvWindow->scalex >= 1.0 || pPdvWindow->ZoomToWindow())
    {
	bottom->hide();
	pPdvWindow->zoom.SetSourceX(0);
    }    
    else
    {
	gl_h -= scroll_width;
    }

    if (pPdvWindow->scaley >= 1.0 || pPdvWindow->ZoomToWindow())
    {
	right->hide();
	pPdvWindow->zoom.SetSourceY(0);
    }
    else
    {
	gl_w -= scroll_width;
    }

    if (gl_w < pPdvWindow->scaled_width)
    {
	int max_start;

	max_start = (int) (pPdvWindow->show_width - (gl_w / pPdvWindow->zoom.GetZoomX()));

	if (pPdvWindow->zoom.GetSourceX() > max_start)
	    pPdvWindow->zoom.SetSourceX( max_start);

	bottom->range(0,pPdvWindow->show_width - (int) (pPdvWindow->scalex * pPdvWindow->show_width));
	bottom->slider_size(pPdvWindow->scalex);
	bottom->show();

	bottom->Fl_Valuator::value(pPdvWindow->zoom.GetSourceX());
	
    }

    if (gl_h < pPdvWindow->scaled_height)
    {
	int max_start;

	max_start = (int) (pPdvWindow->show_height - (gl_h / pPdvWindow->zoom.GetZoomY()));

	if (pPdvWindow->zoom.GetSourceY() > max_start)
	    pPdvWindow->zoom.SetSourceY( max_start);

	right->range(0,pPdvWindow->show_height - (int) (pPdvWindow->scaley * pPdvWindow->show_height));
	right->slider_size(pPdvWindow->scaley);
	right->show();
	right->Fl_Valuator::value((double) pPdvWindow->zoom.GetSourceY());
    }

    pPdvWindow->resize(x(), y(), gl_w, gl_h);

    
}

void
pdv_gl_hscroll_cb(Fl_Scrollbar *w, void *p)

{
    PdvGlScroller *window = (PdvGlScroller *) p;

    double newx = w->value();

    window->pPdvWindow->zoom.SetSourceX((int) newx);

    window->pPdvWindow->redraw();

}

void
pdv_gl_vscroll_cb(Fl_Scrollbar *w, void *p)

{
    PdvGlScroller *window = (PdvGlScroller *) p;

    double newy = w->value();

    window->pPdvWindow->zoom.SetSourceY((int) newy);

    window->pPdvWindow->redraw();

}

PdvGlScroller::PdvGlScroller(int x, int y, int w, int h, const char *l, PdvGlWindowFactory f) : Fl_Group(x,y,w,h,l)

{
    scroll_width = 20;
    bottom = new Fl_Scrollbar(0,h - scroll_width, w, scroll_width);
     bottom->type(FL_HORIZONTAL);
   right = new  Fl_Scrollbar(w - scroll_width, y, scroll_width, h);
    right->type(FL_VERTICAL);

    bottom->hide();
    right->hide();

    bottom->callback((Fl_Callback *)pdv_gl_hscroll_cb,this);
    right->callback((Fl_Callback *)pdv_gl_vscroll_cb,this);


    if (f)
	pPdvWindow = f(x,y,w-scroll_width,h-scroll_width,l);
    else
	pPdvWindow = new PdvGlWindow(x,y,w-scroll_width,h-scroll_width,l);
  
}
