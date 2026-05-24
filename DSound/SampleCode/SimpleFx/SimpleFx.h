//************************************************************************
// SimpleFx.h : Definitions for SimpleFx.cpp
//
//************************************************************************

 /***************************************************************************\
|*                                                                           *|
|*            Copyright NVIDIA Corporation.  All rights reserved.            *|
|*                                                                           *|
|*     NOTICE TO USER:   The source code  is copyrighted under  U.S. and     *|
|*     international laws.  Users and possessors of this source code are     *|
|*     hereby granted a nonexclusive,  royalty-free copyright license to     *|
|*     use this code in individual and commercial software.                  *|
|*                                                                           *|
|*     Any use of this source code must include,  in the user documenta-     *|
|*     tion and  internal comments to the code,  notices to the end user     *|
|*     as follows:                                                           *|
|*                                                                           *|
|*     NVIDIA CORPORATION MAKES NO REPRESENTATION ABOUT THE  SUITABILITY     *|
|*     OF  THIS SOURCE  CODE  FOR ANY PURPOSE.  IT IS  PROVIDED  "AS IS"     *|
|*     WITHOUT EXPRESS OR IMPLIED WARRANTY OF  ANY KIND.  NVIDIA CORPOR-     *|
|*     ATION DISCLAIMS ALL WARRANTIES  WITH REGARD  TO THIS SOURCE CODE,     *|
|*     INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGE-     *|
|*     MENT,  AND FITNESS  FOR A PARTICULAR PURPOSE.   IN NO EVENT SHALL     *|
|*     NVIDIA CORPORATION  BE LIABLE FOR  ANY SPECIAL,  INDIRECT,  INCI-     *|
|*     DENTAL, OR CONSEQUENTIAL DAMAGES,  OR ANY DAMAGES  WHATSOEVER RE-     *|
|*     SULTING FROM LOSS OF USE,  DATA OR PROFITS,  WHETHER IN AN ACTION     *|
|*     OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT OF     *|
|*     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.     *|
|*                                                                           *|
|*     U.S. Government  End  Users.   This source code  is a "commercial     *|
|*     item,"  as that  term is  defined at  48 C.F.R. 2.101 (OCT 1995),     *|
|*     consisting  of "commercial  computer  software"  and  "commercial     *|
|*     computer  software  documentation,"  as such  terms  are  used in     *|
|*     48 C.F.R. 12.212 (SEPT 1995)  and is provided to the U.S. Govern-     *|
|*     ment only as  a commercial end item.   Consistent with  48 C.F.R.     *|
|*     12.212 and  48 C.F.R. 227.7202-1 through  227.7202-4 (JUNE 1995),     *|
|*     all U.S. Government End Users  acquire the source code  with only     *|
|*     those rights set forth herein.                                        *|
|*                                                                           *|
|*                                                                    Rev A  *|
 \***************************************************************************/

#if !defined(AFX_SIMPLEFX_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_)
#define AFX_SIMPLEFX_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

//
// Defines
//
#define MAX_EFFECTS          9     // 8 effects, plus the not selected case
#define EFFECT_STRING_MAX   32     // Maximim size of effects names
#define MAX_PARAMS_PER_EFFECT 12   // Reverb has the most parameters (12)

#define FX_NONE              0     // No effect is selected
#define FX_MONO              1     // We are a mono effect (1 in, 1 out)
#define FX_STEREO            2     // We are a stereo effect (2 in, 2 out)

#define MAX_PARAMS_NAME_SIZE 128   // Size of longest possible parameter name string
#define MAX_PARAMS_UNIT_SIZE 32    // Size of longest possible unit string

#define MAX_FORMAT_SIZE      12    // Maximum size of a string used to format the data in Listview.

// Floating point Formats
#define FORMAT_5point1Float     "%5.1f"
#define FORMAT_4point1Float     "%4.1f"
#define FORMAT_3point1Float     "%3.1f"
#define FORMAT_2point2Float     "%2.2f"
#define FORMAT_1point3Float     "%1.3f"
#define FORMAT_DWORD            "%d"
#define FORMAT_NotUsed          ""

// Strings used for parameter units
#define IDS_STRING_UNIT_NULL0   " "
#define IDS_STRING_UNIT_NULL1   "NULL1"
#define IDS_STRING_UNIT_NULL2   "NULL2"
#define IDS_STRING_UNIT_NULL3   "NULL3"
#define IDS_STRING_UNIT_NULL4   "NULL4"
#define IDS_STRING_UNIT_HZ      "Hz"
#define IDS_STRING_UNIT_MSEC    "msec"
#define IDS_STRING_UNIT_DB      "dB"
#define IDS_STRING_UNIT_MB      "mB"
#define IDS_STRING_UNIT_SEC     "seconds"
#define IDS_STRING_UNIT_RATIO   "(x:1)"
#define IDS_STRING_UNIT_PERCENT "%"
#define IDS_STRING_UNIT_METERS  "meters"
#define IDS_STRING_UNIT_FACTOR  "(factor)"
#define IDS_STRING_UNIT_RATE    "Hz "

#define IDS_STRING_PHASE1       "-180 Degrees"
#define IDS_STRING_PHASE2       "-90 Degrees"
#define IDS_STRING_PHASE3       "0 Degrees"
#define IDS_STRING_PHASE4       "90 Degrees"
#define IDS_STRING_PHASE5       "180 Degrees"

#define IDS_STRING_WAVEFORM1    "Triangle"
#define IDS_STRING_WAVEFORM2    "Sine"
#define IDS_STRING_WAVEFORM3    "Square"

#define IDS_STRING_ONOFF1       "Off"
#define IDS_STRING_ONOFF2       "On"

enum FxType
{
    FX_NOP = 0,
    FX_CHORUS,
    FX_COMPRESSOR,
    FX_DISTORTION,
    FX_ECHO,
    FX_FLANGER,
    FX_GARGLE,
    FX_PARAMEQ,
    FX_REVERB
};



//
// Macros
//
#define CheckResult(hr) \
    if(hr != DS_OK) { \
    MessageBox( NULL, "Error occurred while building the effects tree in the hardware", \
                      "WireUpEffect Error", MB_OK | MB_ICONERROR ); }

// A wiring macro
#define SetWire( CnnDesc, SrcId, SrcPin, DestId, DestPin)\
          CnnDesc.SourceId = SrcId;             \
          CnnDesc.SourcePin = SrcPin;           \
          CnnDesc.DestinationId = DestId;       \
          CnnDesc.DestinationPin = DestPin;


//
// Structures
//
typedef struct tParameter
{
    //
    // Slider Setup parameters
    //
    long           SliderMax;                           // Max value to slider
    long           SliderMin;                           // Min value to slider
    long           SliderTic;                           // Tic frequency to slider
    float          fpConstant;                          // Multiply Limits and Data, Divide when read data
    TCHAR          Format[MAX_FORMAT_SIZE];             // Pointer to format to display i.e. %3.1f

} PARAMETER, *PPARAMETER;

//
// Data declarations
//
extern DWORD m_NumOfParms[MAX_EFFECTS];
extern DWORD m_FxSize[MAX_EFFECTS];
extern char m_FxNames[MAX_EFFECTS][EFFECT_STRING_MAX];
extern DWORD m_FxType[MAX_EFFECTS];
extern GUID m_FxGuid[MAX_EFFECTS];
extern GUID m_DataGuid[MAX_EFFECTS];
extern char * m_EParams[MAX_EFFECTS];
extern char * m_EUnits[MAX_EFFECTS];
extern char * m_ELookup[];
extern DWORD m_ParamConv[MAX_EFFECTS][MAX_PARAMS_PER_EFFECT];
extern LPVOID g_pParams[MAX_EFFECTS];


//
// Foward declarations of functions.
//
LRESULT CALLBACK DialogMsgHandler( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
bool InitializeDialog( HWND hDlg );
bool OpenFileToPlay( HWND hDlg );
bool StartPlayingWave( HWND hDlg );
bool StopPlayingWave( HWND hDlg );
bool HandleSlider( HWND hDlg, LPARAM lParam );
bool HandleEffectChange( HWND hDlg );
bool HandleListviewSelect( HWND hDlg, LPARAM lParam );
bool HandleExit( HWND hDlg );
bool WireUpEffect( DWORD EffectSelected );
bool DestroyEffects( DWORD Effect );


//
// Forward declarations for User Interface (UI) functions.
//
bool InitializeUI( HWND hDlg );
HRESULT ValidateWaveFile( HWND hDlg, char* strFileName );
bool UpdateParameterControls( HWND hDlg, DWORD Fx );
bool WriteListViewColumn1( HWND hDlg, DWORD EffectSelected );
bool WriteListViewColumn2( HWND hDlg, DWORD EffectSelected );
bool ProcessData( DWORD ParamSelected, DWORD * pData, TCHAR * pUnits, TCHAR * string );
bool AssembleSlider( HWND hDlg, DWORD EffectSelected );
bool GetSliderData( HWND hDlg );
bool InitializeSliderValues( DWORD EffectSelected );


#endif // !defined(AFX_SIMPLEFX_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_)


//************************************************************************
// End of File: SimpleFx.h
//************************************************************************
