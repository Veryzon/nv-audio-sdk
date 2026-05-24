//*****************************************************************************
//*****************************************************************************
//
// OpenALWave.cpp
//
//
// Description:
// Defines the class behaviors for the application.
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
#include "OpenALWave.h"
#include "OpenALWaveDlg.h"

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
// The one and only COpenALWaveApp object.
//
COpenALWaveApp theApp;

ALdevice*  OpenALDevice  = 0;
ALcontext* OpenALContext = 0;
ALuint BufferNames[WAVE_TYPE_COUNT];
ALuint SourceNames[WAVE_TYPE_COUNT];
CWaveFile WaveFile[WAVE_TYPE_COUNT];

ALuint* AlignedBuffers[WAVE_TYPE_COUNT];
ALuint  AlignedBufferCount[WAVE_TYPE_COUNT] = { 0, 0, 0, 0, 0, 0 };



//*****************************************************************************
//*****************************************************************************
//
// Locals
//
//*****************************************************************************
//*****************************************************************************

static HINSTANCE OpenALDll = 0;



//*****************************************************************************
//*****************************************************************************
//
// Message Map
//
//*****************************************************************************
//*****************************************************************************

BEGIN_MESSAGE_MAP(COpenALWaveApp, CWinApp)
    //{{AFX_MSG_MAP(COpenALWaveApp)
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


//*****************************************************************************
//*****************************************************************************
//
// COpenALWaveApp
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// COpenALWaveApp
//*****************************************************************************
//
COpenALWaveApp::COpenALWaveApp()
{
}


//*****************************************************************************
// ~COpenALWaveApp
//*****************************************************************************
//
COpenALWaveApp::~COpenALWaveApp()
{
    if(OpenALContext)
    {
        alcMakeContextCurrent(OpenALContext);

        alDeleteSources(WAVE_TYPE_COUNT, SourceNames);
        alDeleteBuffers(AlignedBufferCount[WAVE_TYPE_NVADPCM_MONO],   AlignedBuffers[WAVE_TYPE_NVADPCM_MONO]);
        alDeleteBuffers(AlignedBufferCount[WAVE_TYPE_NVADPCM_STEREO], AlignedBuffers[WAVE_TYPE_NVADPCM_STEREO]);
        alDeleteBuffers(WAVE_TYPE_COUNT, BufferNames);

        free(AlignedBuffers[WAVE_TYPE_NVADPCM_MONO]);
        free(AlignedBuffers[WAVE_TYPE_NVADPCM_STEREO]);

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
BOOL COpenALWaveApp::InitInstance()
{
    //
    // Initialize the PCM wave files.
    //
    if(WaveFile[WAVE_TYPE_MONO].LoadFile("WaveFiles\\human_footsteps.wav") ||
       WaveFile[WAVE_TYPE_MONO].GetBitsPerSample() != 16 || WaveFile[WAVE_TYPE_MONO].GetChannels() != 1)
    {
        MessageBox(0, "Failed to load the mono sample file!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    if(WaveFile[WAVE_TYPE_STEREO].LoadFile("WaveFiles\\stereo.wav") || WaveFile[WAVE_TYPE_STEREO].GetChannels() != 2)
    {
        MessageBox(0, "Failed to load the stereo sample file!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    if(WaveFile[WAVE_TYPE_QUAD].LoadFile("WaveFiles\\quad.wav") || WaveFile[WAVE_TYPE_QUAD].GetChannels() != 4)
    {
        MessageBox(0, "Failed to load the quad sample file!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    if(WaveFile[WAVE_TYPE_5POINT1].LoadFile("WaveFiles\\5point1.wav") || WaveFile[WAVE_TYPE_5POINT1].GetChannels() != 6)
    {
        MessageBox(0, "Failed to load the 5.1 sample file!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }


    //
    // Initialize the NVADPCM wave files.
    //
    if(WaveFile[WAVE_TYPE_NVADPCM_MONO].LoadFile("WaveFiles\\mono_nvadpcm.wav") || WaveFile[WAVE_TYPE_NVADPCM_MONO].GetChannels() != 1)
    {
        MessageBox(0, "Failed to load the mono NVADPCM sample file!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    if(WaveFile[WAVE_TYPE_NVADPCM_STEREO].LoadFile("WaveFiles\\stereo_nvadpcm.wav") || WaveFile[WAVE_TYPE_NVADPCM_STEREO].GetChannels() != 2)
    {
        MessageBox(0, "Failed to load the stereo NVADPCM sample file!", "Error", MB_OK);
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

    if(!InitOpenALNvidiaExtension())
    {
        MessageBox(0, "Failed to initialize the NVIDIA OpenAL extensions!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    //
    // Setup the sources.
    //
    alGenBuffers(WAVE_TYPE_COUNT, BufferNames);
    alGenSources(WAVE_TYPE_COUNT, SourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        MessageBox(0, "Failed to create the source!", "Error", MB_OK);
        PostQuitMessage(0);
        return FALSE;
    }

    ALuint i;
    ALbufferFormatNV format;
    for(i = 0; i < WAVE_TYPE_COUNT; i++)
    {
        alSourcei(SourceNames[i], AL_LOOPING, 1);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox(0, "Failed to set looping mode on the source!", "Error", MB_OK);
            PostQuitMessage(0);
            return FALSE;
        }

        if(i != WAVE_TYPE_MONO && i != WAVE_TYPE_NVADPCM_MONO)
        {
            format.SampleRate = WaveFile[i].GetSampleRate();
            format.ValidBitsPerSample = WaveFile[i].GetBitsPerSample();
            format.BitsPerSample = WaveFile[i].GetBitsPerSample();
            format.NumberOfChannels = WaveFile[i].GetChannels();

            switch(i)
            {
                case WAVE_TYPE_STEREO:
                    format.FormatEnum = AL_FORMAT_PCM_NV;
                    format.ChannelMask = AL_CHANNELMASK_STEREO_NV;
                    break;

                case WAVE_TYPE_QUAD:
                    format.FormatEnum = AL_FORMAT_PCM_NV;
                    format.ChannelMask = AL_CHANNELMASK_QUAD_NV;
                    break;

                case WAVE_TYPE_5POINT1:
                    format.FormatEnum = AL_FORMAT_PCM_NV;
                    format.ChannelMask = AL_CHANNELMASK_5POINT1_NV;
                    break;

                case WAVE_TYPE_NVADPCM_STEREO:
                    format.FormatEnum = AL_FORMAT_NVADPCM_NV;
                    format.ChannelMask = AL_CHANNELMASK_STEREO_NV;
                    break;
            }

            alBufferDataNV(BufferNames[i], &format, WaveFile[i].GetDataBuffer(), WaveFile[i].GetDataSize());
            if(alGetError() != AL_NO_ERROR)
            {
                MessageBox(0, "Failed to setup the buffer!", "Error", MB_OK);
                PostQuitMessage(0);
                return FALSE;
            }
        }
    }

    //
    // Figure out how many buffer chunks we need for ADPCM.
    //
    ALuint bytesPerPage;
    for(i = WAVE_TYPE_NVADPCM_MONO; i < WAVE_TYPE_COUNT; i++)
    {
        bytesPerPage = 4096 / (WaveFile[i].GetChannels() * 36) * (WaveFile[i].GetChannels() * 36);
        AlignedBufferCount[i] = WaveFile[i].GetDataSize() / bytesPerPage + 1;
        AlignedBuffers[i] = (ALuint*)malloc(sizeof(ALuint) * AlignedBufferCount[i]);
        alGenBuffers(AlignedBufferCount[i], AlignedBuffers[i]);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox(0, "Failed to generate the NVADPCM buffers!", "Error", MB_OK);
            PostQuitMessage(0);
            return FALSE;
        }
    }

    //
    // Initialize the ADPCM buffer on page aligned boundaries.  This is the way ADPCM buffers should
    // really be streamed on NVIDIA hardware as the hardware cannot cross page boundaries when streaming
    // as it grabs the data in chunks which can't be done when (as the stream can starve).
    //
    ALuint offset;
    ALuint size;
    bytesPerPage = 4096 / (WaveFile[WAVE_TYPE_NVADPCM_STEREO].GetChannels() * 36) *
        (WaveFile[WAVE_TYPE_NVADPCM_STEREO].GetChannels() * 36);
    for(i = 0; i < AlignedBufferCount[WAVE_TYPE_NVADPCM_STEREO]; i++)
    {
        offset = i * bytesPerPage;
        size = offset + bytesPerPage > WaveFile[WAVE_TYPE_NVADPCM_STEREO].GetDataSize() ?
            WaveFile[WAVE_TYPE_NVADPCM_STEREO].GetDataSize() - offset : bytesPerPage;

        alBufferDataNV(AlignedBuffers[WAVE_TYPE_NVADPCM_STEREO][i], &format,
                       (ALubyte*)WaveFile[WAVE_TYPE_NVADPCM_STEREO].GetDataBuffer() + offset, size);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox(0, "Failed to setup the aligned stereo ADPCM buffer!", "Error", MB_OK);
            PostQuitMessage(0);
            return FALSE;
        }
    }

    //
    // Standard initialization.
    //
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif

    COpenALWaveDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    //
    // Since the dialog has been closed, return FALSE so that we exit the
    // application, rather than start the application's message pump.
    //
    return FALSE;
}
