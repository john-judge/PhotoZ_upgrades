#include "NIDAQmx.h"
#include "edtinc.h"
#include <fcntl.h>
#include <malloc.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <commctrl.h>

#include <sm.h>
#include "smDirectX.h"
#include "smfiles.h"
#include "smsecure.h"

#define DAVINCIBITFLIP

#define O_MODE (O_BINARY | O_CREAT | O_TRUNC | O_SEQUENTIAL | O_RDWR)
#define S_MODE S_IWRITE

extern PdvDev *pdv_pt[MAXCHANNELS], *pdv_p, *pcd_p;
extern signed short int *image_data, *single_img_data;
extern int config_num_col, config_num_row, live_factor, shutter_wait, stim_delay, curConfig, multi_pix_read, multi_pix_av, fd_no_deinterleave_flag;
extern HWND main_window_handle;
extern int bad_row_ar[10][5], bad_col_ar[10][5], bad_pixel_ar[300][9];
extern char config_list[8][50];
extern char home_dir[MAX_PATH];
extern int SFC_flag;

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);
void stripAD_data(unsigned short int *image_data, signed short int *BNC_data, char *cameraType, int file_height,
	int frame_width, int frame_height, int loops, int cds);
//void SM_convertDiskData(long num_frames, char *DISKfilename, int f_handle, int handleBNC, char *cameraType, short int numBNC_chan, short int BNC_ratio, int file_width, int file_height,
//	int frame_width, int frame_height, int stripes, int layers, int factor, int quadFlag, int cds, int rotateFlag, int twoKFlag);
static void saveNI_data(long num_frames, char filename[MAX_PATH], short int numBNC_chan, short int BNC_ratio, float64 *NI_data, int delay, int isTwoK);
static void saveBNC_data(long num_frames, char filename[MAX_PATH], short int numBNC_chan, short int BNC_ratio, signed short int *BNC_data, char *cameraType, int cam_num_col, int stripes);
static void saveBNC_block(long num_frames, int handle, long tt_frames_flag, short int numBNC_chan, short int BNC_ratio, signed short int *BNC_data, char *cameraType, long lastDiskFrame, int cam_num_col, int stripes);
static void SM_makeHeader(int images, int exposures, int rows, int columns, char *camera, double exposure);

static char FitsHeader[36][80];
static char FitsEnder[36*80];
int lut_size;
unsigned int *lut = (unsigned int *)NULL;
static unsigned short int *image_buffer = (unsigned short int *)NULL;
static int cdsFlag = FALSE, OffCenter_crop = FALSE, numBNC_chan = 4, FastDiskFlag; // , skip_chan = 0;
static char fastD_drive[10];
SYSTEMTIME acq_end_sysTime;
char multi_file_names[50][_MAX_FNAME];

#ifdef DAVINCIBITFLIP
#define SB(P,B) (((unsigned __int64)1)<<(((int)(P-'A')*16)+B+2))
//unsigned short int imageIn[maxLength];
//unsigned __int64 imageOut[maxLength / 4];
static unsigned int *bitlut = (unsigned int *)NULL;
unsigned __int64 bitLut[0x40000];
unsigned __int64 bitPattern[4][16] = {
	{ 0x00000000,0x00000000,SB('A',12),SB('B',13),SB('A', 0),SB('B',11),SB('A',11),SB('B',12),SB('B',10),SB('B', 9),SB('A',10),SB('B', 7),SB('B', 6),SB('B', 5),SB('A', 9),SB('B', 8) },
	{ 0x00000000,0x00000000,SB('B', 3),SB('B', 2),SB('B', 1),SB('B', 0),SB('A', 8),SB('B', 4),SB('C', 6),SB('C', 5),SB('C', 4),SB('C',13),SB('C', 3),SB('C', 1),SB('C', 7),SB('C', 2) },
	{ 0x00000000,0x00000000,SB('C', 0),SB('C',11),SB('D',12),SB('D',11),SB('D',13),SB('C',12),SB('D',10),SB('C', 9),SB('D', 8),SB('D', 7),SB('D', 6),SB('D', 5),SB('D', 9),SB('C',10) },
	{ 0x00000000,0x00000000,SB('D', 3),SB('D', 4),SB('C', 8),SB('D', 0),SB('D', 2),SB('D', 1),SB('A', 3),SB('A',13),SB('A', 4),SB('A', 7),SB('A', 2),SB('A', 1),SB('A', 5),SB('A', 6) } };
unsigned int bitMask[16] = { 0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000 };
#endif

extern int AD_flag, NI_flag, MNC_flag, Bit16_flag, pcdFlag, num_trials;
int NI_add_frames = 0, shutter_add_frames;

//for monochromator
extern int ratio_type, frame_per_ratio;
extern double MNC_in1_vol, MNC_in2_vol, MNC_out1_vol, MNC_out2_vol, MNC_w1_vol, MNC_w2_vol;

static int	horizontalBin, binFlag = 0, twoKFlag = FALSE, Falcon_flag = false, superFrame = 1, loops = 1, image_layers, image_stripes; // , twoK_start_missed = 0;
static unsigned int frame_width, frame_height, frame_length,image_width, image_height, image_length, file_length;
unsigned int file_width, file_height;

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

static int initAcquire(int numChannels, int cdsF, int layers, int stripes, char *cameraname, int quadFlag, int rotateFlag, int bin, int factor, int superframe)
{
	char *p;

	numBNC_chan = 4;
	if (bin <= 1) {
		horizontalBin = 1;
		binFlag = 0;
	}
	else {
		horizontalBin = bin;
		binFlag = 1;
	}
	superFrame = superframe;
	if (NI_flag) {
		if (strstr(cameraname, "-NIU8"))
			numBNC_chan = 8;
		else
			numBNC_chan = 4;
	}

	image_layers = layers;
	image_stripes = stripes;
	FastDiskFlag = 0;
	Falcon_flag = false;
	if (strstr(cameraname, "DM64"))
		Falcon_flag = true;
	else if (strstr(cameraname, "DM2K") || strstr(cameraname, "DM1K")) {
		twoKFlag = TRUE;
		if (numChannels > 2)
			image_stripes <<= 1;
		image_layers <<= 1;
		if (strstr(cameraname, "-OCR"))
			OffCenter_crop = 1;
		if (p = strstr(cameraname, "-FASTD")) {
			FastDiskFlag = 1;
			strcpy(fastD_drive, (p + 7));
			*(fastD_drive + 1) = '\0';
			char tmp_str[256];
			strcpy(tmp_str, fastD_drive);
			if (!existDir(strcat(tmp_str, ":\\"))) {
				MessageBox(main_window_handle, "The PCI-SSD drive was not specified properly. It should be -FASTD-H where H (or other letter) is the name of the drive", "message", MB_OK);
				return FALSE;
			}
		}
	}
	cdsFlag = cdsF;

	if (pcdFlag != 1) {
		file_width = image_width = (frame_width = pdv_get_width(pdv_pt[0]));
		file_height = image_height = (frame_height = pdv_get_height(pdv_pt[0]));
	}
	else {
		int config_id = checkCfgLUT();
		file_width = image_width = (frame_width = get_pcdWH(config_id, 0));
		file_height = image_height = (frame_height = get_pcdWH(config_id, 1));
	}
	file_length = (image_length = (frame_length = frame_width * frame_height * 2));

	if (twoKFlag) {
		image_width *= numChannels>>1;
		image_height *= 2;
		image_length *= 2*(numChannels >> 1);
		if (superFrame > 1) {
			file_height = (file_height / factor + 1) / superFrame - 2;
			file_length = file_width*file_height * 2;
		}
		else if (factor > 1) {
			file_height = file_height / factor;
			file_length = file_width*file_height * 2;
		}
		file_height *= 2;
		if (cdsFlag) {
			file_width /=  2 / (numChannels >> 1);
			file_length *= (numChannels >> 1);
		}
		else {
			file_width *= (numChannels >> 1);
			file_length *= 2 * (numChannels >> 1);
		}
	}
	else {
		if (cdsFlag)
			file_width = file_width / 2;
		if (factor > 1)
			file_height = file_height / factor;
		file_length = file_width*file_height * 2;
	}
	if (AD_flag) {
		if (strstr(cameraname, "DW"))
			file_height -= 1;
		else
			file_height -= 2;
		file_length = file_width*file_height * 2;
	}
//	skip_chan = 0;
	return TRUE;
}

/*
void checkCropping(int numChannels, int crop_w)
{
	if (numChannels > 2 && (crop_w <= file_width / 4)) {
		image_width /= 4;
		file_width /= 4;
		image_length /= 4;
		skip_chan = 4;
	}
	else if (crop_w <= file_width / 2) {
		image_width /= 2;
		file_width /= 2;
		image_length /= 2;
		skip_chan = 2;
	}
	file_length = file_width * file_height * 2;
}*/

int initLive(int numChannels, int cdsF, int images, int layers, int stripes, char *cameraname, int quadFlag, int rotateFlag, int bad_pix_index)
{
	char    badPixFname[_MAX_FNAME];
	FILE	*fp_bad;
	unsigned int image_height_sv;

	if (!initAcquire(numChannels, cdsF, layers, stripes, cameraname, quadFlag, rotateFlag, 1, 1, 1))
		return FALSE;
	loops = 1;
	if (image_buffer != NULL)
		_aligned_free(image_buffer);
	image_buffer = (unsigned short *)_aligned_malloc(file_length*superFrame, 2);
	if (lut != NULL)
		_aligned_free(lut);
	lut_size = file_length * superFrame * sizeof(lut[0]);
	lut = (unsigned int *)_aligned_malloc(lut_size, 2);

	image_height_sv = image_height;
	if (AD_flag) {
		if (strstr(cameraname, "DW"))
			image_height -= 1;
		else
			image_height -= 2;
	}
	makeLookUpTable(lut, image_width, image_height, file_width, file_height, image_stripes, image_layers, superFrame, quadFlag, cdsFlag, rotateFlag, twoKFlag);
	image_height = image_height_sv;

	if (pcdFlag != 1) {
		for (int i = 0; i < numChannels; ++i)
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row*live_factor);
	}

	//	correct bad rows and cols for DM2K or bad pixels for 128X - read map
	if (bad_pix_index) {
		unsigned int row = 0, tmp, bad_x, bad_y;
		int xx[] = { 0, -1, -1, -1, 0, 0, 1, 1, 1 };
		int yy[] = { 0, -1, 0, 1, -1, 1, -1, 0, 1 };
		char buf[256], tempStr[100];

		memset(bad_pixel_ar, 0, sizeof(bad_pixel_ar));
		sprintf(badPixFname, ".\\SMSYSDAT\\bad_pixels%d.txt", bad_pix_index);
		if (fp_bad = fopen(badPixFname, "r")) {
			if (!fgets(buf, 255, fp_bad))
				goto exit_pix;
			sscanf(buf, "%s", tempStr);
			while (strcmp(tempStr, "END")) {
				if (row > sizeof(bad_pixel_ar) / sizeof(bad_pixel_ar[0]))
					break;
				tmp = atoi(tempStr) - 1;
				if ((tmp / file_width >= (file_width - file_height) / 2 + file_height) || (tmp / file_width < (file_width - file_height) / 2))
					goto OUT_PIX;
				bad_pixel_ar[row][0] = (tmp / file_width - (file_width - file_height) / 2)*file_width + (tmp % file_width) + 1;

				bad_x = ((bad_pixel_ar[row][0] - 1) % file_width);
				bad_y = (bad_pixel_ar[row][0] - 1) / file_width;
				bad_x = min(max(0, bad_x), file_width - 1);
				bad_y = min(max(0, bad_y), file_height - 1);

				int mm, l;
				for (mm = 1; mm <= 8; mm++) {
					if ((bad_x + xx[mm] < 0) || (bad_x + xx[mm] >= file_width) || (bad_y + yy[mm] < 0) || (bad_y + yy[mm] >= file_height))
						goto BAD_PIX;
					for (l = 0; l < sizeof(bad_pixel_ar) / sizeof(bad_pixel_ar[0]); l++) {
						if (bad_pixel_ar[l][mm] == 0)
							break;
						if (file_width*(bad_y + yy[mm]) + bad_x + xx[mm] + 1 == bad_pixel_ar[l][0])
							goto BAD_PIX;
					}
					bad_pixel_ar[row][mm] = file_width * (bad_y + yy[mm]) + bad_x + xx[mm] + 1;
				BAD_PIX:
					mm = mm;
				}

				row++;
			OUT_PIX:
				if (!fgets(buf, 255, fp_bad))
					goto exit_pix;
				sscanf(buf, "%s", tempStr);
			}
		exit_pix:
			fclose(fp_bad);
			for (unsigned int i = 0; i < row; i++) {
				for (int mm = 1; mm <= 8; mm++) {
					for (unsigned int l = 0; l < row; l++) {
						if (bad_pixel_ar[i][mm] == bad_pixel_ar[l][0]) {
							bad_pixel_ar[i][mm] = 0;
							break;
						}
					}
				}
			}
		}
	}
	else if (twoKFlag) {//check for bad_row, bad_col files
		char tempStr[256];
		char buf[256];
		int line;
		char *badLineFmt = "%d %d %d %d %d\n";

		sprintf(badPixFname, ".\\SMSYSDAT\\bad_rows.txt");
		if (fp_bad = fopen(badPixFname, "r")) {
			if (!fgets(buf, 255, fp_bad))
				goto exit_row;
			line = 0;
			if (!fgets(buf, 255, fp_bad))
				goto exit_row;
			sscanf(buf, "%s", tempStr);
			while (strcmp(tempStr, "END")) {
				sscanf(buf, badLineFmt, &bad_row_ar[line][0], &bad_row_ar[line][1], &bad_row_ar[line][2], &bad_row_ar[line][3], &bad_row_ar[line][4]);
				if (!fgets(buf, 255, fp_bad))
					goto exit_row;
				sscanf(buf, "%s", tempStr);
				line++;
			}
		exit_row:
			fclose(fp_bad);
		}

		sprintf(badPixFname, ".\\SMSYSDAT\\bad_cols.txt");
		if (fp_bad = fopen(badPixFname, "r")) {
			if (!fgets(buf, 255, fp_bad))
				goto exit_col;
			line = 0;
			if (!fgets(buf, 255, fp_bad))
				goto exit_col;
			sscanf(buf, "%s", tempStr);
			while (strcmp(tempStr, "END")) {
				sscanf(buf, badLineFmt, &bad_col_ar[line][0], &bad_col_ar[line][1], &bad_col_ar[line][2], &bad_col_ar[line][3], &bad_col_ar[line][4]);
				if (!fgets(buf, 255, fp_bad))
					goto exit_col;
				sscanf(buf, "%s", tempStr);
				line++;
			}
		exit_col:
			fclose(fp_bad);
		}
	}
	return TRUE;
}

void multiPixReadAv(signed short *img)
{
	if (multi_pix_av) {
		int img_jump = file_width / multi_pix_read;
		signed short *scr_pt = img;
		signed short *dst_pt = img;
		long pix_sum;
		for (unsigned int l = 0; l < file_height; l++) {
			for (int n = 0; n < img_jump; n++) {
				pix_sum = 0;
				for (int m = 0; m < multi_pix_read; m++)
					pix_sum += *(scr_pt + img_jump * m);
				*dst_pt = (signed short)(pix_sum / multi_pix_read);
				scr_pt++;
				dst_pt++;
			}
			scr_pt += img_jump * (multi_pix_read - 1);
			dst_pt += img_jump * (multi_pix_read - 1);
		}
	}
}

int SM_take_Live(signed short *live_img, int numChannels, int cdsF, int images, int layers, int stripes, int factor, char *cameraname, int file_img_w, int hbin, int quadFlag, int rotateFlag, 
	int bad_pix_index, int ndrFlag, int num_NDRsub_frames, int segmented)
{
	int i;
	u_char *data_ptr[MAXCHANNELS] = { NULL, NULL, NULL, NULL };
	u_char *image_ptr;
	int times[MAXCHANNELS][2];
	signed short int *BNC_data = (signed short int *)NULL;
	signed short int *live_img_ptr;
	char buf[200];

	initAcquire(numChannels, cdsF, layers, stripes, cameraname, quadFlag, rotateFlag, hbin, factor, 1);
	if (ndrFlag)
		loops = max(2, (num_NDRsub_frames+1) / factor  + (((num_NDRsub_frames+1) % factor )>0));
	else
		loops = (images + cdsFlag ) / (factor * superFrame) + (((images + cdsFlag) % (factor * superFrame))>0);
	if (AD_flag)
		BNC_data = (signed short *)_aligned_malloc(loops*image_width * 2 * sizeof(BNC_data[0]), 2);
	if (!file_img_w)
		file_img_w = file_width;
	for (i = 0; i < numChannels; ++i)
		data_ptr[i] = (u_char *)live_img + i*frame_length / factor;

	if (pcdFlag) {
		int bufsize = image_length; // image_length;
		int numbufs = 4;
		PdvDev *pdv_p_sv = pdv_p;
	//	if (pcdFlag == 2)
			pdv_p = pcd_p;
		edt_reg_write(pdv_p, SSD16_CHEN, 0x0);
		edt_flush_channel(pdv_p, 0);
		if (edt_configure_ring_buffers(pdv_p, bufsize, numbufs, EDT_READ, NULL) != -1)
		{
			edt_start_buffers(pdv_p, numbufs);
			edt_reg_write(pdv_p, SSD16_CHEN, 0x0001);

			image_ptr = edt_wait_for_buffers(pdv_p, 1);
			memcpy(data_ptr[0], image_ptr, frame_length);
			data_ptr[0] += image_length;
		}
		else 
			MessageBox(main_window_handle, "edt_configure_ring_buffers failed", "message", MB_OK);
		pdv_p = pdv_p_sv;
	}
	else {
		for (i = 0; i < numChannels; ++i) {
			pdv_flush_fifo(pdv_pt[i]);
			pdv_multibuf(pdv_pt[i], 4);	//has to be 4
		}

		if (numChannels > 1 && omp_get_num_procs() >= numChannels) {
			if (ndrFlag) {
				int mismatchFlag;

				for (int n = 0; n < 20; n++) {
					omp_set_num_threads(numChannels);
#pragma omp parallel private(image_ptr, buf)
					{
						int thread_id;

						thread_id = omp_get_thread_num();
						pdv_start_images(pdv_pt[thread_id], 4);
						image_ptr = pdv_wait_image_timed_raw(pdv_pt[thread_id], (unsigned int *)&times[thread_id][0], 1);
						pdv_start_image(pdv_pt[thread_id]);
						for (int i = 0; i < factor; ++i) {
							memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
							image_ptr += frame_length / factor;
							data_ptr[thread_id] += image_length / factor;
						}
						mismatchFlag = 1;
						if ((numChannels < 4 && (abs(times[1][1] - times[0][1]) < 100000)) || ((abs(times[1][1] - times[0][1]) < 100000) && (abs(times[2][1] - times[0][1]) < 100000) && (abs(times[3][1] - times[0][1]) < 100000))) {
							for (int j = 1; j < loops; j++) {
								image_ptr = pdv_wait_image_raw(pdv_pt[thread_id]);
								pdv_start_image(pdv_pt[thread_id]);
							}
							for (int i = 0; i < factor; ++i) {
								memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
								image_ptr += frame_length / factor;
								data_ptr[thread_id] += image_length / factor;
							}
							mismatchFlag = 0;
						}
					}

					if (!mismatchFlag) {
						int min_ndr = 0;
						unsigned int single_img_len = image_length / (2 * factor);
						long ndr_jump = single_img_len * (factor + num_NDRsub_frames % factor);
						live_img_ptr = (signed short int *)live_img;
						for (int j = 0; j < (int)(single_img_len); live_img_ptr++, j++) {
							*live_img_ptr = *(live_img_ptr + ndr_jump) - *live_img_ptr;
							if (min_ndr > *live_img_ptr)
								min_ndr = *live_img_ptr;
						}
						if (min_ndr > -200)
							break;
					}
					for (i = 0; i < numChannels; ++i)
						data_ptr[i] = (u_char *)live_img + i * frame_length / factor;
					for (i = 0; i < numChannels; ++i) {
						pdv_flush_fifo(pdv_pt[i]);
						pdv_multibuf(pdv_pt[i], 4);	//has to be 4
					}
					//				sprintf(buf, "n: %d, Timging Diff: %d %d %d\n", n, (times[1][1] - times[0][1]) / 1000, (times[2][1] - times[0][1]) / 1000, (times[3][1] - times[0][1]) / 1000);
					//				OutputDebugString(buf);
				}
			}
			else {
				for (int m = 0; m < 20; m++) {
					omp_set_num_threads(numChannels);
#pragma omp parallel private(image_ptr, buf)
					{
						int thread_id;

						thread_id = omp_get_thread_num();
						pdv_start_images(pdv_pt[thread_id], loops + (factor > 1 ? 1 : 0));
						for (int j = 0; j < loops; j++) {
							image_ptr = pdv_wait_image_timed_raw(pdv_pt[thread_id], (unsigned int *)&times[thread_id][0], 1);
							//			image_ptr = pdv_wait_image_raw(pdv_pt[thread_id]);
							for (int i = 0; i < factor; ++i) {
								memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
								image_ptr += frame_length / factor;
								data_ptr[thread_id] += image_length / factor;
							}
						}
					}
					if (numChannels < 4 && (abs(times[1][1] - times[0][1]) < 100000))
						break;
					else if ((abs(times[1][1] - times[0][1]) < 100000) && (abs(times[2][1] - times[0][1]) < 100000) && (abs(times[3][1] - times[0][1]) < 100000))
						break;
					//				sprintf(buf, "**** %d: %d %d %d\n", m, (times[1][1] - times[0][1]) / 1000, (times[2][1] - times[0][1]) / 1000, (times[3][1] - times[0][1]) / 1000);
					//				OutputDebugString(buf);
					for (int i = 0; i < numChannels; ++i)
						data_ptr[i] = (u_char *)live_img + i * frame_length / factor;
					if (factor > 1) {
						for (i = 0; i < numChannels; ++i) {
							pdv_flush_fifo(pdv_pt[i]);
							pdv_multibuf(pdv_pt[i], 4);	//has to be 4
						}
					}
				}
			}
		}
		else {
			pdv_start_images(pdv_pt[0], loops + (factor > 1 ? 1 : 0));
			for (int j = 0; j < loops; j++) {
				image_ptr = pdv_wait_image_raw(pdv_pt[0]);
				memcpy(data_ptr[0], image_ptr, frame_length);
				data_ptr[0] += image_length;
			}
			if (Falcon_flag) {
				unsigned __int64 *long_pt = (unsigned __int64 *)live_img;
				unsigned short int *short_pt0, *short_pt1, *short_pt2, *short_pt3;
				short_pt0 = (unsigned short int *)long_pt;
				short_pt1 = short_pt0 + 1;
				short_pt2 = short_pt1 + 1;
				short_pt3 = short_pt2 + 1;
				for (i = 0; i < (signed)frame_length >> 2; i++) {
					*long_pt++ = (bitLut[*short_pt0] | bitLut[(*short_pt1) | 0x10000L] | bitLut[(*short_pt2) | 0x20000L] | bitLut[(*short_pt3) | 0x30000L]) & 0x3fff3fff3fff3fffL;
					short_pt0 += 4;
					short_pt1 += 4;
					short_pt2 += 4;
					short_pt3 += 4;
				}
			}
			if (ndrFlag > 1) {
				memcpy(live_img, live_img + file_length * (ndrFlag - 1)/2, file_length);
			}
		}
	}

	if (AD_flag) {
		if (factor > 1) {
			frame_height /= factor;
			factor = 1;
			loops = images;
		}
		stripAD_data((unsigned short *)live_img, BNC_data, cameraname, file_height, frame_width, frame_height, loops, cdsFlag);
		image_height = file_height;
		image_length = image_width*image_height * 2;
	}
	if (factor > 1) {
		image_height = file_height;
		image_length = image_width*image_height * 2;
		factor = 1;
		loops = (images + cdsFlag) / superFrame + (((images + cdsFlag) % superFrame)>0);
	}

	if (Bit16_flag) {
		unsigned short *img_ptr = (unsigned short *)live_img;
		for (i = 0; i < (int)(image_length*loops>>1); i++)
			*img_ptr++ = (*img_ptr >> 2);
	}

	if (cdsFlag)
		subtractCDS((unsigned short *)live_img, loops, image_width, image_height, image_length, factor, quadFlag, twoKFlag, numChannels);
	if (cdsFlag)
		--loops;

	data_ptr[0] = (u_char *)live_img;
	unsigned int file_length_sv = file_length;
	for (int j = 0, k = 0; j < loops; ++j, k += superFrame) {
		file_length = file_length_sv;
		frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_ptr[0], image_buffer);
		frameRepair(image_buffer, bad_pix_index, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
		file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
		multiPixReadAv((signed short *)image_buffer);
		memcpy(data_ptr[0], (void *)image_buffer, (k <= images ? superFrame : images % superFrame)*file_length);
		data_ptr[0] += image_length;
	}
	if (AD_flag) {
		if (BNC_data != NULL)
			_aligned_free(BNC_data);
		BNC_data = NULL;
	}
	return TRUE;
}

extern int runPHT_flag;
int SM_take_Dark(signed short *dk_img, int images, int numChannels, int cdsF, int layers, int stripes, double frame_interval, char *cameraname, int file_img_w, int hbin, int quadFlag, int rotateFlag,
	int bad_pix_index, int ndrFlag, int num_NDRsub_frames, int segmented)
{
	int i;
	u_char *data_ptr[MAXCHANNELS] = { NULL, NULL, NULL, NULL };
	u_char *image_ptr;
	signed short *dark_imgs, *dk_ptr;
	int times[MAXCHANNELS][2];
	signed short int *BNC_data = (signed short int *)NULL;
	signed short int *dk_img_ptr;
	char buf[200];
	long dataLength;
	int factor;

	superFrame = 1;
//	images = 16;
	if (numChannels > 1)
		factor = 10 * (int)(1024 / config_num_row);
	else
		factor = 20; // (int)max(20, 20 * (1 + cdsF) / frame_interval);
	if (pcdFlag != 1) {
		for (i = 0; i < numChannels; ++i) {
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row*factor);
			if (!pcdFlag) {
				pdv_flush_fifo(pdv_pt[i]);
				pdv_multibuf(pdv_pt[i], 4);	//has to be 4
			}
		}
	}
	initAcquire(numChannels, cdsF, layers, stripes, cameraname, quadFlag, rotateFlag, hbin, factor, 1);
	loops = (images + cdsFlag) / (factor * superFrame) + (((images + cdsFlag) % (factor * superFrame))>0);
	if (AD_flag)
		BNC_data = (signed short *)_aligned_malloc(loops*image_width * 2 * sizeof(BNC_data[0]), 2);
	if (!file_img_w)
		file_img_w = file_width;
	dataLength = (unsigned long long)(loops + 1)*(unsigned long long)image_length;
	dark_imgs = (signed short *)_aligned_malloc(dataLength, 2);
	for (i = 0; i < numChannels; ++i)
		data_ptr[i] = (u_char *)dark_imgs + i * frame_length / factor;

	if (numChannels > 1 && omp_get_num_procs() >= numChannels) {
		if (ndrFlag) {
			int mismatchFlag;

			for (int n = 0; n < 20; n++) {
				omp_set_num_threads(numChannels);
#pragma omp parallel private(image_ptr, buf)
				{
					int thread_id;

					thread_id = omp_get_thread_num();
					pdv_start_images(pdv_pt[thread_id], 4);
					image_ptr = pdv_wait_image_timed_raw(pdv_pt[thread_id], (unsigned int *)&times[thread_id][0], 1);
					pdv_start_image(pdv_pt[thread_id]);
					for (int i = 0; i < factor; ++i) {
						memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
						image_ptr += frame_length / factor;
						data_ptr[thread_id] += image_length / factor;
					}
					mismatchFlag = 1;
					if ((numChannels < 4 && (abs(times[1][1] - times[0][1]) < 100000)) || ((abs(times[1][1] - times[0][1]) < 100000) && (abs(times[2][1] - times[0][1]) < 100000) && (abs(times[3][1] - times[0][1]) < 100000))) {
						for (int j = 1; j < loops; j++) {
							image_ptr = pdv_wait_image_raw(pdv_pt[thread_id]);
							pdv_start_image(pdv_pt[thread_id]);
						}
						for (int i = 0; i < factor; ++i) {
							memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
							image_ptr += frame_length / factor;
							data_ptr[thread_id] += image_length / factor;
						}
						mismatchFlag = 0;
					}
				}

				if (!mismatchFlag) {
					int min_ndr = 0;
					unsigned int single_img_len = image_length / (2 * factor);
					long ndr_jump = single_img_len * (factor + num_NDRsub_frames % factor);
					dk_img_ptr = (signed short int *)dark_imgs;
					for (int j = 0; j < (int)(single_img_len); dk_img_ptr++, j++) {
						*dk_img_ptr = *(dk_img_ptr + ndr_jump) - *dk_img_ptr;
						if (min_ndr > *dk_img_ptr)
							min_ndr = *dk_img_ptr;
					}
					if (min_ndr > -200)
						break;
				}
				for (i = 0; i < numChannels; ++i)
					data_ptr[i] = (u_char *)dark_imgs + i * frame_length / factor;
				for (i = 0; i < numChannels; ++i) {
					pdv_flush_fifo(pdv_pt[i]);
					pdv_multibuf(pdv_pt[i], 4);	//has to be 4
				}
				//				sprintf(buf, "n: %d, Timging Diff: %d %d %d\n", n, (times[1][1] - times[0][1]) / 1000, (times[2][1] - times[0][1]) / 1000, (times[3][1] - times[0][1]) / 1000);
				//				OutputDebugString(buf);
			}
		}
		else {
			for (int m = 0; m < 20; m++) {
				omp_set_num_threads(numChannels);
#pragma omp parallel private(image_ptr, buf)
				{
					int thread_id;

					thread_id = omp_get_thread_num();
					pdv_start_images(pdv_pt[thread_id], loops + (factor>1 ? 1 : 0));
					for (int j = 0; j < loops; j++) {
						image_ptr = pdv_wait_image_timed_raw(pdv_pt[thread_id], (unsigned int *)&times[thread_id][0], 1);
						//			image_ptr = pdv_wait_image_raw(pdv_pt[thread_id]);
						for (int i = 0; i < factor; ++i) {
							memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
							image_ptr += frame_length / factor;
							data_ptr[thread_id] += image_length / factor;
						}
					}
				}
				if (numChannels < 4 && (abs(times[1][1] - times[0][1]) < 100000))
					break;
				else if ((abs(times[1][1] - times[0][1]) < 100000) && (abs(times[2][1] - times[0][1]) < 100000) && (abs(times[3][1] - times[0][1]) < 100000))
					break;
				//				sprintf(buf, "**** %d: %d %d %d\n", m, (times[1][1] - times[0][1]) / 1000, (times[2][1] - times[0][1]) / 1000, (times[3][1] - times[0][1]) / 1000);
				//				OutputDebugString(buf);
				for (int i = 0; i < numChannels; ++i)
					data_ptr[i] = (u_char *)dark_imgs + i * frame_length / factor;
				if (factor > 1) {
					for (i = 0; i < numChannels; ++i) {
						pdv_flush_fifo(pdv_pt[i]);
						pdv_multibuf(pdv_pt[i], 4);	//has to be 4
					}
				}
			}
		}
	}
	else {
		pdv_start_images(pdv_pt[0], loops + (factor>1 ? 1 : 0));
		for (int j = 0; j < loops; j++) {
			image_ptr = pdv_wait_image_raw(pdv_pt[0]);
			memcpy(data_ptr[0], image_ptr, frame_length);
			data_ptr[0] += image_length;
		}
	}

	if (AD_flag) {
		if (factor > 1) {
			frame_height /= factor;
			factor = 1;
			loops = images;
		}
		stripAD_data((unsigned short *)dark_imgs, BNC_data, cameraname, file_height, frame_width, frame_height, loops, cdsFlag);
		image_height = file_height;
		image_length = image_width * image_height * 2;
	}
	if (factor > 1) {
		image_height = file_height;
		image_length = image_width * image_height * 2;
		factor = 1;
		loops = (images + cdsFlag) / superFrame + (((images + cdsFlag) % superFrame)>0);
	}

	if (Bit16_flag) {
		unsigned short *img_ptr = (unsigned short *)dark_imgs;
		for (i = 0; i < (int)(image_length*loops>>1); i++)
			*img_ptr++ = (*img_ptr >> 2);
	}

	if (cdsFlag)
		subtractCDS((unsigned short *)dark_imgs, loops, image_width, image_height, image_length, factor, quadFlag, twoKFlag, numChannels);
	if (cdsFlag)
		--loops;

	data_ptr[0] = (u_char *)dark_imgs;
	unsigned int file_length_sv = file_length;
	for (int j = 0, k = 0; j < loops; ++j, k += superFrame) {
		file_length = file_length_sv;
		frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_ptr[0], image_buffer);
		frameRepair(image_buffer, bad_pix_index, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
		file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
		multiPixReadAv((signed short *)image_buffer);
		memcpy(data_ptr[0], (void *)image_buffer, (k <= images ? superFrame : images % superFrame)*file_length);
		data_ptr[0] += image_length;
	}
	if (!runPHT_flag) {
		long this_sum;
		dk_ptr = dk_img;
		for (unsigned int m = 0; m < file_length >> 1; m++, dk_ptr++) {
			dk_img_ptr = dark_imgs + m;
			this_sum = (long)*dk_img_ptr;
			for (int j = 1; j < loops; ++j) {
				dk_img_ptr += image_length >> 1;
				this_sum += *dk_img_ptr;
			}
			*dk_ptr = (signed short)(this_sum/loops);
		}
	}
	else
		memcpy(dk_img, dark_imgs, file_length*images);

	if (pcdFlag != 1) {
		for (int i = 0; i < numChannels; ++i)
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row*live_factor);
	}
	if (AD_flag) {
		if (BNC_data != NULL)
			_aligned_free(BNC_data);
		BNC_data = NULL;
	}
	if (dark_imgs != NULL)
		_aligned_free(dark_imgs);
	return TRUE;
}

static HWND acq_hwndPB = NULL;
static int num_steps = 30;
void createAcqProgBar()
{
	// making a progress bar
	RECT rcClient;  // Client area of parent window.
	int cyVScroll = 50;  // Height of scroll bar arrow.
	GetClientRect(main_window_handle, &rcClient);
	acq_hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE | WS_CAPTION, rcClient.left + rcClient.right / 6,
		rcClient.top + (rcClient.bottom - cyVScroll) * 2 / 5,
		rcClient.right / 4, cyVScroll,
		main_window_handle, NULL, NULL, NULL);
	SendMessage(acq_hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, num_steps+1));
	SendMessage(acq_hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
	SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
}

// this is only for slow frame rate - streaming to disk for McCormik
static int Ddisk_flag = 0;
int SM_take_disk(int numChannels, char *file_name, int cdsF, int images, int layers, int stripes, double frame_interval, int factor, char *cameraname, int file_img_w, int hbin, int BNC_ratio,
	int ExtTriggerFlag, int preTrigger_frames, int quadFlag, int rotateFlag, int bad_pix_index, int segmented)
{
	int     i, j;
	int		numFiles = 1;
	int     overrun[MAXCHANNELS], overruns[MAXCHANNELS] = { 0,0,0,0 }, timeouts[MAXCHANNELS] = { 0,0,0,0 };// , last_timeouts = 0, recovering_timeout = FALSE;
	int		useFrameFormat = FALSE;
	char    frameFormat[_MAX_FNAME] = "";
	char    tsmfname[MAXCHANNELS][_MAX_FNAME] = { "","","","" };
	int		tsmhandle[MAXCHANNELS] = { NULL, NULL, NULL, NULL };
	int		debug = FALSE;
	u_char *data_ptr[MAXCHANNELS];
	u_char *image_ptr, *data_pt;
	signed short *tmp_frame = NULL;
	unsigned long long dataLength, fileDataLength;
	unsigned long dataLeft;
	int     numExposures = 1;
	char    edt_devname[128] = "";
	char    errorBuf[256] = "";
	long triggerPos = 0;
	int times[MAXCHANNELS][2];
	char buf[200];
	int start_frames = 64;
	int esc_hit = 0;

	loops = 1;
	superFrame = 1;
	strcpy(tsmfname[0], file_name);
	if (strlen(tsmfname[0])) {
		if ((tsmhandle[0] = _open(tsmfname[0], O_MODE, S_MODE)) == -1) {
			sprintf(errorBuf, "Cannot open %s\n", tsmfname[0]);
			debug = TRUE;
			goto cleanUp;
		}
	}
	else
		return 0;

	if (pcdFlag != 1) {
		for (i = 0; i < numChannels; ++i) {
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row*factor);
			if (!pcdFlag) {
				pdv_flush_fifo(pdv_pt[i]);
				pdv_multibuf(pdv_pt[i], 4);	//has to be 4
			}
		}
	}

	initAcquire(numChannels, cdsF, layers, stripes, cameraname, quadFlag, rotateFlag, hbin, factor, 1);
	if (NI_flag)
		NI_add_frames = 2;
	else
		NI_add_frames = 0;
	if (!NI_flag && !AD_flag) {
		char BNCname[_MAX_PATH];
		char *p;
		strcpy(BNCname, file_name);
		p = strstr(BNCname, ".tsm");
		*p = '\0';
		strcat(BNCname, ".tbn");
		DeleteFile(BNCname);
	}

	loops = (images + cdsFlag + NI_add_frames) / (factor * superFrame) + (((images + cdsFlag + NI_add_frames) % (factor * superFrame)) > 0);
	if (!file_img_w)
		file_img_w = file_width;
	if (twoKFlag && ((file_img_w != file_width) || (horizontalBin > 1)))
		SM_makeHeader(images, numExposures, file_height, min(file_img_w, (signed int)file_width) / horizontalBin, cameraname, (double)frame_interval / 1000.0);
	else
		SM_makeHeader(images, numExposures, file_height, file_width, cameraname, (double)frame_interval / 1000.0);
	if (AD_flag)
		++loops;
	dataLength = 2 * (unsigned long long)image_length;
	fileDataLength = (unsigned long long)(images + 1)*(unsigned long long)file_length*superFrame;
	dataLeft = sizeof(FitsHeader) - (fileDataLength) % sizeof(FitsHeader);
	memset(&FitsEnder, 0, min(sizeof(FitsEnder), dataLeft));

	if (image_data != NULL)
		_aligned_free(image_data);
	if ((image_data = (signed short *)_aligned_malloc(dataLength, 2)) == NULL) {
		//		writeToDisk = TRUE;
		strcpy(errorBuf, "Not enough memory for Data");
	}
	if (tmp_frame != NULL)
		_aligned_free(tmp_frame);
	tmp_frame = (signed short *)_aligned_malloc(image_length / factor, 2);

	if (twoKFlag)
		for (i = 0; i < numChannels; ++i)
			data_ptr[i] = (u_char *)image_data + i * frame_length / factor;
	else
		data_ptr[0] = (u_char *)image_data;
	_lseek(tsmhandle[0], 0L, SEEK_SET);
	_write(tsmhandle[0], (char *)FitsHeader, sizeof(FitsHeader));

	createAcqProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / loops;
	char str[256];
	sprintf(str, "Acquired %d of %d images", 0, images);
	SetWindowText(acq_hwndPB, str);

	unsigned int image_height1 = image_height / factor;
	unsigned int image_length1 = image_length / factor;
	int loops1 = factor;
	long img_cnt = 0;

	Ddisk_flag = 1;
	for (i = 0; i < numChannels; ++i)
		pdv_start_images(pdv_pt[i], start_frames);

	if (numChannels > 1 && omp_get_num_procs() >= numChannels) {
		omp_set_num_threads(numChannels);
		for (j = 0; j < loops; j++) {
#pragma omp parallel private(image_ptr)
			{
				int thread_id, i;

				thread_id = omp_get_thread_num();
				image_ptr = pdv_wait_image_timed_raw(pdv_pt[thread_id], (unsigned int *)&times[thread_id][0], 1);
				//					image_ptr = pdv_wait_image_raw(pdv_pt[thread_id]);
				if ((overrun[thread_id] = (edt_reg_read(pdv_pt[thread_id], PDV_STAT) & PDV_OVERRUN)))
					++overruns[thread_id];
				for (i = 0; i < factor; ++i) {
					memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
					image_ptr += frame_length / factor;
					data_ptr[thread_id] += image_length / factor;
				}
				data_ptr[thread_id] -= image_length;
				if (!j)
					data_ptr[thread_id] += image_length / factor;
			}
			if (j < loops - start_frames + 1)
				for (i = 0; i < numChannels; ++i)
					pdv_start_image(pdv_pt[i]);
			if (cdsFlag) {
				data_pt = (u_char *)image_data + image_length;
				if (!j)
					data_pt -= image_length / factor;
				memcpy(tmp_frame, data_pt, image_length / factor);
				subtractCDS((unsigned short *)image_data, loops1, image_width, image_height1, image_length1, 1, quadFlag, twoKFlag, numChannels);
			}
			data_pt = (u_char *)image_data;
			unsigned int file_length_sv = file_length;
			for (int m = 0; m < loops1 - cdsFlag; ++m) {
				file_length = file_length_sv;
				frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_pt, image_buffer);
				frameRepair(image_buffer, bad_pix_index, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
				file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
				_write(tsmhandle[0], (void *)image_buffer, file_length);
				data_pt += image_length1;
				img_cnt++;
				if (img_cnt >= images)
					break;
			}
			file_length = file_length_sv;
			if (cdsFlag) {
				loops1 = factor + 1;
				memcpy(image_data, tmp_frame, image_length / factor);
			}
			sprintf(str, "Acquired %d of %d images", j*factor, images);
			SetWindowText(acq_hwndPB, str);
			while ((int)(j*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
			if (KEY_DOWN(VK_ESCAPE)) {
				esc_hit = 1;
				break;
			}

			//			memcpy(single_img_data, image_buffer, file_length);
//			getDispImage(0);
		}

		//test quadrant shift using time stamps
		int quad_shifts[4];
		int min_shift = 0, isShifted = 0;
		quad_shifts[0] = 0;
		sprintf(buf, "%d %d %d\n", (times[1][1] - times[0][1]) / 1000, (times[2][1] - times[0][1]) / 1000, (times[3][1] - times[0][1]) / 1000);
		OutputDebugString(buf);
	}
	else {
/*		for (j = 0; j < loops; j++) {
			for (i = 0; i < numChannels; ++i) {
				image_ptr = pdv_wait_image_raw(pdv_pt[i]);
				if ((overrun[i] = (edt_reg_read(pdv_pt[i], PDV_STAT) & PDV_OVERRUN)))
					++overruns[i];
				memcpy(data_ptr[i], image_ptr, frame_length);
				data_ptr[i] += image_length;
			}
		}*/
	}
	SM_stopAD();
	if (esc_hit) {
		images = img_cnt;
		if (twoKFlag && ((file_img_w != file_width) || (horizontalBin > 1)))
			SM_makeHeader(images, numExposures, file_height, min(file_img_w, (signed int)file_width) / horizontalBin, cameraname, (double)frame_interval / 1000.0);
		else
			SM_makeHeader(images, numExposures, file_height, file_width, cameraname, (double)frame_interval / 1000.0);
		dataLength = 2 * (unsigned long long)image_length;
		fileDataLength = (unsigned long long)(images + 1)*(unsigned long long)file_length*superFrame;
		dataLeft = sizeof(FitsHeader) - (fileDataLength) % sizeof(FitsHeader);
		memset(&FitsEnder, 0, min(sizeof(FitsEnder), dataLeft));
	}
	memset(data_ptr[0], 0, file_length);
	_write(tsmhandle[0], (void *)data_ptr[0], file_length);
	_write(tsmhandle[0], (char *)FitsEnder, dataLeft);
	if (esc_hit) {
		_lseek(tsmhandle[0], 0L, SEEK_SET);
		_write(tsmhandle[0], (char *)FitsHeader, sizeof(FitsHeader));
	}
	for (i = 0; i < numChannels; ++i)
		timeouts[i] = pdv_timeouts(pdv_pt[i]);
	DestroyWindow(acq_hwndPB);
	acq_hwndPB = NULL;
	Ddisk_flag = 0;
cleanUp:
	if (debug)
		MessageBox(main_window_handle, errorBuf, "message", MB_OK);
	if (pcdFlag != 1) {
		for (int i = 0; i < numChannels; ++i)
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row*live_factor);
	}
	for (i = numFiles - 1; i >= 0; --i) {
		if (tsmhandle[i]) {
			_close(tsmhandle[i]);
		}
	}
/*	if (AD_flag) {
		if (BNC_data != NULL)
			_aligned_free(BNC_data);
		BNC_data = NULL;
	}
	else if (NI_flag) {
		if (NI_data) {
			if (NI_data != NULL)
				_aligned_free(NI_data);
			NI_data = NULL;
		}
	}*/
	return 1;
}

static int loops_sv = 0, NI_add_frames_sv = 0, mulit_file_len_sv = 0;
int SM_take_tb(int numChannels, char *file_name, int cdsF, int NDR_flag, int images, int layers, int stripes, double frame_interval, int factor, char *cameraname, int file_img_w, int hbin, int BNC_ratio,
	int ExtTriggerFlag, int preTrigger_frames, int darkFlag, int quadFlag, int rotateFlag, int bad_pix_index, int segmented)
{
	int     i, j, k;
	int		esc_hit = 0;
	int		numFiles = 1;
	int     overrun[MAXCHANNELS], overruns[MAXCHANNELS] = { 0,0,0,0 }, timeouts[MAXCHANNELS] = { 0,0,0,0 };// , last_timeouts = 0, recovering_timeout = FALSE;
	unsigned int *dark_image_buffer = (unsigned int *)NULL;
	signed short int *BNC_data = (signed short int *)NULL;
	float64 *NI_data = (float64 *)NULL;
	int		useFrameFormat = FALSE;
	char    frameFormat[_MAX_FNAME] = "";
	char    tsmfname[MAXCHANNELS][_MAX_FNAME] = { "","","","" };
	int		tsmhandle[MAXCHANNELS] = { NULL, NULL, NULL, NULL };
	int		writeToDisk = FALSE;
	int		debug = FALSE;
	u_char *data_ptr[MAXCHANNELS];
	u_char *image_ptr;
	unsigned long long dataLength, fileDataLength;
	unsigned long dataLeft;
	int     numExposures = 1;
	char    edt_devname[128] = "";
	char    errorBuf[256] = "";
	long triggerPos = 0;
	int times[10][MAXCHANNELS][2];
	char buf[200];
	char disk_tmp_file[MAXCHANNELS][_MAX_FNAME] = { "", "", "","" };
	int diskF_handle[4];
	int disk_chunk_size;
	double stim_train_w, stim_train_intvl, stim_train_delay;
	double stim_train_w2, stim_train_intvl2, stim_train_delay2;
	double pacing_train_w2, pacing_train_intvl2, pacing_train_delay2;
	double pacing_train_w3, pacing_train_intvl3, pacing_train_delay3;
	int num_stim1 = 0, num_stim2 = 0, num_stim3=0;
	double ArtificialCell_freq, ArtificialCell_percent, ArtificialCell_volt;
	int ACell_flag = 0;
	HWND acq_hwndPB_disk;

//	debug = TRUE;
	loops = 1;
	superFrame = 1;
	strcpy(tsmfname[0], file_name);
	for (i = 0; i < MAXCHANNELS; ++i) {
		if (strlen(tsmfname[i])) {
			if ((tsmhandle[i] = _open(tsmfname[i], O_MODE, S_MODE)) == -1) {
				sprintf(errorBuf, "Cannot open %s\n", tsmfname[i]);
				debug = TRUE;
				goto cleanUp;
			}
			else {
				_close(tsmhandle[i]);
				tsmhandle[i] = _open(tsmfname[i], O_MODE, S_MODE);
				_lseek(tsmhandle[i], 0L, SEEK_SET);
			}
		}
	}
	if (pcdFlag != 1) {
		for (i = 0; i < numChannels; ++i) {
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row*factor);
			if (!pcdFlag) {
				pdv_flush_fifo(pdv_pt[i]);
				pdv_multibuf(pdv_pt[i], 4);	//has to be 4
			}
		}
	}

	initAcquire(numChannels, cdsF, layers, stripes, cameraname, quadFlag, rotateFlag, hbin, factor, 1);
	if (NI_flag) {
		shutter_add_frames = (int)(shutter_wait/frame_interval);
		NI_add_frames = 2 + shutter_add_frames;

		char flagFname[MAX_PATH];
		FILE *fp;
		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\pacing_pattern.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			if (fgets(buf, 255, fp))
				sscanf(buf, "%d %lf %lf %lf%s", &num_stim1, &stim_train_w, &stim_train_intvl, &stim_train_delay, buf);
			if (fgets(buf, 255, fp))
				sscanf(buf, "%d %lf %lf %lf%s", &num_stim2, &pacing_train_w2, &pacing_train_intvl2, &pacing_train_delay2, buf);
			if (fgets(buf, 255, fp))
				sscanf(buf, "%d %lf %lf %lf%s", &num_stim3, &pacing_train_w3, &pacing_train_intvl3, &pacing_train_delay3, buf);
			fclose(fp);
		}
		else {
			strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\stim_pattern.txt");
			if (FOPEN_S(fp, flagFname, "r")) {
				if (fgets(buf, 255, fp))
					sscanf(buf, "%lf%s", &stim_train_w, buf);
				if (fgets(buf, 255, fp))
					sscanf(buf, "%lf%s", &stim_train_intvl, buf);
				if (fgets(buf, 255, fp))
					sscanf(buf, "%lf%s", &stim_train_delay, buf);
				fclose(fp);
			}
			else
				stim_train_w = 0.0;
		}
		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\stim_pattern2.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			if (fgets(buf, 255, fp))
				sscanf(buf, "%lf%s", &stim_train_w2, buf);
			if (fgets(buf, 255, fp))
				sscanf(buf, "%lf%s", &stim_train_intvl2, buf);
			if (fgets(buf, 255, fp))
				sscanf(buf, "%lf%s", &stim_train_delay2, buf);
			fclose(fp);
		}
		else
			stim_train_w2 = 0.0;

		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\artificial_cell.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			if (fgets(buf, 255, fp))
				sscanf(buf, "%lf%s", &ArtificialCell_freq, buf);
			if (fgets(buf, 255, fp))
				sscanf(buf, "%lf%s", &ArtificialCell_percent, buf);
			if (fgets(buf, 255, fp))
				sscanf(buf, "%lf%s", &ArtificialCell_volt, buf);
			fclose(fp);
			ACell_flag = 1;
		}
		else
			ACell_flag = 0;
	}
	else {
		NI_add_frames = 0;
		shutter_add_frames = 0;
	}
	if (!NI_flag && !AD_flag) {
		char BNCname[_MAX_PATH];
		char *p;
		strcpy(BNCname, file_name);
		p = strstr(BNCname, ".tsm");
		*p = '\0';
		strcat(BNCname, ".tbn");
		DeleteFile(BNCname);
	}

	loops = (images + cdsFlag + NI_add_frames + 1) / (factor * superFrame) + (((images + cdsFlag + NI_add_frames + 1) % (factor * superFrame))>0);
	if (!file_img_w)
		file_img_w = file_width;

	int images_header = images;
	int fd_file_size_cutoff = 5000;
	if (FastDiskFlag && fd_no_deinterleave_flag && images >= fd_file_size_cutoff)
		images_header = 2000;
	if (twoKFlag && ((file_img_w != file_width) || (horizontalBin > 1)))
		SM_makeHeader(darkFlag? 1 : images_header, numExposures, file_height, min(file_img_w, (signed int)file_width) / horizontalBin, cameraname, (double)frame_interval / 1000.0);
	else
		SM_makeHeader(darkFlag ? 1 : images_header, numExposures, file_height, file_width, cameraname, (double)frame_interval / 1000.0);

	if (AD_flag)
		++loops;
	dataLength = (unsigned long long)(loops + 1)*(unsigned long long)image_length;
	fileDataLength = (unsigned long long)(images + 1)*(unsigned long long)file_length*superFrame;

	createAcqProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / loops;
	char str[256];
	sprintf(str, "Allocating memory");
	SetWindowText(acq_hwndPB, str);

	if (!twoKFlag)
		FastDiskFlag = 0;
	if (image_data != NULL)
		_aligned_free(image_data);
	if (FastDiskFlag) {
		for (i = 0; i < numChannels; i++) {
			sprintf(disk_tmp_file[i], "%s:\\TB_disk_file_ch%d.dat", fastD_drive, i+1);
			if ((diskF_handle[i] = _open(disk_tmp_file[i], O_MODE, S_MODE)) == -1) {
				sprintf(errorBuf, "Cannot open %s\n", disk_tmp_file[i]);
				debug = TRUE;
				goto cleanUp;
			}
		}
		if (fd_no_deinterleave_flag && images >= fd_file_size_cutoff) {
			image_data = (signed short *)_aligned_malloc(image_length*(images_header / factor + 1), 2);
		}
		else {
			disk_chunk_size = factor * 50;
			image_data = (signed short *)_aligned_malloc(image_length*(disk_chunk_size / factor + 1), 2);
		}
	}
	else if ((image_data = (signed short *)_aligned_malloc(dataLength, 2)) == NULL) {
		writeToDisk = TRUE;
		strcpy(errorBuf, "Not enough memory for Data - please reduce the number of frames to be acquired");
	}
	if (AD_flag && !writeToDisk) {
		if ((BNC_data = (signed short *)_aligned_malloc(loops*image_width*factor * 2 * sizeof(BNC_data[0]), 2)) == NULL) {
			writeToDisk = TRUE;
			strcpy(errorBuf, "Not enough memory for BNC_data");
		}
	}
	if (writeToDisk) {
		debug = TRUE;
		goto cleanUp;
	}

	sprintf(str, "Waiting for trigger");
	SetWindowText(acq_hwndPB, str);
	dataLeft = sizeof(FitsHeader) - (fileDataLength) % sizeof(FitsHeader);
	memset(&FitsEnder, 0, min(sizeof(FitsEnder),dataLeft));

	if (twoKFlag)
		for (i = 0; i < numChannels; ++i)
			data_ptr[i] = (u_char *)image_data + i*frame_length/factor;
	else
			data_ptr[0] = (u_char *)image_data;
	for (i = 0; i < numFiles; ++i)
		_write(tsmhandle[i], (char *)FitsHeader, sizeof(FitsHeader));

	for (i = 0; i < numChannels; ++i) 
		pdv_start_images(pdv_pt[i], loops + 1);

	if (ExtTriggerFlag == 1) {
		sprintf(str, "Waiting for trigger");
		SetWindowText(acq_hwndPB, str);
	}
	if (ExtTriggerFlag && AD_flag) {
		long numAcquire = 0, numTriggeredAcq = 0, frame_pos = 0, triggerPix_inc;
		signed short *triggerPixel;
		int triggered = FALSE;
			
		if (strstr(cameraname, "DW"))
			triggerPix_inc = frame_length /sizeof(signed short) - frame_width + 6;
		else
			triggerPix_inc = 6;
		long timeout_frame = min(200000, (long)(300000/frame_interval));
		int trigger_val;
		trigger_val = 6500;
		while (numAcquire < timeout_frame && numTriggeredAcq < loops -preTrigger_frames) {
			image_ptr = pdv_wait_image_raw(pdv_pt[0]);
			if ((overrun[0] = (edt_reg_read(pdv_pt[0], PDV_STAT) & PDV_OVERRUN)))
				++overruns[0];
			memcpy(data_ptr[0], image_ptr, frame_length);

			if (!triggered) {
				pdv_start_image(pdv_pt[0]);
				triggerPixel = (signed short *)data_ptr[0];
				if (*(triggerPixel+triggerPix_inc) > trigger_val) {
					triggered = TRUE;
					numTriggeredAcq = 0;
				}
			}
			else {
				numTriggeredAcq++;
				sprintf(str, "Acquired %d of %d images", numTriggeredAcq, images);
				SetWindowText(acq_hwndPB, str);
				while ((int)(numTriggeredAcq*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
			}
			numAcquire++;
			frame_pos++;
			if (frame_pos >= loops) {
				data_ptr[0] = (u_char *)image_data;
				frame_pos = 0;
			}
			else
				data_ptr[0] += image_length;
		}
		data_ptr[0] = (u_char *)image_data;
		triggerPos = frame_pos;
	}
	else {
		/*****for NI acquisition***/
#ifdef _USE_NI
		int       error = 0;
		TaskHandle  taskHandle_in = 0, taskHandle_out = 0, taskHandle_clk = 0, taskHandle_AO = 0;
		uInt32 *output_data = 0;
		float64 *AO_data = NULL;
		char        errBuff[2048] = { '\0' };
		int i, delay, output_sample, resting_v, trigger_v, num_ni_out = 2;
		float64 output_rate, sample_chan_rate;
		float64 *NI_pt = NULL;
		long total_read = 0, total_written = 0, BNC_num_frames;

		if (NI_flag) {
			delay = 0; // max(1, (int)(4.0 / frame_interval));

			// config outputs
			sample_chan_rate = BNC_ratio*1005.0 / frame_interval;
			output_rate = BNC_ratio*1000.0 / frame_interval;
			output_sample = (images + NI_add_frames + 1)*BNC_ratio + delay + 2;
			BNC_num_frames = output_sample - delay - 2;
			output_data = (uInt32 *)_aligned_malloc(output_sample * sizeof(output_data[0]), 4);
			NI_data = (float64 *)_aligned_malloc(BNC_num_frames*numBNC_chan*BNC_ratio * sizeof(NI_data[0]), 2);
			NI_pt = NI_data;

			if (NI_flag == 1) {
				resting_v = 1;
				trigger_v = 0;
			}
			else {
				resting_v = 0;
				trigger_v = 1;
			}

			for (i = 0; i < delay; ++i)
				output_data[i] = resting_v + 2;
			if (BNC_ratio > 1) {
				int m;
				for (; i < output_sample / BNC_ratio; ++i) {
					for (m = 0; m < BNC_ratio / 2; m++)
						output_data[i*BNC_ratio + m] = trigger_v + 2;
					for (; m < BNC_ratio; m++)
						output_data[i*BNC_ratio + m] = resting_v + 2;
				}
				for (m = (output_sample / BNC_ratio)*BNC_ratio; m < output_sample; m++)
					output_data[m] = trigger_v + 2;
				output_data[output_sample - 1] = trigger_v + 2;
			}
			else {
				for (; i < output_sample; ++i)
					output_data[i] = trigger_v + 2;
			}
			if ((stim_train_w < 0.0000001) || (stim_train_w < frame_interval/BNC_ratio)) {
				if (!MNC_flag) {
					int stim_delay_frame = (int)(BNC_ratio * stim_delay / frame_interval);
					for (i = 0; i < BNC_ratio * 2; i++)
						output_data[delay + NI_add_frames*BNC_ratio + stim_delay_frame + i] += 4;
					//add TTL output to P0.3 at the end of acqusition
					for (i = output_sample - BNC_ratio * 2; i < output_sample; i++)
						output_data[i] += 8;
				}
				else {
					int AO_frames = images + NI_add_frames + 1;
					if (ratio_type) {
						for (i = 0; i < frame_per_ratio*BNC_ratio; ++i)
							output_data[i] += 8;
						for (; i < (AO_frames- frame_per_ratio)*BNC_ratio; ++i)
							output_data[i] += 4;
						for (; i < output_sample; ++i)
							output_data[i] += 8;
					}
					else {
						for (i = 0; i < AO_frames / frame_per_ratio; ++i) {
							for (int m = 0; m < frame_per_ratio*BNC_ratio; ++m) {
								if (i % 2)
									output_data[i*frame_per_ratio + m] += 8;
								else
									output_data[i*frame_per_ratio + m] += 4;
							}
						}
					//	for (i = (AO_frames / frame_per_ratio - 1)*frame_per_ratio; i < AO_frames*BNC_ratio; ++i)
					//		output_data[i] += 8;
					}
				}
			}
			else {
				int stim_train_w_frame = (int)(BNC_ratio * stim_train_w / frame_interval);
				int stim_train_intvl_frame = (int)(BNC_ratio * stim_train_intvl / frame_interval);
				int stim_train_delay_frame = (int)(BNC_ratio * stim_train_delay / frame_interval);
				int train_stim_pos = delay + NI_add_frames* BNC_ratio + stim_train_delay_frame;
				if (stim_train_intvl_frame > 0) {
					if (num_stim1) {
						for (j = 0; j < num_stim1; j++) {
							for (i = 0; i < stim_train_w_frame; i++)
								output_data[train_stim_pos + i] += 4;
							train_stim_pos += stim_train_intvl_frame;
							if (train_stim_pos + stim_train_w_frame >= output_sample)
								break;
						}
						if (num_stim2) {
							stim_train_w_frame = (int)(BNC_ratio * pacing_train_w2 / frame_interval);
							stim_train_intvl_frame = (int)(BNC_ratio *  pacing_train_intvl2 / frame_interval);
							stim_train_delay_frame = (int)(BNC_ratio *  pacing_train_delay2 / frame_interval);
							train_stim_pos = delay + NI_add_frames* BNC_ratio + stim_train_delay_frame;
							for (j = 0; j < num_stim2; j++) {
								for (i = 0; i < stim_train_w_frame; i++)
									if (output_data[train_stim_pos + i] < 4)
										output_data[train_stim_pos + i] += 4;
								train_stim_pos += stim_train_intvl_frame;
								if (train_stim_pos + stim_train_w_frame >= output_sample)
									break;
							}
						}
						if (num_stim3) {
							stim_train_w_frame = (int)(BNC_ratio * pacing_train_w3 / frame_interval);
							stim_train_intvl_frame = (int)(BNC_ratio *  pacing_train_intvl3 / frame_interval);
							stim_train_delay_frame = (int)(BNC_ratio *  pacing_train_delay3 / frame_interval);
							train_stim_pos = delay + NI_add_frames* BNC_ratio + stim_train_delay_frame;
							for (j = 0; j < num_stim3; j++) {
								for (i = 0; i < stim_train_w_frame; i++)
									if (output_data[train_stim_pos + i] < 4)
										output_data[train_stim_pos + i] += 4;
								train_stim_pos += stim_train_intvl_frame;
								if (train_stim_pos + stim_train_w_frame >= output_sample)
									break;
							}
						}
					}
					else {
						while (train_stim_pos + stim_train_w_frame < output_sample) {
							for (i = 0; i < stim_train_w_frame; i++)
								output_data[train_stim_pos + i] += 4;
							train_stim_pos += stim_train_intvl_frame;
						}
					}
				}
				if ((stim_train_w2 >= frame_interval / BNC_ratio) && (stim_train_w2 > 0.0000001)) {
					stim_train_w_frame = (int)(BNC_ratio * stim_train_w2 / frame_interval);
					stim_train_intvl_frame = (int)(BNC_ratio * stim_train_intvl2 / frame_interval);
					stim_train_delay_frame = (int)(BNC_ratio * stim_train_delay2 / frame_interval);
					train_stim_pos = delay + NI_add_frames* BNC_ratio + stim_train_delay_frame;
					if (stim_train_intvl_frame > 0) {
						while (train_stim_pos + stim_train_w_frame < output_sample) {
							for (i = 0; i < stim_train_w_frame; i++)
								output_data[train_stim_pos + i] += 8;
							train_stim_pos += stim_train_intvl_frame;
						}
					}
				}
			}

			// config clock channel M series don't have internal clock for output.
			DAQmxErrChk(DAQmxCreateTask("", &taskHandle_clk));
			DAQmxErrChk(DAQmxCreateCOPulseChanTime(taskHandle_clk, "Dev1/ctr0", "", DAQmx_Val_Seconds, DAQmx_Val_Low, 0.00, 0.50 / output_rate, 0.50 / output_rate));
			DAQmxErrChk(DAQmxCfgImplicitTiming(taskHandle_clk, DAQmx_Val_ContSamps, output_sample));

			DAQmxErrChk(DAQmxCreateTask("", &taskHandle_out));
			DAQmxErrChk(DAQmxCreateDOChan(taskHandle_out, "Dev1/port0/line0:2", "", DAQmx_Val_ChanForAllLines));
			DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle_out, "/Dev1/PFI12", output_rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, output_sample));		//P
			if (ExtTriggerFlag)
				DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(taskHandle_clk, "/Dev1/PFI1", DAQmx_Val_Rising));

			if (ACell_flag) {
				// config AO for artificial cell
				int AO_frames = images + NI_add_frames + 1;
				int one_cycle = (int)(1000 / (ArtificialCell_freq*frame_interval));
				AO_data = (float64 *)_aligned_malloc(AO_frames * sizeof(AO_data[0]), 2);
				for (i = 0; i < AO_frames; i++)
					AO_data[i] = ArtificialCell_volt;
				for (int j = 0; j < AO_frames / one_cycle; ++j) {
					for (i = (int)(one_cycle*0.9); i < one_cycle; ++i)
						AO_data[j* one_cycle + i] = ArtificialCell_volt*(1+ ArtificialCell_percent/100);
				}
				DAQmxErrChk(DAQmxCreateTask("", &taskHandle_AO));
				DAQmxErrChk(DAQmxCreateAOVoltageChan(taskHandle_AO, "Dev1/ao0", "", 0.0, 5.0, DAQmx_Val_Volts, NULL)); 		// for 6341 - 2 analog lines
				DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle_AO, "/Dev1/PFI0", sample_chan_rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, AO_frames));
			}
			else if (MNC_flag) {
				// config AO for monochromator
				int AO_frames = images + NI_add_frames + 1;
				int wavelen_jump;
				if (MNC_flag == 2) {
					AO_data = (float64 *)_aligned_malloc(AO_frames * sizeof(AO_data[0]) * 2, 2);		
					wavelen_jump = 1;
				}
				else {
					AO_data = (float64 *)_aligned_malloc(AO_frames * sizeof(AO_data[0]) * 3, 2);
					wavelen_jump = 2;
				}
				if (ratio_type) {
					for (i = 0; i < frame_per_ratio; ++i) {
						AO_data[i] = MNC_in1_vol;
						AO_data[i + AO_frames] = MNC_out1_vol;
						AO_data[i + AO_frames * wavelen_jump] = MNC_w1_vol;
					}
					for (; i < AO_frames - frame_per_ratio; ++i) {
						AO_data[i] = MNC_in2_vol;
						AO_data[i + AO_frames] = MNC_out2_vol;
						AO_data[i + AO_frames * wavelen_jump] = MNC_w2_vol;
					}
					for (; i < AO_frames; ++i) {
						AO_data[i] = MNC_in1_vol;
						AO_data[i + AO_frames] = MNC_out1_vol;
						AO_data[i + AO_frames * wavelen_jump] = MNC_w1_vol;
					}
				}
				else {
					for (i = 0; i < AO_frames / frame_per_ratio; ++i) {
						for (int m = 0; m < frame_per_ratio; ++m) {
							if (i % 2) {
								AO_data[i*frame_per_ratio + m] = MNC_in2_vol;
								AO_data[i*frame_per_ratio + m + AO_frames] = MNC_out2_vol;
								AO_data[i*frame_per_ratio + m + AO_frames * wavelen_jump] = MNC_w2_vol;
							}
							else {
								AO_data[i*frame_per_ratio + m] = MNC_in1_vol;
								AO_data[i*frame_per_ratio + m + AO_frames] = MNC_out1_vol;
								AO_data[i*frame_per_ratio + m + AO_frames * wavelen_jump] = MNC_w1_vol;
							}
						}
					}
					for (i = (AO_frames / frame_per_ratio - 1)*frame_per_ratio; i < AO_frames; ++i) {
						AO_data[i] = MNC_in2_vol;
						AO_data[i + AO_frames] = MNC_out2_vol;
						AO_data[i + AO_frames * wavelen_jump] = MNC_w2_vol;
					}
				}
				DAQmxErrChk(DAQmxCreateTask("", &taskHandle_AO));
				if (MNC_flag == 2)
					DAQmxErrChk(DAQmxCreateAOVoltageChan(taskHandle_AO, "Dev1/ao0:1", "", 0.0, 5.0, DAQmx_Val_Volts, NULL)); 		// for 6341 - 2 analog lines
				else
					DAQmxErrChk(DAQmxCreateAOVoltageChan(taskHandle_AO, "Dev1/ao0:2", "", 0.0, 5.0, DAQmx_Val_Volts, NULL));
				DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle_AO, "/Dev1/PFI0", sample_chan_rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, AO_frames));
			}

			// config inputs and trigger
			DAQmxErrChk(DAQmxCreateTask("", &taskHandle_in));
			if (numBNC_chan == 8)
				DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle_in, "Dev1/ai0:7", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL));
			else
				DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle_in, "Dev1/ai0:3", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL));
			if (BNC_ratio > 1) {
				DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle_in, "/Dev1/PFI12", sample_chan_rate, DAQmx_Val_Falling, DAQmx_Val_FiniteSamps, BNC_num_frames));	//frame-by-frame clock trigger
				DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(taskHandle_in, "/Dev1/PFI0", DAQmx_Val_Rising));
			}
			else
				DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle_in, "/Dev1/PFI0", sample_chan_rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, BNC_num_frames));	//frame-by-frame clock trigger
			if (NI_flag != 1)
				DAQmxErrChk(DAQmxCfgDigEdgeStartTrig(taskHandle_in, "/Dev1/PFI2", DAQmx_Val_Rising));

			DAQmxErrChk(DAQmxRegisterDoneEvent(taskHandle_clk, 0, DoneCallback, NULL));
			DAQmxErrChk(DAQmxWriteDigitalU32(taskHandle_out, output_sample, 0, 10.0, DAQmx_Val_GroupByChannel, output_data, &total_written, NULL));
			if (MNC_flag || ACell_flag)
				DAQmxErrChk(DAQmxWriteAnalogF64(taskHandle_AO, images + 1, 0, 10.0, DAQmx_Val_GroupByChannel, AO_data, NULL, NULL));
			DAQmxErrChk(DAQmxStartTask(taskHandle_in));
			if (MNC_flag || ACell_flag)
				DAQmxErrChk(DAQmxStartTask(taskHandle_AO));
			DAQmxErrChk(DAQmxStartTask(taskHandle_out));
			DAQmxErrChk(DAQmxStartTask(taskHandle_clk));
			if (debug)
				fprintf(stdout, "\n\noutput_sample: %d, total_written :%d, %lf, %lf, %d\n\n", output_sample, total_written, sample_chan_rate, output_rate, BNC_ratio);
		}
#endif
		/****End of NI****/

		if (SFC_flag)
			startSFC_scan(-1);

//		twoK_start_missed = 0;
		if (pcdFlag) {
			int bufsize = image_length;
			int numbufs = 4;
			PdvDev *pdv_p_sv = pdv_p;
		//	if (pcdFlag == 2)
				pdv_p = pcd_p;
			edt_reg_write(pdv_p, SSD16_CHEN, 0x0);
			edt_flush_channel(pdv_p, 0);
			if (edt_configure_ring_buffers(pdv_p, bufsize, numbufs, EDT_READ, NULL) != -1)
			{
				edt_start_buffers(pdv_p, loops);
				edt_reg_write(pdv_p, SSD16_CHEN, 0x0001);

				for (j = 0; (j < loops || loops == 0); j++)
				{
					image_ptr = edt_wait_for_buffers(pdv_p, 1);
					if (FastDiskFlag)
						_write(diskF_handle[0], (void *)image_ptr, frame_length);
					else {
						memcpy(data_ptr[0], image_ptr, frame_length);
						data_ptr[0] += image_length;
					}
					sprintf(str, "Acquired %d of %d images", j*factor, images);
					SetWindowText(acq_hwndPB, str);
					while ((int)(j*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
						SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
						step_cnt++;
					}
				}

			}
			else
				MessageBox(main_window_handle, "edt_configure_ring_buffers failed", "message", MB_OK);
			pdv_p = pdv_p_sv;
		}
		else if (numChannels > 1 && omp_get_num_procs() >= numChannels) {
			for (int m = 0; m < 10; m++) {
				for (int n = 0; n < numChannels; n++) {
					image_ptr = pdv_wait_image_timed_raw(pdv_pt[n], (unsigned int *)&times[0][n][0], 1);
					if (FastDiskFlag)
						_write(diskF_handle[n], (void *)image_ptr, frame_length);
					else {
						for (i = 0; i < factor; ++i) {
							memcpy(data_ptr[n], image_ptr, frame_length / factor);
							image_ptr += frame_length / factor;
							data_ptr[n] += image_length / factor;
						}
					}
				}
				break;
			}
			omp_set_num_threads(numChannels);
			for (j = 1; j < loops; j++) {
#pragma omp parallel private(image_ptr)
				{
					int thread_id, i;

					thread_id = omp_get_thread_num();
//					image_ptr = pdv_wait_image_timed_raw(pdv_pt[thread_id], (unsigned int *)&times[0][thread_id][0], 1);
					image_ptr = pdv_wait_image_raw(pdv_pt[thread_id]);
					if ((overrun[thread_id] = (edt_reg_read(pdv_pt[thread_id], PDV_STAT) & PDV_OVERRUN)))
						++overruns[thread_id];
					if (FastDiskFlag)
						_write(diskF_handle[thread_id], (void *)image_ptr, frame_length);
					else {
						for (i = 0; i < factor; ++i) {
							memcpy(data_ptr[thread_id], image_ptr, frame_length / factor);
							image_ptr += frame_length / factor;
							data_ptr[thread_id] += image_length / factor;
						}
					}
				}
				/*****for NI acquisition***/
#ifdef _USE_NI
				if (NI_flag && !darkFlag) {
					long read;
					if (j == loops - 1)
						DAQmxReadAnalogF64(taskHandle_in, factor*BNC_ratio*(j + 1) - total_read, 5.0, DAQmx_Val_GroupByScanNumber, NI_pt, (factor*BNC_ratio*(j + 1) - total_read + 1)*numBNC_chan, &read, NULL);
					else
						DAQmxReadAnalogF64(taskHandle_in, factor*BNC_ratio*(j + 1) - total_read, 0.0, DAQmx_Val_GroupByScanNumber, NI_pt, (factor*BNC_ratio*(j + 1) - total_read)*numBNC_chan, &read, NULL);
					NI_pt += read*numBNC_chan;
					total_read += read;
				}
#endif
				/****End of NI****/
				sprintf(str, "Acquired %d of %d images", j*factor, images);
				SetWindowText(acq_hwndPB, str);
				while ((int)(j*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
				if (KEY_DOWN(VK_ESCAPE)) {
					esc_hit = 1;
					goto exitAcq;
				}
			}

		}
		else {
			for (j = 0; j < loops; j++) {
				for (i = 0; i < numChannels; ++i) {
					image_ptr = pdv_wait_image_raw(pdv_pt[i]);
					if ((overrun[i] = (edt_reg_read(pdv_pt[i], PDV_STAT) & PDV_OVERRUN)))
						++overruns[i];
					memcpy(data_ptr[i], image_ptr, frame_length);
					data_ptr[i] += image_length;
				}
				if (Falcon_flag) {
					unsigned __int64 *long_pt = (unsigned __int64 *)(data_ptr[0] - image_length);
					unsigned short int *short_pt0, *short_pt1, *short_pt2, *short_pt3;
					short_pt0 = (unsigned short int *)long_pt;
					short_pt1 = short_pt0 + 1;
					short_pt2 = short_pt1 + 1;
					short_pt3 = short_pt2 + 1;
					for (i = 0; i < (signed)frame_length >> 2; i++) {
						*long_pt++ = (bitLut[*short_pt0] | bitLut[(*short_pt1) | 0x10000L] | bitLut[(*short_pt2) | 0x20000L] | bitLut[(*short_pt3) | 0x30000L]) & 0x3fff3fff3fff3fffL;
						short_pt0 += 4;
						short_pt1 += 4;
						short_pt2 += 4;
						short_pt3 += 4;
					}
				}

				/*****for NI acquisition***/
#ifdef _USE_NI
				if (NI_flag && !darkFlag) {
					long read;
					if (j == loops - 1)
						DAQmxReadAnalogF64(taskHandle_in, factor*BNC_ratio*(j + 1) - total_read, 5.0, DAQmx_Val_GroupByScanNumber, NI_pt, (factor*BNC_ratio*(j + 1) - total_read + 1)*numBNC_chan, &read, NULL);
					else
						DAQmxReadAnalogF64(taskHandle_in, factor*BNC_ratio*(j + 1) - total_read, 0.0, DAQmx_Val_GroupByScanNumber, NI_pt, (factor*BNC_ratio*(j + 1) - total_read)*numBNC_chan, &read, NULL);
					NI_pt += read*numBNC_chan;
					total_read += read;
				}
#endif
				/****End of NI****/
				sprintf(str, "Acquired %d of %d images", j*factor, images);
				SetWindowText(acq_hwndPB, str);
				while ((int)(j*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
			}
		}
exitAcq:
		GetSystemTime(&acq_end_sysTime);
		for (i = 0; i < numChannels; ++i)
			timeouts[i] = pdv_timeouts(pdv_pt[i]);

		/*****for NI acquisition***/
#ifdef _USE_NI
		Error:
		if (NI_flag && !darkFlag) {
			if (DAQmxFailed(error)) {
				debug = TRUE;
				DAQmxGetExtendedErrorInfo(errBuff, 2048);
			}
			if (taskHandle_clk) {
				DAQmxStopTask(taskHandle_clk);
				DAQmxClearTask(taskHandle_clk);
			}
			if (taskHandle_AO) {
				DAQmxStopTask(taskHandle_AO);
				DAQmxClearTask(taskHandle_AO);
			}
			if (taskHandle_out != 0) {
				DAQmxStopTask(taskHandle_out);
				DAQmxClearTask(taskHandle_out);
			}
			if (taskHandle_in != 0) {
				DAQmxStopTask(taskHandle_in);
				DAQmxClearTask(taskHandle_in);
			}
			if (DAQmxFailed(error)) {
				char errStr[256];
				sprintf(errStr, "National Instruments Device Error: %s", errBuff);
				MessageBox(main_window_handle, errStr, "message", MB_OK);
			}
			if (output_data)
				_aligned_free(output_data);
			if (AO_data)
				_aligned_free(AO_data);

			NI_add_frames -= shutter_add_frames;
			saveNI_data(images, tsmfname[0], numBNC_chan, BNC_ratio, NI_data, delay, twoKFlag);
		}
#endif
		/****End of NI****/
	}
	SM_stopAD();

	if (esc_hit)
		goto cleanUp;

	if (AD_flag) {
		if (factor > 1) {
			frame_height /= factor;
			factor = 1;
			loops = images + 1;
		}
		stripAD_data((unsigned short *)image_data, BNC_data, cameraname, file_height, frame_width, frame_height, loops, cdsFlag);
		--loops;

		saveBNC_data(loops, tsmfname[0], numBNC_chan, BNC_ratio, BNC_data, cameraname, frame_width, stripes);
		image_height = file_height;
		image_length = image_width*image_height*2;
	}
	if (factor > 1) {
		image_height = image_height / factor;
		image_length = image_width*image_height * 2;
		factor = 1;
		loops = (images + cdsFlag + NI_add_frames) / superFrame + (((images + cdsFlag + NI_add_frames) % superFrame)>0);
	//	if (twoK_start_missed)
		//	loops++;
	}

	if (Bit16_flag) {
		unsigned short *img_ptr = (unsigned short *)image_data;
		for (i = 0; i < (int)(image_length*loops>>1); i++)
			*img_ptr++ = (*img_ptr >> 2);
	}

	if (FastDiskFlag) {
		int stt_byte;
		int read_len;
		unsigned int file_length_sv;

		acq_hwndPB_disk = acq_hwndPB;
		acq_hwndPB = NULL;
		SendMessage(acq_hwndPB_disk, PBM_SETPOS, (WPARAM)0, 0);
		step_cnt = 0;
		step_size = ((double)num_steps) / loops;
		long this_img;

		signed short *last_frame = (signed short *)_aligned_malloc(image_length, 2);
		if (NI_flag && shutter_add_frames)
			stt_byte = shutter_add_frames * image_length/numChannels;
		else
			stt_byte = 0;
		disk_chunk_size = min(disk_chunk_size, loops);
		data_ptr[0] = (u_char *)image_data;
		read_len = image_length / numChannels;
		for (i = 0; i < numChannels; i++) {
			_close(diskF_handle[i]);
			diskF_handle[i] = _open(disk_tmp_file[i], O_BINARY | O_RDONLY);
			_lseeki64(diskF_handle[i], stt_byte, SEEK_SET);
		//	if (cdsFlag) {
				_read(diskF_handle[i], (u_char *)data_ptr[0], read_len);
				data_ptr[0] += image_length / numChannels;
		//	}
		}
		this_img = 1;
		file_length_sv = file_length;
		if (fd_no_deinterleave_flag && images >= fd_file_size_cutoff) {
			step_size = ((double)num_steps) / images_header;
			for (k = 0; k < images_header / 2; k++) {
				for (i = 0; i < numChannels; i++) {
					_read(diskF_handle[i], (u_char *)data_ptr[0], read_len);
					data_ptr[0] += image_length / numChannels;
				}
			}
			for (i = 0; i < numChannels; i++)
				_lseeki64(diskF_handle[i], stt_byte + read_len*(images - images_header / 2), SEEK_SET);
			for (k = 0; k < images_header / 2 + 1; k++) {
				for (i = 0; i < numChannels; i++) {
					_read(diskF_handle[i], (u_char *)data_ptr[0], read_len);
					data_ptr[0] += image_length / numChannels;
				}
			}
			if (cdsFlag)
				subtractCDS((unsigned short *)image_data, images_header + 1, image_width, image_height, image_length, factor, quadFlag, twoKFlag, numChannels);
			data_ptr[0] = (u_char *)image_data;
			for (k = 0; k < images_header; k++) {
				file_length = file_length_sv;
				frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_ptr[0], image_buffer);
				frameRepair(image_buffer, bad_pix_index, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
				file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
				if (ExtTriggerFlag && AD_flag && triggerPos) {
					if (!k)
						_lseek(tsmhandle[0], 2880L + (loops - triggerPos - 1)*file_length, SEEK_SET);
					else if (k == triggerPos)
						_lseek(tsmhandle[0], 2880L, SEEK_SET);
				}
				_write(tsmhandle[0], (void *)image_buffer, file_length);
				data_ptr[0] += image_length;

				sprintf(str, "Deinterleaving %d of %d images", this_img, images_header);
				SetWindowText(acq_hwndPB_disk, str);
				while ((int)(this_img*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(acq_hwndPB_disk, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
				this_img++;
			}
			memset(data_ptr[0], 0, file_length);
			dataLeft = sizeof(FitsHeader) - ((images_header+1)*file_length) % sizeof(FitsHeader);
			_lseeki64(tsmhandle[0], 2880L + (unsigned long long)images_header*(unsigned long long)file_length, SEEK_SET);
			_write(tsmhandle[0], (void *)data_ptr[0], file_length);
			_write(tsmhandle[0], (char *)FitsEnder, dataLeft);
			char new_diskF_name[_MAX_PATH], new_diskF_name2[_MAX_PATH];
			char *newP;
			char mssg[256];
			for (i = 0; i < numChannels; i++) {
				_close(diskF_handle[i]);
				strcpy(new_diskF_name, file_name);
				while (newP = strstr(new_diskF_name, "\\"))
					strcpy(new_diskF_name, newP + 1);
				if (newP = strstr(new_diskF_name, ".tsm")) {
					*newP = '\0';
					strcpy(new_diskF_name2, new_diskF_name);
					sprintf(new_diskF_name, "%s:\\%s_disk_file_ch%d.dat", fastD_drive, new_diskF_name2, i+1);
					if ((diskF_handle[i] = _open(new_diskF_name, O_BINARY | O_RDONLY)) != -1) {
						_close(diskF_handle[i]);
						sprintf(mssg, "%s already exists. Yes to overwrite it, No to keep the name %s, which you need to delete or rename before next acqusition", new_diskF_name, disk_tmp_file[i]);
						if (MessageBox(main_window_handle,
							(LPCTSTR) mssg,
							(LPCTSTR) "Quit Turbo-SM",
							MB_YESNO |
							MB_ICONQUESTION |
							MB_SYSTEMMODAL) == IDYES) {
							DeleteFile(new_diskF_name);
							rename(disk_tmp_file[i], new_diskF_name);
						}
						else
							strcpy(new_diskF_name, disk_tmp_file[i]);
					}
					else
						rename(disk_tmp_file[i], new_diskF_name);
					if (!i) {
						if ((diskF_handle[i] = _open(new_diskF_name, O_APPEND | O_BINARY | O_SEQUENTIAL | O_RDWR, S_MODE)) != -1) {
							_lseeki64(diskF_handle[i], 0, SEEK_END);
							_write(diskF_handle[i], &images, sizeof(int));
							_write(diskF_handle[i], &image_width, sizeof(unsigned int));
							_write(diskF_handle[i], &image_height, sizeof(unsigned int));
							_write(diskF_handle[i], &image_length, sizeof(unsigned int));
							_write(diskF_handle[i], &file_width, sizeof(unsigned int));
							_write(diskF_handle[i], &file_height, sizeof(unsigned int));
							_write(diskF_handle[i], &image_stripes, sizeof(int));
							_write(diskF_handle[i], &image_layers, sizeof(int));
							_write(diskF_handle[i], &stt_byte, sizeof(int));
							_write(diskF_handle[i], &cdsFlag, sizeof(int));
							_write(diskF_handle[i], &numChannels, sizeof(int));
							_write(diskF_handle[i], &horizontalBin, sizeof(int));
							_write(diskF_handle[i], &file_img_w, sizeof(int));
							_write(diskF_handle[i], &binFlag, sizeof(int));
							_write(diskF_handle[i], &segmented, sizeof(int));
							_close(diskF_handle[i]);
						}
					}
				}
			}
		}
		else {
			for (j = 0; j < loops / disk_chunk_size; ++j) {
				for (k = 0; k < disk_chunk_size; k++) {
					for (i = 0; i < numChannels; i++) {
						_read(diskF_handle[i], (u_char *)data_ptr[0], read_len);
						data_ptr[0] += image_length / numChannels;
					}
				}
				if (cdsFlag) {
					memcpy(last_frame, (data_ptr[0] - image_length), image_length);
					subtractCDS((unsigned short *)image_data, disk_chunk_size + 1, image_width, image_height, image_length, factor, quadFlag, twoKFlag, numChannels);
				}
				data_ptr[0] = (u_char *)image_data;
				for (k = 0; k < disk_chunk_size; k++) {
					file_length = file_length_sv;
					frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_ptr[0], image_buffer);
					frameRepair(image_buffer, bad_pix_index, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
					file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
					if (ExtTriggerFlag && AD_flag && triggerPos) {
						if (!k)
							_lseek(tsmhandle[0], 2880L + (loops - triggerPos - 1)*file_length, SEEK_SET);
						else if (k == triggerPos)
							_lseek(tsmhandle[0], 2880L, SEEK_SET);
					}
					_write(tsmhandle[0], (void *)image_buffer, file_length);
					data_ptr[0] += image_length;

					sprintf(str, "Deinterleaving %d of %d images", this_img, images);
					SetWindowText(acq_hwndPB_disk, str);
					while ((int)(this_img*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
						SendMessage(acq_hwndPB_disk, PBM_STEPIT, 0, 0);
						step_cnt++;
					}
					this_img++;
				}

				data_ptr[0] = (u_char *)image_data;
				if (cdsFlag) {
					memcpy(data_ptr[0], last_frame, image_length);
					data_ptr[0] += image_length;
				}
			}
			if (loops % disk_chunk_size) {
				for (k = 0; k < (loops % disk_chunk_size); k++) {
					for (i = 0; i < numChannels; i++) {
						_read(diskF_handle[i], (u_char *)data_ptr[0], read_len);
						data_ptr[0] += image_length / numChannels;
					}
				}
				if (cdsFlag) {
					memcpy(last_frame, (data_ptr[0] - image_length), image_length);
					subtractCDS((unsigned short *)image_data, (loops % disk_chunk_size) + 1, image_width, image_height, image_length, factor, quadFlag, twoKFlag, numChannels);
				}
				data_ptr[0] = (u_char *)image_data;
				for (k = 0; k < (loops % disk_chunk_size); k++) {
					file_length = file_length_sv;
					frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_ptr[0], image_buffer);
					frameRepair(image_buffer, bad_pix_index, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
					file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
					if (ExtTriggerFlag && AD_flag && triggerPos) {
						if (!k)
							_lseek(tsmhandle[0], 2880L + (loops - triggerPos - 1)*file_length, SEEK_SET);
						else if (k == triggerPos)
							_lseek(tsmhandle[0], 2880L, SEEK_SET);
					}
					_write(tsmhandle[0], (void *)image_buffer, file_length);
					data_ptr[0] += image_length;

					sprintf(str, "Deinterleaving %d of %d images", this_img, images);
					SetWindowText(acq_hwndPB_disk, str);
					while ((int)(this_img*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
						SendMessage(acq_hwndPB_disk, PBM_STEPIT, 0, 0);
						step_cnt++;
					}
					this_img++;
				}
			}
			memset(data_ptr[0], 0, file_length);
			_lseeki64(tsmhandle[0], 2880L + (unsigned long long)images*(unsigned long long)file_length, SEEK_SET);
			_write(tsmhandle[0], (void *)data_ptr[0], file_length);
			_write(tsmhandle[0], (char *)FitsEnder, dataLeft);
			for (i = 0; i < numChannels; i++) {
				_close(diskF_handle[i]);
				DeleteFile(disk_tmp_file[i]);
			}
		}
		DestroyWindow(acq_hwndPB_disk);
		acq_hwndPB_disk = NULL;
		goto cleanUp;
	}

	if (NI_flag && shutter_add_frames) {
		signed short *scr_ptr, *dst_ptr;
		scr_ptr = image_data + shutter_add_frames * image_length / 2 ;
		dst_ptr = image_data;
		for (i = 0; i < loops; i++, scr_ptr += (image_length>>1), dst_ptr += (image_length >> 1))
			memcpy(dst_ptr, scr_ptr, image_length);
	}

	if (!fd_no_deinterleave_flag || (num_trials == 1)) {
		if (cdsFlag) {
			subtractCDS((unsigned short *)image_data, loops, image_width, image_height, image_length, factor, quadFlag, twoKFlag, numChannels);
			--loops;
		}
		loops -= NI_add_frames;

		SendMessage(acq_hwndPB, PBM_SETPOS, (WPARAM)0, 0);
		step_cnt = 0;
		step_size = ((double)num_steps) / loops;

		unsigned int file_length_sv = file_length;
		for (i = 0; i < numFiles; ++i) {
			data_ptr[i] = (u_char *)image_data;
			if (NI_flag && NI_add_frames)
				data_ptr[i] += image_length * (long)NI_add_frames;
			for (j = 0, k = 0; j < loops; ++j, k += superFrame) {
				file_length = file_length_sv;
				frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_ptr[i], image_buffer);
				frameRepair(image_buffer, bad_pix_index, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
				file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
				if (ExtTriggerFlag && AD_flag && triggerPos) {
					if (!j)
						_lseek(tsmhandle[i], 2880L + (loops - triggerPos - 1)*file_length, SEEK_SET);
					else if (j == triggerPos)
						_lseek(tsmhandle[i], 2880L, SEEK_SET);
				}
				multiPixReadAv((signed short *)image_buffer);
				if (!darkFlag || !j)
					_write(tsmhandle[i], (void *)image_buffer, (k <= images ? superFrame : images % superFrame)*file_length);
				data_ptr[i] += image_length;
				sprintf(str, "Deinterleaving %d of %d images", j, images);
				SetWindowText(acq_hwndPB, str);
				while ((int)(j*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
			}
		}
		memset(data_ptr[0], 0, file_length);
		for (i = 0; i < numFiles; ++i) {
			_write(tsmhandle[i], (void *)data_ptr[0], file_length);
			_write(tsmhandle[i], (char *)FitsEnder, dataLeft);
		}
	}
	else {
		char tmp_file_name[MAX_PATH];
		char *p;
		int tmptsm_handle;

		loops_sv = loops;
		NI_add_frames_sv = NI_add_frames;
		mulit_file_len_sv = file_length;
		strcpy(tmp_file_name, tsmfname[0]);
		if (p = strstr(tmp_file_name, ".tsm")) {
			*p = '\0';
			strcat(tmp_file_name, ".tmp");
			if ((tmptsm_handle = _open(tmp_file_name, O_MODE, S_MODE)) != -1) {
				_write(tmptsm_handle, (void *)image_data, image_length*loops);
				_close(tmptsm_handle);
			}
		}
	}
	DestroyWindow(acq_hwndPB);
	acq_hwndPB = NULL;

cleanUp:
	if (debug)
		MessageBox(main_window_handle, errorBuf, "message", MB_OK);
	if (pcdFlag != 1) {
		for (int i = 0; i < numChannels; ++i)
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row*live_factor);
	}
	for (i = numFiles - 1; i >= 0; --i) {
		if (tsmhandle[i]) {
			_close(tsmhandle[i]);
		}
	}
	if (dark_image_buffer != NULL) {
		_aligned_free(dark_image_buffer);
		dark_image_buffer = NULL;
	}
	if (AD_flag) {
		if (BNC_data != NULL)
			_aligned_free(BNC_data);
		BNC_data = NULL;
	}
	else if (NI_flag && !darkFlag) {
		if (NI_data) {
			if (NI_data != NULL)
				_aligned_free(NI_data);
			NI_data = NULL;
		}
	}
	if (esc_hit) {
		if (FastDiskFlag) {
			for (i = 0; i < numChannels; i++) {
				_close(diskF_handle[i]);
				DeleteFile(disk_tmp_file[i]);
			}
		}
		DestroyWindow(acq_hwndPB);
		acq_hwndPB = NULL;
		return FALSE;
	}
	return TRUE;
}

void rearrange_File(int images, int file_id, int numChannels, int quadFlag, int file_img_w, int segmented)	
{
	int j, k;
	char str[256], tmp_file_name[MAX_PATH];
	u_char *data_ptr;
	char *p;
	int tmp_handle, tsm_handle;
	unsigned long dataLeft;

	int step_cnt = 0;
	double step_size = ((double)num_steps) / loops;

	createAcqProgBar();
	unsigned int file_length_sv = mulit_file_len_sv;
	SendMessage(acq_hwndPB, PBM_SETPOS, (WPARAM)0, 0);
	loops = loops_sv;
	NI_add_frames = NI_add_frames_sv;
	if ((tsm_handle = _open(multi_file_names[file_id], O_APPEND | O_BINARY | O_SEQUENTIAL | O_RDWR, S_MODE)) == -1) {
		MessageBox(main_window_handle, "Can't find the *.tsm file.", "message", MB_OK);
		return;
	}
	strcpy(tmp_file_name, multi_file_names[file_id]);
	if (p = strstr(tmp_file_name, ".tsm")) {
		*p = '\0';
		strcat(tmp_file_name, ".tmp");
		if ((tmp_handle = _open(tmp_file_name, O_BINARY | O_RDONLY)) == -1) {
			MessageBox(main_window_handle, "Can't find the *.tsm file.", "message", MB_OK);
			return;
		}
	}

	_read(tmp_handle, (void *)image_data, image_length*loops);
	if (cdsFlag) {
		subtractCDS((unsigned short *)image_data, loops, image_width, image_height, image_length, 1, quadFlag, twoKFlag, numChannels);
		--loops;
	}
	loops -= NI_add_frames;

	data_ptr = (u_char *)image_data;
	if (NI_flag && NI_add_frames)
		data_ptr += image_length * (long)NI_add_frames;
	_lseek(tsm_handle, 2880L, SEEK_SET);
	for (j = 0, k = 0; j < loops; ++j, k += superFrame) {
		file_length = file_length_sv;
		frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_ptr, image_buffer);
		frameRepair(image_buffer, 0, twoKFlag, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
		file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
		multiPixReadAv((signed short *)image_buffer);
		_write(tsm_handle, (void *)image_buffer, (k <= images ? superFrame : images % superFrame)*file_length);
		data_ptr += image_length;
		sprintf(str, "Deinterleaving %d of %d images of %s", j, images, multi_file_names[file_id]);
		SetWindowText(acq_hwndPB, str);
		while ((int)(j*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}
	memset(data_ptr, 0, file_length);
	_write(tsm_handle, (void *)data_ptr, file_length);
	dataLeft = sizeof(FitsHeader) - ((images + 1)*file_length) % sizeof(FitsHeader);
	_write(tsm_handle, (char *)FitsEnder, dataLeft);
	_close(tmp_handle);
	DeleteFile(tmp_file_name);
	_close(tsm_handle);
	DestroyWindow(acq_hwndPB);
	acq_hwndPB = NULL;
}

void
stripAD_data(unsigned short int *image_data, signed short int *BNC_data, char *cameraType, int file_height, int frame_width, int frame_height, int loops, int cds)
{
	long dst_inc, src_inc, BNC_frame_len, BNC_dst_inc;
	int i;
	unsigned short int *sptr, *dptr;
	signed short int *BNC_sptr, *BNC_dptr;
	dptr = (unsigned short *)image_data;
	sptr = (unsigned short *)image_data;
	BNC_dptr = (signed short *)BNC_data;
	BNC_sptr = (signed short *)image_data;

	dst_inc = frame_width*file_height;
	src_inc = frame_width*frame_height;
	BNC_frame_len = frame_width*2;
	BNC_dst_inc = frame_width;
	if (strstr(cameraType, "DW")) {
		BNC_sptr += dst_inc;
		if (cds)
			BNC_sptr += src_inc;
	}
	else {	// ***CCD39 and CCD67*** 
		sptr += frame_width * 2;
		BNC_dst_inc = frame_width*2;
		BNC_frame_len = frame_width*4;
	}

	for (i = 0; i < loops; i++) {
		memcpy((void *)BNC_dptr, (void *)BNC_sptr, BNC_frame_len);
		BNC_dptr += BNC_dst_inc;
		BNC_sptr += src_inc;

		memcpy((void *)dptr, (void *)sptr, frame_width*file_height * 2);
		dptr += dst_inc;
		sptr += src_inc;
	}
}

void frameRepair(unsigned short int *image_buffer, int bad_pix_index, int twoKFlag, unsigned int image_width, unsigned int file_width, unsigned int file_height, int horizontal_bin, int superFrame, int file_img_w, int binFlag, int segmented)
{
	if (bad_pix_index && bad_pixel_ar[0][0]) {
		//fudge bad pixels
		int i, tmp;
		signed short *bad_pt, *nb_pt;

		for (i = 0; i < sizeof(bad_pixel_ar) / sizeof(bad_pixel_ar[0]); i++) {
			if (bad_pixel_ar[i][0]) {
				bad_pt = (signed short *)(image_buffer + bad_pixel_ar[i][0] - 1);

				int nn = 0;
				tmp = 0;
				for (int mm = 1; mm <= 8; mm++) {
					if (bad_pixel_ar[i][mm] > 0) {
						nb_pt = (signed short *)(image_buffer + bad_pixel_ar[i][mm] - 1);
						tmp = tmp + *nb_pt;
						nn++;
					}
				}
				if (nn > 0)
					*bad_pt = tmp / nn;
			}
			else
				break;
		}
	}
	else if (twoKFlag) {
		//fix bad rows and bad cols
		signed short *bad_pt, *nb_pt1, *nb_pt2;
		int ii;
		if (bad_row_ar[0][0] && file_width <= 2048) {
			ii = 0;
			while (bad_row_ar[ii][0]) {
				bad_pt = (signed short *)(image_buffer + bad_row_ar[ii][0] * file_width);
				nb_pt1 = (signed short *)(image_buffer + bad_row_ar[ii][3] * file_width);
				nb_pt2 = (signed short *)(image_buffer + bad_row_ar[ii][4] * file_width);
				if (bad_row_ar[ii][3] == bad_row_ar[ii][4])
					memcpy(bad_pt, nb_pt1, file_width * 2);
				else {
					for (unsigned int kk = 0; kk < file_width; kk++) {
						*bad_pt = (*nb_pt1 + *nb_pt2) / 2;
						bad_pt++;
						nb_pt1++;
						nb_pt2++;
					}
				}
				ii++;
			}
		}
		if (bad_col_ar[0][0] && file_width <= 2048) {
			ii = 0;
			int y1, y2;
			while (bad_col_ar[ii][0]) {
				if (bad_col_ar[ii][1] == 0) {
					y1 = 0;
					y2 = file_height / 2 - 1;
				}
				else {
					y1 = file_height / 2;
					y2 = file_height - 1;
				}
				bad_pt = (signed short *)image_buffer + y1*file_width + bad_col_ar[ii][0];
				nb_pt1 = (signed short *)image_buffer + y1*file_width + bad_col_ar[ii][3];
				nb_pt2 = (signed short *)image_buffer + y1*file_width + bad_col_ar[ii][4];
				for (int kk = y1; kk <= y2; kk++) {
					if (bad_row_ar[ii][3] == bad_row_ar[ii][4])
						*bad_pt = *nb_pt1;
					else
						*bad_pt = (*nb_pt1 + *nb_pt2) / 2;
					bad_pt += file_width;
					nb_pt1 += file_width;
					nb_pt2 += file_width;
				}
				ii++;
			}
		}

		if (file_img_w != file_width) {
			unsigned short int *sptr, *dptr;
			if (segmented)
				sptr = image_buffer + file_width / 2;
			else {
				if (OffCenter_crop)
					sptr = image_buffer + file_width - file_img_w;
				else
					sptr = image_buffer + (file_width - file_img_w) / 2;
			}
			dptr = image_buffer;
			int l, m; 
			for (m = 0; m < (signed int)file_height; m++) {
				for (l = 0; l < file_img_w; l++)
					*dptr++ = *sptr++;
				sptr += file_width - file_img_w;
			}
		}
		if (horizontal_bin > 1) {
			unsigned short int *sptr, *dptr;
			sptr = image_buffer;
			dptr = image_buffer;
			int l, m, kk;
			unsigned long bin_val;
			for (m = 0; m < (signed int)file_height; m++) {
				for (l = 0; l < min(file_img_w, (signed int)file_width) / horizontal_bin; l++) {
					bin_val = 0;
					for (kk = 0; kk < horizontal_bin; kk++)
						bin_val += (unsigned long)*sptr++;
					*dptr++ = (unsigned short)(bin_val / horizontal_bin);
				}
			}
		}
	}
}

void saveNI_data(long num_frames, char filename[MAX_PATH], short int numBNC_chan, short int BNC_ratio, float64 *NI_data, int delay, int isTwoK)
{
	// write BNC data
	char BNCname[MAX_PATH];
	char *p;
	int handle, i, m;
	short neg_numChan;
	float64 *NI_one_chan_data = NULL, *NI_dpt, *NI_spt;

	strcpy(BNCname, filename);
	p = strstr(BNCname, ".tsm");
	*p = '\0';
	strcat(BNCname, ".tbn");
	if ((handle = _open(BNCname, O_MODE, S_MODE)) == -1)
		return;

	NI_one_chan_data = (float64 *)_aligned_malloc(num_frames*BNC_ratio*sizeof(float64), 2);

	neg_numChan = -numBNC_chan;
	_write(handle, (void *)&neg_numChan, sizeof(neg_numChan));
	_write(handle, (void *)&BNC_ratio, sizeof(BNC_ratio));
	for (i = 0; i < numBNC_chan; i++) {
		NI_dpt = NI_one_chan_data;
		if (cdsFlag) {
			if (BNC_ratio)
				NI_spt = NI_data + numBNC_chan*(NI_add_frames + BNC_ratio) + i;
			else
				NI_spt = NI_data + numBNC_chan*NI_add_frames + i;
		}
		else {
			if (BNC_ratio > 2)
				NI_spt = NI_data + numBNC_chan * (max(0, (NI_add_frames - 1)) + BNC_ratio) + i;
			else
				NI_spt = NI_data + numBNC_chan * max(0, (NI_add_frames - 1)) + i;
		}
		NI_spt += shutter_add_frames * numBNC_chan*BNC_ratio;
		for (m = 0; m < num_frames*BNC_ratio; m++) {
			*NI_dpt++ = *NI_spt;
			NI_spt += numBNC_chan;
		}
		_write(handle, NI_one_chan_data, num_frames*BNC_ratio*sizeof(float64));
	}
	if (NI_one_chan_data)
		_aligned_free(NI_one_chan_data);
	_commit(handle);
	_close(handle);
}

void saveBNC_data(long num_frames, char filename[MAX_PATH], short int numBNC_chan, short int BNC_ratio, signed short int *BNC_data, char *cameraType, int cam_num_col, int stripes)
{
	// write BNC data
	char BNCname[MAX_PATH];
	char *p;
	int handle;

	strcpy(BNCname, filename);
	p = strstr(BNCname, ".tsm");
	*p = '\0';
	strcat(BNCname, ".tbn");
	if ((handle = _open(BNCname, O_MODE, S_MODE)) == -1)
		return;

	_write(handle, (void *)&numBNC_chan, sizeof(numBNC_chan));
	_write(handle, (void *)&BNC_ratio, sizeof(BNC_ratio));
	saveBNC_block(num_frames, handle, 0, numBNC_chan, BNC_ratio, BNC_data, cameraType, 0, cam_num_col, stripes);
	_commit(handle);
	_close(handle);
}

static unsigned long lastDiskBNC;
void saveBNC_block(long num_frames, int handle, long tt_frames_flag, short int numBNC_chan, short int BNC_ratio, signed short int *BNC_data, char *cameraType, long lastDiskFrame, int cam_num_col, int stripes)
{
	int k, i;
	unsigned short *BNC_dptr0, *BNC_sptr, *tmp_ptr, *chan_ptr;

	signed short int *tmpBNC = (signed short int *)_aligned_malloc(num_frames*BNC_ratio*numBNC_chan*sizeof(signed short int), 2);
	if (tmpBNC == NULL)
		return;

	BNC_sptr = (unsigned short *)BNC_data;
	BNC_dptr0 = (unsigned short *)tmpBNC;
	if (strstr(cameraType, "DW")) {
		BNC_sptr += cam_num_col;
		for (i = 0; i < num_frames; i++) {
			for (k = 0; k < numBNC_chan; k++) {
				tmp_ptr = BNC_dptr0 + k*num_frames + i;
				chan_ptr = BNC_sptr + k;
				*tmp_ptr = *chan_ptr;
				*tmp_ptr = (*tmp_ptr) << 2;
			}
			BNC_sptr += cam_num_col;
		}
	}
	else if (strstr(cameraType, "CCD67")) {
		if (lastDiskFrame <= 1)
			BNC_sptr += cam_num_col*2;
		for (i = 0; i < num_frames; i++) {
			for (k = 0; k < numBNC_chan; k++) {
				tmp_ptr = BNC_dptr0 + k*num_frames*BNC_ratio + i*BNC_ratio;
				for (int m = 0; m < BNC_ratio; m++) {
					if (m < 32)
						chan_ptr = BNC_sptr + m*8 + k;
					else
						chan_ptr = BNC_sptr + cam_num_col + (m-32)*8 + k;
					*tmp_ptr = *chan_ptr;
					*tmp_ptr = (*tmp_ptr) << 2;
					tmp_ptr ++;
				}
			}
			BNC_sptr += cam_num_col*2;
		}
	}
	else {
		BNC_sptr += cam_num_col*2;
		for (i = 0; i < num_frames; i++) {
			for (k = 0; k < numBNC_chan; k++) {
				tmp_ptr = BNC_dptr0 + k*num_frames + i;
				chan_ptr = BNC_sptr + k;
				*tmp_ptr = *chan_ptr;
				*tmp_ptr = (*tmp_ptr) << 2;
			}
			BNC_sptr += cam_num_col*2;
		}
	}

	if (tt_frames_flag) {
		unsigned long BNC_Length = (unsigned long)num_frames*BNC_ratio*2;
		signed short *BNC_ptr = (signed short *)tmpBNC;
		for (k = 0; k < numBNC_chan; k++) {
			_lseek(handle,sizeof(BNC_ratio)+sizeof(numBNC_chan)+tt_frames_flag*BNC_ratio*k*2+max(lastDiskFrame-1, 0)*BNC_ratio*2,SEEK_SET);
			_write(handle,BNC_ptr,BNC_Length);
			BNC_ptr += num_frames*BNC_ratio;
		}
	}
	else {
		unsigned long BNC_Length = (unsigned long)num_frames*BNC_ratio*numBNC_chan*2;
		memcpy(BNC_data, tmpBNC, BNC_Length);
		_write(handle, (signed short *)BNC_data, BNC_Length);
	}
	_aligned_free(tmpBNC);
}

void SM_makeHeader(int images, int numExposures, int rows, int columns, char *camera, double exposure)
{
	char buf[81];
	int card = 0, year = 2013, month = 4, day = 21, hour = 10, minute = 20, second = 40;

	memset(&FitsHeader, 0x20, sizeof(FitsHeader));
	sprintf(buf, "SIMPLE  = %20s", "T");
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "BITPIX  = %20d", 16);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "NAXIS   = %20d", 2);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "NAXIS1  = %20d", columns);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "NAXIS2  = %20d", rows);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "NAXIS3  = %20d", images);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "DATE    = %04d-%02d-%02dT%02d:%02d:%02d", year, month, day, hour, minute, second);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	strcpy(buf, "ORIGIN  = SciMeasure Analytical Systems, Inc.");
	strncpy(FitsHeader[card++], buf, strlen(buf));
	strcpy(buf, "CREATOR = Turbo-SM");
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "INSTRUME= %s, %s", camera, config_list[curConfig]);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "EXPOSURE= %20.10lf", exposure*numExposures);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "SATURATE= %20d", 16383);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "DATAMAX = %20.0lf", 16383.0);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "DATAMIN = %20.0lf", 0.0);
	strncpy(FitsHeader[card++], buf, strlen(buf));
	sprintf(buf, "END");
	strncpy(FitsHeader[card++], buf, strlen(buf));
}

void
subtractCDS(unsigned short int *image_data, int loops, unsigned int width, unsigned int height, unsigned int length, int factor, int QUAD, int TWOK, int num_pdvChan)
{
	int l, m, n, row, col, doublecols = 0, rows = 0, cols = 0;
	unsigned short int *new_data, *old_data, *reset_data;
	int CDS_add = 256;
	int step_cnt, total_cds_loops;
	double step_size;
	char str[256];

	total_cds_loops = loops;
	if (acq_hwndPB && !Ddisk_flag) {
		SendMessage(acq_hwndPB, PBM_SETPOS, (WPARAM)0, 0);
		step_cnt = 0;
		step_size = ((double)num_steps) / total_cds_loops;
	}

	if (Falcon_flag) {
		rows = height / 2;
		cols = width;// *2;
		new_data = image_data;
		reset_data = image_data + cols;
		old_data = image_data + length / 2;
	}
	else if (TWOK) {
		if (factor == 1)
			rows = height * (num_pdvChan>>1);
		else
			rows = (height + 2) / (2 * factor) - 2;
		cols = width / num_pdvChan;
		doublecols = cols * 2;
		new_data = image_data;
		reset_data = image_data + cols;
		if (factor == 1)
			old_data = image_data + length / 2;
		else
			old_data = image_data + (rows + 2)*doublecols;
	}
	else if (QUAD) {
		rows = height / 2;
		cols = width;
		new_data = image_data;
		reset_data = image_data + width;
		old_data = image_data + length / 2;
	}
	else {
		rows = height;
		cols = width / 2;
		new_data = image_data;
		reset_data = image_data + cols;
		old_data = image_data + length / 2;
	}
	if (factor == 1) {
		for (--loops, m = 0; loops > 0; --loops, ++m) {
			for (row = 0; row < rows; ++row) {
				for (col = cols; col; --col)
					*new_data++ = CDS_add + *old_data++ - *reset_data++;
				new_data += cols;
				reset_data += cols;
				old_data += cols;
			}
			if (acq_hwndPB && !Ddisk_flag) {
				sprintf(str, "CDS Subtraction: %d of %d images", m, total_cds_loops);
				SetWindowText(acq_hwndPB, str);
				while ((int)(m*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(acq_hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
			}
		}
	}
	else {
		for (m = 1; loops; --loops, m += factor) {
			for (n = 4; n; --n) {
				for (l = factor - 1; l; --l, ++m) {
					for (row = rows; row; --row) {
						for (col = cols; col; --col)
							*new_data++ = CDS_add + *old_data++ - *reset_data++;
						new_data += cols;
						old_data += cols;
						reset_data += cols;
					}
					new_data += doublecols * 2;
					old_data += doublecols * 2;
					reset_data += doublecols * 2;
				}
				if (loops > 0) {
					old_data += width * 3 * height / 4 - doublecols;
					for (row = rows; row; --row) {
						for (col = cols; col; --col)
							*new_data++ = CDS_add + *old_data++ - *reset_data++;
						new_data += cols;
						old_data += cols;
						reset_data += cols;
					}
					old_data -= width * 3 * height / 4 - doublecols;
					new_data += doublecols;
					old_data += doublecols;
					reset_data += doublecols;
					++m;
					m -= factor;
				}
			}
		}
	}
}

extern int image_chan_order[MAXOFFSETS];
int
makeLookUpTable(unsigned int *Lut, int image_width, int image_height, int file_width, int file_height, int stripes, int layers, int factor, int QUAD, int CDS, int ROTATE, int TWOK)
{
//	int SEGS, file_offset, image_offset, frame_length, image_length, file_length;
	int SEGS, image_length, file_length;
//	int frame, segment, row, rowIndex, destIndex, rows, cols, swid, dwid, srcIndex;
	int segment, row, rowIndex, destIndex, rows, cols, swid, dwid, srcIndex;
	static int twokchannel[] = { 3, 2, 0, 1, 7, 6, 4, 5, 9, 8, 10, 11, 13, 12, 14, 15 };
	static int onekchannel[] = { 3, 2, 0, 1, 5, 4, 6, 7 };
	int *channelOrder = NULL;
	int num_pdvChan;

	
	SEGS = stripes*layers;
	image_length = image_width*image_height;
	file_length = file_width*file_height;
	rows = file_height/layers;

	if (TWOK) {
		if (stripes == 8) {
			channelOrder = twokchannel;
			num_pdvChan = 4;
		}
		else {
			channelOrder = onekchannel;
			num_pdvChan = 2;
		}
	}
	swid = image_width / stripes;
	dwid = file_width / stripes;

#ifdef DAVINCIBITFLIP
	int tab, bit;
	unsigned int val;
	unsigned __int64 entry, *vptr;

	for (tab = 0, vptr = bitLut; tab < 4; ++tab)
		for (val = 0; val < 0x10000; ++val, ++vptr) {
			entry = 0;
			for (bit = 0; bit < 16; ++bit)
				if (val&bitMask[bit])
					entry |= bitPattern[tab][bit];
			*vptr = entry;
		}
#endif


	if (Falcon_flag) {
		omp_set_num_threads(omp_get_num_procs());
#pragma omp parallel
		{
			int segment;

#pragma omp parallel for private(row,cols,segment,rowIndex,srcIndex,destIndex)
			for (segment = 0; segment < stripes; ++segment) {
				srcIndex = (segment == 0);
				for (row = 0, rowIndex = 0; row < rows; ++row, rowIndex += file_width) {
					for (destIndex = rowIndex + dwid * segment, cols = dwid; cols > 0; --cols, ++destIndex) {
						Lut[destIndex] = srcIndex;
						srcIndex += SEGS;
					}
					if (CDS)
						srcIndex += dwid * SEGS;
				}
			}
#pragma omp parallel for private(row,cols,segment,rowIndex,srcIndex,destIndex)
			for (segment = stripes; segment < SEGS; ++segment) {
				srcIndex = segment;
				for (row = 0, rowIndex = file_length - file_width; row < rows; ++row, rowIndex -= file_width) {
					for (destIndex = rowIndex + dwid * (segment - SEGS / 2), cols = dwid; cols > 0; --cols, ++destIndex) {
						Lut[destIndex] = srcIndex;
						srcIndex += SEGS;
					}
					if (CDS)
						srcIndex += dwid * SEGS;
				}
			}
		}
	}
	else if (TWOK) {
		omp_set_num_threads(omp_get_num_procs());
#pragma omp parallel
		{
			int segment;

#pragma omp parallel for private(row,cols,segment,rowIndex,srcIndex,destIndex)
			for (segment = 0; segment < stripes; ++segment) {
				srcIndex = channelOrder[segment] % 4;
				if (num_pdvChan == 4)
					srcIndex += (image_length / 4)*(channelOrder[segment] / 4);
				for (row = 0, rowIndex = 0; row < rows; ++row, rowIndex += file_width) {
					for (destIndex = rowIndex + dwid*segment, cols = dwid; cols > 0; --cols, ++destIndex) {
						Lut[destIndex] = srcIndex;
						srcIndex += SEGS / num_pdvChan;
					}
					if (CDS)
						srcIndex += dwid*SEGS / num_pdvChan;
				}
			}
#pragma omp parallel for private(row,cols,segment,rowIndex,srcIndex,destIndex)
			for (segment = stripes; segment < SEGS; ++segment) {
				srcIndex = channelOrder[segment] % 4;
				if (num_pdvChan == 4)
					srcIndex += (image_length / 4)*(channelOrder[segment] / 4);
				else
					srcIndex += (image_length / 2);
				for (row = 0, rowIndex = file_length - file_width; row < rows; ++row, rowIndex -= file_width) {
					for (destIndex = rowIndex + dwid*(segment - SEGS / 2), cols = dwid; cols > 0; --cols, ++destIndex) {
						Lut[destIndex] = srcIndex;
						srcIndex += SEGS / num_pdvChan;
					}
					if (CDS)
						srcIndex += dwid*SEGS / num_pdvChan;
				}
			}
		}
	}
	else {
		if (CDS) {
			swid = swid / 2;
			image_width = image_width / 2;
			image_length = image_length / 2;
		}
		if (layers == 2) {
			// TOP LAYER
			if (QUAD) {
				for (segment = 0; segment < SEGS; segment += 4) {
					srcIndex = (channelOrder == NULL) ? segment : channelOrder[segment];
					for (row = 0, rowIndex = 0; row < rows; ++row, rowIndex += image_width) {
						for (destIndex = rowIndex + (segment / 4) * 2 * swid, cols = swid; cols > 0; --cols, ++destIndex) {
							Lut[destIndex] = srcIndex;
							srcIndex += SEGS;
						}
						if (CDS)
							srcIndex += swid*SEGS;
					}
				}
				for (segment = 1; segment < SEGS; segment += 4) {
					srcIndex = (channelOrder == NULL) ? segment : channelOrder[segment];
					for (row = 0, rowIndex = 0; row < rows; ++row, rowIndex += image_width) {
						for (destIndex = rowIndex + (segment / 4 + 1) * 2 * swid - 1, cols = swid; cols > 0; --cols, --destIndex) {
							Lut[destIndex] = srcIndex;
							srcIndex += SEGS;
						}
						if (CDS)
							srcIndex += swid*SEGS;
					}
				}
				for (segment = 2; segment < SEGS; segment += 4) {
					srcIndex = (channelOrder == NULL) ? segment : channelOrder[segment];
					for (row = 0, rowIndex = image_length - image_width; row < rows; ++row, rowIndex -= image_width) {
						for (destIndex = rowIndex + (segment / 4) * 2 * swid, cols = swid; cols > 0; --cols, ++destIndex) {
							Lut[destIndex] = srcIndex;
							srcIndex += SEGS;
						}
						if (CDS)
							srcIndex += swid*SEGS;
					}
				}
				for (segment = 3; segment < SEGS; segment += 4) {
					srcIndex = (channelOrder == NULL) ? segment : channelOrder[segment];
					for (row = 0, rowIndex = image_length - image_width; row < rows; ++row, rowIndex -= image_width) {
						for (destIndex = rowIndex + ((segment / 4) + 1) * 2 * swid - 1, cols = swid; cols > 0; --cols, --destIndex) {
							Lut[destIndex] = srcIndex;
							srcIndex += SEGS;
						}
						if (CDS)
							srcIndex += swid*SEGS;
					}
				}
			}
			else {
				/*				for (segment = 1; segment < SEGS; segment += 2) {
									srcIndex = (channelOrder == NULL) ? segment : channelOrder[segment];
									for (row = rows - 1, rowIndex = (rows - 1)*image_width; row >= rows; --row, rowIndex -= image_width) {
										for (destIndex = rowIndex + (segment / 2)*swid, cols = swid; cols > 0; --cols, ++destIndex) {
											Lut[destIndex] = srcIndex;
											srcIndex += SEGS;
										}
									}
								}*/
								/*					//  CCID83 noise prog
								int cam_board = 4;
								long board_size = Height*Width/2;
								for (segment = 0; segment < SEGS/cam_board; ++segment) {
								srcIndex = segment*cam_board;
								for (row = 0, rowIndex = 2L*segment*Height*Width/SEGS; row < rows/layers; ++row,rowIndex += Width) {
								for (destIndex = rowIndex, cols = swid; cols > 0; --cols, ++destIndex) {
								Lut[destIndex] = srcIndex;
								Lut[destIndex + cols*2 - 1] = srcIndex + 3;
								Lut[destIndex + board_size] = srcIndex + 1;
								Lut[destIndex + board_size + cols*2 - 1] = srcIndex + 2;
								srcIndex += SEGS;
								}
								}
				}*/

				// CCID79 && CCID66
				// TOP LAYER
				int mirror = 1, inc = -1; 
				rows = file_height;
				for (segment = 0; segment < SEGS / 2; segment++) {
					srcIndex = image_chan_order[segment];
					for (row = 0, rowIndex = 0; row < rows >> 1; ++row, rowIndex += image_width) {
						for (destIndex = rowIndex + (segment + mirror)*swid - mirror, cols = swid; cols > 0; --cols, destIndex += inc) {
							Lut[destIndex] = srcIndex;
							srcIndex += SEGS;
						}
					}
				}
				// BOTTOM LAYER
				for (segment = SEGS / 2; segment < SEGS; segment++) {
					srcIndex = image_chan_order[segment];
					for (row = rows - 1, rowIndex = (rows - 2)*image_width; row >= rows >> 1; --row, rowIndex -= image_width) {
						for (destIndex = rowIndex + (segment + mirror)*swid - mirror, cols = swid; cols > 0; --cols, destIndex += inc) {
							Lut[destIndex] = srcIndex;
							srcIndex += SEGS;
						}
					}
				}
				if (ROTATE) {
					int half_length = image_length/2;
					unsigned int temp;

					for (int i = 0, j = image_length - 1; i < half_length; ++i, --j) {
						temp = Lut[i];
						Lut[i] = Lut[j];
						Lut[j] = temp;
					}
				}
				if (multi_pix_read > 1) {
					unsigned int *lut_tmp = (unsigned int *)_aligned_malloc(file_length * sizeof(lut[0]), 2);
					int img_jump = image_width / multi_pix_read;
					srcIndex = 0;
					destIndex = 0;
					for (int n = 0; n < file_height; n++) {
						for (int m = 0; m < img_jump; m++) {
							lut_tmp[destIndex] = lut[srcIndex];
							srcIndex++;
							for (int i = 1; i < multi_pix_read; i++) {
								lut_tmp[destIndex + img_jump*i] = lut[srcIndex];
								srcIndex++;
							}
							destIndex++;
						}
						destIndex += img_jump * (multi_pix_read - 1);
					}
					memcpy(lut, lut_tmp, file_length * sizeof(lut[0]));
					_aligned_free(lut_tmp);
				}
			}
		}
		else { // LAYERS != 2
			if (SEGS == 20) {
				if (ROTATE) { // rotate image
					for (segment = 0; segment < SEGS; ++segment) {
						srcIndex = (channelOrder == NULL) ? segment : channelOrder[segment];
						for (row = 0, rowIndex = image_height*image_width; row < rows; ++row, rowIndex -= image_width) {
							for (destIndex = rowIndex - segment*swid - 1, cols = swid; cols > 0; --cols, --destIndex) {
								Lut[destIndex] = srcIndex;
								srcIndex += SEGS;
							}
						}
					}
				}
				else {
					for (segment = 0; segment < SEGS; ++segment) {
						srcIndex = (channelOrder == NULL) ? segment : channelOrder[segment];
						for (row = 0, rowIndex = 0; row < rows; ++row, rowIndex += image_width) {
							for (destIndex = rowIndex + segment*swid, cols = swid; cols > 0; --cols, ++destIndex) {
								Lut[destIndex] = srcIndex;
								srcIndex += SEGS;
							}
						}
					}
				}
			}
			else {
				if (ROTATE) { // rotate image
					for (segment = 0; segment < SEGS; ++segment) {
						srcIndex = 4 * (segment % 4) + segment / 4; // DM0018 segment mapping for 16 channels
						for (row = 0, rowIndex = image_height*image_width; row < rows; ++row, rowIndex -= image_width) {
							for (destIndex = rowIndex - segment*swid - 1, cols = swid; cols > 0; --cols, --destIndex) {
								Lut[destIndex] = srcIndex;
								srcIndex += SEGS;
							}
						}
					}
				}
				else {
					srcIndex = 0;
					destIndex = 0;
					if (SEGS == 2) {
						for (row = 0; row < rows; ++row) {
							for (cols = swid; cols > 0; --cols, ++destIndex) {
								Lut[destIndex] = srcIndex;
								srcIndex += SEGS;
							}
							srcIndex -= image_width - 1;
							destIndex += swid-1;
							for (cols = swid; cols > 0; --cols, --destIndex) {
								Lut[destIndex] = srcIndex;
								srcIndex += SEGS;
							}
							srcIndex -= SEGS - 1;
							destIndex += swid+1;
						}
					}
					else {
						for (row = 0; row < rows; ++row) {
							for (segment = 0; segment < SEGS; ++segment) {
								for (cols = swid; cols > 0; --cols, ++destIndex) {
									Lut[destIndex] = srcIndex;
									srcIndex += SEGS;
								}
								srcIndex -= image_width - 1;
							}
							srcIndex += image_width*(1 + CDS) - SEGS;
						}
					}
				}
			}
		}
	}
	return 0;
}

void frame_deInterleave(int length, unsigned *lookuptable, unsigned short *old_images, unsigned short *new_images)
{

	for (int i = 0; i < length; ++i)
		*(new_images++) = old_images[*(lookuptable++)];
	
}

/*****for NI acquisition***/
#ifdef _USE_NI
// add c:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\lib64\msvc\NIDAQmx.lib to Linker>>Input>>Additional Dependencies
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	int32   error = 0;
	char    errBuff[2048] = { '\0' };

	// Check to see if an error stopped the task.
	DAQmxErrChk(status);

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		DAQmxClearTask(taskHandle);
		fprintf(stdout,"DAQmx Error: %s\n", errBuff);
	}
	return 0;
}
#endif
/***end of NI***/
