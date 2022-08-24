// edt_camlink.cpp : Defines the entry point for the DLL application.
//

// #include "stdafx.h"

#include "edtinc.h"

#include "edt_camlink.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }


    return TRUE;
}

#ifdef __cplusplus

extern "C"
{

#endif


#ifdef OUTPUT_DEBUG
static	EdtMsgHandler *msg_p;
#define EDT_MESSAGE_LEVEL EDTAPP_MSG_INFO_1
#endif

EDT_CAMLINK_API CLINT32 clSerialInit(CLUINT32 serialIndex, hSerRef *serialRefPtr)

{
	// Note that this doesn't cover the two cameras available per board, so
	// serialIndex must be unit * 2 + channel
	PdvDev *pdv_p;

	int unit = serialIndex >> 1;
	int channel = serialIndex & 1;
	CLINT32 ret;

	if ((pdv_p = pdv_open_channel(NULL,unit,channel)) == NULL)
	{
		ret = CL_ERR_INVALID_REFERENCE;
	}
	else if (pdv_get_dmasize(pdv_p) <= 0)
	{
		ret = CL_ERR_EDT_NOT_INITIALIZED;
	}
	else if (!serialRefPtr)
	{
		ret = CL_ERR_INVALID_PTR;
	}
	else
	{
		*serialRefPtr = (void *) pdv_p;
		ret = CL_ERR_NO_ERR;
	}

#ifdef OUTPUT_DEBUG
		msg_p = edt_msg_default_handle();
		edt_msg_set_name(msg_p, "c:\\edt\\pdv\\clseredt.out");
		edt_msg_set_target(msg_p, msg_p->file);
		edt_msg_set_level(msg_p, 0xffff);

// DEBUG ONLY -- testing numports
	if (ret == CL_ERR_NO_ERR)
	{
		CLUINT32 numPorts;
		clGetNumPorts(&numPorts);
		edt_msg(EDT_MESSAGE_LEVEL, "numPorts %d\n", numPorts);
	}

	edt_msg(EDT_MESSAGE_LEVEL, "clSerialInit: ret %d\n", ret);

#endif
		
	return ret;
}

EDT_CAMLINK_API CLINT32 clSerialRead(hSerRef serialRef,
			CLINT8 buffer[], CLUINT32 *bufferSize, CLUINT32 serialTimeout)

{
	PdvDev *pdv_p = (PdvDev *) serialRef;
	CLINT32 ret;

	if (pdv_p)
	{
		int len = *bufferSize;

		if ((len = pdv_serial_wait(pdv_p, serialTimeout, len)) > 0)
			len  = pdv_serial_read_nullterm(pdv_p, buffer, len, FALSE);
		*bufferSize = len;

#ifdef OUTPUT_DEBUG
		int i;
		edt_msg(EDT_MESSAGE_LEVEL, "clSerialRead: len %d\n", len);;
		for (i=0;i<len;i++)
		    edt_msg(EDT_MESSAGE_LEVEL, "<%02x>", (u_char) buffer[i]);
		edt_msg(EDT_MESSAGE_LEVEL,"\n");
		for (i=0;i<len;i++)
		    if (isascii(buffer[i]))
				edt_msg(EDT_MESSAGE_LEVEL,"%c",(u_char) buffer[i]);
		    else
				edt_msg(EDT_MESSAGE_LEVEL,"<%02x>",(u_char) buffer[i]);
		edt_msg(EDT_MESSAGE_LEVEL,"\n");
#endif
		ret = CL_ERR_NO_ERR;		
	}	
	else
		ret = CL_ERR_INVALID_REFERENCE;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clSerialRead: ret %d\n", ret);
#endif
	return ret;
}


EDT_CAMLINK_API CLINT32 clSerialWrite(hSerRef serialRef,
			CLINT8 buffer[], CLUINT32 *bufferSize, CLUINT32 serialTimeout)

{
	PdvDev *pdv_p = (PdvDev *) serialRef;

	if (pdv_p && buffer && bufferSize )
	{
		int len = *bufferSize;

#ifdef OUTPUT_DEBUG
		int i;
		edt_msg(EDT_MESSAGE_LEVEL, "--- write %d ---\n", len);
		for (i=0;i<len;i++)
		    edt_msg(EDT_MESSAGE_LEVEL, "<%02x>", (u_char) buffer[i]);
		edt_msg(EDT_MESSAGE_LEVEL,"\n");
		for (i=0;i<len;i++)
		    if (isascii(buffer[i]))
			edt_msg(EDT_MESSAGE_LEVEL,"%c",(u_char) buffer[i]);
		    else
			edt_msg(EDT_MESSAGE_LEVEL,"<%02x>",(u_char) buffer[i]);
		edt_msg(EDT_MESSAGE_LEVEL,"\n");
#endif		
		
		pdv_serial_binary_command(pdv_p, buffer, len);

		return CL_ERR_NO_ERR;
	}
	else
		return CL_ERR_INVALID_REFERENCE;
}


EDT_CAMLINK_API CLINT32 clFlushPort(hSerRef serialRef)

{
	PdvDev *pdv_p = (PdvDev *) serialRef;
	CLINT8 buf[12];
	CLINT32 ret;
	int i = 0;

    if (pdv_p)
    {
		// flush up to 10000 bytes, 10 at a time;
		for(i=0; i<1000; i++)
		{
			if (pdv_serial_read(pdv_p, buf, 10) == 0)
				break;
		}
		ret = CL_ERR_NO_ERR;
    }
    else
		ret = CL_ERR_INVALID_REFERENCE;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clFlushPort: looped %d times, ret %d\n", i, ret);
#endif

	return ret;
}

EDT_CAMLINK_API CLINT32 clGetErrorText(const CLINT8 *manuName,
				 CLINT32 errorCode, CLINT8 *errorText, CLUINT32 *errorTextSize)
{
	CLINT32 ret = CL_ERR_NO_ERR;
	char *s = NULL;

    if (*errorTextSize <= 5)if ((manuName) && (strcmp(manuName, "EDT") == 0))
    {
		 // we don't have any special error codea at this time
		switch(errorCode)
		{
			case CL_ERR_EDT_NOT_INITIALIZED:
				s = "EDT DV board has not been initialized";
				break;
			case CL_ERR_INVALID_PTR: // this code is in the template .h file but not the CL spec
						// or some versions of clallserial.dll? So here just in case?
				s = "Invalid pointer";
				break;
			default:
				ret = CL_ERR_ERROR_NOT_FOUND;
				break;
		}
    }
   	else
		ret = CL_ERR_MANU_DOES_NOT_EXIST;

	if (s)
	{
		if (*errorTextSize >= strlen(s))
		{
			*errorTextSize = strlen(s);
			strcpy(errorText, s);
		}
		else ret = CL_ERR_BUFFER_TOO_SMALL;
	}
	else ret = CL_ERR_ERROR_NOT_FOUND;

#ifdef OUTPUT_DEBUG
    edt_msg(EDT_MESSAGE_LEVEL, "clGetErrorText ret %d\n", ret);
#endif

	return ret;
}

EDT_CAMLINK_API CLINT32 clGetManufacturerInfo(CLINT8 *mfgName,
						CLUINT32 *bufferSize, CLUINT32 *version)
{
	CLINT32 ret;
	
	if (mfgName && version && bufferSize && ((*bufferSize) > 3))
	{
		strcpy(mfgName, "EDT");
		*version = CL_DLL_VERSION_1_1;
		ret = CL_ERR_NO_ERR;
	}
	else
	{
		ret = CL_ERR_BUFFER_TOO_SMALL;
		*bufferSize = 4;
	}
	
#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clGetManufacturerInfo: name '%s' version %d ret %d\n", mfgName, version, ret);
#endif
	return ret;
}



EDT_CAMLINK_API CLINT32 clGetNumBytesAvail(hSerRef serialRef, CLUINT32 *numBytes)

{
    PdvDev *pdv_p = (PdvDev *) serialRef;
	CLINT32 ret;


    if (pdv_p && numBytes)
    {
		*numBytes = (CLUINT32)pdv_serial_get_numbytes(pdv_p);
		ret = CL_ERR_NO_ERR;
	}
    else
		ret = CL_ERR_INVALID_REFERENCE;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clGetNumBytesAvail: bytes %d ret %d\n", *numBytes, ret);
#endif
	return ret;
}

// alias subroutine due to bogus inconsistency in the spec between VB and C++ versions
EDT_CAMLINK_API CLINT32 clGetNumPorts(CLUINT32 *numPorts)
{
		return clGetNumSerialPorts(numPorts);
}

EDT_CAMLINK_API CLINT32 clGetNumSerialPorts(CLUINT32 *numPorts)
{
	CLINT32 ret;
	int nunits;
	

	if (numPorts)
	{
		Edt_bdinfo *bdinfo;

	    // always assume base mode and return 2 ports per board
		// documentation will need to reflect that with medium/full
		// cameras they need to always access only even-numbered ports
		bdinfo = edt_detect_boards("pdv", -1, &nunits, 0);
	    *numPorts = nunits * 2;

	    ret = CL_ERR_NO_ERR;
	}
	else
	    ret = CL_ERR_INVALID_PTR;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clGetNumPorts: numPorts %d ret %d\n", *numPorts, ret);
#endif

	return ret;
}

EDT_CAMLINK_API CLINT32 clGetPortInfo(CLUINT32 serialIndex,
	CLINT8* manufacturerName, CLUINT32* nameBytes, CLINT8* portID, CLUINT32* IDBytes, CLUINT32* version)
{
	CLINT32 ret = CL_ERR_NO_ERR;
	int unit = 0, channel = 0;

	if (manufacturerName && nameBytes && portID && IDBytes && version)
	{

	    if (*nameBytes < 4)
	    	ret = CL_ERR_BUFFER_TOO_SMALL;
	    else
	    	strcpy((char *)manufacturerName, "EDT");

	    *nameBytes = 4;

	    if (*IDBytes < 7)
	    	ret = CL_ERR_BUFFER_TOO_SMALL;
	    else
	    {
		unit = serialIndex/2;
		channel = serialIndex % 2;
		sprintf((char *)portID, "pdv%d_%d", unit, channel);
	    }
	    *IDBytes = 7;

	    *version = CL_DLL_VERSION_1_1;

	    return ret;
	}
	else ret = CL_ERR_BUFFER_TOO_SMALL;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clGetPortInfo: name '%s' ID '%s' version %d ret %d\n", manufacturerName, portID, *version, ret);
#endif

	return ret;
}

EDT_CAMLINK_API CLINT32 clGetSupportedBaudRates(hSerRef serialRef,
								CLUINT32 *baudRates)
{
    PdvDev *pdv_p = (PdvDev *) serialRef;
	CLINT32 ret;

    if (pdv_p && baudRates)
    {
		*baudRates = 0x1ffff;
		ret = CL_ERR_NO_ERR;
    }
    else
		ret = CL_ERR_INVALID_PTR;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clGetSupportedBaudRates: rates 0x%08x ret %d\n", *baudRates, ret);
#endif

	return ret;
}

EDT_CAMLINK_API CLINT32 clSetBaudRate(hSerRef serialRef, CLUINT32 baudRate)
{
    CLINT32 ret = CL_ERR_NO_ERR;
	int brate = 0;
    PdvDev *pdv_p = (PdvDev *) serialRef;

    if (pdv_p)
    {
		switch(baudRate)
		{
	    case CL_BAUDRATE_9600: brate = 9600; break;
	    case CL_BAUDRATE_19200: brate = 19200; break;
	    case CL_BAUDRATE_38400: brate = 38400; break;
	    case CL_BAUDRATE_57600: brate = 57600; break;
	    case CL_BAUDRATE_115200: brate = 115200; break; // most but not all PDV models...

	    case CL_BAUDRATE_230400: 			// nope
	    case CL_BAUDRATE_460800:			// nope
	    case CL_BAUDRATE_921600:			// nope
	    default: ret = CL_ERR_BAUD_RATE_NOT_SUPPORTED;
		}
    }
    else
		ret = CL_ERR_INVALID_REFERENCE;

    if (brate)
		pdv_set_baud(pdv_p, brate);

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clSetBaudRate: rate %d set %d ret %d\n", baudRate, brate, ret);
#endif

    return ret;
}

EDT_CAMLINK_API CLINT32 clGetSerialPortIdentifier (CLUINT32 serialIndex, CLINT8* portID, CLUINT32* bufferSize)

{
    int ret = CL_ERR_NO_ERR;
    int unit = 0, channel = 0;

    if (bufferSize)
    {
		if (*bufferSize < 7)
		    ret = CL_ERR_BUFFER_TOO_SMALL;
		else if (portID)
		{
			 unit = serialIndex/2;
			 channel = serialIndex % 2;
			 sprintf((char *)portID, "pdv%d_%d", unit, channel);
		}
		*bufferSize = 7;
    }
    else
		ret = CL_ERR_INVALID_INDEX;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clGetSerialPortIdentifier: ID '%s' size %d ret %d\n", portID, bufferSize, ret);
#endif
	return ret;

}

EDT_CAMLINK_API CLINT32 clSerialClose(hSerRef serialRef)
{
	PdvDev *pdv_p = (PdvDev *) serialRef;
	CLINT32 ret;

	if (pdv_p)
	{
	    pdv_close(pdv_p);	

	    ret = CL_ERR_NO_ERR;
	}
	else
	    ret = CL_ERR_INVALID_REFERENCE;

#ifdef OUTPUT_DEBUG
	edt_msg(EDT_MESSAGE_LEVEL, "clSerialClose: ret %d\n", ret);
#endif
	return ret;
}


#ifdef __cplusplus

}

#endif
