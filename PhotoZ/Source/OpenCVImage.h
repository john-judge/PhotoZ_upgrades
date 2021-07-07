//=============================================================================
// OpenCVImage.h
//=============================================================================
/*
Fl_OpenCv 0.9
License : LGPL
Fl_OpenCv is a widget which is render OpenCv image within the FLTK window
Programmed by BlueKid
http://derindelimavi.blogspot.com/
Sent me any suggestion, modification or bugs. Don't hesitate to contact
me for any question, I will be very grateful with your feedbacks.
bluekid70@gmail.com
*/

#ifndef Fl_OpenCV_H
#define Fl_OpenCV_H

#include <FL/Fl_Widget.H>
#include <FL/fl_ask.H>
#include <opencv.hpp>
using namespace cv;


Mat fl_LoadImage(char*filename, int iscolor);
bool fl_SaveImage(char*filename, Mat image);

//Convert the fltk mouse event into opencv ones (to be able to use open cv callback functions and events)
int fl_event2cv_event(int fl_event);



class FL_EXPORT Fl_OpenCV : public Fl_Widget
{
	int       _x, _y, _w, _h;

protected:
	Mat fimage;
	int iscolor;
	MouseCallback mouse_cb;
	void *cb_param; //a parameter for the callback function
	bool fit;
	void draw(int, int, int, int);
	void draw() {
		draw(Fl_Widget::x(), Fl_Widget::y(), w(), h());
	}
	void resize(int x, int y, int ww, int hh) {
		w(ww);
		h(hh);
		Fl_Widget::resize(x, y, ww, hh);
	}

	void x(int X) { _x = X; };
	void y(int Y) { _y = Y; };
	void w(int W) { _w = W; };
	void h(int H) { _h = H; };

public:
	void SetMouseCallback(MouseCallback on_mouse, void * param = 0);
	int handle(int event);
	//IplImage *image;
	Mat *image;
	int x() { return _x; };
	int y() { return _y; };
	int w() { return _w; };
	int h() { return _h; };
	//void SetImage(IplImage *nimage);
	void SetImage(Mat *nimage);
	void FitWindows();
	Fl_OpenCV(int X, int Y, int W, int H);
	Fl_OpenCV(int X, int Y);
	~Fl_OpenCV();

};

#endif

