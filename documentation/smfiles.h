/*** NI start ***/
#define _USE_NI						1
#ifdef _USE_NI
// add NIDAQmx.lib in Project>>Setting>>Link for static link

//#define DLL_FUNCTIONS_N				7

#define DAQmxCreateTaskFN			0
#define DAQmxCreateDOChanFN			1
#define DAQmxStartTaskFN			2
#define DAQmxWriteDigitalLinesFN	3
#define DAQmxGetExtendedErrorInfoFN	4
#define DAQmxStopTaskFN				5
#define DAQmxClearTaskFN			6

int NI_setOutZero(void);
int NI_openShutter(void);
//int NI_pulses(void);
//int NI_stim_down(void);
int NI_setMono_offline(double in_vol, double out_vol, double w_vol);
#endif
/*** NI end ***/


int SM_take_Live(signed short *live_img, int numChannels, int cdsF, int images, int layers, int stripes, int factor, char *cameraname, 
	int file_img_w, int horizontal_bin, int quadFlag, int rotateFlag, int bad_pix_index,
	int ndrFlag, int num_NDRsub_frames, int segmented);
int SM_take_Dark(signed short *dk_img, int images, int numChannels, int cdsF, int layers, int stripes, double frame_interval, 
	char *cameraname, int file_img_w, int horizontal_bin, int quadFlag, int rotateFlag, int bad_pix_index,
	int ndrFlag, int num_NDRsub_frames, int segmented);
int initLive(int numChannels, int cdsF, int images, int layers, int stripes, char *cameraname, int quadFlag, int rotateFlag, int bad_pix_index);
int SM_take_tb(int numChannels, char *file_name, int cdsF, int NDR_flag, int images, int layers, int stripes, double frame_interval, int factor, 
	char *cameraname, int file_img_w, int horizontal_bin, int BNC_ratio,
	int ExtTriggerFlag, int preTrigger_frames, int darkFlag, int quadFlag, int rotateFlag, int bad_pix_index, int segmented);
int SM_take_disk(int numChannels, char *file_name, int cdsF, int images, int layers, int stripes, double frame_interval, int factor, 
	char *cameraname, int file_img_w, int horizontal_bin, int BNC_ratio,
	int ExtTriggerFlag, int preTrigger_frames, int quadFlag, int rotateFlag, int bad_pix_index, int segmented);

/* 
static int initAcquire(int numChannels, int cdsF, int layers, int stripes, char *cameraname, int quadFlag, int rotateFlag, int bin, int factor, int superframe);
int initLive(int numChannels, int cdsF, int images, int layers, int stripes, char *cameraname, int quadFlag, int rotateFlag, int bad_pix_index);
void multiPixReadAv(signed short *img);
void rearrange_File(int images, int file_id, int numChannels, int quadFlag, int file_img_w, int segmented);
void stripAD_data(unsigned short int *image_data, signed short int *BNC_data, char *cameraType, int file_height, int frame_width, int frame_height, int loops, int cds)
void frameRepair(unsigned short int *image_buffer, int bad_pix_index, int twoKFlag, unsigned int image_width, unsigned int file_width, unsigned int file_height, int horizontal_bin, int superFrame, int file_img_w, int binFlag, int segmented)
void saveNI_data(long num_frames, char filename[MAX_PATH], short int numBNC_chan, short int BNC_ratio, float64 *NI_data, int delay, int isTwoK)
void saveBNC_data(long num_frames, char filename[MAX_PATH], short int numBNC_chan, short int BNC_ratio, signed short int *BNC_data, char *cameraType, int cam_num_col, int stripes)
void saveBNC_block(long num_frames, int handle, long tt_frames_flag, short int numBNC_chan, short int BNC_ratio, signed short int *BNC_data, char *cameraType, long lastDiskFrame, int cam_num_col, int stripes)
void SM_makeHeader(int images, int numExposures, int rows, int columns, char *camera, double exposure)

void subtractCDS(unsigned short int *image_data, int loops, unsigned int width, unsigned int height, unsigned int length, int factor, int QUAD, int TWOK, int num_pdvChan)
int makeLookUpTable(unsigned int *Lut, int image_width, int image_height, int file_width, int file_height, int stripes, int layers, int factor, int QUAD, int CDS, int ROTATE, int TWOK)
void frame_deInterleave(int length, unsigned *lookuptable, unsigned short *old_images, unsigned short *new_images)


*****for NI acquisition***
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)


*/
