
#include "edtdefines.h"

#include "CameraConfigEditor.h"

#include "CameraConfigUI.h"


#define MAX_PDV_DEVICES 8

static int pdv_devices[MAX_PDV_DEVICES]; 

static int ndevices = 0;

static int FileWindowWidth = 600;
static int FileWindowHeight = 700;

#include <FL/Fl_File_Chooser.H>

void
get_pdv_devices()

{
    int i;

    for (i=0;i<MAX_PDV_DEVICES;i++)
    {
	PdvDev *pdv_p = pdv_open((char *) "pdv",i);
	if (pdv_p)
	{
	    pdv_devices[ndevices] = pdv_p->devid;
	    ndevices++;

	    pdv_close(pdv_p);
	}
	else
	    break;

    }    

}

static CameraConfigEditor *editor = NULL;

int fl_get_full_path(Fl_Widget *w, char *full_path)

{
    int start=0;

    if (w->parent())
    {
	start=fl_get_full_path(w->parent(),full_path);
	if (start && w->label())
	{
	    full_path[start] = '.';
	    start++;
	}
    }

    if (w->label())
    {
	strcpy(full_path + start, w->label());
	return start + strlen(w->label());
    }

    return start;

}

static int help_key_handler(int newevent)

{
    Fl_Widget *w = Fl::belowmouse();

    if (newevent == FL_SHORTCUT)
    {

	int key = Fl::event_key();

	if (key == FL_F + 1)
	{
	    if (w)
	    {
		const char *label = w->label();

		char full_path[1024];

		full_path[0]=0;

		fl_get_full_path(w, full_path);
		
		if (w->window() == editor->pUI->CameraConfigWindow)
		{
		    //Fl_Text_Buffer *buf = editor->pUI->HelpWindow->buffer();

		    //buf->replace(0,buf->length(), full_path);

		    return 1;
		}
	    }
	    
	}
    }

    return 0;
}

CameraConfigEditor::CameraConfigEditor(void *new_owner, PdvInput *camera, 
				       CamSelector_cb cb, const char *path, 
				       const char *name)

{
    owner = new_owner;
    owner_cb = cb;
    pCamera = camera;
    roi_enabled = false;

    start_dd_p = NULL;

    pFileWindow = NULL;
    save_chooser = NULL;

    pCfg = new CameraConfig(path, name);

    pUI = new CameraConfigUI(this, pCfg, path);

    framebox_x_margin = pUI->GeometryFrame->w() - pUI->FrameBox->w();
    framebox_y_margin = pUI->GeometryFrame->h() - pUI->FrameBox->h();

    has_sim = false;
    has_pdv = false;

    if (!ndevices)
	get_pdv_devices();

    int i;

    if (pCamera )
    {
	has_pdv = true;
	pdv_unit = pCamera->GetUnitNumber();
	if (pCamera->GetDevice())
	    pdv_channel = pCamera->GetDevice()->channel_no;
    }
    else
    {
	pdv_unit = 0;
	pdv_channel = 0;
    }

    for (i=0;i<MAX_PDV_DEVICES;i++)
    {
	if (i < ndevices)
	{
	    if (pdv_devices[i] == PDVCL2_ID)
	    {
		if (!has_sim)
		    sim_unit = i;

		has_sim = true;
		pUI->SimUnitChoice->mode(i,0);
		pUI->PdvUnitChoice->mode(i,FL_MENU_INVISIBLE);
	    }
	    else
	    {
		if (!has_pdv)
		    pdv_unit = i;
		has_pdv = true;
		pUI->SimUnitChoice->mode(i,FL_MENU_INVISIBLE);
		pUI->PdvUnitChoice->mode(i,0);
	    }
	}
	else
	{
	    pUI->SimUnitChoice->mode(i,FL_MENU_INVISIBLE);
	    pUI->PdvUnitChoice->mode(i,FL_MENU_INVISIBLE);
	}

    }

    update_display();
 
    if (!has_sim)
    {
	pUI->SimUnitChoice->hide();
    }

    if (!has_pdv)
    {
	pUI->PdvChannelChoice->hide();
	pUI->PdvUnitChoice->hide();
	pUI->SampleFullButton->hide();
	pUI->SampleXYButton->hide();
    }

    update_check();


    pUI->show();

    //Fl::add_handler(help_key_handler);

    editor = this;

    Fl::focus(pUI->CameraConfigWindow);

}

CameraConfigEditor::~CameraConfigEditor()

{
        delete pUI;
        if (own_config)
            delete pCfg;
}


void CameraConfigEditor::update_check()

{
    if (!start_dd_p)
	start_dd_p = (PdvDependent *) malloc(sizeof(PdvDependent));;

    if (pCfg->dd_p)
	memcpy(start_dd_p, pCfg->dd_p, sizeof(PdvDependent));


}

void CameraConfigEditor::update_config_file()

{
    // bring the config file values in line with the 
    // dependent

    // Look at differences between start and current dd_p



}

bool CameraConfigEditor::is_modified()

{
    if (pCfg && pCfg->dd_p && start_dd_p)
    {
	return (memcmp(pCfg->dd_p, start_dd_p, sizeof(PdvDependent)) != 0);

    }

    return false;

}

void CameraConfigEditor::check_modified()

{
    if (is_modified())
	pUI->ModifiedTag->show();
    else
	pUI->ModifiedTag->hide();

}


void CameraConfigEditor::update_display()

{
    // go through UI updating values based on cfg

    if (pCfg && pUI)
    {
	char *s;

	if (!pCfg->loaded)
	{
	    pCfg->LoadFull();
	    if (!pCfg->loaded)
		return;
	}

	pUI->cfgname->value(pCfg->filename);

	pUI->camera_class->value(pCfg->camera_class);
	pUI->camera_model->value(pCfg->camera_model);
	pUI->camera_info->value(pCfg->camera_info);

	pUI->width->value(pCfg->dd_p->width);
	pUI->widthslider->value(pCfg->dd_p->width);
	pUI->height->value(pCfg->dd_p->height);
	pUI->heightslider->value(pCfg->dd_p->height);


	update_roi_values(pCfg->dd_p->hactv || pCfg->dd_p->vactv || pCfg->dd_p->hskip || pCfg->dd_p->vskip);

	pUI->camera_link->value(pCfg->camera_link);

	if (pCfg->camera_link)
	{

	    int taps = (pCfg->dd_p->cl_data_path >> 4) + 1;

	    set_taps(taps);

	    char cfgs[10];
	    sprintf(cfgs,"%02x", pCfg->dd_p->cl_data_path);

	    pUI->cl_data_path_norm->value(cfgs);

	    set_cl_flags(pCfg->dd_p->cl_cfg);

	    sprintf(cfgs,"%02x", pCfg->dd_p->cl_cfg);

	    pUI->cl_cfg_norm->value(cfgs);

	}
	else
	{
	    set_taps(1);



	}

	pUI->SerialInitInput->value(pCfg->dd_p->serial_init);

	pUI->bitfile->value(pCfg->dd_p->rbtfile);

	set_depth(pCfg->dd_p->depth);

	update_geometry();

	pUI->PdvChannelChoice->value(pdv_channel);
	pUI->PdvUnitChoice->value(pdv_channel);
	pUI->SimUnitChoice->value(sim_unit);


    }

}

void CameraConfigEditor::update_geometry()

{
    // set labels
    char num_s[80];
    PdvDependent *dd_p = pCfg->dd_p;


    int width = dd_p->width;
    int height = dd_p->height;
    int fullwidth = width + dd_p->cls.hblank;
    int fullheight = height + dd_p->cls.vblank;

    int framebox_width = pUI->GeometryFrame->w() - framebox_x_margin;
    int framebox_height = pUI->GeometryFrame->h() - framebox_y_margin;

    bool reduce_height = fullwidth>fullheight;

    double hproportion, vproportion;

    double aspect = (double) fullwidth/(double) fullheight;

    sprintf(num_s, "%d", dd_p->width);
    pUI->WidthLabel->copy_label(num_s);

    sprintf(num_s, "%d", dd_p->height);
    pUI->HeightLabel->copy_label(num_s);

    if (roi_enabled)
    {
	sprintf(num_s,"%d,%d", dd_p->hskip, dd_p->vskip);
	pUI->ROIXYLabel->copy_label(num_s);
	pUI->RoiImageGroup->show();

	sprintf(num_s,"%d", dd_p->hactv);
	pUI->ROIWidthLabel->copy_label(num_s);
	sprintf(num_s,"%d", dd_p->vactv);
	pUI->ROIHeightLabel->copy_label(num_s);

    }
    else
    {
	pUI->RoiImageGroup->hide();

    }

    if (dd_p->cls.hblank)
    {
	pUI->HBlankLabelHolder->show();
	sprintf(num_s,"%d", dd_p->cls.hblank);
	pUI->HBlankLabel->copy_label(num_s);
	hproportion = (double) width / (double) (fullwidth);
	pUI->HBlankBox->show();
    }
    else
    {
	pUI->HBlankBox->hide();
	pUI->HBlankLabelHolder->hide();
	hproportion = 1.0;

    }

    if (dd_p->cls.vblank)
    {
	pUI->VBlankLabelHolder->show();
	sprintf(num_s,"%d", dd_p->cls.vblank);
	pUI->VBlankLabel->copy_label(num_s);
	vproportion = (double) height / (double) (fullheight);
    }
    else
    {
	pUI->VBlankLabelHolder->hide();
	vproportion = 1.0;
    }

    double new_w, new_h, full_w, full_h;

    if (reduce_height)
    {
	full_w = framebox_width;
	new_w = (hproportion * full_w);
	full_h = ((double) full_w / aspect);
	new_h = (double) (full_h) * vproportion;
    }
    else
    {
	full_h = framebox_height;
	new_h = (vproportion * full_h);
	full_w = (full_h * aspect);
	new_w = (double) (full_w) * hproportion;
    }

    pUI->GeometryFrame->damage(FL_DAMAGE_ALL);

    pUI->FrameBox->size((int) full_w,(int) full_h);
    pUI->FrameBox->redraw();
    pUI->CameraBox->size((int) new_w, (int) new_h);
    pUI->CameraBox->redraw();
    

    if (dd_p->cls.hblank)
    {
	pUI->HBlankBox->position(pUI->CameraBox->x() + (int) new_w, 
	    pUI->CameraBox->y());

	pUI->HBlankBox->size((int) (full_w-new_w), (int) new_h);
	pUI->HBlankBox->redraw();
    }

    if (dd_p->hactv )
    {
	double rx, ry, rw, rh;

	rx = ((double) dd_p->hskip / (double) dd_p->width) * new_w + pUI->CameraBox->x();
	ry = ((double) dd_p->vskip / (double) dd_p->height) * new_h + pUI->CameraBox->y();
	rw = ((double) dd_p->hactv / (double) dd_p->width) * new_w;
	rh = ((double) dd_p->vactv / (double) dd_p->height) * new_h;

	pUI->RoiBOX->position((int) rx, (int) ry);
	pUI->RoiBOX->size((int) rw, (int) rh);

	pUI->RoiBOX->redraw();

	pUI->RoiXYLabelHolder->position((int) rx + 5, (int) ry + 20);
	pUI->RoiWidthLabelHolder->position((int) (rx) - 30, (int) (ry + rh) - 20);
	pUI->RoiHeightLabelHolder->position((int) (rx + rw) -30, (int) (ry) + 10);

    }

    // update all displayed values

    pUI->width->value(pCfg->dd_p->width);
    pUI->widthslider->value(pCfg->dd_p->width);
    pUI->height->value(pCfg->dd_p->height);
    pUI->heightslider->value(pCfg->dd_p->height);

    pUI->hskip->value(pCfg->dd_p->hskip);
    pUI->hskipslider->value(pCfg->dd_p->hskip);
    pUI->hactv->value(pCfg->dd_p->hactv);
    pUI->hactvslider->value(pCfg->dd_p->hactv);

    pUI->vskip->value(pCfg->dd_p->vskip);
    pUI->vskipslider->value(pCfg->dd_p->vskip);
    pUI->vactv->value(pCfg->dd_p->vactv);
    pUI->vactvslider->value(pCfg->dd_p->vactv);

    pUI->hblank->value(dd_p->cls.hblank);
    pUI->hblankslider->value(dd_p->cls.hblank);

    pUI->vblank->value(dd_p->cls.vblank);
    pUI->vblankslider->value(dd_p->cls.vblank);

    check_modified();

}

void CameraConfigEditor::set_cl_flags(int flags)

{
    pUI->cl_flag_RGB->value((flags & 1) != 0);
    pUI->cl_flag_IgnoreDV->value((flags & 2) != 0);
    pUI->cl_flag_LineScan->value((flags & 4) != 0);
    pUI->cl_flag_DisableROI->value((flags & 8) != 0);
    pUI->cl_flag_InvertDV->value((flags & 32) != 0);

}

void CameraConfigEditor::set_taps(int taps)

{
    pUI->taps_counter->value(taps);
    pUI->taps_counter2->value(taps);
    pUI->taps_counter3->value(taps);

    pCfg->dd_p->cl_channels = taps;

    if (taps > 1)
    {
	pUI->TapOrientation->show();
    }
    else
    {
	pUI->TapOrientation->hide();
    }

    set_depth(pCfg->dd_p->depth);
}

void CameraConfigEditor::set_intlv_taps(int taps)

{
    pUI->taps_counter->value(taps);
    pUI->taps_counter2->value(taps);
    pUI->taps_counter3->value(taps);

    pCfg->dd_p->cl_channels = taps;

    if (taps > 1)
    {
	pUI->TapOrientation->show();
    }
    else
    {
	pUI->TapOrientation->hide();
    }


}

void CameraConfigEditor::enable_taps(const bool state)

{
    
    if (state)
    {
	pUI->taps_counter->show();
	pUI->taps_counter2->show();
	pUI->taps_counter3->show();

    }
    else
    {
	pUI->taps_counter->hide();
	pUI->taps_counter2->hide();
	pUI->taps_counter3->hide();
    }
}

void CameraConfigEditor::set_depth(int depth, int extdepth)

{
    pUI->depth_slider->value(depth);
    
    pCfg->dd_p->depth = depth;

    if (extdepth)
    {
	pUI->extdepth_slider->value(extdepth);
	pCfg->dd_p->extdepth = extdepth;
    }

    int pathdepth;

    if (pCfg->dd_p->extdepth)
	pathdepth = pCfg->dd_p->extdepth - 1;
    else
	pathdepth = pCfg->dd_p->depth - 1;

    pCfg->dd_p->cl_data_path = 
	    ((pCfg->dd_p->cl_channels - 1) << 4) | pathdepth;

    char s[4];
    sprintf(s, "%02x", pCfg->dd_p->cl_data_path);
    pUI->cl_data_path_norm->value(s);

}

void CameraConfigEditor::show()
{
    if (pUI)
	pUI->show();
}

#define ROI_EL_SELECT_HACTV 0
#define ROI_EL_SELECT_VACTV 1
#define ROI_EL_SELECT_HSKIP 2
#define ROI_EL_SELECT_VSKIP 3
#define ROI_EL_CHECK_ALL 4

void CameraConfigEditor::set_roi_value(int selector, int value, bool enable)

{
    Dependent *dd_p = pCfg->dd_p;

    switch (selector)
    {
    case ROI_EL_SELECT_HACTV:
	if (value <= pCfg->dd_p->width)
	{    
	    pCfg->dd_p->hactv = value;
	    if (pCfg->dd_p->hskip + pCfg->dd_p->hactv > pCfg->dd_p->width)
	    {
		pCfg->dd_p->hskip = pCfg->dd_p->width - pCfg->dd_p->hactv;
	    }
	}
	else
	{
	    pCfg->dd_p->hactv = pCfg->dd_p->width;
	    pCfg->dd_p->hskip = 0;

	}

	break;

    case ROI_EL_SELECT_VACTV:
	if (value <= pCfg->dd_p->height)
	{    
	    pCfg->dd_p->vactv = value;
	    if (pCfg->dd_p->vskip + pCfg->dd_p->vactv > pCfg->dd_p->height)
	    {
		pCfg->dd_p->vskip = pCfg->dd_p->height - pCfg->dd_p->vactv;
	    }
	}
	else
	{
	    pCfg->dd_p->vactv = pCfg->dd_p->height;
	    pCfg->dd_p->vskip = 0;

	}
	break;

    case ROI_EL_SELECT_HSKIP:
	if (value < pCfg->dd_p->width)
	{    
	    pCfg->dd_p->hskip = value;
	    if (pCfg->dd_p->hskip + pCfg->dd_p->hactv > pCfg->dd_p->width)
	    {
		pCfg->dd_p->hactv = pCfg->dd_p->width - pCfg->dd_p->hskip;
	    }
	}
	else
	{
	    pCfg->dd_p->hactv = pCfg->dd_p->width;
	    pCfg->dd_p->hskip = 0;

	}

	break;

    case ROI_EL_SELECT_VSKIP:
	if (value < pCfg->dd_p->height)
	{    
	    pCfg->dd_p->vskip = value;
	    if (pCfg->dd_p->vactv + pCfg->dd_p->vskip > pCfg->dd_p->height)
	    {
		pCfg->dd_p->vactv = pCfg->dd_p->height - pCfg->dd_p->vskip;
	    }
	}
	else
	{
	    pCfg->dd_p->vactv = pCfg->dd_p->height;
	    pCfg->dd_p->vskip = 0;

	}
	break;

	// make sure ROI doesn't exceed size
    case ROI_EL_CHECK_ALL:
	if (dd_p->vskip >= dd_p->height)
	{
	    dd_p->vskip = dd_p->height-1;
	    dd_p->vactv = 1;
	}
	else if (dd_p->vskip + dd_p->vactv > dd_p->height)
	{
	    dd_p->vactv = dd_p->height - dd_p->vskip;
	}
	if (dd_p->hskip >= dd_p->width)
	{
	    dd_p->hskip = dd_p->width-1;
	    dd_p->hactv = 1;
	}
	else if (dd_p->hskip + dd_p->hactv > dd_p->width)
	{
	    dd_p->hactv = dd_p->width - dd_p->hskip;
	}


    }

    update_roi_values(enable);

    update_geometry();
}

void CameraConfigEditor::update_roi_values(const bool enable)

{

    roi_enabled = enable;
    if (enable)
    {

	if (pCfg->dd_p->hskip > pCfg->dd_p->width)
	    pCfg->dd_p->hskip = 0;

	if (pCfg->dd_p->vskip > pCfg->dd_p->height)
	    pCfg->dd_p->vskip = 0;

	if (pCfg->dd_p->hactv == 0)
	{
	    pCfg->dd_p->hactv = pCfg->dd_p->width - pCfg->dd_p->hskip;
	}
	if (pCfg->dd_p->vactv == 0)
	{
	    pCfg->dd_p->vactv = pCfg->dd_p->height - pCfg->dd_p->hskip;
	}

    }
    else
    {
	pCfg->dd_p->hskip = 0;
	pCfg->dd_p->vskip = 0;
	pCfg->dd_p->hactv = 0;
	pCfg->dd_p->vactv = 0;
    }

    pUI->hskip->value(pCfg->dd_p->hskip);
    pUI->hskipslider->value(pCfg->dd_p->hskip);
    pUI->hactv->value(pCfg->dd_p->hactv);
    pUI->hactvslider->value(pCfg->dd_p->hactv);

    pUI->vskip->value(pCfg->dd_p->vskip);
    pUI->vskipslider->value(pCfg->dd_p->vskip);
    pUI->vactv->value(pCfg->dd_p->vactv);
    pUI->vactvslider->value(pCfg->dd_p->vactv);
    
    pUI->Roi_Enable->value(enable);
  

}



#define dd_offset(dd_p, element) ((u_char *) &dd_p->element - (u_char *) dd_p)

#define check_and_change_int(pcfg, name, dd_p, start_dd_p, field) \
    do { \
	if (dd_p->field != start_dd_p->field) \
	    pcfg->cfg.set_int(name,dd_p->field) \
    } while (0)

void CameraConfigEditor::check_int_value(const char *name, 
					 const int offset)

{
    int *test1 = (int *) ((u_char *)(pCfg->dd_p) + offset);
    int *test2 = (int *) ((u_char *)(start_dd_p) + offset);

    if (*test1 != *test2)
	cfg_set_set_int(pCfg->cfg, name, *test1);

}

void CameraConfigEditor::update_config()

{
    PdvDependent *dd_p = pCfg->dd_p;

// go through modifications to config file
    cfg_set_set_string(pCfg->cfg, "camera_class",pUI->camera_class->value());
    cfg_set_set_string(pCfg->cfg, "camera_model",pUI->camera_model->value());
    cfg_set_set_string(pCfg->cfg, "camera_info",pUI->camera_info->value());

    cfg_set_set_string(pCfg->cfg, "serial_init", pUI->SerialInitInput->value());

    check_int_value("width",dd_offset(dd_p, width));
    check_int_value("height", dd_offset(dd_p,height));
    check_int_value("depth", dd_offset(dd_p,depth));
    check_int_value("extdepth", dd_offset(dd_p,extdepth));
    check_int_value("CL_DATA_PATH_NORM", dd_offset(dd_p,cl_data_path));
    check_int_value("CL_CFG_NORM", dd_offset(dd_p,cl_cfg));


    check_int_value("hactv",dd_offset(dd_p,hactv));
    check_int_value("vactv",dd_offset(dd_p,vactv));
    check_int_value("hskip",dd_offset(dd_p,hskip));
    check_int_value("vskip",dd_offset(dd_p,vskip));



}

// callbacks from UI

void config_exit_cb(class Fl_Button *w,void *p)

{
    
}

#include "pdv_dep_config.h"

void file_save_cb(Fl_File_Chooser *w, void *p)

{
    CameraConfigEditor *pE = (CameraConfigEditor *) p;

    printf("Chose %s\n", w->value());

    // make sure we have an updated config
    

    pdv_cfg_mark_nondefault(pE->pCfg->dd_p, pE->pCfg->cfg);

    pdv_cfg_write(pE->pCfg->cfg,w->value(), CFG_STATUS_DEFAULT);


}


void cfg_edit_save_cb(class Fl_Button *w,void *p)

{
    // open a file browser
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;
   
    char fullname[FL_PATH_MAX];

 
    sprintf(fullname,"%s/%s", pE->pCfg->path, pUI->cfgname->value());

    if (!pE->save_chooser)
	pE->save_chooser = new Fl_File_Chooser(fullname,"*.cfg",Fl_File_Chooser::CREATE,"Save Configuration File");
    else
	pE->save_chooser->value(fullname);

    pE->save_chooser->show();

    pE->save_chooser->callback(file_save_cb, pE);

 

}

void cfg_edit_load_cb(class Fl_Button *w,void *p)

{

}

void config_file_in_cb(class Fl_File_Input *w,void *p)
{


}

void cfg_edit_baud_cb(class Fl_Round_Button *w, void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;

    // figure out which one was set

}

// callback for values in the CL_CFG_NORM
void cfg_edit_cl_flag_cb(class Fl_Check_Button *w, void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int newcfg =  pUI->cl_flag_RGB->value() |
		  (pUI->cl_flag_IgnoreDV->value() << 1) |
		  (pUI->cl_flag_LineScan->value() << 2) | 
		  (pUI->cl_flag_DisableROI->value() << 3) |
		  (pUI->cl_flag_InvertDV->value() << 5);

    pE->pCfg->dd_p->cl_cfg = newcfg;

    char s[32];
    sprintf(s, "%02x", newcfg);

    pUI->cl_cfg_norm->value(s);


    if (pUI->cl_flag_RGB->value())
    {
	pE->set_taps(1);
    }
    pE->enable_taps(!pUI->cl_flag_RGB->value());

    pE->check_modified();
}

// callback for various simulator flags

void cfg_edit_cls_flags_cb(class Fl_Check_Button *w, void *p)

{

}

/////////////////////////////////////////////
// callbacks for geometry inputs/sliders
/////////////////////////////////////////////

void cfg_edit_hblank_cb(class Fl_Value_Input *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_h = (int) w->value();

    pE->pCfg->dd_p->cls.hblank = new_h;
    pUI->hblankslider->value(new_h);

    pE->update_geometry();
}


void cfg_edit_hblank_slider_cb(class Fl_Slider *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_h = (int) w->value();

    pE->pCfg->dd_p->cls.hblank = new_h;
    pUI->hblank->value(new_h);

    pE->update_geometry();
}

void cfg_edit_vblank_cb(class Fl_Value_Input *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_h = (int) w->value();

    pE->pCfg->dd_p->cls.vblank = new_h;
    pUI->vblankslider->value(new_h);

    pE->update_geometry();
}

void cfg_edit_vblank_slider_cb(class Fl_Slider *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_h = (int) w->value();

    pE->pCfg->dd_p->cls.vblank = new_h;
    pUI->vblank->value(new_h);

    pE->update_geometry();
}


void cfg_edit_height_slider_cb(class Fl_Slider *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_h = (int) w->value();

    pE->pCfg->dd_p->height = new_h;
    pUI->height->value(new_h);

    
    pE->set_roi_value(ROI_EL_CHECK_ALL, new_h, pE->roi_enabled);

}

void cfg_edit_height_cb(class Fl_Value_Input *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_h = (int) w->value();

    pE->pCfg->dd_p->height = new_h;
    pUI->heightslider->value(new_h);


    
    pE->set_roi_value(ROI_EL_CHECK_ALL, new_h, pE->roi_enabled);

}

void cfg_edit_width_slider_cb(class Fl_Slider *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->width = new_w;
    pUI->width->value(new_w);
   
    pE->set_roi_value(ROI_EL_CHECK_ALL, new_w, pE->roi_enabled);
}

void cfg_edit_width_cb(class Fl_Value_Input *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->width = new_w;
    pUI->widthslider->value(new_w);
    
    pE->set_roi_value(ROI_EL_CHECK_ALL, new_w, pE->roi_enabled);
}

void cfg_edit_vactv_slider_cb(class Fl_Slider *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->vactv = new_w;

    pE->set_roi_value(ROI_EL_SELECT_VACTV, new_w);

}

void cfg_edit_vactv_cb(class Fl_Value_Input *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->vactv = new_w;

    pE->set_roi_value(ROI_EL_SELECT_VACTV, new_w);


}

void cfg_edit_hactv_slider_cb(class Fl_Slider *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->hactv = new_w;
 
    pE->set_roi_value(ROI_EL_SELECT_HACTV, new_w);

}
void cfg_edit_hactv_cb(class Fl_Value_Input *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->hactv = new_w;
    pE->set_roi_value(ROI_EL_SELECT_HACTV, new_w);
}

void cfg_edit_vskip_slider_cb(class Fl_Slider *w,void *p)
{ 
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->vskip = new_w;
    pE->set_roi_value(ROI_EL_SELECT_VSKIP, new_w);
}


void cfg_edit_vskip_cb(class Fl_Value_Input *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->vskip = new_w;
    pE->set_roi_value(ROI_EL_SELECT_VSKIP, new_w);
}

void cfg_edit_hskip_slider_cb(class Fl_Slider *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->hskip = new_w;
    pE->set_roi_value(ROI_EL_SELECT_HSKIP, new_w);
}

void cfg_edit_hskip_cb(class Fl_Value_Input *w,void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    int new_w = (int) w->value();

    pE->pCfg->dd_p->hskip = new_w;
    pE->set_roi_value(ROI_EL_SELECT_HSKIP, new_w);
}




void cfg_edit_depth_slider_cb(class Fl_Value_Slider *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    pE->set_depth((int) w->value());
}

void cfg_edit_extdepth_slider_cb(class Fl_Value_Slider *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    pE->set_depth(pE->pCfg->dd_p->depth, (int) w->value());
}

void cfg_edit_taps_counter_cb(class Fl_Counter *w, void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    pE->set_taps((int) w->value());
   
}

void cfg_edit_intlv_taps_counter_cb(class Fl_Counter *w, void *p)
{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    pE->set_intlv_taps((int) w->value());
   
}

void cfg_edit_roi_enable_cb(class Fl_Check_Button *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    pE->update_roi_values(w->value());

    pE->update_geometry();

}

extern "C" {

#include "cl_logic_lib.h"

}

void cfg_edit_full_sample_timing(class Fl_Button *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;


    if (pE->pCamera)
    {
	ClLogicSummary *clsp;

	clsp = new ClLogicSummary;


	cl_logic_summary_init(clsp, 0, 0, 0, 0, 0);

	pdv_cl_logic_sample(pE->pCamera->GetDevice(),
			    NULL,
			    clsp,
			    0,
			    0,
			    1,
			    NULL,
			    10,
			    2000,
			    5);

	pE->pCfg->dd_p->width = clsp->width.mean;
	pE->pCfg->dd_p->height = clsp->height.mean;
	pE->pCfg->dd_p->cls.vblank = clsp->frame_gap.mean /
		clsp->totallineclocks.mean;
	pE->pCfg->dd_p->cls.hblank = clsp->hblank.mean;


	pUI->pixel_clock_input->value(clsp->pixel_clock);


	pE->set_roi_value(ROI_EL_CHECK_ALL, 
	    0, 
	    pE->roi_enabled);

	delete clsp;
    }

}

void cfg_edit_sample_timing(class Fl_Button *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;


    if (pE->pCamera->GetDevice())
    {

	pE->pCfg->dd_p->height = edt_reg_read(pE->pCamera->GetDevice(), PDV_CL_LINESPERFRAME);
	pE->pCfg->dd_p->width = edt_reg_read(pE->pCamera->GetDevice(), PDV_CL_PIXELSPERLINE);

	pE->set_roi_value(ROI_EL_CHECK_ALL, 
	    0, 
	    pE->roi_enabled);

    }

}


void cfg_edit_sim_unit_cb(class Fl_Choice *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;
}

void cfg_edit_pdv_unit_cb(class Fl_Choice *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;
}

void cfg_edit_pdv_channel_cb(class Fl_Choice *w, void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE; 
}

void cfg_edit_apply_cls_cb(class Fl_Button *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE; 

}

void cfg_edit_apply_fg_cb(class Fl_Button *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE; 

}

void cfg_edit_intlv_bayer_cb(class Fl_Button *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE; 

    if (w->value())
	pUI->InterleaveTabs->value(pUI->BayerFilterOptions);

}

void cfg_edit_show_cfg_cb(class Fl_Button *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE; 

    if (!pE->pFileWindow)
    {
	pE->pFileWindow = new CfgFileTextEditor(FileWindowWidth,FileWindowHeight, 
	    pE->pCfg->fullfilename);
    }

    pE->update_config();

    pE->pFileWindow->SetEditor(pE);
  
    pE->pFileWindow->show();
}

void cfg_edit_intlv_method_cb(class Fl_Button *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE;

    // which widget	

    int index = pUI->InterleaveMethodButtons->find(w);

    if (index < pUI->InterleaveMethodButtons->children())
    {
    	switch (index)
	{
	case 0: // no interleave
	    pUI->InterleaveTabs->value(pUI->NoIntlvOptions);
	    break;

	case 1:
	case 2:
	case 3:
	case 4:

	    pUI->InterleaveTabs->value(pUI->GenericIntlvOptions);
	    break;

	case 5:
	case 6:
	case 7:

	    pUI->InterleaveTabs->value(pUI->LineIntlvOptions);
	    break;

	}
    }
}

void cfg_edit_tap_orientation_cb(class Fl_Button *w,void *p)

{
    CameraConfigUI *pUI = (CameraConfigUI *) p;
    CameraConfigEditor *pE = pUI->pE; 
    PdvDependent *dd_p = pE->pCfg->dd_p;


    int index = pUI->TapOrientation->find(w);

    switch(index)
    {
    case 0:
	dd_p->htaps = dd_p->cl_channels;
	dd_p->vtaps = 1;
	break;

    case 1:
	dd_p->vtaps = dd_p->cl_channels;

	dd_p->htaps = 1;
	break;

	/* FIX - need to be able to specify each  of htapa nd vtap */
    case 2:

	dd_p->htaps = dd_p->cl_channels / 2;
	dd_p->vtaps = dd_p->cl_channels / 2;
	break;
    }

    char stat[64];

    sprintf(stat,"Setting htaps = %d vtaps = %d\n", dd_p->htaps, dd_p->vtaps);

    pUI->StatusOutput->value(stat); 
}
