//*****************************************************************************
//*****************************************************************************
//
// OpenALOrbit.cpp
//
//
// Description:
// This file implements the entry point for the application.
//
//
//
//            Copyright NVIDIA Corporation.  All rights reserved.
//
//     NOTICE TO USER:   The source code  is copyrighted under  U.S. and
//     international laws.  Users and possessors of this source code are
//     hereby granted a nonexclusive,  royalty-free copyright license to
//     use this code in individual and commercial software.
//
//     Any use of this source code must include,  in the user documenta-
//     tion and  internal comments to the code,  notices to the end user
//     as follows:
//
//     NVIDIA CORPORATION MAKES NO REPRESENTATION ABOUT THE  SUITABILITY
//     OF  THIS SOURCE  CODE  FOR ANY PURPOSE.  IT IS  PROVIDED  "AS IS"
//     WITHOUT EXPRESS OR IMPLIED WARRANTY OF  ANY KIND.  NVIDIA CORPOR-
//     ATION DISCLAIMS ALL WARRANTIES  WITH REGARD  TO THIS SOURCE CODE,
//     INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGE-
//     MENT,  AND FITNESS  FOR A PARTICULAR PURPOSE.   IN NO EVENT SHALL
//     NVIDIA CORPORATION  BE LIABLE FOR  ANY SPECIAL,  INDIRECT,  INCI-
//     DENTAL, OR CONSEQUENTIAL DAMAGES,  OR ANY DAMAGES  WHATSOEVER RE-
//     SULTING FROM LOSS OF USE,  DATA OR PROFITS,  WHETHER IN AN ACTION
//     OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT OF
//     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
//
//     U.S. Government  End  Users.   This source code  is a "commercial
//     item,"  as that  term is  defined at  48 C.F.R. 2.101 (OCT 1995),
//     consisting  of "commercial  computer  software"  and  "commercial
//     computer  software  documentation,"  as such  terms  are  used in
//     48 C.F.R. 12.212 (SEPT 1995)  and is provided to the U.S. Govern-
//     ment only as  a commercial end item.   Consistent with  48 C.F.R.
//     12.212 and  48 C.F.R. 227.7202-1 through  227.7202-4 (JUNE 1995),
//     all U.S. Government End Users  acquire the source code  with only
//     those rights set forth herein.
//
//                                                                    Rev A
//
//
//*****************************************************************************
//*****************************************************************************




#include "stdafx.h"
#include "resource.h"
#include <commctrl.h>
#include "SimpleOrbiter.h"
#include "../common/wavefile.h"


//*****************************************************************************
//*****************************************************************************
//
// Defines
//
//*****************************************************************************
//*****************************************************************************

#define NVIDIA_DEVICE_NAME      "NVIDIA® nForce(TM) Audio"



//*****************************************************************************
//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
//*****************************************************************************

ALdevice*  nvDevice  = 0;
ALcontext* nvContext = 0;
ALuint BufferNames[1];
ALuint SourceNames[1];



//*****************************************************************************
//*****************************************************************************
//
// Local Variables
//
//*****************************************************************************
//*****************************************************************************

static HINSTANCE AppInstance;
static COrbiter  Orbiter;
static CWaveFile WaveFile;
static HINSTANCE OpenALDll = 0;


//*****************************************************************************
//*****************************************************************************
//
// Forward Declarations
//
//*****************************************************************************
//*****************************************************************************

LRESULT CALLBACK DialogMsgHandler
(
    IN  HWND hDlg,
    IN  UINT message,
    IN  WPARAM wParam,
    IN  LPARAM lParam
);

bool InitializeDialog(HWND hDlg);
bool HandleExit();
void HandleSlider(HWND hDlg, LPARAM lParam);


//*****************************************************************************
//*****************************************************************************
//
// Entry Points
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// WinMain
//*****************************************************************************
//
int APIENTRY WinMain
(
    IN  HINSTANCE hInstance,
    IN  HINSTANCE hPrevInstance,
    IN  LPSTR     lpCmdLine,
    IN  int       nCmdShow
)
{
    AppInstance = hInstance;

    //
    // Init the common control dll.
    //
    InitCommonControls();

    HWND hDlg = CreateDialog(hInstance, (LPCTSTR)IDD_MAIN, NULL, (DLGPROC)DialogMsgHandler);
    if(!hDlg)
    {
        return FALSE;
    }

    HACCEL accelTable = LoadAccelerators(hInstance, (LPCTSTR)IDA_OPENALORBIT);

    //
    // Main message loop.
    //
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(!TranslateAccelerator(msg.hwnd, accelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}


//*****************************************************************************
// DialogMsgHandler
//*****************************************************************************
// Handles all windows messages.
//
LRESULT CALLBACK DialogMsgHandler
(
    IN  HWND hDlg,
    IN  UINT message,
    IN  WPARAM wParam,
    IN  LPARAM lParam
)
{
    int wmId, wmEvent;

    switch(message)
    {
        case WM_COMMAND:
             wmId    = LOWORD(wParam);
             wmEvent = HIWORD(wParam);
             switch(wmId)
             {
                 case IDOK:
                 case IDCANCEL:
                 case IDABORT:
                      DestroyWindow(hDlg);
                      break;

                 //
                 // Handle the buttons:
                 //
                 case IDC_PLAY:
                     alSourcePlay(SourceNames[0]);
                     break;

                 case IDC_STOP:
                     alSourceStop(SourceNames[0]);
                     break;
             }
             break;

        //
        // The next 4 messages are for the Orbiter only!
        //
        case WM_LBUTTONDOWN:
             Orbiter.SetMouseState(TRUE);
             return TRUE; // Message handled

        case WM_MOUSEMOVE:
             Orbiter.OnMouseMovement();
             return TRUE; // Message handled

        case WM_LBUTTONUP:
             Orbiter.SetMouseState(FALSE);
             return TRUE; // Message handled

        case WM_TIMER:
             Orbiter.OnMovementTimer();
             return TRUE;


        case WM_VSCROLL:
             HandleSlider(hDlg, lParam);
             break;


        case WM_DESTROY:
             HandleExit();
             PostQuitMessage(0);
             break;

        case WM_INITDIALOG:
             if(!InitializeDialog(hDlg))
             {
                 MessageBox(0, "Failed to initialize the application!", "Error", MB_OK);
                 HandleExit();
                 PostQuitMessage(0);
             }

             else
             {
                 ShowWindow(hDlg, TRUE);
             }
             break;

        default:
             return FALSE;
   }

   return 0;
}


//*****************************************************************************
// InitializeDialog
//*****************************************************************************
// Setup the main dialog controls.
//
bool InitializeDialog
(
    IN  HWND hDlg
)
{
    //
    // Load the icon
    //
    HICON icon = LoadIcon(AppInstance, MAKEINTRESOURCE(IDI_NVIDIA));
    PostMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)icon);
    PostMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)icon);

    //
    // Set the range and positions of the sliders.
    // Remember the that top is 0 and the bottom is 1000.
    //
    HWND slider = GetDlgItem(hDlg, IDC_GAIN_CONTEXT);
    SendMessage(slider, TBM_SETTICFREQ,  (WPARAM)1000, (LPARAM)0);
    SendMessage(slider, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000);
    SendMessage(slider, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0);
    SendMessage(slider, TBM_SETPOS,      (WPARAM)TRUE, (LPARAM)0);

    slider = GetDlgItem(hDlg, IDC_GAIN);
    SendMessage(slider, TBM_SETTICFREQ,  (WPARAM)1000, (LPARAM)0);
    SendMessage(slider, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000);
    SendMessage(slider, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0);
    SendMessage(slider, TBM_SETPOS,      (WPARAM)TRUE, (LPARAM)0);

    slider = GetDlgItem(hDlg, IDC_GAIN_MIN);
    SendMessage(slider, TBM_SETTICFREQ,  (WPARAM)1000, (LPARAM)0);
    SendMessage(slider, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000);
    SendMessage(slider, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0);
    SendMessage(slider, TBM_SETPOS,      (WPARAM)TRUE, (LPARAM)1000);

    slider = GetDlgItem(hDlg, IDC_GAIN_MAX);
    SendMessage(slider, TBM_SETTICFREQ,  (WPARAM)1000, (LPARAM)0);
    SendMessage(slider, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)1000);
    SendMessage(slider, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0);
    SendMessage(slider, TBM_SETPOS,      (WPARAM)TRUE, (LPARAM)0);

    slider = GetDlgItem(hDlg, IDC_PITCH);
    SendMessage(slider, TBM_SETTICFREQ,  (WPARAM)2000, (LPARAM)0);
    SendMessage(slider, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)2000);
    SendMessage(slider, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0);
    SendMessage(slider, TBM_SETPOS,      (WPARAM)TRUE, (LPARAM)1000);

    //
    // Initialize the wave file.
    //
    if(WaveFile.LoadFile("WaveFiles\\human_footsteps.wav") ||
       WaveFile.GetBitsPerSample() != 16 || WaveFile.GetChannels() != 1)
    {
        return FALSE;
    }

    //
    // Initialize OpenAL.
    //
    TCHAR path[MAX_PATH];
    GetSystemDirectory(path, MAX_PATH);
    lstrcat(path, TEXT("\\OpenAL32.dll"));
    OpenALDll = LoadLibrary(path);
    if(!OpenALDll)
    {
        return FALSE;
    }

    if(!InitOpenAL(OpenALDll))
    {
        return FALSE;
    }

    nvDevice = alcOpenDevice((ALubyte*)NVIDIA_DEVICE_NAME);
    if(!nvDevice)
    {
        return FALSE;
    }

    nvContext = alcCreateContext(nvDevice, 0);
    if(!nvContext)
    {
        return FALSE;
    }

    if(!alcMakeContextCurrent(nvContext))
    {
        return FALSE;
    }

    if(!InitOpenALStandardEnums(nvDevice))
    {
        return FALSE;
    }

    //
    // Setup the source.
    //
    alGenBuffers(1, BufferNames);
    alGenSources(1, SourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        return FALSE;
    }

    alBufferData(BufferNames[0], AL_FORMAT_MONO16, WaveFile.GetDataBuffer(), WaveFile.GetDataSize(), WaveFile.GetSampleRate());
    if(alGetError() != AL_NO_ERROR)
    {
        return FALSE;
    }

    alSourcei(SourceNames[0], AL_BUFFER, BufferNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        return FALSE;
    }

    alSourcei(SourceNames[0], AL_LOOPING, 1);
    if(alGetError() != AL_NO_ERROR)
    {
        return FALSE;
    }

    //
    // Initialize the orbiter.
    //
    Orbiter.Initialize(hDlg);
    return TRUE;
}


//*****************************************************************************
// HandleExit
//*****************************************************************************
// This is the last function executed before exiting.
//
bool HandleExit()
{
    if(nvContext)
    {
        alcMakeContextCurrent(nvContext);

        alDeleteSources(1, SourceNames);
        alDeleteBuffers(1, BufferNames);

        alcMakeContextCurrent(0);
        alcDestroyContext(nvContext);
    }

    if(nvDevice)
    {
        alcCloseDevice(nvDevice);
    }

    if(OpenALDll)
    {
        FreeLibrary(OpenALDll);
    }

    return TRUE;
}


//*****************************************************************************
// HandleSlider
//*****************************************************************************
//
void HandleSlider
(
    IN  HWND   hDlg,
    IN  LPARAM lParam
)
{
    //
    // Get slider position.  We always set them here because I know the NVIDIA OpenAL
    // implementation doesn't needlessly write the values to the hardware if they didn't
    // change.  Other implementations may not which wastes cycles...
    //
    HWND slider = GetDlgItem(hDlg, IDC_GAIN_CONTEXT);
    ALfloat data = ((ALfloat)SendMessage(slider, TBM_GETPOS, (WPARAM)0, (LPARAM)0) - 1000.0f) / -1000.0f;
    alListenerf(AL_GAIN, data);

    slider = GetDlgItem(hDlg, IDC_GAIN);
    data = ((ALfloat)SendMessage(slider, TBM_GETPOS, (WPARAM)0, (LPARAM)0) - 1000.0f) / -1000.0f;
    alSourcef(SourceNames[0], AL_GAIN, data);

    slider = GetDlgItem(hDlg, IDC_GAIN_MIN);
    data = ((ALfloat)SendMessage(slider, TBM_GETPOS, (WPARAM)0, (LPARAM)0) - 1000.0f) / -1000.0f;
    alSourcef(SourceNames[0], AL_MIN_GAIN, data);

    slider = GetDlgItem(hDlg, IDC_GAIN_MAX);
    data = ((ALfloat)SendMessage(slider, TBM_GETPOS, (WPARAM)0, (LPARAM)0) - 1000.0f) / -1000.0f;
    alSourcef(SourceNames[0], AL_MAX_GAIN, data);

    slider = GetDlgItem(hDlg, IDC_PITCH);
    data = ((ALfloat)SendMessage(slider, TBM_GETPOS, (WPARAM)0, (LPARAM)0)) / 1000.0f;
    alSourcef(SourceNames[0], AL_PITCH, data);
    return;
}

