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
	"200 Hz   2048x1024", // = 2097152 px
	"2000 Hz  2048x100",  // = 204800 px
	"1000 Hz  1024x320",  // = 327680 px
	"2000 Hz  1024x160",  // = 163840 px
	"2000 Hz  512x160",   // = 81920 px
	"4000 Hz  512x80",    // = 40960 px
	"5000 Hz  256x60",    // = 15360 px
	"7500 Hz  256x40" };  // = 10240 px

// The final widths to display
const int Camera::DISPLAY_WIDTH[] =  { 2048, 2048, 1024, 1024, 512, 512, 256, 256 };
const int Camera::DISPLAY_HEIGHT[] = { 1024, 100,  320,  160,  160, 80,  60,  40 };

// The internal quadrant widths based on .cfg files
const int Camera::WIDTH[] = { 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 };
const int Camera::HEIGHT[] = { 512, 50,  160,   80,  80,   40,   30,  20 };

const int Camera::FREQ[] = { 200,	2000,	1000,	2000,	2000,	4000,	5000,	7500 };
const char* Camera::PROG[] = { //size		size factor to display size
	"DM2K_2048x512.cfg", // = 1048576 px	1x width	2x height
	"DM2K_2048x50.cfg",  // = 102400 px		1x width	2x height
	//"DM2K_2048x500.cfg",  //   <- superframing x10 attempt
	"DM2K_1024x160.cfg", // = 163840 px		1x width	2x height
	"DM2K_1024x80.cfg",  // = 81920 px		1x width	2x height
	"DM2K_1024x80.cfg",  // = 81920 px		1/2x width	2x height
	"DM2K_1024x40.cfg",  // = 40960 px		1/2x width	2x height
	"DM2K_1024x30.cfg",  // = 30720 px		1/4x width	2x height
	"DM2K_1024x20.cfg"   // = 20480 px		1/4x width	2x height
};

// CDS subtraction => width is divided by a factor of 2
// Quadrants (4 PDV channels) => width and height are multiplied by a factor of 2
// Then all heights match final heights. However, there's a remaining width factor by which to divide:
const int Camera::WIDTH_DIVISIVE_FACTOR[] = { 1, 1, 1, 1, 2, 2, 4, 4 };

const double Camera::sm_lib_rates[] = { 0.20, 0.50, 1.0, 0.64, 1.0, 2.0, 1.25, 2.0 };
const double Camera::sm_cam_lib_rates[] = { 0.2016948, 0.5145356, 1.02354144, 0.6480876, 1.03412632, 2.05128256, 1.27713928, 2.0 };
const double Camera::sm_integration_increments[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.147 };

// CCD lib and start line lib are used in camera serial commands
const int Camera::ccd_lib_bin[] = { 1, 1, 2, 2, 4, 4, 8, 8 };
const int Camera::start_line_lib[] = { 576, 1038, 766, 926, 766, 926, 846, 926 };


const int Camera::config_load_lib[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
const int Camera::cds_lib[] = { 1, 1, 1, 1, 1, 1, 1, 0 };
const int Camera::ndr_lib[] = { 0, 0, 0, 0, 0, 0, 0, 1 };
const int Camera::stripes_lib[] = { 4, 4, 4, 4, 4, 4, 4, 4 };
const int Camera::layers_lib[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
const int Camera::rotate_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const int Camera::bad_pix_lib[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// Based on experimenting on 7/5/21, it appears only the 200 Hz works without superframing
// From Chun on 1/18/21:
// However, for high-speed configurations (all config of DaVinci), 
// we need:
//			pdv_setsize(pdv_ptr, width, height*factor) 
// to make it super-frame, meaning the frame grabber will get, say 10 frames 
// (factor=10 for first config, and proportional more>10 for other configs ) as one super frame. 
//This is to reduce the interrupts.Otherwise the computer can�t keep up.
const int Camera::super_frame_lib[] = { 1, 1, 1, 1, 1, 1, 10, 10 };

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
	m_program = 4;                

}

Camera::~Camera() {
	close_channels();
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

	cout << " Camera open_channel size " << pdv_get_allocated_size(pdv_pt[ipdv]) << "\n";
	return 0;
}

int Camera::close_channels() {
	int retries = 2;
	for (int i = 0; i < NUM_PDV_CHANNELS; i++) {
		if (pdv_pt[i]) {
			while (pdv_close(pdv_pt[i]) < 0 && retries != 1) {
				if (retries > 0) {
					cout << "Current state of pdv_pt[]\n";
					for (int j = 0; j < 4; j++)  cout << "pdv_pt[" << j << "]: " << pdv_pt[j] << "\n";
				}
				if (retries > 0) cout << "Reattempting to close channel " << i << "...\n";
				retries--;
			}
		}
	}
	return (retries == 2) ? 1 : 0;
}

unsigned char* Camera::single_image(int ipdv)			//used by LiveFeed.cpp
{
	if (!pdv_pt[ipdv])
		return nullptr;
	pdv_flush_fifo(pdv_pt[ipdv]);
	pdv_start_image(pdv_pt[ipdv]);
	return pdv_wait_image_raw(pdv_pt[ipdv]);
}

// Load cfg files by running PDV initcam script. Do this before opening channels.
void Camera::load_cfg() {
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
}

void Camera::init_cam()				// entire module based on code from Chun - sm_init_camera.cpp
{
	char command[80];
	char buf[256];

	//-------------------------------------------
	// Attempt channel open before allocating memory
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		if (open_channel(ipdv)) {
			fl_alert("Camera::init_cam Failed to open the channel!\n");
			return;
		}
	}

	// 200 Hz seems to like when cfg is loaded after open channel 
	load_cfg();

	// RCL # is recall settings -- accesses settings from the camera's storage
	sprintf(command, "@RCL %d", m_program); // 
	serial_write(command);
	Sleep(20);
	sprintf(command, "@SEQ 0"); // stop
	serial_write(command);
	Sleep(20);

	/*
	cam->serial_write("@AAM?\r");
	cam->serial_read(buf, bufsize);
	printf("%s\n", buf);

	cam->serial_write("@REP?\r");
	cam->serial_read(buf, bufsize);
	printf("%s\n", buf);
	*/

	program(m_program);

	// More from Little Joe book (Little Dave maybe uses the same commands? Chun's code sends these commands to 2K's, at least).
	// TXC - take external control
	// AAM - attenuation/gain
	// SEQ - stop(0) or start(1)
	// TMP? - query the temperature

	int hbin = int(ccd_lib_bin[m_program] > 1);
	//if (ccd_lib_bin[m_program] > 1) hbin = 1;
	int start_line = start_line_lib[m_program];
	// Changed start line to hard-coded lib only.
	//if (start_line_lib[m_program]) start_line = start_line_lib[m_program];
	//else start_line = 65 + (1024 - HEIGHT[m_program] * ccd_lib_bin[m_program] / 2);		// Chun says cam_num_row gets doubled - did not fully understand

	// prog #1 seems to not like when pdv_setsize is called here
	//program(m_program); // calls pdv_setsize for superframed, like sm.cpp line 1982

	sprintf(command, "@SPI 0; 2");
	serial_write(command);
	Sleep(20);
	sprintf(command, "@SPI 0; 0");
	serial_write(command);
	Sleep(20);
	sprintf(command, "@SPI 0; 4");
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

	Sleep(50);
	// @TXC 1 -- external control from DO.0 (line 0 -- black sync). This is needed if you want p-clamp to trigger camera
	// @TXC 0 -- camera will start from software and then trigger all other writing/acquisition on clock PFI0. Simple, seems to work well.
	sprintf(command, "@TXC 0");
	serial_write(command);
	sprintf(command, "@SEQ 1"); // start 
	serial_write(command);
}

void Camera::prepare_acqui() {
	char command[80];
	sprintf(command, "@TXC 1");
	serial_write(command);
}

void Camera::set_freerun_mode() {
	char command[80];
	sprintf(command, "@TXC 0");
	serial_write(command);
}

// Starts image acquisition for one channel
void Camera::start_images(int ipdv, int count) {
	if (!pdv_pt[ipdv]) 		return;
	pdv_flush_fifo(pdv_pt[ipdv]);				 // pdv_timeout_restart(pdv_p, 0);	same as pdv_flush  7-4-2020
	pdv_multibuf(pdv_pt[ipdv], 4); 	// Suggested by Chun 5-14-2020		//change pdv_p to pdv_pt[0]  9-23-2020
	// count 	number of images to start. A value of 0 starts freerun. To stop freerun, call pdv_start_images again with a count of 1.
	pdv_start_images(pdv_pt[ipdv], count); 
	/*
	if (get_superframe_factor() > 1) {
		pdv_start_images(pdv_pt[ipdv], get_superframe_factor()); // changed COUNT arg from 0 to superframe factor.
	}
	else {
		pdv_start_images(pdv_pt[ipdv], 0); // freerun
	}*/
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
	unsigned char* image = pdv_wait_image_raw(pdv_pt[ipdv]);
	if (edt_reg_read(pdv_pt[ipdv], PDV_STAT) & PDV_OVERRUN)
		++overruns;
	return image;
}

unsigned char** Camera::wait_images(int ipdv, int count) {
	pdv_wait_images(pdv_pt[ipdv], count);
	if (edt_reg_read(pdv_pt[ipdv], PDV_STAT) & PDV_OVERRUN)
		++overruns;
	unsigned char** image = pdv_buffer_addresses(pdv_pt[ipdv]);
	return image;
}

// Write a command from buf to IPDVth channel
// Important: Only channel 0 of Little Dave supports serial write
void Camera::serial_write(const char* buf) {
	// Writes a commend 
	char buffer[512];
	/*
	
	pdv_serial_read(pdv_pt[0], buffer, 512 - 1); // flush camera buffer of lingering data
	memset(buffer, 0, sizeof(char) * 512);
	strcpy_s(buffer, buf);
	pdv_serial_write(pdv_pt[0], buffer, (int)strlen(buffer));
	*/
	//From SM_serial command 
	//char ret_c[256];
	//int toRead;
	pdv_serial_read(pdv_pt[0], buffer, 512 - 1); // flush camera buffer of lingering data
	memset(buffer, 0, sizeof(char) * 512);
	strcpy_s(buffer, buf);

	pdv_serial_command(pdv_pt[0], buffer);
	//toRead = pdv_serial_wait(pdv_pt[0], 255, 256);
	//pdv_serial_read(pdv_pt[0], ret_c, toRead);
	
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
			//if (pdv_pt[ipdv]) {
				//sprintf_s(buf, "@RCL %d\r", m_program);		// sending RCL first did not work!
				//serial_write(buf);
				//if (get_superframe_factor() > 1) {
			if (pdv_setsize(pdv_pt[ipdv], width(), height() * get_superframe_factor()) < 0)
				cout << "Enabling superframing via pdv_setsize failed! Camera::program()\n";
			else
				cout << "pdv_setsize called with \n\twidth: " << width() << "\n\theight: " << height() * get_superframe_factor() << "\n";
			//} 
		}
	}
}

int Camera::get_superframe_factor() {
	return super_frame_lib[m_program];
}

int Camera::get_ccd_bin_factor() {
	return ccd_lib_bin[m_program];
}


// width of one quadrant BEFORE CDS subtraction
int Camera::width() {
	return WIDTH[m_program];
}

// height of one quadrant BEFORE CDS subtraction
int Camera::height() {
	return HEIGHT[m_program];
}

// width of full image to display
int Camera::get_display_width() {
	return DISPLAY_WIDTH[m_program];
}

// height of full image to display
int Camera::get_display_height() {
	return DISPLAY_HEIGHT[m_program];
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
	int array_diodes_quadrant = (num_diodes - num_fp_diodes) / NUM_PDV_CHANNELS * 2; // x2 for CDS pixels
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
		int PDV_size = get_buffer_size(ipdv) / get_superframe_factor();
		if (get_ccd_bin_factor() == 4) { // tmp
			PDV_size /= 2;
		}
		if (get_ccd_bin_factor() == 8) { // tmp
			PDV_size /= 4;
		}
		if (array_diodes_quadrant != PDV_size / 2) {
			cout << "\nAborting acquisition. The size PhotoZ expects for quadrant " << ipdv << " is: \t" \
				<< array_diodes_quadrant << " pixels (including CDS reset)" \
				<< "\nBut the size allocated by PDV for this PDV channel quadrant is:\t\t" \
				<< PDV_size << " bytes = " << PDV_size / 2 << " pixels.\n" \
				<< "\tPDV width: " << pdv_get_cam_width(pdv_pt[ipdv]) << "\n" \
				<< "\tPDV height: " << pdv_get_cam_height(pdv_pt[ipdv]) << "\n" \
				<< "\tsuperframe factor: " << get_superframe_factor() << "\n";
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
	//unsigned short* memory = new(std::nothrow) unsigned short[(size_t)num_diodes * (numPts + 1) * NUM_PDV_CHANNELS];
	unsigned short* memory = new(std::nothrow) unsigned short[(size_t)(width() * height()) * (numPts + 1) * NUM_PDV_CHANNELS];
	if (!memory) {
		fl_alert("Ran out of memory!\n");
		return nullptr;
	}
	memset(memory, 0, (width() * height()) * (numPts + 1) * NUM_PDV_CHANNELS * sizeof(short));
	return memory;
}

// FIRST: set to True if this camera session has not acquired images yet
// NOTLAST: set to True if this camera session will continue after this acquisition (i.e. for RLI)
// Returns: whether there was an error
bool Camera::acquireImages(unsigned short* memory, int numPts, int first, int notLast) {
	//if (first) serial_write("@SEQ 0\@SEQ 1\r@TXC 1\r");

	if(acquireImages(memory, numPts)) return true;

	if (!notLast) {
		for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {
			end_images(ipdv);
		}
		//serial_write("@TXC 0\r");
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
		start_images(ipdv,1);
		int tos = 0;
		//for (int ii = 0; ii < 7; ii++) image = wait_image(ipdv);		// throw away first seven frames to clear camera saturation	
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
				//if (ipdv == 0) serial_write("@TXC 0\r"); // only write to channel 0	
				failed = true; //Don't return from a parallel section	
			}
			//cout << "Channel " << ipdv << " acquired its portion of image " << i << "\n";
		}
	}
	return failed;
}

// Apply CDS subtraction, deinterleave, and quadrant remapping to a list of raw images.
void Camera::reassembleImages(unsigned short* images, int nImages) {

	// NOTE: cam->height and cam->width are the RAW QUADRANT sizes
	// i.e. the size of a quadrant BEFORE CDS subtraction
	
	int channelOrders[16] = { 2, 3, 1, 0,
							  1, 0, 2, 3,
							  2, 3, 1, 0,
							  1, 0, 2, 3, };

	// CDS subtraction for entire image (halves total memory needed for images)
	subtractCDS(images, nImages, height(), width()); 

	size_t quadSize = width() * height() / 2;
	size_t oneProcessedImageSize = quadSize * NUM_PDV_CHANNELS;
	size_t nProcessedImagesSize = oneProcessedImageSize * nImages;
	size_t oneChannelBlockSize = quadSize * nImages; 

	unsigned short* img, *tmpBuf;

	img = images;
	tmpBuf = images + nProcessedImagesSize; // After CDS subtract, the second half of IMAGES array is now free for use 
	// Deinterleave
	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {

		// adopted new raw image format, skip channel blocks ("channel-major order")
		// destination is in image-by-image format ("image-major order")
		for (int i = 0; i < nImages; i++) {

			deinterleave(img, 
				tmpBuf, 
				height(),
				width() / 2,
				channelOrders + (ipdv * 4));

			tmpBuf += oneProcessedImageSize;
			img += quadSize;

		}
		tmpBuf -= nProcessedImagesSize;
		tmpBuf += quadSize;
	}

	img = images;
	tmpBuf = images + nProcessedImagesSize;
	// Remap and move to final location (first half of IMAGES array)
	for (int i = 0; i < nImages; i++) {
		
		// Operates on all 4 PDV quadrants, overwriting orig image
		remapQuadrantsOneImage(tmpBuf, img, height(), width() / 2);
		img += oneProcessedImageSize;
		tmpBuf += oneProcessedImageSize;

		if (i % 100 == 0) cout << "Image " << i << " of " << nImages << " done.\n";
	}
}


// Subtract reset data (stripes) for correlated double sampling (CDS) for a single image.
// This halves the number of columns of each raw image
// quad_width is the final width of the frame
void Camera::subtractCDS(unsigned short* image_data, int nImages, int quad_height, int quad_width)
{
	int CDS_add = 2048;
	int CDS_width_fixed = quad_width / 2; // 6/25/21 - empirically, seems like this is always half of cfg width
	int CDS_height_total = nImages * (quad_width / CDS_width_fixed) / 2 * quad_height * NUM_PDV_CHANNELS; // div by 2 since loop skips 2 CDS-size rows per iter

	unsigned short* new_data = image_data;
	unsigned short* reset_data = image_data + CDS_width_fixed;
	unsigned short* old_data = image_data;

	for (int i = 0; i < CDS_height_total; i++) {
		for (int j = 0; j < CDS_width_fixed; j++) {
			*new_data++ = CDS_add + *old_data++ - *reset_data++;
		}
		reset_data += CDS_width_fixed;
		old_data += CDS_width_fixed;
	}
}

// This deinterleave follows Chun's specs over email (row-by-row) instead of quadrant_readout.docx
// Before deinterleaving, the raw data order for each row comes in like this (d for pixel data, r for pixel reset, which would be used for the next frame):
// d1, d64, d128, d192, d2, d65, d129, d192, d3,d256, r1, r64, r128, r192 r256 (total 512)
// Note quadWidth is the width NOT doubled for CDS, i.e. the final image width
void Camera::deinterleave(unsigned short* srcBuf, unsigned short* dstBuf, int quadHeight, int quadWidth, const int* channelOrder) {

	int quadSize = quadWidth * quadHeight;

	int numCamChannels = 4; // each PDV channel (quadrant) has 4 camera channels

	unsigned short* data_ptr = srcBuf;
	// This is the width spanned by the pixels from 1 of the 4 camera channels per PDV channel:
	int camChannelWidth = quadWidth / numCamChannels;

	for (int row = 0; row < quadHeight; row++) {
		for (int col = 0; col < quadWidth; col++) {
			int chOrd = channelOrder[col % numCamChannels];
			int camChOffset = chOrd * camChannelWidth;
			
			dstBuf[row * quadWidth + (col / numCamChannels) + camChOffset]
				= *data_ptr++;
		}
	}
}


// Quadrant Mapping
// Since CDS subtraction halved the image size, we now have auxiliary space
// to use to arrange the quadrants of the full image
//		- space out the rows of quadrants 0 and 2
//		- interleave in the rows of quadrants 1 and 3
void Camera::remapQuadrantsOneImage(unsigned short* srcBuf, unsigned short* dstBuf, int quadHeight, int quadWidth) {

	int quadSize = quadHeight * quadWidth;
	int img_width = quadWidth * 2;
	int img_height = quadHeight * 2;
	bool flipVertically, flipHorizontally;

	for (int ipdv = 0; ipdv < NUM_PDV_CHANNELS; ipdv++) {

		flipVertically = (ipdv % 2 == 1);
		flipHorizontally = false;

		int img_row, img_col;

		// PDV channels for little Dave are readout in this order: 

		//  0 - upper left, 
		if (ipdv == 0) {
			img_row = 0;
			img_col = 0;
		}
		//  1 - upper right,  
		if (ipdv == 1) {
			img_row = quadHeight;
			img_col = 0;
		}
		//  2 - lower left, 
		if (ipdv == 2) {
			img_row = 0;
			img_col = quadWidth;
		}
		//  3 - lower right.
		if (ipdv == 3) {
			img_col = quadWidth;
			img_row = quadHeight;
		}

		for (int row = 0; row < quadHeight; row++) {
			for (int col = 0; col < quadWidth; col++) {

				int r = flipVertically ? (quadHeight - 1 - row) : row;
				int c = flipHorizontally ? (quadWidth - 1 - col) : col;

				dstBuf[(img_row + row) * img_width + (img_col + col)]
					= srcBuf[ipdv * quadSize + r * quadWidth + c];
			}
		}
	}
}


// Utilities for debugging
void Camera::printFinishedImage(unsigned short* image, const char* filename, bool CDS_done) {
	int full_img_size = NUM_PDV_CHANNELS * width() * height(); 
	if (CDS_done) full_img_size /= 2; // Divide by 2 if you need to account for CDS subtraction

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

// Issue a serial command to PDV channel 0
void Camera::serial_command(const char* command)
{
	char ret_c[256];
	int toRead;

	pdv_serial_command(pdv_pt[0], command);
	toRead = pdv_serial_wait(pdv_pt[0], 255, 256);
	pdv_serial_read(pdv_pt[0], ret_c, toRead);
}