//************************************************************************
// Simple6Chand3D.h : Definitions for Simple6Chand3D.cpp
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

#if !defined(AFX_SIMPLE6CHAND3D_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_)
#define AFX_SIMPLE6CHAND3D_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

//
// Defines
//
#define MAX_FILES                   2     // We only support 2 wave files in this app.
#define RANDOM_MAX_WAIT_PERIOD      1000  // In hundreths of a second


//
// Structures
//
typedef struct 
{
    bool            Active;                 // TRUE if file is open and ready to use
    bool            Playing;                // TRUE is buffer is playing.
    bool            Is3D;                   // TRUE if 3D
    DWORD           Looping;                // Buffer is lopping flags.
    DWORD           FileControl;            // Control ID for the 'Edit' filename control.
    DWORD           OpenControl;            // Control ID for the 'Open' button control.

    DWORD           RandomRestart;          // Restart after waiting 1 to 10 seconds
    DWORD           RandomCount;            // Count hundreths of seconds here

    CSound          *pSound;                // Pointer the sound class
    LPKSPROPERTYSET pKsPropSet;             // Property set path to DirectSound
    LPDIRECTSOUNDBUFFER pDSB;               // Pointer to Direct Sound buffer.

    TCHAR           strFileName[MAX_PATH];  // Filename of buffer

} BUFFER, *PBUFFER;


//
// Foward declarations of functions.
//
LRESULT CALLBACK DialogMsgHandler( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
bool InitializeDialog( HWND hDlg );
HRESULT OpenHardwarePipe( HWND hDlg );
bool OpenFileToPlay( HWND hDlg, DWORD FileNum, TCHAR * pFilename, bool Is3D );
bool StartPlayingWave( HWND hDlg );
bool StopPlayingWave( HWND hDlg );
bool CloseBuffer( HWND hDlg, DWORD FileNum );
bool HandleExit( HWND hDlg );
bool PlayIfOthersPlaying( HWND hDlg, DWORD BufferNum );


//
// Forward declarations for User Interface (UI) functions.
//
void ProcessCommandLine( HWND hDlg, LPSTR pCmdLineA );
bool InitializeUI( HWND hDlg );
bool EnablePlayButton( HWND hDlg, PBUFFER pBuffer );

HRESULT Validate3DWaveFile( HWND hDlg, char* strFileName, DWORD FileControl );
HRESULT ValidateExtensibleWaveFile( HWND hDlg, char* strFileName, DWORD FileControl );


#endif // !defined(AFX_SIMPLE6CHAND3D_H__928F9296_CEE4_4F7F_B92E_07397D44EA6C__INCLUDED_)

//************************************************************************
// End of File: Simple6Chand3D.h
//************************************************************************
