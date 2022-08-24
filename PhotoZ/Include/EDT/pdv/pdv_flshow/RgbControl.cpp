


#include "EdtImage.h"

#include "RgbControl.h"

#include <FL/Fl_Button.H>

#include <stdio.h>


RgbControl::RgbControl(PdvGlWindow *pWindow, PdvInput *dev)

{
    pUI = new RgbControlUI();

    pUI->owner = this;

    pdv = dev;

    channel = 0;

    pWin = pWindow; 

    refresh();

    m_raw_image = m_image = NULL;

}

void RgbControl::SetImages(EdtImage *pRaw, EdtImage *pTarget)

{
    m_raw_image = pRaw;
    m_image = pTarget;
}


void RgbControl::refresh(PdvGlWindow *pWindow)

{
    int band;

    if (pWindow && (pWindow != pWin))
	pWin = pWindow;

    m_image = pWin->GetImage();
    m_raw_image = pWin->GetRawImage();

    if (pUI)
    {
        pUI->RedSlider->value(converter.redscale);
        pUI->GreenSlider->value(converter.greenscale);
        pUI->BlueSlider->value(converter.bluescale);

        switch(converter.Order())
        {
        case PDV_BAYER_ORDER_BGGR:
            pUI->BGGR_Button->value(1);
            break;
        case PDV_BAYER_ORDER_RGGB:
            pUI->RGGB_Button->value(1);
            break;
        case PDV_BAYER_ORDER_GBRG:
            pUI->GBRG_Button->value(1);
            break;
        case PDV_BAYER_ORDER_GRBG:
            pUI->GRBG_Button->value(1);
            break;
        }
    }
}

void RgbControl::update()

{
    Setup();

    if (m_raw_image)
        converter.process(m_raw_image, m_image);

    pWin->redraw();
}

void RgbControl::GetInitial()

{
    if (has_pdv)
    {
        initial.GetCurrent();
        converter.GetCurrent();
    }
}

void rgb_gamma_slider_cb(Fl_Value_Slider * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    double v = b->value();

    pRgb->converter.gamma = v;

    pRgb->update();


}

void rgb_red_slider_cb(Fl_Value_Slider * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    double v = b->value();


    pRgb->converter.redscale = v;
    pRgb->update();

}

void rgb_green_slider_cb(Fl_Value_Slider * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    double v = b->value();


    pRgb->converter.greenscale = v;

    pRgb->update();


}

void rgb_blue_slider_cb(Fl_Value_Slider * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    double v = b->value();


    pRgb->converter.bluescale = v;

    pRgb->update();

}

void rgb_Cancel_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->converter = pRgb->initial;
    pRgb->Setup();

    pUI->RgbControlWindow->hide();

}

void rgb_OK_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->Setup();

    pUI->RgbControlWindow->hide();

}

void rgb_apply_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->Setup();

    if (pRgb->m_raw_image)
        pRgb->converter.process(pRgb->m_raw_image, pRgb->m_image);

    pRgb->pWin->redraw();

}

void rgb_sample_wb_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;
        
    pRgb->refresh();

    if (pRgb->m_raw_image)
        pRgb->converter.ComputeWhiteBalance(pRgb->m_raw_image);

    pRgb->converter.process(pRgb->m_raw_image, pRgb->m_image);

    pRgb->refresh();

    pRgb->pWin->redraw();

}

void grbg_button_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->converter.SetOrder(1,1);
    pRgb->Setup();

}

void gbrg_button_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->converter.SetOrder(0,1);
    pRgb->Setup();

}

void rggb_button_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->converter.SetOrder(1,0);
    pRgb->Setup();

}

void bggr_button_cb(Fl_Button * b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->converter.SetOrder(0,0);
    pRgb->Setup();

}

void rgb_reset_button_cb(Fl_Button *b, void *p)

{
    RgbControlUI *pUI = (RgbControlUI *) p;
    RgbControl *pRgb = (RgbControl *) pUI->owner;

    pRgb->converter.redscale = 
        pRgb->converter.greenscale = 
        pRgb->converter.bluescale = 
        pRgb->converter.gamma = 1.0;

    pRgb->refresh();

    pRgb->update();
}


void RgbControl::Setup()
{
    converter.setup(m_raw_image, pdv);
}
