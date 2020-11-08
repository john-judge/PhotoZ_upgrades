
#ifndef CameraConfig_H
#define CameraConfig_H

extern "C" {

#include "edtinc.h"
#include "pdv_dependent.h"
#include "initcam.h"

}
#include "EdtList.h"

#include "cfgfileset.h"


class CameraConfig {
public:

    CfgFileSet *cfg;

    PdvDependent *dd_p;

    int index;
    char *filename;
    char *fullfilename;
    char *path;
    char *cameratype;
    char *camera_class;
    char *camera_model;
    char *camera_info;
    char *help;
    int width;
    int height;
    int depth;

    bool loaded;
    bool visible;
    bool opened;
    bool camera_link;
    
    int selected;

    CameraConfig *parent;
    EdtListHead<CameraConfig> children;

    CameraConfig *prev;
    CameraConfig *next;

    CameraConfig(const char *newpath, const char *fname);

    ~CameraConfig();
	

    void Cutout()
    {
	prev->next = next;
	next->prev = prev;
	prev = next = this;
    }

    void InsertAfter(CameraConfig *pNew)
    {
	pNew->next = next;
	next->prev = pNew;

	pNew->prev = this;
	next = pNew;

    }

    void InsertBefore(CameraConfig *pNew)
    {
	pNew->prev = prev;
	prev->next = pNew;

	pNew->next = this;
	prev = pNew;
    }

    bool IsParent()
    {
	return (children.Length() > 0);
    }

    void LoadFull();

    int LoadInitial(const char *newpath, const char *fname);

};


#endif
