////////////////////////////////////////////////////////////////


#include "edtdefines.h"

#include "CamBrowser.h"

#include "CamSelector.h"

#include "CamSelectorUI.h"

#include <FL/fl_draw.H>

static int INDENT=20;

#include <stdio.h>


static void CamBrowser_callback(Fl_Widget *o,void *) {
  ((CamBrowser *)o)->callback();
}

CamBrowser::CamBrowser(int X,int Y,int W,int H,const char*l)
: Fl_Browser_(X,Y,W,H,l) {
  type(FL_HOLD_BROWSER);
  when(FL_WHEN_RELEASE);
}

#if 1

void *CamBrowser::item_first() const 

{
    CameraConfig *pItem= cfg->pFirstItem;

    return pItem;
}

void *CamBrowser::item_next(void *pItem) const 

{
    CameraConfig *pNext = ((CameraConfig*)pItem)->next;

    if (pNext == cfg->pFirstItem)
	return NULL;
    else
	return pNext;
}

void *CamBrowser::item_prev(void *pItem) const 

{
    if (pItem == cfg->pFirstItem)
	return NULL;
    else
	return ((CameraConfig*)pItem)->prev;
}

int CamBrowser::item_selected(void *pItem) const 

{
    return ((CameraConfig*)pItem)->selected;
}

void CamBrowser::item_select(void *pItem,int v) 

{
    ((CameraConfig*)pItem)->selected = (v != 0);
}

#endif

int CamBrowser::item_height(void *pItem) const 

{
  return ((CameraConfig *)pItem)->visible ? textsize()+2 : 0;
}

int CamBrowser::incr_height() const 

{
    return textsize()+2;
}


void CamBrowser::item_draw(void *v, int X, int Y, int, int) const 


{
    CameraConfig *pItem = (CameraConfig *)v;
     
     
    fl_color(fl_rgb_color(100,100,100));


    if (pItem->children.Length()) {
	// draw a plus/minus inside a box

	fl_rect(X,Y,boxwidth(),boxwidth());

	fl_line(X+2,Y+boxwidth()/2,X+boxwidth() - 2,Y+boxwidth()/2);
	if (!pItem->opened)
	{
	    fl_line(X+boxwidth()/2,Y+2,X+boxwidth()/2,Y+boxwidth()-2);
	}

    }

    if (pItem->selected) 
	fl_color(fl_contrast(FL_BLACK,FL_SELECTION_COLOR));
    else 
	fl_color(FL_BLACK);

    X += textsize() + 4;

    fl_font(textfont() | FL_BOLD, textsize());


    if (pItem->children.Length())
	fl_draw(pItem->camera_class, X, Y+13);
    else
    {
	char description[256];

	sprintf(description, "%s: %s", pItem->camera_model, pItem->camera_info);	
	
	fl_font(textfont() | FL_BOLD, textsize());

	fl_draw(description, X, Y+13);
    }

 
}

int CamBrowser::item_width(void *v) const 

{
  CameraConfig *pItem = (CameraConfig *)v;

  if (!pItem->visible) return 0;

  int width = 0;

  if (pItem->children.Length()) 
  {
      width += (int) (INDENT + fl_width(pItem->camera_class));
  }
  else
  {
      width += (int) (fl_width(pItem->camera_model));
  }

  return width;
}


int CamBrowser::handle(int e) {


    static CameraConfig *title;

    CameraConfig *pItem;

    int X,Y,W,H; 
        
    static int change;
    static int py;
    int my;

    bbox(X,Y,W,H);
    
    if (Fl_Group::handle(e)) 
	return 1;
	
    int mx = Fl::event_x() - hposition();

    switch(e)
    {
	case FL_PUSH:

    // figure out which item we're on

	if (!Fl::event_inside(X, Y, W, H)) 
	    return 0;

	if (Fl::visible_focus()) 
	    Fl::focus(this);

	my = py = Fl::event_y();

	change = 0;
        
	change = select_only(find_item(my), when() & FL_WHEN_CHANGED);

	pItem = (CameraConfig *) selection();


    
	if (pItem && pItem->IsParent())
	{
	    CameraConfig *pChild;
	    // toggle
	    if (pItem->opened)
	    {
		//close
		    // open
		for (pChild = pItem->children.GetFirst(); pChild != NULL; pChild=pItem->children.GetNext())
		{
		    pChild->visible = false;
		}
	    }
	    else
	    {
		// open
		for (pChild = pItem->children.GetFirst(); pChild != NULL; pChild=pItem->children.GetNext())
		{
		    pChild->visible = true;
		}
	    }

	    pItem->opened = !pItem->opened;

	    redraw();

	}

	return 1;
     
    case FL_RELEASE:
     
	if (change) {
	    set_changed();
	    if (when() & FL_WHEN_RELEASE) 
		do_callback();
	}
	return 1;

    }

  return Fl_Browser_::handle(e);
}

