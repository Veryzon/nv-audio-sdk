//*****************************************************************************
//*****************************************************************************
//
// I3dl2Properties.h
//
//
// Description:
// This file defines the I3DL2 property classes.
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




#ifndef __I3DL2PROPERTIES_H__
#define __I3DL2PROPERTIES_H__


//*****************************************************************************
//*****************************************************************************
//
// Global Headers
//
//*****************************************************************************
//*****************************************************************************

#include "../common/OpenALHelp.h"
#include "../../../../inc/3dl2.h"


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
extern I3DL2Get I3dl2Get;
extern I3DL2Set I3dl2Set;



//*****************************************************************************
//*****************************************************************************
//
// Classes
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// CPropertyPageI3dl2Listener
//*****************************************************************************
//
class CPropertyPageI3dl2Listener : public CPropertyPage
{
    DECLARE_DYNCREATE(CPropertyPageI3dl2Listener)

// Construction
public:
    CPropertyPageI3dl2Listener();
    ~CPropertyPageI3dl2Listener();

    // Dialog Data
    //{{AFX_DATA(CPropertyPageI3dl2Listener)
    enum { IDD = IDD_I3DL2LIST };
    CSliderCtrl m_Room;
    CStatic m_RoomText;
    CSliderCtrl m_RoomHF;
    CStatic m_RoomHFText;
    CSliderCtrl m_Rolloff;
    CStatic m_RolloffText;
    CSliderCtrl m_DecayTime;
    CStatic m_DecayTimeText;
    CSliderCtrl m_DecayHF;
    CStatic m_DecayHFText;
    CSliderCtrl m_Reflections;
    CStatic m_ReflectionsText;
    CSliderCtrl m_ReflectionsDelay;
    CStatic m_ReflectionsDelayText;
    CSliderCtrl m_Reverb;
    CStatic m_ReverbText;
    CSliderCtrl m_ReverbDelay;
    CStatic m_ReverbDelayText;
    CSliderCtrl m_Diffusion;
    CStatic m_DiffusionText;
    CSliderCtrl m_Density;
    CStatic m_DensityText;
    CSliderCtrl m_HFReference;
    CStatic m_HFReferenceText;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageI3dl2Listener)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageI3dl2Listener)
    virtual BOOL OnInitDialog();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};



//*****************************************************************************
// CPropertyPageI3dl2Buffer
//*****************************************************************************
//
class CPropertyPageI3dl2Buffer : public CPropertyPage
{
    DECLARE_DYNCREATE(CPropertyPageI3dl2Buffer)

// Construction
public:
    CPropertyPageI3dl2Buffer();
    ~CPropertyPageI3dl2Buffer();

    // Dialog Data
    //{{AFX_DATA(CPropertyPageI3dl2Buffer)
    enum { IDD = IDD_I3DL2BUF };
    CSliderCtrl m_Direct;
    CStatic m_DirectText;
    CSliderCtrl m_DirectHF;
    CStatic m_DirectHFText;
    CSliderCtrl m_Room;
    CStatic m_RoomText;
    CSliderCtrl m_RoomHF;
    CStatic m_RoomHFText;
    CSliderCtrl m_Rolloff;
    CStatic m_RolloffText;
    CSliderCtrl m_Obstruction;
    CStatic m_ObstructionText;
    CSliderCtrl m_ObstructionLF;
    CStatic m_ObstructionLFText;
    CSliderCtrl m_Occlusion;
    CStatic m_OcclusionText;
    CSliderCtrl m_OcclusionLF;
    CStatic m_OcclusionLFText;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageI3dl2Buffer)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageI3dl2Buffer)
    afx_msg void OnPlay();
    afx_msg void OnStop();
    virtual BOOL OnInitDialog();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};



#endif // __I3DL2PROPERTIES_H__
