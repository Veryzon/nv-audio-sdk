//************************************************************************
// SimpleFx.cpp : A simple program for experimenting with nVidia 
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
#include "SimpleFx.h"
#include "SimpleFxData.h"
#include "HwInterface.h"

//
// Global Variables:
//
HINSTANCE               g_hInst;                 // Current instance
CSoundManager         * g_pSoundManager = NULL;  // Only one SoundManager for everyone
CSound                * g_pSound;                // Only one sound open at a time
NVFXINST_FXMANAGER_ID   g_FxManagerId;           // nVidia Fx Manager Id
LPKSPROPERTYSET         g_pKsPropertySet;        // Property set path to DirectSound
int                     g_SelectedParam;         // Currently selected parameter
int                     g_SelectedFx;            // Currently selected effect
int                     g_PreviousEffect;        // Previous effect, for removal.
bool                    g_Playing;               // TRUE if sound is playing

NVFXDATA_FX_ID          g_FxId;                  // The effect we add (only one at a time)
NVFXDATA_FX_ID          g_FxIdVoice;             // A VoiceMix that we attach to.
NVFXDATA_FX_ID          g_FxIdSplitter;          // A splitter, we only use one.
NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR g_FxCD[6];   // Structure of Wires (we wire 5 or 6 wires max)
DWORD                   g_NumberOfWires;         // Number of wires sent to hardware.
PARAMETER               g_ParmCtrl[MAX_PARAMS_PER_EFFECT]; // Data to setup slider for each parameter


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

    HWND hDlg = CreateDialog(g_hInst, (LPCTSTR)IDD_SIMPLEFX, NULL, (DLGPROC)DialogMsgHandler );
    if (!hDlg)
    {
        return FALSE;
    }


	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDA_SIMPLEFX);

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
                     OpenFileToPlay( hDlg );
                     break;

                case IDC_PLAY:
                     StartPlayingWave( hDlg );
                     break;

                case IDC_STOP:
                     StopPlayingWave( hDlg );
                     break;

                case IDC_COMBO:
                     if( wmEvent == 0x0001 )
                     {
                         HandleEffectChange( hDlg );
                     }
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

        case WM_HSCROLL:
             HandleSlider( hDlg, lParam );
             break;

        case WM_NOTIFY:
             HandleListviewSelect( hDlg, lParam );
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
    g_pKsPropertySet = 0;
    g_PreviousEffect = FX_NOP;

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
                    "nVidia SimpleFx", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    HRESULT hr;
    if( FAILED( hr = g_pSoundManager->Initialize( hDlg, DSSCL_PRIORITY, 2, 22050, 16 ) ) )
    {
        DXTRACE_ERR( TEXT("Initialize"), hr );
        MessageBox( hDlg, "Error initializing DirectSound.  Program will now exit.", 
                    "nVidia SimpleFx", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    //
    // Initialize the Guid arrays.
    //
    m_FxGuid[0] = GUID_NULL;
    m_FxGuid[1] = GUID_DSFX_STANDARD_CHORUS;
    m_FxGuid[2] = GUID_DSFX_STANDARD_COMPRESSOR;
    m_FxGuid[3] = GUID_DSFX_STANDARD_DISTORTION;
    m_FxGuid[4] = GUID_DSFX_STANDARD_ECHO;
    m_FxGuid[5] = GUID_DSFX_STANDARD_FLANGER;
    m_FxGuid[6] = GUID_DSFX_STANDARD_GARGLE;
    m_FxGuid[7] = GUID_DSFX_STANDARD_PARAMEQ;
    m_FxGuid[8] = GUID_NVFX_Reverb;

    m_DataGuid[0] = GUID_NULL;
    m_DataGuid[1] = GUID_DSFX_STANDARD_CHORUS;
    m_DataGuid[2] = GUID_DSFX_STANDARD_COMPRESSOR;
    m_DataGuid[3] = GUID_DSFX_STANDARD_DISTORTION;
    m_DataGuid[4] = GUID_DSFX_STANDARD_ECHO;
    m_DataGuid[5] = GUID_DSFX_STANDARD_FLANGER;
    m_DataGuid[6] = GUID_DSFX_STANDARD_GARGLE;
    m_DataGuid[7] = GUID_DSFX_STANDARD_PARAMEQ;
    m_DataGuid[8] = GUID_DSFX_STANDARD_I3DL2REVERB;

    return TRUE;
}

//************************************************************************
// Name: OpenFileToPlay
// Desc: Prompt the user for a wave file to play.
//************************************************************************
bool OpenFileToPlay( HWND hDlg )
{
    static TCHAR strFileName[MAX_PATH] = { TEXT("") };
    static TCHAR strPath[MAX_PATH] = { TEXT("") };

    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), FALSE );

    // Setup the OPENFILENAME structure
    OPENFILENAME ofn = { sizeof(OPENFILENAME), hDlg, NULL,
                         TEXT("Wave Files\0*.wav\0All Files\0*.*\0\0"), NULL,
                         0, 1, strFileName, MAX_PATH, NULL, 0, strPath,
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
        SetDlgItemText( hDlg, IDC_EDIT_FILENAME, TEXT("No File Selected") );
        return FALSE;
    }

    SetDlgItemText( hDlg, IDC_EDIT_FILENAME, TEXT("") );

    // Make sure wave file is a valid wav file
    if( FAILED( ValidateWaveFile( hDlg, strFileName ) ) )
    {
        // Set the UI controls
        SetDlgItemText( hDlg, IDC_EDIT_FILENAME, TEXT("") );
        return S_FALSE;
    }

    //
    // Destroy previous FxManager and effects, if any are created.
    // This should never execute on the first time into the application.
    //
    if((g_FxManagerId.FxManagerId != NULL) && (g_pKsPropertySet != NULL))
    {
        DestroyEffects( g_SelectedFx );
        DestroyFxManager( g_pKsPropertySet, &g_FxManagerId );
    }

    //
    // Initialize DS stuff for buffer
    //
    DWORD dwCreationFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY ;
    GUID Guid = GUID_NULL;
    HRESULT hr;
    if( FAILED( hr =  g_pSoundManager->Create( &g_pSound, strFileName, dwCreationFlags, Guid )))
    {
        MessageBox( hDlg, TEXT("Failure creating the SoundManager."), 
                    "nVidia SimpleFx", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    //
    // Get the property set interface to the sound.
    //
    if( FAILED( hr = g_pSound->GetKsPropSetInterface( 0, &g_pKsPropertySet )))
    {
        MessageBox( hDlg, TEXT("Failure getting property set interface."), 
                    "nVidia SimpleFx", MB_OK | MB_ICONERROR );
        DestroyWindow( hDlg );
        return FALSE;
    }

    //
    // Create the nVidia Local FxManager.
    //
    DWORD FxManFlags = 0;
    if(CreateFxMananger(g_pKsPropertySet, FxManFlags, &g_FxManagerId) != DS_OK)
    {
        // We continue if this fails, we just wont be able to access the hardware.
        // DestroyWindow( hDlg );
        // return FALSE;
    }

    // We select the 'No Effect" case.
    g_SelectedFx = FX_NOP;
    g_PreviousEffect = FX_NOP;

    // Set the ComboBox to 'No Effects"
    HWND hComboBox = GetDlgItem( hDlg, IDC_COMBO );
    SendMessage( hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0 ) ;

    // Clear out the ListView (all columns and rows)
    HWND hListView = GetDlgItem( hDlg, IDC_LISTVIEW );
    ListView_DeleteAllItems( hListView );

    // Everything is OK, we can play now.
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), TRUE );
    EnableWindow( GetDlgItem( hDlg, IDC_LISTVIEW ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_SLIDER ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_COMBO ), TRUE );

    return TRUE;
}

//************************************************************************
// Name: StartPlayingWave
// Desc: Begin playing the currently open wave file.
//************************************************************************
bool StartPlayingWave( HWND hDlg )
{
    HRESULT hr;
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), TRUE );

    g_pSound->Reset();

    // Play the sound
    if( FAILED( hr = g_pSound->Play( 0, DSBPLAY_LOOPING ) ) )
    {
        DXTRACE_ERR( TEXT("Play"), hr );
        return FALSE;
    }

    // Update the UI controls to show the sound as playing
    g_Playing = TRUE;

    // We can no longer change the effect type, until 'stop' is selected.
    EnableWindow( GetDlgItem( hDlg, IDC_COMBO ), FALSE );

    // We cannot open a new file while one is playing
    EnableWindow( GetDlgItem( hDlg, IDC_OPEN ), FALSE );

    return TRUE;
}

//************************************************************************
// Name: StopPlayingWave
// Desc: Stop playing the current wave file.
//************************************************************************
bool StopPlayingWave( HWND hDlg )
{
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), TRUE );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_COMBO ), TRUE );
    EnableWindow( GetDlgItem( hDlg, IDC_OPEN ), TRUE );

    if(g_Playing)
    {
        g_Playing = FALSE;

        if( g_pSound )
        {
            g_pSound->Stop();
            g_pSound->Reset();
        }
    }
    return TRUE;
}

//************************************************************************
// Name: HandleSlider
// Desc: Process all slider movement. Update parameters screen and hw.
//************************************************************************
bool HandleSlider( HWND hDlg, LPARAM lParam )
{
    // Get Slider Position
    HWND hSlider = GetDlgItem( hDlg, IDC_SLIDER );
    DWORD Data = SendMessage( hSlider, TBM_GETPOS, (WPARAM)0, (LPARAM)0 );

    // Get Selected Effect
    HWND hComboBox = GetDlgItem( hDlg, IDC_COMBO );
    DWORD Fx = SendMessage( hComboBox, CB_GETCURSEL, (WPARAM)0, (LPARAM)0 ) ;

    // Get the data from the slider.
    GetSliderData( hDlg );

    // Update the hardware
    NVFXINST_FXPROPERTY_DESCRIPTOR FxPropDesc;
    FxPropDesc.FxInstance.FxManagerId = g_FxManagerId.FxManagerId;
    FxPropDesc.FxInstance.FxId = g_FxId;
    FxPropDesc.PropertyId = m_DataGuid[Fx];

    SetEffectsParameters( g_pKsPropertySet, &FxPropDesc, m_FxSize[Fx], g_pParams[Fx] );

    return TRUE;
}

//************************************************************************
// Name: HandleEffectChange
// Desc: When the user changes the effect, we execute this code.
//************************************************************************
bool HandleEffectChange( HWND hDlg )
{
    // Get Selected Effect from Combo box.
    HWND hComboBox = GetDlgItem( hDlg, IDC_COMBO );
    DWORD Fx = SendMessage( hComboBox, CB_GETCURSEL, (WPARAM)0, (LPARAM)0 );

    // Setup the slider for this effect (parameter 0)
    g_SelectedParam = 0;
    g_SelectedFx = Fx;

    // Delete any existing effect we previously created.
    DestroyEffects( g_PreviousEffect );

    // Remember this effect, to simplify deletion if we reenter here.
    g_PreviousEffect = Fx;

    if(Fx == 0)  // The 'No Effects' case.
    {
        // Clear out the ListView (all columns and rows)
        HWND hListView = GetDlgItem( hDlg, IDC_LISTVIEW );
        ListView_DeleteAllItems( hListView );
        return TRUE;
    }

    // Wire up the effect in hardware (one is selected if we are here)
    WireUpEffect( Fx );

    // Read the parameter data from the driver
    NVFXINST_FXPROPERTY_DESCRIPTOR FxPropDesc;
    FxPropDesc.FxInstance.FxManagerId = g_FxManagerId.FxManagerId;
    FxPropDesc.FxInstance.FxId = g_FxId;
    FxPropDesc.PropertyId = m_DataGuid[Fx];

    GetEffectsParameters( g_pKsPropertySet, &FxPropDesc, m_FxSize[Fx], g_pParams[Fx] );

    // Update the ListView and Slider
    UpdateParameterControls( hDlg, Fx );

    return TRUE;
}

//************************************************************************
// Name: HandleListviewSelect
// Desc: When the user select a different parameter we execute this code.
//************************************************************************
bool HandleListviewSelect( HWND hDlg, LPARAM lParam )
{
    HWND hListView = GetDlgItem( hDlg, IDC_LISTVIEW );
    NM_LISTVIEW* pNM = (NM_LISTVIEW *)lParam;

    if((pNM->hdr.code == NM_CLICK) ||
       (pNM->hdr.code == NM_DBLCLK) ||
       (pNM->hdr.code == NM_RCLICK))
    {
        long ItemNum = pNM->iItem;
        long SubItemNum = pNM->iSubItem;

        if((SubItemNum == 0) &&  // Column 1
           (ItemNum > -1))
        {
            // Use ItemNum as selected effect
            g_SelectedParam = ItemNum;

            // Update the slider
            AssembleSlider( hDlg, g_SelectedFx );
        }
    }

    return TRUE;
}

//************************************************************************
// Name: HandleExit
// Desc: This is the last function executed before exiting.
//************************************************************************
bool HandleExit( HWND hDlg )
{
    LPKSPROPERTYSET pPropPath = g_pKsPropertySet;
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;

    if( pFxMgrId->FxManagerId != NULL )
    {
        DestroyFxManager( pPropPath, pFxMgrId );
    }

    SAFE_RELEASE( pPropPath );
    SAFE_DELETE( g_pSound );
    SAFE_DELETE( g_pSoundManager );
    return TRUE;
}



//************************************************************************
//************************************************************************
// Routines to Create and Destroy nVidia Effects Topologies.
//************************************************************************
//************************************************************************


//************************************************************************
// Name: WireUpEffect
// Desc: Create the complete topology...Effects, Wires, Enables etc..
//
// Note: This routine will always create 1 VoiceMix, 1 Effect, 1 Splitter.
//       VoiceMix input is attached to the Voice's output.
//       For Simplicity we are just using the Mono sound.
//
//       For a mono effect, VoiceMix is attached to the effect and
//                          the splitter is after the effect.
//       For a stereo effect, VoiceMix is attached to the Splitter and
//                          the Splitter is attached to the effect.
//
//       Both methods drive the left and right final outputs.
//
//************************************************************************
bool WireUpEffect( DWORD EffectSelected )
{
    LPKSPROPERTYSET pPropPath = g_pKsPropertySet;
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;

    DWORD EffectType = m_FxType[EffectSelected];
    DWORD IO = EffectType == FX_STEREO ? 2 : 1;
    GUID  Guid = m_FxGuid[EffectSelected];
    DWORD WireNum = 0;

    if(EffectSelected == FX_NOP)
        return TRUE;

    // Begin touching the hardware.

    SetBatchMode( pPropPath, pFxMgrId );

    // Add the VoiceMix
    AddEffect( pPropPath, pFxMgrId, &g_FxIdVoice, 1, 1, GUID_NVFX_Mix, NVFXDATA_FXMANAGER_FXADD_MIX_VOICE );

    if(EffectType == FX_MONO)
    {
        // Note: You must add the effects in the correct data flow order,
        //       otherwise there is no guarentee that the effect will work.

        // Add our Effect
        AddEffect( pPropPath, pFxMgrId, &g_FxId, IO, IO, Guid, 0 );

        // Add a Splitter
        AddEffect( pPropPath, pFxMgrId, &g_FxIdSplitter, 1, 2, GUID_NVFX_Mix, 0 );

        // Add a wire from the Voice to the VoiceMix
        SetWire( g_FxCD[WireNum], NVFX_SOURCE_IS_VOICE, 4, g_FxIdVoice, 0 );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from the VoiceMix to our Effect
        SetWire( g_FxCD[WireNum], g_FxIdVoice, 0, g_FxId, 0 );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from our Effect to a Splitter
        SetWire( g_FxCD[WireNum], g_FxId, 0, g_FxIdSplitter, 0 );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from the Splitter to the Left Final Output
        SetWire( g_FxCD[WireNum], g_FxIdSplitter, 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_LEFT );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from the Splitter to the Right Final Output
        SetWire( g_FxCD[WireNum], g_FxIdSplitter, 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_RIGHT );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );
    }

    else if(EffectType == FX_STEREO)
    {
        // Note: You must add the effects in the correct data flow order,
        //       otherwise there is no guarentee that the effect will work.

        // Add a Splitter
        AddEffect( pPropPath, pFxMgrId, &g_FxIdSplitter, 1, 2, GUID_NVFX_Mix, 0 );

        // Add our Effect
        AddEffect( pPropPath, pFxMgrId, &g_FxId, IO, IO, Guid, 0 );

        // Add a wire from the Voice to the VoiceMix
        SetWire( g_FxCD[WireNum], NVFX_SOURCE_IS_VOICE, 4, g_FxIdVoice, 0 );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from the VoiceMix to a Splitter
        SetWire( g_FxCD[WireNum], g_FxIdVoice, 0, g_FxIdSplitter, 0 );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from the Splitter to our Effect (Left input)
        SetWire( g_FxCD[WireNum], g_FxIdSplitter, 0, g_FxId, 0 );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from the Splitter to our Effect (Right input)
        SetWire( g_FxCD[WireNum], g_FxIdSplitter, 0, g_FxId, 1 );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from our Effect (Left output) to the Left Final Output
        SetWire( g_FxCD[WireNum], g_FxId, 0, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_LEFT );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );

        // Add a wire from our Effect (Right output) to the Right Final Output
        SetWire( g_FxCD[WireNum], g_FxId, 1, NVFX_DESTINATION_IS_FINAL_MIX, NVA_FX_PIN_GLOBAL_MIX_RIGHT );
        AttachWire( pPropPath, pFxMgrId, &g_FxCD[WireNum++] );
    }

    // Save the total number of wires that were sent to the hardware.
    g_NumberOfWires = WireNum;


    // Enable All added effects.

    // Enable the Voice
    EnableEffect( pPropPath, pFxMgrId, g_FxIdVoice );

    // Enable the Effect
    EnableEffect( pPropPath, pFxMgrId, g_FxId);
       
    // Enable the Splitter
    EnableEffect( pPropPath, pFxMgrId, g_FxIdSplitter );

    ResetBatchMode( g_pKsPropertySet, &g_FxManagerId );
    return TRUE;
}

//************************************************************************
// Name: DestroyEffects
// Desc: Delete the entire topology (that we just created).
//************************************************************************
bool DestroyEffects( DWORD Effect )
{
    LPKSPROPERTYSET pPropPath = g_pKsPropertySet;
    PNVFXINST_FXMANAGER_ID pFxMgrId = &g_FxManagerId;

    // If nothing was setup previously, skip this routine.
    if(Effect == FX_NOP)
        return TRUE;

    // First we set batch mode.
    SetBatchMode( pPropPath, pFxMgrId );

    // Now we disable each effect (that we previously enabled)
    DisableEffect( pPropPath, pFxMgrId, g_FxIdSplitter );
    DisableEffect( pPropPath, pFxMgrId, g_FxId );
    DisableEffect( pPropPath, pFxMgrId, g_FxIdVoice );

    // Now we could delete each wire, but its not required, as the 
    // driver will remove all wires before it deletes each effect.
    // However, we will do it anyway, because its good coding style.
    DWORD Wires;
    for(Wires = 0; Wires < g_NumberOfWires; Wires++)
    {
        DetachWire( pPropPath, pFxMgrId, &g_FxCD[Wires] );
    }

    // Delete each effect (in reverse order of creation, for performance reasons)

    DWORD EffectType = m_FxType[Effect];

    if(EffectType == FX_MONO)
    {
        DeleteEffect( pPropPath, pFxMgrId, g_FxIdSplitter );
        DeleteEffect( pPropPath, pFxMgrId, g_FxId );
        DeleteEffect( pPropPath, pFxMgrId, g_FxIdVoice );
    }
    else if(EffectType == FX_STEREO)
    {
        DeleteEffect( pPropPath, pFxMgrId, g_FxId );
        DeleteEffect( pPropPath, pFxMgrId, g_FxIdSplitter );
        DeleteEffect( pPropPath, pFxMgrId, g_FxIdVoice );
    }

    // Reset batch mode
    ResetBatchMode( pPropPath, pFxMgrId );
    return TRUE;
}


//************************************************************************
// End of File: SimpleFx.cpp
//************************************************************************
