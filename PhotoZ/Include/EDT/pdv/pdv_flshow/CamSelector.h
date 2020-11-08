

#ifndef CamSelector_H
#define CamSelector_H

class CamSelectorUI;


#include "EdtList.h"

#include "CameraConfig.h"


typedef void (*CamSelector_cb)(void *owner, bool ok, const char *name, const char *path);

class CamSelector {
    
    CamSelectorUI *pUI;


    char *current_name;
    char *current_path;

public:

    CamSelector(void *owner, CamSelector_cb cb, const char *path, const char *name);
    ~CamSelector();

    
    CamSelector_cb owner_cb;
    void *owner;

    void Load();

    CameraConfig *pFirstItem;
    CameraConfig *pLastItem;

    EdtListHead<CameraConfig> class_list;

    void show();
  

    void hide();
  


} ;


#endif
