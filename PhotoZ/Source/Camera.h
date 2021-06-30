//=============================================================================
// Camera.h
//=============================================================================
#ifndef CAMERA_H_
#define CAMERA_H_

#include "edtinc.h"
#define NUM_PDV_CHANNELS 4

extern class RecControl* recControl;  // from definitions.h for getting camGain
// compiler camera switch
//#define LILJOE
#define LILDAVE

class Camera {
	char edt_devname[128];
	int m_depth;
	PdvDev* pdv_pt[NUM_PDV_CHANNELS];
	int timeouts[NUM_PDV_CHANNELS], m_num_timeouts;
	int last_timeouts;
	int overruns = 0;
	bool recovering_timeout;
	int m_program;				// which program is loaded currently

	unsigned int* lut;

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
	static const int DISPLAY_WIDTH[];
	static const int DISPLAY_HEIGHT[];
	static const int WIDTH_DIVISIVE_FACTOR[];
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
	static const int channelOrders[16];
	char cameraType[80], cameraStr[256];

#endif // LILDAVE*/

	Camera();
	~Camera();

	int open_channel(int ipdv);

	unsigned char* single_image(int ipdv);
	void start_images(int ipdv);
	void end_images(int ipdv);
	void init_cam();

	void setCamProgram(int p);
	unsigned char* wait_image(int ipdv);

	void serial_write(const char* buf);
	void serial_read(char* buf, int size);

	int num_timeouts(int ipdv);

	void get_image_info(int ipdv);
	int get_buffer_size(int ipdv);

	int program();
	void program(int p);

	int width();
	int height();
	int get_display_width();
	int get_display_height();
	int depth();
	int freq();

	// Main camera functions
	bool isValidPlannedState(int num_diodes);
	bool isValidPlannedState(int num_diodes, int num_fp_diodes);

	unsigned short* allocateImageMemory(int num_diodes, int numPts);
	bool acquireImages(unsigned short* memory, int numPts, int first, int notLast);
	bool acquireImages(unsigned short* images, int numPts);

	// camera utility functions. Generally drivers should not need to call
	void remapQuadrantsOneImage(unsigned short* buf, int quadHeight, int quadWidth);
	void reassembleImages(unsigned short* images, int nImages);

	void deinterleave(unsigned short* buf, int quad_height, int quad_width, const int* channelOrder, bool flipVertically);
	void subtractCDS(unsigned short* image_data, int nImages, int quad_height, int quad_width);

	// Debugging
	void printFinishedImage(unsigned short* image, const char* filename, bool CDS_done);
	void printQuadrant(unsigned short* image, const char* filename);

	int makeLookUpTable(unsigned int * Lut, int image_width, int image_height, int file_width, int file_height);
	void frame_deInterleave(int length, unsigned * lookuptable, unsigned short * old_images, unsigned short * new_images);

	// From TurboSM
	/*
	int SM_initCamera(int load_config_flag, int close_flag, int load_pro_flag);
	int SM_pdv_open();
	void SM_pdv_close();
	int checkCfgLUT();
	*/
};

/*
typedef struct SM_CONFIGINFO {
	const char *cfg_str;
	int width[8];
	int height[8];
} SM_CONFIGTYPE, *SM_CONFIG_PTR;

SM_CONFIGTYPE configLUT[] = {
{ "CCID79-PCD",				{ 128, 256, 2304, 1024, 1024, 1024 },					//width
							{ 128, 128, 912, 160, 32, 15, 128, 40 } },				//height
{ "2kx2k_NEURO_BINNED_d_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },		//width
							{ 512, 200, 50, 160, 32, 15, 128, 40 } },				//height
{ "2kx2kCARDIO_s2K_",		{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 1024, 200, 512, 320, 160, 80, 256, 128 } },
{ "2kx2k_NEURO_BINNED_s_",	{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 200, 256, 160, 80, 32, 128, 80 } },
{ "2kx2k_NEURO_BINNED_",	{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },		//Manuela used the same name, need to rename
							{ 512, 50, 160, 80, 80, 40, 30, 20 } },
{ "2kx2k_NEURO_BINNED2_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 200, 100, 256, 160, 80, 128, 112 } },
{ "2kx2k_NEURO_BINNED3_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 100, 50, 160, 80, 32, 80, 31 } },
{ "2kx2k_NEURO_BINNED4_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },
							{ 1024, 100, 50, 160, 80, 80, 31, 128 } },
{ "2kx2k_NEURO_v",			{ 2048, 2048, 2048, 2048, 2048, 1024, 1024, 1024 },
							{ 1024, 512, 200, 100, 50, 320, 160, 80 } },
{ "2kx2k_marco_",			{ 2048, 2048, 2048, 2048, 1024, 1024, 1024, 512 },
							{ 512, 256, 50, 19, 32, 15, 7, 22 } },
{ "2kx2k_v4.0.vNDR",		{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 1024, 512, 512, 460, 256, 228, 112, 90 } },
{ "2kx2k_FASTNEURO_",		{ 2048, 1024, 1024, 1024, 512, 512, 512, 2048 },
							{ 512, 160, 15, 7, 23, 10, 4, 9 } },
{ "2kx2k_FASTNEURO2_",		{ 2048, 1024, 1024, 1024, 1024, 512, 512, 512 },
							{ 512, 160, 80, 32, 15, 23, 10, 4 } },
{ "2kx2k_FASTNEURO3_",		{ 2048, 1024, 1024, 1024, 512, 512, 512, 512 },
							{ 512, 160, 80, 32, 47, 23, 10, 4 } },
{ "2k_CARDIO_BINNED_1024_",	{ 2048, 1024, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 256, 160, 80, 128, 80, 64, 39 } },
{ "2k_CARDIO_BINNED_v",		{ 2048, 1024, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 640, 320, 160, 80, 160, 80, 79, 39 } },
{ "2k_NEURO_s2K_",			{ 2048, 2048, 2048, 2048, 1024, 1024, 1024, 1024 },
							{ 1024, 200, 100, 50, 320, 160, 80, 32 } },
};*/

#endif // CAMERA_H_
