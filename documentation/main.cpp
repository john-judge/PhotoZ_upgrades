#include "edtinc.h"
#include "time.h"

/* Short program that I use to test all 8 of the camera's programs
 * and print out information about the camera.
 * This provides a good example of how to acquire images and change
 * framerates and images sizes etc.
 * Most of the sizes are hardcoded for the specific camera that I
 * was testing
 * The Camera class was taken directly from PhotoZ
 */

class Camera {
public:
	char edt_devname[128];
	int unit;					// Some parameters that can be set, but are
	int channel;				// usually just 0
	int numbufs;				// ring buffers
	int m_width, m_height, m_depth;
	PdvDev *pdv_p;
	int timeouts, m_num_timeouts;
	int last_timeouts;
	int overruns;
	bool recovering_timeout;

public:
	Camera();
	~Camera();

	int open_channel();
	void start_images(int);
	unsigned char* wait_image();
	void serial_write(const char *buf);
	void serial_read(char *buf, int size);

	int num_timeouts();

	int width();
	int height();
	int depth();
	char* devname();

};



int main(int argc, char **argv)
{
	int numPts = 1000;
	const int num_progs = 8;
	const int bufsize = 80;

	int i, j;
	clock_t start, end;
	char buf[80];
	memset(buf, 0, sizeof(char) * bufsize);

	return 0;

	Camera cam;
	if (cam.open_channel())
	{
		printf("Couldn't open camera\n");
		return 1;
	}
	unsigned char *image;
	
	printf("cameracfg: %s\n", cam.pdv_p->dd_p->cfgname);
	//return 0;

	for (i = 0; i < 8; i++) {
		cam.serial_read(buf, bufsize);
		printf("%d: %s\n", i, buf);
	}

	/*
	cam.serial_write("@JOE?\r");
	cam.serial_read(buf, bufsize);
	printf("%s\n", buf);
	*/

	cam.serial_write("@PRG?\r");
	cam.serial_read(buf, bufsize);
	printf("%s\n", buf);

	cam.serial_write("@AAM?\r");
	cam.serial_read(buf, bufsize);
	printf("%s\n", buf);

	cam.serial_write("@TXC?\r");
	cam.serial_read(buf, bufsize);
	printf("%s\n", buf);

	cam.serial_write("@REP?\r");
	cam.serial_read(buf, bufsize);
	printf("%s\n", buf);

	
	for (j = 0; j < num_progs; j++) {
		printf("=========== %d ==========\n", j);
		sprintf_s(buf, "@PRG %d\r", j);
		cam.serial_write(buf);

		cam.serial_write("@PRG?\r");
		cam.serial_read(buf, bufsize);
		printf("%s\n", buf);

		sprintf_s(buf, "@RCL %d\r", j);
		cam.serial_write(buf);
		cam.serial_read(buf, bufsize);
		printf("%s\n", buf);

		cam.serial_write("@AAM?\r");
		cam.serial_read(buf, bufsize);
		printf("%s\n", buf);

		cam.serial_write("@REP?\r");
		cam.serial_read(buf, bufsize);
		printf("%s\n", buf);

		Sleep(100);

		if (j == 0)
			pdv_setsize(cam.pdv_p, 2048, 1024);
		else if (j == 1)
			pdv_setsize(cam.pdv_p, 2048, 100);
		else if (j == 2)
			pdv_setsize(cam.pdv_p, 1024, 320);
		else if (j == 3)
			pdv_setsize(cam.pdv_p, 1024, 160);
		else if (j == 4)
			pdv_setsize(cam.pdv_p, 512, 160);
		else if (j == 5)
			pdv_setsize(cam.pdv_p, 512, 80);
		else if (j == 6)
			pdv_setsize(cam.pdv_p, 256, 60);
		else
			pdv_setsize(cam.pdv_p, 256, 40);

		int width = cam.width();
		int height = cam.height();

		if (j == 0)
			numPts = 400;
		else if (j < 5)
			numPts = 2000;
		else
			numPts = 10000;

		cam.start_images(numPts);

		for (i = 0; i < numPts; i++) {
			image = cam.wait_image();
			if (i == 0)
				start = clock();

			if (cam.num_timeouts() > 10)
			{
				printf("Too many timeouts\n");
				return cam.num_timeouts();
			}
		}
		end = clock();

		if (cam.num_timeouts() > 0)
			printf("timeouts: %d\n", cam.num_timeouts());

		double runtime = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%d.   %dx%d   %fsec   %fHz\n\n", j, width, height, runtime, numPts / runtime);
	}
	return 0;
}



Camera::Camera() {
	edt_devname[0] = '\0';
	strcpy(edt_devname, EDT_INTERFACE);
	unit = 0;
	channel = 0;
	numbufs = 4;
	pdv_p = NULL;
	m_width = m_height = m_depth = -1;
	timeouts = last_timeouts = m_num_timeouts = 0;
	overruns = 0;
	recovering_timeout = false;
}

Camera::~Camera() {
	if (pdv_p)
		pdv_close(pdv_p);
	pdv_p = NULL;
}

int Camera::open_channel() {
	if (pdv_p)
		pdv_close(pdv_p);
	if ((pdv_p = pdv_open_channel(edt_devname, unit, channel)) == NULL)
		return 1;

	pdv_flush_fifo(pdv_p);
	m_width = pdv_get_width(pdv_p);
	m_height = pdv_get_height(pdv_p);
	m_depth = pdv_get_depth(pdv_p);

	/* allocate ring buffers
	*/
	pdv_multibuf(pdv_p, numbufs);

	// printf data for now
	printf("interlace method: %d\n", pdv_interlace_method(pdv_p));
	printf("width: %d\nheight: %d\ndepth: %d\n", m_width, m_height, m_depth);
	printf("device name: %s\n", edt_devname);

	pdv_set_waitchar(pdv_p, 1, '\r');
	return 0;
}



void Camera::start_images(int n) {
	pdv_start_images(pdv_p, n);
}

unsigned char* Camera::wait_image() {
	unsigned char *image = pdv_wait_image(pdv_p);

	if (edt_reg_read(pdv_p, PDV_STAT) & PDV_OVERRUN)
		++overruns;

	timeouts = pdv_timeouts(pdv_p);
	if (timeouts > last_timeouts) {
		pdv_timeout_restart(pdv_p, TRUE);
		last_timeouts = timeouts;
		m_num_timeouts++;
		recovering_timeout = true;
	}
	else if (recovering_timeout) {
		pdv_timeout_restart(pdv_p, TRUE);
		recovering_timeout = false;
	}

	return image;
}

void Camera::serial_write(const char *buf)
{
	char buffer[512];
	// flush any junk
	pdv_serial_read(pdv_p, buffer, 512-1);
	memset(buffer, 0, sizeof(char) * 512);
	strcpy_s(buffer, buf);
	printf("%s\n", buffer);
	pdv_serial_write(pdv_p, buffer, strlen(buffer));
}
void Camera::serial_read(char *buf, int size)
{
	pdv_serial_wait(pdv_p, 1000, size-1);
	int n = pdv_serial_read(pdv_p, buf, size-1);
	if (n < size)
		buf[n] = '\0';
	buf[size-1] = '\0';
}

int Camera::num_timeouts() {
	return m_num_timeouts;
}
int Camera::width() {
	m_width = pdv_get_width(pdv_p);
	return m_width;
}
int Camera::height() {
	m_height = pdv_get_height(pdv_p);
	return m_height;
}
int Camera::depth() {
	m_depth = pdv_get_depth(pdv_p);
	return m_depth;
}
char* Camera::devname() {
	return edt_devname;
}

int SM_initCamera(int load_config_flag, int close_flag, int load_pro_flag)
{
	char command[256];
	int prog;
	char *p;
	char home_drive[256];
	int config_id = 0;

	if (close_flag > 0)
		SM_pdv_close();

	char buf[256], tmp_str[256];
	int m, i;
	FILE *fp;
	char flagFname[MAX_PATH];
	//Read Image Chan_order
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\image_chan_order.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		for (int i = 0; i < layers_lib[0] * stripes_lib[0] * pdv_chan_num; i++) {
			if (!fgets(buf, 255, fp))
				goto exit;
			sscanf(buf, "%d", &image_chan_order[i]);
		}
	}

	strcpy(home_drive, home_dir);
	if (p = strstr(home_drive, ":"))
		*p = '\0';

	//read sub format for 2K
	seqROI_flag = 0;
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\draw_seqROIs.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		memset(seqROI_ar, 0, sizeof(seqROI_ar));
		if (!fgets(buf, 255, fp))
			goto exit;
		for (i = 0; i < 8; i++) {
			if (!fgets(buf, 255, fp))
				goto exit;
			if (!strcmp(buf, ""))
				goto exit;
			strcpy(tmp_str, buf);
			m = 0;
			while (p = strstr(tmp_str, ",")) {
				strcpy(buf, p + 1);
				*p = '\0';
				sscanf(tmp_str, "%d", &seqROI_ar[m][i]);
				strcpy(tmp_str, buf);
				m++;
			}
			seqROI_flag = m;
		}
	exit:
		fclose(fp);
	}

	//read Disk Data flag
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\DiskData_flag.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		writeToDisk_flag = true;
		fclose(fp);
	}
	else
		writeToDisk_flag = false;

	if (load_config_flag && !demoFlag) {
		char edt_cfg_name[_MAX_PATH];
		sprintf(edt_cfg_name, "%s:\\EDT\\pdv\\camera_config\\%s", home_drive, liveConfig_list[curConfig]);
		if (TwoK_flag) {
			config_id = checkCfgLUT();
			if (!config_loaded || !config_id) {
				if (FOPEN_S(fp, edt_cfg_name, "r"))
					fclose(fp);
				else {
					sprintf(command, "%s does not exist.", edt_cfg_name);
					MessageBox(main_window_handle, command, "message", MB_OK);
					return false;
				}
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv0_0 -f %s", home_drive, edt_cfg_name);
				system(command);
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv0_1 -f %s", home_drive, edt_cfg_name);
				system(command);
				if (pdv_chan_num > 2) {
					sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv1_0 -f %s", home_drive, edt_cfg_name);
					system(command);
					sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv1_1 -f %s", home_drive, edt_cfg_name);
					system(command);
				}
				config_loaded = 1;
			}
		}
		else {
			if (FOPEN_S(fp, edt_cfg_name, "r"))
				fclose(fp);
			else {
				sprintf(command, "%s does not exist.", edt_cfg_name);
				MessageBox(main_window_handle, command, "message", MB_OK);
				return false;
			}
			if (pcdFlag == 1)
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv%d -f %s", home_drive, 1, edt_cfg_name);
			else
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv%d -f %s", home_drive, pdv_dev_unit, edt_cfg_name);
			system(command);
		}
	}

	if (demoFlag) {
		if (darkImage != NULL)
			_aligned_free(darkImage);
		darkImage = (signed short int *)_aligned_malloc(cam_num_col*cam_num_row * sizeof(signed short int), 2);
		resetZoom(1);
		return TRUE;	//	no cam
	}

	if (close_flag >= 0) {
		if (!SM_pdv_open(0))
			return FALSE;
	}

	if (load_pro_flag) {
		// determin if needed to stream to disk
		setStream_outputs();

		long repetitions = 0L;

		if (curCamGain >= 0)
			prog = 8 * curCamGain + curConfig;
		else
			prog = curConfig;
		sprintf(command, "@RCL %d", prog);
		SM_serial_command(command);

		sprintf(command, "@SEQ 0");
		SM_serial_command(command);

		if (AD_flag && !demoFlag) {
			// Set EventMode
			sprintf(command, "@ICW $CC; 3; $21; 0; 2");
			SM_serial_command(command);
			//Close shutter
			sprintf(command, "@ICW $CC; 3; $25; 1; 1");
			SM_serial_command(command);
			//Set ADAC rate
			if (strstr(cameraType, "DW")) {
				long AD_rate = (long)(50000.0*frame_interval / (8.2*BNC_ratio));
				sprintf(command, "@ICW $CC; 4; $14; $%02lx; $%02lx; $%02lx", (AD_rate & 0xff0000L) >> 16, (AD_rate & 0xff00L) >> 8, (AD_rate & 0xffL));
			}
			else if (strstr(cameraType, "CCD67")) {
				long AD_rate = (long)(50000.0 / 8.2);
				if (win64_flag)
					BNC_ratio = min((int)frame_interval, cam_num_col / 4);
				else
					BNC_ratio = 1;
				sprintf(command, "@ICW $CC; 4; $14; $%02lx; $%02lx; $%02lx", (AD_rate & 0xff0000L) >> 16, (AD_rate & 0xff00L) >> 8, (AD_rate & 0xffL));
			}
			else {
				if (curConfig == 4)
					sprintf(command, "@ICW $CC; 4; $14; 0; 2; 97");
				else if (curConfig == 5)
					sprintf(command, "@ICW $CC; 4; $14; 0; 7; 216");
				else if (curConfig == 6)
					sprintf(command, "@ICW $CC; 4; $14; 0; 4; 198");
				else if (curConfig == 0 || curConfig == 7)
					sprintf(command, "@ICW $CC; 4; $14; 0; 11; 214");
				else {
					long AD_rate = (long)(50000.0*frame_interval / (8.0*BNC_ratio));
					sprintf(command, "@ICW $CC; 4; $14; $%02lx; $%02lx; $%02lx", (AD_rate & 0xff0000L) >> 16, (AD_rate & 0xff00L) >> 8, (AD_rate & 0xffL));
				}
			}
			strcpy(AD_rate_command, command);
			SM_serial_command(_strupr(command));
			//set BNC 1-4 to bipolar
			sprintf(command, "@ICW $CC; 9; $13; 16; 16; 16; 16; 0; 0; 0; 0");
			SM_serial_command(command);
		}

		if (strstr(cameraType, "DW")) {
			SM_SetHourCursor();
			if (chipgainSave != curChipGain) {
				switch (curChipGain) {
				case 0:
					sprintf(command, "@ICW $48; 9; 0;1;0; 0;1;0; 0;1;0");
					break;
				case 1:
					sprintf(command, "@ICW $48; 9; 0;1;0; 0;1;0; 2;3;2");
					break;
				case 2:
					sprintf(command, "@ICW $48; 9; 2;3;2; 2;3;2; 2;3;2");
					break;
				}
				for (int i = 1; i < 132; i++)
					SM_serial_command(command);
			}
			sprintf(command, "@AAM %d", curCamGain);
			SM_serial_command(command);
			SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
		}
		else if (strstr(cameraType, "CCID83")) {
			if (start_line_lib[curConfig])
				noise_seq_flag = TRUE;
			else
				noise_seq_flag = FALSE;
		}
		else if (TwoK_flag) {
			if (config_loaded && config_id) {
				for (int i = 0; i < pdv_chan_num; ++i)
					pdv_setsize(pdv_pt[i], configLUT[config_id - 1].width[curConfig], configLUT[config_id - 1].height[curConfig]);
			}

			int hbin = 0;
			if (ccd_lib_bin[curConfig] > 1)
				hbin = 1;
			int start_line;
			if (start_line_lib[curConfig])
				start_line = start_line_lib[curConfig];
			else
				start_line = 65 + (1024 - cam_num_row * ccd_lib_bin[curConfig]);

			sprintf(command, "@SPI 0; 2");
			SM_serial_command(command);
			Sleep(20);
			sprintf(command, "@SPI 0; 0");
			SM_serial_command(command);
			Sleep(20);
			sprintf(command, "@SPI 0; 4");
			SM_serial_command(command);
			Sleep(20);
			sprintf(command, "@PSR %d; %d", ccd_lib_bin[curConfig], start_line);
			SM_serial_command(command);
			Sleep(20);
			sprintf(command, "@SPI 0; %d", hbin);
			SM_serial_command(command);
			Sleep(20);

			if (pdv_chan_num > 2)
				sprintf(command, "%s:\\EDT\\pdv\\setgap2k", home_drive);
			else
				sprintf(command, "%s:\\EDT\\pdv\\setgap1k", home_drive);
			system(command);

		}

		else if (strstr(cameraType, "128X")) {
			if (strstr(cameraType, "128X-V2")) {
				int psr_ar[] = { 1, 49, 57, 57, 1, 49, 57, 57 };
				if (start_line_lib[curConfig])
					sprintf(command, "@PSR 1; %d", start_line_lib[curConfig]);
				else
					sprintf(command, "@PSR 1; %d", psr_ar[curConfig]);
				Sleep(50);
				SM_serial_command(command);
				Sleep(20);
				sprintf(command, "@BIN %d", ccd_lib_bin[curConfig] - 1);
				SM_serial_command(command);
			}
			if (ndr_lib[curConfig] > 1)
				sprintf(command, "%s:\\EDT\\pdv\\setgap_c3", home_drive);
			else
				sprintf(command, "%s:\\EDT\\pdv\\setgap_80", home_drive);
			system(command);
		}

		if (ndr_lib[curConfig] == 1)
			repetitions = 0;
		else if (ndr_lib[curConfig] > 1) {
			if (curConfig == 7)
				repetitions = max(0, ndr_lib[curConfig] - 1);
			else
				repetitions = max(0, ndr_lib[curConfig] - 2);
		}
		else if (!ndr_lib[curConfig])
			repetitions = set_repetition();

		if (TwoK_flag && cds_lib[curConfig]) {
			setFocusRep(focusMode);
			OM[SM_FOCUSBOX].opt->grey = FALSE;
			OM[SM_FOCUS].opt->grey = FALSE;
		}
		else {
			if (pcdFlag) {		// This is to get rid of funny characters
				char ret_c[256];
				sprintf(command, "@REP?");
				SM_pdv_query_command(command, ret_c, 256);
			}

			sprintf(command, "@REP %ld", repetitions);
			SM_serial_command(command);

			OM[SM_FOCUSBOX].opt->grey = TRUE;
			OM[SM_FOCUS].opt->grey = TRUE;
		}
		if (!TwoK_flag && !strstr(cameraType, "128X"))
			loadCamOffsets();

		Sleep(50);
		sprintf(command, "@TXC 0");
		SM_serial_command(command);
		sprintf(command, "@SEQ 1");
		SM_serial_command(command);
		SM_pdv_vals();

		if (offset_chan_order[0] < 0 || cds_lib[curConfig] || TwoK_flag)
			OM[SM_AUTOOFFSET].opt->grey = TRUE;
		else
			OM[SM_AUTOOFFSET].opt->grey = FALSE;

		cam_num_col = config_num_col;
		cam_num_row = config_num_row;
		if (TwoK_flag) {
			if (!auto_scale_w_acq)
				auto_scale_w_acq = (pdv_chan_num / 2) * cam_num_col / ((1 + cds_lib[curConfig])*max(1, ccd_lib_bin[curConfig] / 2));
			auto_scale_w = auto_scale_w_acq;

			if (!file_frame_w)
				file_frame_w = (pdv_chan_num >> 1)*cam_num_col / ((1 + cds_lib[curConfig])*max(1, ccd_lib_bin[curConfig] / 2));
			else {
				if (file_frame_w < 10)
					file_frame_w = 10;
				else if (file_frame_w > (pdv_chan_num >> 1)*cam_num_col / (1 + cds_lib[curConfig]))
					file_frame_w = (pdv_chan_num >> 1)*cam_num_col / (1 + cds_lib[curConfig]);
			}
			if (file_frame_w < cam_num_row*(pdv_chan_num >> 1))
				cam_num_col = cam_num_row * (pdv_chan_num >> 1);
		}
		if (cam_num_col_last != cam_num_col || cam_num_row_last != cam_num_row) {
			clearTracePix();
			cam_num_row_last = cam_num_row;
			cam_num_col_last = cam_num_col;
			if (darkImage != NULL)
				_aligned_free(darkImage);
			darkImage = (signed short int *)_aligned_malloc(cam_num_col*cam_num_row*pdv_chan_num * sizeof(signed short int) / (1 + cds_lib[curConfig]), 2);
		}

		if (pcdFlag)
			live_factor = 2;
		else if (TwoK_flag) {
			live_factor = max(1, 1024 / config_num_row);
			if (ndr_lib[curConfig])
				live_factor *= 2;
		}
		else if (ndr_lib[curConfig] > 1)
			live_factor = ndr_lib[curConfig];
		else
			live_factor = (int)max(1, (1 + cds_lib[curConfig]) / frame_interval);

		// for CCID79 multiple pixel read
		if (strstr(cameraType, "CCID")) {
			multi_pix_read = super_frame_lib[curConfig];
			if (multi_pix_read > 0)
				multi_pix_av = 1;
			else {
				multi_pix_read = -multi_pix_read;
				multi_pix_av = 0;
			}
		}
		else {
			multi_pix_read = 0;
			multi_pix_av = 0;
		}

		single_img_size = (unsigned long)cam_num_col*cam_num_row*(1 + cds_lib[curConfig])*(1 + ndr_lib[curConfig])*pdv_chan_num*live_factor;
		if (single_img_data != NULL)
			_aligned_free(single_img_data);
		single_img_data = (signed short int *)_aligned_malloc(single_img_size * sizeof(signed short int) * 2, 2);
		if (single_img_float != NULL)
			_aligned_free(single_img_float);
		single_img_float = (float *)_aligned_malloc(single_img_size * sizeof(float), 2);
		if (!initLive(pdv_chan_num, cds_lib[curConfig], 1, layers_lib[curConfig], stripes_lib[curConfig], cameraType, QuadFlag, rotate_lib[curConfig], bad_pix_lib[curConfig]))
			return FALSE;
		if (newDarkFlag) {
			char ref_name[MAX_PATH];
			DeleteFile(strcat(strcpy(ref_name, tmp_file_dir), "\\record_ref.tsm"));
			dataExist = FALSE;
			darkExist = FALSE;
			d_darkExist = FALSE;
			deactivateImgMenu();
			darkSubFlag = FALSE;
			if (OM[SM_DKSUBBOX].attr && !d_darkExist)
				setChckBox(SM_DKSUBBOX);
		}

		disableBNCs();
		dataFile = FALSE;
		if (selectPixMode == DRAG_ERASE)
			selectPixMode = DRAG_MODE;
		resetScaleB();
		setToolPanel(1);
		repaintMenubox();
		resetZoom(1);
		if (photonTFlag && (strstr(cameraType, "CCD") || strstr(cameraType, "CCID")))
			readPHT_config2();
	}

	GetSystemTime(&lastSystemTime);
	return TRUE;
}



