//=============================================================================
// LiveFeed.h
//============================================================================= 
#ifndef _Live_Feed_h
#define _Live_Feed_h
//=============================================================================

#include "Camera.h"
#include "ArrayWindow.h"
#include <vector>

class LiveFeed
{
    Camera *cam;

    short *image;
    int imagesize;

    int x1, y1, x2, y2;

    // holds overwritten variables
    int background;
	char showTrace;
	char showRliValue;
	char showDiodeNum;

public:
    LiveFeed();
	~LiveFeed();
    //~LiveFeed();

    void update_image();
    bool running();
    void drawBackground();
    bool begin_livefeed();
    void stop_livefeed();
	void getImage(std::string&);
};



//=============================================================================
#endif
//=============================================================================