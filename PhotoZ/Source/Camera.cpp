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

// These are the original sizes:
//const int Camera::WIDTH[]  = {2048,	2048,	1024,	1024,	512,	512,	256,	256};
//const int Camera::HEIGHT[] = {1024,	100,	320,	160,	160,	80,		60,		40}; 

// JMJ 2/6/21 -- For testing, these are sizes based on of .cfg files:
//const int Camera::WIDTH[] = { 2048,	2048,	1024,	1024,	1024,	1024,	1024,	1024 };
//const int Camera::HEIGHT[] = { 512,	50,	320,	160,	80,	40,		30,		20 };

// JMJ 6/18/21 -- For more testing, these are sizes based on PDV readout:
//const int Camera::WIDTH[] = { 1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024 };
//const int Camera::HEIGHT[] = { 20,	20,	20,	20,	20,	20,		20,		20 };

// JMJ 6/23/2021 -- these are from TurboSM (sm.cpp) for 2kx2k_NEURO_BINNED_d_
const int Camera::WIDTH[] = { 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 };
const int Camera::HEIGHT[] = { 512, 200, 50, 160, 32, 15, 128, 20 };

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
	m_program = 7;                // default camera program
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
	// allocate ring buffers, 4 (per channel) is EDT's recommendation
	pdv_multibuf(pdv_pt[ipdv], 4);

	// The following is never needed, instead use cfg file to change img dimension (Chun): pdv_setsize(pdv_pt[ipdv], WIDTH[m_program]/2, HEIGHT[m_program] / 2);
	cout << " Camera open_channel size " << pdv_get_allocated_size(pdv_pt[ipdv]) << "\n";
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
	cout << "Camera init_cam  program #  " << PROG[m_program] << "  \n";
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_0 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv0_1 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_0 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);
	sprintf(command, "c:\\EDT\\pdv\\initcam -u pdv1_1 -f c:\\EDT\\pdv\\camera_config\\%s", PROG[m_program]);
	system(command);

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
	return image;
}

// Write a command from buf to IPDVth channel
// Important: Only channel 0 of Little Dave supports serial write
void Camera::serial_write(const char* buf) {
	// Writes a commend 
	char buffer[512];
	pdv_serial_read(pdv_pt[0], buffer, 512 - 1); // flush camera buffer of lingering data
	memset(buffer, 0, sizeof(char) * 512);
	strcpy_s(buffer, buf);
	pdv_serial_write(pdv_pt[0], buffer, (int)strlen(buffer));
}

int Camera::num_timeouts(int ipdv) {
	return timeouts[ipdv];
}

int Camera::program() {
	return m_program;
}

void Camera::setCamProgram(int p) {
	m_program = p;
}
void Camera::program(int p) {
	char buf[80];
	m_program = p;
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		if (pdv_pt[ipdv]) {
			int prog = m_program;
			if (pdv_pt[ipdv]) {
				sprintf_s(buf, "@RCL %d\r", prog);		// sending RCL first did not work!
				serial_write(buf);
				pdv_setsize(pdv_pt[ipdv], width(), height());
			}
		}
	}
}

// width of one quadrant BEFORE CDS subtraction
int Camera::width() {
	return WIDTH[m_program];
}

// height of one quadrant BEFORE CDS subtraction
int Camera::height() {
	return HEIGHT[m_program];
}

int Camera::depth() {
	return m_depth;
}

int Camera::freq() {
	return FREQ[m_program];
}

bool Camera::isValidPlannedState(int num_diodes) {
	return isValidPlannedState(num_diodes, 0);
}

// Validate that we are about to allocate the proper amount of memory for a quadrant
// I.e. we check if PDV's internals match PhotoZ's expectations
// Displays warnings to user if there are problems
bool Camera::isValidPlannedState(int num_diodes, int num_fp_diodes) {
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		int PDV_size = get_buffer_size(ipdv);
		if (num_diodes - num_fp_diodes != PDV_size / 2) {
			cout << "\nAborting acquisition. The size PhotoZ expects for quadrant " << ipdv << " is: \t" \
				<< num_diodes << " pixels" \
				<< "\nBut the size allocated by PDV for this PDV channel quadrant is:\t\t" \
				<< PDV_size << " bytes = " << PDV_size / 2 << " pixels.\n" \
				<< "\tPDV width: " << pdv_get_cam_width(pdv_pt[ipdv]) << "\n" \
				<< "\tPDV height: " << pdv_get_cam_height(pdv_pt[ipdv]) << "\n";
			fl_alert(" Acquisition failed. Please retry once before examining debugging output.\n");
			return false;
		}
		/*
		// Wdith of quadrant is image half width
		if (width() / 2 != pdv_get_cam_width(pdv_pt[ipdv])) {
			cout << "\nAborting acquisition. PhotoZ expects width " << width() / 2 << \
				" for quadrant " << ipdv << " but PDV is set to " << pdv_get_cam_width(pdv_pt[ipdv]) << "\n";
			fl_alert(" Acquisition failed. Please retry once before examining debugging output.\n");
			return false;
		}
		// Height of quadrant is image half height
		if (height() / 2 != pdv_get_cam_height(pdv_pt[ipdv])) {
			cout << "\nAborting acquisition. PhotoZ expects height " << height() / 2<< \
				" for quadrant " << ipdv << " but PDV is set to " << pdv_get_cam_height(pdv_pt[ipdv]) << "\n";
			fl_alert(" Acquisition failed. Please retry once before examining debugging output.\n");
			return false;
		}
		*/
	}
	return true;
}

// Properly memory allocation for nImages images in preparation for image acquisition
// Exposing this functions allows drivers to reuse pointers/buffers more easily
unsigned short* Camera::allocateImageMemory(int num_diodes, int numPts) {
	unsigned short* memory = new(std::nothrow) unsigned short[(size_t)num_diodes * (numPts + 1) * NUM_PDV_CHANNELS];
	if (!memory) {
		fl_alert("Ran out of memory!\n");
		return nullptr;
	}
	memset(memory, 0, num_diodes * (numPts + 1) * NUM_PDV_CHANNELS * sizeof(short));
	return memory;
}

// FIRST: set to True if this camera session has not acquired images yet
// NOTLAST: set to True if this camera session will continue after this acquisition (i.e. for RLI)
// Returns: whether there was an error
bool Camera::acquireImages(unsigned short* memory, int numPts, int first, int notLast) {
	if (first) serial_write("@SEQ 0\@SEQ 1\r@TXC 1\r");

	if(acquireImages(memory, numPts)) return true;

	if (!notLast) {
		for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
			end_images(ipdv);
		}
		serial_write("@TXC 0\r");
	}
	return false;
}

// The full parallel acquistion WITHOUT image reassembly
// Returns: whether there was an error
bool Camera::acquireImages(unsigned short* memory, int numPts) {
	// Start Acquisition
	//joe->dave; should work for dave cam
	unsigned char *image;
	int quadrantSize = width() * height();

	int tos = 0;
	bool failed = false;

	omp_set_num_threads(4);
	#pragma omp parallel for 	
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		cout << "Number of active threads: " << omp_get_num_threads() << "\n";
		start_images(ipdv);
		int tos = 0;
		for (int ii = 0; ii < 7; ii++) image = wait_image(ipdv);		// throw away first seven frames to clear camera saturation	
																	// be sure to add 7 to COUNT in lines 327 and 399	
		for (int i = 0; i < numPts; i++) {
			unsigned short* privateMem = memory + ipdv * quadrantSize; // pointer to this thread's section of MEMORY	
			// acquire data for this image from the IPDVth channel	
			image = wait_image(ipdv);
			// Save the image to process later	
			memcpy(privateMem + (quadrantSize * i), image, quadrantSize * sizeof(short));
			if (num_timeouts(ipdv) != tos) {
				printf("Camera acquireImages timeout on %d\n", i);
				tos = num_timeouts(ipdv);
			}
			if (num_timeouts(ipdv) > 20) {
				end_images(ipdv);
				if (ipdv == 0) serial_write("@TXC 0\r"); // only write to channel 0	
				failed = true; //Don't return from a parallel section	
			}
			//cout << "Channel " << ipdv << " acquired its portion of image " << i << "\n";
		}
	}
	return failed;

}

// Apply CDS subtraction, deinterleave, and quadrant remapping to a list of raw images.
void Camera::reassembleImages(unsigned short* images, int nImages) {

	// NOTE: cam.height and cam.width are the RAW QUADRANT sizes
	// i.e. the size of a quadrant BEFORE CDS subtraction
	
	int channelOrders[16] = { 2, 3, 1, 0,
							  1, 0, 2, 3,
							  2, 3, 1, 0,
							  1, 0, 2, 3, };

	// CDS subtraction for entire image (halves total memory needed for images)
	subtractCDS(images, height() * nImages, width()); 

	size_t imageSize = width() * height(); 

	for (int i = 0; i < nImages; i++) {
		unsigned short* img = images + imageSize / 2 * i;
		for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
			//deinterleave(img + (ipdv * imageSize / 4), 
			//			 height(), 
			//			 width(), 
			//			 channelOrders + ipdv * 4, 
			//			 (ipdv % 2 == 1)); // channels 1 and 3 are lower-half quadrants, must flip across horizontal axis
			//remapQuadrantsOneImage(img + ipdv * imageSize / 4, height(), width());
		}
		if (i % 100 == 0) cout << "Image " << i << " of " << nImages << " done.\n";
	}
}

// This deinterleave follows Chun's specs over email (row-by-row) instead of quadrant_readout.docx
// Before deinterleaving, the raw data order for each row comes in like this (d for pixel data, r for pixel reset, which would be used for the next frame):
// d1, d64, d128, d192, d2, d65, d129, d192, d3,d256, r1, r64, r128, r192 r256 (total 512)
// Note quadWidth is the width NOT doubled for CDS, i.e. the final image width
// flipVertically: set to true for 
void Camera::deinterleave(unsigned short* buf, int quadHeight, int quadWidth, const int* channelOrder, bool flipVertically) {

	// quadHeight *= 2; // Use this if we haven't yet done CDS subtraction

	int quadSize = quadWidth * quadHeight;
	// Idea: do this in place for mem efficiency if needed
	vector<unsigned short> tmpBuf(quadSize, 0);
	int numCamChannels = 4; // each PDV channel (quadrant) has 4 camera channels

	unsigned short* data_ptr = buf;
	// This is the width spanned by the pixels from 1 of the 4 camera channels per PDV channel:
	int camChannelWidth = quadWidth / numCamChannels;

	for (int row = 0; row < quadHeight; row++) {
		for (int col = 0; col < quadWidth; col++) {
			int chOrd = channelOrder[col % numCamChannels];
			int camChOffset = chOrd * camChannelWidth;
			tmpBuf[row * quadWidth + (col / numCamChannels) + camChOffset]
				= *data_ptr++;
		}
	}
	// copy back to output array
	for (int row = 0; row < quadHeight; row++) {
		for (int col = 0; col < quadWidth; col++) {
			if (flipVertically) {
				buf[row * quadWidth + col] = tmpBuf[(quadHeight - 1 - row) * quadWidth + col];
			}
			else {
				buf[row * quadWidth + col] = tmpBuf[row * quadWidth + col];
			}
			
		}
	}
}

// Subtract reset data (stripes) for correlated double sampling (CDS) for a single image.
// This halves the number of columns of each raw image
// quad_width is the final width of the frame
void Camera::subtractCDS(unsigned short* image_data, int quad_height, int quad_width)
{
	int CDS_add = 2048;
	int CDS_width_fixed = 1024; // 6/25/21 - empirically, seems like this never changes
	int CDS_height = (quad_width / CDS_width_fixed) / 2 * quad_height * NUM_PDV_CHANNELS; // div by 2 since loop skips 2 CDS-size rows per iter

	unsigned short* new_data = image_data;
	unsigned short* reset_data = image_data + CDS_width_fixed;
	unsigned short* old_data = image_data;

	for (int i = 0; i < CDS_height; i++) {
		for (int j = 0; j < CDS_width_fixed; j++) {
			*new_data++ = CDS_add + *old_data++ - *reset_data++;
		}
		reset_data += CDS_width_fixed;
		old_data += CDS_width_fixed;
	}
}

// Quadrant Mapping
// Since CDS subtraction halved the image size, we now have auxiliary space
// to use to arrange the quadrants of the full image
//		- space out the rows of quadrants 0 and 2
//		- interleave in the rows of quadrants 1 and 3

// PDV channels for little Dave are readout in this order: 
//  0 - upper left, 
//  1 - lower left, 
//  2 - upper right, 
//  3 - lower right.

void Camera::remapQuadrantsOneImage(unsigned short* image, int quadHeight, int quadWidth) {

	int quadrantSize = quadHeight * quadWidth;

	// Even channels (Quadrants 0 and 2) - space out
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv += 2) {
		unsigned short* srcRow = image + quadrantSize * ipdv + quadrantSize / 2 - quadWidth;
		unsigned short* dstRow = image + quadrantSize * ipdv + quadrantSize - quadWidth;

		for (int row = int(quadHeight / 2) - 1; row >= 0; row--) {
			memcpy(dstRow, srcRow, quadWidth * sizeof(unsigned short));
			dstRow -= 2 * quadWidth;
			srcRow -= quadWidth;
		}
	}

	// Odd channels (Quadrants 1 and 3) - intereave-in rows into the previous channel's rows
	for (int ipdv = 1; ipdv < NUM_PDV_CHANNELS; ipdv += 2) {

		unsigned short* srcRow = image + quadrantSize * ipdv;
		unsigned short* dstRow = image + quadrantSize * (ipdv - 1) + quadWidth;
		// Quadrant 1
		for (int row = 0; row < int(quadHeight / 2); row++) {
			memcpy(dstRow, srcRow, quadWidth * sizeof(unsigned short));
			dstRow += 2 * quadWidth;
			srcRow += quadWidth;
		}
	}
}


// Utilities for debugging
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

/*
int Camera::checkCfgLUT()
{
	int i;

	for (i = 0; i < sizeof(configLUT) / sizeof(configLUT[0]); i++) {
		if (strstr(cameraStr, configLUT[i].cfg_str))
			return i + 1;
	}
	return 0;
}


int Camera::SM_initCamera(int load_config_flag, int close_flag, int load_pro_flag)
{
	char command[256];
	int prog;
	char *p;
	char home_drive[256];
	int config_id = 0;

	extern char home_dir[MAX_PATH];
	int pdv_chan_num = 4;
	int image_chan_order[16]; // [MAXOFFSETS];
	int seqROI_flag;
	int seqROI_ar[20][8];

	bool TwoK_flag = true;
	int demoFlag;
	char liveConfig_list[8][80];

	int curConfig = 7 - m_program;
	static int config_loaded = 0;
	int pcdFlag = 0;

	// if (close_flag > 0) SM_pdv_close();

	char buf[256], tmp_str[256];
	int m, i;
	FILE *fp;
	char flagFname[MAX_PATH];
	//Read Image Chan_order
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\image_chan_order.txt");
	if (fopen_s(&fp, flagFname, "r")) {
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
	if (fopen_s(&fp, flagFname, "r")) {
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
	bool writeToDisk_flag;
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\DiskData_flag.txt");
	if (fopen_s(&fp, flagFname, "r")) {
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
				if (fopen_s(&fp, edt_cfg_name, "r"))
					fclose(fp);
				else {
					sprintf(command, "%s does not exist.", edt_cfg_name);
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
			if (fopen_s(&fp, edt_cfg_name, "r"))
				fclose(fp);
			else {
				sprintf(command, "%s does not exist.", edt_cfg_name);
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

	}
	return TRUE;
}


int Camera::SM_pdv_open()
{
	int i;
	int pdv_units[NUM_PDV_CHANNELS] = { 0, 0, 1, 1 };
	int pdv_channels[NUM_PDV_CHANNELS] = { 0, 1, 0, 1 };


	for (i = 0; i < NUM_PDV_CHANNELS; i++)
		pdv_pt[i] = pdv_open_channel(EDT_INTERFACE, pdv_units[i], pdv_channels[i]);


	for (i = 0; i < NUM_PDV_CHANNELS; i++) {
		if (pdv_pt[i] == NULL) {
			SM_pdv_close();
			return FALSE;
		}
	}

	return TRUE;
}


void Camera::SM_pdv_close()
{
	int i;

	for (i = 0; i < NUM_PDV_CHANNELS; i++) {
		if (pdv_pt[i] != NULL)
			pdv_close(pdv_pt[i]);
	}
}*/