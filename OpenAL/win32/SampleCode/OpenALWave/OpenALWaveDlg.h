//*****************************************************************************
//*****************************************************************************
//
// OpenALWaveDlg.h
//
//
// Description:
// This file defines the dialog class.
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




#if !defined(AFX_OPENALWAVEDLG_H__8832D9E2_A734_4578_98B2_CC0CE4D02941__INCLUDED_)
#define AFX_OPENALWAVEDLG_H__8832D9E2_A734_4578_98B2_CC0CE4D02941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//*****************************************************************************
//*****************************************************************************
//
// Classes
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// COpenALWaveDlg
//*****************************************************************************
//
class COpenALWaveDlg :
    public CDialog
{
// Construction
public:
    COpenALWaveDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(COpenALWaveDlg)
    enum { IDD = IDD_OPENALWAVE_DIALOG };
    CButton m_AlignAdpcm;
    CButton m_Stream;
    CButton m_NvAdpcmStereo;
    CButton m_NvAdpcmMono;
    CButton m_NvAdpcmFrontRight;
    CButton m_NvAdpcmFrontLeft;
    CButton m_NvAdpcmFrontCenter;
    CButton m_NvAdpcmBackRight;
    CButton m_NvAdpcmBackLeft;
    CButton m_Stereo;
    CButton m_Quad;
    CButton m_Mono;
    CButton m_5point1;
    CButton m_FrontRight;
    CButton m_FrontLeft;
    CButton m_FrontCenter;
    CButton m_BackRight;
    CButton m_BackLeft;
    CButton m_Button;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COpenALWaveDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    BOOL m_Play[WAVE_TYPE_COUNT];

    // Generated message map functions
    //{{AFX_MSG(COpenALWaveDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnMonoButton();
    afx_msg void OnStereoButton();
    afx_msg void OnQuadButton();
    afx_msg void On5point1Button();
    afx_msg void OnMonoButton2();
    afx_msg void OnStereoButton2();
    afx_msg void OnStream();
    afx_msg void OnAlign();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    void OnButton(CButton* Button, int Name);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENALWAVEDLG_H__8832D9E2_A734_4578_98B2_CC0CE4D02941__INCLUDED_)
