//************************************************************************
// SimpleFxUI.cpp : This is the User Interface (UI) code.
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
#include "SimpleFx.h"

extern HINSTANCE g_hInst;
extern int       g_SelectedParam;
extern int       g_SelectedFx;
extern PARAMETER g_ParmCtrl[MAX_PARAMS_PER_EFFECT];
extern LPVOID    g_pParams[MAX_EFFECTS];
extern bool      g_Playing;


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
    // Setup the ListView column information
    //
    HWND hListView = GetDlgItem( hDlg, IDC_LISTVIEW );

    LV_COLUMN column;

    RECT LVRect;
    GetWindowRect( hListView, &LVRect );

    DWORD ColumnSize = ((LVRect.right - LVRect.left)/2) - 10;

    column.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    column.fmt = LVCFMT_LEFT;
    column.cchTextMax = 0;  // Not used for insert
    column.iImage = 0;      // not currently used
    column.iOrder = 0;      // not currently used
    column.cx = ColumnSize; // Column size
    column.iSubItem = 0;    // Column number

    column.pszText = "Parameter";
    ListView_InsertColumn( hListView, column.iSubItem, &column );

    column.iSubItem++;
    column.pszText = "Value";

    ListView_InsertColumn( hListView, column.iSubItem, &column );

    //
    // Setup the effects dropdown list.
    //
    DWORD i;
    HWND hComboBox = GetDlgItem( hDlg, IDC_COMBO );

    for(i = 0; i < MAX_EFFECTS; i++)
    {
        SendMessage( hComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)m_FxNames[i] ) ;
    }

    // wParam is the selection, we set it to zero.
    SendMessage( hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0 ) ;

    //
    // Disable the Play/Stop buttons and the ListView and Slider controls.
    //
    EnableWindow( GetDlgItem( hDlg, IDC_PLAY ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_STOP ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_LISTVIEW ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_SLIDER ), FALSE );
    EnableWindow( GetDlgItem( hDlg, IDC_COMBO ), FALSE );

    //
    // Initialize some internal flags.
    //
    g_Playing = FALSE;
    g_SelectedFx = 0;

    return TRUE;
}


//************************************************************************
// Name: ValidateWaveFile
// Desc: Open the wave file with the helper class to make sure it's valid.
//************************************************************************
HRESULT ValidateWaveFile( HWND hDlg, char* strFileName )
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
            MessageBox( hDlg, TEXT("Wave file must be PCM for effects control."), 
                              TEXT("ValidateWaveFile Error"), MB_OK);
            SetDlgItemText( hDlg, IDC_EDIT_FILENAME, TEXT("") );
            return E_FAIL;
        }
  
        // Update the UI controls to show the sound as the file is loaded
        waveFile.Close();
        SetDlgItemText( hDlg, IDC_EDIT_FILENAME, strFileName );
        return S_OK;
    }
}


//************************************************************************
// Name: UpdateParameterControls
// Desc: Handle the UI for the selected effect.
//************************************************************************
bool UpdateParameterControls( HWND hDlg, DWORD Fx )
{
    // Enable the ListView and Slider
    EnableWindow( GetDlgItem( hDlg, IDC_LISTVIEW ), TRUE );
    EnableWindow( GetDlgItem( hDlg, IDC_SLIDER ), TRUE );

    // Setup Slider parameters for this effect
    InitializeSliderValues( Fx );

    // Write the parameter names to the ListView (Column 1)
    WriteListViewColumn1( hDlg, Fx );

    // Format and Write the paramater data to the ListView (Column 2)
    WriteListViewColumn2( hDlg, Fx );

    // Setup and Write the Slider.
    AssembleSlider( hDlg, Fx );

    return TRUE;
}



//************************************************************************
// Name: WriteListViewColumn1
// Desc: Write all of the parameter name fields for selected effect.
//************************************************************************
bool WriteListViewColumn1( HWND hDlg, DWORD EffectSelected )
{
    DWORD i;
    LV_ITEM item;

    HWND hListView = GetDlgItem( hDlg, IDC_LISTVIEW );

    // Clear out the ListView (all columns and rows)
    ListView_DeleteAllItems( hListView );

    item.state = LVIS_SELECTED;
    item.stateMask = LVIS_SELECTED;
    item.cchTextMax = 0;  // Not used when setting the value
    item.iImage = 0;
    item.lParam = 0;
    item.iIndent = 0;
    item.mask = LVIF_TEXT | LVIF_STATE;
    item.iSubItem = 0;    // First Column 

    char * pName = m_EParams[EffectSelected];

    for(i = 0; i < m_NumOfParms[EffectSelected]; i++)
    {
        item.iItem = i;
        item.pszText = pName;

        ListView_InsertItem( hListView, &item );

        pName += MAX_PARAMS_NAME_SIZE;
    }

    return TRUE;
}

//************************************************************************
// Name: WriteListViewColumn2
// Desc: Write all of the parameter data and units for selected effect.
//************************************************************************
bool WriteListViewColumn2( HWND hDlg, DWORD EffectSelected )
{
    DWORD i;
    LV_ITEM item;
    TCHAR string[256];

    HWND hListView = GetDlgItem( hDlg, IDC_LISTVIEW );

    DWORD * pDrvParms = (DWORD *)g_pParams[EffectSelected];
    char * pUnits = m_EUnits[EffectSelected];

    item.state = LVIS_SELECTED;
    item.stateMask = LVIS_SELECTED;
    item.cchTextMax = 0;  // Not used when setting the value
    item.iImage = 0;
    item.lParam = 0;
    item.iIndent = 0;
    item.mask = LVIF_TEXT;
    item.iSubItem = 1;    // Second Column 

    for(i = 0; i < m_NumOfParms[EffectSelected]; i++)
    {
        ProcessData( i, pDrvParms, pUnits, string );

        item.pszText = string;
        item.iItem = i;
        ListView_SetItem( hListView, &item );

        pDrvParms++;
        pUnits += MAX_PARAMS_UNIT_SIZE;
    }

    return TRUE;
}


//************************************************************************
// Name: ProcessData
// Desc: Process the current selected parameter's data.
//************************************************************************
bool ProcessData( DWORD ParamSelected, DWORD * pData, TCHAR * pUnits, TCHAR * string )
{
    DWORD dwValue;
    FLOAT fpValue;
    bool DataIsFloat;

    // For an item with a NULL unit (NULL1, 2, 3 or 4) we must do a value lookup
    bool IsItANullIndex = (_tcscmp(pUnits, IDS_STRING_UNIT_NULL1) == 0) ||
                          (_tcscmp(pUnits, IDS_STRING_UNIT_NULL2) == 0) ||
                          (_tcscmp(pUnits, IDS_STRING_UNIT_NULL3) == 0) ||
                          (_tcscmp(pUnits, IDS_STRING_UNIT_NULL4) == 0) ;

    bool IsItAnOddball = (_tcscmp(pUnits,IDS_STRING_UNIT_MB) == 0) ||
                         (_tcscmp(pUnits,IDS_STRING_UNIT_RATE) == 0) ||
                         (_tcscmp(pUnits,IDS_STRING_UNIT_NULL0) == 0);

    if(IsItANullIndex || IsItAnOddball)
    {
        dwValue = (DWORD)*pData;
        fpValue = 0.0f;
        DataIsFloat = FALSE;
    }
    else
    {
        dwValue = 0;
        fpValue = *((float *)pData);
        DataIsFloat = TRUE;
    }

    if(IsItANullIndex)
    {
        DWORD NullIndex = 0;
        NullIndex = (_tcscmp(pUnits, "NULL1") == 0) ? 0 : NullIndex;
        NullIndex = (_tcscmp(pUnits, "NULL2") == 0) ? 1 : NullIndex;
        NullIndex = (_tcscmp(pUnits, "NULL3") == 0) ? 2 : NullIndex;
        NullIndex = (_tcscmp(pUnits, "NULL4") == 0) ? 3 : NullIndex;

        char * pLookup = m_ELookup[NullIndex];
        pLookup += dwValue * MAX_PARAMS_UNIT_SIZE;
        _stprintf( string, "%s", pLookup );
    }
    else
    {
        TCHAR strTemp[256];
        _stprintf(strTemp, TEXT("%s %s"), g_ParmCtrl[ParamSelected].Format, "%s" );

        if(DataIsFloat)
        {
            _stprintf(string, strTemp, fpValue, pUnits);
        }
        else
        {
            _stprintf(string, strTemp, dwValue, pUnits);
        }
    }

    return TRUE;
}


//************************************************************************
// Name: AssembleEditSlider
// Desc: Setup the slider (by position) on the GlobalEq Edit dialog
//************************************************************************
bool AssembleSlider( HWND hDlg, DWORD EffectSelected )
{
    // First we get our parameter's slider information.
    PPARAMETER pParm = &g_ParmCtrl[0];
    pParm += g_SelectedParam;  // Index into selected parameter

    // Next we get the parameter data.
    DWORD * pDrvParms = (DWORD *)g_pParams[EffectSelected];
    pDrvParms += g_SelectedParam;

    // Set the range and position of the slider.
    HWND hSlider = GetDlgItem( hDlg, IDC_SLIDER );
    SendMessage( hSlider, TBM_SETTICFREQ,  (WPARAM)pParm->SliderTic, (LPARAM)0 );
    SendMessage( hSlider, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)pParm->SliderMax );
    SendMessage( hSlider, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)pParm->SliderMin );

    // Now we process the parameter data and send it to the slider.
    long Data;
    DWORD AreWeFloatingPoint = (m_ParamConv[EffectSelected][g_SelectedParam] == 1) ? TRUE : FALSE;
    if(AreWeFloatingPoint)
    {
        Data = (long)((*(float *)pDrvParms) * pParm->fpConstant);
    }
    else
    {
        Data = *pDrvParms;
    }

    SendMessage( hSlider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)Data );
    return TRUE;
}


//************************************************************************
// Name: GetSliderData
// Desc: Process slider movements.
//************************************************************************
bool GetSliderData( HWND hDlg )
{
    DWORD Fx = g_SelectedFx;
    DWORD Parm = g_SelectedParam;

    // Get the position of the slider.
    HWND hSlider = GetDlgItem( hDlg, IDC_SLIDER );
    long SliderValue = SendMessage( hSlider, TBM_GETPOS, (WPARAM)0, (LPARAM)0 );

    // Get a pointer to where the new data will be saved in the parameter structure.
    DWORD * pDrvParms = (DWORD *)g_pParams[Fx];
    pDrvParms += Parm;

    // Save the new data into the parameter structure.
    DWORD AreWeFloatingPoint = (m_ParamConv[Fx][Parm] == 1) ? TRUE : FALSE;
    if(AreWeFloatingPoint)
    {
        PPARAMETER pParm = &g_ParmCtrl[Parm];
        *(FLOAT*)pDrvParms = ((FLOAT)SliderValue)/pParm->fpConstant;
    }
    else
    {
        *pDrvParms = SliderValue;
    }

    // Get the correct units for the ListView display, column 2.
    char * pUnits = m_EUnits[Fx];
    pUnits += Parm * MAX_PARAMS_UNIT_SIZE;

    // Process the data returned from the slider.
    TCHAR string[256];
    ProcessData( Parm, pDrvParms, pUnits, string );

    // Update the ListView  entry (column 2).
    HWND hListView = GetDlgItem( hDlg, IDC_LISTVIEW );
    LV_ITEM item;
    item.state = LVIS_SELECTED;
    item.stateMask = LVIS_SELECTED;
    item.cchTextMax = 0;  // Not used when setting the value
    item.iImage = 0;
    item.lParam = 0;
    item.iIndent = 0;
    item.mask = LVIF_TEXT;
    item.iSubItem = 1;    // Second Column 
    item.pszText = string;
    item.iItem = Parm;
    ListView_SetItem( hListView, &item );

    return TRUE;
}



//************************************************************************
// Name: InitializeSliderValues
// Desc: Setup slider ranges, TicFreq, Data Formats etc...
//************************************************************************
bool InitializeSliderValues( DWORD EffectSelected )
{
    PPARAMETER pParm = &g_ParmCtrl[0];

    switch(EffectSelected)
    {
        case FX_CHORUS:
             pParm->SliderMax = (long)DSFXCHORUS_WETDRYMIX_MAX;
             pParm->SliderMin = (long)DSFXCHORUS_WETDRYMIX_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXCHORUS_DEPTH_MAX;
             pParm->SliderMin = (long)DSFXCHORUS_DEPTH_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXCHORUS_FEEDBACK_MAX;
             pParm->SliderMin = (long)DSFXCHORUS_FEEDBACK_MIN;
             pParm->SliderTic = 20;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXCHORUS_FREQUENCY_MAX * 10.0f);
             pParm->SliderMin = (long)(DSFXCHORUS_FREQUENCY_MIN * 10.0f);
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 10.0f;
             pParm++;
             pParm->SliderMax = DSFXCHORUS_WAVE_SIN;
             pParm->SliderMin = DSFXCHORUS_WAVE_TRIANGLE;
             pParm->SliderTic = 1;
             memcpy(pParm->Format, FORMAT_NotUsed, sizeof(FORMAT_NotUsed));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXCHORUS_DELAY_MAX * 5.0f);
             pParm->SliderMin = (long)(DSFXCHORUS_DELAY_MIN * 5.0f);
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_2point2Float, sizeof(FORMAT_2point2Float));
             pParm->fpConstant = 5.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXCHORUS_PHASE_180;
             pParm->SliderMin = (long)DSFXCHORUS_PHASE_NEG_180;
             pParm->SliderTic = 1;
             memcpy(pParm->Format, FORMAT_NotUsed, sizeof(FORMAT_NotUsed));
             pParm->fpConstant = 1.0f;
             break;

        case FX_COMPRESSOR:
             pParm->SliderMax = (long)DSFXCOMPRESSOR_GAIN_MAX;
             pParm->SliderMin = (long)DSFXCOMPRESSOR_GAIN_MIN;
             pParm->SliderTic = 12;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXCOMPRESSOR_ATTACK_MAX * 100.0f);
             pParm->SliderMin = (long)(DSFXCOMPRESSOR_ATTACK_MIN * 100.0f);
             pParm->SliderTic = 5000;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 100.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXCOMPRESSOR_RELEASE_MAX;
             pParm->SliderMin = (long)DSFXCOMPRESSOR_RELEASE_MIN;
             pParm->SliderTic = 295;
             memcpy(pParm->Format, FORMAT_4point1Float, sizeof(FORMAT_4point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXCOMPRESSOR_THRESHOLD_MAX * 10.0f);
             pParm->SliderMin = (long)(DSFXCOMPRESSOR_THRESHOLD_MIN * 10.0f);
             pParm->SliderTic = 60;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 10.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXCOMPRESSOR_RATIO_MAX;
             pParm->SliderMin = (long)DSFXCOMPRESSOR_RATIO_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXCOMPRESSOR_PREDELAY_MAX * 25.0f);
             pParm->SliderMin = (long)(DSFXCOMPRESSOR_PREDELAY_MIN * 25.0f);
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_2point2Float, sizeof(FORMAT_2point2Float));
             pParm->fpConstant = 25.0f;
             pParm++;
             break;

        case FX_DISTORTION:
             pParm->SliderMax = (long)DSFXDISTORTION_GAIN_MAX;
             pParm->SliderMin = (long)DSFXDISTORTION_GAIN_MIN;
             pParm->SliderTic = 6;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXDISTORTION_EDGE_MAX;
             pParm->SliderMin = (long)DSFXDISTORTION_EDGE_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX;
             pParm->SliderMin = (long)DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN;
             pParm->SliderTic = 790;
             memcpy(pParm->Format, FORMAT_4point1Float, sizeof(FORMAT_4point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXDISTORTION_POSTEQBANDWIDTH_MAX;
             pParm->SliderMin = (long)DSFXDISTORTION_POSTEQBANDWIDTH_MIN;
             pParm->SliderTic = 790;
             memcpy(pParm->Format, FORMAT_4point1Float, sizeof(FORMAT_4point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXDISTORTION_PRELOWPASSCUTOFF_MAX;
             pParm->SliderMin = (long)DSFXDISTORTION_PRELOWPASSCUTOFF_MIN;
             pParm->SliderTic = 790;
             memcpy(pParm->Format, FORMAT_4point1Float, sizeof(FORMAT_4point1Float));
             pParm->fpConstant = 1.0f;
             break;

        case FX_ECHO:
             pParm->SliderMax = (long)DSFXECHO_WETDRYMIX_MAX;
             pParm->SliderMin = (long)DSFXECHO_WETDRYMIX_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXECHO_FEEDBACK_MAX;
             pParm->SliderMin = (long)DSFXECHO_FEEDBACK_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXECHO_LEFTDELAY_MAX;
             pParm->SliderMin = (long)DSFXECHO_LEFTDELAY_MIN;
             pParm->SliderTic = 200;
             memcpy(pParm->Format, FORMAT_4point1Float, sizeof(FORMAT_4point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXECHO_RIGHTDELAY_MAX;
             pParm->SliderMin = (long)DSFXECHO_RIGHTDELAY_MIN;
             pParm->SliderTic = 200;
             memcpy(pParm->Format, FORMAT_4point1Float, sizeof(FORMAT_4point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXECHO_PANDELAY_MAX;
             pParm->SliderMin = (long)DSFXECHO_PANDELAY_MIN;
             pParm->SliderTic = 1;
             memcpy(pParm->Format, FORMAT_2point2Float, sizeof(FORMAT_2point2Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             break;

        case FX_FLANGER:
             pParm->SliderMax = (long)DSFXFLANGER_WETDRYMIX_MAX;
             pParm->SliderMin = (long)DSFXFLANGER_WETDRYMIX_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXFLANGER_DEPTH_MAX;
             pParm->SliderMin = (long)DSFXFLANGER_DEPTH_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXFLANGER_FEEDBACK_MAX;
             pParm->SliderMin = (long)DSFXFLANGER_FEEDBACK_MIN;
             pParm->SliderTic = 20;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXFLANGER_FREQUENCY_MAX * 10.0f);
             pParm->SliderMin = (long)(DSFXFLANGER_FREQUENCY_MIN * 10.0f);
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_2point2Float, sizeof(FORMAT_2point2Float));
             pParm->fpConstant = 10.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXFLANGER_WAVE_SIN;
             pParm->SliderMin = (long)DSFXFLANGER_WAVE_TRIANGLE;
             pParm->SliderTic = 1;
             memcpy(pParm->Format, FORMAT_NotUsed, sizeof(FORMAT_NotUsed));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXFLANGER_DELAY_MAX * 25.0f);
             pParm->SliderMin = (long)(DSFXFLANGER_DELAY_MIN * 25.0f);
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_2point2Float, sizeof(FORMAT_2point2Float));
             pParm->fpConstant = 25.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXFLANGER_PHASE_180;
             pParm->SliderMin = (long)DSFXFLANGER_PHASE_NEG_180;
             pParm->SliderTic = 1;
             memcpy(pParm->Format, FORMAT_NotUsed, sizeof(FORMAT_NotUsed));
             pParm->fpConstant = 1.0f;
             pParm++;
             break;

        case FX_GARGLE:
             pParm->SliderMax = (long)DSFXGARGLE_RATEHZ_MAX;
             pParm->SliderMin = (long)DSFXGARGLE_RATEHZ_MIN;
             pParm->SliderTic = 100;
             memcpy(pParm->Format, FORMAT_DWORD, sizeof(FORMAT_DWORD));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFXGARGLE_WAVE_SQUARE;
             pParm->SliderMin = (long)DSFXGARGLE_WAVE_TRIANGLE;
             pParm->SliderTic = 1;
             memcpy(pParm->Format, FORMAT_NotUsed, sizeof(FORMAT_NotUsed));
             pParm->fpConstant = 1.0f;
             pParm++;
             break;

        case FX_PARAMEQ:
             pParm->SliderMax = (long)DSFXPARAMEQ_CENTER_MAX;
             pParm->SliderMin = (long)DSFXPARAMEQ_CENTER_MIN;
             pParm->SliderTic = 1592;
             memcpy(pParm->Format, FORMAT_4point1Float, sizeof(FORMAT_4point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXPARAMEQ_BANDWIDTH_MAX * 10.0f);
             pParm->SliderMin = (long)(DSFXPARAMEQ_BANDWIDTH_MIN * 10.0f);
             pParm->SliderTic = 36;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 10.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFXPARAMEQ_GAIN_MAX * 10.0f);
             pParm->SliderMin = (long)(DSFXPARAMEQ_GAIN_MIN * 10.0f);
             pParm->SliderTic = 30;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 10.0f;
             pParm++;
             break;

        case FX_REVERB:
             pParm->SliderMax = (long)DSFX_I3DL2REVERB_ROOM_MAX;
             pParm->SliderMin = (long)DSFX_I3DL2REVERB_ROOM_MIN;
             pParm->SliderTic = 1000;
             memcpy(pParm->Format, FORMAT_DWORD, sizeof(FORMAT_DWORD));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFX_I3DL2REVERB_ROOMHF_MAX;
             pParm->SliderMin = (long)DSFX_I3DL2REVERB_ROOMHF_MIN;
             pParm->SliderTic = 1000;
             memcpy(pParm->Format, FORMAT_DWORD, sizeof(FORMAT_DWORD));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MAX * 10.0f);
             pParm->SliderMin = (long)(DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN * 10.0f);
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 10.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFX_I3DL2REVERB_DECAYTIME_MAX * 10.0f);
             pParm->SliderMin = (long)(DSFX_I3DL2REVERB_DECAYTIME_MIN * 10.0f);
             pParm->SliderTic = 20;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 10.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFX_I3DL2REVERB_DECAYHFRATIO_MAX * 100.0f);
             pParm->SliderMin = (long)(DSFX_I3DL2REVERB_DECAYHFRATIO_MIN * 100.0f);
             pParm->SliderTic = 20;
             memcpy(pParm->Format, FORMAT_2point2Float, sizeof(FORMAT_2point2Float));
             pParm->fpConstant = 100.0f;
             pParm++;
             pParm->SliderMax = (long)DSFX_I3DL2REVERB_REFLECTIONS_MAX;
             pParm->SliderMin = (long)DSFX_I3DL2REVERB_REFLECTIONS_MIN;
             pParm->SliderTic = 1000;
             memcpy(pParm->Format, FORMAT_DWORD, sizeof(FORMAT_DWORD));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFX_I3DL2REVERB_REFLECTIONSDELAY_MAX * 1000.0f);
             pParm->SliderMin = (long)(DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN * 1000.0f);
             pParm->SliderTic = 30;
             memcpy(pParm->Format, FORMAT_1point3Float, sizeof(FORMAT_1point3Float));
             pParm->fpConstant = 1000.0f;
             pParm++;
             pParm->SliderMax = (long)DSFX_I3DL2REVERB_REVERB_MAX;
             pParm->SliderMin = (long)DSFX_I3DL2REVERB_REVERB_MIN;
             pParm->SliderTic = 1200;
             memcpy(pParm->Format, FORMAT_DWORD, sizeof(FORMAT_DWORD));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)(DSFX_I3DL2REVERB_REVERBDELAY_MAX * 1000.0f);
             pParm->SliderMin = (long)(DSFX_I3DL2REVERB_REVERBDELAY_MIN * 1000.0f);
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_1point3Float, sizeof(FORMAT_1point3Float));
             pParm->fpConstant = 1000.0f;
             pParm++;
             pParm->SliderMax = (long)DSFX_I3DL2REVERB_DIFFUSION_MAX;
             pParm->SliderMin = (long)DSFX_I3DL2REVERB_DIFFUSION_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFX_I3DL2REVERB_DENSITY_MAX;
             pParm->SliderMin = (long)DSFX_I3DL2REVERB_DENSITY_MIN;
             pParm->SliderTic = 10;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             pParm++;
             pParm->SliderMax = (long)DSFX_I3DL2REVERB_HFREFERENCE_MAX;
             pParm->SliderMin = (long)DSFX_I3DL2REVERB_HFREFERENCE_MIN;
             pParm->SliderTic = 1998;
             memcpy(pParm->Format, FORMAT_3point1Float, sizeof(FORMAT_3point1Float));
             pParm->fpConstant = 1.0f;
             break;
    }

    return TRUE;
}


//************************************************************************
// End of File: SimpleFxUI.cpp
//************************************************************************
