
#ifndef PDVGLLIVEWINDOW_H
#define PDVGLLIVEWINDOW_H

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Scrollbar.H>

#include "edtinc.h"

#include "PdvInput.h"


#include "ImageEventFunc.h"
#include "EdtList.h"

#include "BufferTimerData.h"

#include "ZoomData.h"

#include "EdtLut.h"

#include "PdvGlWindow.h"

class PdvGlLiveWindow : public PdvGlWindow {
 
public:

    bool m_bOwnCamera;


    u_char *buffer;
    bool capturing;
    int nbuffers;
    int total_skipped;

    void capture_single();
    void capture_single_and_draw();

    void start_streaming();

    void stop_streaming();

 protected:

 
    bool m_bDoLocks;
    bool m_bActiveFlag;
    bool m_bTimedOut;

    mutable CRITICAL_SECTION m_lock;

    thread_t m_pThread;

    int FrameCount;
    double m_dStartTime;
    double m_dEndTime;

    BufferTimerData m_Time;

    int m_nMaxCount;

    mutable int m_nLocks;

    int m_nLastTimeStamp;
    int m_nFrameMod;

    int m_nChannelID;
    
    PdvInput * m_pCamera;

    int m_nLastCount;

    bool m_bStartPending;

    int m_nAcquireSkip;

    int m_nDisplaySkip;

    EdtImage *m_pLastImageCaptured;

    EdtListHead<EventFuncRec> m_AcqList;
    EdtListHead<EventFuncRec> m_FinishList;

    
    bool m_bWndDirect;
    bool m_bDrawLive;

public:
    
    THREAD_RETURN DoLiveThreadFunc();

    virtual bool AllFinished();

    bool Start(bool bUseThread = true);

    virtual int GetTimeouts();

    int GetFrameCount();

    PdvGlLiveWindow(int x, int y, int w, int h, const char *l=0, PdvInput *in_pCamera = NULL);

    PdvGlLiveWindow(char *devname = NULL, 
	const int nUnit = 0, const int nChannel = 0,
	const bool bNoReset = false);

    PdvGlLiveWindow(PdvInput  *pCamera);

    PdvGlLiveWindow(PdvDev *pdv_p);

    virtual ~PdvGlLiveWindow();

    void SetPdv(PdvInput *in_pCamera);

    PdvInput * GetPdv()
    {
	return m_pCamera;
    }

    void Lock() 
    {
	if (m_bDoLocks) {

	    EnterCriticalSection(&m_lock);

	      m_nLocks++;
	}
    }

    void Unlock() 
    {
	if (m_nLocks > 0) {
	    LeaveCriticalSection(&m_lock);

	    m_nLocks--;
	}
    }

    void SetActiveFlag(bool nActiveFlag) 
    {
	Lock();
	m_bActiveFlag = nActiveFlag;
	Unlock();
    }

    bool GetActiveFlag() 
    {
	Lock();
	bool rc = m_bActiveFlag;
	Unlock();
	return rc;
    }

    void SetTimedOut(bool nTimedOut) 
    {
	Lock();
	m_bTimedOut = nTimedOut;
	Unlock();
    }

    bool GetTimedOut() 
    {
	Lock();
	bool rc = m_bTimedOut;
	Unlock();
	return rc;
    }
    

    void NotifyEventFuncs(EdtImage *pImage, long nFrameNumber);
    void AddEventFunc(ImageEventFunc f, void *target);
    void RemoveEventFunc(ImageEventFunc f, void *target);
    void RemoveAllEventFuncs();

    void NotifyFinishFuncs();
    void AddFinishFunc(ImageEventFunc f, void *target);
    void RemoveFinishFunc(ImageEventFunc f, void *target);
  

    EdtImage * DoLiveWindow(bool display = true);

    bool IsLive() {
	Lock();
	bool rc = (m_pThread != (thread_t) 0);
	Unlock();
	return rc;
    }

    bool StopLive();

    // from VCRChannel


    int Stop()

    {
	StopLive();

	return 0;
    }

    void SetDisplaySkip(int nSkip)

    {
	m_nDisplaySkip = nSkip;
    }

    int GetDisplaySkip()
    {
	return m_nDisplaySkip;
    }


    void SetNBuffers(int n = 1);

    void SetMaxCount(const int nMaxCount)
    {
	m_nMaxCount = nMaxCount;
    }

    int GetMaxCount() const 
    {
	return m_nMaxCount;
    }

    int GetNFrames()
    {
	return 
	    GetMaxCount();
    }

    void SetBufferTime(int nBuffer, const double dtime)
    {
	m_Time.SetBufferTime(nBuffer, dtime);
    }

    double GetBufferTime(int nBuffer)
    {
	return m_Time.GetBufferTime(nBuffer);
    }

    
    int GetLastBuffer()
    {
	return m_Time.m_nLastBuffer;
    }

    double GetFrameRate()
    {
	
	return m_Time.GetFrameRate();
	
    
    }

    void SetNFrames(int nMaxCount)
    {
	SetMaxCount(nMaxCount);
    }


    double * GetTimeVector(int &nTimes)
    {
	nTimes = m_Time.m_nLastBuffer;
	return m_Time.m_pTimeVec;
    }

    EdtImage * GetCaptureImage()
    {
	return pDataImage;
    }
    void SetCamera(PdvInput *pCamera);

    void InitVariables();

    void InitValues(const char *PdvDev, const int nPdvUnit);

    PdvInput * GetCamera();

    void StartTimer()
    {
	m_Time.StartTimer();
    }

    virtual bool BuffersFinished();
    virtual EdtImage *GetNextBufferImage(int nCount);

    // return the last image captured
    virtual EdtImage *GetLastImage();

    virtual void StopPump();

    void StartPump();

    // stop acquiring immediately

    virtual void Abort();


    
};

PdvGlWindow *newLiveWindow(int x, int y, int w, int h, const char *l);



#endif
