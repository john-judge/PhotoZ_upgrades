
#ifndef _CfgFileTextEditor_H
#define _CfgFileTextEditor_H

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Help_View.H>

#include "CameraConfig.h"

class CameraConfigEditor;

class CfgTextEditWin;



class CfgFileTextEditor : public Fl_Double_Window 

{

    
    CfgTextEditWin *ed;

    CameraConfigEditor *pCfgEd;
    CameraConfig *pCfg;

    Fl_Text_Buffer *buf, *stylebuf;
    Fl_Text_Display *helpwin;

    Fl_Double_Window *helpframe;

    bool modified;

    void parse_colors(int start, int end);
    void style_init();

    CfgFileSet *pHelp;

public:

    CfgFileTextEditor(int w, int h, const char *t = NULL);
    ~CfgFileTextEditor();

    void SetEditor(CameraConfigEditor * ED); // this will set CFG
    void SetCfg(CameraConfig *CFG);

    
    bool is_modified();

    void set_help(const char *name);

};


#endif
