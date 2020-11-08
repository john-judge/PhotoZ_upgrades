
#include "edtdefines.h"

#include "CamSelector.h"

#include "CamSelectorUI.h"


#include "cfgfileset.h"

#include <stdlib.h>

#include <FL/filename.H>

#include <FL/Fl_Text_Buffer.H>

#define MAX_COMMENT 2048


CamSelector::CamSelector(void *new_owner, CamSelector_cb cb, const char *path, const char *name)

{
    current_name = strdup(name);
    current_path = strdup(path);

    pUI = new CamSelectorUI(this, path);
    pUI->ConfigBrowser->cfg = this;
    owner = new_owner;
    owner_cb = cb;

    pFirstItem = NULL;
    pLastItem = NULL;

    Load();

    pUI->show();

 

    

}

CamSelector::~CamSelector()

{
   free(current_path);
   free(current_name);

   delete pUI;


}

void CamSelector::show()
{
    pUI->CamSelectorWindow->show();
}

void CamSelector::hide()
{
    pUI->CamSelectorWindow->hide();

}

void CamSelector::Load()

{
    dirent **plist;

    int nfiles;
    int i;

    CfgFileSet camclasses;

    cfg_set_init(&camclasses);

    camclasses.copy_value = false;

    char path[FL_PATH_MAX];
    sprintf(path,"%s", current_path);

    nfiles = fl_filename_list(path, &plist, fl_casealphasort);

    for (i=0;i<nfiles;i++)
    {
	if (fl_filename_match(plist[i]->d_name,"*.cfg"))
	{
	    // get config info
	    CameraConfig *parent;

	    CameraConfig *pItem = new CameraConfig(current_path, 
		plist[i]->d_name);

	    // get its class item

	    // append to class item's list

	    char *p;

	    if (p = cfg_set_lookup(&camclasses, pItem->camera_class))
	    {
		parent = (CameraConfig *) p;
		parent->children.Append(pItem);
	    }
	    else
	    {
		parent = new CameraConfig(pItem->camera_class, NULL);
		cfg_set_add_item(&camclasses, pItem->camera_class, (char *) parent, NULL);
		parent->children.Append(pItem);
		
	    }
	}
    }

    // build complete list of items
//	    pUI->ConfigBrowser->add(pItem->filename, pItem);

    // Go through the list of 

    CfgFileItem *pClass = cfg_set_FirstItem(&camclasses);

    while (pClass)
    {
	CameraConfig *pItem;
	CameraConfig *parent = (CameraConfig *) pClass->ItemValue;

	if (pFirstItem == NULL)
	{
	    pFirstItem = pLastItem = parent;
	}
	else
	{
	    pFirstItem->InsertBefore(parent);
	}

	pItem = parent->children.GetFirst();

	while (pItem)
	{
	    pFirstItem->InsertBefore(pItem);
	    //pUI->ConfigBrowser->add(pItem->filename, pItem);
	    pItem = parent->children.GetNext();
	}

	pClass = cfg_set_NextItem(&camclasses);

    }
}


CameraConfig *
CamBrowser::value() const

{
    CameraConfig *p = (CameraConfig *)selection();
    return p;
}



void camselector_ok_cb(Fl_Button *b, void *p)

{

    CamSelectorUI *win = (CamSelectorUI *) p;

    CamSelector *config = win->owner;

    if (config && config->owner_cb)
	config->owner_cb(config->owner, 1, win->ConfigFileInput->value(), win->ConfigPathInput->value());

    
    win->CamSelectorWindow->hide();

}

void camselector_cancel_cb(Fl_Button *b, void *p)

{

    CamSelectorUI *win = (CamSelectorUI *) p;

    CamSelector *config = win->owner;

    if (config && config->owner_cb)
	config->owner_cb(config->owner, 0, NULL, NULL);

    win->CamSelectorWindow->hide();

}

void camselector_reload_cb(Fl_Button *b, void *p)

{


}

void camselector_browser_cb(CamBrowser *b, void *p)

{

    // figure out which element is selected

    // get file name

    // fill in file name

    CamSelectorUI* pUI = (CamSelectorUI *) p;

    CameraConfig *pItem;

    if (b->value())
    {
	pItem = (CameraConfig *) b->value();
    
	if (pItem && pItem->filename && pItem->filename[0])
	{
	    char description[256];

	    sprintf(description, "%s: %s", pItem->camera_model, pItem->camera_info);
	    pUI->CameraTypeOutput->value(description);
	    pUI->ConfigFileInput->value(pItem->filename);

	    pUI->CameraWidthOutput->value(pItem->width);
	    pUI->CameraHeightOutput->value(pItem->height);
	    pUI->CameraDepthOutput->value(pItem->depth);

	    Fl_Text_Buffer *phelp = pUI->InitialCommentOutput->buffer();

	    phelp->remove(0,phelp->length());
	    phelp->insert(0,pItem->help);
	}
    }
}


