
/** ********************************
 * 
 * @file EdtImage/ErrorHandler.h
 *
 * Copyright (c) Engineering Design Team 2009
 * All Rights Reserved
 */


// EdtImage/ErrorHandler.h: interface for the ErrorHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_EDT_ERROR_HANDLER_H_)
#define _EDT_ERROR_HANDLER_H_

#include "edt_error.h"

#include "edt_utils.h"

#include <stdarg.h>

#include <stdio.h>

typedef void (*LogHandlerCallback)(void *objp, const char *fmt, ...);

typedef void (*log_function)(void *target, const char *s);

#include <string>


class ErrorHandler  
{
protected:
    int m_last_error;
    std::string  m_last_error_string;
    bool throwing_errors;
    bool own_file;
    std::string m_FileName;
    FILE *m_File;
    bool m_bAlertWindow;
#ifdef __APPLE__
    mutex_t *m_lock;
#else
    mutex_t m_lock;
#endif
    log_function m_logger;

    void *m_target;

public:
    ErrorHandler();
    virtual ~ErrorHandler();


    void SetTarget(const char *name);

    void SetTarget(void *target);

    void SetFile(FILE *file);

    // print a message to the file

    void Message(const char *fmt, ...);

    // Prints the system error message
    void SysErrorMessage(const int errorid, const char *fmt, ...);

    // Prints the last system error
    void SysErrorMessage(const char *fmt, ...);

    void LogString(const char *s);

    void Lock()
    {
	wait_mutex(m_lock);
    }

    void Unlock()
    {
        release_mutex(m_lock);
    }

    int SetLastError(int error_type, const char *fmt, ...);

    const std::string & GetLastErrorString();
    int GetLastError()
    {
        return m_last_error;
    }

    void SetThrowingErrors(const bool state)
    {
        throwing_errors = state;
    }

    bool GetThrowingErrors()
    {
        return throwing_errors;
    }

    void SetLogger(log_function f)
    {
        m_logger = f;
    }

    void CloseFile();
    void OpenFile();

    void *GetTarget()
    {
        return m_target;
    }
    
    log_function GetLogger()
    {
        return m_logger;
    }

    static int MessageFunction(void *target, int level, char *message);
};

extern ErrorHandler SysError;
extern ErrorHandler SysLog;

void   SetEdtErrorHandler();

#endif 

