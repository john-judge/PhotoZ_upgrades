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

//const int Camera::WIDTH[]  = {2048,	2048,	1024,	1024,	512,	512,	256,	256};
//const int Camera::HEIGHT[] = {1024,	100,	320,	160,	160,	80,		60,		40}; 

// JMJ 2/6/21 -- For testing, these are sizes based on of .cfg files:
const int Camera::WIDTH[] = { 2048,	2048,	1024,	1024,	1024,	1024,	1024,	1024 };
const int Camera::HEIGHT[] = { 512,	50,	320,	160,	80,	40,		30,		20 };


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

const int Camera::channelOrders[16] = { 2, 3, 1, 0,
										1, 0, 2, 3,
										2, 3, 1, 0,
										1, 0, 2, 3, };
#endif // LILDAVE

Camera::Camera() {
	edt_devname[0] = '\0';
	strcpy(edt_devname, EDT_INTERFACE);

	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		pdv_pt[ipdv] = NULL;
		timeouts[ipdv] = 0;
	}
	
	last_timeouts = m_num_timeouts = 0;
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
				retry = fl_choice("Failed to close PDV channel. Retry or restart the camera manually.", "Retry", "Exit", "Debugging Info");
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

int Camera::open_channel(int ipdv) {
	if (pdv_pt[ipdv])
		pdv_close(pdv_pt[ipdv]);
	//		if ((pdv_pt[ipdv] = pdv_open_channel(edt_devname, unit, channel)) == NULL)
	//			return 1;
	int unit = ipdv & 1; // last bit
	int channel = (ipdv >> 1) & 1; // second-to-last bit
	if ((pdv_pt[ipdv] = pdv_open_channel(EDT_INTERFACE, unit, channel)) == NULL)
		return 1;

	pdv_enable_external_trigger(pdv_pt[ipdv], 0);	 // disable the trigger in case it was left on - it persists over restarts
	pdv_set_timeout(pdv_pt[ipdv], 1000);
	pdv_flush_fifo(pdv_pt[ipdv]);
	m_depth = pdv_get_depth(pdv_pt[ipdv]);
	// allocate ring buffers
	pdv_multibuf(pdv_pt[ipdv], 4);
	//		pdv_setsize(pdv_pt[ipdv], WIDTH[m_program]/2, HEIGHT[m_program] / 2);
	cout << " line 115 open_channel size " << pdv_get_allocated_size(pdv_pt[ipdv]) << "\n";
	return 0;
}

unsigned char* Camera::single_image(int ipdv)			//used by LiveFeed.cpp
{
	if (!pdv_pt[ipdv])
		return nullptr;
	pdv_flush_fifo(pdv_pt[ipdv]);
	pdv_start_image(pdv_pt[ipdv]);
	return pdv_wait_image(pdv_pt[ipdv]);
}

void Camera::init_cam()				// entire module based on code from Chun - sm_init_camera.cpp
{
	char command[80];
	cout << "camera line 129 init_cam  program #  " << PROG[m_program] << "  \n";
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_0 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_1 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_0 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_1 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);

	// Channels are already opened in open_channel
	//	pdv_pt[0] = pdv_open_channel(EDT_INTERFACE, 0, 0);
	//	pdv_pt[1] = pdv_open_channel(EDT_INTERFACE, 0, 1);
	//	pdv_pt[2] = pdv_open_channel(EDT_INTERFACE, 1, 0);
	//	pdv_pt[3] = pdv_open_channel(EDT_INTERFACE, 1, 1);									// according Chun: pdv_units = [0, 0, 1, 1]   pdv_channels = [0, 1, 0, 1]

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

// Starts image acquisition for one channel
void Camera::start_images(int ipdv) {
	if (!pdv_pt[ipdv]) 		return;
	pdv_flush_fifo(pdv_pt[ipdv]);				 // pdv_timeout_restart(pdv_p, 0);	same as pdv_flush  7-4-2020
	pdv_multibuf(pdv_pt[ipdv], 4);		// Suggested by Chun 5-14-2020		//change pdv_p to pdv_pt[0]  9-23-2020
	pdv_start_images(pdv_pt[ipdv], 0);
}

// Ends image acquisition for one channel
void Camera::end_images(int ipdv) {
	if (!pdv_pt[ipdv])
		return;
	pdv_start_images(pdv_pt[ipdv], 1); // "To stop freerun, call pdv_start_images again with a count of 1."
}

// Print image acquisition info for one channel
void Camera::get_image_info(int ipdv) {
	cout << " program       " << m_program << "\t   ipdv = " << ipdv << "\n";
	cout << " allocated size      " << pdv_get_allocated_size(pdv_pt[ipdv]) << "\n";
	cout << " image size (bytes per image) " << pdv_get_imagesize(pdv_pt[ipdv]) << "\n";
	cout << " buffer size      " << pdv_image_size(pdv_pt[ipdv]) << "\n";
	cout << " image height      " << pdv_get_height(pdv_pt[ipdv]) << "\n";
	cout << " cam height      " << pdv_get_cam_height(pdv_pt[ipdv]) << "\n";
	cout << " image width      " << pdv_get_width(pdv_pt[ipdv]) << "\n";
	cout << " cam width       " << pdv_get_cam_width(pdv_pt[ipdv]) << "\n";
}

// Get the buffer size for images from one channel
int Camera::get_buffer_size(int ipdv) {
	return pdv_get_imagesize(pdv_pt[0]);
}

unsigned char* Camera::wait_image(int ipdv) {
	unsigned char* image = pdv_wait_image(pdv_pt[ipdv]);
	if (edt_reg_read(pdv_pt[ipdv], PDV_STAT) & PDV_OVERRUN)
		++overruns;
	/* timeouts = pdv_timeouts(pdv_pt[ipdv]);
	 if (timeouts > last_timeouts) {
		 pdv_timeout_restart(pdv_pt[ipdv], TRUE);
		 last_timeouts = timeouts;
		 m_num_timeouts++;
		 recovering_timeout = true;
	 } else if (recovering_timeout) {
		 pdv_timeout_restart(pdv_pt[ipdv], TRUE);
		 recovering_timeout = false;
	 }*/
	return image;
}

// Write a command from buf to IPDVth channel
// Important: Only channel 0 of Little Dave supports serial write
void Camera::serial_write(const char* buf)
{
	// Writes a commend 
	char buffer[512];
	pdv_serial_read(pdv_pt[0], buffer, 512 - 1); // flush camera buffer of lingering data
	memset(buffer, 0, sizeof(char) * 512);
	strcpy_s(buffer, buf);
	pdv_serial_write(pdv_pt[0], buffer, strlen(buffer));
}

/*
void Camera::serial_read(char* buf, int size)
{
	pdv_serial_wait(pdv_pt[ipdv], 1000, size - 1);
	int n = pdv_serial_read(pdv_pt[ipdv], buf, size - 1);
	if (n < size)
		buf[n] = '\0';
	buf[size - 1] = '\0';
}
*/

int Camera::num_timeouts(int ipdv) {
	return timeouts[ipdv];
}

int Camera::program()
{
	return m_program;
}

void Camera::setCamProgram(int p)
{
	m_program = p;
}
void Camera::program(int p)
{
	char buf[80];
	m_program = p;
	if (pdv_pt[0]) { 
		int prog = m_program;
		if (pdv_pt[0]) {
			sprintf_s(buf, "@RCL %d\r", prog);		// sending RCL first did not work!
			serial_write(buf);
			pdv_setsize(pdv_pt[0], WIDTH[p], HEIGHT[p]);
		}
	}
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

// Apply CDS subtraction and deinterleave to one raw image
// TO DO: reverse order of operations for efficiency (less swapping in deinterleave).
// mapQuadrants: whether to remap quadrants to full image view
// verbose: whether to print out the image data to file at intermediate stages
void Camera::reassembleImage(unsigned short* image, bool mapQuadrants, bool verbose) {
	int w = width();
	int h = height();
	size_t quadrantSize = (size_t)w * (size_t)h;
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		std::string filename = "RLI-ch0.txt";
		if (verbose && ipdv == 1) printQuadrant(image + ipdv * quadrantSize, filename.c_str());
		
		subtractCDS(image + ipdv * quadrantSize, h, w);
		filename = "Output-ch0.txt";
		if (verbose && ipdv == 1) printQuadrant(image + ipdv * quadrantSize, filename.c_str());

		deinterleave(image + ipdv * quadrantSize, h, w, channelOrders + ipdv * 4);
		filename = "OutputCDS-ch0.txt";
		if (verbose && ipdv == 1) printQuadrant(image + ipdv * quadrantSize, filename.c_str());
	}

	if (!mapQuadrants) return;

	// ============================================================
	// Quadrant Mapping
	// Since CDS subtraction halved the image size, we now have auxiliary space
	// to use to arrange the quadrants of the full image
	//		- space out the rows of quadrants 0 and 2
	//		- interleave in the rows of quadrants 1 and 3

	// Even channels (Quadrants 0 and 2) - space out
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv += 2) {
		unsigned short* srcRow = image + quadrantSize * ipdv + quadrantSize / 2 - w;
		unsigned short* dstRow = image + quadrantSize * ipdv + quadrantSize - w;
		
		for (int row = int(h/2) - 1; row >= 0; row--) {
			memcpy(dstRow, srcRow, w * sizeof(unsigned short));
			dstRow -= 2 * w;
			srcRow -= w;
		}
	}

	// Odd channels (Quadrants 1 and 3) - intereave-in rows into the previous channel's rows
	for (int ipdv = 1; ipdv < NUM_PDV_CHANNELS; ipdv += 2) {

		unsigned short* srcRow = image + quadrantSize * ipdv;
		unsigned short* dstRow = image + quadrantSize * (ipdv-1) + w;
		// Quadrant 1
		for (int row = 0; row < int(h / 2); row++) {
			memcpy(dstRow, srcRow, w * sizeof(unsigned short));
			dstRow += 2 * w;
			srcRow += w;
		}
	}
	// ============================================================
}

// Apply CDS subtraction and deinterleave to a list of raw images.
void Camera::reassembleImages(unsigned short* images, int nImages) {
	size_t imageSize = width() * height();
	int channelOrders[16] = { 2, 3, 1, 0,
							  1, 0, 2, 3,
							  2, 3, 1, 0,
							  1, 0, 2, 3, };
	for (int i = 0; i < nImages; i++) {
		unsigned short* img = images + imageSize * i;
		for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
			subtractCDS(img + ipdv * imageSize / 4, height(), width());
			deinterleave(img + ipdv * imageSize / 4, height(), width(), channelOrders + ipdv * 4);
		}
	}
}

// This deinterleave follows Chun's specs over email (row-by-row) instead of quadrant_readout.docx
// Before deinterleaving, the raw data order for each row comes in like this (d for pixel data, r for pixel reset, which would be used for the next frame):
// d1, d64, d128, d192, d2, d65, d129, d192, d3,d256, r1, r64, r128, r192 r256 (total 512)
// Note quadWidth is the width NOT doubled for CDS, i.e. the final image width

void Camera::deinterleave(unsigned short* buf, int quadHeight, int quadWidth, const int* channelOrder) {

	// We say that CDS "doubles" the width
	// Alternative, treat the 1-D array as if we are processing 2x the number of rows
	quadHeight *= 2;

	int quadSize = quadWidth * quadHeight;
	// Idea: do this in place for mem efficiency if needed
	vector<unsigned short> tmpBuf(quadSize, 0);
	int numCamChannels = 4; // each PDV channel (quadrant) has 4 camera channels

	unsigned short* data_ptr = buf;
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

// PDV channels are readout in this order: 
//  0 - upper left, 
//  1 - lower left, 
//  2 - upper right, 
//  3 - lower right.
void Camera::remapQuadrants(unsigned short* buf, int quadHeight, int quadWidth) {

}

// Subtract reset data (stripes) for correlated double sampling (CDS) for a single image.
// This halves the number of columns of each raw image
// quad_width is the final width of the frame
void Camera::subtractCDS(unsigned short* image_data, int quad_height, int quad_width)
{
	int CDS_add = 1024;

	unsigned short* new_data = image_data;
	unsigned short* reset_data = image_data + quad_width;
	unsigned short* old_data = image_data;

	for (int i = 0; i < quad_height; i++) {
		for (int j = 0; j < quad_width; j++) {
			*new_data++ = CDS_add + *old_data++ - *reset_data++;
		}
		reset_data += quad_width;
		old_data += quad_width;
	}
}

void Camera::printFinishedImage(unsigned short* image, const char* filename) {
	int full_img_size = NUM_PDV_CHANNELS * width() * height(); // Divide by 2 to account for CDS subtraction
	std::ofstream outFile;
	outFile.open(filename, std::ofstream::out | std::ofstream::trunc);
	for (int k = 0; k < full_img_size; k++)
		outFile << k << " " << image[k] << "\n";
	outFile.close();
	cout << "\nWrote full image's raw data to PhotoZ/" << filename << "\n";
}



void Camera::printQuadrant(unsigned short* image, const char* filename) {
	int quadrantSize = width() * height(); // pre-CDS subtracted size
	std::ofstream outFile;
	outFile.open(filename, std::ofstream::out | std::ofstream::trunc);
	for (int k = 0; k < quadrantSize; k++)
		outFile << k << " " << image[k] << "\n";
	outFile.close();
	cout << "\nWrote quadrant raw data to PhotoZ/" << filename << "\n";
}