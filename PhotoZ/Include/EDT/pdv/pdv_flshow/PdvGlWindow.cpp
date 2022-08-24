
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <FL/math.h>
#include <FL/Fl_Output.H>

#include "edtinc.h"


#include "PdvGlScroller.h"

#include "PdvGlWindow.h"

#ifdef WIN32

#include <process.h>

#endif

#define ARROW_SIZE 8

PdvGlWindow::PdvGlWindow(int x, int y, int w, int h , const char *l) :
     Fl_Gl_Window(x,y,w,h,l)

{

    // initialize all variables

    InitVariables();

}


void PdvGlWindow::draw()

{

    int draw_w = w(), draw_h = h();
    int draw_w_pixels = draw_w;
    int draw_h_pixels = draw_h;

    static int count = 0;

    EdtImage *pImage;

    if (!valid()) {

	valid(1);


	glViewport(0,0,draw_w,draw_h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,(GLfloat)draw_w,0.0,(GLfloat)draw_h,-1.0,1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_FOG);
	glDisable(GL_DITHER);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);

    }

    int format = GL_BGR_EXT;
    int type = GL_UNSIGNED_BYTE;

    if (TransformActive && pDisplayImage)
    {
	pImage = pDisplayImage;
    }
    else
    {
	pImage = pDataImage;
    }

    switch (pImage->GetType())
	{
	case TypeBYTE:
	format = GL_LUMINANCE;
	break;

	case TypeUSHORT:
	format = GL_LUMINANCE;
	type = GL_UNSIGNED_SHORT;
	break;

	case TypeBGRA:
	format = GL_BGRA_EXT;
	break;

	case TypeRGB:
	format = GL_RGB;
	break;

	case TypeRGBA:
	format = GL_RGBA;
	break;

    }

    GLfloat rasterpos[4];
    int err = glGetError();

    int row_length,
    skip_rows,
    skip_pixels,
    alignment;

    int xpos = zoom.GetSourceX();
    int ypos = zoom.GetSourceY();

    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    if (show_width <= 0 || show_height <= 0)
    {
	//Fl::unlock();
	return;
    }

 
    glClearColor(0.5,0.5,0.5,0.0);
 
    glClear( GL_ACCUM_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPixelStorei(GL_UNPACK_ROW_LENGTH,pImage->GetWidth());


    if (zoom_to_window)
    {
	glPixelZoom(scalex,-scaley);
	glRasterPos2i(0,h());
	draw_w_pixels = pImage->GetWidth();
	draw_h_pixels = pImage->GetHeight();

	buffer = (u_char *) pImage->GetFirstPixel();

    }
    else
    {
	glPixelZoom(zoom.GetZoomX(),-zoom.GetZoomY());
	draw_w_pixels = (int) (draw_w / zoom.GetZoomX());
	if (draw_w_pixels > pImage->GetWidth() - xpos)
	    draw_w_pixels = pImage->GetWidth() - xpos;

	draw_h_pixels = (int) (draw_h / zoom.GetZoomY());
	if (draw_h_pixels > pImage->GetHeight() - ypos)
	    draw_h_pixels = pImage->GetHeight() - ypos;

	// calculate new raster pos from zoom

	glRasterPos2i(0,draw_h);

	buffer = (u_char *) pImage->GetFirstPixel();
	glPixelStorei(GL_UNPACK_SKIP_PIXELS,xpos);
	glPixelStorei(GL_UNPACK_SKIP_ROWS,ypos);

	
    }

    glGetFloatv(GL_CURRENT_RASTER_POSITION,rasterpos);

    if (buffer) 
	glDrawPixels(draw_w_pixels, draw_h_pixels, format,type, buffer);

    err = glGetError();

    if (line.active) 
    {
	double x1, y1, x2, y2;
	
	zoom.ScalePoint(line.x1,line.y1, x1, y1);
	zoom.ScalePoint(line.x2,line.y2, x2, y2);

	y1 = h() -y1;
	y2 = h() - y2;

	// compute angle

	double angle;

	angle = atan2((double) (y2 - y1),  (double) (x2 - x1)) ;

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glBegin(GL_LINES);

	glVertex2d(x1, y1);

	glVertex2d(x2, y2);

	glEnd();

	double d1_x = cos(angle + (M_PI * 0.75)) * ARROW_SIZE;
	double d1_y = sin(angle + (M_PI * 0.75)) * ARROW_SIZE;
	double d2_x = cos(angle + (M_PI * 1.25)) * ARROW_SIZE;
	double d2_y = sin(angle + (M_PI * 1.25)) * ARROW_SIZE;

	glBegin(GL_TRIANGLES);

	glVertex2d(x2, y2);
	glVertex2d(x2 + d1_x, y2 + d1_y );
	glVertex2d(x2 + d2_x, y2 + d2_y );
	
	glEnd();
	
	glPopMatrix();

    }
    
    if (box.active)
    {
	gl_color(FL_RED);

	double x1, y1, x2, y2;
	
	zoom.ScalePoint(box.x1,box.y1, x1, y1);
	zoom.ScalePoint(box.x2,box.y2, x2, y2);
	y1 = h() -y1;
	y2 = h() - y2;


	glBegin(GL_LINE_LOOP);
	glVertex2d(x1, y1);
	glVertex2d(x2, y1);
	glVertex2d(x2, y2);
	glVertex2d(x1, y2);
	glEnd();


    }

    needs_redraw = false;
    glFlush();

}


void PdvGlWindow::resize(int x, int y, int w, int h)

{

    Fl_Gl_Window::resize(x,y,w,h);

}

// change zoom around a point
//

void PdvGlWindow::ChangeZoom(double dChange, 
			     int centerx, int centery)

{
    double dZX, dZY;
    int nX, nY;

    zoom.GetZoomValues(nX, nY, dZX, dZY);

    int nWidth = w();
    int nHeight = h();

    if (!pDataImage)
	    return;

    if (dChange)
    {
	
	dZX *= dChange;
	dZY *= dChange;

	zoom.SetZoomXY(dZX, dZY);

    }

    // change window size to image size;

    nWidth = (int) (nWidth / dZX);
    nHeight = (int) (nHeight / dZX);

    if (nWidth > pDataImage->GetWidth())
	nWidth = pDataImage->GetWidth();
    if (nHeight > pDataImage->GetHeight())
	nHeight = pDataImage->GetHeight();

    int left, top, right, bottom;

    left = centerx - nWidth / 2;
    top = centery - nHeight / 2;

    if (left < 0)
	    left = 0;

    if (top < 0)
	    top = 0;

    right = left + nWidth;
    bottom = top + nHeight;

    if (right > w())
    {
	left = w() - nWidth;
	right = w();
    }

    if (bottom > h())
    {
	top = h() - nHeight;
	bottom = h();
    }
    

    if (left < 0)
	left = 0;

    if (top < 0)
	top = 0;

    zoom.SetZoom(left, top, dZX, dZX);

	
    ((PdvGlScroller *) parent())->UpdateScrollbars();

    redraw();

}

void PdvGlWindow::SetCursorCoords(int new_x, int new_y)

{
    cursor_x = new_x;
    cursor_y = new_y;
	
    GetCursorImageCoords(image_cursor_x, image_cursor_y);



}

int PdvGlWindow::handle(int eventn)

{
    switch (eventn)
    {
 
    case FL_PUSH:

	SetCursorCoords(Fl::event_x(),Fl::event_y());

	switch(cursor_mode)
	{
	case ZoomUp:
	    if (Fl::event_button() == FL_LEFT_MOUSE)
		ChangeZoom(1.5, cursor_x, cursor_y);
	    else if (Fl::event_button() == FL_RIGHT_MOUSE)
		ChangeZoom(0.6667, cursor_x, cursor_y);
	    break;
	case ZoomDown:
	    if (Fl::event_button() == FL_RIGHT_MOUSE)
		ChangeZoom(1.5, cursor_x, cursor_y);
	    else if (Fl::event_button() == FL_LEFT_MOUSE)
		ChangeZoom(0.6667, cursor_x, cursor_y);
	    break;
	case Line:
	    StartBox(&line,image_cursor_x, image_cursor_y);
	    break;
	case Box:
	    StartBox(&box,image_cursor_x, image_cursor_y);
	    break;
	}

    return 1;


    case FL_DRAG: 
	SetCursorCoords(Fl::event_x(),Fl::event_y());
	switch(cursor_mode)
	{
	case Line:
	    DragBox(&line,image_cursor_x, image_cursor_y);
	    break;
	case Box:
	    DragBox(&box,image_cursor_x, image_cursor_y);
	    break;
	}
	return 1;

    case FL_RELEASE:
	SetCursorCoords(Fl::event_x(),Fl::event_y());
	switch(cursor_mode)
	{
	case Line:
	    EndBox(&line,image_cursor_x, image_cursor_y);
	    break;
	case Box:
	    EndBox(&box,image_cursor_x, image_cursor_y);
	    break;
	}

    return 1;

    case FL_MOVE:
	SetCursorCoords(Fl::event_x(),Fl::event_y());
	
	
    return 1;


    default:
      return Fl_Gl_Window::handle(eventn);


    }

}


void PdvGlWindow::StartBox(bounding_box *b, const int x, const int y)

{
    b->x1 = b->x2 = x;
    b->y1 = b->y2 = y;
    b->active = false;
    redraw();

}

void PdvGlWindow::DragBox(bounding_box *b, const int x, const int y)

{   
    b->x2 = x;
    b->y2 = y;

    if (b->x1 == b->x2 && b->y1 == b->y2)
	b->active = false;
    else
	b->active = true;

    redraw();

}

void PdvGlWindow::EndBox(bounding_box *b, const int x, const int y)

{
    b->x2 = x;
    b->y2 = y;

    if (b->x1 == b->x2 && b->y1 == b->y2)
	b->active = false;
    else
	b->active = true;
      
    redraw();

}


PdvGlWindow::~PdvGlWindow()
{
 
}




void PdvGlWindow::GetCursorImageCoords(int &x, int &y) const

{
    x = cursor_x;
    y = cursor_y;

    if (zoom.IsActive())
    {

 	x = (int) (x / zoom.GetZoomX());
 	y = (int) (y / zoom.GetZoomY());

 	x += zoom.GetSourceX();
 	y += zoom.GetSourceY();

    }
	
}



void PdvGlWindow::TransformSetup()

{
    if (TransformActive && pDataImage)
    {
	// allocate image
	if (pDisplayImage)
	{   
	    if (pDisplayImage->SameSize(pDataImage) && pDataImage->GetType() == last_display_type) 
	    {
		return;
	    }

	    pDisplayImage->Destroy();

	    pDisplayImage->Create((Mono2Color || pDataImage->GetNColors() > 1)?TypeRGB:TypeBYTE,
                pDataImage->GetWidth(), pDataImage->GetHeight());

	}
	else
	{
	    pDisplayImage = new EdtImage((Mono2Color || pDataImage->GetNColors() > 1)?TypeRGB:TypeBYTE,
                pDataImage->GetWidth(), pDataImage->GetHeight());
	}

	if (!pLut)
	{
	    pLut = new EdtLut();

	}

	pLut->Setup(pDataImage, pDisplayImage);

	last_display_type = pDataImage->GetType();

    }
    else
    {


    }

}

void PdvGlWindow::InitVariables()

{
    pSingle = NULL;
    pDataImage = NULL;
    pDisplayImage = NULL;
    pRawImage = NULL;

    pLut = NULL;

    TransformActive = false;
    Mono2Color = false;

    show_width = 1;
    show_height = 1;

    scaled_width = 1;
    scaled_height = 1;

    cursor_x = cursor_y = 0;
    image_cursor_x = image_cursor_y = 0;

    buffer = NULL;

    scalex = scaley = 1.0; 
    
    zoom_to_window = false;

    cursor_mode = NoOp;

    last_display_type = 0;
    
    needs_redraw = false;
}

void PdvGlWindow::UpdateDisplayImage()

{

    if (TransformActive)
    {
	TransformSetup();


	pLut->Transform(pDataImage,pDisplayImage);
    
    }

    needs_redraw = true;

}

