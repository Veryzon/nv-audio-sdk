//*****************************************************************************
//*****************************************************************************
//
// I3dl2Properties.cpp
//
//
// Description:
// This file implements the I3DL2 property classes.
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
#include "I3dl2Properties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPropertyPageI3dl2Listener, CPropertyPage)
IMPLEMENT_DYNCREATE(CPropertyPageI3dl2Buffer, CPropertyPage)


//*****************************************************************************
//*****************************************************************************
//
// CPropertyPageI3dl2Listener
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// CPropertyPageI3dl2Listener
//*****************************************************************************
//
CPropertyPageI3dl2Listener::CPropertyPageI3dl2Listener() :
    CPropertyPage(CPropertyPageI3dl2Listener::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageI3dl2Listener)
    //}}AFX_DATA_INIT
}


//*****************************************************************************
// ~CPropertyPageI3dl2Listener
//*****************************************************************************
//
CPropertyPageI3dl2Listener::~CPropertyPageI3dl2Listener()
{
}


//*****************************************************************************
// DoDataExchange
//*****************************************************************************
//
void CPropertyPageI3dl2Listener::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageI3dl2Listener)
    DDX_Control(pDX, IDC_I3DL2LIST_ROOM, m_Room);
    DDX_Control(pDX, IDC_I3DL2LIST_ROOM_VALUE, m_RoomText);
    DDX_Control(pDX, IDC_I3DL2LIST_ROOMHF, m_RoomHF);
    DDX_Control(pDX, IDC_I3DL2LIST_ROOMHF_VALUE, m_RoomHFText);
    DDX_Control(pDX, IDC_I3DL2LIST_ROOMROLLOFF, m_Rolloff);
    DDX_Control(pDX, IDC_I3DL2LIST_ROOMROLLOFF_VALUE, m_RolloffText);
    DDX_Control(pDX, IDC_I3DL2LIST_DECAYTIME, m_DecayTime);
    DDX_Control(pDX, IDC_I3DL2LIST_DECAYTIME_VALUE, m_DecayTimeText);
    DDX_Control(pDX, IDC_I3DL2LIST_DECAYHF, m_DecayHF);
    DDX_Control(pDX, IDC_I3DL2LIST_DECAYHF_VALUE, m_DecayHFText);
    DDX_Control(pDX, IDC_I3DL2LIST_REFLECTIONS, m_Reflections);
    DDX_Control(pDX, IDC_I3DL2LIST_REFLECTIONS_VALUE, m_ReflectionsText);
    DDX_Control(pDX, IDC_I3DL2LIST_REFLECTIONSDELAY, m_ReflectionsDelay);
    DDX_Control(pDX, IDC_I3DL2LIST_REFLECTIONSDELAY_VALUE, m_ReflectionsDelayText);
    DDX_Control(pDX, IDC_I3DL2LIST_REVERB, m_Reverb);
    DDX_Control(pDX, IDC_I3DL2LIST_REVERB_VALUE, m_ReverbText);
    DDX_Control(pDX, IDC_I3DL2LIST_REVERBDELAY, m_ReverbDelay);
    DDX_Control(pDX, IDC_I3DL2LIST_REVERBDELAY_VALUE, m_ReverbDelayText);
    DDX_Control(pDX, IDC_I3DL2LIST_DIFFUSION, m_Diffusion);
    DDX_Control(pDX, IDC_I3DL2LIST_DIFFUSION_VALUE, m_DiffusionText);
    DDX_Control(pDX, IDC_I3DL2LIST_DENSITY, m_Density);
    DDX_Control(pDX, IDC_I3DL2LIST_DENSITY_VALUE, m_DensityText);
    DDX_Control(pDX, IDC_I3DL2LIST_HFREFERENCE, m_HFReference);
    DDX_Control(pDX, IDC_I3DL2LIST_HFREFERENCE_VALUE, m_HFReferenceText);
    //}}AFX_DATA_MAP
}


//*****************************************************************************
// Message Map
//*****************************************************************************
//
BEGIN_MESSAGE_MAP(CPropertyPageI3dl2Listener, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageI3dl2Listener)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//*****************************************************************************
//*****************************************************************************
//
// Message Handlers
//
//*****************************************************************************
//*****************************************************************************

BOOL CPropertyPageI3dl2Listener::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    //
    // Get the default environment settings.
    //
    I3DL2_LISTENERPROPERTIES prop;
    I3dl2Get(&DSPROPSETID_I3DL2_ListenerProperties,
           DSPROPERTY_I3DL2LISTENER_ALL,
           0, &prop, sizeof(I3DL2_LISTENERPROPERTIES));


    // DSPROPERTY_I3DL2LISTENER_ROOM
    m_Room.SetTicFreq(1);
    m_Room.SetRange(-10000, 0, TRUE);
    m_Room.SetPos(int(prop.lRoom));

    char  buf[256];
    sprintf(buf, "%d", prop.lRoom);
    m_RoomText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_ROOMHF
    m_RoomHF.SetTicFreq(1);
    m_RoomHF.SetRange(-10000, 0, TRUE);
    m_RoomHF.SetPos(int(prop.lRoomHF));

    sprintf(buf, "%d", prop.lRoomHF);
    m_RoomHFText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_ROOMROLLOFFFACTOR
    m_Rolloff.SetTicFreq(1);
    m_Rolloff.SetRange(0, 1000, TRUE);
    m_Rolloff.SetPos(int(prop.flRoomRolloffFactor * 100));

    sprintf(buf, "%.2f", prop.flRoomRolloffFactor);
    m_RolloffText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_DECAYTIME
    m_DecayTime.SetTicFreq(1);
    m_DecayTime.SetRange(10, 2000, TRUE);
    m_DecayTime.SetPos(int(prop.flDecayTime * 100));

    sprintf(buf, "%.2f", prop.flDecayTime);
    m_DecayTimeText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_DECAYHFRATIO
    m_DecayHF.SetTicFreq(1);
    m_DecayHF.SetRange(100, 2000, TRUE);
    m_DecayHF.SetPos(int(prop.flDecayHFRatio * 1000));

    sprintf(buf, "%.2f", prop.flDecayHFRatio);
    m_DecayHFText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_REFLECTIONS
    m_Reflections.SetTicFreq(1);
    m_Reflections.SetRange(-10000, 1000, TRUE);
    m_Reflections.SetPos(int(prop.lReflections));

    sprintf(buf, "%d", prop.lReflections);
    m_ReflectionsText.SetWindowText(buf);

    // DSPROPERTY_I3DL2LISTENER_REFLECTIONSDELAY
    m_ReflectionsDelay.SetTicFreq(1);
    m_ReflectionsDelay.SetRange(0, 300, TRUE);
    m_ReflectionsDelay.SetPos(int(prop.flReflectionsDelay * 1000));

    sprintf(buf, "%.3f", prop.flReflectionsDelay);
    m_ReflectionsDelayText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_REVERB
    m_Reverb.SetTicFreq(1);
    m_Reverb.SetRange(-10000, 2000, TRUE);
    m_Reverb.SetPos(int(prop.lReverb));

    sprintf(buf, "%d", prop.lReverb);
    m_ReverbText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_REVERBDELAY
    m_ReverbDelay.SetTicFreq(1);
    m_ReverbDelay.SetRange(0, 100, TRUE);
    m_ReverbDelay.SetPos(int(prop.flReverbDelay * 1000));

    sprintf(buf, "%.3f", prop.flReverbDelay);
    m_ReverbDelayText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_DIFFUSION
    m_Diffusion.SetTicFreq(1);
    m_Diffusion.SetRange(0, 10000, TRUE);
    m_Diffusion.SetPos(int(prop.flDiffusion * 100));

    sprintf(buf, "%.2f", prop.flDiffusion);
    m_DiffusionText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_DENSITY
    m_Density.SetTicFreq(1);
    m_Density.SetRange(0, 10000, TRUE);
    m_Density.SetPos(int(prop.flDensity * 100));

    sprintf(buf, "%.2f", prop.flDensity);
    m_DensityText.SetWindowText(buf);


    // DSPROPERTY_I3DL2LISTENER_HFREFERENCE
    m_HFReference.SetTicFreq(1);
    m_HFReference.SetRange(2000, 2000000, TRUE);
    m_HFReference.SetPos(int(prop.flHFReference * 100));

    sprintf(buf, "%.2f", prop.flHFReference);
    m_HFReferenceText.SetWindowText(buf);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


//*****************************************************************************
// OnHScroll
//*****************************************************************************
//
void CPropertyPageI3dl2Listener::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    char  buf[256];
    int   size;
    void* data;
    float f;
    long  l;
    int   property;

    if((CWnd*)pScrollBar == (CWnd*)&m_Room)
    {
        property = DSPROPERTY_I3DL2LISTENER_ROOM;
        size = sizeof(long);
        data = &l;
        l = (long)m_Room.GetPos();
        sprintf(buf, "%d", l);
        m_RoomText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_RoomHF)
    {
        property = DSPROPERTY_I3DL2LISTENER_ROOMHF;
        size = sizeof(long);
        data = &l;
        l = (long)m_RoomHF.GetPos();
        sprintf(buf, "%d", l);
        m_RoomHFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Rolloff)
    {
        property = DSPROPERTY_I3DL2LISTENER_ROOMROLLOFFFACTOR;
        size = sizeof(float);
        data = &f;
        f = (float)m_Rolloff.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_RolloffText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_DecayTime)
    {
        property = DSPROPERTY_I3DL2LISTENER_DECAYTIME;
        size = sizeof(float);
        data = &f;
        f = (float)m_DecayTime.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_DecayTimeText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_DecayHF)
    {
        property = DSPROPERTY_I3DL2LISTENER_DECAYHFRATIO;
        size = sizeof(float);
        data = &f;
        f = (float)m_DecayHF.GetPos() / 1000;
        sprintf(buf, "%.2f", f);
        m_DecayHFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Reflections)
    {
        property = DSPROPERTY_I3DL2LISTENER_REFLECTIONS;
        size = sizeof(long);
        data = &l;
        l = (long)m_Reflections.GetPos();
        sprintf(buf, "%d", l);
        m_ReflectionsText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_ReflectionsDelay)
    {
        property = DSPROPERTY_I3DL2LISTENER_REFLECTIONSDELAY;
        size = sizeof(float);
        data = &f;
        f = (float)m_ReflectionsDelay.GetPos() / 1000;
        sprintf(buf, "%.2f", f);
        m_ReflectionsDelayText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Reverb)
    {
        property = DSPROPERTY_I3DL2LISTENER_REVERB;
        size = sizeof(long);
        data = &l;
        l = (long)m_Reverb.GetPos();
        sprintf(buf, "%d", l);
        m_ReverbText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_ReverbDelay)
    {
        property = DSPROPERTY_I3DL2LISTENER_REVERBDELAY;
        size = sizeof(float);
        data = &f;
        f = (float)m_ReverbDelay.GetPos() / 1000;
        sprintf(buf, "%.2f", f);
        m_ReverbDelayText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Diffusion)
    {
        property = DSPROPERTY_I3DL2LISTENER_DIFFUSION;
        size = sizeof(float);
        data = &f;
        f = (float)m_Diffusion.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_DiffusionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Density)
    {
        property = DSPROPERTY_I3DL2LISTENER_DENSITY;
        size = sizeof(float);
        data = &f;
        f = (float)m_Density.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_DensityText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_HFReference)
    {
        property = DSPROPERTY_I3DL2LISTENER_HFREFERENCE;
        size = sizeof(float);
        data = &f;
        f = (float)m_HFReference.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_HFReferenceText.SetWindowText(buf);
    }


    I3dl2Set(&DSPROPSETID_I3DL2_ListenerProperties,
           property, 0, data, size);


    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}



//*****************************************************************************
//*****************************************************************************
//
// CPropertyPageI3dl2Buffer
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// CPropertyPageI3dl2Buffer
//*****************************************************************************
//
CPropertyPageI3dl2Buffer::CPropertyPageI3dl2Buffer() :
    CPropertyPage(CPropertyPageI3dl2Buffer::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageI3dl2Buffer)
    //}}AFX_DATA_INIT
}


//*****************************************************************************
// ~CPropertyPageI3dl2Buffer
//*****************************************************************************
//
CPropertyPageI3dl2Buffer::~CPropertyPageI3dl2Buffer()
{
}


//*****************************************************************************
// DoDataExchange
//*****************************************************************************
//
void CPropertyPageI3dl2Buffer::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageI3dl2Buffer)
    DDX_Control(pDX, IDC_I3DL2BUF_DIRECT, m_Direct);
    DDX_Control(pDX, IDC_I3DL2BUF_DIRECT_VALUE, m_DirectText);
    DDX_Control(pDX, IDC_I3DL2BUF_DIRECTHF, m_DirectHF);
    DDX_Control(pDX, IDC_I3DL2BUF_DIRECTHF_VALUE, m_DirectHFText);
    DDX_Control(pDX, IDC_I3DL2BUF_ROOM, m_Room);
    DDX_Control(pDX, IDC_I3DL2BUF_ROOM_VALUE, m_RoomText);
    DDX_Control(pDX, IDC_I3DL2BUF_ROOMHF, m_RoomHF);
    DDX_Control(pDX, IDC_I3DL2BUF_ROOMHF_VALUE, m_RoomHFText);
    DDX_Control(pDX, IDC_I3DL2BUF_ROOMROLLOFF, m_Rolloff);
    DDX_Control(pDX, IDC_I3DL2BUF_ROOMROLLOFF_VALUE, m_RolloffText);
    DDX_Control(pDX, IDC_I3DL2BUF_OBSTRUCTION, m_Obstruction);
    DDX_Control(pDX, IDC_I3DL2BUF_OBSTRUCTION_VALUE, m_ObstructionText);
    DDX_Control(pDX, IDC_I3DL2BUF_OBSTRUCTIONLF, m_ObstructionLF);
    DDX_Control(pDX, IDC_I3DL2BUF_OBSTRUCTIONLF_VALUE, m_ObstructionLFText);
    DDX_Control(pDX, IDC_I3DL2BUF_OCCLUSION, m_Occlusion);
    DDX_Control(pDX, IDC_I3DL2BUF_OCCLUSION_VALUE, m_OcclusionText);
    DDX_Control(pDX, IDC_I3DL2BUF_OCCLUSIONLF, m_OcclusionLF);
    DDX_Control(pDX, IDC_I3DL2BUF_OCCLUSIONLF_VALUE, m_OcclusionLFText);
    //}}AFX_DATA_MAP
}


//*****************************************************************************
// Message Map
//*****************************************************************************
//
BEGIN_MESSAGE_MAP(CPropertyPageI3dl2Buffer, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageI3dl2Buffer)
    ON_BN_CLICKED(IDC_PLAY, OnPlay)
    ON_BN_CLICKED(IDC_STOP, OnStop)
    ON_WM_HSCROLL()
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
BOOL CPropertyPageI3dl2Buffer::OnInitDialog()
{
    CPropertyPage::OnInitDialog();


    //
    // Get the default environment settings.
    //
    I3DL2_BUFFERPROPERTIES prop;
    I3dl2Get(&DSPROPSETID_I3DL2_BufferProperties,
           DSPROPERTY_I3DL2BUFFER_ALL,
           SourceNames[0], &prop, sizeof(I3DL2_BUFFERPROPERTIES));

    // DSPROPERTY_I3DL2BUFFER_DIRECT
    m_Direct.SetTicFreq(1);
    m_Direct.SetRange(-10000, 1000, TRUE);
    m_Direct.SetPos(int(prop.lDirect));

    char  buf[256];
    sprintf(buf, "%d", prop.lDirect);
    m_DirectText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_DIRECTHF
    m_DirectHF.SetTicFreq(1);
    m_DirectHF.SetRange(-10000, 0, TRUE);
    m_DirectHF.SetPos(int(prop.lDirectHF));

    sprintf(buf, "%d", prop.lDirectHF);
    m_DirectHFText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_ROOM
    m_Room.SetTicFreq(1);
    m_Room.SetRange(-10000, 1000, TRUE);
    m_Room.SetPos(int(prop.lRoom));

    sprintf(buf, "%d", prop.lRoom);
    m_RoomText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_ROOMHF
    m_RoomHF.SetTicFreq(1);
    m_RoomHF.SetRange(-10000, 0, TRUE);
    m_RoomHF.SetPos(int(prop.lRoomHF));

    sprintf(buf, "%d", prop.lRoomHF);
    m_RoomHFText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_ROOMROLLOFFFACTOR
    m_Rolloff.SetTicFreq(1);
    m_Rolloff.SetRange(0, 1000, TRUE);
    m_Rolloff.SetPos(int(prop.flRoomRolloffFactor * 100));

    sprintf(buf, "%.2f", prop.flRoomRolloffFactor);
    m_RolloffText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_OBSTRUCTION
    m_Obstruction.SetTicFreq(1);
    m_Obstruction.SetRange(-10000, 0, TRUE);
    m_Obstruction.SetPos(int(prop.Obstruction.lHFLevel));

    sprintf(buf, "%d", prop.Obstruction.lHFLevel);
    m_ObstructionText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_OBSTRUCTIONLFRATIO
    m_ObstructionLF.SetTicFreq(1);
    m_ObstructionLF.SetRange(0, 100, TRUE);
    m_ObstructionLF.SetPos(int(prop.Obstruction.flLFRatio * 100));

    sprintf(buf, "%.2f", prop.Obstruction.flLFRatio);
    m_ObstructionLFText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_OCCLUSION
    m_Occlusion.SetTicFreq(1);
    m_Occlusion.SetRange(-10000, 0, TRUE);
    m_Occlusion.SetPos(int(prop.Occlusion.lHFLevel));

    sprintf(buf, "%d", prop.Occlusion.lHFLevel);
    m_OcclusionText.SetWindowText(buf);


    // DSPROPERTY_I3DL2BUFFER_OCCLUSIONLFRATIO
    m_OcclusionLF.SetTicFreq(1);
    m_OcclusionLF.SetRange(0, 100, TRUE);
    m_OcclusionLF.SetPos(int(prop.Occlusion.flLFRatio * 100));

    sprintf(buf, "%.2f", prop.Occlusion.flLFRatio);
    m_OcclusionLFText.SetWindowText(buf);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


//*****************************************************************************
// OnPlay
//*****************************************************************************
//
void CPropertyPageI3dl2Buffer::OnPlay()
{
    alSourcePlay(SourceNames[0]);
}


//*****************************************************************************
// OnStop
//*****************************************************************************
//
void CPropertyPageI3dl2Buffer::OnStop()
{
    alSourceStop(SourceNames[0]);
}


//*****************************************************************************
// OnHScroll
//*****************************************************************************
//
void CPropertyPageI3dl2Buffer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    char  buf[256];
    int   size;
    void* data;
    float f;
    long  l;
    int   property;

    if((CWnd*)pScrollBar == (CWnd*)&m_Direct)
    {
        property = DSPROPERTY_I3DL2BUFFER_DIRECT;
        size = sizeof(long);
        data = &l;
        l = (long)m_Direct.GetPos();
        sprintf(buf, "%d", l);
        m_DirectText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_DirectHF)
    {
        property = DSPROPERTY_I3DL2BUFFER_DIRECTHF;
        size = sizeof(long);
        data = &l;
        l = (long)m_DirectHF.GetPos();
        sprintf(buf, "%d", l);
        m_DirectHFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Room)
    {
        property = DSPROPERTY_I3DL2BUFFER_ROOM;
        size = sizeof(long);
        data = &l;
        l = (long)m_Room.GetPos();
        sprintf(buf, "%d", l);
        m_RoomText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_RoomHF)
    {
        property = DSPROPERTY_I3DL2BUFFER_ROOMHF;
        size = sizeof(long);
        data = &l;
        l = (long)m_RoomHF.GetPos();
        sprintf(buf, "%d", l);
        m_RoomHFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Rolloff)
    {
        property = DSPROPERTY_I3DL2BUFFER_ROOMROLLOFFFACTOR;
        size = sizeof(float);
        data = &f;
        f = (float)m_Rolloff.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_RolloffText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Obstruction)
    {
        property = DSPROPERTY_I3DL2BUFFER_OBSTRUCTION;
        size = sizeof(long);
        data = &l;
        l = (long)m_Obstruction.GetPos();
        sprintf(buf, "%d", l);
        m_ObstructionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_ObstructionLF)
    {
        property = DSPROPERTY_I3DL2BUFFER_OBSTRUCTIONLFRATIO;
        size = sizeof(float);
        data = &f;
        f = (float)m_ObstructionLF.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_ObstructionLFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Occlusion)
    {
        property = DSPROPERTY_I3DL2BUFFER_OCCLUSION;
        size = sizeof(long);
        data = &l;
        l = (long)m_Occlusion.GetPos();
        sprintf(buf, "%d", l);
        m_OcclusionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_OcclusionLF)
    {
        property = DSPROPERTY_I3DL2BUFFER_OCCLUSIONLFRATIO;
        size = sizeof(float);
        data = &f;
        f = (float)m_OcclusionLF.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_OcclusionLFText.SetWindowText(buf);
    }

    I3dl2Set(&DSPROPSETID_I3DL2_BufferProperties,
           property, SourceNames[0], data, size);


    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

