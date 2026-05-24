//************************************************************************
// SimpleWave.h : Definitions for SimpleWave.cpp
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

#if !defined(AFX_SIMPLEWAVE_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_)
#define AFX_SIMPLEWAVE_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

//
// Defines
//
#define TEMPORARY_FILENAME      TEXT("SimpleWaveTempExtensible.wav")

#define RUN_METHOD1             0
#define RUN_METHOD2             1


//
// Macros
//
// A wiring macro
#define SetWire( CnnDesc, SrcId, SrcPin, DestId, DestPin)\
          CnnDesc.SourceId = SrcId;             \
          CnnDesc.SourcePin = SrcPin;           \
          CnnDesc.DestinationId = DestId;       \
          CnnDesc.DestinationPin = DestPin;


//
// Structures
//
typedef struct 
{
    bool            Active;                 // TRUE if file is open and ready to use
    bool            Playing;                // TRUE is buffer is playing.
    bool            Is3D;                   // TRUE if 3D
    DWORD           Looping;                // Buffer is lopping flags.

    CSound          *pSound;                // Pointer the sound class
    LPKSPROPERTYSET pKsPropSet;             // Property set path to DirectSound
    LPDIRECTSOUNDBUFFER pDSB;               // Pointer to Direct Sound buffer.

    TCHAR           strFileName[MAX_PATH];  // Filename of buffer
    TCHAR           strTempName[MAX_PATH];  // Temporary file for conversion

} BUFFER, *PBUFFER;


//
// Foward declarations of functions.
//
LRESULT CALLBACK DialogMsgHandler( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
bool InitializeDialog( HWND hDlg );
bool OpenFile( HWND hDlg );
bool InitSoundFile( HWND hDlg, TCHAR *filename, DWORD Method );
bool StartPlayingWave( HWND hDlg );
bool StopPlayingWave( HWND hDlg );
bool CloseBuffer( HWND hDlg );
bool HandleExit( HWND hDlg );
bool SetupPlayMethod( HWND hDlg );
bool DestroyPlayMethod( HWND hDlg );
bool WireSoundToOutputs( HWND hDlg );
bool DestroyTopology( HWND hDlg );


//
// Forward declarations for User Interface (UI) functions.
//
bool InitializeUI( HWND hDlg );
bool BeginPlayUI( HWND hDlg, bool bPlaying );
bool EnablePlayButton( HWND hDlg, PBUFFER pBuffer );

HRESULT ValidateMonoWaveFile( HWND hDlg, char* strFileName, DWORD FileControl );
bool ConvertMonoPCMtoMonoExtensible(  HWND hDlg, PBUFFER pBuffer );


#endif // !defined(AFX_SIMPLEWAVE_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_)

//************************************************************************
// End of File: SimpleSubmix.h
//************************************************************************
