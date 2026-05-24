//*****************************************************************************
//*****************************************************************************
//
// OpenALEax.cpp
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
#include "OpenALEax.h"
#include "initguid.h"
#include "MyPropertySheet.h"
#include "../common/wavefile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



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

//
// The one and only COpenALEaxApp object.
//
COpenALEaxApp theApp;

ALdevice*  OpenALDevice  = 0;
ALcontext* OpenALContext = 0;
ALuint BufferNames[1];
ALuint SourceNames[1];
EAXGet EaxGet = 0;
EAXSet EaxSet = 0;



//*****************************************************************************
//*****************************************************************************
//
// Locals
//
//*****************************************************************************
//*****************************************************************************

static CWaveFile WaveFile;
static HINSTANCE OpenALDll = 0;



//*****************************************************************************
//*****************************************************************************
//
// Message Map
//
//*****************************************************************************
//*****************************************************************************

BEGIN_MESSAGE_MAP(COpenALEaxApp, CWinApp)
    //{{AFX_MSG_MAP(COpenALEaxApp)
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()



//*****************************************************************************
//*****************************************************************************
//
// COpenALEaxApp
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// COpenALEaxApp
//*****************************************************************************
//
COpenALEaxApp::COpenALEaxApp()
{
}



//*****************************************************************************
// ~COpenALEaxApp
//*****************************************************************************
//
COpenALEaxApp::~COpenALEaxApp()
{
    if(OpenALContext)
    {
        alcMakeContextCurrent(OpenALContext);

        alDeleteSources(1, SourceNames);
        alDeleteBuffers(1, BufferNames);

        alcMakeContextCurrent(0);
        alcDestroyContext(OpenALContext);
    }

    if(OpenALDevice)
    {
        alcCloseDevice(OpenALDevice);
    }

    if(OpenALDll)
    {
        FreeLibrary(OpenALDll);
    }
}



//*****************************************************************************
// InitInstance
//*****************************************************************************
//
BOOL COpenALEaxApp::InitInstance()
{
    //
    // Initialize the wave file.
    //
    if(WaveFile.LoadFile("WaveFiles\\human_footsteps.wav") ||
       WaveFile.GetBitsPerSample() != 16 || WaveFile.GetChannels() != 1)
    {
        MessageBox(0, "Failed to load the sample file!", "Error", MB_OK);
        PostQuitMessage(0);
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
        MessageBox(0, "Failed to load OpenAL32.dll!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    if(!InitOpenAL(OpenALDll))
    {
        MessageBox(0, "Failed to initialize the OpenAL functions!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    OpenALDevice = alcOpenDevice((ALubyte*)NVIDIA_DEVICE_NAME);
    if(!OpenALDevice)
    {
        MessageBox(0, "Failed to open the device!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    OpenALContext = alcCreateContext(OpenALDevice, 0);
    if(!OpenALContext)
    {
        MessageBox(0, "Failed to create the context!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    if(!alcMakeContextCurrent(OpenALContext))
    {
        MessageBox(0, "Failed to set the context!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    if(!InitOpenALStandardEnums(OpenALDevice))
    {
        MessageBox(0, "Failed to get the standard OpenAL enumerations!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    //
    // Setup the source.
    //
    alGenBuffers(1, BufferNames);
    alGenSources(1, SourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        MessageBox(0, "Failed to create the source!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    alBufferData(BufferNames[0], AL_FORMAT_MONO16, WaveFile.GetDataBuffer(), WaveFile.GetDataSize(), WaveFile.GetSampleRate());
    if(alGetError() != AL_NO_ERROR)
    {
        MessageBox(0, "Failed to setup the buffer!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    alSourcei(SourceNames[0], AL_BUFFER, BufferNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        MessageBox(0, "Failed to assign the buffer to a source!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    alSourcei(SourceNames[0], AL_LOOPING, 1);
    if(alGetError() != AL_NO_ERROR)
    {
        MessageBox(0, "Failed to set looping mode on the source!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }


    EaxGet = (EAXGet)alGetProcAddress((ALubyte*)"EAXGet");
    EaxSet = (EAXSet)alGetProcAddress((ALubyte*)"EAXSet");
    if(!EaxGet || !EaxSet)
    {
        MessageBox(0, "Failed to set get the addresses of the EAXGet and/or EAXSet !", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    //
    // Standard initialization.
    //
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif

        CMyPropertySheet propSheet;
        propSheet.DoModal();

    //
    // Since the dialog has been closed, return FALSE so that we exit the
    // application, rather than start the application's message pump.
    //
    return FALSE;
}
