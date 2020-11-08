
#include "edtdefines.h"

#include "CameraConfig.h"

#include "ConfigDataSet.h"



#include "ConfigDataSet.h"

#include <stdlib.h>

#include <FL/filename.H>

#include <FL/Fl_Text_Buffer.H>

#include "pdv_dep_config.h"

#define MAX_COMMENT 2048

CameraConfig::CameraConfig(const char *newpath, const char *fname)


{

    next = prev = this;
    
    parent = NULL;

    selected = false;

    index = 0;

    opened = false;

    dd_p = NULL;

    loaded = false;

    cfg = NULL;
    filename = NULL;
    fullfilename = NULL;
    path = NULL;
    cameratype = NULL;
    camera_class = NULL;
    camera_model = NULL;
    camera_info = NULL;
    help = NULL;


    width = height = 0;
    depth = 0;

    LoadInitial(newpath, fname);

}

CameraConfig::~CameraConfig()

{
    if (cfg)
	cfg_set_destroy(cfg);
}

int
CameraConfig::LoadInitial(const char *newpath, const char *fname)

{

    if (cfg == NULL)
    {
	cfg = new_cfg_set();
    }
    else
	cfg_set_destroy(cfg);

    if (fname)
    {

	if (strrchr(fname, '/'))
	    filename=strdup(strrchr(fname, '/')+1);
	else
	    filename = strdup(fname);

	char s[FL_PATH_MAX];

	if (newpath)
	    path = strdup(newpath);
	else
	    path = strdup(".");

	sprintf(s, "%s/%s", newpath, filename);

	cfg->strip_quotes = true;

	if (cfg_set_load(cfg, s) == -1)
	{
	    cfg_set_load(cfg, filename);
	    fullfilename = strdup(filename);
	}
	else
	    fullfilename = strdup(s);


	char *test;

	if (test = cfg_set_lookup(cfg,  "cameratype"))
	    cameratype = strdup(test); 
	else 
	    cameratype = strdup("");

	if (test = cfg_set_lookup(cfg,  "camera_class"))
	    camera_class = strdup(test); 
	else 
	    camera_class = strdup("");

	if (test = cfg_set_lookup(cfg,  "camera_model"))
	    camera_model = strdup(test); 
	else 
	    camera_model = strdup("");

	if (test = cfg_set_lookup(cfg,  "camera_info"))
	    camera_info = strdup(test); 
	else 
	    camera_info = strdup("");

	if (test = cfg_set_lookup(cfg,  "cameralink"))
	    camera_link = atoi(test);
	else if (cfg_set_lookup(cfg,  "CL_CFG_NORM") || cfg_set_lookup(cfg,  "CL_DATA_PATH_NORM"))
	    camera_link = 1;
	else
	    camera_link = 0;

	if (test = cfg_set_lookup(cfg,  "width"))
	    width = atoi(test);

	if (test = cfg_set_lookup(cfg,  "height"))
	    height = atoi(test);

	if (test = cfg_set_lookup(cfg,  "depth"))
	    depth = atoi(test);


	CfgFileItem *pComment = cfg_set_FirstItem(cfg);

	char comment_buffer[MAX_COMMENT];
	comment_buffer[0] = 0;

	while (pComment && pComment->ItemName==NULL)
	{
	    int len = 0;
	    
	    if (pComment->ItemComment) 
		len = strlen(pComment->ItemComment);

	    if (len && (strlen(comment_buffer) + len < MAX_COMMENT))
	    {
		if (comment_buffer[0]);
		    strcat(comment_buffer,"\n");
		strcat(comment_buffer, pComment->ItemComment);
	    }

	    pComment = cfg_set_NextItem(cfg);
	}

	help = strdup(comment_buffer);

	visible = false;

    }
    else
    {
	// item for camera class - passed in in newpath value

	camera_info = strdup("");
	camera_model = strdup("");
	cameratype = strdup("");
	if (newpath)
	    camera_class = strdup(newpath);
	else
	    camera_class = strdup("");

	filename = strdup("");
	fullfilename = strdup("");

	camera_link = false;
	visible = true;

	help = strdup("");
	width = height = depth = 0;

    }


    loaded = false;

    return 0;

}

void CameraConfig::LoadFull()

{
    if (dd_p == NULL)
    {
	dd_p = (PdvDependent *) edt_alloc(sizeof(PdvDependent));
    }

    if (pdv_cfg_file_read(fullfilename, dd_p, FALSE, cfg) != 0)
	loaded = true;

}

   
