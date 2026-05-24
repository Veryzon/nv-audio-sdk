//*****************************************************************************
//*****************************************************************************
//
// OpenALWaveDlg.cpp
//
//
// Description:
// This file implements the dialog class.
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
// Locals
//
//*****************************************************************************
//*****************************************************************************

void SetupMonoAdpcm(ALuint mask)
{
    ALbufferFormatNV format;
    format.SampleRate = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetSampleRate();
    format.ValidBitsPerSample = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetBitsPerSample();
    format.BitsPerSample = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetBitsPerSample();
    format.NumberOfChannels = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetChannels();
    format.FormatEnum = AL_FORMAT_NVADPCM_NV;
    format.ChannelMask = mask;

    //
    // Initialize the ADPCM buffer on page aligned boundaries.  Better to
    // do it now that at run time eh?
    //
    ALuint offset;
    ALuint size;
    ALuint bytesPerPage = 4096 / (WaveFile[WAVE_TYPE_NVADPCM_MONO].GetChannels() * 36) *
        (WaveFile[WAVE_TYPE_NVADPCM_MONO].GetChannels() * 36);
    ALuint i;
    for(i = 0; i < AlignedBufferCount[WAVE_TYPE_NVADPCM_MONO]; i++)
    {
        offset = i * bytesPerPage;
        size = offset + bytesPerPage > WaveFile[WAVE_TYPE_NVADPCM_MONO].GetDataSize() ?
            WaveFile[WAVE_TYPE_NVADPCM_MONO].GetDataSize() - offset : bytesPerPage;

        alBufferDataNV(AlignedBuffers[WAVE_TYPE_NVADPCM_MONO][i], &format,
                       (ALubyte*)WaveFile[WAVE_TYPE_NVADPCM_MONO].GetDataBuffer() + offset,
                       size);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox(0, "Failed to setup the buffer!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }
    }
}



//*****************************************************************************
//*****************************************************************************
//
// COpenALWaveDlg
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// COpenALWaveDlg
//*****************************************************************************
//
COpenALWaveDlg::COpenALWaveDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COpenALWaveDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(COpenALWaveDlg)
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_NVIDIA);

    int i;
    for(i = 0; i < WAVE_TYPE_COUNT; i++)
    {
        m_Play[i] = FALSE;
    }
}


//*****************************************************************************
// DoDataExchange
//*****************************************************************************
//
void COpenALWaveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COpenALWaveDlg)
    DDX_Control(pDX, IDC_ALIGN, m_AlignAdpcm);
    DDX_Control(pDX, IDC_STREAM, m_Stream);
    DDX_Control(pDX, IDC_STEREO_BUTTON2, m_NvAdpcmStereo);
    DDX_Control(pDX, IDC_MONO_BUTTON2, m_NvAdpcmMono);
    DDX_Control(pDX, IDC_FRONTRIGHT2, m_NvAdpcmFrontRight);
    DDX_Control(pDX, IDC_FRONTLEFT2, m_NvAdpcmFrontLeft);
    DDX_Control(pDX, IDC_FRONTCENTER2, m_NvAdpcmFrontCenter);
    DDX_Control(pDX, IDC_BACKRIGHT2, m_NvAdpcmBackRight);
    DDX_Control(pDX, IDC_BACKLEFT2, m_NvAdpcmBackLeft);
    DDX_Control(pDX, IDC_STEREO_BUTTON, m_Stereo);
    DDX_Control(pDX, IDC_QUAD_BUTTON, m_Quad);
    DDX_Control(pDX, IDC_MONO_BUTTON, m_Mono);
    DDX_Control(pDX, IDC_5POINT1_BUTTON, m_5point1);
    DDX_Control(pDX, IDC_FRONTRIGHT, m_FrontRight);
    DDX_Control(pDX, IDC_FRONTLEFT, m_FrontLeft);
    DDX_Control(pDX, IDC_FRONTCENTER, m_FrontCenter);
    DDX_Control(pDX, IDC_BACKRIGHT, m_BackRight);
    DDX_Control(pDX, IDC_BACKLEFT, m_BackLeft);
    //}}AFX_DATA_MAP
}


//*****************************************************************************
// Message Map
//*****************************************************************************
//
BEGIN_MESSAGE_MAP(COpenALWaveDlg, CDialog)
    //{{AFX_MSG_MAP(COpenALWaveDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_MONO_BUTTON, OnMonoButton)
    ON_BN_CLICKED(IDC_STEREO_BUTTON, OnStereoButton)
    ON_BN_CLICKED(IDC_QUAD_BUTTON, OnQuadButton)
    ON_BN_CLICKED(IDC_5POINT1_BUTTON, On5point1Button)
    ON_BN_CLICKED(IDC_MONO_BUTTON2, OnMonoButton2)
    ON_BN_CLICKED(IDC_STEREO_BUTTON2, OnStereoButton2)
    ON_BN_CLICKED(IDC_STREAM, OnStream)
    ON_BN_CLICKED(IDC_ALIGN, OnAlign)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()



//*****************************************************************************
//*****************************************************************************
//
// Message Handlers
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// OnInitDialog
//*****************************************************************************
//
BOOL COpenALWaveDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE  unless you set the focus to a control
}


//*****************************************************************************
// OnPaint
//*****************************************************************************
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
//
void COpenALWaveDlg::OnPaint()
{
    if(IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }

    else
    {
        CDialog::OnPaint();
    }
}


//*****************************************************************************
// OnQueryDragIcon
//*****************************************************************************
//
HCURSOR COpenALWaveDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}



//*****************************************************************************
// OnMonoButton
//*****************************************************************************
//
void COpenALWaveDlg::OnMonoButton()
{
    if(!m_Play[WAVE_TYPE_MONO])
    {
        ALbufferFormatNV format;
        format.FormatEnum = AL_FORMAT_PCM_NV;
        format.SampleRate = WaveFile[WAVE_TYPE_MONO].GetSampleRate();
        format.ValidBitsPerSample = WaveFile[WAVE_TYPE_MONO].GetBitsPerSample();
        format.BitsPerSample = WaveFile[WAVE_TYPE_MONO].GetBitsPerSample();
        format.NumberOfChannels = WaveFile[WAVE_TYPE_MONO].GetChannels();

        format.ChannelMask = AL_CHANNELMASK_NONE_NV;
        if(m_FrontLeft.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_FRONT_LEFT_NV;
        }

        if(m_FrontRight.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_FRONT_RIGHT_NV;
        }

        if(m_FrontCenter.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_FRONT_CENTER_NV;
        }

        //
        // No LFE this time.
        //

        if(m_BackLeft.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_BACK_LEFT_NV;
        }

        if(m_BackRight.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_BACK_RIGHT_NV;
        }

        //
        // If nothing was selected, then we have a 3D buffer at the origin.
        //
        if(!format.ChannelMask)
        {
            MessageBox("No channels selected creates a 3D buffer at the origin.", "Information", MB_OK);
        }

        alBufferDataNV(BufferNames[WAVE_TYPE_MONO], &format, WaveFile[WAVE_TYPE_MONO].GetDataBuffer(), WaveFile[WAVE_TYPE_MONO].GetDataSize());
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to setup the buffer!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        if(!m_Stream.GetCheck())
        {
            alSourcei(SourceNames[WAVE_TYPE_MONO], AL_BUFFER, BufferNames[WAVE_TYPE_MONO]);
        }

        else
        {
            alSourceQueueBuffers(SourceNames[WAVE_TYPE_MONO], 1, &BufferNames[WAVE_TYPE_MONO]);
        }

        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to assign the buffer to a source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        alSourcePlay(SourceNames[WAVE_TYPE_MONO]);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to play the source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        m_Mono.SetWindowText("Stop");
        m_Play[WAVE_TYPE_MONO] = TRUE;
    
        m_FrontRight.EnableWindow(FALSE);
        m_FrontLeft.EnableWindow(FALSE);
        m_FrontCenter.EnableWindow(FALSE);
        m_BackRight.EnableWindow(FALSE);
        m_BackLeft.EnableWindow(FALSE);
    }

    else
    {
        alSourceStop(SourceNames[WAVE_TYPE_MONO]);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to stop the source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        if(!m_Stream.GetCheck())
        {
            alSourcei(SourceNames[WAVE_TYPE_MONO], AL_BUFFER, 0);
        }

        else
        {
            alSourceUnqueueBuffers(SourceNames[WAVE_TYPE_MONO], 1, &BufferNames[WAVE_TYPE_MONO]);
        }

        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to unqueue the buffer!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        m_Mono.SetWindowText("Play");
        m_Play[WAVE_TYPE_MONO] = FALSE;

        m_FrontRight.EnableWindow(TRUE);
        m_FrontLeft.EnableWindow(TRUE);
        m_FrontCenter.EnableWindow(TRUE);
        m_BackRight.EnableWindow(TRUE);
        m_BackLeft.EnableWindow(TRUE);
    }
}


//*****************************************************************************
// OnButton
//*****************************************************************************
//
void COpenALWaveDlg::OnButton(CButton* Button, int Name)
{
    if(!m_Play[Name])
    {
        if(Name == WAVE_TYPE_NVADPCM_STEREO && m_Stream.GetCheck() && m_AlignAdpcm.GetCheck())
        {
            alSourceQueueBuffers(SourceNames[Name],
                                 AlignedBufferCount[Name],
                                 AlignedBuffers[Name]);
        }

        else if(!m_Stream.GetCheck())
        {
            alSourcei(SourceNames[Name], AL_BUFFER, BufferNames[Name]);
        }

        else
        {
            alSourceQueueBuffers(SourceNames[Name], 1, &BufferNames[Name]);
        }

        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to assign the buffer to a source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        alSourcePlay(SourceNames[Name]);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to play the source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        Button->SetWindowText("Stop");
        m_Play[Name] = TRUE;
    }

    else
    {
        alSourceStop(SourceNames[Name]);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to stop the source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        if(!m_Stream.GetCheck())
        {
            alSourcei(SourceNames[Name], AL_BUFFER, 0);
        }

        else
        {
            if(Name == WAVE_TYPE_NVADPCM_STEREO && m_AlignAdpcm.GetCheck())
            {
                alSourceUnqueueBuffers(SourceNames[Name],
                                       AlignedBufferCount[Name],
                                       AlignedBuffers[Name]);
            }

            else
            {
                alSourceUnqueueBuffers(SourceNames[Name], 1, &BufferNames[Name]);
            }
        }

        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to assign the buffer to a source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        Button->SetWindowText("Play");
        m_Play[Name] = FALSE;
    }
}


//*****************************************************************************
// OnStereoButton
//*****************************************************************************
//
void COpenALWaveDlg::OnStereoButton()
{
    OnButton(&m_Stereo, WAVE_TYPE_STEREO);
}


//*****************************************************************************
// OnQuadButton
//*****************************************************************************
//
void COpenALWaveDlg::OnQuadButton()
{
    OnButton(&m_Quad, WAVE_TYPE_QUAD);
}


//*****************************************************************************
// On5point1Button
//*****************************************************************************
//
void COpenALWaveDlg::On5point1Button()
{
    OnButton(&m_5point1, WAVE_TYPE_5POINT1);
}


//*****************************************************************************
// OnMonoButton2
//*****************************************************************************
//
void COpenALWaveDlg::OnMonoButton2()
{
    if(!m_Play[WAVE_TYPE_NVADPCM_MONO])
    {
        ALbufferFormatNV format;
        format.FormatEnum = AL_FORMAT_NVADPCM_NV;
        format.SampleRate = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetSampleRate();
        format.ValidBitsPerSample = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetBitsPerSample();
        format.BitsPerSample = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetBitsPerSample();
        format.NumberOfChannels = WaveFile[WAVE_TYPE_NVADPCM_MONO].GetChannels();

        format.ChannelMask = AL_CHANNELMASK_NONE_NV;
        if(m_NvAdpcmFrontLeft.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_FRONT_LEFT_NV;
        }

        if(m_NvAdpcmFrontRight.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_FRONT_RIGHT_NV;
        }

        if(m_NvAdpcmFrontCenter.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_FRONT_CENTER_NV;
        }

        //
        // No LFE this time.
        //

        if(m_NvAdpcmBackLeft.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_BACK_LEFT_NV;
        }

        if(m_NvAdpcmBackRight.GetCheck())
        {
            format.ChannelMask |= AL_CHANNELMASK_BACK_RIGHT_NV;
        }

        //
        // If nothing was selected, then we have a 3D buffer at the origin.
        //
        if(!format.ChannelMask)
        {
            MessageBox("No channels selected creates a 3D buffer at the origin.", "Information", MB_OK);
        }

        alBufferDataNV(BufferNames[WAVE_TYPE_NVADPCM_MONO], &format,
            WaveFile[WAVE_TYPE_NVADPCM_MONO].GetDataBuffer(), WaveFile[WAVE_TYPE_NVADPCM_MONO].GetDataSize());
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to setup the buffer!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        SetupMonoAdpcm(format.ChannelMask);


        //
        // Check if we are in buffer or streaming mode.
        //
        if(m_Stream.GetCheck() && m_AlignAdpcm.GetCheck())
        {
            alSourceQueueBuffers(SourceNames[WAVE_TYPE_NVADPCM_MONO],
                                 AlignedBufferCount[WAVE_TYPE_NVADPCM_MONO],
                                 AlignedBuffers[WAVE_TYPE_NVADPCM_MONO]);
        }

        else if(!m_Stream.GetCheck())
        {
            alSourcei(SourceNames[WAVE_TYPE_NVADPCM_MONO], AL_BUFFER, BufferNames[WAVE_TYPE_NVADPCM_MONO]);
        }

        else
        {
            alSourceQueueBuffers(SourceNames[WAVE_TYPE_NVADPCM_MONO], 1, &BufferNames[WAVE_TYPE_NVADPCM_MONO]);
        }

        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to assign the buffer to a source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        alSourcePlay(SourceNames[WAVE_TYPE_NVADPCM_MONO]);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to play the source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        m_NvAdpcmMono.SetWindowText("Stop");
        m_Play[WAVE_TYPE_NVADPCM_MONO] = TRUE;

        m_NvAdpcmFrontRight.EnableWindow(FALSE);
        m_NvAdpcmFrontLeft.EnableWindow(FALSE);
        m_NvAdpcmFrontCenter.EnableWindow(FALSE);
        m_NvAdpcmBackRight.EnableWindow(FALSE);
        m_NvAdpcmBackLeft.EnableWindow(FALSE);
    }

    else
    {
        alSourceStop(SourceNames[WAVE_TYPE_NVADPCM_MONO]);
        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to stop the source!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        if(!m_Stream.GetCheck())
        {
            alSourcei(SourceNames[WAVE_TYPE_NVADPCM_MONO], AL_BUFFER, 0);
        }

        else
        {
            if(m_AlignAdpcm.GetCheck())
            {
                alSourceUnqueueBuffers(SourceNames[WAVE_TYPE_NVADPCM_MONO], AlignedBufferCount[WAVE_TYPE_NVADPCM_MONO],
                                       AlignedBuffers[WAVE_TYPE_NVADPCM_MONO]);
            }

            else
            {
                alSourceUnqueueBuffers(SourceNames[WAVE_TYPE_NVADPCM_MONO], 1, &BufferNames[WAVE_TYPE_NVADPCM_MONO]);
            }
        }

        if(alGetError() != AL_NO_ERROR)
        {
            MessageBox("Failed to unqueue the buffer!", "Error", MB_OK);
            PostQuitMessage(0);
            return;
        }

        m_NvAdpcmMono.SetWindowText("Play");
        m_Play[WAVE_TYPE_NVADPCM_MONO] = FALSE;

        m_NvAdpcmFrontRight.EnableWindow(TRUE);
        m_NvAdpcmFrontLeft.EnableWindow(TRUE);
        m_NvAdpcmFrontCenter.EnableWindow(TRUE);
        m_NvAdpcmBackRight.EnableWindow(TRUE);
        m_NvAdpcmBackLeft.EnableWindow(TRUE);
    }
}


//*****************************************************************************
// OnStereoButton2
//*****************************************************************************
//
void COpenALWaveDlg::OnStereoButton2()
{
    OnButton(&m_NvAdpcmStereo, WAVE_TYPE_NVADPCM_STEREO);
}


//*****************************************************************************
// OnStream
//*****************************************************************************
//
void COpenALWaveDlg::OnStream()
{
    int i;
    for(i = 0; i < WAVE_TYPE_COUNT; i++)
    {
        if(m_Play[i])
        {
            alSourceStop(SourceNames[i]);
            if(alGetError() != AL_NO_ERROR)
            {
                MessageBox("Failed to stop the source!", "Error", MB_OK);
                PostQuitMessage(0);
                return;
            }

            //
            // Do the opposite of the check state.
            //
            if(m_Stream.GetCheck())
            {
                alSourcei(SourceNames[i], AL_BUFFER, 0);
            }

            else
            {
                if((i == WAVE_TYPE_NVADPCM_MONO || i == WAVE_TYPE_NVADPCM_STEREO) && m_AlignAdpcm.GetCheck())
                {
                    alSourceUnqueueBuffers(SourceNames[i], AlignedBufferCount[i], AlignedBuffers[i]);
                }

                else
                {
                    alSourceUnqueueBuffers(SourceNames[i], 1, &BufferNames[i]);
                }
            }

            if(alGetError() != AL_NO_ERROR)
            {
                MessageBox("Failed to unqueue the buffer!", "Error", MB_OK);
                PostQuitMessage(0);
                return;
            }

            m_Play[i] = FALSE;
        }
    }

    if(m_Stream.GetCheck())
    {
        m_AlignAdpcm.EnableWindow(TRUE);
    }

    else
    {
        m_AlignAdpcm.SetCheck(FALSE);
        m_AlignAdpcm.EnableWindow(FALSE);
    }


    m_Mono.SetWindowText("Play");
    m_FrontRight.EnableWindow(TRUE);
    m_FrontLeft.EnableWindow(TRUE);
    m_FrontCenter.EnableWindow(TRUE);
    m_BackRight.EnableWindow(TRUE);
    m_BackLeft.EnableWindow(TRUE);

    m_Stereo.SetWindowText("Play");
    m_Quad.SetWindowText("Play");
    m_5point1.SetWindowText("Play");

    m_NvAdpcmMono.SetWindowText("Play");
    m_NvAdpcmFrontRight.EnableWindow(TRUE);
    m_NvAdpcmFrontLeft.EnableWindow(TRUE);
    m_NvAdpcmFrontCenter.EnableWindow(TRUE);
    m_NvAdpcmBackRight.EnableWindow(TRUE);
    m_NvAdpcmBackLeft.EnableWindow(TRUE);

    m_NvAdpcmStereo.SetWindowText("Play");
}


//*****************************************************************************
// OnStream
//*****************************************************************************
//
void COpenALWaveDlg::OnAlign()
{
    int i;
    for(i = 0; i < WAVE_TYPE_COUNT; i++)
    {
        if(m_Play[i])
        {
            alSourceStop(SourceNames[i]);
            if(alGetError() != AL_NO_ERROR)
            {
                MessageBox("Failed to stop the source!", "Error", MB_OK);
                PostQuitMessage(0);
                return;
            }

            if((i == WAVE_TYPE_NVADPCM_MONO || i == WAVE_TYPE_NVADPCM_STEREO) && !m_AlignAdpcm.GetCheck())
            {
                alSourceUnqueueBuffers(SourceNames[i], AlignedBufferCount[i], AlignedBuffers[i]);
            }

            else
            {
                alSourceUnqueueBuffers(SourceNames[i], 1, &BufferNames[i]);
            }

            m_Play[i] = FALSE;
        }
    }

    m_Mono.SetWindowText("Play");
    m_FrontRight.EnableWindow(TRUE);
    m_FrontLeft.EnableWindow(TRUE);
    m_FrontCenter.EnableWindow(TRUE);
    m_BackRight.EnableWindow(TRUE);
    m_BackLeft.EnableWindow(TRUE);

    m_Stereo.SetWindowText("Play");
    m_Quad.SetWindowText("Play");
    m_5point1.SetWindowText("Play");

    m_NvAdpcmMono.SetWindowText("Play");
    m_NvAdpcmFrontRight.EnableWindow(TRUE);
    m_NvAdpcmFrontLeft.EnableWindow(TRUE);
    m_NvAdpcmFrontCenter.EnableWindow(TRUE);
    m_NvAdpcmBackRight.EnableWindow(TRUE);
    m_NvAdpcmBackLeft.EnableWindow(TRUE);

    m_NvAdpcmStereo.SetWindowText("Play");
}
