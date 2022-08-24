


#include "EdtImage.h"

#include "DisplayContrast.h"

#include <FL/Fl_Button.H>

#include <stdio.h>

#include <string>

using namespace std;


void contrast_xy_cb(PlotWindow *w, DisplayContrast *p)

{
    bool changed = false;

    int band = p->channel;
    int bandlow = p->channel;
    int bandhigh = p->channel;

    if (band < 0)
    {
	bandlow=0;
	bandhigh = p->nchannels-1;
    }

    for (band = bandlow;band<=bandhigh;band++)
    {


	switch (p->pWin->pLut->GetMapType())
	{
	case EdtLut::Linear:
	    // check for closest point
    	
	    if (w->cursor_y < 30)
	    {
		p->pWin->pLut->SetMinValue((int) w->cursor_x, band);
		changed = true;
	    }
	    else if (abs((int) w->cursor_y - 255) < 30)
	    {
		p->pWin->pLut->SetMaxValue((int) w->cursor_x, band);
		changed = true;
	    }
	    else if (abs((int) w->cursor_y - 128) < 30)
	    {
		int x1 = p->pWin->pLut->GetMinValue(band);
		int x2 = p->pWin->pLut->GetMaxValue(band);

		int curmid = x2 - x1;

		if (curmid > 0)
		{
		    curmid = x1 + curmid/2;
		}
		else
		{
		    curmid = x2 - curmid/2;		
		}

		int dx = (int) w->cursor_x - curmid;

		x1 += dx;
		x2 += dx;
    	    
		if (x1 < 0) 
		    x1 = 0;
		else if (x1 >= p->pWin->pLut->GetDomainLength())
		    x1 = p->pWin->pLut->GetDomainLength() - 1;
    	    
		if (x2 < 0) 
		    x2 = 0;
		else if (x2 >= p->pWin->pLut->GetDomainLength())
		    x2 = p->pWin->pLut->GetDomainLength() - 1;

		p->pWin->pLut->SetMinValue(x1, band);
		p->pWin->pLut->SetMaxValue(x2, band);

		changed = true;

	    }

	    break;
	}
    }

    if (changed)
    {
	p->update_display((w->LastEvent() == FL_RELEASE));
	p->update();
    }
	

}



DisplayContrast::DisplayContrast(PdvGlWindow *pWindow)

{
    pUI = new DisplayContrastUI();

    pUI->owner = this;

    channel = -1;

    pWin = pWindow;

    // set up plot window 
    pUI->Contrast->SetYAxis(0,255);
    pUI->Contrast->SetYFixed(true);

    pUI->Contrast->callback((Fl_Callback *) contrast_xy_cb, this);
    pUI->Contrast->CursorLine(false);
    pUI->Contrast->PointBox(true);
    

}

void DisplayContrast::reset()

{
    int band;

    int low = 0;
    int high = pWin->pLut->GetDomainLength();

    char buf[32];

    sprintf(buf,"%d", 0);

    pUI->MinValueBox->copy_label(buf);

    sprintf(buf,"%d", high);

    pUI->MaxValueBox->copy_label(buf);

    for (band = 0;band < nchannels;band++)
    {
        pWin->pLut->SetMinValue(0);
        pWin->pLut->SetMaxValue(high);

        pWin->pLut->SetGamma(1.0);
    }

}

void DisplayContrast::update(PdvGlWindow *pWindow)

{
    int band;

    if (pWindow && (pWindow != pWin))
	pWin = pWindow;

    EdtImage *pImage = pWin->GetImage();

    if (pImage->GetNColors() > 1)
	pUI->ChannelButtons->show();
    else
	pUI->ChannelButtons->hide();

    nchannels = pImage->GetNColors();

    // set min max based on image depth

    if (pWin->pLut)
    {
	for (int band = 0; band < pImage->GetNColors();band++)
	{

	    uchar *pLutArray = (uchar *) pWin->pLut->GetLutElement(band);
	    pUI->Contrast->SetVector(pWin->pLut->GetDomainLength(), TYPE_BYTE, 
		pLutArray, band);
	}

	char buf[32];

	if (channel >= 0)
	{
	    sprintf(buf,"%d", pWin->pLut->GetMinValue(channel));

	    pUI->MinValueBox->copy_label(buf);

	    sprintf(buf,"%d", pWin->pLut->GetMaxValue(channel));

	    pUI->MaxValueBox->copy_label(buf);
	}

    }

    
    pUI->Contrast->redraw();
}

void DisplayContrast::update_display(bool redraw_image)

{
 
    update_lut_values();

    pWin->pLut->UpdateMap(pWin->GetImage());

    if (redraw_image)
    {
	pWin->UpdateDisplayImage();
    
	pWin->redraw();
    }

}


void DisplayContrast::set_map_type(EdtLut::MapType type)

{

    m_mode = type;

    if (m_mode == EdtLut::Copy)
        pWin->ClearTransform();
    else
        pWin->SetTransformActive();

    if (pWin->pLut)
        pWin->pLut->SetMapType(type);

    update_display(true);
 
    update();
}

void DisplayContrast::update_lut_values()

{
    if (channel >= 0)
	pWin->pLut->SetGamma(pUI->GammaSlider->value(), channel);
    else
	for (int band = 0;band < nchannels; band++)
	{
	    pWin->pLut->SetGamma(pUI->GammaSlider->value(), band);
	}

    
}


void contrast_linear_cb(Fl_Round_Button* b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;

    pContrast->set_map_type(EdtLut::Linear);


}

void contrast_auto_cb(Fl_Round_Button* b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;
    pContrast->set_map_type(EdtLut::AutoLinear);

}

void contrast_heq_cb(Fl_Round_Button* b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;
    pContrast->set_map_type(EdtLut::AutoHEQ);

}

void contrast_custom_cb(Fl_Round_Button* b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;
    pContrast->set_map_type(EdtLut::Custom);

}

void contrast_gamma_cb(Fl_Round_Button* b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;
    pContrast->set_map_type(EdtLut::Gamma);

}

void contrast_rawdata_cb(Fl_Round_Button* b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;

    pContrast->reset();

    pContrast->set_map_type(EdtLut::Copy);

}

void contrast_select_band_cb(Fl_Button* b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;
  
    string s = b->label();

    if (s == "Red")
        pContrast->channel = 0;
    else if (s == "Green")
        pContrast->channel = 1;
    else if (s == "Blue")
        pContrast->channel = 2;
    else 
        pContrast->channel = -1;
}

void gamma_slider_cb(Fl_Value_Slider * b, void *p)

{
    DisplayContrastUI *pUI = (DisplayContrastUI *) p;
    DisplayContrast *pContrast = (DisplayContrast *) pUI->owner;

    pContrast->set_map_type(EdtLut::Gamma);


}
