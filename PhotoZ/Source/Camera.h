//=============================================================================
// Camera.h
//=============================================================================
#ifndef CAMERA_H_
#define CAMERA_H_

#include "edtinc.h"

  extern class RecControl *recControl;  // from definitions.h for getting camGain
// compiler camera switch
#define LILJOE
//#define LILDAVE

class Camera {
	char edt_devname[128];
	int unit;					// Some parameters that can be set, but are
	int channel;				// usually just 0
	int numbufs;				// ring buffers in pdv calls
	int m_depth;
    PdvDev *pdv_p;
	int timeouts, m_num_timeouts;
	int last_timeouts;
	int overruns;
	bool recovering_timeout;
	int m_program;				// which program is loaded currently

public:

#ifdef LILJOE

#define NUM_CAM_PRGS 8
	static const char* LABEL[];
	static const int WIDTH[];
	static const int HEIGHT[];
	static const int FREQ[];

#endif // LILJOE

/*public:
#ifdef LILDAVE

#define NUM_CAM_PRGS 8
	static const char* LABEL[];
	static const int WIDTH[];
	static const int HEIGHT[];
	static const int FREQ[];

#endif // LILDAVE*/

    Camera();
    ~Camera();

    int open_channel();

	unsigned char* single_image();
    void start_images();
	void end_images();
    unsigned char* wait_image();

	void serial_write(const char *buf);
	void serial_read(char *buf, int size);

    int num_timeouts();

	int program();
	void program(int p);
	int width();
	int height();
	int depth();
	int freq();
	char* devname();

};

#endif // CAMERA_H_
