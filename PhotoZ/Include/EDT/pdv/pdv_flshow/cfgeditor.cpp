



#include "CameraConfigEditor.h"

#include <stdlib.h>

#include <stdio.h>


CameraConfigEditor *pCameraConfig = NULL;

PdvInput *pCamera;

void camconfig_done_cb(void *owner, bool ok, const char *fname, const char *config_path)

{
    printf("camconfig_done_cb %s filename %s config_path %s\n", (ok)?"ok":"cancel", fname, config_path);


}

char *filename = NULL;


char *config_path = 
#ifdef WIN32
"C:/edt/pdv/camera_config";
#else
"/opt/EDTpdv/camera_config";
#endif


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

	case 'f':
	    i++;
	    if (i == argc)
		usage();
	    filename = strdup(argv[i]);
	    break;

	case 'p':
	    i++;
	    if (i == argc)
		usage();
	    config_path = strdup(argv[i]);
	    break;


	default:
	    usage();

	}
    }

    int width, height;

    pCamera = new PdvInput(EDT_INTERFACE, unit, channel);

    if (!filename)
    {
	if (pCamera)
	{
	    filename = pCamera->GetLoadedConfig();
	    if (filename)
		filename = strdup(filename);
	    else
		filename = strdup("");
	}
	else
	    filename = strdup("");
    }

 
    if (!pCameraConfig)
       pCameraConfig = new CameraConfigEditor(NULL, pCamera, camconfig_done_cb, config_path, filename);

    pCameraConfig->show();

    
    //Fl::lock();

    return Fl::run();
}
