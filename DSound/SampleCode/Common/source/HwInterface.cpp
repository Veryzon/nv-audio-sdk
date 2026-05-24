//************************************************************************
//
// HwInterface.cpp
//
// This file contains routines used to talk to the driver (hardware).
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
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <ks.h>
#include <ksmedia.h>

#include "nvfxapi.h"
#include "HwInterface.h"

bool g_NvidiaHwAvail; // FALSE if failed to open Nvidia h/w, 
                      // so the program will continue to function properly.

//************************************************************************
// Name: CreateFxMananger
// Desc: Setup the global or local effects manager
//************************************************************************
HRESULT CreateFxMananger
(
    LPKSPROPERTYSET            pKsPropertySet,
    DWORD                      FxManagerType,
    PNVFXINST_FXMANAGER_ID     pFxManagerId
)
{
    NVFXINST_FXMANAGER_ID FxManagerId = { 0 };
    DWORD RetVal = 0;
    HRESULT hr = DS_OK;
    g_NvidiaHwAvail = FALSE;

    const DWORD Size = sizeof(NVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR) + sizeof(NVFX_LOCAL_MIX_DESCRIPTOR);
    BYTE pdata[Size];
    memset(pdata, 0, sizeof(NVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR) + sizeof(NVFX_LOCAL_MIX_DESCRIPTOR));

    PNVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR pdescriptor = (PNVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR)pdata;

    pdescriptor->Flags = FxManagerType;
    pdescriptor->NumberOfLocalMixes = 2;
    pdescriptor->LocalMixDescriptor[0].Inputs = 1;
    pdescriptor->LocalMixDescriptor[0].Outputs = 1;
    pdescriptor->LocalMixDescriptor[0].LocalMixToGlobalMixMap = NVA_FX_GLOBAL_MIX_MAP_LEFT;

    pdescriptor->LocalMixDescriptor[1].Inputs = 1;
    pdescriptor->LocalMixDescriptor[1].Outputs = 1;
    pdescriptor->LocalMixDescriptor[1].LocalMixToGlobalMixMap = NVA_FX_GLOBAL_MIX_MAP_RIGHT;

    if(SUCCEEDED (hr = pKsPropertySet->Get(NVPROPSETID_NvAudioFxManager_DirectSound, 
                        NVFXPROPERTY_FXMANAGER_ALLOCATE, 
                        pdescriptor, Size, 
                        &FxManagerId, sizeof(NVFXINST_FXMANAGER_ID),
                        &RetVal) ))
    {
        if(RetVal == sizeof(NVFXINST_FXMANAGER_ID))
        {
            // Save the FxManager ID
            pFxManagerId->FxManagerId = FxManagerId.FxManagerId;
            g_NvidiaHwAvail = TRUE;
        }
        else
        {
            pFxManagerId->FxManagerId = NULL;
    
            MessageBox( NULL, "Incorrect size returned during FXMANAGER_ALLOCATE!", 
                              "FxManager Error", MB_OK | MB_ICONERROR );
            hr = DSERR_GENERIC;
        }
    }
    else
    {
        MessageBox( NULL, "Creation of NVIDIA FxManager failed!\nIf running on NVIDIA Audio H/W, make sure H/W acceleration is set to full in control panel!",
                          "FxManager Error", MB_OK | MB_ICONERROR );

        pFxManagerId->FxManagerId = NULL;
    }

    return hr;
}


//************************************************************************
// Name: DestroyFxManager
// Desc: A routine to destroy the global or local FxManager in the driver.
//************************************************************************
HRESULT DestroyFxManager
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId
)
{
    if( pFxManagerId->FxManagerId )
    {
        if(g_NvidiaHwAvail)
        {
            pKsPropertySet->Set(NVPROPSETID_NvAudioFxManager_DirectSound, 
                                NVFXPROPERTY_FXMANAGER_DEALLOCATE, 
                                pFxManagerId, sizeof(NVFXINST_FXMANAGER_ID), 0,0);
        }

        pFxManagerId->FxManagerId = NULL;
    }
    else
    {
        MessageBox( NULL, "Destruction of NVIDIA FxManager failed.",
                          "FxManager Error", MB_OK | MB_ICONERROR );
    }

    return DS_OK;
}


//************************************************************************
// Name: AddEffect
// Desc: Add an effect to the topology
//************************************************************************
HRESULT AddEffect
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    PNVFXDATA_FX_ID            pFxId, 
    DWORD                      Inputs, 
    DWORD                      Outputs, 
    GUID                       Guid, 
    DWORD                      Flags
)
{
    HRESULT hr = DS_OK;
    DWORD RetVal = sizeof(NVFXDATA_FX_ID);

    NVFXDATA_FX_ID fxId = 0;

    if(!pFxManagerId->FxManagerId)
    {
        *pFxId = (NVFXDATA_FX_ID)(-1);
        return S_FALSE;
    }

    NVFXINST_FXMANAGER_FXADD_DESCRIPTOR fxDesc;
    fxDesc.Type = Guid;  
    fxDesc.Flags = Flags;
    fxDesc.FxManagerId = pFxManagerId->FxManagerId; 
    fxDesc.Inputs = Inputs;
    fxDesc.Outputs = Outputs;
            
    if(g_NvidiaHwAvail)
    {
        // Add Effect...Must be a Get because it returns useful data.
        hr = pKsPropertySet->Get(NVPROPSETID_NvAudioFxManager_DirectSound, 
                        NVFXPROPERTY_FXMANAGER_FXADD, 
                        &fxDesc, sizeof(NVFXINST_FXMANAGER_FXADD_DESCRIPTOR),
                        &fxId, sizeof(NVFXDATA_FX_ID), &RetVal);

        if(!((hr == DS_OK) && (RetVal == sizeof(NVFXDATA_FX_ID))))
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_FXADD failed!",
                              "Add Effect Failure", MB_OK | MB_ICONERROR );
        }
    }

    *pFxId = fxId;

    return hr;
}


//************************************************************************
// Name: AttachWire
// Desc: Attach a wire to the topology.
//************************************************************************
HRESULT AttachWire
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    PNVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR pFxCD
)
{
    HRESULT hr = DS_OK;

    if(!pFxManagerId->FxManagerId)
    {
        return S_FALSE;
    }

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFxManager_DirectSound, 
                        NVFXPROPERTY_FXMANAGER_ATTACH, 
                        pFxManagerId, sizeof(NVFXINST_FXMANAGER_ID),
                        pFxCD, sizeof(NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR));

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_ATTACH failed!",
                              "Attach Wire Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: EnableEffect
// Desc: Enable an effect on the topology.
//************************************************************************
HRESULT EnableEffect
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    NVFXDATA_FX_ID             FxId
)
{
    HRESULT hr = DS_OK;

    if(!pFxManagerId->FxManagerId || !FxId)
    {
        return S_FALSE;
    }

    // Enable Audio Path, no more effects can be added now
    NVFXDATA_FX_ENABLE FxEnable = TRUE;
    NVFXINST_FX_DESCRIPTOR FxDesc;

    FxDesc.FxManagerId = pFxManagerId->FxManagerId;
    FxDesc.FxId = FxId;

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFx_DirectSound, 
                        NVFXPROPERTY_FX_ENABLE, 
                        &FxDesc, sizeof(NVFXINST_FX_DESCRIPTOR),
                        &FxEnable, sizeof(NVFXDATA_FX_ENABLE));

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_ENABLE (enable) failed!",
                              "Enable Effect Failure", MB_OK | MB_ICONERROR );       
        }
    }

    return hr;
}


//************************************************************************
// Name: DisableEffect
// Desc: Disable and effect on the topology.
//************************************************************************
HRESULT DisableEffect
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    NVFXDATA_FX_ID             FxId
)
{
    HRESULT hr = DS_OK;

    if(!pFxManagerId->FxManagerId || !FxId)
    {
        return S_FALSE;
    }

    // Disable Audio Path, effects can now be added.
    NVFXDATA_FX_ENABLE FxEnable = FALSE;
    NVFXINST_FX_DESCRIPTOR FxDesc;

    FxDesc.FxManagerId = pFxManagerId->FxManagerId;
    FxDesc.FxId = FxId;

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFx_DirectSound, 
                        NVFXPROPERTY_FX_ENABLE, 
                        &FxDesc, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                        &FxEnable, sizeof(NVFXDATA_FX_ENABLE));

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_ENABLE (disable) failed!",
                              "Disable Effect Failure", MB_OK | MB_ICONERROR );       
        }
    }

    return hr;
}


//************************************************************************
// Name: DetachWire
// Desc: Detach a wire in the topology
//************************************************************************
HRESULT DetachWire
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    PNVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR pFxCD
)
{
    HRESULT hr = DS_OK;

    if(!pFxManagerId->FxManagerId)
    {
        return S_FALSE;
    }

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFxManager_DirectSound, 
                        NVFXPROPERTY_FXMANAGER_DETACH, 
                        pFxManagerId, sizeof(NVFXINST_FXMANAGER_ID),
                        pFxCD, sizeof(NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR));

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_DETACH failed!",
                              "Detach Wire Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: DeleteEffect
// Desc: Delete and effect from the topology.
//************************************************************************
HRESULT DeleteEffect
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    NVFXDATA_FX_ID             FxId
)
{
    HRESULT hr = DS_OK;

    if(!pFxManagerId->FxManagerId || !FxId)
    {
        return S_FALSE;
    }

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFxManager_DirectSound, 
                        NVFXPROPERTY_FXMANAGER_FXDELETE, 
                        pFxManagerId, sizeof(NVFXINST_FXMANAGER_ID),
                        &FxId, sizeof(NVFXDATA_FX_ID));

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_FXDELETE failed!",
                              "Delete Effect Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: SetBatchMode
// Desc: Set batch mode in the driver
//************************************************************************
HRESULT SetBatchMode
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId
)
{
    NVFXDATA_FXMANAGER_BATCH BatchState = 0xdead;
    HRESULT hr = DS_OK;

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFxManager_DirectSound, 
                        NVFXPROPERTY_FXMANAGER_BATCH_MODE, 
                        pFxManagerId, sizeof(NVFXINST_FXMANAGER_ID),
                        &BatchState, sizeof(NVFXDATA_FXMANAGER_BATCH));

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_BATCH_MODE (set) failed!",
                              "Set Batch Mode Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: ResetBatchMode
// Desc: Reset batch mode in the driver
//************************************************************************
HRESULT ResetBatchMode
(
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId
)
{
    NVFXDATA_FXMANAGER_BATCH BatchState = 0;
    HRESULT hr = DS_OK;

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFxManager_DirectSound, 
                        NVFXPROPERTY_FXMANAGER_BATCH_MODE, 
                        pFxManagerId, sizeof(NVFXINST_FXMANAGER_ID),
                        &BatchState, sizeof(NVFXDATA_FXMANAGER_BATCH));

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FXMANAGER_BATCH_MODE (reset) failed!",
                              "Reset Batch Mode Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: SetEffectsParameters
// Desc: Set parameters for a given effect.
//************************************************************************
HRESULT SetEffectsParameters
( 
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXPROPERTY_DESCRIPTOR pFxPropDesc,
    DWORD                      dwParamSize,
    LPVOID                     pParams
)
{
    HRESULT hr = DS_OK;

    if(!pParams || !pFxPropDesc->FxInstance.FxManagerId || !pFxPropDesc->FxInstance.FxId)
    {
        return S_FALSE;
    }

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFx_DirectSound, 
                        NVFXPROPERTY_FX_FXPROPERTY, 
                        pFxPropDesc, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                        pParams, dwParamSize);

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FX_FXPROPERTY (Set) failed.",
                              "Set Effects Parameters Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: GetEffectsParameters
// Desc: Get parameters for a given effect.
//************************************************************************
HRESULT GetEffectsParameters
( 
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXPROPERTY_DESCRIPTOR pFxPropDesc,
    DWORD                      dwParamSize,
    LPVOID                     pParams
)
{
    DWORD RetVal;
    HRESULT hr = DS_OK;

    if(!pParams || !pFxPropDesc->FxInstance.FxManagerId || !pFxPropDesc->FxInstance.FxId)
    {
        return S_FALSE;
    }

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Get( NVPROPSETID_NvAudioFx_DirectSound, 
                        NVFXPROPERTY_FX_FXPROPERTY, 
                        pFxPropDesc, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                        pParams, dwParamSize, &RetVal);

        if((RetVal != dwParamSize) || (hr != DS_OK))
        {
            MessageBox( NULL, "Property NVFXPROPERTY_FX_FXPROPERTY (Get) failed.",
                              "Get Effects Parameters Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: GetMixLevelCaps
// Desc: This routine will query the hardware, which will fill in the 
//       KSAUDIO_MIXCAP_TABLE structure. Call this as often as you would like.
//       Data is saved to m_MixCapsTable in the class. 
//       User should call this at least once before mucking with the volume levels.
//************************************************************************
HRESULT GetMixLevelCaps
( 
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    NVFXDATA_FX_ID             FxSubmixId,

    PKSAUDIO_MIXCAP_TABLE      pMixCapsTable,
    PKSAUDIO_MIX_CAPS          pMixCaps
)
{
    HRESULT status = E_FAIL;
    DWORD RetVal = 0;

    // Dont call this if the Submix has not been created yet.
    if(( FxSubmixId == 0 ) || ( pFxManagerId->FxManagerId == 0 ))
        return S_OK;

    NVFXINST_FXPROPERTY_DESCRIPTOR MixCapsParams;
        
    DWORD MixCapsData[2] = { 0, 0 };

    MixCapsParams.FxInstance.FxManagerId = pFxManagerId->FxManagerId;
    MixCapsParams.FxInstance.FxId = FxSubmixId;
    MixCapsParams.PropertyId = KSPROPSETID_Audio;
    MixCapsParams.Property = KSPROPERTY_AUDIO_MIX_LEVEL_CAPS;

    // Get the data from the hardware now
    if(pKsPropertySet)
    {
        status = pKsPropertySet->Get(NVPROPSETID_NvAudioFx_DirectSound, NVFXPROPERTY_FX_FXPROPERTY, 
                          &MixCapsParams, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                          &MixCapsData, sizeof(DWORD)*2, &RetVal );
    }

    LPVOID pMixCapsData = 0;

    if( status == DS_OK )
    {
        // Store the pertinent data to the class
        pMixCapsTable->InputChannels = MixCapsData[0];
        pMixCapsTable->OutputChannels = MixCapsData[1];

        DWORD dwMixcapSizeSecondPass = sizeof(KSAUDIO_MIXCAP_TABLE) - 
                                       sizeof(KSAUDIO_MIX_CAPS *) +
                                       ( pMixCapsTable->InputChannels * 
                                         pMixCapsTable->OutputChannels * 
                                         sizeof(KSAUDIO_MIX_CAPS) );

        pMixCapsData = GlobalAlloc( GPTR, dwMixcapSizeSecondPass );

        if(pMixCapsData == NULL)
        {
            MessageBox( NULL, TEXT("Error: Mixcaps Params Structure Creation Failed"), 
                              TEXT("GetMixCapsLevels Error"), MB_OK | MB_ICONERROR );
            return E_FAIL;
        }

        ZeroMemory( pMixCapsData, dwMixcapSizeSecondPass );

        status = pKsPropertySet->Get(NVPROPSETID_NvAudioFx_DirectSound, NVFXPROPERTY_FX_FXPROPERTY, 
                          &MixCapsParams, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                          pMixCapsData, dwMixcapSizeSecondPass, &RetVal);

        if(status == DS_OK)
        {
            // Copy the MixCaps data to our class
            PKSAUDIO_MIX_CAPS pMixCapsx = (PKSAUDIO_MIX_CAPS)(((DWORD *)pMixCapsData) + 2);
            DWORD CapsSize = dwMixcapSizeSecondPass - (sizeof(DWORD) * 2 );

            memcpy( pMixCaps, pMixCapsx, CapsSize );
        }
    }

    // Free up the temporary stuff
    if( pMixCapsData )
        GlobalFree( pMixCapsData );

    return status;
}

//************************************************************************
// Name: SetMixLevels
// Desc: Set the volume levels and mute for all I/O of this Submix.
//************************************************************************
HRESULT SetMixLevels
( 
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    NVFXDATA_FX_ID             FxSubmixId,

    PKSAUDIO_MIXCAP_TABLE      pMixCapsTable,
    PKSAUDIO_MIXLEVEL          pMixLevels
)
{
    HRESULT status = E_FAIL;
    DWORD RetVal = 0;

    // Dont call this if the Submix has not been created yet.
    if(( FxSubmixId == 0 ) || ( pFxManagerId->FxManagerId == 0 ))
        return S_OK;

    NVFXINST_FXPROPERTY_DESCRIPTOR MixCapsParams;
        
    MixCapsParams.FxInstance.FxManagerId = pFxManagerId->FxManagerId;
    MixCapsParams.FxInstance.FxId = FxSubmixId;
    MixCapsParams.PropertyId = KSPROPSETID_Audio;
    MixCapsParams.Property = KSPROPERTY_AUDIO_MIX_LEVEL_TABLE;

    DWORD dwSize = sizeof(KSAUDIO_MIXLEVEL) * pMixCapsTable->InputChannels 
                                            * pMixCapsTable->OutputChannels;

    // Send the data to the hardware now
    if(pKsPropertySet)
    {
        status = pKsPropertySet->Set(NVPROPSETID_NvAudioFx_DirectSound, 
                            NVFXPROPERTY_FX_FXPROPERTY, 
                            &MixCapsParams, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                            pMixLevels, dwSize );
    }

    return status;
}


//************************************************************************
// Name: GetMixLevels
// Desc: Get the volume levels and mute for all I/O of this Submix.
//************************************************************************
HRESULT GetMixLevels
( 
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    NVFXDATA_FX_ID             FxSubmixId,

    PKSAUDIO_MIXCAP_TABLE      pMixCapsTable,
    PKSAUDIO_MIXLEVEL          pMixLevels
)
{
    HRESULT status = E_FAIL;
    DWORD RetVal = 0;

    // Dont call this if the Submix has not been created yet.
    if(( FxSubmixId == 0 ) || ( pFxManagerId->FxManagerId == 0 ))
        return S_OK;

    NVFXINST_FXPROPERTY_DESCRIPTOR MixCapsParams;
        
    MixCapsParams.FxInstance.FxManagerId = pFxManagerId->FxManagerId;
    MixCapsParams.FxInstance.FxId = FxSubmixId;
    MixCapsParams.PropertyId = KSPROPSETID_Audio;
    MixCapsParams.Property = KSPROPERTY_AUDIO_MIX_LEVEL_TABLE;

    DWORD dwSize = sizeof(KSAUDIO_MIXLEVEL) * pMixCapsTable->InputChannels 
                                            * pMixCapsTable->OutputChannels;

    // Get the data from the hardware now
    if(pKsPropertySet)
    {
        status = pKsPropertySet->Get(NVPROPSETID_NvAudioFx_DirectSound, 
                            NVFXPROPERTY_FX_FXPROPERTY, 
                            &MixCapsParams, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                            pMixLevels, dwSize, &RetVal );
    }

    return status;
}


//************************************************************************
// Name: Set3DSubmixParameters
// Desc: Set parameters in submix for a given 3D control.
//************************************************************************
HRESULT Set3DSubmixParameters
( 
    LPKSPROPERTYSET            pKsPropertySet,
    PNVFXINST_FXMANAGER_ID     pFxManagerId,
    NVFXDATA_FX_ID             FxSubmixId,

    DWORD                      dwProperty3D, 
    DWORD                      dwParamSize,
    LPVOID                     pParams
)
{
    HRESULT hr = DS_OK;

    if(( pParams == NULL ) || ( dwParamSize == 0 ) || 
       ( FxSubmixId == 0 ) || ( pFxManagerId->FxManagerId == 0 )) 
        return S_FALSE;

    NVFXINST_FXPROPERTY_DESCRIPTOR Props3DDesc;
    Props3DDesc.FxInstance.FxManagerId = pFxManagerId->FxManagerId;
    Props3DDesc.FxInstance.FxId = FxSubmixId;
    Props3DDesc.PropertyId = KSPROPSETID_DirectSound3DBuffer;
    Props3DDesc.Property = dwProperty3D;

    if(g_NvidiaHwAvail)
    {
        hr = pKsPropertySet->Set(NVPROPSETID_NvAudioFx_DirectSound, 
                        NVFXPROPERTY_FX_FXPROPERTY, 
                        &Props3DDesc, sizeof(NVFXINST_FXPROPERTY_DESCRIPTOR),
                        pParams, dwParamSize);

        if(hr != DS_OK)
        {
            MessageBox( NULL, "Property KSPROPSETID_DirectSound3DBuffer (Set) failed.",
                              "Set SubMix Parameters Failure", MB_OK | MB_ICONERROR );
        }
    }

    return hr;
}


//************************************************************************
// Name: Set3DParameters
// Desc: Set 3D parameters for a given buffer.
//************************************************************************
HRESULT Set3DParameters
( 
    LPDIRECTSOUNDBUFFER        pDSB,
    DWORD                      dwProperty3D, 
    DWORD                      dwParamSize,
    LPVOID                     pParams,
    DWORD                      Deferred
)
{
  HRESULT status = DS_OK;

    if(( pParams == NULL ) || ( dwParamSize == 0 )) 
        //  || ( dwProperty3D == 0 )) Note: The 'ALL" property is 0
        return S_FALSE;


    // Get the 3D interface
    LPDIRECTSOUND3DBUFFER pDS3DB = NULL;
    if( FAILED( status = pDSB->QueryInterface(IID_IDirectSound3DBuffer, (void**)&pDS3DB)))
        return status;


    if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_POSITION)
    {
        DS3DVECTOR * pV = (DS3DVECTOR *)pParams;
        status = pDS3DB->SetPosition( pV->x, pV->y, pV->z, Deferred );
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_VELOCITY )
    {
        DS3DVECTOR * pV = (DS3DVECTOR *)pParams;
        status = pDS3DB->SetVelocity( pV->x, pV->y, pV->z, Deferred );
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_CONEOUTSIDEVOLUME )
    {
        LONG Volume = *(LONG *)pParams;
        status = pDS3DB->SetConeOutsideVolume( Volume, Deferred );
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_MAXDISTANCE )
    {
        D3DVALUE MaxDistance = *(D3DVALUE *)pParams;
        status = pDS3DB->SetMaxDistance( MaxDistance, Deferred);
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_MINDISTANCE )
    {
        D3DVALUE MinDistance = *(D3DVALUE *)pParams;
        status = pDS3DB->SetMinDistance( MinDistance, Deferred);
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_CONEANGLES )
    {
        KSDS3D_BUFFER_CONE_ANGLES * Ca =  (KSDS3D_BUFFER_CONE_ANGLES *)pParams;
        status = pDS3DB->SetConeAngles( Ca->InsideConeAngle, Ca->OutsideConeAngle, Deferred);
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_CONEORIENTATION )
    {
        DS3DVECTOR * pCo = (DS3DVECTOR *)pParams;
        status = pDS3DB->SetConeOrientation( pCo->x, pCo->y, pCo->z, Deferred );
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_MODE )
    {
        DWORD Mode = *(DWORD *)pParams;
        status = pDS3DB->SetMode( Mode, Deferred );
    }
    else if(dwProperty3D == KSPROPERTY_DIRECTSOUND3DBUFFER_ALL )
    {
        DS3DBUFFER * pAll = (DS3DBUFFER *)pParams;
        status = pDS3DB->SetAllParameters( pAll, Deferred );
    }

    pDS3DB->Release();  // Release the interface

    return status;
}


//************************************************************************
// End of File: HwInterface.cpp
//************************************************************************
