//=============================================================================
// Camera.cpp
//=============================================================================
#include <vector>
#include <string>
#include <FL/fl_ask.h>
#include <omp.h>

#include "Camera.h"
#include <iostream>
#include "DapController.h"
#include "RecControl.h"		//added to get camGain on line 157

using namespace std;

#ifdef LILJOE
const char* Camera::LABEL[] = {
	"1000 Hz  80x80",
	"500 Hz   80x80",
	"125 Hz   80x80",
	"40 Hz    80x80",
	"10000 Hz 80x12",
	"3000 Hz  40x40",
	"5000 Hz  26x26",
	"2000 Hz  80x80"
};
const int Camera::WIDTH[] = { 80,	80,		80,		80,		80,		40,		26,		80 };
const int Camera::HEIGHT[] = { 80,	80,		80,		80,		12,		40,		26,		80 };
const int Camera::FREQ[] = { 1000,	500,	125,	40,		10000,	3000,	5000,	2000 };
#endif // LILJOE

#ifdef LILDAVE
const char* Camera::LABEL[] = {
	"200 Hz   2048x1024",
	"2000 Hz  2048x100",
	"1000 Hz  1024x320",
	"2000 Hz  1024x160",
	"2000 Hz  512x160",
	"4000 Hz  512x80",
	"5000 Hz  256x60",
	"7500 Hz  256x40" };

const int Camera::WIDTH[] = { 2048,	2048,	1024,	1024,	512,	512,	256,	256 };
const int Camera::HEIGHT[] = { 1024,	100,	320,	160,	160,	80,		60,		40 };
const int Camera::FREQ[] = { 200,	2000,	1000,	2000,	2000,	4000,	5000,	7500 };
const char* Camera::PROG[] = {
	"DM2K_2048x512.cfg",
	"DM2K_2048x50.cfg",
	"DM2K_1024x160.cfg",
	"DM2K_1024x80.cfg",
	"DM2K_1024x80.cfg",
	"DM2K_1024x40.cfg",
	"DM2K_1024x30.cfg",
	"DM2K_1024x20.cfg"
};

const double Camera::sm_lib_rates[] = { 0.20, 0.50, 1.0, 0.64, 1.0, 2.0, 1.25, 2.0 };
const double Camera::sm_cam_lib_rates[] = { 0.2016948, 0.5145356, 1.02354144, 0.6480876, 1.03412632, 2.05128256, 1.27713928, 2.0 };
const double Camera::sm_integration_increments[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.147 };

const int Camera::ccd_lib_bin[] = { 1, 1, 1, 2, 2, 2, 4, 1 };
const int Camera::config_load_lib[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
const int Camera::cds_lib[] = { 1, 1, 1, 1, 1, 1, 1, 0 };
const int Camera::ndr_lib[] = { 0, 0, 0, 0, 0, 0, 0, 1 };
const int Camera::stripes_lib[] = { 4, 4, 4, 4, 4, 4, 4, 4 };
const int Camera::layers_lib[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
const int Camera::rotate_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const int Camera::bad_pix_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const int Camera::start_line_lib[] = { 576, 888, 988, 574, 766, 926, 574, 976 };
const int Camera::super_frame_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const int Camera::NDR_start_lib[] = { 0, 0, 0, 0, 0, 0, 0, 11 };
const int Camera::NDR_inc_lib[] = { 0, 0, 0, 0, 0, 0, 0, 8 };
const int Camera::NDR_code_lib[] = { 0, 0, 0, 0, 0, 0, 0, 64 };
const int Camera::NDR_max_lib[] = { 0, 0, 0, 0, 0, 0, 0, 2000 };
const int Camera::reserve1_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const int Camera::reserve2_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const int Camera::reserve3_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
#endif // LILDAVE

Camera::Camera() {
	edt_devname[0] = '\0';
	strcpy(edt_devname, EDT_INTERFACE);

	pdv_pt[0] = NULL;
	pdv_pt[1] = NULL;
	pdv_pt[2] = NULL;
	pdv_pt[3] = NULL;

	timeouts[0] = timeouts[1] = timeouts[2] = timeouts[3] = last_timeouts = m_num_timeouts = 0;
	m_depth = -1;
	overruns = 0;
	recovering_timeout = false;
	m_program = 7;                // the camera always boots up with prg0
}

Camera::~Camera() {
	int retry = 0;
	for (int i = 0; i < 4; i++) {
		if (pdv_pt[i]) {
			while (pdv_close(pdv_pt[i]) < 0 && retry != 1) {
				retry = fl_choice("Failed to close PDV channel. If you choose not to retry, restart the camera manually.", "Retry", "Exit", "Debugging Info");
				if (retry == 2) {
					cout << "Current state of pdv_pt[]\n";
					for (int j = 0; j < 4; j++)  cout << "pdv_pt[" << j << "]: " << pdv_pt[j] << "\n";
				}
				if (retry != 1) cout << "Reattempting to close channel " << i << "...\n";
			}

			//pdv_pt[i] = NULL; // shouldn't be necessary, and may be misleading during memory dumps
		}
	}
}

// Old open channel used for Lil Joe
// DEPRECATED?
int Camera::open_channel() {			//converted to 4
	int error = 0;
	for (int i = 0; i < 4; i++)
	{
		if (pdv_pt[i] == NULL) {
			error = 1;
		}
		else {

			pdv_enable_external_trigger(pdv_pt[i], 0);	 // disable the trigger in case it was left on - it persists over restarts
			pdv_set_timeout(pdv_pt[i], 1000);
			pdv_flush_fifo(pdv_pt[i]);
			//    m_depth = pdv_get_depth(pdv_pt[ipdv]);	// allocate ring buffers
			pdv_multibuf(pdv_pt[i], 4);
			pdv_setsize(pdv_pt[i], WIDTH[m_program] / 2, HEIGHT[m_program] / 2);
		}
	}
	return error;
}

// Take single image for IPDVth channel
unsigned char* Camera::single_image(int ipdv)			//used by LiveFeed.cpp
{
	if (!pdv_pt[ipdv])
		return nullptr;
	pdv_flush_fifo(pdv_pt[ipdv]);
	pdv_start_image(pdv_pt[ipdv]);
	return pdv_wait_image(pdv_pt[ipdv]);
}

// Initialize camera and open all 4 channels
void Camera::init_cam()				// entire module based on code from Chun - sm_init_camera.cpp
{
	char command[80];
	cout << "camera line 133 init_cam  program #  " << PROG[m_program] << "  \n";
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_0 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_1 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_0 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_1 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);

	for (int i = 0; i < 4; i++) {
		if (pdv_pt[i]) {
			if (pdv_close(pdv_pt[i]) < 0) {
				cout << "When checking for already open channels, we failed to close channel i=" << i << " (Camera.cpp, line 151)\n";
			}
		}
		// according Chun:    
		pdv_pt[i] = pdv_open_channel(edt_devname,
			(i >> 1) | 1,	// pdv_units	 = [0, 0, 1, 1]
			i | 1);			// pdv_channels	 = [0, 1, 0, 1]
		if (!pdv_pt[i]) {
			cout << "Failed to open channel " << i << " Camera::init_cam()\n";
			return;
		}
	}

	cout << "PDV timeouts for channels:\n";
	for (int i = 0; i < 4; i++) {
		cout << "\t Channel " << i << "\t" << pdv_get_timeout(pdv_pt[i]) << " ms\n";
	}

	int hbin = 0;
	if (ccd_lib_bin[m_program] > 1) hbin = 1;
	int start_line;
	if (start_line_lib[m_program]) start_line = start_line_lib[m_program];
	else start_line = 65 + (1024 - 2 * HEIGHT[m_program] * ccd_lib_bin[m_program]);		// Chun says cam_num_row gets doubled - did not fully understand

	sprintf(command, "@SPI 0;2");
	serial_write(command);
	Sleep(20);
	sprintf(command, "@SPI 0;0");
	serial_write(command);
	Sleep(20);
	sprintf(command, "@SPI 0;4");
	serial_write(command);
	Sleep(20);
	sprintf(command, "@PSR %d; %d", ccd_lib_bin[m_program], start_line);
	serial_write(command);
	Sleep(20);
	sprintf(command, "@SPI 0; %d", hbin);
	serial_write(command);
	Sleep(20);
	
	sprintf(command, "c:\\EDT\\pdv\\setgap2k");// this is a batch job that calls pdb -u 0 -c 0 -f setgap200.txt four times for the different channels. The console indicates successful execution
	system(command);
	return;
}

// Starts images for all 4 channels.
void Camera::start_images() {					// converted to 4
	for (int i = 0; i < 4; i++) {
		if (!pdv_pt[i]) return; // Don't start any channels if a single channel is null
	}
	for (int i = 0; i < 4; i++) {
		pdv_flush_fifo(pdv_pt[i]);				 // pdv_timeout_restart(pdv_p, 0);	same as pdv_flush  7-4-2020
		pdv_multibuf(pdv_pt[i], 4);				// Suggested by Chun 5-14-2020		//change pdv_p to pdv_pt[0]  9-23-2020
		pdv_start_images(pdv_pt[i], 0);
		Sleep(100);
		cout << " cam start_images line 185  " << pdv_pt[0] << "\n";
	}
}

// Ends images for all 4 channels.
void Camera::end_images() {						//	converted to 4
	for (int i = 0; i < 4; i++) 
		end_images(i);
}

// Ends images for channel IPDV
void Camera::end_images(int ipdv) {
	if (pdv_pt[ipdv])
		pdv_start_images(pdv_pt[ipdv], 1);
}

// Get image info for IPDVth channel
void Camera::get_image_info(int ipdv) {
	cout << " program       " << m_program << "\n";
	cout << " allocated size      " << pdv_get_allocated_size(pdv_pt[ipdv]) << "\n";
	cout << " image size (bytes per image) " << pdv_get_imagesize(pdv_pt[ipdv]) << "\n";
	cout << " buffer size      " << pdv_image_size(pdv_pt[ipdv]) << "\n";
	cout << " image height      " << pdv_get_height(pdv_pt[ipdv]) << "\n";
	cout << " cam height      " << pdv_get_cam_height(pdv_pt[ipdv]) << "\n";
	cout << " image width      " << pdv_get_width(pdv_pt[ipdv]) << "\n";
	cout << " cam width       " << pdv_get_cam_width(pdv_pt[ipdv]) << "\n";
}

// Get buffer size for IPDVth channel
int Camera::get_buffer_size(int ipdv) {
	if (!pdv_pt[ipdv]) return -1;
	return pdv_get_imagesize(pdv_pt[ipdv]);
}

// Wait for image  for IPDVth channel
unsigned char* Camera::wait_image(int ipdv) {
	unsigned char *image = pdv_wait_image(pdv_pt[ipdv]);
	if (edt_reg_read(pdv_pt[ipdv], PDV_STAT) & PDV_OVERRUN)
		++overruns;
	timeouts[0] = pdv_timeouts(pdv_pt[ipdv]);
	if (timeouts[0] > last_timeouts) {
		cout << "Channel " << ipdv << " reached " << timeouts[ipdv] << " timeouts. (Camera::wait_image())\n";
		pdv_timeout_restart(pdv_pt[ipdv], TRUE);
		last_timeouts = timeouts[0];
		m_num_timeouts++;
		recovering_timeout = true;
	}
	else if (recovering_timeout) {
		pdv_timeout_restart(pdv_pt[ipdv], TRUE);
		cout << "Channel " << ipdv << " was restarted due to timeouts. (Camera::wait_image())\n";
		recovering_timeout = false;
	}
	return image;
}

// Write a command from buf to IPDVth channel
// Only channel 0 of Little Dave supports serial write
void Camera::serial_write(const char *buf)
{
	char buffer[512];
	pdv_serial_read(pdv_pt[0], buffer, 512 - 1);
	memset(buffer, 0, sizeof(char) * 512);
	strcpy_s(buffer, buf);
	if (pdv_serial_write(pdv_pt[0], buffer, strlen(buffer)) < 0) {
		cout << "Failed to serial write to channel 0. Failed command:\n" << buffer << "\n";
	}
}

/*void Camera::serial_read(char *buf, int size)
{
	pdv_serial_wait(pdv_pt[ipdv], 1000, size-1);
	int n = pdv_serial_read(pdv_pt[ipdv], buf, size-1);
	if (n < size)
		buf[n] = '\0';
	buf[size-1] = '\0';
}*/

// Get timeouts for IPDVth channel
int Camera::num_timeouts(int ipdv) {
	return timeouts[ipdv];
}

// Get which program is loaded currently
int Camera::program()
{
	return m_program;
}

// Set which program is loaded currently and initialize the camera
void Camera::setCamProgram(int p)
{
	m_program = p;
	init_cam();							//added 11/25
}

// Set which program is loaded currently and initialize the camera
void Camera::program(int p)
{
	//   char buf[80];
	m_program = p;
	init_cam();							//added 11/25
 /*   if (pdv_pt[ipdv]) {
		int prog = m_program;
			if (pdv_pt[ipdv]) {
				sprintf_s(buf, "@RCL %d\r", prog);		// sending RCL first did not work!
				serial_write(buf);
				pdv_setsize(pdv_pt[ipdv], WIDTH[p]/2, HEIGHT[p]/2);
		}
	}*/
	cout << "cam line 295 cam.program " << p << " width " << WIDTH[p] << " height " << HEIGHT[p] << "\n";
}

int Camera::width() {
	return WIDTH[m_program];
}

int Camera::height() {
	return HEIGHT[m_program];
}

int Camera::depth() {
	return m_depth;
}

int Camera::freq() {
	return FREQ[m_program];
}

// Apply CDS subtraction and deinterleave to a list of raw images.
void Camera::reassembleImages(unsigned short *images, int nImages) {
	size_t imageSize = width() * height();
	int channelOrder[] = { 2, 3, 1, 0 };
	for (int i = 0; i < nImages; i++) {
		subtractCDS(images + imageSize * i, height(), width());
		deinterleave(images + imageSize * i,height(),width(), channelOrder);
	}
}

// This deinterleave follows Chun's specs over email (row-by-row) instead of quadrant_readout.docx
// Before deinterleaving, the raw data order for each row comes in like this (d for pixel data, r for pixel reset, which would be used for the next frame):
// d1, d64, d128, d192, d2, d65, d129, d192, d3,…d256, r1, r64, r128, r192… r256 (total 512)
// Note quadWidth is the width NOT doubled for CDS, i.e. the final image width
void Camera::deinterleave(unsigned short * buf, int quadHeight, int quadWidth, int* channelOrder) {
	

	// We say that CDS "doubles" the width
	// Alternative, treat the 1-D array as if we are processing 2x the number of rows
	quadHeight *= 2;

	int quadSize = quadWidth * quadHeight;
	// Idea: do this in place for mem efficiency if needed
	vector<unsigned short> tmpBuf(quadSize, 0);
	int numCamChannels = 4; // each PDV channel (quadrant) has 4 camera channels

	unsigned short *data_ptr = buf;
	// This is the width spanned by the pixels from 1 of the 4 camera channels per PDV channel:
	int camChannelWidth = quadWidth / numCamChannels;
	
	for (int row = 0; row < quadHeight; row++) {
		// Every other row is reset data, but is interleaved in same pattern
		for (int col = 0; col < quadWidth; col++) {
			int chOrd = channelOrder[col % numCamChannels];
			int camChOffset = chOrd * camChannelWidth;
			tmpBuf[row * quadWidth + (col / numCamChannels) + camChOffset]
				= *data_ptr++;
		}
	}

	// copy back to output array
	for (int i = 0; i < quadSize; i++) {
		buf[i] = tmpBuf[i];
	}
}


// Subtract reset data (stripes) for correlated double sampling (CDS) for a single image.
// This halves the number of columns of each raw image
// quad_width is the final width of the frame
void Camera::subtractCDS(unsigned short *image_data, int quad_height, int quad_width)
{
	int CDS_add = 1024;

	unsigned short *new_data = image_data;
	unsigned short *reset_data = image_data + quad_width;
	unsigned short *old_data = image_data;

	for (int i = 0; i < quad_height; i++) {
		for (int j = 0; j < quad_width; j++) {
			*new_data++ = CDS_add + *old_data++ - *reset_data++;
		}
		reset_data += quad_width;
		old_data += quad_width;
	}
}





// ============================================================================
// Code modified from Chun (SM_take_tb-Chun.cpp) for CDS subtraction and frame 
// deinterleave.
// The parameters have been removed and hard-coded to match how they get read in
// in sm.cpp from sm_config.dat 
// Assuming: 
//	TWOK = true
//	QUAD = false
//	pdv_chan_num = 4
// factor = 1



// This is the "Rosetta stone" mapping the SM_config.dat values to the arguments used here.
// arguments: stripes = image_stripes, layers = image_layers, factor = superFrame
// SM_take_tb(pdv_chan_num,		file_name,			, 	curNFrames,		
// SM_take_tb(int numChannels,	char *file_name,	,	int images,					

// cds_lib[curConfig]		=>  int cdsF = 1
// layers_lib[curConfig],	=>	int layers = 1
// stripes_lib[curConfig],  =>	int stripes = 4
// rotate_lib[curConfig],   =>  rotateFlag = 0
// bad_pix_lib[curConfig],  =>  bad_pix_index = 0
// ndr_lib[curConfig],		=>  NDR_flag = 0
// ccd_lib_bin[curConfig] / 2 is passed to value of h_bin, and varies for each little dave program
// super_frame_lib likely determines superFrame and thus factor? super_frame_lib is almost always 0 in the config.

// frame_interval,			f_size_ratio,		cameraType,			file_frame_w,
// double frame_interval,	int factor,			char *cameraname,	int file_img_w, 

//BNC_ratio,		ExtTriggerFlag,		preTrigger_frames,		dark_f_flag,	QuadFlag,			 segment_lib[curConfig]);
//int BNC_ratio,	int ExtTriggerFlag, int preTrigger_frames,	int darkFlag,	int quadFlag, 		 int segmented)

//file_width = image_width
//file_height = image_height

/*int makeLookUpTable(unsigned int *Lut, int image_width, int image_height)
{
	int layers = 1;
	int stripes = 4;
	int file_width = image_width; // CDS subtraction means file_width is twice the final image_width
	int file_height = image_height;

	//	int SEGS, file_offset, image_offset, frame_length, image_length, file_length;
	int SEGS, image_length, file_length;
	//	int frame, segment, row, rowIndex, destIndex, rows, cols, swid, dwid, srcIndex;
	int segment, row, rowIndex, destIndex, rows, swid, dwid, cols, srcIndex;
	static int twokchannel[] = { 3, 2, 0, 1, 7, 6, 4, 5, 9, 8, 10, 11, 13, 12, 14, 15 };
	static int onekchannel[] = { 3, 2, 0, 1, 5, 4, 6, 7 };
	int *channelOrder = NULL;
	int num_pdvChan;


	SEGS = stripes * layers;
	image_length = image_width * image_height;
	file_length = file_width * file_height;
	rows = file_height / layers;

	if (stripes == 8) {
		channelOrder = twokchannel;
		num_pdvChan = 4;
	}
	else {
		channelOrder = onekchannel;
		num_pdvChan = 2;
	}

	swid = image_width / stripes;
	dwid = file_width / stripes;

	for (int segment = 0; segment < stripes; ++segment) {
		srcIndex = channelOrder[segment] % 4;
		if (num_pdvChan == 4)
			srcIndex += (image_length / 4)*(channelOrder[segment] / 4);
		for (row = 0, rowIndex = 0; row < rows; ++row, rowIndex += file_width) {
			for (destIndex = rowIndex + dwid * segment, cols = dwid; cols > 0; --cols, ++destIndex) {
				Lut[destIndex] = srcIndex;
				srcIndex += SEGS / num_pdvChan;
			}
			srcIndex += dwid * SEGS / num_pdvChan;
		}
	}
		
	for (int segment = stripes; segment < SEGS; ++segment) {
		srcIndex = channelOrder[segment] % 4;
		if (num_pdvChan == 4)
			srcIndex += (image_length / 4)*(channelOrder[segment] / 4);
		else
			srcIndex += (image_length / 2);
		for (row = 0, rowIndex = file_length - file_width; row < rows; ++row, rowIndex -= file_width) {
			for (destIndex = rowIndex + dwid * (segment - SEGS / 2), cols = dwid; cols > 0; --cols, ++destIndex) {
				Lut[destIndex] = srcIndex;
				srcIndex += SEGS / num_pdvChan;
			}
			srcIndex += dwid * SEGS / num_pdvChan;
		}
	}
		
		
	
	return 0;
}
*/

void frame_deInterleave(int length, unsigned *lookuptable, unsigned short *old_images, unsigned short *new_images)
{
	for (int i = 0; i < length; ++i)
		*(new_images++) = old_images[*(lookuptable++)];

}


// We perform this in-place
// Assumptions for Chun's parameters (removed): 
//    int loops: N/A (This is the number of images; we'll write function for 1 image)
//    int factor: 1
//    int QUAD: 0
//    int TWOK: 1 
//    int num_pdvChan: 4
/*
void subtractCDS(unsigned short int *image_data, int loops, unsigned int width, unsigned int height, unsigned int length)
{
	int num_pdvChan = 1;
	bool TWOK = true;
	bool QUAD = false;
	int factor = 1;

	int l, m, n, row, col, doublecols = 0, rows = 0, cols = 0;
	unsigned short int *new_data, *old_data, *reset_data;
	int CDS_add = 256;
	int step_cnt, total_cds_loops;
	double step_size;
	char str[256];

	total_cds_loops = loops;



	if (factor == 1)
		rows = height * (num_pdvChan >> 1);
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
	

	if (factor == 1) {
		for (--loops, m = 0; loops > 0; --loops, ++m) {
			for (row = 0; row < rows; ++row) {
				for (col = cols; col; --col)
					*new_data++ = CDS_add + *old_data++ - *reset_data++;
				new_data += cols;
				reset_data += cols;
				old_data += cols;
			}
		}
	}

}*/



