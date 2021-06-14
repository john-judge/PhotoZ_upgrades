// $Header:   M:\eng\dapio\src32\dll\import\c\dapio32.h_v   1.45   07 Mar 2002 12:41:36   XIAOZHI  $
// Copyright (c) 1992-2002, Microstar Laboratories, Inc.
/* 
 DAPIO32.H

 This header file declares the DAPIO32 interface version 2.00

*/

#ifndef __DAPIO32_H
#define __DAPIO32_H

#ifdef __GNUC__
#    define __stdcall __attribute__ ((stdcall))
#    define __cdecl   __attribute__ ((cdecl))
#    undef M_DapIo32Linkage
#    define M_DapIo32Linkage
#endif
 
// Use pack size of 4 bytes in all DAPIO structures.
//
#pragma pack (4)

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
// Define TDapIoInt64 type
#ifdef M_DapIoNoInt64
typedef struct tag_TDapIoInt64
{
  unsigned long dwLowPart;
  unsigned long dwHighPart;
} TDapIoInt64;
#else
#    ifdef __GNUC__
typedef long long TDapIoInt64;
#    else
typedef __int64 TDapIoInt64;
#    endif
#endif

// Open mode constants for DapHandleOpen(. . .)
// Note:
//   DAPOPEN_READ and DAPOPEN_WRITE constants match the Win32 constants
//   GENERIC_READ and GENERIC_WRITE. They can be interchanged with these
//   constants but are declared separately so that there is no dependency on
//   WINNT.H
#define DAPOPEN_READ                    (0x80000000L)
#define DAPOPEN_WRITE                   (0x40000000L)
#define DAPOPEN_QUERY                   (0x20000000L)
#define DAPOPEN_DISKIO                  (0x10000000L)

// File share mode constants for DapPipeDiskLog(. . .) and
//   DapPpeDiskFeed(. . .)
// Note:
//   These constants match the Win32 constants FILE_SHARE_READ and
//   FILE_SHARE_WRITE. They can be interchanged with these constants but are
//   declared separately so that there is no dependency on WINNT.H
#define DAPIO_FILE_SHARE_READ           (0x00000001)
#define DAPIO_FILE_SHARE_WRITE          (0x00000002)

// File open attribute constants for DapPipeDiskLog(. . .) and
//   DapPipeDiskFeed(. . .)
// Note:
//   These constants match the Win32 constants CREATE_NEW, CREATE_ALWAYS,
//   OPEN_EXISTING and OPEN_ALWAYS. They can be interchanged with these
//   constants but are declared separately so that there is no dependency on
//   WINBASE.H
#define DAPIO_CREATE_NEW                1
#define DAPIO_CREATE_ALWAYS             2
#define DAPIO_OPEN_EXISTING             3
#define DAPIO_OPEN_ALWAYS               4

// File attributes constants for DapPipeDiskLog(. . .) and
//   DapPipeDiskFeed(. . .)
// Note:
//   These constants match the Win32 constants FILE_ATTRIBUTE_NORMAL,
//   FILE_ATTRIBUTE_ENCRYPTED and FILE_ATTRIBUTE_READONLY. They can be
//   interchanged with these constants but are declared separately so that
//   there is no dependency on  WINNT.H
#define DAPIO_FILE_ATTRIBUTE_NORMAL         0x00000080
#define DAPIO_FILE_ATTRIBUTE_ENCRYPTED      0x00000040
#define DAPIO_FILE_ATTRIBUTE_READONLY       0x00000001

// File flags constants for DapPipeDiskLog(. . .) and
//   DapPpeDiskFeed(. . .)
// Note:
//   These constants match the Win32 constants FILE_FLAG_WRITE_THROUGH,  
//   FILE_FLAG_RANDOM_ACCESS, and FILE_FLAG_SEQUENTIAL_SCAN. They can be
//   interchanged with these constants but are declared separately so that
//   there is no dependency on WINBASE.H
#define DAPIO_FILE_FLAG_WRITE_THROUGH       0x80000000
#define DAPIO_FILE_FLAG_RANDOM_ACCESS       0x10000000
#define DAPIO_FILE_FLAG_SEQUENTIAL_SCAN     0x08000000

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

typedef struct tag_TDapBufferGetEx3
// information structure for DapBufferGetEx(. . .)
{
    int iInfoSize;            // size of this info structure
    int iBytesGetMin;         // minimum number of bytes to get
    int iBytesGetMax;         // maximum number of bytes to get
    int iReserved1;           // not used must be zero
    unsigned long dwTimeWait; // time to wait with no data before return
    unsigned long dwTimeOut;  // time to wait with data before return
    int iBytesMultiple;       // bytes to get is always a multiple of this
} TDapBufferGetEx3;

typedef struct tag_TDapBufferPutEx1
// information structure for DapBufferPutEx(. . .)
{
    int iInfoSize;            // size of this info structure
    int iBytesPut;            // number of bytes to put
    unsigned long dwTimeWait; // longest time in milliseconds to wait
    unsigned long dwTimeOut;  // longest time in milliseconds to put all
} TDapBufferPutEx1;

typedef struct tag_TDapBufferPutEx2
// information structure for DapBufferPutEx(. . .)
{
    int iInfoSize;            // size of this info structure
    int iBytesPut;            // number of bytes to put
    unsigned long dwTimeWait; // longest time in milliseconds to wait
    unsigned long dwTimeOut;  // longest time in milliseconds to put all
    int iBytesMultiple;       // bytes to put is always a multiple of this
    int iReserved1;           // not used, must be zero
} TDapBufferPutEx2;

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

// Enumerated data types used in DAPIO32. The type values are consistent to
//   Microsoft Win32 registry key value types except for DAPIO_VARIANT,
//   that is:
//
//   DAPIO_NONE     = REG_NONE
//   DAPIO_BINARY   = REG_BINARY
//   DAPIO_SZ       = REG_SZ
//   DAPIO_MULTI_SZ = REG_MULTI_SZ
//
enum DAPIO_ETYPES
{
  DAPIO_NONE      = 0,        // nothing
  DAPIO_BINARY    = 3,        // binary result
  DAPIO_SZ        = 1,        // null terminated result
  DAPIO_MULTI_SZ  = 7,        // Multiple null terminated results
  DAPIO_VARIANT   = 64,       // Dapio specific variant  
};

typedef struct tag_TDapHandleQuery2A
// information query structure for DapHandleQueryA(. . .) function
{
    int iInfoSize;        // size of this info structure
    char *pszQueryKey;    // pointer to a query key string
    union {               // If the iBufferSize is 0, the queury result is a 
      char *psz;          //   DWORD, returned in QueryResult.dw; otherwise,   
      void *pvoid;        //   the result is returned as a double null 
      unsigned long dw;   //   terminated multiple strings in a user-provided 
      } QueryResult;      //   buffer QueryResult.psz points to. If eResultType
    int iBufferSize;      //   is specified, the result is in the buffer
    int eResultType;      //   QueryResult.pvoid points to and the result                               
} TDapHandleQuery2A;      //   type is eResultType.      

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
// Disk I/O status values returned by "DiskIoStatus" query
//
enum TEDapDiskIoStatus
{
  ddios_Completed,         // disk I/O completed successfully
  ddios_Active,            // disk I/O is still active
  ddios_Aborted,           // disk I/O aborted due to an error
  ddios_FileError          // file error on disk I/O
};

//  Disk logging flags, used by the bmFlags field in the TDapPipeDiskLog
//    structure.
//  
enum TEDapPipeDiskLog
{
  dpdl_ServerSide    =  0x00000001,     // log on server side
  dpdl_FlushBefore   =  0x00000002,     // flush pipe before logging
  dpdl_FlushAfter    =  0x00000004,     // flush pipe after logging
  dpdl_MirrorLog     =  0x00000008,     // enable mirror logging
  dpdl_AppendData    =  0x00000010,     // append data to existing file
  dpdl_BlockTransfer =  0x00000020      // higher throughput using blocks
};

typedef struct tag_TDapPipeDiskLogA
// information structure for DapPipeDiskLog(. . .)
{
    int iInfoSize;                 // size of this info structure
    unsigned long bmFlags;         // flags to DapPipeDiskLog()
    char *pszFileName;             // pointer to string of log file names
    unsigned long dwFileShareMode; // file sharing attributes
    unsigned long dwOpenFlags;     // file opening attributes
    unsigned long dwFileFlagsAttributes; // file flags and attributes
    TDapIoInt64 i64MaxCount;       // maximum bytes to log
    unsigned long dwBlockSize;     // byte threshold for writing to file    
    unsigned long dwReserved[16];  // reserved must be zero
} TDapPipeDiskLogA;

enum TEDapPipeDiskFeed
{
  dpdf_ServerSide    =  0x00000001,   // source on server side
  dpdf_FlushBefore   =  0x00000002,   // flush pipe before feeding
  dpdf_BlockTransfer =  0x00000004,   // higher throughput using blocks
  dpdf_ContinuousFeed = 0x00000008    // repeat feed from start of file
};

typedef struct tag_TDapPipeDiskFeedA
// information structure for DapPipeDiskFeed(. . .)
{
    int iInfoSize;                 // size of this info structure
    unsigned long bmFlags;         // flags to DapPipeDiskLog()
    char *pszFileName;             // pointer to string of source file name
    unsigned long dwFileShareMode; // file sharing attributes
    unsigned long dwFileFlagsAttributes; // file flags and attributes    
    unsigned long dwBlockSize;     // block size in bytes for reading
    TDapIoInt64 i64MaxCount;       // maximum bytes to log
    unsigned long dwReserved[16];  // reserved must be zero
} TDapPipeDiskFeedA;

// mode bits for DapBufferPeek
#define dbpk_Absolute     0x00000000
#define dbpk_Relative     0x00000001

typedef struct tag_TDapBufferPeek1
// information structure for DapBufferPeek(...)
{
  int iInfoSize;            // size of this info structure
  int iBytesGetMin;         // minimum number of bytes to get
  int iBytesGetMax;         // maximum number of bytes to get
  unsigned long bmMode;     // bit mode
  unsigned long dwTimeWait; // time to wait with no data before return
  unsigned long dwTimeOut;  // time to wait with data before return
  int iBytesMultiple;       // bytes to get is always a multiple of this
  void *pBuffer;            // pointer to a user-supplied buffer    
  TDapIoInt64 i64OffsetRequested; // requested offset of data
  TDapIoInt64 i64OffsetResult;    // actuall offset of data returned
  int iBytesResult;         // number of bytes returned in the buffer    
} TDapBufferPeek1;

typedef TDapBufferGetEx3  TDapBufferGetEx;
typedef TDapBufferPutEx2  TDapBufferPutEx;
typedef TDapHandleQuery2A TDapHandleQueryA;
typedef TDapBufferPeek1   TDapBufferPeek;

// Module service flag bits
//
enum TEDapModuleFlags
{
    dmf_NoCopy              =  0x00000001, // do not copy the module file
    dmf_NoLoad              =  0x00000002, // do not load/unload the module
    dmf_ForceLoad           =  0x00000004, // force loading/unloading
    dmf_NoReplace           =  0x00000008, // do not replace the existing module file
    dmf_ForceRegister       =  0x00000010, // force register/unregister
    dmf_RemoveDependents    =  0x00000020  // remove dependent modules
};

// If using C++ then add a required extern "C" around function prototypes
#ifdef __cplusplus
  extern "C" {
#endif

// Functions supported by all Data Acquisition Processor servers.
//    
M_DapIo32Linkage BOOL __stdcall DapComPipeCreateA(const char *pszPipeInfo);
M_DapIo32Linkage BOOL __stdcall DapComPipeDeleteA(const char *pszPipeInfo);
M_DapIo32Linkage
  BOOL __stdcall DapHandleQueryA(HDAP hAccel, TDapHandleQueryA *pHandleInfo);
M_DapIo32Linkage BOOL __stdcall DapHandleQueryInt32A(HDAP hAccel, const char *pszKey, int * i32Result);
M_DapIo32Linkage BOOL __stdcall DapHandleQueryInt64A(HDAP hAccel, const char *pszKey, TDapIoInt64 *i64Result);

M_DapIo32Linkage BOOL __stdcall DapHandleClose(HDAP hAccel);
M_DapIo32Linkage HDAP __stdcall DapHandleOpenA(const char *pszAccelName, unsigned long ulOpenFlags);
M_DapIo32Linkage int  __stdcall DapInputAvail(HDAP hAccel);
M_DapIo32Linkage int  __stdcall DapOutputSpace(HDAP hAccel);

M_DapIo32Linkage int  __stdcall DapBufferGet(HDAP hAccel, int iLength, void *pvBuffer);
M_DapIo32Linkage int  __stdcall DapBufferGetEx(HDAP hAccel, const TDapBufferGetEx *pGetInfo, void *pvBuffer);
M_DapIo32Linkage int  __stdcall DapBufferPut(HDAP hAccel, int iLength, const void *pvBuffer);
M_DapIo32Linkage int  __stdcall DapBufferPutEx(HDAP hAccel, const TDapBufferPutEx *pPutInfo, const void *pvBuffer);
M_DapIo32Linkage BOOL __stdcall DapCharGetA(HDAP hAccel, char *pch);
M_DapIo32Linkage BOOL __stdcall DapCharPutA(HDAP hAccel, char ch);
M_DapIo32Linkage BOOL __stdcall DapCommandDownloadA(const TDapCommandDownloadA *pdcdl);

M_DapIo32Linkage BOOL __stdcall DapConfigA(HDAP hAccel, const char *pszDaplFilename);
M_DapIo32Linkage BOOL __stdcall DapConfigParamsClear(void);
M_DapIo32Linkage BOOL __stdcall DapConfigParamSetA(int iParamNumber, const char *pszParam);
M_DapIo32Linkage BOOL __stdcall DapConfigRedirectA(const char *pszOutputFilename);
M_DapIo32Linkage int  __stdcall DapInputFlush(HDAP hAccel);
M_DapIo32Linkage BOOL __stdcall DapInputFlushEx(HDAP hAccel, unsigned long dwTimeOut, unsigned long dwTimeWait, unsigned long *pdwFlushed);
M_DapIo32Linkage BOOL __stdcall DapOutputEmpty(HDAP hAccel);
M_DapIo32Linkage BOOL __stdcall DapInt16Get(HDAP hAccel, short *pi16);
M_DapIo32Linkage BOOL __stdcall DapInt16Put(HDAP hAccel, short i16);
M_DapIo32Linkage BOOL __stdcall DapInt32Get(HDAP hAccel, long *pi32);
M_DapIo32Linkage BOOL __stdcall DapInt32Put(HDAP hAccel, long i32);
M_DapIo32Linkage char * __stdcall DapLastErrorTextGetA(char *pszError, int iLength);
M_DapIo32Linkage BOOL __cdecl   DapStringFormatA(HDAP hAccel, const char *pszFormat, ...);
M_DapIo32Linkage BOOL __stdcall DapStringGetA(HDAP hAccel, int iLength, char *psz);
M_DapIo32Linkage BOOL __stdcall DapStringPutA(HDAP hAccel, const char *psz);
M_DapIo32Linkage int  __stdcall DapLineGetA(HDAP hAccel, int iLength, char *psz, unsigned long dwTimeWait);
M_DapIo32Linkage int  __stdcall DapLinePutA(HDAP hAccel, const char *psz);
M_DapIo32Linkage void __stdcall DapStructPrepare(void *pStruct, int size);
M_DapIo32Linkage BOOL __stdcall DapModuleInstallA(HDAP hAccel, const char *pszModPath, unsigned long bmFlags, void *pDapList);
M_DapIo32Linkage BOOL __stdcall DapModuleUninstallA(HDAP hAccel, const char *pszModName, unsigned long bmFlags, void *pDapList);
M_DapIo32Linkage BOOL __stdcall DapModuleLoadA(HDAP hAccel, const char *pszModPath, unsigned long bmFlags, void *pDapList);
M_DapIo32Linkage BOOL __stdcall DapModuleUnloadA(HDAP hAccel, const char *pszModName, unsigned long bmFlags, void *pDapList);
M_DapIo32Linkage BOOL __stdcall DapReinitialize(HDAP hAccel);
M_DapIo32Linkage BOOL __stdcall DapReset(HDAP hAccel);
M_DapIo32Linkage BOOL __stdcall DapServerControlA(const char *pszServer, const char *pszKey);
M_DapIo32Linkage BOOL __stdcall DapBufferPeek(HDAP hAccel, TDapBufferPeek *pPeekInfo);

// Functions only supported by DAPcell and DAPcell Local servers.
//    
M_DapIo32Linkage
  BOOL __stdcall DapPipeDiskLogA(HDAP hAccel, TDapPipeDiskLogA *pLogInfo, TDapBufferGetEx *pGetInfo);  
M_DapIo32Linkage
  BOOL __stdcall DapPipeDiskFeedA(HDAP hAccel, TDapPipeDiskFeedA *pFeedInfo, TDapBufferPutEx *pPutInfo);  

#ifdef __cplusplus
  }
#endif

#ifdef __cplusplus
template <class T> inline void DapStructPrepare(T & t)
{
   DapStructPrepare(&t, sizeof(T));
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
#    define TDapPipeDiskLog         TDapPipeDiskLogA
#    define TDapPipeDiskFeed        TDapPipeDiskFeedA
  
// Functions
//
#    define DapHandleOpen           DapHandleOpenA
#    define DapHandleOpenReadWrite  DapHandleOpenReadWriteA
#    define DapCharGet              DapCharGetA
#    define DapCharPut              DapCharPutA
#    define DapCommandDownload      DapCommandDownloadA
#    define DapComPipeCreate        DapComPipeCreateA
#    define DapComPipeDelete        DapComPipeDeleteA
#    define DapPipeDiskLog          DapPipeDiskLogA
#    define DapPipeDiskFeed         DapPipeDiskFeedA    
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
#    define DapHandleQueryInt32     DapHandleQueryInt32A
#    define DapHandleQueryInt64     DapHandleQueryInt64A
#    define DapModuleInstall        DapModuleInstallA
#    define DapModuleUninstall      DapModuleUninstallA
#    define DapModuleLoad           DapModuleLoadA
#    define DapModuleUnload         DapModuleUnloadA
#    define DapServerControl        DapServerControlA
#  endif

// Restore the original pack size.
#pragma pack ()

#endif  // __DAPIO32_H
