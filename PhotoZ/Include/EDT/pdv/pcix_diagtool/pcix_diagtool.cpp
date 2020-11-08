#include "edtinc.h"
#include "pcixcfg.h"
#include <stdio.h>
#include <iostream>
#include <bitset>
#include <sstream>

using namespace std;


int main(int argc, char **argv)
{
    edt_buf buf;
    u_int capId = 0;
    int unit = 0; //default dev unit
    int ret;
   
    while(argc > 1 && argv[1][0] == '-')
    {
	switch(argv[1][1])
	{
	    case 'u':
		++argv;
		--argc;
		unit = atoi(argv[1]);
		break;
        }
        --argc;
	++argv;
    }

    EdtDev *edt_p;

    if((edt_p = edt_open_channel(EDT_INTERFACE, unit, 0)) == NULL)
    {
    	printf("Failed to open channel");
    	return 0;
    }

    //start with address 0 and check the cap ID
    //if cap ID == 0x10 then we are at the 
    //capability register (where we want to be)
    u_int addr = 0x0;

    buf.desc = addr;
    ret = edt_ioctl(edt_p, EDTG_CONFIG, &buf);

    u_int devId = (buf.value >> 16);
    u_int venId = (buf.value & 0x0000ffff);

    //create configuration object
    pcixCfg *cfgObj = new pcixCfg();
    cfgObj->set_ids(devId, venId);

    addr = 0x4;
    buf.desc = addr;
    ret = edt_ioctl(edt_p, EDTG_CONFIG, &buf);
    u_int mbit = (buf.value & 0x0002) >> 1;
    cfgObj->set_masterbit(mbit);

    //get the pointer to the capabilities register
    addr = 0x34;
    buf.desc = addr;
    edt_ioctl(edt_p, EDTG_CONFIG, &buf);
    u_int cap_ptr = buf.value;
    u_int prev_addr;

    do
    {
	buf.desc = cap_ptr;
        prev_addr = cap_ptr;
        edt_ioctl(edt_p, EDTG_CONFIG, &buf);
        capId = (buf.value & 0x000000ff);
    	cap_ptr = ((buf.value & 0x0000ffff) >> 8);


    } while(capId != cfgObj->get_capId() && cap_ptr != 0);

    if(capId != 0x10)
    {
    	cout << "Error: No express configuration register found. "  << endl;
    	return 0;
    }

    cfgObj->display_ids();
    cfgObj->display_masterbit();

    //get device capabilities
    buf.desc = prev_addr + 0x4;
    edt_ioctl(edt_p, EDTG_CONFIG, &buf);
    cfgObj->set_dcr(buf.value);
    cfgObj->display_dcr();

    buf.desc = prev_addr + 0x8;
    edt_ioctl(edt_p, EDTG_CONFIG, &buf);
    cfgObj->set_devstat(buf.value);
    cfgObj->display_devstat();

    buf.desc = prev_addr + 0x8;
    edt_ioctl(edt_p, EDTG_CONFIG, &buf);
    cfgObj->set_devctrl(buf.value);
    cfgObj->display_devctrl();
    cout << endl;

    buf.desc = prev_addr + 0x10;
    edt_ioctl(edt_p, EDTG_CONFIG, &buf);
    cfgObj->set_linkstat(buf.value);
    cfgObj->display_linkstat();
    cout << endl;

	return 1;
    
}



		
    



