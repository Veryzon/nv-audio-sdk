//************************************************************************
// SimpleSubmix.cpp : A simple program for experimenting with nVidia 
//                audio effects.
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

#define INITGUID
#define STRICT

#include "stdafx.h"
#include "SimpleSubmix.h"
#include "HwInterface.h"

//
// Global Variables:
//
HINSTANCE               g_hInst;                 // Current instance
CSoundManager         * g_pSoundManager = NULL;  // Only one SoundManager for everyone
NVFXINST_FXMANAGER_ID   g_FxManagerId;           // nVidia Fx Manager Id
LPKSPROPERTYSET         g_pKsPropSet;            // Property set path to DirectSound (on dummy buffer)
LPDIRECTSOUNDBUFFER     g_pDSBPath = NULL;       // Direct Sound path
LPDIRECTSOUND3DLISTENER g_pDSListener = NULL;    // Listener interface

NVFXDATA_FX_ID          g_FxIdSubmix;            // The effect we add (only one at a time)
NVFXDATA_FX_ID          g_FxIdVoice;             // A VoiceMix that we attach to.
NVFXDATA_FX_ID          g_FxIdSplitter;          // A splitter, we only use one.

NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR g_FxCD[6];   // Structure of Wires (we wire 5 or 6 wires max)
DWORD                   g_NumberOfWires;         // Number of wires sent to hardware.

BUFFER                  g_Buffer[MAX_FILES];     // Buffer information.
COrbiter              * g_pCOrbiter;             // Orbiter class


//************************************************************************
// Name: WinMain
// Desc: The programs entry point.
//************************************************************************
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
    g_hInst = hInstance; // Store instance handle in our global variable

    // Init the common control dll 
    InitCommonControls();

    HWND hDlg = CreateDialog(g_hInst, (LPCTSTR)IDD_SIMPLESUBMIX, NULL, (DLGPROC)DialogMsgHandler );
    if (!hDlg)
    {
        return FALSE;
    }

    // Process the command line
    ProcessCommandLine( hDlg, lpCmdLine );

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDA_SIMPLESUBMIX);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//************************************************************************
// Name: DialogMsgHandler
// Desc: This handles all windows messages.
//************************************************************************
LRESULT CALLBACK DialogMsgHandler( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;

	switch (message) 
	{
		case WM_COMMAND:
			 wmId    = LOWORD(wParam); 
			 wmEvent = HIWORD(wParam); 
			 // Handle the buttons:
			 switch (wmId)
             {
				 case IDOK:
                 case IDCANCEL:
                 case IDABORT:
				      DestroyWindow( hDlg );
				      break;

                 case IDC_OPEN1:
                 case IDC_OPEN2:
                 case IDC_OPEN3:
                 case IDC_OPEN4:
                 case IDC_OPEN5:
                 case IDC_OPEN6:
                      OpenFileToPlay( hDlg, (wmId - IDC_OPEN1), NULL );
                      break;

                 case IDC_PLAY:
                      StartPlayingWave( hDlg );
                      break;

                 case IDC_STOP:
                      StopPlayingWave( hDlg );
                      break;
             }
			 break;

             //
             // The next 4  Messages are for the Orbiter only!
             //

        case WM_LBUTTONDOWN:
             if(g_pCOrbiter)
                g_pCOrbiter->SetMouseState( TRUE );
             return TRUE; // Message handled 
        
        case WM_MOUSEMOVE:
             if(g_pCOrbiter)
                g_pCOrbiter->OnMouseMovement( hDlg );
             return TRUE; // Message handled 
        
        case WM_LBUTTONUP:
             if(g_pCOrbiter)
                g_pCOrbiter->SetMouseState( FALSE );
             return TRUE; // Message handled

        case WM_TIMER:
             if(g_pCOrbiter)
             {
                g_pCOrbiter->OnMovementTimer( hDlg );
                CheckRandomWaves( hDlg );
             }
             return TRUE;

		case WM_DESTROY:
             HandleExit( hDlg );
			 PostQuitMessage(0);
			 break;

        case WM_INITDIALOG:
             InitializeDialog(hDlg);
             break;

		default:
			 return FALSE;
   }
   return 0;
}


//************************************************************************
// Name: InitializeDialog
// Desc: Setup the main dialog controls.
//************************************************************************
bool InitializeDialog( HWND hDlg )
{
    //
    // Initialize User Interface controls.
    //
    InitializeUI( hDlg );

    // Initialize some global variables.
    g_FxManagerId.FxManagerId = NULL;

    // Initialize the buffer data
    memset(g_Buffer, 0, sizeof(BUFFER)*MAX_FILES);

    DWORD i;
    for(i = 0;  i < MAX_FILES; i++)
    {
        g_Buffer[i].FileControl = IDC_FILE1 + i;
        g_Buffer[i].OpenControl = IDC_OPEN1 + i;

        g_Buffer[i].Looping = DSBPLAY_LOOPING;
    }

    //
    // Create the Orbiter class
    //
    g_pCOrbiter = new COrbiter();

    //
    // Create and Initialize the Sound Manager
    //

    // Create a static IDirectSound in the CSound class.  
    // Set coop level to DSSCL_PRIORITY, and set primary buffer 
    // format to stereo, 22kHz and 16-bit output.
    g_pSoundManager = new CSoundManager();
    if(g_pSoundManager == NULL)
    {
        DXTRACE_ERR( TEXT("Creation"), 0 );
        MessageBox( hDlg, "Error creating Sound manager.  Program will now exit.", 
                    "nVidia SimpleSubmix", MB_OK | MB_ICONERROR );
                     HandleExit( hDlg );
        DestroyWindow( hDlg );
        return FALSE;
    }

    HRESULT hr;
    if( FAILED( hr = g_pSoundManager->Initialize( hDlg, DSSCL_PRIORITY, 2, 22050, 16 ) ) )
    {
        DXTRACE_ERR( TEXT("Initialize"), hr );
        MessageBox( hDlg, "Error initializing DirectSound.  Program will now exit.", 
                    "nVidia SimpleSubmix", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    if( FAILED( hr = OpenHardwarePipe( hDlg ) ))
    {
        DXTRACE_ERR( TEXT("Initialize"), hr );
        MessageBox( hDlg, "Error initializing hardware pipe.  Program will now exit.", 
                    "nVidia SimpleSubmix", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    if(g_pCOrbiter)
    {
        g_pCOrbiter->Initialize( hDlg, g_FxManagerId, g_pKsPropSet, g_pDSListener );
    }

    return TRUE;
}


//************************************************************************
// Name: OpenHardwarePipe
// Desc: Open up a pipe to the hardware for use during entire duration.
//************************************************************************
HRESULT OpenHardwarePipe( HWND hDlg )
{
    HRESULT hr = DS_OK;

    //
    // Create a dummy 3D buffer, that is always present.
    // We will use the interface from this buffer to access the hardware.
    //
    static TCHAR strMsg[100];
    DSBUFFERDESC DSBufferDesc3DPath;
    DSBUFFERDESC * pDSBufferDesc3DPath = &DSBufferDesc3DPath;

    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = 1; 
    wfx.nSamplesPerSec  = 44100; 
    wfx.wBitsPerSample  = 16; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    memset(&DSBufferDesc3DPath, 0, sizeof(DSBUFFERDESC));

    DSBufferDesc3DPath.dwSize = sizeof(DSBUFFERDESC);
    DSBufferDesc3DPath.dwFlags = DSBCAPS_CTRL3D;
    DSBufferDesc3DPath.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
    DSBufferDesc3DPath.dwBufferBytes = DSBSIZE_MIN;
    DSBufferDesc3DPath.lpwfxFormat = (LPWAVEFORMATEX)&wfx;

    LPDIRECTSOUND lpDS  = g_pSoundManager->GetDirectSound();
    LPDIRECTSOUNDBUFFER8 lpDSB8 = NULL;

    //
    // Create the dummy buffer
    //
    hr = lpDS->CreateSoundBuffer(pDSBufferDesc3DPath, &g_pDSBPath, NULL);
    if(g_pDSBPath == NULL)
    {
        sprintf( strMsg, "Error: 0x%08x", hr );
        MessageBox( NULL, strMsg, "Buffer Creation Error", MB_OK | MB_ICONERROR);
        return hr;
    }

    //
    // Query the interface for the property set
    //
    if( FAILED( hr = g_pDSBPath->QueryInterface(IID_IKsPropertySet, (void**)&g_pKsPropSet)))
    {
        sprintf( strMsg, "Error: Can't Get KsPropertySet" );
        MessageBox( NULL, strMsg, "Query interface for property set error!", MB_OK | MB_ICONERROR);
        SAFE_RELEASE( g_pKsPropSet );
        return hr;
    }

    //
    // Get the 3D Listener interface pointer.
    //
    if( FAILED( hr = g_pSoundManager->Get3DListenerInterface( &g_pDSListener )))
    {
        MessageBox( NULL, TEXT("Could not get 3D listener interface."),  
            TEXT("QueryInterface Error"), MB_OK | MB_ICONERROR);
        return hr; 
    }

    //
    // Create the nVidia Local FxManager.
    //
    DWORD FxManFlags = 0;
    if(CreateFxMananger(g_pKsPropSet, FxManFlags, &g_FxManagerId) != DS_OK)
    {
        // We continue if this fails, we just wont be able to access the hardware.
        // DestroyWindow( hDlg );
        // return FALSE;
    }

    return hr;
}


//************************************************************************
// Name: OpenFileToPlay
// Desc: Prompt the user for a wave file to play.
//************************************************************************
bool OpenFileToPlay( HWND hDlg, DWORD FileNum, TCHAR * pFilename )
{
    static TCHAR strPath[MAX_PATH] = { TEXT("") };

    CloseBuffer( hDlg, FileNum );

    if(pFilename == NULL)
    {
        // Setup the OPENFILENAME structure
        OPENFILENAME ofn = { sizeof(OPENFILENAME), hDlg, NULL,
                             TEXT("Wave Files\0*.wav\0All Files\0*.*\0\0"), NULL,
                             0, 1, g_Buffer[FileNum].strFileName, MAX_PATH, NULL, 0, strPath,
                             TEXT("Open Sound File"),
                             OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, 0, 0,
                             TEXT(".wav"), 0, NULL, NULL };

        // Get the default media path (something like C:\WINDOWS\MEDIA)
        if( '\0' == strPath[0] )
        {
            GetWindowsDirectory( strPath, MAX_PATH );
            if( strcmp( &strPath[strlen(strPath)], TEXT("\\") ) )
                strcat( strPath, TEXT("\\") );
            strcat( strPath, TEXT("MEDIA") );
        }

        // Display the OpenFileName dialog. Then, try to load the specified file
        if( TRUE != GetOpenFileName( &ofn ) )
        {
            // Set the UI controls
            SetDlgItemText( hDlg, g_Buffer[FileNum].FileControl, TEXT("No File Selected") );
            return FALSE;
        }
    }
    else
    {
        _tcscpy( g_Buffer[FileNum].strFileName, pFilename );
    }

    SetDlgItemText( hDlg, g_Buffer[FileNum].FileControl, TEXT("") );

    // Make sure wave file is a valid wav file
    if( FAILED( ValidateWaveFile( hDlg, g_Buffer[FileNum].strFileName, g_Buffer[FileNum].FileControl ) ) )
    {
        // Set the UI controls
        SetDlgItemText( hDlg, g_Buffer[FileNum].FileControl, TEXT("") );
        return S_FALSE;
    }

    //
    // Initialize DS stuff for buffer (all buffers mono, 2D)
    //
    DWORD dwCreationFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY ;
    GUID Guid = GUID_NULL;
    HRESULT hr;
    if( FAILED( hr =  g_pSoundManager->Create( &g_Buffer[FileNum].pSound, g_Buffer[FileNum].strFileName, dwCreationFlags, Guid )))
    {
        MessageBox( hDlg, TEXT("Failure creating the SoundManager."), 
                    "nVidia SimpleSubmix", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    //
    // Get the property set interface to this sound.
    //
    if( FAILED( hr = g_Buffer[FileNum].pSound->GetKsPropSetInterface( 0, &g_Buffer[FileNum].pKsPropSet )))
    {
        MessageBox( hDlg, TEXT("Failure getting property set interface."), 
                    "nVidia SimpleSubmix", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    //
    // Everything opened and was setup properly, so set this flag.
    //
    g_Buffer[FileNum].Active = TRUE;

    //
    // If other buffers are already playing, lets attach this buffer right now.
    //
    AttachVoiceToPlayingSubmix( hDlg, FileNum );

    //
    // Enable the Play buffon unless something is already playing.
    //
    EnablePlayButton( hDlg, &g_Buffer[0] );

    return TRUE;
}


//************************************************************************
// Name: StartPlayingWave
// Desc: Begin playing all of the currently open wave files.
//************************************************************************
bool StartPlayingWave( HWND hDlg )
{
    HRESULT hr;
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), TRUE );

    if(!WireUpSubmix())
    {
        EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_STOP ), FALSE );
        return FALSE;
    }

    // Notify orbiter of submix ID so updates to harware will commence.
    g_pCOrbiter->SetSubmixId( g_FxIdSubmix );

    DWORD i;
    for(i = 0; i < MAX_FILES; i++)
    {
        if(g_Buffer[i].Active)
        {
            // Reset the sound to the beginning
            g_Buffer[i].pSound->Reset();

            // Disable the currently playing 'Open' button
            EnableWindow( GetDlgItem( hDlg, g_Buffer[i].OpenControl), FALSE );

            // Play the sound
            DWORD looping = g_Buffer[i].Looping;
            if( FAILED( hr = g_Buffer[i].pSound->Play( 0, looping ) ) )
            {
                DXTRACE_ERR( TEXT("Play"), hr );
                return FALSE;
            }

            // Update the UI controls to show the sound as playing
            g_Buffer[i].Playing = TRUE;
        }
    }


    return TRUE;
}

//************************************************************************
// Name: StopPlayingWave
// Desc: Stop playing all of the currently playing wave files.
//************************************************************************
bool StopPlayingWave( HWND hDlg )
{
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), TRUE );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), FALSE );

    DWORD i;
    for(i = 0; i < MAX_FILES; i++)
    {
        if(g_Buffer[i].Active)
        {
            if(g_Buffer[i].Playing)
            {
                g_Buffer[i].Playing = FALSE;

                // Enable the currently playing 'Open' button
                EnableWindow( GetDlgItem( hDlg, g_Buffer[i].OpenControl), TRUE );

                if( g_Buffer[i].pSound )
                {
                    g_Buffer[i].pSound->Stop();
                    g_Buffer[i].pSound->Reset();
                }
            }
        }
    }

    // Turn off orbiting updates
    g_pCOrbiter->SetSubmixId( 0 );

    // Destroy topology
    DestroySubmix();

    return TRUE;
}


//************************************************************************
// Name: CheckRandomWaves
// Desc: Check all non looping wave files, if orbiting, turn on randomly.
//************************************************************************
bool CheckRandomWaves( HWND hDlg )
{
    HWND hAutoMove = GetDlgItem( hDlg, IDC_ORBIT );
    long AutoMove = SendMessage( hAutoMove, BM_GETCHECK, 0, 0);
    DWORD i;

    if(AutoMove == BST_CHECKED)
    {
        for(i = 0; i < MAX_FILES; i++)
        {
            if(g_Buffer[i].Active && (g_Buffer[i].Looping != DSBPLAY_LOOPING))
            {
                if(!g_Buffer[i].pSound->IsSoundPlaying())
                {
                    g_Buffer[i].RandomCount++;
                    EnableWindow( GetDlgItem( hDlg, g_Buffer[i].OpenControl), TRUE );

                    if(g_Buffer[i].RandomRestart <= g_Buffer[i].RandomCount)
                    {
                        // Restart the buffer
                        // Reset the random count to a different number each time
                        // between 0 and 10 seconds.
                        g_Buffer[i].RandomCount = 0;
                        g_Buffer[i].RandomRestart = rand() % RANDOM_MAX_WAIT_PERIOD;
                        g_Buffer[i].pSound->Reset();
                        g_Buffer[i].Playing = TRUE;
                        g_Buffer[i].pSound->Play( 0, 0 );
            
                        EnableWindow( GetDlgItem( hDlg, g_Buffer[i].OpenControl), FALSE );
                    }
                }
                else
                {
                    g_Buffer[i].Playing = FALSE;
                    EnableWindow( GetDlgItem( hDlg, g_Buffer[i].OpenControl), FALSE );
                }
            }
        }
    }

    return TRUE;
}


//************************************************************************
// Name: CloseBuffer
// Desc: Release anything that was previously created.
//************************************************************************
bool CloseBuffer( HWND hDlg, DWORD FileNum )
{
    if(!g_Buffer[FileNum].Active)
        return TRUE;

    SAFE_RELEASE( g_Buffer[FileNum].pKsPropSet );
    SAFE_DELETE( g_Buffer[FileNum].pSound );

    g_Buffer[FileNum].Active = FALSE;

    return TRUE;
}


//************************************************************************
// Name: HandleExit
// Desc: This is the last function executed before exiting.
//************************************************************************
bool HandleExit( HWND hDlg )
{
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;

    //
    // Check if we are still playing anything, and stop if we are.
    //
    DWORD i;
    for(i = 0; i < MAX_FILES; i++)
    {
        if(g_Buffer[i].Playing)
        {
            // Stop any playing.
            StopPlayingWave( hDlg );
            break;
        }
    }

    //
    // Kill the Orbiter class
    //
    SAFE_DELETE( g_pCOrbiter );

    if( pFxMgrId->FxManagerId != NULL )
    {
        DestroyFxManager( g_pKsPropSet, pFxMgrId );
    }

    for(i = 0; i < MAX_FILES; i++)
    {
        SAFE_RELEASE( g_Buffer[i].pKsPropSet );
        SAFE_DELETE( g_Buffer[i].pSound );
    }

    SAFE_RELEASE( g_pKsPropSet );
    SAFE_RELEASE( g_pDSBPath );
    SAFE_DELETE( g_pSoundManager );
    return TRUE;
}



//************************************************************************
//************************************************************************
// Routines to Create and Destroy nVidia Submix Topologies.
//************************************************************************
//************************************************************************


//************************************************************************
// Name: WireUpSubmix
// Desc: Create the complete topology...Effects, Wires, Enables etc..
//
// Note: This routine will always create 1 Submix and attach all open 
//       voices to it. Then we enable the submix.
//
//
//************************************************************************
bool WireUpSubmix(  )
{
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;
    DWORD WireNum = 0;
    DWORD i;

    // Begin touching the hardware.

    SetBatchMode( g_pKsPropSet, pFxMgrId );

    // Add our SubMix, Make it a 3D submix.
    DWORD Flags = NVFXDATA_FXMANAGER_FXADD_SUBMIX_3D;
    AddEffect( g_pKsPropSet, pFxMgrId, &g_FxIdSubmix, 6, 1, GUID_NVFX_Submix, Flags );

    // Add a wire from each Voice to the SubMix
    for(i = 0; i < MAX_FILES; i++)
    {
        if(g_Buffer[i].Active && g_Buffer[i].pKsPropSet)
        {
            SetWire( g_FxCD[WireNum], NVFX_SOURCE_IS_VOICE, 4, g_FxIdSubmix, 0 );
            AttachWire( g_Buffer[i].pKsPropSet, pFxMgrId, &g_FxCD[WireNum++] );
        }
    }

    // Save the total number of wires that were sent to the hardware.
    g_NumberOfWires = WireNum;

    // Modify the submix's volume based on how many voices are attached to it.
    ModifySubmixVolume( WireNum );

    // Enable the Submix
    EnableEffect( g_pKsPropSet, pFxMgrId, g_FxIdSubmix);
       
    ResetBatchMode( g_pKsPropSet, &g_FxManagerId );
    return TRUE;
}


//************************************************************************
// Name: AttachVoiceToPlayingSubmix
// Desc: If we are already playing, add this Voice to the Submix and Play it.
//************************************************************************
bool AttachVoiceToPlayingSubmix( HWND hDlg, DWORD BufferNum )
{
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;

    DWORD i;
    bool bPlaying = FALSE;
    for(i = 0; i < MAX_FILES; i++)
    {
        if(g_Buffer[i].Playing)
        {
            bPlaying = TRUE;
            break;
        }
    }

    if(!bPlaying)
        return FALSE;

    DWORD WireNum = g_NumberOfWires;

    SetWire( g_FxCD[WireNum], NVFX_SOURCE_IS_VOICE, 4, g_FxIdSubmix, 0 );
    AttachWire( g_Buffer[BufferNum].pKsPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    
    g_NumberOfWires = WireNum;

    // Modify the submix's volume based on how many voices are attached to it.
    ModifySubmixVolume( WireNum );

    HRESULT hr;
    if( FAILED( hr = g_Buffer[BufferNum].pSound->Play( 0, DSBPLAY_LOOPING ) ) )
    {
        DXTRACE_ERR( TEXT("Play"), hr );
        return FALSE;
    }

    // Update the UI controls to show the sound as playing
    g_Buffer[BufferNum].Playing = TRUE;

    // Disable the currently playing 'Open' button
    EnableWindow( GetDlgItem( hDlg, g_Buffer[BufferNum].OpenControl), FALSE );

    return TRUE;

}


//************************************************************************
// Name: ModifySubmixVolume
// Desc: Modify the Submix's volume based on the number of voices wired up.
//************************************************************************
bool ModifySubmixVolume( DWORD VoicesWired )
{
    KSAUDIO_MIXCAP_TABLE    MixCapsTable;
    KSAUDIO_MIX_CAPS        MixCaps[MAX_MIXLEVELS];
    KSAUDIO_MIXLEVEL        MixLevels[MAX_MIXLEVELS];
    long Level = 0;
    bool Mute = FALSE;

    // Now lets get the current state of the MixLevels since we can update
    // them one at a time from the app, the driver will load all 8.
    GetMixLevelCaps( g_pKsPropSet, &g_FxManagerId, g_FxIdSubmix, &MixCapsTable, MixCaps );
    GetMixLevels( g_pKsPropSet, &g_FxManagerId, g_FxIdSubmix, &MixCapsTable, MixLevels );

    // Set the Levels to...
    // Leave the Mute flag alone for now.
    DWORD i;
    for(i = 0; i < MAX_MIXLEVELS; i++)
    {
        // Adjust the Level based on the number of Voices that are attached.
        // We decrease the Volume by 7% for each Voice added.
        long Range = MixCaps[i].Maximum - MixCaps[i].Minimum;
        long DecreaseBy = (Range * VoicesWired * PRECENT_DEC_VOL_PER_VOICE)/100;
        Level = MixCaps[i].Maximum - DecreaseBy;

        // Check Range
        if((MixCaps[i].Minimum > Level) || (MixCaps[i].Maximum < Level))
            return FALSE;

        // Set the data into the structure
        MixLevels[i].Mute = Mute;
        MixLevels[i].Level = Level;
    }

    // Update the hardware with the new levels
    SetMixLevels( g_pKsPropSet, &g_FxManagerId, g_FxIdSubmix, &MixCapsTable, MixLevels );

    return TRUE;
}


//************************************************************************
// Name: DestroySubmix
// Desc: Delete the entire topology (that we just created).
//************************************************************************
bool DestroySubmix(  )
{
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;

    // First we set batch mode.
    SetBatchMode( g_pKsPropSet, pFxMgrId );

    // Now we disable the Submix.
    DisableEffect( g_pKsPropSet, pFxMgrId, g_FxIdSubmix );

    // Now we could delete each wire, but its not required, as the 
    // driver will remove all wires before it deletes each effect.
    // However, we will do it anyway, because its good coding style.
    DWORD Wires;
    for(Wires = 0; Wires < g_NumberOfWires; Wires++)
    {
        DetachWire( g_pKsPropSet, pFxMgrId, &g_FxCD[Wires] );
    }

    // Delete the Submix.
    DeleteEffect( g_pKsPropSet, pFxMgrId, g_FxIdSubmix );

    // Reset batch mode
    ResetBatchMode( g_pKsPropSet, pFxMgrId );
    return TRUE;
}


//************************************************************************
// End of File: SimpleSubmix.cpp
//************************************************************************
