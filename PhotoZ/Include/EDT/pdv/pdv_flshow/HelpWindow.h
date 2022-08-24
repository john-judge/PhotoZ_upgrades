
#ifndef _PDV_HELP_WINDOW_H
#define _PDV_HELP_WINDOW_H

#include "LogWindowUI.h"

#include "HelpCatalog.h"



class HelpWindow {

public:

    HelpWindow(const char *helpfile = NULL);

    ~HelpWindow();

    LogWindowUI *pWin;

    HelpCatalog *cat;

    void show_help(const char *key);

}


#endif
