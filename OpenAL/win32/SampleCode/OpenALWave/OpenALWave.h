//*****************************************************************************
//*****************************************************************************
//
// OpenALWave.h
//
//
// Description:
// This file defines the application class.
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




#if !defined(AFX_OPENALWAVE_H__AC907493_5221_4E31_A9CC_63F51E3815A7__INCLUDED_)
#define AFX_OPENALWAVE_H__AC907493_5221_4E31_A9CC_63F51E3815A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


//*****************************************************************************
//*****************************************************************************
//
// Global Headers
//
//*****************************************************************************
//*****************************************************************************

#include "../common/OpenALHelp.h"
#include "../common/wavefile.h"



//*****************************************************************************
//*****************************************************************************
//
// Defines
//
//*****************************************************************************
//*****************************************************************************

enum
{
    WAVE_TYPE_MONO = 0,
    WAVE_TYPE_STEREO,
    WAVE_TYPE_QUAD,
    WAVE_TYPE_5POINT1,
    WAVE_TYPE_NVADPCM_MONO,
    WAVE_TYPE_NVADPCM_STEREO,

    WAVE_TYPE_COUNT
};


//*****************************************************************************
//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
//*****************************************************************************

extern ALdevice*  OpenALDevice;
extern ALcontext* OpenALContext;
extern ALuint BufferNames[WAVE_TYPE_COUNT];
extern ALuint SourceNames[WAVE_TYPE_COUNT];
extern CWaveFile WaveFile[WAVE_TYPE_COUNT];
extern ALuint* AlignedBuffers[WAVE_TYPE_COUNT];
extern ALuint  AlignedBufferCount[WAVE_TYPE_COUNT];



//*****************************************************************************
//*****************************************************************************
//
// Classes
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// COpenALWaveApp
//*****************************************************************************
//
class COpenALWaveApp :
    public CWinApp
{

//*****************************************************************************
// PUBLIC METHODS
//*****************************************************************************
public:
    COpenALWaveApp();
    virtual ~COpenALWaveApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COpenALWaveApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(COpenALWaveApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENALWAVE_H__AC907493_5221_4E31_A9CC_63F51E3815A7__INCLUDED_)
