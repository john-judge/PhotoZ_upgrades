//=============================================================================
// Camera.cpp
//=============================================================================
#include "Camera.h"
#include <iostream>
#include "DapController.h"
#include "RecControl.h"		//added to get camGain on line 157
#include <vector>
#include <string>
#include <FL/fl_ask.h>

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
void Camera::postAcquisitionProcessing(unsigned short *images, int nImages) {
	size_t imageSize = width() * height();
	for (int i = 0; i < nImages; i++) {
		subtractCDS(images + imageSize * i);
		deinterleave(images + imageSize * i);
	}
}


// Deinterleave the image in buf given that they were produced by 4 channels
void Camera::deinterleave(unsigned short * buf) {
	int m = width() / 2;
	int n = height() / 2;
	deinterleave(buf, n, m);
}

// JMJ 12/26/2020
// Deinterleave the image in buf given that they were produced by 4 channels
// Each channel memory is contiguous in 1-D array. The channels' memory buffers are back-to-back.
void Camera::deinterleave(unsigned short * buf, int n, int m) {

	// Idea: do this in place for mem efficiency if needed
	vector<short> tmpBuf(4 * n * m, 0);

	// loop over quadrants, rows, and columns and transform from interleaved to deinterleaved
	for (int ipdv = 0; ipdv < 4; ipdv++) {
		for (int iRow = 0; iRow < n; iRow++) {
			for (int jCol = 0; jCol < m; jCol++) {
				tmpBuf[mapToDeinterleaved(iRow,jCol,n,m,ipdv)] = buf[mapFromInterleaved(iRow, jCol, n, m, ipdv)];
			}
		}
	}
	// copy back to output array
	for (int i = 0; i < 4 * n * m; i++) {
		buf[i] = tmpBuf[i];
	}
}

// Return the 1D array index of the interleaved (source) array given:
// row i of n rows, column j of m columns, in quadrant QUADRANT
int Camera::mapFromInterleaved(int i, int j, int n, int m, int quadrant) {
	if (i >= n || j >= m) {
		cout << "Index out of range error in Camera::mapFromInterleaved, Quadrant " << quadrant << "  \n";
		return -1;
	}
	return quadrant * n * m +	// quadrant offset (each quadrant of size n * m)
			i * m +				// row offset (each row of size m)
			j;					// offset into current column of quadrant
}

// Return the 1D array index of the deinterleaved (destination) array given:
// row i of n rows, column j of m columns, in quadrant QUADRANT
int Camera::mapToDeinterleaved(int i, int j, int n, int m, int quadrant) {
	if (i >= n || j >= m) {
		cout << "Index out of range error in Camera::mapToDeinterleaved, Quadrant " << quadrant << "  \n";
		return -1;
	}
	switch (quadrant) {
		case 0:
			// Channel 0 (upper left quadrant), no reflections
			return 4 * (i * m + j);
		case 1:
			// Channel 1: reflect col indices across the vertical axis
			return 4 * (i * m + (m - j - 1)) + 1;
		case 2:
			// Channel 2: reflect row indices across horizontal
			return 4 * ((n - i - 1) * m + j) + 2;
		case 3:
			// Channel 3: reflect col indices across the vertical axis and row across horizontal
			return 4 * ((n - i - 1) * m + (m - j - 1)) + 3;
		default:
			return -1;
	}
}

// Subtract reset data (stripes) for correlated double sampling (CDS) for a single image.
void Camera::subtractCDS(unsigned short *image_data)
{
	subtractCDS(image_data, width(), height());
}

// Subtract reset data (stripes) for correlated double sampling (CDS) for a single image.
// This halves the number of columns, m, of each image.
// We perform this in-place
// Assumptions for Chun's parameters (removed): 
//    int loops: N/A (This is the number of images; we'll write function for 1 image)
//    int factor: 1
//    int QUAD: 0
//    int TWOK: 1 
//    int num_pdvChan: 4
void Camera::subtractCDS(unsigned short int *image_data, unsigned int m, unsigned int n)
{
	// if (factor == 1)
	int rows = n * 2; // height * # channels / 2
	int cols = m / 4;  // width / # channels
	int CDS_add = 256;

	unsigned short int *new_data = image_data;
	unsigned short int *reset_data = image_data + cols;
	unsigned short int *old_data = image_data;

	for (int i = 0; i < rows; i++) {
		for (int j = k; j < k + cols; j++) {
			*new_data++ = CDS_add + *old_data++ - *reset_data++;
		}
		reset_data += cols;
		old_data += cols;
	}
}