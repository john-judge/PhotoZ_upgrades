//=============================================================================
// Camera.h
//=============================================================================
#ifndef CAMERA_H_
#define CAMERA_H_

#include "edtinc.h"

  extern class RecControl *recControl;  // from definitions.h for getting camGain
// compiler camera switch
//#define LILJOE
#define LILDAVE

class Camera {
	char edt_devname[128];
	int m_depth;

	PdvDev *pdv_pt[4];
	int timeouts[4], m_num_timeouts;
	int last_timeouts;
	int overruns = 0;
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

public:
#ifdef LILDAVE

#define NUM_CAM_PRGS 8
	static const char* LABEL[];
	static const int WIDTH[];
	static const int HEIGHT[];
	static const char* PROG[];
	static const int FREQ[];
	static const double sm_lib_rates[];
	static const double sm_cam_lib_rates[];
	static const double sm_integration_increments[];
	static const int ccd_lib_bin[];
	static const int config_load_lib[];
	static const int cds_lib[];
	static const int ndr_lib[];
	static const int stripes_lib[];
	static const int layers_lib[];
	static const int rotate_lib[];
	static const int bad_pix_lib[];
	static const int start_line_lib[];
	static const int super_frame_lib[];
	static const int NDR_start_lib[];
	static const int NDR_inc_lib[];
	static const int NDR_code_lib[];
	static const int NDR_max_lib[];
	static const int reserve1_lib[];
	static const int reserve2_lib[];
	static const int reserve3_lib[];
#endif // LILDAVE*/

    Camera();
    ~Camera();

    int open_channel();

	unsigned char* single_image(int ipdv);
    void start_images();
	void end_images();
	void end_images(int ipdv);
	void init_cam();

	void setCamProgram(int p);
    unsigned char* wait_image(int ipdv);

	void serial_write(const char *buf);
	//void serial_read(char *buf, int size);

    int num_timeouts(int ipdv);
	
	void get_image_info(int ipdv);
	int get_buffer_size(int ipdv);

	int program();
	void program(int p);

	int width();
	int height();
	int depth();
	int freq();

	void deinterleave(unsigned short *buf, int n, int m);
	void deinterleave(unsigned short *buf);
	void deinterleave2(unsigned short * buf, int n, int m);
	void deinterleave3(unsigned short * buf, int n, int m);
	int mapFromInterleaved(int i, int j, int n, int m, int quadrant);
	int mapToDeinterleaved(int i, int j, int n, int m, int quadrant);

	void subtractCDS(unsigned short *image_data);
	void subtractCDS(unsigned short *image_data, unsigned int m, unsigned int n);

	void mapQuadrants(unsigned short* memory);
	void mapQuadrants(unsigned short* memory, int n, int m);

	void postAcquisitionProcessing(unsigned short *images, int nImages);

};

#endif // CAMERA_H_
