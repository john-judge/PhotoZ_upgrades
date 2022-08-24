/**
 * @file
 * An example program to show usage of EDT PCI DV library to acquire and
 * optionally save single or multiple images from devices connected
 * to EDT high speed digital imaging interface such as the PCI DV C-Link
 * or PCI DV FOX / RCX.
 * 
 * This is the simplest example for acquiring single images, and is
 * provided as a starting point incorporating digital image acquisition
 * in a user application.  It doesn't provide for any arguments to specify
 * board or channel number, number of buffers, number of loops, etc.,
 * optimization such as ring buffering or pipelining or error checking -- just the
 * basics to get a simple image grab. For an example that includes those
 * operations, see simple_take.c, other simple_*.c programs.  For even more
 * more complex operations, including error detection, diagnostics, changing
 * camera exposure times, and tuning the acquisition in various ways, see the
 * take.c utility. For serial, see serial_cmd.c.
 * 
 * For a sample Windows GUI application code, see wintake.
 * 
 * (C) 2005-2007 Engineering Design Team, Inc.
 */
#include "edtinc.h"

static void 
save_image(u_char * image_p, int width, int height, int depth,
	   char *basename, int count);

/*
 * main
 */
main(int argc, char **argv)
{
    int     unit = 0, channel = 0;
    char    edt_devname[128];
    char    bmpfname[128];
    char    errstr[64];
    u_char *image_p;
    PdvDev *pdv_p;

    *bmpfname = '\0';

    if (argc > 1)
	if (argv[1][0] == '-')
	    printf("usage: simplest_take [filename]\n");
    else strcpy(bmpfname, argv[1]);

    /*
     * open the interface
     * 
     * EDT_INTERFACE is defined in edtdef.h (included via edtinc.h).
     * With one EDT PCI DV board in the chassis it's uddressed as
     * unit 0; the first channel (bottom connector) on multi-channel
     * boards such as the PCI DV C-Link, is channel 0
     *
     */
    if ((pdv_p = pdv_open_channel(EDT_INTERFACE, unit, channel)) == NULL)
    {
	sprintf(errstr, "pdv_open_channel(%s%d_%d)", edt_devname, unit, channel);
	pdv_perror(errstr);
	return (1);
    }

    /*
     * acquire an image
     */
    image_p = pdv_image(pdv_p);

    /* 
     * optional (but recommended) -- check for timeouts (missing data)
     */
    if (pdv_timeouts(pdv_p) > 0)
	printf("got timeout, check camera and cables\n");
    else printf("got one image\n");

    /*
     * process the image. in this app we just save it if indicated.
     */
    if (*bmpfname)
	save_image(image_p, pdv_get_width(pdv_p), pdv_get_height(pdv_p), pdv_get_depth(pdv_p), bmpfname, 0);


    pdv_close(pdv_p);

    exit(0);
}

static void
save_image(u_char * image_p, int s_width, int s_height, int s_depth, char *tmpname, int count)
{
    int     s_db = bits2bytes(s_depth);
    u_char *bbuf = NULL;
    char    fname[256];

#ifdef _NT_
    if ((strcmp(&tmpname[strlen(tmpname) - 4], ".bmp") == 0)
	|| (strcmp(&tmpname[strlen(tmpname) - 4], ".BMP") == 0))
	tmpname[strlen(tmpname) - 4] = '\0';
    sprintf(fname, "%s%02d.bmp", tmpname, count);
#else
    if ((strcmp(&tmpname[strlen(tmpname) - 4], ".ras") == 0)
	|| (strcmp(&tmpname[strlen(tmpname) - 4], ".RAS") == 0))
	tmpname[strlen(tmpname) - 4] = '\0';
    sprintf(fname, "%s%02d.ras", tmpname, count);
#endif

    /*
     * write bmp file on Windows systems, or Sun Raster on Unux/Linux
     * systems. Switch on number of bytes per pixel
     */
    switch (s_db)
    {
    case 1:
#ifdef _NT_

	dvu_write_bmp(fname, image_p, s_width, s_height);
#else
	printf("writing %dx%dx%d raster file to %s\n",
	       s_width, s_height, s_depth, fname);

	dvu_write_rasfile(fname, (u_char *) image_p, s_width, s_height);
#endif
	break;

    case 2:
	printf("converting %dx%dx%d image to 8 bits, writing to %s\n",
	       s_width, s_height, s_depth, fname);

#ifdef _NT_
	if (!bbuf)
	    bbuf = (u_char *) pdv_alloc(s_width * s_height);

	if (bbuf == NULL)
	{
	    pdv_perror("data buf malloc");
	    exit(1);
	}
	dvu_word2byte((u_short *) image_p, (u_char *) bbuf,
		      s_width * s_height, s_depth);
	dvu_write_bmp(fname, bbuf, s_width, s_height);
#else
	dvu_write_rasfile16(fname, (u_char *) image_p, s_width, s_height, s_depth);
#endif
	break;

    case 3:
	printf("writing %dx%dx%d bmp file to %s\n",
	       s_width, s_height, s_depth, fname);

#ifdef _NT_
	dvu_write_bmp_24(fname, (u_char *) image_p, s_width, s_height);
#else
	dvu_write_rasfile24(fname, (u_char *) image_p, s_width, s_height);
#endif

	break;

    default:
	printf("invalid image depth for file write...!\n");
	break;
    }
}

