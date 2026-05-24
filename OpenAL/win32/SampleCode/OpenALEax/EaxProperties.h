//*****************************************************************************
//*****************************************************************************
//
// EaxProperties.h
//
//
// Description:
// This file defines the EAX property classes.
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




#ifndef __EAXPROPERTIES_H__
#define __EAXPROPERTIES_H__


//*****************************************************************************
//*****************************************************************************
//
// Global Headers
//
//*****************************************************************************
//*****************************************************************************

#include "../common/OpenALHelp.h"
#include "../../../../inc/eax.h"


//*****************************************************************************
//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
//*****************************************************************************

extern ALdevice*  OpenALDevice;
extern ALcontext* OpenALContext;
extern ALuint BufferNames[1];
extern ALuint SourceNames[1];
extern EAXGet EaxGet;
extern EAXSet EaxSet;



//*****************************************************************************
//*****************************************************************************
//
// Classes
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// CPropertyPageEaxListener
//*****************************************************************************
//
class CPropertyPageEaxListener : public CPropertyPage
{
    DECLARE_DYNCREATE(CPropertyPageEaxListener)

// Construction
public:
    CPropertyPageEaxListener();
    ~CPropertyPageEaxListener();

    // Dialog Data
    //{{AFX_DATA(CPropertyPageEaxListener)
    enum { IDD = IDD_EAXLIST };
    CStatic m_ReflectionsDelayText;
    CStatic m_ReverbDelayText;
    CButton m_ScaleReverbDelay;
    CButton m_ClipDelay;
    CButton m_ScaleReverb;
    CButton m_ScaleReflectionsDelay;
    CButton m_ScaleDelayTime;
    CButton m_ScaleReflections;
    CStatic m_RolloffText;
    CSliderCtrl m_Rolloff;
    CStatic m_RoomHFText;
    CSliderCtrl m_RoomHF;
    CStatic m_RoomText;
    CSliderCtrl m_ReverbDelay;
    CStatic m_ReverbText;
    CSliderCtrl m_Reverb;
    CSliderCtrl m_ReflectionsDelay;
    CStatic m_ReflectionsText;
    CSliderCtrl m_Reflections;
    CStatic m_EnvironmentSizeText;
    CSliderCtrl m_EnvironmentSize;
    CComboBox m_EnvironmentPreset;
    CStatic m_DiffusionText;
    CSliderCtrl m_Diffusion;
    CStatic m_DecayTimeText;
    CSliderCtrl m_DecayTime;
    CStatic m_DecayHFText;
    CSliderCtrl m_DecayHF;
    CStatic m_AirAbsorptionText;
    CSliderCtrl m_AirAbsorption;
    CSliderCtrl m_Room;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageEaxListener)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageEaxListener)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeEaxlistEnvironmentPreset();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnEaxlistScaleReflections();
    afx_msg void OnEaxlistScaleReflectionsdelay();
    afx_msg void OnEaxlistScaleReverb();
    afx_msg void OnEaxlistScaleReverbdelay();
    afx_msg void OnEaxlistScaleDecaytime();
    afx_msg void OnEaxlistClipDecay();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};



//*****************************************************************************
// CPropertyPageEaxBuffer
//*****************************************************************************
//
class CPropertyPageEaxBuffer : public CPropertyPage
{
    DECLARE_DYNCREATE(CPropertyPageEaxBuffer)

// Construction
public:
    CPropertyPageEaxBuffer();
    ~CPropertyPageEaxBuffer();

    // Dialog Data
    //{{AFX_DATA(CPropertyPageEaxBuffer)
    enum { IDD = IDD_EAXBUF };
    CButton m_AutoRoomHF;
    CSliderCtrl m_OutsideHF;
    CSliderCtrl m_OcclusionRoom;
    CSliderCtrl m_OcclusionLF;
    CStatic m_OcclusionLFText;
    CSliderCtrl m_ObstructionLF;
    CSliderCtrl m_DirectHF;
    CButton m_AutoDirectHF;
    CStatic m_RolloffText;
    CSliderCtrl m_Rolloff;
    CStatic m_RoomHFText;
    CSliderCtrl m_RoomHF;
    CStatic m_RoomText;
    CSliderCtrl m_Room;
    CStatic m_OutsideHFText;
    CStatic m_OcclusionRoomText;
    CStatic m_OcclusionText;
    CSliderCtrl m_Occlusion;
    CStatic m_ObstructionLFText;
    CStatic m_ObstructionText;
    CSliderCtrl m_Obstruction;
    CStatic m_DirectHFText;
    CStatic m_DirectText;
    CSliderCtrl m_Direct;
    CButton m_AutoRoom;
    CStatic m_AirAbsorptionText;
    CSliderCtrl m_AirAbsorption;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageEaxBuffer)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageEaxBuffer)
    afx_msg void OnPlay();
    afx_msg void OnStop();
    virtual BOOL OnInitDialog();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnEaxbufAutoRoom();
    afx_msg void OnEaxbufAutoRoomhf();
    afx_msg void OnEaxbuffAutoDirecthf();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};



#endif // __EAXPROPERTIES_H__
