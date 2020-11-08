#ifndef _EDT_BAYER_CONVERTER_H_
#define _EDT_BAYER_CONVERTER_H_

#include "EdtImage.h"


extern "C" {

#include "pdv_interlace_methods.h"

extern double  bayer_rgb_scale[3] ;

extern int bayer_red_first;
extern int bayer_green_first;
extern int bayer_current_base_size;

}


#include "ColorBalance.h"
#include "PdvInput.h"

#define PDV_BAYER_ORDER_BGGR 0
#define PDV_BAYER_ORDER_GBRG 1
#define PDV_BAYER_ORDER_RGGB 2
#define PDV_BAYER_ORDER_GRBG 3

class BayerConverter : public EdtImageData {

     double rgb_scale[3];
     int order;

public:
     int input_bits;
     double gamma;
     int blackoffset;
     int red_row_first;
     int green_pixel_first;
     bool is_rgb;

     double bluescale;
     double greenscale;
     double redscale;

     EdtColorBalance balance;
     PdvInput *pdv;

    BayerConverter()

    {
        gamma = 1.0;
        blackoffset = 0;
        red_row_first = 1;
        green_pixel_first = 1;
        rgb_scale[0] = 1.0;
        rgb_scale[1] = 1.0;
        rgb_scale[2] = 1.0;
        is_rgb = false;
        input_bits = 8;
        bluescale = 1.0;
        redscale = 1.0;
        greenscale = 1.0;

        order = PDV_BAYER_ORDER_GRBG;

        pdv = NULL;

    }

    int setup(EdtImage *src = NULL, PdvInput *in_pdv = NULL)
    {
        if (src)
           input_bits = src->GetDepth();

        if (in_pdv)
            pdv = in_pdv;

        if (is_rgb)
        {
            order = (!(red_row_first) ?2:0) | ((green_pixel_first)?1:0);
            rgb_scale[0] = bluescale;
            rgb_scale[2] = redscale;
        }
        else
        {
            order = ((red_row_first) ?2:0) | ((green_pixel_first)?1:0);
            rgb_scale[2] = bluescale;
            rgb_scale[0] = redscale;
        }

        rgb_scale[1] = greenscale;

        set_bayer_parameters(input_bits,
		      rgb_scale,
		      gamma,
		      blackoffset,
                      (is_rgb) ? !red_row_first : red_row_first,
		      green_pixel_first);

        if (pdv)
            pdv->SetBayerParameters(input_bits,
                    rgb_scale,
		      gamma,
		      blackoffset,
                      (is_rgb) ? !red_row_first : red_row_first,
		      green_pixel_first);
        else
            set_bayer_parameters(input_bits,
		      rgb_scale,
		      gamma,
		      blackoffset,
                      (is_rgb) ? !red_row_first : !red_row_first,
		      green_pixel_first);


        return 0;
    }

    int process(EdtImage *src, EdtImage *dest)

    {

        setup(src);

       int width = src->GetWidth();
       int rows = src->GetHeight();
       int pitch = src->GetPitch(); 

       int rc = -1;

       if (input_bits == 8)
       {
            rc = convert_bayer_image_8_BGR((u_char *) src->GetBaseData(), 
			              width, rows, pitch, 
			              (u_char *) dest->GetBaseData(), 
			              order);
       }
       else
       {
            rc = convert_bayer_image_16_BGR((u_short *)src->GetBaseData(), 
			              width, rows, pitch, 
			              (u_char *) dest->GetBaseData(), 
			              order, input_bits);
       }

       return rc;
    }

    int ComputeWhiteBalance(EdtImage *src_image, EdtImage *dest_image = NULL)

    {
        EdtImage work_rgb;
        EdtImage *dp;

        int i;

        redscale = greenscale = bluescale = 1;
        double start_gamma = gamma;
        gamma = 1;


        if (!dest_image)
        {
            work_rgb.Create((is_rgb)?TypeRGB:TypeBGR,
                                src_image->GetWidth(),
                                src_image->GetHeight());

            dp = &work_rgb;
        }
        else
            dp = dest_image;


        process(src_image, dp);

        balance.ComputeColorBalance(dp, rgb_scale);

        bluescale = rgb_scale[2];
        redscale  = rgb_scale[0];

        rgb_scale[1] = 1;
        gamma = start_gamma;

        return 0;

    }

    int ComputeWhiteBalance(EdtImage *src_image, int x, int y, int w, int h)

    {

        EdtImage work;
        EdtImage work_rgb;
        int i;

        work.AttachToParent(src_image, x,y,w,h);

        work_rgb.Create(TypeRGB, w, h);

        for (i=0;i<3;i++)
            rgb_scale[i] = 1;

        process(&work, &work_rgb);

        balance.ComputeColorBalance(&work_rgb, rgb_scale);

        rgb_scale[1] = 1;

        return 0;
    }

    void GetCurrent()
    {
        int i;
        for (i=0;i<3;i++)
            rgb_scale[i] = bayer_rgb_scale[i];
        red_row_first = bayer_red_first;
        green_pixel_first = bayer_green_first;
    }

    void SetOrder(int red_first, int green_first)
    {
        red_row_first = red_first;
        green_pixel_first = green_first;


        if (is_rgb)
            order = ((red_row_first) ?2:0) | ((green_pixel_first)?1:0);
        else
            order = ((!red_row_first) ?2:0) | ((green_pixel_first)?1:0);

    }

    int Order()

    {
        return ((red_row_first) ?2:0) | ((green_pixel_first)?1:0);
    }


};


#endif
