//=============================================================================
// LiveFeed.cpp
//=============================================================================

#include <fstream>
#include "LiveFeed.h"
#include <string>
#include <iostream>
/*#include <Paintlib/planydec.h>
#include <Paintlib/plwinbmp.h>
#include <Paintlib/pljpegenc.h>
*/

#include <FL/fl.h>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.h>
#include <FL/fl_draw.h>
#include <FL/fl_ask.h>

#include "Color.h"
#include "DapController.h"
#include "Definitions.h"

using namespace std;
//HDAP dap820Put;
LiveFeed::LiveFeed()
{
    cam = nullptr;
    image = nullptr;
    imagesize = -1;
	background=BG_None;
	showTrace=1;
	showRliValue=0;
	showDiodeNum=0;
}

LiveFeed::~LiveFeed() {
	delete cam;
	delete image;
}


//=============================================================================
void continue_livefeed(void *arg)
{
    if (!lf->running())
    {
        //aw->redraw();
        //Fl::flush();
        return;
    }
	
    lf->update_image();
    aw->redraw();
    Fl::flush();
    Fl::repeat_timeout(1.0/20.0, continue_livefeed);
}

//=============================================================================
void LiveFeed::update_image()
{
	for (int i = 0; i < 4; i++) {
		unsigned char* im = cam->single_image(0);
		memcpy(image + imagesize * i / 4, im, sizeof(short)*imagesize/4);
	}
}

//=============================================================================
bool LiveFeed::running()
{
    return cam != nullptr;
}

//=============================================================================
void LiveFeed::drawBackground()
{
    int high = 0;
    int low = 65535;
    for (int i = 0; i < imagesize; i++)
    {
        high = max(high, image[i]);
        low = min(low, image[i]);
    }
    high -= low;
    if (high <= 0)
        high = 1;

    // width and height for each diode
    int dw = (x2-x1) / cam->width();
    int dh = (y2-y1) / cam->height();
    for (int i = 0; i < imagesize; i++)
    {
        int x = x1 + (i % cam->width()) * dw;
        int y = y1 + (i / cam->width()) * dh;
        colorControl->setGrayScale(((double) image[i] - low) / high);
        fl_push_matrix();
        {
            fl_translate(x,y);
            {
                fl_begin_polygon();
                    fl_vertex(0,0);
                    fl_vertex(dw,0);
                    fl_vertex(dw,dh);
                    fl_vertex(0,dh);
                fl_end_polygon();
            }
        }
        fl_pop_matrix();
    }
}

//=============================================================================
bool LiveFeed::begin_livefeed()
{
	/*
	dap820Put = DapHandleOpen("\\\\.\\Dap0\\$SysIn", DAPOPEN_WRITE);
	if (!dap820Put) {
		char buf[64];
		DapLastErrorTextGet(buf, 64);
		printf("ERROR: setDAPs could not open dap820Put: %s\n", buf);
	}

	DapLinePut(dap820Put, "RESET");
	char fileName1[64] = "\\PhotoZ\\";

	strcat_s(fileName1, 64, "LiveFeed.dap");

	if (!DapConfig(dap820Put, fileName1)) {
		char buf[64];
		DapLastErrorTextGet(buf, 64);
		printf("DAP ERROR: %s\n", buf);
	}*/
	Sleep(100);		// wait .5 second for dap file to reach dap	 ?? needed

	//DapLinePut(dap820Put, "START Send_Pipe_Output, Start_Output");
    cam = new Camera();
    if (cam->open_channel(0))
    {
        cam = nullptr;
        fl_alert("The camera is already in use!\n");
        return false;
    }
    cam->program(dc->getCameraProgram());
    imagesize = cam->width() * cam->height();
    image = new short[imagesize];
    background = aw->getBackground();
    showTrace = aw->getShowTrace();
    showRliValue = aw->getShowRliValue();
    showDiodeNum = aw->getShowDiodeNum();
    aw->setBackground(BG_Live_Feed);
    aw->setShowTrace(0);
    aw->setShowRliValue(0);
    aw->setShowDiodeNum(0);
    //cam->serial_write("@SEQ 1\r");

    // layout where the display will go. taken from ArrayWindow::resizeDiodes
	const int num = 7;
	const int den = 8;

	/* digital output placing */
	const int do_width = 30;
	const int do_height = 30;
	const int do_yoffset = 0;

	int width = cam->width();
	int height = cam->height();

	int diode_width = min(aw->w() * num / (den * width), aw->h() * num / (den * height));
	int diode_height = diode_width;

	// Center the array
	x1 = (aw->w() - diode_width*width) / 2;
	y1 = (aw->h() - diode_height*height) / 2;
	if (y1 + height * diode_height > aw->h())
		y1 = do_yoffset + do_height;
    x2 = x1 + diode_width*width;
    y2 = y1 + diode_height*height;

    Fl::add_timeout(1.0/20.0, continue_livefeed);
    return true;
}

//=============================================================================
void LiveFeed::stop_livefeed()
{
	//DapConfig(dap820Put, "\\PhotoZ\\Stop v5.dap");
	//DapLinePut(dap820Put, "reset");
	//DapHandleClose(dap820Put);
	delete cam;
	cam = nullptr;
	delete[] image;
	image = nullptr;
	
	aw->setBackground(background);
	aw->setShowTrace(showTrace);
	aw->setShowRliValue(showRliValue);
	aw->setShowDiodeNum(showDiodeNum);

	imagesize = -1;
	background = BG_None;
	showTrace = 1;
	showRliValue = 0;
	showDiodeNum = 0;
}

void LiveFeed::getImage(string& filename)
{
	if (image == nullptr) {
		fl_alert("Begin live feed first");
		return;
	}/*
	unsigned char* im = cam->single_image();
	cout << "image is in im";
	// Create a decoder.
		PLAnyPicDecoder Decoder;

	// Create a bitmap object.
		try {
			PLWinBmp Bmp;
			PLJPEGEncoder Encoder;
			cout << "Bmp and encoder created";
			Decoder.MakeBmpFromMemory(im, imagesize, &Bmp);
			cout << "decoder has made bmp from im";
			Encoder.MakeFileFromBmp("Check.bmp", &Bmp);
			cout << "encoder has made file from bmp";
		}
		catch (PLTextException e) {
			cerr << "Error " << e.GetCode()<<": "<<e<<endl;
		}*/
	/*size_t imgdata_size = imagesize;
	std::fstream imgout("MyImage.bin", ios::out | ios::binary);

	imgout.write(reinterpret_cast<char*>(image), imgdata_size);
	imgout.close();

	if (image == nullptr) {
		fl_alert("Begin live feed first");
		return;
	}
	int height = cam->height();
	int width = cam->width();
	ofstream f(filename.c_str(), ios::binary);
	if (!f) return;

	// Put the width and height as 16-bit entities, stored in little-endian format (LSB first)
	f.put(width & 0xFF);
	f.put((width >> 8) & 0xFF);

	f.put(height & 0xFF);
	f.put((height >> 8) & 0xFF);

	// Now put the individual byte values of the array in row-major order
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			f.put(image[(y * width) + x] & 0xFF);
	}
	f.close();*/
}