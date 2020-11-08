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

int existDir(char *dirName);
int checkDir(char *dirName);
int SM_take_Live(signed short *live_img, int numChannels, int cdsF, int images, int layers, int stripes, int factor, char *cameraname, int file_img_w, int horizontal_bin, int quadFlag, int rotateFlag, int bad_pix_index,
	int ndrFlag, int num_NDRsub_frames, int segmented);
int SM_take_Dark(signed short *dk_img, int images, int numChannels, int cdsF, int layers, int stripes, double frame_interval, char *cameraname, int file_img_w, int horizontal_bin, int quadFlag, int rotateFlag, int bad_pix_index,
	int ndrFlag, int num_NDRsub_frames, int segmented);
int initLive(int numChannels, int cdsF, int images, int layers, int stripes, char *cameraname, int quadFlag, int rotateFlag, int bad_pix_index);
int SM_take_tb(int numChannels, char *file_name, int cdsF, int NDR_flag, int images, int layers, int stripes, double frame_interval, int factor, char *cameraname, int file_img_w, int horizontal_bin, int BNC_ratio,
	int ExtTriggerFlag, int preTrigger_frames, int darkFlag, int quadFlag, int rotateFlag, int bad_pix_index, int segmented);
int SM_take_disk(int numChannels, char *file_name, int cdsF, int images, int layers, int stripes, double frame_interval, int factor, char *cameraname, int file_img_w, int horizontal_bin, int BNC_ratio,
	int ExtTriggerFlag, int preTrigger_frames, int quadFlag, int rotateFlag, int bad_pix_index, int segmented);


