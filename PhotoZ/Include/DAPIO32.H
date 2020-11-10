// $Header:   M:\eng\dapio\src32\dll\import\c\dapio32.h_v   1.16   13 Aug 1998 14:17:12   XIAOZHI  $
// Copyright (c) 1992-1998, Microstar Laboratories, Inc.
/* 
 DAPIO32.H

 This header file declares the routines in DAPIO32.DLL so that they can be
 called from C/C++ programs.

 Copyright (c) 1992-1996, Microstar Laboratories, Inc   
*/

#ifndef __DAPIO32_H
#define __DAPIO32_H


#ifndef M_DapIo32Linkage
#  define M_DapIo32Linkage __declspec(dllimport)
// Note: For versions of Borland C++ before 5.0 define M_DapIo32Linkage
// empty before including DAPIO32.H
#endif

#ifndef M_DapIo32DoNotDefineBOOL
#  ifndef _WINDEF_
// If WINDEF.H has not been included define a BOOL data type which is
// compatible with the BOOL defined in WINDEF.H.
// If this conflicts with another header define M_DapIo32DoNotDefineBOOL
// before including DAPIO32.H
// A BOOL data type must be defined before include of DAPIO32.H if
// M_DapIo32DoNotDefineBOOL is defined.
typedef int BOOL;
#    ifndef FALSE
#      define FALSE 0
#    endif
#    ifndef TRUE
#      define TRUE  1
#    endif
#  endif 
#endif

// Define a unique type for a handle to the DAP
typedef int HDAP;
// Define the TDapHandle type for compatability with Delphi symbols.
typedef HDAP TDapHandle;

// Open mode constants for DapHandleOpen(. . .)
// Note:
//   These constants match the Win32 constants GENERIC_READ and
//   GENERIC_WRITE. They can be interchanged with these constants but are
//   declared separately so that there is no dependency on WINNT.H
#define DAPOPEN_READ                    (0x80000000L)
#define DAPOPEN_WRITE                   (0x40000000L)
#define DAPOPEN_QUERY                   (0x20000000L)

// Structures for use in DAPIO services.
typedef struct tag_TDapBufferGetEx1
// obsolete information structure for DapBufferGetEx(. . .)
{
    int iInfoSize;          // size of this info structure
    int iMinCount;          // minimum number of bytes to get
    int iMaxCount;          // maximum number of bytes to get
} TDapBufferGetEx1;

typedef struct tag_TDapBufferGetEx2
// information structure for DapBufferGetEx(. . .)
{
    int iInfoSize;            // size of this info structure
    int iBytesGetMin;         // minimum number of bytes to get
    int iBytesGetMax;         // maximum number of bytes to get
    int iReserved;            // not used must be zero
    unsigned long dwTimeWait; // time to wait with no data before return
    unsigned long dwTimeOut;  // time to wait with data before return
} TDapBufferGetEx2;

#ifdef M_DapIoV100
// Force version 1.00 interface compatibility.

// Note: This compatibility switch will be removed in the next release
// of DAPIO32.H. It is only included to ease the transition to the new interface
// for DapBufferGetEx(. . .).
  typedef TDapBufferGetEx1 TDapBufferGetEx;
#else
  typedef TDapBufferGetEx2 TDapBufferGetEx;
#endif

typedef struct tag_TDapBufferPutEx
// information structure for DapBufferPutEx(. . .)
{
    int iInfoSize;            // size of this info structure
    int iBytesPut;            // number of bytes to put
    unsigned long dwTimeWait; // longest time in milliseconds to wait
    unsigned long dwTimeOut;  // longest time in milliseconds to put all
} TDapBufferPutEx;

typedef struct tag_TDapCommandDownloadA
// Information structure for DapCommandDownloadA(. . .)
{
  int iInfoSize;
  HDAP hdapSysPut;
  HDAP hdapSysGet;
  char *pszCCFileName;
  char *pszCCName;
  int iCCStackSize;
} TDapCommandDownloadA;

typedef struct tag_TDapHandleQuery1A
// information query structure for DapHandleQueryA(. . .) function
{
    int iInfoSize;        // size of this info structure
    char *pszQueryKey;    // pointer to a query key string
    union {               // If the queury result is a DWORD, it is returned 
      char *psz;          //   in QueryResult.dw; otherwise, it is returned as
      unsigned long dw;   //   a double null terminated multiple string in a
      } QueryResult;      //   user-provided buffer QueryResult.psz points to.
    int iBufferSize;      // If 0, the result will be dword; otherwise, it will
} TDapHandleQuery1A;      //   be a string, and this field is the size of the 
                          //   user-provided buffer

#ifndef M_DapIoV111i
typedef TDapHandleQuery1A TDapHandleQueryA;
#endif
  
// If using C++ then add a required extern "C" around function prototypes
#ifdef __cplusplus
  extern "C" {
#endif

// Function only supported under Windows NT
M_DapIo32Linkage BOOL __stdcall DapComPipeCreateA(const char *pszPipeInfo);
M_DapIo32Linkage BOOL __stdcall DapComPipeDeleteA(const char *pszPipeInfo);
M_DapIo32Linkage
  BOOL  __stdcall DapHandleQueryA(HDAP hAccel, TDapHandleQueryA *pHandleInfo);

// Functions supported by all versions of DAPIO32 DLL
M_DapIo32Linkage BOOL __stdcall DapHandleClose(HDAP hAccel);
M_DapIo32Linkage HDAP __stdcall DapHandleOpenA(const char *pszAccelName,
  unsigned long ulOpenFlags);
M_DapIo32Linkage int  __stdcall DapInputAvail(HDAP hAccel);
M_DapIo32Linkage int  __stdcall DapOutputSpace(HDAP hAccel);

M_DapIo32Linkage int  __stdcall DapBufferGet(HDAP hAccel, int iLength, void *pvBuffer);
M_DapIo32Linkage int  __stdcall DapBufferGetEx(HDAP hAccel,
  const TDapBufferGetEx *pGetInfo,
  void *pvBuffer);
M_DapIo32Linkage int  __stdcall DapBufferPut(HDAP hAccel, int iLength, const void *pvBuffer);
M_DapIo32Linkage int  __stdcall DapBufferPutEx(HDAP hAccel,
  const TDapBufferPutEx *pPutInfo,
  const void *pvBuffer);
M_DapIo32Linkage BOOL __stdcall DapCharGetA(HDAP hAccel, char *pch);
M_DapIo32Linkage BOOL __stdcall DapCharPutA(HDAP hAccel, char ch);
M_DapIo32Linkage BOOL __stdcall DapCommandDownloadA(const TDapCommandDownloadA *pdcdl);

M_DapIo32Linkage BOOL __stdcall DapConfigA(HDAP hAccel, const char *pszDaplFilename);
M_DapIo32Linkage BOOL __stdcall DapConfigParamsClear(void);
M_DapIo32Linkage BOOL __stdcall DapConfigParamSetA(int iParamNumber, const char *pszParam);
M_DapIo32Linkage BOOL __stdcall DapConfigRedirectA(const char *pszOutputFilename);
M_DapIo32Linkage int  __stdcall DapInputFlush(HDAP hAccel);
M_DapIo32Linkage BOOL __stdcall DapInputFlushEx(HDAP hAccel,
  unsigned long dwTimeOut,
  unsigned long dwTimeWait,
  unsigned long *pdwFlushed);
M_DapIo32Linkage BOOL __stdcall DapOutputEmpty(HDAP hAccel);
M_DapIo32Linkage BOOL __stdcall DapInt16Get(HDAP hAccel, short *pi16);
M_DapIo32Linkage BOOL __stdcall DapInt16Put(HDAP hAccel, short i16);
M_DapIo32Linkage BOOL __stdcall DapInt32Get(HDAP hAccel, long *pi32);
M_DapIo32Linkage BOOL __stdcall DapInt32Put(HDAP hAccel, long i32);
M_DapIo32Linkage char * __stdcall DapLastErrorTextGetA(char *pszError, int iLength);
M_DapIo32Linkage BOOL __cdecl   DapStringFormatA(HDAP hAccel, const char *pszFormat, ...);
M_DapIo32Linkage BOOL __stdcall DapStringGetA(HDAP hAccel, int iLength, char *psz);
M_DapIo32Linkage BOOL __stdcall DapStringPutA(HDAP hAccel, const char *psz);


M_DapIo32Linkage BOOL __stdcall DapLineGetA(HDAP hAccel, int iLength,
  char *psz, unsigned long dwTimeWait);
M_DapIo32Linkage int  __stdcall DapLinePutA(HDAP hAccel, const char *psz);

#ifdef __cplusplus
  }
#endif

// UNICODE and ANSI version mapping macros. Currently only ANSI verison is
//   supported.
//
#  ifdef UNICODE
//   DAPIO32 does not support UNICODE.
#    pragma message("DAPIO32 does not support UNICODE")
#  else
// ANSI mappings for DAPIO interfaces.
// Structures
//
#    define QUERY_INFO              QUERY_INFOA
#    define TDapCommandDownload     TDapCommandDownloadA
#    define TDapHandleQuery         TDapHandleQueryA

// Functions
//
#    define DapHandleOpen           DapHandleOpenA
#    define DapHandleOpenReadWrite  DapHandleOpenReadWriteA
#    define DapCharGet              DapCharGetA
#    define DapCharPut              DapCharPutA
#    define DapCommandDownload      DapCommandDownloadA
#    define DapComPipeCreate        DapComPipeCreateA
#    define DapComPipeDelete        DapComPipeDeleteA
#    define DapConfig               DapConfigA
#    define DapConfigParamSet       DapConfigParamSetA
#    define DapConfigRedirect       DapConfigRedirectA
#    define DapHandleQuery          DapHandleQueryA
#    define DapLastErrorTextGet     DapLastErrorTextGetA
#    define DapLineGet              DapLineGetA
#    define DapLinePut              DapLinePutA
#    define DapStringFormat         DapStringFormatA
#    define DapStringGet            DapStringGetA
#    define DapStringPut            DapStringPutA
#  endif 
#endif  // __DAPIO32_H
