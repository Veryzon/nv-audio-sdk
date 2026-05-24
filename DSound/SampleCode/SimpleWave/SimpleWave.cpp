//************************************************************************
// SimpleWave.cpp : A simple program for playing a mono file to any
//                  of the 6 outputs (L, R, C, RL, RR, Sub).
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
#include "SimpleWave.h"
#include "HwInterface.h"

//
// Global Variables:
//
HINSTANCE               g_hInst;                 // Current instance
CSoundManager         * g_pSoundManager = NULL;  // Only one SoundManager for everyone

BUFFER                  g_Buffer;                // Buffer information (only 1).

//
// For method 2 usage.
//
NVFXINST_FXMANAGER_ID   g_FxManagerId;           // nVidia Fx Manager Id
NVFXDATA_FX_ID          g_FxIdVoice[6];          // 6 VoiceMixes that we attach to.
NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR g_FxCD[12];  // Structure of Wires(12 max)
DWORD                   g_NumberOfWires;         // Number of wires sent to hardware.



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

    HWND hDlg = CreateDialog(g_hInst, (LPCTSTR)IDD_SIMPLEWAVE, NULL, (DLGPROC)DialogMsgHandler );
    if (!hDlg)
    {
        return FALSE;
    }

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDA_SIMPLEWAVE);

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

                 case IDC_OPEN:
                      OpenFile( hDlg );
                      break;

                 case IDC_PLAY:
                      StartPlayingWave( hDlg );
                      break;

                 case IDC_STOP:
                      StopPlayingWave( hDlg );
                      break;
             }
			 break;

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
    memset(&g_Buffer, 0, sizeof(BUFFER));

    g_Buffer.Looping = DSBPLAY_LOOPING;

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
                    "nVidia SIMPLEWAVE", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    HRESULT hr;
    if( FAILED( hr = g_pSoundManager->Initialize( hDlg, DSSCL_PRIORITY, 2, 22050, 16 ) ) )
    {
        DXTRACE_ERR( TEXT("Initialize"), hr );
        MessageBox( hDlg, "Error initializing DirectSound.  Program will now exit.", 
                    "nVidia SIMPLEWAVE", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    return TRUE;
}


//************************************************************************
// Name: OpenFile
// Desc: Prompt the user for a wave file to play.
//************************************************************************
bool OpenFile( HWND hDlg )
{
    static TCHAR strPath[MAX_PATH] = { TEXT("") };

    CloseBuffer( hDlg );

    // Setup the OPENFILENAME structure
    OPENFILENAME ofn = { sizeof(OPENFILENAME), hDlg, NULL,
                         TEXT("Wave Files\0*.wav\0All Files\0*.*\0\0"), NULL,
                         0, 1, g_Buffer.strFileName, MAX_PATH, NULL, 0, strPath,
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
        SetDlgItemText( hDlg, IDC_FILE, TEXT("No File Selected") );
        return FALSE;
    }

    SetDlgItemText( hDlg, IDC_FILE, TEXT("") );

    // Make sure wave file is a valid wav file
    if( FAILED( ValidateMonoWaveFile( hDlg, g_Buffer.strFileName, IDC_FILE ) ) )
    {
        // Set the UI controls
        SetDlgItemText( hDlg, IDC_FILE, TEXT("") );
        return S_FALSE;
    }

    //
    // Everything opened and was setup properly, so set this flag.
    //
    g_Buffer.Active = TRUE;
    g_Buffer.Is3D = FALSE;

    //
    // Enable the Play buffon unless something is already playing.
    //
    EnablePlayButton( hDlg, &g_Buffer );

    return TRUE;
}


//************************************************************************
// Name: InitSoundFile
// Desc: We execute this before playing the file, if method 1 is selected
//       then we use the converted file instead of the actual file.
//************************************************************************
bool InitSoundFile( HWND hDlg, TCHAR *filename, DWORD Method )
{
    //
    // Destroy previous FxManager.
    // This should never execute on the first time into the application.
    //
    if((g_FxManagerId.FxManagerId != NULL) && (g_Buffer.pKsPropSet != NULL))
    {
        DestroyFxManager( g_Buffer.pKsPropSet, &g_FxManagerId );
    }

    //
    // Initialize DS stuff for buffer (all buffers mono, 2D)
    //
    DWORD dwCreationFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY ;
    GUID Guid = GUID_NULL;
    HRESULT hr;
    if( FAILED( hr =  g_pSoundManager->Create( &g_Buffer.pSound, filename, dwCreationFlags, Guid )))
    {
        MessageBox( hDlg, TEXT("Failure creating the SoundManager."), 
                    "nVidia SIMPLEWAVE", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    //
    // Get the property set interface to this sound.
    //
    if( FAILED( hr = g_Buffer.pSound->GetKsPropSetInterface( 0, &g_Buffer.pKsPropSet )))
    {
        MessageBox( hDlg, TEXT("Failure getting property set interface."), 
                    "nVidia SIMPLEWAVE", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    //
    // Create the nVidia Local FxManager.
    //
    g_FxManagerId.FxManagerId = NULL;
    if(Method == RUN_METHOD2)
    {
        DWORD FxManFlags = 0;
        if(CreateFxMananger(g_Buffer.pKsPropSet, FxManFlags, &g_FxManagerId) != DS_OK)
        {
            // We continue if this fails, we just wont be able to access the hardware.
            // DestroyWindow( hDlg );
            // return FALSE;
        }
    }

    //
    // Get the Direct Sound buffer
    //
    g_Buffer.pDSB = g_Buffer.pSound->GetBuffer( 0 );
    if( g_Buffer.pDSB == NULL )
    {
        MessageBox( hDlg, TEXT("Failure getting Direct Sound buffer."), 
                    "nVidia SIMPLEWAVE", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    return TRUE;
}


//************************************************************************
// Name: StartPlayingWave
// Desc: Begin playing all of the currently open wave files.
//************************************************************************
bool StartPlayingWave( HWND hDlg )
{
    // Gray out all buttons so the user cant change while we play.
    BeginPlayUI( hDlg, TRUE );

    // Setup the method we will use to play this buffer.
    if(!SetupPlayMethod( hDlg ))
    {
        BeginPlayUI( hDlg, FALSE );
        return FALSE;
    }

    if((g_Buffer.Active) && (g_Buffer.pSound))
    {
        // Reset the sound to the beginning
        g_Buffer.pSound->Reset();

        // Play the sound
        DWORD looping = g_Buffer.Looping;
        HRESULT hr;
        if( FAILED( hr = g_Buffer.pSound->Play( 0, looping ) ) )
        {
            DXTRACE_ERR( TEXT("Play"), hr );
            return FALSE;
        }

        // Update the UI controls to show the sound as playing
        g_Buffer.Playing = TRUE;
    }

    return TRUE;
}

//************************************************************************
// Name: StopPlayingWave
// Desc: Stop playing all of the currently playing wave files.
//************************************************************************
bool StopPlayingWave( HWND hDlg )
{
    BeginPlayUI( hDlg, FALSE );

    if((g_Buffer.Active) && (g_Buffer.Playing))
    {
        g_Buffer.Playing = FALSE;

        if( g_Buffer.pSound )
        {
            g_Buffer.pSound->Stop();
            g_Buffer.pSound->Reset();
        }
    }

    DestroyPlayMethod( hDlg );

    return TRUE;
}


//************************************************************************
// Name: CloseBuffer
// Desc: Release anything that was previously created.
//************************************************************************
bool CloseBuffer( HWND hDlg )
{
    if(!g_Buffer.Active)
        return TRUE;

    SAFE_RELEASE( g_Buffer.pKsPropSet );
    SAFE_DELETE( g_Buffer.pSound );

    g_Buffer.Active = FALSE;

    return TRUE;
}


//************************************************************************
// Name: HandleExit
// Desc: This is the last function executed before exiting.
//************************************************************************
bool HandleExit( HWND hDlg )
{
    //
    // Check if we are still playing, and stop if we are.
    //
    if(g_Buffer.Playing)
    {
        // Stop playing.
        StopPlayingWave( hDlg );
    }

    //
    // Kill the Fx Manager
    //
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;
    if( pFxMgrId->FxManagerId != NULL )
    {
        DestroyFxManager( g_Buffer.pKsPropSet, pFxMgrId );
    }

    SAFE_RELEASE( g_Buffer.pKsPropSet );
    SAFE_DELETE ( g_Buffer.pSound );
    SAFE_DELETE ( g_pSoundManager );
    return TRUE;
}


//************************************************************************
// Name: SetupPlayMethod
// Desc: Setup the file or hardware based on the method the user selects.
//************************************************************************
bool SetupPlayMethod( HWND hDlg )
{
    HWND hMethod1 = GetDlgItem( hDlg, IDC_METHOD1 );
    long method1 = SendMessage( hMethod1, BM_GETCHECK, 0, 0);

    if(method1 == BST_CHECKED)
    {
        //
        // Convert the current file to a temporary one that is
        // extensible with the appropriate channels selected.
        //
        ConvertMonoPCMtoMonoExtensible( hDlg, &g_Buffer );

        // 
        // Now the rest is just opening a normal wave file.
        //
        if(!InitSoundFile( hDlg, g_Buffer.strTempName, RUN_METHOD1 ))
        {
            return FALSE;
        }
    }
    else  // method 2
    {
        //
        // Open a normal wave file.
        //
        if(!InitSoundFile( hDlg, g_Buffer.strFileName, RUN_METHOD2 ))
        {
            return FALSE;
        }

        //
        // Wire the Voice to a VoiceMix, then to the appropriate
        // global outputs.
        //
        WireSoundToOutputs( hDlg );
    }

    return TRUE;
}


//************************************************************************
// Name: DestroyPlayMethod
// Desc: Undo what we did in SetupPlayMethod.
//************************************************************************
bool DestroyPlayMethod( HWND hDlg )
{
    HWND hMethod2 = GetDlgItem( hDlg, IDC_METHOD2 );
    long method2 = SendMessage( hMethod2, BM_GETCHECK, 0, 0);

    if(method2 == BST_CHECKED)
    {
        DestroyTopology( hDlg );
    }

    return TRUE;
}


//************************************************************************
//************************************************************************
// Routines to Create and Destroy nVidia Topologies.
//************************************************************************
//************************************************************************


//************************************************************************
// Name: WireSoundToOutputs
// Desc: Wire the voice to the final outputs based on the user selection.
//************************************************************************
bool WireSoundToOutputs( HWND hDlg )
{
    LPKSPROPERTYSET pPropSet = g_Buffer.pKsPropSet;
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;
    DWORD WireNum = 0;

    // Begin touching the hardware.

    SetBatchMode( pPropSet, pFxMgrId );

    //
    // Add the 6 VoiceMixes.
    //
    DWORD i;
    for(i = 0; i < 6; i++)
    {
        AddEffect( pPropSet, pFxMgrId, &g_FxIdVoice[i], 1, 1, GUID_NVFX_Mix, NVFXDATA_FXMANAGER_FXADD_MIX_VOICE );
    }

    // Add a wire from the Voice to each VoiceMix
    for(i = 0; i < 6; i++)
    {
        SetWire( g_FxCD[WireNum], NVFX_SOURCE_IS_VOICE, i, g_FxIdVoice[i], 0 );
        AttachWire( pPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    }

    // Read the status of the check boxes.
    long left      = SendMessage( GetDlgItem( hDlg, IDC_LEFT ), BM_GETCHECK, 0, 0);
    long right     = SendMessage( GetDlgItem( hDlg, IDC_RIGHT ), BM_GETCHECK, 0, 0);
    long center    = SendMessage( GetDlgItem( hDlg, IDC_CENTER ), BM_GETCHECK, 0, 0);
    long rearleft  = SendMessage( GetDlgItem( hDlg, IDC_REARLEFT ), BM_GETCHECK, 0, 0);
    long rearright = SendMessage( GetDlgItem( hDlg, IDC_REARRIGHT ), BM_GETCHECK, 0, 0);
    long subwoofer = SendMessage( GetDlgItem( hDlg, IDC_SUBWOOFER ), BM_GETCHECK, 0, 0);

    if(left == BST_CHECKED)
    {
        // Add a wire from the VoiceMix to the Left Final Output
        SetWire( g_FxCD[WireNum], g_FxIdVoice[0], 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_LEFT );
        AttachWire( pPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    }

    if(right == BST_CHECKED)
    {
        // Add a wire from the VoiceMix to the Right Final Output
        SetWire( g_FxCD[WireNum], g_FxIdVoice[1], 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_RIGHT );
        AttachWire( pPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    }

    if(center == BST_CHECKED)
    {
        // Add a wire from the VoiceMix to the Center Final Output
        SetWire( g_FxCD[WireNum], g_FxIdVoice[2], 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_CENTER );
        AttachWire( pPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    }

    if(rearleft == BST_CHECKED)
    {
        // Add a wire from the VoiceMix to the Rear Left Final Output
        SetWire( g_FxCD[WireNum], g_FxIdVoice[3], 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_REAR_LEFT );
        AttachWire( pPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    }

    if(rearright == BST_CHECKED)
    {
        // Add a wire from the VoiceMix to the Rear Right Final Output
        SetWire( g_FxCD[WireNum], g_FxIdVoice[4], 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_REAR_RIGHT );
        AttachWire( pPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    }

    if(subwoofer == BST_CHECKED)
    {
        // Add a wire from the VoiceMix to the Subwoofer Final Output
        SetWire( g_FxCD[WireNum], g_FxIdVoice[5], 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_LFE );
        AttachWire( pPropSet, pFxMgrId, &g_FxCD[WireNum++] );
    }

    // Save the total number of wires that were sent to the hardware.
    g_NumberOfWires = WireNum;

    // Enable the Voice
    for(i = 0; i < 6; i++)
    {
        EnableEffect( pPropSet, pFxMgrId, g_FxIdVoice[i] );
    }

    ResetBatchMode( pPropSet, &g_FxManagerId );

    return TRUE;
}


//************************************************************************
// Name: DestroyTopology
// Desc: Delete the entire topology (that we just created).
//************************************************************************
bool DestroyTopology( HWND hDlg )
{
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;
    LPKSPROPERTYSET pPropSet = g_Buffer.pKsPropSet;

    // First we set batch mode.
    SetBatchMode( pPropSet, pFxMgrId );

    // Now we disable the VoiceMixes.
    DWORD i;
    for(i = 0; i < 6; i++)
    {
        DisableEffect( pPropSet, pFxMgrId, g_FxIdVoice[i] );
    }

    // Now we could delete each wire, but its not required, as the 
    // driver will remove all wires before it deletes each effect.
    // However, we will do it anyway, because its good coding style.
    DWORD Wires;
    for(Wires = 0; Wires < g_NumberOfWires; Wires++)
    {
        DetachWire( pPropSet, pFxMgrId, &g_FxCD[Wires] );
    }

    // Delete the VoiceMixes.
    for(i = 0; i < 6; i++)
    {
        DeleteEffect( pPropSet, pFxMgrId, g_FxIdVoice[i] );
    }

    // Reset batch mode
    ResetBatchMode( pPropSet, pFxMgrId );
    return TRUE;
}


//************************************************************************
// End of File: SimpleWave.cpp
//************************************************************************
