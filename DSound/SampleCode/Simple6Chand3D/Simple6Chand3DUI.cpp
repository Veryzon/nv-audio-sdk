//************************************************************************
// Simple6Chand3DUI.cpp : This is the User Interface (UI) code.
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

#define STRICT

#include "stdafx.h"
#include "Simple6Chand3D.h"

extern HINSTANCE g_hInst;
extern BUFFER    g_Buffer[MAX_FILES];     // Buffer information.

//************************************************************************
// Name: ProcessCommandLine
// Desc: Process command line parameters if there are any.
//************************************************************************
void ProcessCommandLine( HWND hDlg, LPSTR pCmdLineA )
{
    int NumFiles = 0;
    int i;
    TCHAR strFileCache[MAX_FILES][MAX_PATH];  // Command line Filename storage
    bool  bOrbitOn = FALSE;
    bool  bStartNow = FALSE;

    // Init the random seed
    srand( (unsigned)time( NULL ) );
    rand();


    TCHAR seps[] = " ";
    TCHAR *token;
    TCHAR string[MAX_PATH];

    token = _tcstok(pCmdLineA, seps);

    //
    // Parse the command line (could have some error checking I guess).
    // Anything not a command is assumed to be a filename 
    // (file Validation routine will report any errors here).
    // Filenames cannot have spaces, sorry.
    //
    while(token != NULL)
    {
        _stscanf(token, "%s", &string );

        if(_tcscmp(string, TEXT("OrbitOn")) == 0)
        {
            bOrbitOn = TRUE;
        }
        else if(_tcscmp(string, TEXT("StartNow")) == 0)
        {
            bStartNow = TRUE;
        }
        else if(_tcscmp(string, TEXT("Always")) == 0)
        {
            if(NumFiles != 0)
                g_Buffer[NumFiles - 1].Looping = DSBPLAY_LOOPING;
        }
        else if(_tcscmp(string, TEXT("Random")) == 0)
        {
            if(NumFiles != 0)
            {
                g_Buffer[NumFiles - 1].Looping = 0;
                g_Buffer[NumFiles - 1].RandomCount = 0;

                // Select a random number between 1 and 10 (seconds)
                // One orbits takes 16 seconds, after done wait 
                // this amount of time before restarting.
                g_Buffer[NumFiles - 1].RandomRestart = rand() % RANDOM_MAX_WAIT_PERIOD;
            }
        }
        else
        {
            _tcscpy(strFileCache[NumFiles], string);
            NumFiles++;
        }

        token = _tcstok(NULL, seps);
    }

    //
    // Load all of the files
    //
    for(i = 0; i < NumFiles; i++)
    {
        // First file found is 6 channel by definition.
        bool Is3D = (i == 0) ? FALSE : TRUE;
        OpenFileToPlay( hDlg, i, strFileCache[i], Is3D );
    }

    if(bOrbitOn)
    {
        HWND hAutoMove = GetDlgItem( hDlg, IDC_ORBIT );
        long AutoMove = SendMessage( hAutoMove, BM_SETCHECK, BST_CHECKED, 0);
    }

    if(bStartNow)
    {
        StartPlayingWave( hDlg );
    }
}


//************************************************************************
// Name: InitializeUI
// Desc: Initialize all User Interface controls.
//************************************************************************
bool InitializeUI( HWND hDlg )
{
    //
    // Load the icon
    //
    HICON hIcon = LoadIcon( g_hInst, MAKEINTRESOURCE( IDI_NVIDIA ) );
    PostMessage( hDlg, WM_SETICON, ICON_BIG, ( LPARAM ) hIcon );
    PostMessage( hDlg, WM_SETICON, ICON_SMALL, ( LPARAM ) hIcon );


    //
    // Disable the Play/Stop buttons and the ListView and Slider controls.
    //
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), FALSE );

    return TRUE;
}


//************************************************************************
// Name: EnablePlayButton
// Desc: Enable the play button, if we are not already playing.
//************************************************************************
bool EnablePlayButton( HWND hDlg, PBUFFER pBuffer )
{
    DWORD i;
    bool bPlaying = FALSE;
    PBUFFER pBuf = pBuffer;

    for(i = 0; i < MAX_FILES; i++)
    {
        if(pBuf->Playing)
        {
            bPlaying = TRUE;
            break;
        }

        pBuf++;
    }

    if(!bPlaying)
        EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), TRUE );

    return TRUE;
}


//************************************************************************
// Name: Validate3DWaveFile
// Desc: Open the wave file to make sure it's a valid file for 3D usage.
//************************************************************************
HRESULT Validate3DWaveFile( HWND hDlg, char* strFileName, DWORD FileControl )
{
    HRESULT hr;
    CWaveFile waveFile;

    if( -1 == GetFileAttributes(strFileName) )
        return E_FAIL;
    
    // Load the wave file
    if( FAILED( hr = waveFile.Open( strFileName, NULL, WAVEFILE_READ ) ) )
    {
        waveFile.Close();
        MessageBox( hDlg, TEXT("Bad wave file."), 
                          TEXT("ValidateWaveFile Error"), MB_OK);
        return DXTRACE_ERR( TEXT("Open"), hr );
    }
    else // The load call succeeded
    {
        WAVEFORMATEX* pwfx = waveFile.GetFormat();
        if( pwfx->wFormatTag != WAVE_FORMAT_PCM )
        {
            // Sound must be PCM when using DSBCAPS_CTRLFX
            MessageBox( hDlg, TEXT("Wave file must be PCM for 3D control."), 
                              TEXT("Validate3DWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }

        if( pwfx->nChannels != 1)
        {
            // Sound must be PCM when using DSBCAPS_CTRLFX
            MessageBox( hDlg, TEXT("Wave file must be mono for 3D control."), 
                              TEXT("Validate3DWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }
  
        // Update the UI controls to show the sound as the file is loaded
        waveFile.Close();
        SetDlgItemText( hDlg, FileControl, strFileName );
        return S_OK;
    }
}


//************************************************************************
// Name: ValidateExtensibleWaveFile
// Desc: Open the wave file to make sure it's a valid file for 6 channel usage.
//************************************************************************
HRESULT ValidateExtensibleWaveFile( HWND hDlg, char* strFileName, DWORD FileControl )
{
    HRESULT hr;
    CWaveFile waveFile;

    if( -1 == GetFileAttributes(strFileName) )
        return E_FAIL;
    
    // Load the wave file
    if( FAILED( hr = waveFile.Open( strFileName, NULL, WAVEFILE_READ ) ) )
    {
        waveFile.Close();
        MessageBox( hDlg, TEXT("Bad wave file."), 
                          TEXT("ValidateWaveFile Error"), MB_OK);
        return DXTRACE_ERR( TEXT("Open"), hr );
    }
    else // The load call succeeded
    {
        WAVEFORMATEX* pwfx = waveFile.GetFormat();
        if( pwfx->wFormatTag != WAVE_FORMAT_EXTENSIBLE )
        {
            // Sound must be PCM when using DSBCAPS_CTRLFX
            MessageBox( hDlg, TEXT("Wave file must be EXTENSIBLE for 6 channel control."), 
                              TEXT("ValidateExtensibleWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }

        if( pwfx->nChannels != 6)
        {
            // Sound must be PCM when using DSBCAPS_CTRLFX
            MessageBox( hDlg, TEXT("Wave file must contain 6 channels."), 
                              TEXT("ValidateExtensibleWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }
  
        // Update the UI controls to show the sound as the file is loaded
        waveFile.Close();
        SetDlgItemText( hDlg, FileControl, strFileName );
        return S_OK;
    }
}


//************************************************************************
// End of File: Simple6Chand3DUI.cpp
//************************************************************************
