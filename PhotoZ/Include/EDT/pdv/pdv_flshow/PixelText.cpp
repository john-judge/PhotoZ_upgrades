


#include "PixelTextUI.h"

#include "PixelText.h"

#include <stdio.h>

PixelText::PixelText(EdtImage *pImage)

{
    pUI = new PixelTextUI(pImage);

    pUI->owner = this;

    follow_cursor = true;

}

void PixelText::update(EdtImage *pImage, const int x, const int y)

{
    pUI->PixelTableWindow->set_image(pImage);

    if (pImage->GetNColors() > 1)
	pUI->ChannelButtons->show();
    else
	pUI->ChannelButtons->hide();

    pUI->PixelTableWindow->set_n_colors(pImage->GetNColors());
 
    if (follow_cursor)
    {
	char val[10];

	pUI->PixelTableWindow->update(x, y);
	pUI->CursorXInput->value(x);
	pUI->CursorYInput->value(y);
    }
    else
    {
	int px = (int) pUI->CursorXInput->value();
	int py = (int) pUI->CursorYInput->value();
	pUI->PixelTableWindow->update(px,py);
    }


    
}

void follow_cursor_cb(Fl_Check_Button *w, void *data)

{
    PixelTextUI *pUI = (PixelTextUI *) data;

    PixelText *pPT = (PixelText *) pUI->owner;

    pPT->follow_cursor = w->value();

}

void pixel_text_channel_red_cb(Fl_Button *w, void *data)

{
    PixelTextUI *pUI = (PixelTextUI *) data;

    pUI->PixelTableWindow->set_channel(0);    
    int px = (int) pUI->CursorXInput->value();
    int py = (int) pUI->CursorYInput->value();
    pUI->PixelTableWindow->update(px,py);

  
}

void pixel_text_channel_green_cb(Fl_Button *w, void *data)

{
    PixelTextUI *pUI = (PixelTextUI *) data;

    pUI->PixelTableWindow->set_channel(1);    
     int px = (int) pUI->CursorXInput->value();
    int py = (int) pUI->CursorYInput->value();
    pUI->PixelTableWindow->update(px,py);


}

void pixel_text_channel_blue_cb(Fl_Button *w, void *data)

{
    PixelTextUI *pUI = (PixelTextUI *) data;

    pUI->PixelTableWindow->set_channel(2);    
     int px = (int) pUI->CursorXInput->value();
    int py = (int) pUI->CursorYInput->value();
    pUI->PixelTableWindow->update(px,py);
 

}
