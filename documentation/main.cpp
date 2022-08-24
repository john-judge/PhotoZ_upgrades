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


