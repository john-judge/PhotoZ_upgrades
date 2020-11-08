/** ********************************
 * 
 * @file ErrorHandler.cpp
 *
 * Copyright (c) Engineering Design Team 2010
 * All Rights Reserved
 */


// ErrorHandler.cpp: implementation of the ErrorHandler class.
//
//////////////////////////////////////////////////////////////////////

//#include "edtdefines.h"
#include "ErrorHandler.h"

using namespace std;

static void LogToFile(void *target, const char *s)

{
    FILE *f = (FILE *) target;

    if (f)
	fprintf(f, s);

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ErrorHandler::ErrorHandler()
{
    m_File = NULL;
    m_FileName = "";
    m_bAlertWindow = false;
    //init_critical(m_lock);
    m_logger = LogToFile;
    SetTarget(stdout);
    m_last_error = 0;
    throwing_errors = false;
    own_file = false;
#ifdef __APPLE__
    create_mutex(*m_lock);
#else
    create_mutex(m_lock);
#endif


}

ErrorHandler::~ErrorHandler()
{
    CloseFile();
}

void ErrorHandler::CloseFile()

{
    if (m_File && own_file)
    {
        fclose(m_File);
        m_File = NULL;
        own_file = false;
    }
}

void ErrorHandler::SetFile(FILE *f)

{

    if (m_File == f)
    {
        return;
    }

    CloseFile();
    m_File = f;
    m_target = f;
    own_file = false;
}

void ErrorHandler::OpenFile()

{
    m_File = fopen(m_FileName.c_str(), "w+");

    if (m_File)
        own_file = true;
    else
        own_file = false;
}


void ErrorHandler::LogString(const char * s)
{
    Lock();

    if (m_logger)
    {
	m_logger(m_target, s);
    }

    Unlock();

}

void ErrorHandler::SysErrorMessage(const char *fmt, ...)

{
    va_list stack;

    va_start(stack,fmt);

    char buffer[1024];

#ifdef WIN32
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL, GetLastError(),0,
	buffer, 1023, NULL);
#else
    buffer[0] = 0;
#endif
    vsprintf(buffer + strlen(buffer), fmt, stack);


    LogString(buffer);

}

void ErrorHandler::SysErrorMessage(const int errorid, const char *fmt, ...)

{
    va_list stack;

    va_start(stack,fmt);

    char buffer[1024];

#ifdef WIN32
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL, errorid,0,
	buffer, 1023, NULL);

#else
    buffer[0] = 0;

#endif
    vsprintf(buffer + strlen(buffer), fmt, stack);

    LogString(buffer);

}

// Log a message to the log file
void ErrorHandler::Message(const char *fmt, ...)

{
    va_list stack;

    va_start(stack,fmt);

    char buffer[1024];

    vsprintf(buffer,fmt, stack);

    LogString(buffer);

}

void ErrorHandler::SetTarget(void *target)

{

    CloseFile();

    m_target = target;
}

void ErrorHandler::SetTarget(const char *name)

{
    

    if (m_File)
    {
	if (name && (name == m_FileName))
	    return;
        
        CloseFile();
    }

    if (name)
    {
        m_FileName = name;
        OpenFile();
    }
    else
        m_FileName = "";

}

int ErrorHandler::SetLastError(int error_type, const char *fmt, ...)

{
    va_list stack;

    va_start(stack,fmt);

    char buffer[1024];

    buffer[0] = 0;

    vsprintf(buffer, fmt, stack);
    m_last_error = error_type;
    m_last_error_string = buffer;

    Message(buffer);

    if (throwing_errors)
        throw m_last_error_string;

    return error_type;
}

const string & ErrorHandler::GetLastErrorString()

{
    return  m_last_error_string;

}

int ErrorHandler::MessageFunction(void *target, int level, char *message)

{
    ErrorHandler *obj = (ErrorHandler *) target;

    obj->LogString(message);

    return 0;
}


ErrorHandler SysLog;
ErrorHandler SysError;

void SetEdtErrorHandler()

{
    EdtMsgHandler *msg_p = edt_msg_default_handle();

    edt_msg_set_target(msg_p, &SysError);

    edt_msg_set_function(msg_p, ErrorHandler::MessageFunction);

    

}
