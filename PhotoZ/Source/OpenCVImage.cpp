#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include "OpenCVImage.h"

/*
Mat fl_LoadImage(char*filename, int iscolor)
{
	Mat cd, *image;
	if ((cd = CvLoadImage(filename, iscolor)) == 0) {
		fl_alert("Image not opened !");
		return NULL;
	}

	if (cd.nChannels == 3) {
		// from showimage
		Mat mat, stub;
		int origin = 0;
		origin = cd->origin;
		mat = cvGetMat(cd, &stub);
		cvConvertImage(mat, cd, (origin != 0 ? CV_CVTIMG_FLIP : 0) + CV_CVTIMG_SWAP_RB);
		// İmaj withStep yanlış hesaplanıyor (with > 600 gibi ) o yüzden resize ederek yapıyoruz !
	}

	if (cd->widthStep != cd->width*cd->nChannels) {
		image = cvCreateImage(cvSize(cd->width, cd->height), IPL_DEPTH_8U, cd->nChannels);
		image->widthStep = image->width*cd->nChannels;
		cvResize(cd, image, CV_INTER_LINEAR);
		cvReleaseImage(&cd);
		return image;
	}
	else
		return cd;
}

bool fl_SaveImage(char*filename, Mat image)
{
	if (image->nChannels == 3) {
		cvConvertImage(image, image, CV_CVTIMG_SWAP_RB);
		if ((cvSaveImage(filename, image)) == 0) {
			fl_alert("Image not saved");
			cvConvertImage(image, image, CV_CVTIMG_SWAP_RB);
			return false;
		}
		cvConvertImage(image, image, CV_CVTIMG_SWAP_RB);
	}
	else if ((cvSaveImage(filename, image)) == 0) {
		fl_alert("Image not saved");
		return false;
	}

	return true;
}
*/
//   Fl_OpenCv


void Fl_OpenCV::draw(int xx, int yy, int ww, int hh)
{
	if (!image) return;
	if (image->cols <= 0 || image->rows <= 0) return;
	fl_push_clip(x(), y(), ww, hh);

	float fx, fy;// , scale;
	fx = (float)image->cols / ww;
	fy = (float)image->rows / hh;


	/*Vincent (removed): needed to keep the image aspect ratio
	if(fx>fy)
	   scale=fx;
	 else
	   scale=fy;
	 if(fit && scale < 1.0){
	   scale=1.0;
	 }*/

	fimage.release();

	fimage = cv::Mat::zeros(cvFloor(image->rows / fy), cvFloor(image->cols / fx), image->type());
	cv::resize(*image, fimage, Size(cvFloor(image->cols / fx), cvFloor(image->rows / fy)), 0, 0, INTER_CUBIC);
	cv::cvtColor(fimage, fimage, COLOR_BGR2RGB);

	fl_draw_image((uchar *)fimage.datastart, xx, yy, fimage.cols, fimage.rows, fimage.channels(), 0);

	fl_pop_clip();
}

Fl_OpenCV::Fl_OpenCV(int X, int Y, int W, int H) :Fl_Widget(X, Y, W, H, 0)
{
	x(X); y(Y); w(W); h(H);
	image = NULL;
	fit = false;
	mouse_cb = NULL;
}

Fl_OpenCV::Fl_OpenCV(int X, int Y) :Fl_Widget(X, Y, 1, 1, 0)
{
	image = NULL;
	fit = false;
	mouse_cb = NULL;
	x(X); y(Y);

}

void Fl_OpenCV::SetImage(Mat *nimage)
{
	if (!nimage || nimage->cols <= 0 || nimage->rows <= 0)
	{
		image = NULL;
	}
	else
	{
		image = nimage;

		if (image && fit == false) {
			w(image->cols);
			h(image->rows);
			resize(x(), y(), w(), h());
		}
	}
	redraw();
}

void Fl_OpenCV::FitWindows()
{
	fit = true;
}

void Fl_OpenCV::SetMouseCallback(MouseCallback on_mouse, void * param)
{
	mouse_cb = on_mouse;

	cb_param = param;
}


//Convert the fltk mouse event into opencv ones (to be able to use open cv callback functions and events)
int fl_event2cv_event(int fl_event)
{
	switch (fl_event)
	{
		//Mouse buttons down events
	case FL_PUSH:
		//Left, middle or right button ?
		switch (Fl::event_button())
		{
		case FL_LEFT_MOUSE:
			return EVENT_LBUTTONDOWN;
		case FL_RIGHT_MOUSE:
			return EVENT_RBUTTONDOWN;
		case FL_MIDDLE_MOUSE:
			return EVENT_MBUTTONDOWN;
		default:
			return -1;
		}
		//Mouse buttons up events
	case FL_RELEASE:
		//Left, middle or right button ?
		switch (Fl::event_button())
		{
		case FL_LEFT_MOUSE:
			return EVENT_LBUTTONUP;
		case FL_RIGHT_MOUSE:
			return EVENT_RBUTTONUP;
		case FL_MIDDLE_MOUSE:
			return EVENT_MBUTTONUP;
		default:
			return -1;
		}

		//Two mouse move events:
	case FL_DRAG:
		return EVENT_MOUSEMOVE;
	case FL_MOVE:
		return EVENT_MOUSEMOVE;

		//Default: nothing
	default:
		return -1;
	}
}


int Fl_OpenCV::handle(int event)
{
	//  Mouse Events
	if (event == FL_PUSH || event == FL_DRAG || event == FL_MOVE || event == FL_RELEASE) {
		if (mouse_cb && image && (Fl::event_x() - x()) < image->cols && (Fl::event_y() - y()) < image->rows)
			//     if(mouse_cb)
			mouse_cb(fl_event2cv_event(event), Fl::event_x() - x(), Fl::event_y() - y(), 0, cb_param);
	}
	return(Fl_Widget::handle(event));
}

Fl_OpenCV::~Fl_OpenCV()
{
	fimage.release();
}



