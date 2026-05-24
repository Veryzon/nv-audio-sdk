//************************************************************************
// SimpleWaveUI.cpp : This is the User Interface (UI) code.
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
#include "SimpleWave.h"

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
    // Set the radio buttons for the method.
    //
    SendMessage( GetDlgItem( hDlg, IDC_METHOD1 ), BM_SETCHECK, BST_CHECKED, 0);
    SendMessage( GetDlgItem( hDlg, IDC_METHOD2 ), BM_SETCHECK, BST_UNCHECKED, 0);

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

    // Handle the method checkboxes
    EnableWindow( GetDlgItem( hDlg, IDC_METHOD1), !bPlaying );
    EnableWindow( GetDlgItem( hDlg, IDC_METHOD2), !bPlaying );

    // Handle the Speaker output checkboxes
    EnableWindow( GetDlgItem( hDlg, IDC_LEFT ), !bPlaying );
    EnableWindow( GetDlgItem( hDlg, IDC_RIGHT ), !bPlaying );
    EnableWindow( GetDlgItem( hDlg, IDC_CENTER), !bPlaying );
    EnableWindow( GetDlgItem( hDlg, IDC_REARLEFT), !bPlaying );
    EnableWindow( GetDlgItem( hDlg, IDC_REARRIGHT), !bPlaying );
    EnableWindow( GetDlgItem( hDlg, IDC_SUBWOOFER), !bPlaying );

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
// Name: ValidateMonoWaveFile
// Desc: Open the wave file to make sure it's a valid file for mono usage.
//************************************************************************
HRESULT ValidateMonoWaveFile( HWND hDlg, char* strFileName, DWORD FileControl )
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
            MessageBox( hDlg, TEXT("Wave file must be PCM for this application."), 
                              TEXT("ValidateMonoWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, FileControl, TEXT("") );
            return E_FAIL;
        }

        if( pwfx->nChannels != 1)
        {
            // Sound must be PCM when using DSBCAPS_CTRLFX
            MessageBox( hDlg, TEXT("Wave file must contain 1 channel."), 
                              TEXT("ValidateMonoWaveFile Error"), MB_OK);
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
// Name: ConvertMonoPCMtoMonoExtensible
// Desc: Convert a standard mono PCM wave file to an extensible file
//       with the appropriate channel mask information(user selected).
//************************************************************************
bool ConvertMonoPCMtoMonoExtensible( HWND hDlg, PBUFFER pBuffer )
{
    //
    // Create a temporary file name in the same directory
    // that is only different by some appended text (an 'x').
    //
    DWORD len = _tcslen( pBuffer->strFileName );
    DWORD i;
    TCHAR * ptrSrc = pBuffer->strFileName;
    TCHAR * ptrDest = pBuffer->strTempName;

    for(i = 0; i < len; i++)
    {
        if( *ptrSrc == '.' )
        {
            *ptrDest = 'x';
            ptrDest++;
        }
            
        *ptrDest = *ptrSrc;
        ptrDest++;
        ptrSrc++;
    }
    *ptrDest = NULL;

    //
    // Copy the original File to a temporary one.
    //
    SHFILEOPSTRUCT FileOp;
    LPSHFILEOPSTRUCT lpFileOp = &FileOp;

    lpFileOp->hwnd   = NULL;
    lpFileOp->wFunc  = FO_COPY;
    lpFileOp->pFrom  = pBuffer->strFileName;
    lpFileOp->pTo    = pBuffer->strTempName;
    lpFileOp->fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
    lpFileOp->fAnyOperationsAborted = 0;
    lpFileOp->hNameMappings = NULL;
    lpFileOp->lpszProgressTitle = NULL;

    if(SHFileOperation( lpFileOp ))
        return FALSE;

    //
    // Change the temporary file's attributes so that we
    // can modify the file.
    //
    DWORD attrib = GetFileAttributes( pBuffer->strTempName );
    if(attrib != 0xffffffff)
    {
        // We retrieved the attributes, now lets change them.
        // We only remove the read only attribute if it is set.
        if(attrib & FILE_ATTRIBUTE_READONLY)
        {
            attrib &= ~FILE_ATTRIBUTE_READONLY;
            SetFileAttributes( pBuffer->strTempName, attrib );
        }
    }

    //
    // Open the File for modification
    //
    CWaveFile * pWaveFile = new CWaveFile();
    if( pWaveFile == NULL )
    {
        return FALSE;
    }

    pWaveFile->Open( pBuffer->strTempName, NULL, WAVEFILE_READ );

    //
    // Get the current header information.
    //
    PWAVEFORMATEX pwf;
    pwf = pWaveFile->GetFormat();
    DWORD dwSize = pWaveFile->GetSize();

    //
    // Modify the header information to extensible.
    //
    WAVEFORMATEXTENSIBLE wfx;
    PWAVEFORMATEXTENSIBLE pwfx = &wfx;
    
    memcpy( pwfx, pwf, sizeof(WAVEFORMATEX) );
    pwfx->Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    pwfx->Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
    pwfx->Samples.wValidBitsPerSample = pwf->wBitsPerSample;
    pwfx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

    long left      = SendMessage( GetDlgItem( hDlg, IDC_LEFT ), BM_GETCHECK, 0, 0);
    long right     = SendMessage( GetDlgItem( hDlg, IDC_RIGHT ), BM_GETCHECK, 0, 0);
    long center    = SendMessage( GetDlgItem( hDlg, IDC_CENTER ), BM_GETCHECK, 0, 0);
    long rearleft  = SendMessage( GetDlgItem( hDlg, IDC_REARLEFT ), BM_GETCHECK, 0, 0);
    long rearright = SendMessage( GetDlgItem( hDlg, IDC_REARRIGHT ), BM_GETCHECK, 0, 0);
    long subwoofer = SendMessage( GetDlgItem( hDlg, IDC_SUBWOOFER ), BM_GETCHECK, 0, 0);

    pwfx->dwChannelMask  = (left == BST_CHECKED) ? SPEAKER_FRONT_LEFT : 0;
    pwfx->dwChannelMask |= (right == BST_CHECKED) ? SPEAKER_FRONT_RIGHT : 0;
    pwfx->dwChannelMask |= (center == BST_CHECKED) ? SPEAKER_FRONT_CENTER : 0;
    pwfx->dwChannelMask |= (rearleft == BST_CHECKED) ? SPEAKER_BACK_LEFT : 0;
    pwfx->dwChannelMask |= (rearright == BST_CHECKED) ? SPEAKER_BACK_RIGHT : 0;
    pwfx->dwChannelMask |= (subwoofer == BST_CHECKED) ? SPEAKER_LOW_FREQUENCY : 0;

    //
    // Create memory to hold the wave data.
    //
    PBYTE pWaveData = NULL;
    HANDLE hWaveHeap = HeapCreate(0, dwSize, 0 );
    if(hWaveHeap)
    {
        pWaveData = (PBYTE)HeapAlloc( hWaveHeap, HEAP_ZERO_MEMORY, dwSize );
        if(pWaveData == NULL)
        {
            HeapDestroy( hWaveHeap );
            pWaveFile->Close();
            delete pWaveFile;
            return FALSE;
        }
    }
    else
    {
        pWaveFile->Close();
        delete pWaveFile;
        return FALSE;
    }


    //
    // Read the wave data from the original wave file.
    //
    DWORD dwSizeRead = 0;
    pWaveFile->Read( pWaveData, dwSize, &dwSizeRead );
    if(dwSizeRead != dwSize)
    {
        HeapFree( hWaveHeap, 0, pWaveData );
        HeapDestroy( hWaveHeap );
        pWaveFile->Close();
        delete pWaveFile;
        return FALSE;
    }

    //
    // Write the new data back
    // Cast the bigger structure (pwfx) to a WAVEFORMATEX structure
    //
    UINT dwSizeWrote = 0;
    pWaveFile->Open( pBuffer->strTempName, (PWAVEFORMATEX)pwfx, WAVEFILE_WRITE );
    pWaveFile->Write( dwSize, pWaveData, &dwSizeWrote );
    pWaveFile->Close();

    //
    // Destroy memory we created above. All pointers valid at this point.
    //
    HeapFree( hWaveHeap, 0, pWaveData );
    HeapDestroy( hWaveHeap );
    delete pWaveFile;
    
    if(dwSizeWrote != dwSize)
        return FALSE;

    return TRUE;
}


//************************************************************************
// End of File: SimpleWaveUI.cpp
//************************************************************************
