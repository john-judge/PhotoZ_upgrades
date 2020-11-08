

#ifndef CAM_BROWSER_H
#define CAM_BROWSER_H

class CamSelector;
class CamSelectorUI;
class CameraConfig;

#include <FL/Fl_Hold_Browser.H>

class CamBrowser : public Fl_Browser_

{
  // required routines for Fl_Browser_ subclass:
  void *item_first() const ;

  void *item_next(void *) const ;

  void *item_prev(void *) const ;

  int item_selected(void *) const ;

  void item_select(void *,int);

  int item_width(void *) const ;

  int item_height(void *) const ;

  void item_draw(void *,int,int,int,int) const ;

  int incr_height() const ;

  int boxwidth() const
  {
      return textsize()-3;
  }
  
public:	

    CameraConfig * value() const;

    CamSelector *cfg;

    int handle(int);

    CamBrowser(int,int,int,int,const char * =0);

    //void add(const char*, void* = 0);
 
    
};


#endif
