
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <FL/math.h>
#include <FL/Fl_Output.H>

#include "edtinc.h"


#include "PdvGlScroller.h"

#include "PdvGlLiveWindow.h"

#ifdef WIN32

#include <process.h>

#endif

#define ARROW_SIZE 8

PdvGlLiveWindow::PdvGlLiveWindow(int x, int y, int w, int h , const char *l, PdvInput *in_pCamera) :
    PdvGlWindow(x,y,w,h,l)

{

    // initialize all variables

    InitVariables();

    SetPdv(in_pCamera);
}

void PdvGlLiveWindow::SetPdv(PdvInput *in_pCamera)
{
    m_pCamera = in_pCamera;

    if (m_pCamera)
    {

        pSingle = m_pCamera->GetCameraImage(pSingle);
        pDataImage = pSingle;

        if (m_pCamera->GetWidth() > 0)
        {
            capture_single();
        }

        ((PdvGlScroller *) parent())->UpdateScrollbars();

    }
}


void PdvGlLiveWindow::start_streaming()

{
    Start(true);
}

void PdvGlLiveWindow::stop_streaming()

{
    StopLive();
}


void PdvGlLiveWindow::capture_single()

{

    if (!IsLive())
    {
        if (m_pCamera->GetDepth() > 8 && m_pCamera->GetDepth() < 16 && !TransformActive)
            SetTransformActive();

        m_pCamera->CaptureImage(pSingle);

        pDataImage = pSingle;
        if ((pRawImage = m_pCamera->GetLastRawImage()) != NULL)
            UpdateDisplayImage();

    }

}

void PdvGlLiveWindow::capture_single_and_draw()

{
    capture_single();
    draw();
    swap_buffers();
}


PdvGlLiveWindow::~PdvGlLiveWindow()
{
    DeleteCriticalSection(&m_lock);

    if (m_bOwnCamera)
        delete m_pCamera;
}

#ifdef _WIN32
UINT __stdcall
#else
void *
#endif
LiveThreadFunc(void *param)

{

    PdvGlLiveWindow *pObj = (PdvGlLiveWindow *) param;

    return (THREAD_RETURN) pObj->DoLiveThreadFunc();

}


bool PdvGlLiveWindow::StopLive()

{

    Lock();

#ifdef __APPLE__
    pthread_t pThread = m_pThread;
#else
    HANDLE pThread = m_pThread;
#endif

    StopPump();

    Unlock();

    if (pThread)
    {
        //    WaitForThread(pThread,INFINITE);
    }


    return true;
}

EdtImage * PdvGlLiveWindow::DoLiveWindow(bool display)
{

    pDataImage = GetNextBufferImage(FrameCount);

    // 

    if (pDataImage)
    {

        m_Time.SetBufferTime(FrameCount, pDataImage->GetTimeStamp());

        NotifyEventFuncs(pDataImage,FrameCount);

        FrameCount ++;

        if (display)
        {

            UpdateDisplayImage();

#if 0
            draw();
#endif

        }

    }

    return pDataImage;

}


void PdvGlLiveWindow::AddEventFunc(ImageEventFunc f, void *target)

{
    // Make sure not already in list

    EventFuncRec *pTest;

    pTest = m_AcqList.GetFirst();

    while (!m_AcqList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
            return;
        pTest = m_AcqList.GetNext();
    }

    m_AcqList.Append(new EventFuncRec(f,target));
}


void PdvGlLiveWindow::RemoveEventFunc(ImageEventFunc f, void *target)
{

    EventFuncRec *pTest;

    pTest = m_AcqList.GetFirst();

    while (!m_AcqList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
        {
            m_AcqList.DeleteCurrent();
            return;
        }
        pTest = m_AcqList.GetNext();
    }

    ASSERT(false);

}

void PdvGlLiveWindow::NotifyEventFuncs(EdtImage * pImage, 
        long nFrameNumber)
{
    EventFuncRec *pTest;

    pTest = m_AcqList.GetFirst();

    while (!m_AcqList.EndOfList())
    {
        if (pTest->pFunc)
            pTest->pFunc(pTest->target, pImage, NULL,  nFrameNumber);

        pTest = m_AcqList.GetNext();
    }

}

void PdvGlLiveWindow::AddFinishFunc(ImageEventFunc f, void *target)
{

    EventFuncRec *pTest;

    pTest = m_FinishList.GetFirst();

    while (!m_FinishList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
            return;
        pTest = m_FinishList.GetNext();
    }

    m_FinishList.Append(new EventFuncRec(f,target));
}

void PdvGlLiveWindow::RemoveFinishFunc(ImageEventFunc f, void *target)
{

    EventFuncRec *pTest;

    pTest = m_FinishList.GetFirst();

    while (!m_FinishList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
        {
            m_FinishList.DeleteCurrent();
            return;
        }
        pTest = m_FinishList.GetNext();
    }

    ASSERT(false);

}

void PdvGlLiveWindow::NotifyFinishFuncs()
{
    EventFuncRec *pTest;

    pTest = m_FinishList.GetFirst();

    while (!m_FinishList.EndOfList())
    {
        if (pTest->pFunc)
            pTest->pFunc(pTest->target, NULL, NULL, m_nChannelID);

        pTest = m_FinishList.GetNext();
    }

}

int PdvGlLiveWindow::GetFrameCount()
{
    return FrameCount;
}

void PdvGlLiveWindow::Abort()

{
    if (m_pThread)
    {
        thread_t start_thread = m_pThread;

        m_pThread = (thread_t) 0;

        SetActiveFlag(false);

        m_pCamera->Abort();

    }
}


bool PdvGlLiveWindow::Start(bool bUseThread)
{

    m_Time.Start();

    // Launch the thread
    if (bUseThread && m_pThread)
        return 0;

    SetActiveFlag(true);

    if (m_pCamera->GetDepth() > 8 && m_pCamera->GetDepth() < 16 && !TransformActive)
    {
        SetTransformActive();
    }

    StartPump();

    FrameCount = 0;
    m_nLastTimeStamp = 0;

    // Set Start Time
    m_dStartTime = edt_timestamp();

    if (bUseThread)
    {

        LaunchThread(m_pThread, LiveThreadFunc, this);

    }

    m_Time.StartTimer();

    return true;
}



bool PdvGlLiveWindow::AllFinished()
{
    return BuffersFinished();
}


    THREAD_RETURN
PdvGlLiveWindow::DoLiveThreadFunc()
{

    bool bStarted = false;

    while (GetActiveFlag())
    {
        // Debugging stuff


        if (m_bStartPending)
        {
            edt_msleep(1);
        }
        else
        {

            if (!bStarted)
            {
                edt_msleep(10);
                bStarted = true;
            }


            DoLiveWindow(true);

            if (AllFinished())
                SetActiveFlag(false);

        }
    }


    if (m_pCamera)
        m_pCamera->StopAcquiring();

    NotifyFinishFuncs();

    m_pThread = (thread_t) 0;


    return (THREAD_RETURN) 0;
}



PdvInput * PdvGlLiveWindow::GetCamera()
{
    return m_pCamera;
}


EdtImage * PdvGlLiveWindow::GetNextBufferImage(int nCount)

{
    static double last_time = 0.0;

    if (m_pCamera)
    {
        EdtImage *pImage;

        pImage = m_pCamera->GetNextBufferImage();

        m_nLastCount = m_pCamera->GetDoneCount();

        if (m_bStartPending)
        {
            m_bStartPending = false;


        }

        double timestamp = pImage->GetTimeStamp();

        SetBufferTime(m_nLastCount, timestamp);
        last_time = timestamp;

        //    pImage->SetSequenceNumber(m_nLastCount);



        m_pLastImageCaptured = pImage;


        return (GetActiveFlag())?pImage:NULL;

    }

    return NULL;
}

void PdvGlLiveWindow::StartPump()
{

    m_nLastCount = 0;

    if (m_pCamera)
    {
        m_pCamera->PrepareBuffers();
        m_pCamera->StartAcquire(m_nMaxCount);
    }

    m_bStartPending = false;
}

void PdvGlLiveWindow::StopPump()

{
    if (m_pCamera && m_pCamera->IsCapturingImages())
    {

        m_pCamera->InitiateStop();

    }
}



bool PdvGlLiveWindow::BuffersFinished()

{
    if (m_pCamera)
    {

        return !(m_bStartPending || m_pCamera->BuffersPending());

    }
    return true;
}


int PdvGlLiveWindow::GetTimeouts()

{
    if (m_pCamera)
        return m_pCamera->GetTimeouts();
    else
        return 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void PdvGlLiveWindow::SetNBuffers(const int nBuffers)

{

    if (m_pCamera)
        m_pCamera->SetNBufs(nBuffers);

}



void PdvGlLiveWindow::InitVariables()

{
    FrameCount = 0;
    m_dStartTime = 0;
    m_dEndTime = 0;

    m_nLocks = 0;
    m_bDoLocks = 1;

    m_pThread = (thread_t) 0;

    m_nFrameMod = 30;

    m_bActiveFlag = false;
    m_bTimedOut = false;


    m_nChannelID = 0;

    InitializeCriticalSection(&m_lock);


    m_bDrawLive = true;

    m_pLastImageCaptured = NULL;

    m_nAcquireSkip = 1;
    m_nDisplaySkip = 1;

    m_nMaxCount = 0;
    m_nLastCount = 0;

    m_bStartPending = false;

    m_pCamera = NULL;
    nbuffers = 4;
    capturing = false;
    pSingle = NULL;
    pDataImage = NULL;

    pDisplayImage = NULL;
    pLut = NULL;
    TransformActive = false;
    Mono2Color = false;


    total_skipped = 0;

    show_width = 1;
    show_height = 1;

    scaled_width = 1;
    scaled_height = 1;

    cursor_x = cursor_y = 0;
    image_cursor_x = image_cursor_y = 0;

    buffer = NULL;

    scalex = scaley = 1.0; 

    zoom_to_window = false;

    cursor_mode = NoOp;

    m_nLastTimeStamp = 0;

    last_display_type = 0;

}


EdtImage * PdvGlLiveWindow::GetLastImage()
{
    return m_pLastImageCaptured;
}


PdvGlWindow *newLiveWindow(int x, int y, int w, int h, const char *l)


{

    return (PdvGlWindow *) new PdvGlLiveWindow(x,y,w,h,l);
}
