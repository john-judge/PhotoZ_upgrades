

#ifndef CameraConfigEditor_H
#define CameraConfigEditor_H

#include "CameraConfig.h"

class CameraConfigUI;

#include "PdvInput.h"

#include "CamSelector.h"

#include "LogWindowUI.h"

#include "CfgFileTextEditor.h"

#include <FL/Fl_File_Chooser.H>


class CameraConfigEditor {

public:
    
    Fl_File_Chooser *save_chooser;

    CameraConfig *pCfg;
    PdvDependent *start_dd_p;

     CameraConfigEditor(
	 void *owner, 
	 PdvInput *pCamera, 
	 CamSelector_cb cb, 
	 const char *path, 
	 const char *name);
  
    ~CameraConfigEditor();

    CameraConfigUI *pUI;

    CfgFileTextEditor *pFileWindow;   
        
    CamSelector_cb owner_cb;

    void *owner;

    PdvInput *pCamera;

    bool own_pdv;

    bool own_config;

    bool roi_enabled;
    
    int sim_unit;
    bool has_sim;
   
    int pdv_unit, pdv_channel;

    bool has_pdv;

    void show();

    int framebox_x_margin, framebox_y_margin;

    void update_display(void);

    void set_config(CameraConfig *pNewCfg)
    {
	if (pCfg)
	{
	    if (own_config)
		delete pCfg;
	}

	own_config = false;

	pCfg = pNewCfg;
    }
    
    void set_cl_flags(int flags);

    void set_taps(int taps);
    void set_intlv_taps(int taps);

    void set_depth(int depth, int extdepth = 0);

    void update_geometry(void);

    void update_roi_values(const bool enable);

    void set_roi_value(int selector, int value, bool enable = true);

    void show_status(char *s);

    void update_check();

    bool is_modified();

    void check_modified();

    void update_config();

    void check_int_value(const char *name, const int offset);

    void update_config_file();

    void enable_taps(const bool state);

};

#endif
