
#ifndef PDVGLWINDOW_H
#define PDVGLWINDOW_H

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Scrollbar.H>

#include "edtinc.h"

#include "PdvInput.h"


#include "ImageEventFunc.h"
#include "EdtList.h"

#include "BufferTimerData.h"

#include "ZoomData.h"

#include "EdtLut.h"

class bounding_box {

public:
    int x1, y1, x2, y2;
    bool active;

    bounding_box() {x1 = y1 = x2 = y2 = 0; active = false;}

    void rect_from_box(EdtRect *bp)
    {
	// get upper corner
	if (x1 >= x2)
	{
	    bp->x = x2;
	    bp->width = x1 - x2 + 1;
	}
	else
	{
	    bp->x = x1;
	    bp->width = x2 - x1 + 1;
	    
	}
	if (y1 >= y2)
	{
	    bp->y = y2;
	    bp->height = y1 - y2 + 1;
	}
	else
	{
	    bp->y = y1;
	    bp->height = y2 - y1 + 1;
	    
	}
    }

    int width() const
    {
	return (abs(x1 - x2) + 1);
    }

    int height() const
    {
	return (abs(y1 - y2) + 1);
    }

};



class PdvGlWindow : public Fl_Gl_Window {
 

 protected:

    void StartBox(bounding_box *b, const int x, const int y);
    void DragBox(bounding_box *b, const int x, const int y);
    void EndBox(bounding_box *b, const int x, const int y);
 
    void SetCursorCoords(int new_x, int new_y);

    bool m_bActiveFlag;
    bool m_bTimedOut;

    void TransformSetup();

    int last_display_type;

    void draw();
 
    int handle(int eventn);

    int cursor_x, cursor_y;
    int image_cursor_x, image_cursor_y;

    u_char *buffer;

    EdtImage *pSingle;
    EdtImage *pDataImage;
    EdtImage *pDisplayImage;
    EdtImage *pRawImage;

    bool TransformActive;
    bool Mono2Color;

    bool zoom_to_window;
    bool transform;


    /* box or line coordinates */




public:
     
 
    PdvGlWindow(int x, int y, int w, int h, const char *l=0);


    void resize(int x, int y, int w, int h);

    virtual ~PdvGlWindow();

    EdtImage * GetImage()
    {
	return pDataImage;
    }

    EdtImage * GetRawImage()
    {
	return pRawImage;
    }

    bounding_box line;

    bounding_box box;
    enum CursorMode {
	NoOp,
	ZoomUp,
	ZoomDown,
	Line,
	Box
    }; 

    
    CursorMode cursor_mode; 
   
    void SetCursorMode(const CursorMode mode)
    {
	cursor_mode = mode;
	if (mode == Box) 
	    line.active = false;
	else if (mode == Line)
	    box.active = false;
    }

    void ChangeZoom(double dChange, 
			     int centerx, int centery);

    void InitVariables();

    void GetCursorImageCoords(int &x, int &y) const;

  
    void SetMono2Color(const bool state)
    {
	Mono2Color = state;
    }
    bool GetMono2Color() const
    {
	return Mono2Color;
    }


    void SetTransformActive()
    {
	TransformActive = true;
	TransformSetup();
    }

    void ClearTransform(void)
    {
	TransformActive = false;
	TransformSetup();
    }

    void UpdateDisplayImage(void);
   
    
    ZoomData zoom;
   

    float scalex;
    float scaley;

    int show_width;
    int show_height;

    int scaled_width;
    int scaled_height;

    EdtLut *pLut;


    bool needs_redraw;
 
    bool ZoomToWindow()
    { 
	return zoom_to_window;
    }
    void ZoomToWindow(const bool val)
    {
	zoom_to_window = val;
    }
};

typedef PdvGlWindow * (*PdvGlWindowFactory)(int x, int y, int w, int h, const char *l);
#endif
