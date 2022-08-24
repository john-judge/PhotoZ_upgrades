//=============================================================================
// Camera.cpp
//=============================================================================
#include "Camera.h"
#include <iostream>
#include "DapController.h"
#include "RecControl.h"		//added to get camGain on line 157

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
const int Camera::WIDTH[]  = {80,	80,		80,		80,		80,		40,		26,		80};
const int Camera::HEIGHT[] = {80,	80,		80,		80,		12,		40,		26,		80};
const int Camera::FREQ[]   = {1000,	500,	125,	40,		10000,	3000,	5000,	2000};
#endif // LILJOE
using namespace std;
#ifdef LILDAVE
const char* Camera::LABEL[] = {
    "200 Hz   2048x1024",
    "2000 Hz  2048x100",
    "1000 Hz  1024x320",
    "2000 Hz  1024x160",
    "2000 Hz  512x160",
    "4000 Hz  512x80",
    "5000 Hz  256x60",
    "7500 Hz  256x40"
};
const int Camera::WIDTH[]  = {2048,	2048,	1024,	1024,	512,	512,	256,	256};
const int Camera::HEIGHT[] = {1024,	100,	320,	160,	160,	80,		60,		40};
const int Camera::FREQ[]   = {200,	2000,	1000,	2000,	2000,	4000,	5000,	7500};
#endif // LILDAVE

Camera::Camera() {
	edt_devname[0] = '\0';
	strcpy(edt_devname, EDT_INTERFACE);
	unit = 0;
	channel = 0;
	numbufs = 4;
	pdv_p = NULL;
	timeouts = last_timeouts = m_num_timeouts = 0;
    m_depth = -1;
	overruns = 0;
    recovering_timeout = false;
    m_program = 7;                // the camera always boots up with prg0
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
	   
	pdv_enable_external_trigger(pdv_p, 0);	 // disable the trigger in case it was left on - it persists over restarts
	pdv_set_timeout(pdv_p, 1000);
    pdv_flush_fifo(pdv_p);
    m_depth = pdv_get_depth(pdv_p);
	// allocate ring buffers
    pdv_multibuf(pdv_p, numbufs);
	pdv_setsize(pdv_p, WIDTH[m_program], HEIGHT[m_program]);
    return 0;
}

unsigned char* Camera::single_image()
{
    if (!pdv_p)
        return nullptr;
	pdv_flush_fifo(pdv_p);
	pdv_start_image(pdv_p);
	return pdv_wait_image(pdv_p);
}

void Camera::start_images() {
    if (!pdv_p)
        return;

	pdv_flush_fifo(pdv_p);				 // pdv_timeout_restart(pdv_p, 0);	same as pdv_flush  7-4-2020
	pdv_multibuf(pdv_p, numbufs);		// Suggested by Chun 5-14-2020
    pdv_start_images(pdv_p, 0);
}

void Camera::end_images() {
    if (!pdv_p)
        return;
    pdv_start_images(pdv_p, 1);
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
    } else if (recovering_timeout) {
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

int Camera::program()
{
    return m_program;
}

void Camera::program(int p)
{
    char buf[80];
	short camGain = recControl->getAcquiGain();
    m_program = p;
    if (pdv_p) {
		serial_write("@FAM 3\r");
		int prog = 8 * camGain + m_program;		//  suggested by Chun in 4/19/2020 email
			if (pdv_p) {
				sprintf_s(buf, "@RCL %d\r", prog);		// sending RCL first did not work!
				serial_write(buf);
				pdv_setsize(pdv_p, WIDTH[p], HEIGHT[p]);
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

char* Camera::devname() {
    return edt_devname;
}

