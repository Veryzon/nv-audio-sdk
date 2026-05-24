//************************************************************************
// SimpleAdpcmUI.cpp : This is the User Interface (UI) code.
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
#include "SimpleAdpcm.h"

extern HINSTANCE g_hInst;


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
// Name: BeginPlayUI
// Desc: Handle all of the controls when we start/stop playing a wave file.
//************************************************************************
bool BeginPlayUI( HWND hDlg, bool bPlaying )
{
    // Handle the play/stop buttons
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), !bPlaying );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), bPlaying );

    // Disable the currently playing 'Open' button
    EnableWindow( GetDlgItem( hDlg, IDC_OPEN), !bPlaying );

    return TRUE;
}


//************************************************************************
// Name: EnablePlayButton
// Desc: Enable the play button, if we are not already playing.
//************************************************************************
bool EnablePlayButton( HWND hDlg, PBUFFER pBuffer )
{
    if(!pBuffer->Playing)
    {
        EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), TRUE );
    }

    return TRUE;
}


//************************************************************************
// Name: ValidateAdpcmWaveFile
// Desc: Open the NVADPCM file to make sure it's a valid file.
//************************************************************************
HRESULT ValidateAdpcmWaveFile( HWND hDlg, char* strFileName, DWORD FileControl )
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
                          TEXT("ValidateAdpcmWaveFile Error"), MB_OK);
        return DXTRACE_ERR( TEXT("Open"), hr );
    }
    else // The load call succeeded
    {
        WAVEFORMATEX* pwfx = waveFile.GetFormat();
        if( pwfx->wFormatTag != WAVE_FORMAT_EXTENSIBLE )
        {
            // Sound must be PCM when using DSBCAPS_CTRLFX
            MessageBox( hDlg, TEXT("Wave file must be WAVEFORMATEXTENSIBLE for this application."), 
                              TEXT("ValidateAdpcmWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }

        DWORD dwXtraSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
        if(pwfx->cbSize != dwXtraSize)
        {
            // Check that the format has the correct cbSize set 
            MessageBox( hDlg, TEXT("EXTENSIBLE format size is incorrect."), 
                              TEXT("ValidateAdpcmWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }

        PWAVEFORMATEXTENSIBLE pwfex = (PWAVEFORMATEXTENSIBLE)pwfx;
        if(pwfex->SubFormat != NVDATAFORMAT_SUBTYPE_ADPCM)
        {
            // Check that the subformat is the correct GUID type.
            MessageBox( hDlg, TEXT("EXTENSIBLE SubFormat is incorrect."), 
                              TEXT("ValidateAdpcmWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }

        if( pwfx->nChannels > 2)
        {
            // Sound must be PCM when using DSBCAPS_CTRLFX
            MessageBox( hDlg, TEXT("ADPCM file must contain 1 or 2 channels."), 
                              TEXT("ValidateAdpcmWaveFile Error"), MB_OK);
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
// End of File: SimpleAdpcmUI.cpp
//************************************************************************
