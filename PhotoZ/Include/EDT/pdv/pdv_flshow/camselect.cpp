


#include "camconfigUI.h"
#include "CamConfig.h"

#include <stdlib.h>

#include <stdio.h>


CamConfig *pCameraConfig = NULL;



void camconfig_done_cb(void *owner, bool ok, const char *fname, const char *path)

{
    printf("camconfig_done_cb %s filename %s path %s\n", (ok)?"ok":"cancel", fname, path);


}



void usage()

{

}

int main(int argc, char **argv) 

{
    int unit = 0;
    int channel = 0;
    char *devname = "pdv";
    int i;

    for (i=1;i<argc;i++)
    {
	if (argv[i][0] == '-')
	switch (argv[i][1])
	{
	case 'u':
	    i++;
	    if (i == argc)
		usage();
	    
	    unit = atoi(argv[i]);
	    
	    break;

	case 'c':
	    i++;
	    if (i == argc)
		usage();

	    channel = atoi(argv[i]);
	break;

	default:
	    usage();

	}
    }

    int width, height;
 
    if (!pCameraConfig)
       pCameraConfig = new CamConfig(NULL, camconfig_done_cb, "c:/edt/pdv/camera_config","");

    pCameraConfig->show();

    
    //Fl::lock();

    return Fl::run();
}
