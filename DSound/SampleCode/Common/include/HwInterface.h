//************************************************************************
//
// HwInterface.h : Hardware access routine prototypes
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

HRESULT CreateFxMananger
(
    LPKSPROPERTYSET             pKsPropertySet,
    DWORD                       FxManagerType,
    PNVFXINST_FXMANAGER_ID      pFxManagerId
);
HRESULT DestroyFxManager
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId
);
HRESULT AddEffect
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    PNVFXDATA_FX_ID             pFxId,
    DWORD                       Inputs,
    DWORD                       Outputs,
    GUID                        Guid,
    DWORD                       Flags
);
HRESULT AttachWire
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    PNVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR pFxCD
);
HRESULT EnableEffect
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    NVFXDATA_FX_ID              FxId
);
HRESULT DisableEffect
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    NVFXDATA_FX_ID              FxId
);
HRESULT DetachWire
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    PNVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR pFxCD
);
HRESULT DeleteEffect
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    NVFXDATA_FX_ID              FxId
);
HRESULT SetBatchMode
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId
);
HRESULT ResetBatchMode
(
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId
);
HRESULT SetEffectsParameters
( 
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXPROPERTY_DESCRIPTOR pFxPropDesc,
    DWORD                       dwParamSize,
    LPVOID                      pParams
);
HRESULT GetEffectsParameters
( 
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXPROPERTY_DESCRIPTOR pFxPropDesc,
    DWORD                       dwParamSize,
    LPVOID                      pParams
);
HRESULT GetMixLevelCaps
( 
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    NVFXDATA_FX_ID              FxSubmixId,

    PKSAUDIO_MIXCAP_TABLE       pMixCapsTable,
    PKSAUDIO_MIX_CAPS           pMixCaps
);
HRESULT SetMixLevels
( 
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    NVFXDATA_FX_ID              FxSubmixId,

    PKSAUDIO_MIXCAP_TABLE       pMixCapsTable,
    PKSAUDIO_MIXLEVEL           pMixLevels
);
HRESULT GetMixLevels
( 
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    NVFXDATA_FX_ID              FxSubmixId,

    PKSAUDIO_MIXCAP_TABLE       pMixCapsTable,
    PKSAUDIO_MIXLEVEL           pMixLevels
);
HRESULT Set3DSubmixParameters
( 
    LPKSPROPERTYSET             pKsPropertySet,
    PNVFXINST_FXMANAGER_ID      pFxManagerId,
    NVFXDATA_FX_ID              FxSubmixId,

    DWORD                       dwProperty3D, 
    DWORD                       dwParamSize,
    LPVOID                      pParams
);
HRESULT Set3DParameters
( 
    LPDIRECTSOUNDBUFFER         pDSB,
    DWORD                       dwProperty3D, 
    DWORD                       dwParamSize,
    LPVOID                      pParams,
    DWORD                       Deferred
);


//************************************************************************
// End of File: HwInterface.h
//************************************************************************
