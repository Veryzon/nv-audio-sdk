//*****************************************************************************
//*****************************************************************************
//
// EaxProperties.cpp
//
//
// Description:
// This file implements the EAX property classes.
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
#include "EaxProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPropertyPageEaxListener, CPropertyPage)
IMPLEMENT_DYNCREATE(CPropertyPageEaxBuffer, CPropertyPage)


//*****************************************************************************
//*****************************************************************************
//
// CPropertyPageEaxListener
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// CPropertyPageEaxListener
//*****************************************************************************
//
CPropertyPageEaxListener::CPropertyPageEaxListener() :
    CPropertyPage(CPropertyPageEaxListener::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageEaxListener)
    //}}AFX_DATA_INIT
}


//*****************************************************************************
// ~CPropertyPageEaxListener
//*****************************************************************************
//
CPropertyPageEaxListener::~CPropertyPageEaxListener()
{
}


//*****************************************************************************
// DoDataExchange
//*****************************************************************************
//
void CPropertyPageEaxListener::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageEaxListener)
    DDX_Control(pDX, IDC_EAXLIST_REFLECTIONSDELAY_VALUE, m_ReflectionsDelayText);
    DDX_Control(pDX, IDC_EAXLIST_REVERBDELAY_VALUE, m_ReverbDelayText);
    DDX_Control(pDX, IDC_EAXLIST_SCALE_REVERBDELAY, m_ScaleReverbDelay);
    DDX_Control(pDX, IDC_EAXLIST_CLIP_DECAY, m_ClipDelay);
    DDX_Control(pDX, IDC_EAXLIST_SCALE_REVERB, m_ScaleReverb);
    DDX_Control(pDX, IDC_EAXLIST_SCALE_REFLECTIONSDELAY, m_ScaleReflectionsDelay);
    DDX_Control(pDX, IDC_EAXLIST_SCALE_DECAYTIME, m_ScaleDelayTime);
    DDX_Control(pDX, IDC_EAXLIST_SCALE_REFLECTIONS, m_ScaleReflections);
    DDX_Control(pDX, IDC_EAXLIST_ROOMROLLOFF_VALUE, m_RolloffText);
    DDX_Control(pDX, IDC_EAXLIST_ROOMROLLOFF, m_Rolloff);
    DDX_Control(pDX, IDC_EAXLIST_ROOMHF_VALUE, m_RoomHFText);
    DDX_Control(pDX, IDC_EAXLIST_ROOMHF, m_RoomHF);
    DDX_Control(pDX, IDC_EAXLIST_ROOM_VALUE, m_RoomText);
    DDX_Control(pDX, IDC_EAXLIST_REVERBDELAY, m_ReverbDelay);
    DDX_Control(pDX, IDC_EAXLIST_REVERB_VALUE, m_ReverbText);
    DDX_Control(pDX, IDC_EAXLIST_REVERB, m_Reverb);
    DDX_Control(pDX, IDC_EAXLIST_REFLECTIONSDELAY, m_ReflectionsDelay);
    DDX_Control(pDX, IDC_EAXLIST_REFLECTIONS_VALUE, m_ReflectionsText);
    DDX_Control(pDX, IDC_EAXLIST_REFLECTIONS, m_Reflections);
    DDX_Control(pDX, IDC_EAXLIST_ENVIRONMENT_SIZE_VALUE, m_EnvironmentSizeText);
    DDX_Control(pDX, IDC_EAXLIST_ENVIRONMENT_SIZE, m_EnvironmentSize);
    DDX_Control(pDX, IDC_EAXLIST_ENVIRONMENT_PRESET, m_EnvironmentPreset);
    DDX_Control(pDX, IDC_EAXLIST_ENVIRONMENT_DIFFUSION_VALUE, m_DiffusionText);
    DDX_Control(pDX, IDC_EAXLIST_ENVIRONMENT_DIFFUSION, m_Diffusion);
    DDX_Control(pDX, IDC_EAXLIST_DECAYTIME_VALUE, m_DecayTimeText);
    DDX_Control(pDX, IDC_EAXLIST_DECAYTIME, m_DecayTime);
    DDX_Control(pDX, IDC_EAXLIST_DECAYHF_VALUE, m_DecayHFText);
    DDX_Control(pDX, IDC_EAXLIST_DECAYHF, m_DecayHF);
    DDX_Control(pDX, IDC_EAXLIST_AIRABSORPTION_VALUE, m_AirAbsorptionText);
    DDX_Control(pDX, IDC_EAXLIST_AIRABSORPTION, m_AirAbsorption);
    DDX_Control(pDX, IDC_EAXLIST_ROOM, m_Room);
    //}}AFX_DATA_MAP
}


//*****************************************************************************
// Message Map
//*****************************************************************************
//
BEGIN_MESSAGE_MAP(CPropertyPageEaxListener, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageEaxListener)
    ON_CBN_SELCHANGE(IDC_EAXLIST_ENVIRONMENT_PRESET, OnSelchangeEaxlistEnvironmentPreset)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_EAXLIST_SCALE_REFLECTIONS, OnEaxlistScaleReflections)
    ON_BN_CLICKED(IDC_EAXLIST_SCALE_REFLECTIONSDELAY, OnEaxlistScaleReflectionsdelay)
    ON_BN_CLICKED(IDC_EAXLIST_SCALE_REVERB, OnEaxlistScaleReverb)
    ON_BN_CLICKED(IDC_EAXLIST_SCALE_REVERBDELAY, OnEaxlistScaleReverbdelay)
    ON_BN_CLICKED(IDC_EAXLIST_SCALE_DECAYTIME, OnEaxlistScaleDecaytime)
    ON_BN_CLICKED(IDC_EAXLIST_CLIP_DECAY, OnEaxlistClipDecay)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//*****************************************************************************
//*****************************************************************************
//
// Message Handlers
//
//*****************************************************************************
//*****************************************************************************

BOOL CPropertyPageEaxListener::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    //
    // Get the default environment settings.
    //
    EAXLISTENERPROPERTIES prop;
    EaxGet(&DSPROPSETID_EAX20_ListenerProperties,
           DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
           0, &prop, sizeof(EAXLISTENERPROPERTIES));

    m_EnvironmentPreset.AddString("Generic");
    m_EnvironmentPreset.AddString("Padded Cell");
    m_EnvironmentPreset.AddString("Room");
    m_EnvironmentPreset.AddString("Bathroom");
    m_EnvironmentPreset.AddString("Livingroom");
    m_EnvironmentPreset.AddString("Stone Room");
    m_EnvironmentPreset.AddString("Auditorium");
    m_EnvironmentPreset.AddString("Concert Hall");
    m_EnvironmentPreset.AddString("Cave");
    m_EnvironmentPreset.AddString("Arena");
    m_EnvironmentPreset.AddString("Hangar");
    m_EnvironmentPreset.AddString("Carpeted Hallway");
    m_EnvironmentPreset.AddString("Hallway");
    m_EnvironmentPreset.AddString("Stone Corridor");
    m_EnvironmentPreset.AddString("Alley");
    m_EnvironmentPreset.AddString("Forest");
    m_EnvironmentPreset.AddString("City");
    m_EnvironmentPreset.AddString("Mountains");
    m_EnvironmentPreset.AddString("Quarry");
    m_EnvironmentPreset.AddString("Plain");
    m_EnvironmentPreset.AddString("Parking Lot");
    m_EnvironmentPreset.AddString("Sewer Pipe");
    m_EnvironmentPreset.AddString("Under Water");
    m_EnvironmentPreset.AddString("Drugged");
    m_EnvironmentPreset.AddString("Dizzy");
    m_EnvironmentPreset.AddString("Psychotic");

    // DSPROPERTY_EAXLISTENER_ENVIRONMENT
    m_EnvironmentPreset.SetCurSel(prop.dwEnvironment);


    // DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE
    m_EnvironmentSize.SetTicFreq(1);
    m_EnvironmentSize.SetRange(100, 10000, TRUE);
    m_EnvironmentSize.SetPos(int(prop.flEnvironmentSize * 100));

    char buf[256];
    sprintf(buf, "%.2f", prop.flEnvironmentSize);
    m_EnvironmentSizeText.SetWindowText(buf);


    // DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION
    m_Diffusion.SetTicFreq(1);
    m_Diffusion.SetRange(0, 100, TRUE);
    m_Diffusion.SetPos(int(prop.flEnvironmentDiffusion * 100));

    sprintf(buf, "%.2f", prop.flEnvironmentDiffusion);
    m_DiffusionText.SetWindowText(buf);


    // DSPROPERTY_EAXLISTENER_ROOM
    m_Room.SetTicFreq(1);
    m_Room.SetRange(-10000, 0, TRUE);
    m_Room.SetPos(int(prop.lRoom));

    sprintf(buf, "%d", prop.lRoom);
    m_RoomText.SetWindowText(buf);


    // DSPROPERTY_EAXLISTENER_ROOMHF
    m_RoomHF.SetTicFreq(1);
    m_RoomHF.SetRange(-10000, 0, TRUE);
    m_RoomHF.SetPos(int(prop.lRoomHF));

    sprintf(buf, "%d", prop.lRoomHF);
    m_RoomHFText.SetWindowText(buf);


    // DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR
    m_Rolloff.SetTicFreq(1);
    m_Rolloff.SetRange(0, 1000, TRUE);
    m_Rolloff.SetPos(int(prop.flRoomRolloffFactor * 100));

    sprintf(buf, "%.2f", prop.flRoomRolloffFactor);
    m_RolloffText.SetWindowText(buf);


    // DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF
    m_AirAbsorption.SetTicFreq(1);
    m_AirAbsorption.SetRange(-10000, 0, TRUE);
    m_AirAbsorption.SetPos(int(prop.flAirAbsorptionHF) * 100);

    sprintf(buf, "%.2f", prop.flAirAbsorptionHF);
    m_AirAbsorptionText.SetWindowText(buf);


    // DSPROPERTY_EAXLISTENER_REFLECTIONS
    m_Reflections.SetTicFreq(1);
    m_Reflections.SetRange(-10000, 1000, TRUE);
    m_Reflections.SetPos(int(prop.lReflections));

    sprintf(buf, "%d", prop.lReflections);
    m_ReflectionsText.SetWindowText(buf);

    m_ScaleReflections.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REFLECTIONSSCALE);


    // DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY
    m_ReflectionsDelay.SetTicFreq(1);
    m_ReflectionsDelay.SetRange(0, 300, TRUE);
    m_ReflectionsDelay.SetPos(int(prop.flReflectionsDelay * 1000));

    sprintf(buf, "%.3f", prop.flReflectionsDelay);
    m_ReflectionsDelayText.SetWindowText(buf);

    m_ScaleReflectionsDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE);


    // DSPROPERTY_EAXLISTENER_REVERB
    m_Reverb.SetTicFreq(1);
    m_Reverb.SetRange(-10000, 2000, TRUE);
    m_Reverb.SetPos(int(prop.lReverb));

    sprintf(buf, "%d", prop.lReverb);
    m_ReverbText.SetWindowText(buf);

    m_ScaleReverb.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REVERBSCALE);


    // DSPROPERTY_EAXLISTENER_REVERBDELAY
    m_ReverbDelay.SetTicFreq(1);
    m_ReverbDelay.SetRange(0, 100, TRUE);
    m_ReverbDelay.SetPos(int(prop.flReverbDelay * 1000));

    sprintf(buf, "%.3f", prop.flReverbDelay);
    m_ReverbDelayText.SetWindowText(buf);

    m_ScaleReverbDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REVERBDELAYSCALE);


    // DSPROPERTY_EAXLISTENER_DECAYTIME
    m_DecayTime.SetTicFreq(1);
    m_DecayTime.SetRange(10, 2000, TRUE);
    m_DecayTime.SetPos(int(prop.flDecayTime * 100));

    sprintf(buf, "%.2f", prop.flDecayTime);
    m_DecayTimeText.SetWindowText(buf);

    m_ScaleDelayTime.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_DECAYTIMESCALE);


    // DSPROPERTY_EAXLISTENER_DECAYHFRATIO
    m_DecayHF.SetTicFreq(1);
    m_DecayHF.SetRange(100, 2000, TRUE);
    m_DecayHF.SetPos(int(prop.flDecayHFRatio * 1000));

    sprintf(buf, "%.2f", prop.flDecayHFRatio);
    m_DecayHFText.SetWindowText(buf);

    m_ClipDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_DECAYHFLIMIT);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


//*****************************************************************************
// OnSelchangeEaxlistEnvironmentPreset
//*****************************************************************************
//
void CPropertyPageEaxListener::OnSelchangeEaxlistEnvironmentPreset()
{
    DWORD env = m_EnvironmentPreset.GetCurSel();
    if(env == CB_ERR)
    {
        return;
    }

    //
    // If the Custom environment is reset, ignore it.
    //
    if(env == EAX_ENVIRONMENT_COUNT)
    {
        return;
    }

    //
    // Remove custom if it is added.
    //
    m_EnvironmentPreset.DeleteString(EAX_ENVIRONMENT_COUNT);

    //
    // Set the environment.
    //
    EaxSet(&DSPROPSETID_EAX20_ListenerProperties,
           DSPROPERTY_EAXLISTENER_ENVIRONMENT,
           0, &env, sizeof(DWORD));

    EAXLISTENERPROPERTIES prop;
    EaxGet(&DSPROPSETID_EAX20_ListenerProperties,
           DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
           0, &prop, sizeof(EAXLISTENERPROPERTIES));

    //
    // Reset all of the other controls based on the environment settings.
    //
    char buf[256];
    m_EnvironmentSize.SetPos(int(prop.flEnvironmentSize * 100));
    sprintf(buf, "%.2f", prop.flEnvironmentSize);
    m_EnvironmentSizeText.SetWindowText(buf);

    m_Diffusion.SetPos(int(prop.flEnvironmentDiffusion * 100));
    sprintf(buf, "%.2f", prop.flEnvironmentDiffusion);
    m_DiffusionText.SetWindowText(buf);

    m_Room.SetPos(int(prop.lRoom));
    sprintf(buf, "%d", prop.lRoom);
    m_RoomText.SetWindowText(buf);

    m_RoomHF.SetPos(int(prop.lRoomHF));
    sprintf(buf, "%d", prop.lRoomHF);
    m_RoomHFText.SetWindowText(buf);

    m_Rolloff.SetPos(int(prop.flRoomRolloffFactor * 100));
    sprintf(buf, "%.2f", prop.flRoomRolloffFactor);
    m_RolloffText.SetWindowText(buf);

    m_AirAbsorption.SetPos(int(prop.flAirAbsorptionHF) * 100);
    sprintf(buf, "%.2f", prop.flAirAbsorptionHF);
    m_AirAbsorptionText.SetWindowText(buf);

    m_Reflections.SetPos(int(prop.lReflections));
    sprintf(buf, "%d", prop.lReflections);
    m_ReflectionsText.SetWindowText(buf);
    m_ScaleReflections.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REFLECTIONSSCALE);

    m_ReflectionsDelay.SetPos(int(prop.flReflectionsDelay * 1000));
    sprintf(buf, "%.3f", prop.flReflectionsDelay);
    m_ReflectionsDelayText.SetWindowText(buf);
    m_ScaleReflectionsDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE);

    m_Reverb.SetPos(int(prop.lReverb));
    sprintf(buf, "%d", prop.lReverb);
    m_ReverbText.SetWindowText(buf);
    m_ScaleReverb.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REVERBSCALE);

    m_ReverbDelay.SetPos(int(prop.flReverbDelay * 1000));
    sprintf(buf, "%.3f", prop.flReverbDelay);
    m_ReverbDelayText.SetWindowText(buf);
    m_ScaleReverbDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REVERBDELAYSCALE);

    m_DecayTime.SetPos(int(prop.flDecayTime * 100));
    sprintf(buf, "%.2f", prop.flDecayTime);
    m_DecayTimeText.SetWindowText(buf);
    m_ScaleDelayTime.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_DECAYTIMESCALE);

    m_DecayHF.SetPos(int(prop.flDecayHFRatio * 1000));
    sprintf(buf, "%.2f", prop.flDecayHFRatio);
    m_DecayHFText.SetWindowText(buf);
    m_ClipDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_DECAYHFLIMIT);
}


//*****************************************************************************
// OnHScroll
//*****************************************************************************
//
void CPropertyPageEaxListener::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    char  buf[256];
    int   size;
    void* data;
    float f;
    long  l;
    int   property;

    if((CWnd*)pScrollBar == (CWnd*)&m_EnvironmentSize)
    {
        property = DSPROPERTY_EAXLISTENER_ENVIRONMENTSIZE;
        size = sizeof(float);
        data = &f;
        f = (float)m_EnvironmentSize.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_EnvironmentSizeText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Diffusion)
    {
        property = DSPROPERTY_EAXLISTENER_ENVIRONMENTDIFFUSION;
        size = sizeof(float);
        data = &f;
        f = (float)m_Diffusion.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_DiffusionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Room)
    {
        property = DSPROPERTY_EAXLISTENER_ROOM;
        size = sizeof(long);
        data = &l;
        l = (long)m_Room.GetPos();
        sprintf(buf, "%d", l);
        m_RoomText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_RoomHF)
    {
        property = DSPROPERTY_EAXLISTENER_ROOMHF;
        size = sizeof(long);
        data = &l;
        l = (long)m_RoomHF.GetPos();
        sprintf(buf, "%d", l);
        m_RoomHFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Rolloff)
    {
        property = DSPROPERTY_EAXLISTENER_ROOMROLLOFFFACTOR;
        size = sizeof(float);
        data = &f;
        f = (float)m_Rolloff.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_RolloffText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_AirAbsorption)
    {
        property = DSPROPERTY_EAXLISTENER_AIRABSORPTIONHF;
        size = sizeof(float);
        data = &f;
        f = (float)m_AirAbsorption.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_AirAbsorptionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Reflections)
    {
        property = DSPROPERTY_EAXLISTENER_REFLECTIONS;
        size = sizeof(long);
        data = &l;
        l = (long)m_Reflections.GetPos();
        sprintf(buf, "%d", l);
        m_ReflectionsText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_ReflectionsDelay)
    {
        property = DSPROPERTY_EAXLISTENER_REFLECTIONSDELAY;
        size = sizeof(float);
        data = &f;
        f = (float)m_ReflectionsDelay.GetPos() / 1000;
        sprintf(buf, "%.2f", f);
        m_ReflectionsDelayText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Reverb)
    {
        property = DSPROPERTY_EAXLISTENER_REVERB;
        size = sizeof(long);
        data = &l;
        l = (long)m_Reverb.GetPos();
        sprintf(buf, "%d", l);
        m_ReverbText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_ReverbDelay)
    {
        property = DSPROPERTY_EAXLISTENER_REVERBDELAY;
        size = sizeof(float);
        data = &f;
        f = (float)m_ReverbDelay.GetPos() / 1000;
        sprintf(buf, "%.2f", f);
        m_ReverbDelayText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_DecayTime)
    {
        property = DSPROPERTY_EAXLISTENER_DECAYTIME;
        size = sizeof(float);
        data = &f;
        f = (float)m_DecayTime.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_DecayTimeText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_DecayHF)
    {
        property = DSPROPERTY_EAXLISTENER_DECAYHFRATIO;
        size = sizeof(float);
        data = &f;
        f = (float)m_DecayHF.GetPos() / 1000;
        sprintf(buf, "%.2f", f);
        m_DecayHFText.SetWindowText(buf);
    }


    EaxSet(&DSPROPSETID_EAX20_ListenerProperties,
           property, 0, data, size);

    if((CWnd*)pScrollBar == (CWnd*)&m_EnvironmentSize)
    {
        EAXLISTENERPROPERTIES prop;
        EaxGet(&DSPROPSETID_EAX20_ListenerProperties,
               DSPROPERTY_EAXLISTENER_ALLPARAMETERS,
               0, &prop, sizeof(EAXLISTENERPROPERTIES));

        //
        // Reset all of the other controls based on the new environment size settings.
        //
        m_Reflections.SetPos(int(prop.lReflections));
        sprintf(buf, "%d", prop.lReflections);
        m_ReflectionsText.SetWindowText(buf);
        m_ScaleReflections.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REFLECTIONSSCALE);

        m_ReflectionsDelay.SetPos(int(prop.flReflectionsDelay * 1000));
        sprintf(buf, "%.3f", prop.flReflectionsDelay);
        m_ReflectionsDelayText.SetWindowText(buf);
        m_ScaleReflectionsDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE);

        m_Reverb.SetPos(int(prop.lReverb));
        sprintf(buf, "%d", prop.lReverb);
        m_ReverbText.SetWindowText(buf);
        m_ScaleReverb.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REVERBSCALE);

        m_ReverbDelay.SetPos(int(prop.flReverbDelay * 1000));
        sprintf(buf, "%.3f", prop.flReverbDelay);
        m_ReverbDelayText.SetWindowText(buf);
        m_ScaleReverbDelay.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_REVERBDELAYSCALE);

        m_DecayTime.SetPos(int(prop.flDecayTime * 100));
        sprintf(buf, "%.2f", prop.flDecayTime);
        m_DecayTimeText.SetWindowText(buf);
        m_ScaleDelayTime.SetCheck(prop.dwFlags & EAXLISTENERFLAGS_DECAYTIMESCALE);
    }

    //
    // Modify the environment to read Custom if it doesn't already exist.
    //
    if(m_EnvironmentPreset.GetCurSel() != EAX_ENVIRONMENT_COUNT)
    {
        m_EnvironmentPreset.AddString("Custom");
        m_EnvironmentPreset.SetCurSel(EAX_ENVIRONMENT_COUNT);
    }


    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}


//*****************************************************************************
// EAXLISTENER_CHECK
//*****************************************************************************
// Generally I believe in local copies of variable settings but I know
// that NVIDIA settings are gotten to very, very quickly.  (No ring
// transitions, etc.  Just a case statement.)  You are on your
// own with other implementations.
//
#define EAXLISTENER_CHECK(control, flag)                                \
    DWORD flags;                                                        \
    EaxGet(&DSPROPSETID_EAX20_ListenerProperties,                       \
           DSPROPERTY_EAXLISTENER_FLAGS,                                \
           0, &flags, sizeof(DWORD));                                   \
                                                                        \
    if(control.GetCheck())                                              \
    {                                                                   \
        flags |= flag;                                                  \
    }                                                                   \
                                                                        \
    else                                                                \
    {                                                                   \
        flags &= ~flag;                                                 \
    }                                                                   \
                                                                        \
    EaxSet(&DSPROPSETID_EAX20_ListenerProperties,                       \
           DSPROPERTY_EAXLISTENER_FLAGS,                                \
           0, &flags, sizeof(DWORD));                                   \
                                                                        \
    if(m_EnvironmentPreset.GetCurSel() != EAX_ENVIRONMENT_COUNT)        \
    {                                                                   \
        m_EnvironmentPreset.AddString("Custom");                        \
        m_EnvironmentPreset.SetCurSel(EAX_ENVIRONMENT_COUNT);           \
    }


//*****************************************************************************
// OnEaxlistScaleReflections
//*****************************************************************************
//
void CPropertyPageEaxListener::OnEaxlistScaleReflections()
{
    EAXLISTENER_CHECK(m_ScaleReflections, EAXLISTENERFLAGS_REFLECTIONSSCALE);
}


//*****************************************************************************
// OnEaxlistScaleReflectionsdelay
//*****************************************************************************
//
void CPropertyPageEaxListener::OnEaxlistScaleReflectionsdelay()
{
    EAXLISTENER_CHECK(m_ScaleReflectionsDelay, EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE);
}


//*****************************************************************************
// OnEaxlistScaleReverb
//*****************************************************************************
//
void CPropertyPageEaxListener::OnEaxlistScaleReverb()
{
    EAXLISTENER_CHECK(m_ScaleReverb, EAXLISTENERFLAGS_REVERBSCALE);
}


//*****************************************************************************
// OnEaxlistScaleReverbdelay
//*****************************************************************************
//
void CPropertyPageEaxListener::OnEaxlistScaleReverbdelay()
{
    EAXLISTENER_CHECK(m_ScaleReverbDelay, EAXLISTENERFLAGS_REVERBDELAYSCALE);
}


//*****************************************************************************
// OnEaxlistScaleDecaytime
//*****************************************************************************
//
void CPropertyPageEaxListener::OnEaxlistScaleDecaytime()
{
    EAXLISTENER_CHECK(m_ScaleDelayTime, EAXLISTENERFLAGS_DECAYTIMESCALE);
}


//*****************************************************************************
// OnEaxlistClipDecay
//*****************************************************************************
//
void CPropertyPageEaxListener::OnEaxlistClipDecay()
{
    EAXLISTENER_CHECK(m_ClipDelay, EAXLISTENERFLAGS_DECAYHFLIMIT);
}



//*****************************************************************************
//*****************************************************************************
//
// CPropertyPageEaxBuffer
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// CPropertyPageEaxBuffer
//*****************************************************************************
//
CPropertyPageEaxBuffer::CPropertyPageEaxBuffer() :
    CPropertyPage(CPropertyPageEaxBuffer::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageEaxBuffer)
        //}}AFX_DATA_INIT
}


//*****************************************************************************
// ~CPropertyPageEaxBuffer
//*****************************************************************************
//
CPropertyPageEaxBuffer::~CPropertyPageEaxBuffer()
{
}


//*****************************************************************************
// DoDataExchange
//*****************************************************************************
//
void CPropertyPageEaxBuffer::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageEaxBuffer)
    DDX_Control(pDX, IDC_EAXBUF_AUTO_ROOMHF, m_AutoRoomHF);
    DDX_Control(pDX, IDC_EAXLBUF_OUTSIDEHF, m_OutsideHF);
    DDX_Control(pDX, IDC_EAXLBUF_OCCLUSIONROOM, m_OcclusionRoom);
    DDX_Control(pDX, IDC_EAXLBUF_OCCLUSIONLF, m_OcclusionLF);
    DDX_Control(pDX, IDC_EAXBUF_OCCLUSIONLF_VALUE, m_OcclusionLFText);
    DDX_Control(pDX, IDC_EAXLBUF_OBSTRUCTIONLF, m_ObstructionLF);
    DDX_Control(pDX, IDC_EAXLBUF_DIRECTHF, m_DirectHF);
    DDX_Control(pDX, IDC_EAXBUFF_AUTO_DIRECTHF, m_AutoDirectHF);
    DDX_Control(pDX, IDC_EAXBUF_ROOMROLLOFF_VALUE, m_RolloffText);
    DDX_Control(pDX, IDC_EAXBUF_ROOMROLLOFF, m_Rolloff);
    DDX_Control(pDX, IDC_EAXBUF_ROOMHF_VALUE, m_RoomHFText);
    DDX_Control(pDX, IDC_EAXBUF_ROOMHF, m_RoomHF);
    DDX_Control(pDX, IDC_EAXBUF_ROOM_VALUE, m_RoomText);
    DDX_Control(pDX, IDC_EAXBUF_ROOM, m_Room);
    DDX_Control(pDX, IDC_EAXBUF_OUTSIDEHF_VALUE, m_OutsideHFText);
    DDX_Control(pDX, IDC_EAXBUF_OCCLUSIONROOM_VALUE, m_OcclusionRoomText);
    DDX_Control(pDX, IDC_EAXBUF_OCCLUSION_VALUE, m_OcclusionText);
    DDX_Control(pDX, IDC_EAXBUF_OCCLUSION, m_Occlusion);
    DDX_Control(pDX, IDC_EAXBUF_OBSTRUCTIONLF_VALUE, m_ObstructionLFText);
    DDX_Control(pDX, IDC_EAXBUF_OBSTRUCTION_VALUE, m_ObstructionText);
    DDX_Control(pDX, IDC_EAXBUF_OBSTRUCTION, m_Obstruction);
    DDX_Control(pDX, IDC_EAXBUF_DIRECTHF_VALUE, m_DirectHFText);
    DDX_Control(pDX, IDC_EAXBUF_DIRECT_VALUE, m_DirectText);
    DDX_Control(pDX, IDC_EAXBUF_DIRECT, m_Direct);
    DDX_Control(pDX, IDC_EAXBUF_AUTO_ROOM, m_AutoRoom);
    DDX_Control(pDX, IDC_EAXBUF_AIRABSORPTION_VALUE, m_AirAbsorptionText);
    DDX_Control(pDX, IDC_EAXBUF_AIRABSORPTION, m_AirAbsorption);
    //}}AFX_DATA_MAP
}


//*****************************************************************************
// Message Map
//*****************************************************************************
//
BEGIN_MESSAGE_MAP(CPropertyPageEaxBuffer, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageEaxBuffer)
    ON_BN_CLICKED(IDC_PLAY, OnPlay)
    ON_BN_CLICKED(IDC_STOP, OnStop)
    ON_WM_HSCROLL()
        ON_BN_CLICKED(IDC_EAXBUF_AUTO_ROOM, OnEaxbufAutoRoom)
        ON_BN_CLICKED(IDC_EAXBUF_AUTO_ROOMHF, OnEaxbufAutoRoomhf)
        ON_BN_CLICKED(IDC_EAXBUFF_AUTO_DIRECTHF, OnEaxbuffAutoDirecthf)
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
BOOL CPropertyPageEaxBuffer::OnInitDialog()
{
    CPropertyPage::OnInitDialog();


    //
    // Get the default environment settings.
    //
    EAXBUFFERPROPERTIES prop;
    EaxGet(&DSPROPSETID_EAX20_BufferProperties,
           DSPROPERTY_EAXBUFFER_ALLPARAMETERS,
           SourceNames[0], &prop, sizeof(EAXBUFFERPROPERTIES));


    // DSPROPERTY_EAXBUFFER_ROOM
    m_Room.SetTicFreq(1);
    m_Room.SetRange(-10000, 1000, TRUE);
    m_Room.SetPos(int(prop.lRoom));

    char buf[256];
    sprintf(buf, "%d", prop.lRoom);
    m_RoomText.SetWindowText(buf);

    m_AutoRoom.SetCheck(prop.dwFlags & EAXBUFFERFLAGS_ROOMAUTO);


    // DSPROPERTY_EAXBUFFER_ROOMHF
    m_RoomHF.SetTicFreq(1);
    m_RoomHF.SetRange(-10000, 0, TRUE);
    m_RoomHF.SetPos(int(prop.lRoomHF));

    sprintf(buf, "%d", prop.lRoomHF);
    m_RoomHFText.SetWindowText(buf);

    m_AutoRoomHF.SetCheck(prop.dwFlags & EAXBUFFERFLAGS_ROOMHFAUTO);


    // DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR
    m_Rolloff.SetTicFreq(1);
    m_Rolloff.SetRange(0, 1000, TRUE);
    m_Rolloff.SetPos(int(prop.flRoomRolloffFactor * 100));

    sprintf(buf, "%.2f", prop.flRoomRolloffFactor);
    m_RolloffText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR
    m_AirAbsorption.SetTicFreq(1);
    m_AirAbsorption.SetRange(0, 1000, TRUE);
    m_AirAbsorption.SetPos(int(prop.flAirAbsorptionFactor * 100));

    sprintf(buf, "%.2f", prop.flAirAbsorptionFactor);
    m_AirAbsorptionText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_DIRECT
    m_Direct.SetTicFreq(1);
    m_Direct.SetRange(-10000, 1000, TRUE);
    m_Direct.SetPos(int(prop.lDirect));

    sprintf(buf, "%d", prop.lDirect);
    m_DirectText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_DIRECTHF
    m_DirectHF.SetTicFreq(1);
    m_DirectHF.SetRange(-10000, 0, TRUE);
    m_DirectHF.SetPos(int(prop.lDirectHF));

    sprintf(buf, "%d", prop.lDirectHF);
    m_DirectHFText.SetWindowText(buf);

    m_AutoDirectHF.SetCheck(prop.dwFlags & EAXBUFFERFLAGS_DIRECTHFAUTO);


    // DSPROPERTY_EAXBUFFER_OBSTRUCTION
    m_Obstruction.SetTicFreq(1);
    m_Obstruction.SetRange(-10000, 0, TRUE);
    m_Obstruction.SetPos(int(prop.lObstruction));

    sprintf(buf, "%d", prop.lObstruction);
    m_ObstructionText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO
    m_ObstructionLF.SetTicFreq(1);
    m_ObstructionLF.SetRange(0, 100, TRUE);
    m_ObstructionLF.SetPos(int(prop.flObstructionLFRatio * 100));

    sprintf(buf, "%.2f", prop.flObstructionLFRatio);
    m_ObstructionLFText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_OCCLUSION
    m_Occlusion.SetTicFreq(1);
    m_Occlusion.SetRange(-10000, 0, TRUE);
    m_Occlusion.SetPos(int(prop.lOcclusion));

    sprintf(buf, "%d", prop.lOcclusion);
    m_OcclusionText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO
    m_OcclusionLF.SetTicFreq(1);
    m_OcclusionLF.SetRange(0, 100, TRUE);
    m_OcclusionLF.SetPos(int(prop.flOcclusionLFRatio * 100));

    sprintf(buf, "%.2f", prop.flOcclusionLFRatio);
    m_OcclusionLFText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO
    m_OcclusionRoom.SetTicFreq(1);
    m_OcclusionRoom.SetRange(0, 1000, TRUE);
    m_OcclusionRoom.SetPos(int(prop.flOcclusionRoomRatio * 100));

    sprintf(buf, "%.2f", prop.flOcclusionRoomRatio);
    m_OcclusionRoomText.SetWindowText(buf);


    // DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF
    m_OutsideHF.SetTicFreq(1);
    m_OutsideHF.SetRange(-10000, 0, TRUE);
    m_OutsideHF.SetPos(int(prop.lOutsideVolumeHF));

    sprintf(buf, "%d", prop.lOutsideVolumeHF);
    m_OutsideHFText.SetWindowText(buf);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


//*****************************************************************************
// OnPlay
//*****************************************************************************
//
void CPropertyPageEaxBuffer::OnPlay()
{
    alSourcePlay(SourceNames[0]);
}


//*****************************************************************************
// OnStop
//*****************************************************************************
//
void CPropertyPageEaxBuffer::OnStop()
{
    alSourceStop(SourceNames[0]);
}


//*****************************************************************************
// OnHScroll
//*****************************************************************************
//
void CPropertyPageEaxBuffer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    char  buf[256];
    int   size;
    void* data;
    float f;
    long  l;
    int   property;

    if((CWnd*)pScrollBar == (CWnd*)&m_Room)
    {
        property = DSPROPERTY_EAXBUFFER_ROOM;
        size = sizeof(long);
        data = &l;
        l = (long)m_Room.GetPos();
        sprintf(buf, "%d", l);
        m_RoomText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_RoomHF)
    {
        property = DSPROPERTY_EAXBUFFER_ROOMHF;
        size = sizeof(long);
        data = &l;
        l = (long)m_RoomHF.GetPos();
        sprintf(buf, "%d", l);
        m_RoomHFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Rolloff)
    {
        property = DSPROPERTY_EAXBUFFER_ROOMROLLOFFFACTOR;
        size = sizeof(float);
        data = &f;
        f = (float)m_Rolloff.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_RolloffText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_AirAbsorption)
    {
        property = DSPROPERTY_EAXBUFFER_AIRABSORPTIONFACTOR;
        size = sizeof(float);
        data = &f;
        f = (float)m_AirAbsorption.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_AirAbsorptionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Direct)
    {
        property = DSPROPERTY_EAXBUFFER_DIRECT;
        size = sizeof(long);
        data = &l;
        l = (long)m_Direct.GetPos();
        sprintf(buf, "%d", l);
        m_DirectText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_DirectHF)
    {
        property = DSPROPERTY_EAXBUFFER_DIRECTHF;
        size = sizeof(long);
        data = &l;
        l = (long)m_DirectHF.GetPos();
        sprintf(buf, "%d", l);
        m_DirectHFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Obstruction)
    {
        property = DSPROPERTY_EAXBUFFER_OBSTRUCTION;
        size = sizeof(long);
        data = &l;
        l = (long)m_Obstruction.GetPos();
        sprintf(buf, "%d", l);
        m_ObstructionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_ObstructionLF)
    {
        property = DSPROPERTY_EAXBUFFER_OBSTRUCTIONLFRATIO;
        size = sizeof(float);
        data = &f;
        f = (float)m_ObstructionLF.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_ObstructionLFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_Occlusion)
    {
        property = DSPROPERTY_EAXBUFFER_OCCLUSION;
        size = sizeof(long);
        data = &l;
        l = (long)m_Occlusion.GetPos();
        sprintf(buf, "%d", l);
        m_OcclusionText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_OcclusionLF)
    {
        property = DSPROPERTY_EAXBUFFER_OCCLUSIONLFRATIO;
        size = sizeof(float);
        data = &f;
        f = (float)m_OcclusionLF.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_OcclusionLFText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_OcclusionRoom)
    {
        property = DSPROPERTY_EAXBUFFER_OCCLUSIONROOMRATIO;
        size = sizeof(float);
        data = &f;
        f = (float)m_OcclusionRoom.GetPos() / 100;
        sprintf(buf, "%.2f", f);
        m_OcclusionRoomText.SetWindowText(buf);
    }

    else if((CWnd*)pScrollBar == (CWnd*)&m_OutsideHF)
    {
        property = DSPROPERTY_EAXBUFFER_OUTSIDEVOLUMEHF;
        size = sizeof(long);
        data = &l;
        l = (long)m_OutsideHF.GetPos();
        sprintf(buf, "%d", l);
        m_OutsideHFText.SetWindowText(buf);
    }

    EaxSet(&DSPROPSETID_EAX20_BufferProperties,
           property, SourceNames[0], data, size);


    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}


//*****************************************************************************
// EAXBUFFER_CHECK
//*****************************************************************************
// Generally I believe in local copies of variable settings but I know
// that NVIDIA settings are gotten to very, very quickly.  (No ring
// transitions, etc.  Just a case statement.)  You are on your
// own with other implementations.
//
#define EAXBUFFER_CHECK(control, flag)                                  \
    DWORD flags;                                                        \
    EaxGet(&DSPROPSETID_EAX20_BufferProperties,                         \
           DSPROPERTY_EAXBUFFER_FLAGS,                                  \
           0, &flags, sizeof(DWORD));                                   \
                                                                        \
    if(control.GetCheck())                                              \
    {                                                                   \
        flags |= flag;                                                  \
    }                                                                   \
                                                                        \
    else                                                                \
    {                                                                   \
        flags &= ~flag;                                                 \
    }                                                                   \
                                                                        \
    EaxSet(&DSPROPSETID_EAX20_BufferProperties,                         \
           DSPROPERTY_EAXBUFFER_FLAGS,                                  \
           SourceNames[0], &flags, sizeof(DWORD))


//*****************************************************************************
// OnEaxbufAutoRoom
//*****************************************************************************
//
void CPropertyPageEaxBuffer::OnEaxbufAutoRoom()
{
    EAXBUFFER_CHECK(m_AutoRoom, EAXBUFFERFLAGS_ROOMAUTO);
}


//*****************************************************************************
// OnEaxbufAutoRoomhf
//*****************************************************************************
//
void CPropertyPageEaxBuffer::OnEaxbufAutoRoomhf()
{
    EAXBUFFER_CHECK(m_AutoRoomHF, EAXBUFFERFLAGS_ROOMHFAUTO);
}


//*****************************************************************************
// OnEaxbuffAutoDirecthf
//*****************************************************************************
//
void CPropertyPageEaxBuffer::OnEaxbuffAutoDirecthf()
{
    EAXBUFFER_CHECK(m_AutoDirectHF, EAXBUFFERFLAGS_DIRECTHFAUTO);
}
