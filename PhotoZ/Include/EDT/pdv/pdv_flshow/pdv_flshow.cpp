
#include "pdv_flshowUI.h"

#include "CamSelector.h"

#include "PixelText.h"

#include "Histogrammer.h"

#include "Profiler.h"

#include "DisplayContrast.h"

#include "CameraConfigEditor.h"

#include "RgbControl.h"

#include <stdlib.h>

#include "tiffio.h"

#include <errno.h>

#include <iostream>

pdv_flshowUI *Program;


using std::cerr;

CamSelector *pCameraSelector = NULL;
CameraConfigEditor *pCameraConfigEditor = NULL;

PixelText *pPixelText = NULL;
Histogrammer *pHistogram = NULL;
Profiler *pProfile = NULL;
DisplayContrast *pContrast = NULL;

PdvGlLiveWindow *pLive = NULL;

RgbControl *pRGB = NULL;


const char *config_path = 
#ifdef WIN32
"C:/edt/pdv/camera_config";
#else
"/opt/EDTpdv/camera_config";
#endif

char filename[256];



u_char *Cast_tbl = NULL;



u_char * create_cast_tbl(int depth_bits, int nents)
{
    int i;
    u_char *tbl;
    float mult;
    float fmin=0.0;
    int ncolors = (int)pow((float)2.0, (float)depth_bits);


    mult = 255.0/(float)ncolors;
    if ((tbl = (u_char *)malloc(ncolors * sizeof(char))) == NULL)
    {
	printf("malloc for cast_tbl failed, size %d\n", ncolors);
	exit(0);
    }

    for (i=0; i < ncolors; i++)
    {
	tbl[i] = (int)(fmin + ((mult * (float)i) + 0.5));
    }
    /*
    if (Dbg_file)
    {
    printf("Cast_tbl: depth (bytes) %d nents %d ncolors %d mult %1.2f\n",
    depth_bits, nents, ncolors, mult);
    printf("Cast_tbl %d: %d\n", 0, tbl[0]);
    printf("Cast_tbl %d: %d\n", ncolors/2, tbl[ncolors/2]);
    printf("Cast_tbl %d: %d\n", ncolors-1, tbl[ncolors-1]);
    }
    */
    return tbl;
}    

void free_cast_tbl()
{
    free(Cast_tbl);
    Cast_tbl = NULL;
}


int edt_write_tiffile(char *fname, u_char *data, int width, int height, int depth_bits)

{
    TIFF *tif;
    u_char *p;
    u_short *shortp;
    u_char *scanline;
    long row;
    int i, lines = 0;
    int bytes_out = 1;
    int errno;
    extern char *errno_fname, *errno_action;

    if ((tif = TIFFOpen(fname, "w")) == NULL)
    {

	//strcpy(errno_fname, fname);
	//strcpy(errno_action, "open");
	printf("ERROR");
	return -1;
    }


    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    /* TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW); */
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    if (depth_bits > 16)
    {
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8, 8, 8);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 0x8000/(width*3));
    }
    else
    {
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 0x8000/width);
    }


    /*
    * 24 bit color
    */
    if (depth_bits > 16)
    {
	row = 0;
	p = data;
	scanline = (u_char *)malloc(width * 3);
	while (row != height)
	{
	    /*
	    * invert BGR to RGB
	    */
	    for (i=0; i<width*3; i+=3, p+=3)
	    {
		scanline[i] = *(p+2);
		scanline[i+1] = *(p+1);
		scanline[i+2] = *p;
	    }
	    TIFFWriteScanline(tif, scanline, row, 0);
	    ++row;
	}
	free(scanline);
    }
    /*
    * 10-16 bit mono (no word save in TIFF, map all words to bytes)
    */
    else if (depth_bits > 8)
    {
	if (Cast_tbl == NULL)
	    Cast_tbl = create_cast_tbl(depth_bits, 256);

	row = 0;
	shortp = (u_short *)data;
	scanline = (u_char *)malloc(width);
	while (row != height)
	{
	    for (i=0; i<width; i++)
		scanline[i] = Cast_tbl[*shortp++];
	    TIFFWriteScanline(tif, scanline, row, 0);
	    ++row;
	}
	free(scanline);
    }
    /*
    * straight 8-bit
    */
    else if (depth_bits <= 8)
    {
	row = 0;
	p = data;
	while (row != height)
	{
	    TIFFWriteScanline(tif, p, row, 0);
	    p += width;
	    ++row;
	}
    }
    TIFFClose(tif);
    return 0;

}

//---------- Save Raster File Function ------------

void save_ras_file(char *newrasfile)

{

    PdvInput * in_pCamera = pLive->GetCamera();

    int depth = in_pCamera->GetDepth();
    int width = in_pCamera->GetWidth();
    int height = in_pCamera->GetHeight();

    if (depth == 8)
    {
	int retval;

	unsigned char * MyImage = pdv_wait_image(in_pCamera->GetDevice()); 

	retval = dvu_write_rasfile(newrasfile, MyImage, width, height);
    }

    else if (depth == 24)

    {
	int retval;

	unsigned char * MyImage = pdv_wait_image(in_pCamera->GetDevice());

	retval = dvu_write_rasfile24(newrasfile, MyImage, width, height);

    }

    else

    {
	int retval;

	unsigned char * MyImage = pdv_wait_image(in_pCamera->GetDevice());

	retval = dvu_write_rasfile16(newrasfile, MyImage, width, height, depth);

    }


}

//-------- Save Tiff File Function ---------

void save_tiff_file(char *newtifffile)

{

    PdvInput * in_pCamera = pLive->GetCamera();

    int depth = in_pCamera->GetDepth();
    int width = in_pCamera->GetWidth();
    int height = in_pCamera->GetHeight();

    int depth_bits = in_pCamera->GetDepth();

    int retval;

    unsigned char * MyImage = pdv_wait_image(in_pCamera->GetDevice());

    retval = edt_write_tiffile(newtifffile, MyImage, width, height, depth_bits);

}

//---------- Save Ras Menu Item Callback ---------

void save_ras_cb(Fl_Menu_ *o, void *p)

{
    char *newrasfile;

    newrasfile = fl_file_chooser("Save Raster File As?", "*.ras", filename);

    if (newrasfile != NULL) save_ras_file(newrasfile);
}

//---------- Save Tiff Menu Item Callback ---------

void save_tiff_cb(Fl_Menu_ *o, void *p)

{
    char *newtifffile;

    newtifffile = fl_file_chooser("Save Tiff File As?", "*.tif", filename);

    if (newtifffile != NULL) save_tiff_file(newtifffile);

}

//--------- Open Product Manual Button Callback ----

void open_manual_cb(Fl_Menu_ *o, void *p)

{

}

//--------- View Toolbar Callback ------------------
//As of now only hides the group - still needs to resize the window

void viewtoolbar_cb(Fl_Menu_ *o, void *p)

{
    int status = Program->ViewToolbarMenuItem->value();

    if (status == 4) Program->toolbar->show();

    if (status == 0) Program->toolbar->hide();

    Program->toolbar->redraw();


}

//------- View Status Bar Callback ---------
//As of now only hides the group - still needs to resize the window

void viewstatusbar_cb(Fl_Menu_ *o, void *p)

{

    int status = Program->ViewStatusbarMenuItem->value();

    if (status == 4) Program->statusbar->show();

    if (status == 0) Program->statusbar->hide();

    Program->statusbar->redraw();

}

//--------- Quit Menu Item Callback ---------------

void exit_cb(Fl_Menu_ *o, void *p)

{
    PdvInput * in_pCamera = pLive->GetCamera();
    delete in_pCamera ;
    exit(0);
}
//---------- Start Continuous Capture Mode Menu Callback

void contcapmenu_cb(Fl_Menu_ *o, void *p)

{
    if (pLive->IsLive())
	pLive->StopLive();
    else
	pLive->Start();

}

//---------- Start Continuous Capture Mode Button Callback

void start_capture_cb(Fl_Button *o, void *p)

{
    if (pLive->IsLive())
	pLive->StopLive();
    else
	pLive->Start();

}

//--------- Capture Single Image Menu Callback -------

void snapmenu_cb(Fl_Menu_ *o, void *p)

{
    pLive->capture_single_and_draw();

}

//--------- Capture Single Image Button Callback -------

void snap_image_cb(Fl_Button *o, void *p)

{
    pLive->capture_single_and_draw();

}

void increaseshutter_cb(Fl_Menu_ *o, void *p)

{
    PdvInput * in_pCamera = pLive->GetCamera();
    int shuttertime = pdv_get_exposure(in_pCamera->GetDevice());
    int max = pdv_get_max_shutter(in_pCamera->GetDevice());
    int min = pdv_get_min_shutter(in_pCamera->GetDevice());

    if (shuttertime < min)
    	shuttertime = min;
    else if (++shuttertime > max)
	shuttertime = max;
    pdv_set_exposure(in_pCamera->GetDevice(), shuttertime);
}

void decreaseshutter_cb(Fl_Menu_ *o, void *p)

{
    PdvInput * in_pCamera = pLive->GetCamera();
    int shuttertime = pdv_get_exposure(in_pCamera->GetDevice());
    int max = pdv_get_max_shutter(in_pCamera->GetDevice());
    int min = pdv_get_min_shutter(in_pCamera->GetDevice());

    if (shuttertime > max)
    	shuttertime = max;
    else if (--shuttertime < min)
	shuttertime = min;
    pdv_set_exposure(in_pCamera->GetDevice(), shuttertime);

}


void camera_settings_cb(Fl_Button *o, void *p)

{



}


void camselector_done_cb(void *owner, bool ok, const char *fname, const char *path)

{
    printf("camselector_done_cb %s filename %s path %s\n", (ok)?"ok":"cancel", fname, path);

    PdvInput *pCamera = pLive->GetPdv();

    pCamera->Setup(path, fname);

    pLive->SetPdv(pCamera);
    

}

void camera_selector_cb(Fl_Button *o, void *p)

{
    PdvInput *pCamera = pLive->GetPdv();
    if (!pCameraSelector)
	pCameraSelector = new CamSelector(Program, camselector_done_cb, config_path, pCamera->GetLoadedConfig());

    pCameraSelector->show();

}

//--------- Setup Menu Callback -----------

void setupmenu_cb(Fl_Menu_ *o, void *p)

{
    PdvInput *pCamera = pLive->GetPdv();
    if (!pCameraSelector)
	pCameraSelector = new CamSelector(Program, camselector_done_cb, config_path, pCamera->GetLoadedConfig());

    pCameraSelector->show();

}

void confeditmenu_cb(Fl_Menu_ *o, void *p)

{
    PdvInput *pCamera = pLive->GetPdv();
    if (!pCameraConfigEditor)
	pCameraConfigEditor = new CameraConfigEditor(Program, 
	pCamera, 
	camselector_done_cb, config_path, 
	pCamera->GetLoadedConfig());

    pCameraConfigEditor->show();

}




void camera_config_editor_cb(Fl_Button *o, void *p)

{
    PdvInput *pCamera = pLive->GetPdv();
    if (!pCameraConfigEditor)
	pCameraConfigEditor = new CameraConfigEditor(Program, 
	pCamera, 
	camselector_done_cb, config_path, 
	pCamera->GetLoadedConfig());

    pCameraConfigEditor->show();

}

void pixeltextmenu_cb(Fl_Menu_ *o, void *p)

{
    if (!pPixelText)
	pPixelText = new PixelText(pLive->GetCaptureImage());

    pPixelText->pUI->show();

}

void pixel_text_cb(Fl_Button *o, void *p)

{
    if (!pPixelText)
	pPixelText = new PixelText(pLive->GetCaptureImage());

    pPixelText->pUI->show();

}

void histogrammenu_cb(Fl_Menu_ *o, void *p)

{
    if (!pHistogram)
	pHistogram = new Histogrammer(pLive->GetCaptureImage());

    pHistogram->show();

}

void histogram_cb(Fl_Button *o, void *p)

{
    if (!pHistogram)
	pHistogram = new Histogrammer(pLive->GetCaptureImage());

    pHistogram->show();

}

void profilemenu_cb(Fl_Menu_ *o, void *p)

{
    if (!pProfile)
	pProfile = new Profiler(pLive->GetCaptureImage());

    if (pLive->line.active)
	pProfile->update(pLive->GetCaptureImage(), 
	pLive->line.x1,
	pLive->line.y1,
	pLive->line.x2,
	pLive->line.y1);

    pProfile->show();

}

void profile_cb(Fl_Button *o, void *p)

{
    if (!pProfile)
	pProfile = new Profiler(pLive->GetCaptureImage());

    if (pLive->line.active)
	pProfile->update(pLive->GetCaptureImage(), 
	pLive->line.x1,
	pLive->line.y1,
	pLive->line.x2,
	pLive->line.y1);

    pProfile->show();

}
void reset_cursor_cb(Fl_Button *o, void *p)

{
    printf("Reset Cursor\n");
    pLive->SetCursorMode( PdvGlWindow::NoOp);
}

void zoom_up_cb(Fl_Button *o, void *p)

{
    printf("Zoom Up\n");
    pLive->SetCursorMode( PdvGlWindow::ZoomUp);
}

void zoom_down_cb(Fl_Button *o, void *p)

{
    printf("Zoom Down\n");
    pLive->SetCursorMode( PdvGlWindow::ZoomDown);
}

void contrast_cb(Fl_Button *o, void *p)

{
    if (!pContrast)
	pContrast = new DisplayContrast(pLive);


    pContrast->update(pLive);

    pContrast->show();

}

void rgb_cb(Fl_Button *o, void *p)

{
    if (!pRGB)
	pRGB = new RgbControl(pLive, pLive->GetCamera());

    pRGB->refresh(pLive);

    pRGB->show();

}

void select_box_cb(Fl_Button *o, void *p)
{

    printf("Select Box\n");
    pLive->SetCursorMode(  PdvGlWindow::Box );
}


void select_line_cb(Fl_Button *o, void *p)
{
    printf("Select Line\n");
    pLive->SetCursorMode( PdvGlWindow::Line );
}

void roi_edit_cb(Fl_Button *o, void *p)

{

}

void set_zoom_value_cb(Fl_Menu_ *o, void *p)

{

    float v = ((long) (o->mvalue())->user_data()) * 0.001;


    pLive->zoom.SetZoomXY(v, v);

    Program->Display->UpdateScrollbars();

    pLive->redraw();

    char val[64];

    sprintf(val, "%6.1f",v * 100);

    Program->ZoomValue->copy_label(val);


}


void zoom_to_window_cb(Fl_Menu_ *o, void *p)

{
    pLive->ZoomToWindow(o->mvalue()->value());

    Program->Display->UpdateScrollbars();

}

void update_status(pdv_flshowUI *Program)

{

    char label[80];

    sprintf(label,"%7.2f", pLive->GetFrameRate());

    Program->FrameRate->value(label);

    EdtImage *pImage = pLive->GetCaptureImage();

    if (pImage)
    {
	int Value[4];
	int x, y;

	pLive->GetCursorImageCoords(x, y);

	int n = pImage->GetPixelV(x, y, Value);

	sprintf(label, "%4d,%4d",  x, y);

	Program->Coords->value(label);

	if (n == 1)
	{
	    sprintf(label,"%4d", Value[0]);
	}
	else if (n == 3)
	{
	    sprintf(label,"%3d %3d %3d", Value[0], Value[1], Value[2]);
	}
	else if (n == 4)
	{
	    sprintf(label,"%3d %3d %3d %3d", Value[0], Value[1], Value[2], Value[3]);
	}

	if (pPixelText && pPixelText->IsOpen())
	    pPixelText->update(pImage, x, y);

	if (pHistogram && pHistogram->visible())
	{
	    EdtRect rect;
	    if (pLive->box.active)
		pLive->box.rect_from_box(&rect);
	    else
		rect.width = rect.height = rect.x = rect.y = 0;

	    pHistogram->update(pImage, &rect);
	}

	if (pProfile && pProfile->visible())
	{

	    bounding_box lp = pLive->line;

	    if (pLive->line.active)
		pProfile->update(pLive->GetCaptureImage(), 
		lp.x1,
		lp.y1,
		lp.x2,
		lp.y2);

	}

	if (pContrast && pContrast->visible())
	{

	    if (pLive->pLut && pLive->pLut->IsDataDependent())
		pContrast->update(pLive);

	}

    }
    else
	label[0] = 0;

    Program->PixelValues->value(label);

    char buf[32];

    sprintf(buf, "%6d %%", (int) (pLive->zoom.GetZoomX() * 100)); 
    Program->ZoomValue->copy_label(buf);

    int color = FL_BLACK;

    int t = (int) (edt_timestamp() * 2.0);

    if (pLive->IsLive() && (t & 1))
    {
	color = FL_RED;

    }

    Program->ActiveState->color(color);
    Program->ActiveState->redraw();


}


void idle_checker(void *obj)

{

    static int last_count = 0;
    pdv_flshowUI *Program = (pdv_flshowUI *) obj;

    if (pLive->needs_redraw)
    {
	pLive->redraw();
    }

    update_status(Program);

    Fl::repeat_timeout(0.03,idle_checker, obj);
}

void select_camera(char *filename, PdvInput *pCamera)
{
   	if (pCamera)
	{
	    filename = pCamera->GetLoadedConfig();
	    if (filename)
		filename = strdup(filename);
	    else
		filename = strdup("");
	}
	else
	    filename = strdup(""); 

	
	if (!pCameraSelector)
	    pCameraSelector = new CamSelector(NULL, camselector_done_cb, config_path, filename);

	pCameraSelector->show();	
} 


void usage(const char *progname)

{
	printf("Usage: %s [options]\n", progname);
	printf("Options:\n");
	printf("-u N      Open unit number N; default is 0\n");
	printf("-c N      Open channel number N; default is 0\n");
	printf("-N N      Use N ring buffers (default 4)\n");
	printf("-nbN      Alternate format to specify number of ring buffers\n");
	printf("-pdvN_C   Open unit N and channel C.  Equivalent to -u N -c C\n");
}

int main(int argc, char **argv) 

{
    int unit = 0;
    int channel = 0;
    int numbufs = 0;
    char devname[128] = "";
    int i;
    const char *progname = argv[0];
    int select = 0; //if true cam select only
    char *filename = NULL;

    for (i=1;i<argc;i++)
    {
	if (argv[i][0] == '-')
	    switch (argv[i][1])
	{
	    case 'u':		/* device unit number */
		i++;
		if (i == argc) {
			cerr << "ERROR: -u requires argument\n";
			usage(progname);
			exit(1);
		}
		strncpy(devname, argv[i], sizeof(devname) - 1);
		break;

	    case 'e':	/*just edit configuration*/
		i++;
		if (i == argc) {
			select = 1;
                }
		break;

	    case 'c': /* channel */
		i++;
		if (i == argc) {
			cerr << "ERROR: -c requires argument\n";
			usage(progname);
			exit(1);
		}
		channel = atoi(argv[i]);
		break;

	    case 'N': /* numbufs, take format */
		i++;
		if (i == argc) {
			cerr << "ERROR: -N requires argument\n";
			usage(progname);
			exit(1);
		}
		numbufs = atoi(argv[i]);
		break;

	     case 'n': /* look for -nbN where N is the number of buffers, pdvshow compat */
                if ((strlen(argv[i]) > 3) && (strncmp(argv[i], "-nb", 3) == 0)) {
                        numbufs = atoi(argv[i]+3);
		} else {
			cerr << "Unknown option: " << argv[i] << "\n";
			usage(progname);
			exit(1);
		}
		break;

	    case 'p': /* look for -pdvN_C */
	    	if (strncmp(argv[i], "-pdv", 4) == 0) {
			strncpy(devname, argv[i]+1, sizeof(devname) - 1);
		} else {
			cerr << "Unknown option: " << argv[i] << "\n";
			usage(progname);
			exit(1);
		}
		break;

	    case 'h':
	    case '-':
	    	if (!strcmp(argv[i], "-help") || !strcmp(argv[i], "--help")) {
			usage(progname);
			exit(0);
		}
		break;

	    default:
		usage(progname);
		exit(1);

	}
    }

    if (devname[0])
    {
	unit = edt_parse_unit_channel(devname, devname, EDT_INTERFACE, &channel);
	if (unit == -1) {
		cerr << "ERROR: couldn't parse " << devname << "\n";
		exit(1);
	}
    }
    else
    {
	strcpy(devname, EDT_INTERFACE);
    }

   
    int width;

    PdvInput *pCamera = new PdvInput(devname, unit, channel);

    if (numbufs)
	pCamera->SetNBufs(numbufs) ;

    Program = new pdv_flshowUI(1024,768);

    pLive = Program->Display->GetLiveWindow();

    pLive->SetPdv(pCamera);

    if (select == 1) 
    {
	select_camera(filename, pCamera);
    }
 
    else
    {
        Program->show();

        if (pCamera->GetWidth() == 0)
        {
            fl_alert("Board not yet initialized -- please select a camera to set configuration");

            select_camera(filename, pCamera);
        }
 
        
        Fl::add_timeout(0.03, idle_checker, Program);
    }

    return Fl::run();
}
