//*****************************************************************************
//*****************************************************************************
//
// nvfxapi.h
//
//
// Description:
// This file defines the NVIDIA Audio FX Property Set.
//
//*****************************************************************************
//*****************************************************************************
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



#ifndef _NVFXAPI_H_
#define _NVFXAPI_H_


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus



//*****************************************************************************
//*****************************************************************************
//
// Random Hacky Stuff
//
//*****************************************************************************
//*****************************************************************************

//
// This is stuff that is tacked onto DSOUND IKsProperty calls.  The DSOUND
// property is actually a KSNODEPROPERTY_AUDIO_PROPERTY + our instance data.
// Note that DSOUND will also make sure that this buffer has a size multiple
// of 8 (on 32-bit architectures at least).
//
// Unfortunately, portclass doesn't define this structure and we also
// don't really have a good way to tell if a property came from DSOUND or
// came directly to the driver through "other" means.
//
typedef struct
{
    //
    // The app context pointer (or DSOUND pointer).
    //
    PVOID AppContext;

    //
    // The size of data following this structure.  Be warned that DSOUND will
    // allocate an instance buffer that is aligned on an 8-byte boundary so
    // the total size may not be correct either.  This is the actual length of
    // the instance data from the application.
    //
    ULONG Length;

    //
    // This is reserved in KSNODEPROPERTY_AUDIO_PROPERTY.
    //
#ifndef _WIN64
    //
    // This is extra padding that comes from the actual size of the
    // KSNODEPROPERTY_AUDIO_PROPERTY when it is allocated by the compiler to
    // properly align the fields.
    //
    // TODO: Check what happens on 64-bit architectures when we get there.
    //
    ULONG Reserved[2];
#endif // _WIN64
} PCPROPERTY_DSOUND_INSTANCE, *PPCPROPERTY_DSOUND_INSTANCE;


//
// Use this version when passing a KS Audio property through the Microsoft
// DSOUND API or prepending a PCPROPERTY_DSOUND_INSTANCE structure to your
// property.
//
// {10DFBA63-40F2-4c5a-9AC0-16EBC30D3037}
//
DEFINE_GUID(KSPROPSETID_Audio_DirectSound,
0x10dfba63, 0x40f2, 0x4c5a, 0x9a, 0xc0, 0x16, 0xeb, 0xc3, 0xd, 0x30, 0x37);



//*****************************************************************************
//*****************************************************************************
//
// NVIDIA Audio Property Set
//
//*****************************************************************************
//*****************************************************************************

//
// NVPROPSETID_NvAudio
// {DB4F1FBF-E9D6-45D8-8818-DCC96D591CAC}
//
DEFINE_GUID(NVPROPSETID_NvAudio,
0xdb4f1fbf, 0xe9d6, 0x45d8, 0x88, 0x18, 0xdc, 0xc9, 0x6d, 0x59, 0x1c, 0xac);

//
// NVPROPSETID_NvAudio_DirectSound
// {98359B14-CBD6-4F05-A74A-C15AAD532DC4}
//
DEFINE_GUID(NVPROPSETID_NvAudio_DirectSound,
0x98359b14, 0xcbd6, 0x4f05, 0xa7, 0x4a, 0xc1, 0x5a, 0xad, 0x53, 0x2d, 0xc4);

//
// Supported NVAUDIO Properties:
//
//     NVAUDIOPROPERTY_RESOURCE_LIMITS
//         This is a property to determine the total hardware
//         resource available.  There is no instance data for this property
//         and a NVFXDATA_AUDIO_RESOURCES_DESCRIPTOR is the data parameter.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVAUDIOPROPERTY_RESOURCE_USAGE
//         This is a property to determine the current hardware
//         resource usage.  There is no instance data for this property
//         and a NVFXDATA_AUDIO_RESOURCES_DESCRIPTOR is the data parameter.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVAUDIOPROPERTY_FXMANAGERLIST
//         Reserved.
//
//
//     NVAUDIOPROPERTY_FXMANAGERLIST_SIZE
//         Reserved.
//
//
typedef enum
{
    NVAUDIOPROPERTY_RESOURCE_LIMITS = 0,
    NVAUDIOPROPERTY_RESOURCE_USAGE,
    NVAUDIOPROPERTY_FXMANAGERLIST,      // Reserved
    NVAUDIOPROPERTY_FXMANAGERLIST_SIZE  // Reserved

} NVAUDIOPROPERTY, *PNVAUDIOPROPERTY;


//
// This data struture is used by the NVFXPROPERTY_FX_RESOURCE_REQUIREMENTS
// property.
//
typedef struct
{
    unsigned long  PMemSize;
    unsigned long  XMemSize;
    unsigned long  YMemSize;
    unsigned long  GpPercentage;
    unsigned long  Voices2d;
    unsigned long  Voices3d;
    unsigned long  VoiceMixBuffers;

} NVFXDATA_AUDIO_RESOURCES_DESCRIPTOR, *PNVFXDATA_AUDIO_RESOURCES_DESCRIPTOR;



//*****************************************************************************
//*****************************************************************************
//
// NVIDIA FX Manager Property Set
//
//*****************************************************************************
//*****************************************************************************

//
// NVPROPSETID_NvAudioFxManager
// {F2A3D3CE-B43A-492f-B762-B66BFB7648AC}
//
DEFINE_GUID(NVPROPSETID_NvAudioFxManager,
0xf2a3d3ce, 0xb43a, 0x492f, 0xb7, 0x62, 0xb6, 0x6b, 0xfb, 0x76, 0x48, 0xac);

//
//
// Use this version when going through the Microsoft DSOUND API or
// prepending a PCPROPERTY_DSOUND_INSTANCE structure to your property.
//
// {218C41C1-B4D0-4E1C-932C-351CB8AD4EF9}
//
DEFINE_GUID(NVPROPSETID_NvAudioFxManager_DirectSound,
0x218c41c1, 0xb4d0, 0x4e1c, 0x93, 0x2c, 0x35, 0x1c, 0xb8, 0xad, 0x4e, 0xf9);


//
// Supported FX Manager Properties:
//
//     NVFXPROPERTY_FXMANAGER_ALLOCATE
//         This property allocates an FX Manager for the application.  This
//         allows custom FX paths to be download that are only accessible to a
//         single instance of an application.  If your application wishes to
//         use 3D submixes, the FX Manager must be created on a 3D buffer to tie
//         the submixes to the 3D listener.  Be aware that DirectSound3D
//         will deallocate the listener when the last 3D object is released.
//         For this reason, at least one 3D DirectSound buffer should be held
//         open for the life of the application and the FX Manager should be
//         deallocated before this buffer is released.  The ID of the
//         allocated FX Manager will be returned as the NVFXINST_FXMANAGER_ID
//         structure which is passed as the property data parameter.  Parameters
//         for the creation of the FX Manager are passed down in the
//         NVFXINST_FXMANAGER_ALLOCATE structure as instance data.  For all
//         other NVIDIA FX property calls, the NVFXINST_FXMANAGER_ID is be
//         passed as part (or all) of the property instance data.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_ATTACH
//         This property is used to build path of the data flow.  Please see
//         the description of NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR for more
//         information regarding the data for this property.
//
//         This property supports IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_BATCH_MODE
//         This property is specified to make batched updates to the FX
//         execution list.  Any FX additions or deletions will not be
//         asserted into the hardware while the FX Manager is in batch mode.
//         It is desirable to add and remove all FX in batch mode to
//         prevent constant change to the execution list.
//
//         This property supports IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_DEALLOCATE
//         This property is used to deallocate the FX Manager.  At that time,
//         all FXs downloaded to this instance of the FX Manager will be
//         freed and all FXs added by this instance of the FX Manager will be
//         removed and freed.  This property requires only the FX Manager ID
//         to be passed in the instance field.
//
//         This property supports IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_DETACH
//         This property is used to partially tear down path of the data flow.
//
//         This property supports IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_DOWNLOADDYNAMICFX
//         Reserved.
//
//
//     NVFXPROPERTY_FXMANAGER_DOWNLOADSTATICFX
//         Reserved.
//
//
//     NVFXPROPERTY_FXMANAGER_FXADD
//         This property is used to add an instance of an FX.  Keep in mind,
//         the order in which FXs are added is critical to build a proper
//         execution list.  The best strategy to add FXs is by following a
//         predetermined the execution order.  This order may be determined
//         using pre-built FX graph by place the FXs which may be executed in
//         parallel into columns and those that are executed in series into
//         rows.  Add all of the FXs from the first column, followed by those
//         in the second, and so on.  FXs may not be arbitrarily inserted
//         into the FX graph.  All FXs except mixes, submixes, and splitters
//         are in-place transforms.
//
//         Please see the NVFXINST_FXMANAGER_FXADD_DESCRIPTOR for more
//         information regarding the instance data for this property.  The
//         NVFXDATA_FX_ID structure is used as the data for this structure
//         and is used as instance data when referencing this FX.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_FXDELETE
//         This property is used to remove an instance of an FX.  The
//         NVFXDATA_FX_ID structure is passed as the data for this call.  An
//         FX removed from the middle of a graph cause all FXs down stream
//         from the FX to be deleted.
//
//         This property supports IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_FXLIST
//         Gets the FX list for the specified FX Manager.  The
//         NVFXINST_FXMANAGER_ID structure is passed as the instance data for
//         this call and the NVFXDATA_FXMANAGER_FXLIST_DESCRIPTOR is passed
//         for the data.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_FXLIST_SIZE
//         Gets the FX list size for the specified FX Manager.  The
//         NVFXINST_FXMANAGER_ID structure is passed as the instance data for
//         this call and a unsigned long is passed for the data.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_WIRELIST
//         Gets the wire list for the specified FX Manager.  The
//         NVFXINST_FXMANAGER_ID structure is passed as the instance data for
//         this call and the NVFXDATA_FXMANAGER_WIRELIST_DESCRIPTOR is passed
//         for the data.  Note that wires connecting voices to FXs are NOT
//         saved.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FXMANAGER_WIRELIST_SIZE
//         Gets the wire list size for the specified FX Manager.  The
//         NVFXINST_FXMANAGER_ID structure is passed as the instance data for
//         this call and a unsigned long is passed for the data.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
typedef enum
{
    NVFXPROPERTY_FXMANAGER_ALLOCATE = 0,
    NVFXPROPERTY_FXMANAGER_ATTACH,
    NVFXPROPERTY_FXMANAGER_BATCH_MODE,
    NVFXPROPERTY_FXMANAGER_DEALLOCATE,
    NVFXPROPERTY_FXMANAGER_DETACH,
    NVFXPROPERTY_FXMANAGER_DOWNLOADDYNAMICFX,  // Reserved
    NVFXPROPERTY_FXMANAGER_DOWNLOADSTATICFX,   // Reserved
    NVFXPROPERTY_FXMANAGER_FXADD,
    NVFXPROPERTY_FXMANAGER_FXDELETE,
    NVFXPROPERTY_FXMANAGER_FXLIST,
    NVFXPROPERTY_FXMANAGER_FXLIST_SIZE,
    NVFXPROPERTY_FXMANAGER_WIRELIST,
    NVFXPROPERTY_FXMANAGER_WIRELIST_SIZE,

} NVFXPROPERTY_FXMANAGER, *PNVFXPROPERTY_FXMANAGER;


//*****************************************************************************
// NVFXPROPERTY_FXMANAGER_ALLOCATE
//*****************************************************************************

//
// These are the flags to specify the local mixes to create on the Local
// FX Manager.  The pin associated with the allocated local mix is defined
// below with the NVA_FX_PIN type.
//
const unsigned long NVA_FX_GLOBAL_MIX_MAP_LEFT             = 0x00000001;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_RIGHT            = 0x00000002;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_CENTER           = 0x00000004;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_LFE              = 0x00000008;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_REAR_LEFT        = 0x00000010;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_REAR_RIGHT       = 0x00000020;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_3D_FRONT_LEFT    = 0x00000040;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_3D_FRONT_RIGHT   = 0x00000080;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_3D_REAR_LEFT     = 0x00000100;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_3D_REAR_RIGHT    = 0x00000200;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_REVERB           = 0x00000400;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_CHORUS_LEFT      = 0x00000800;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_CHORUS_RIGHT     = 0x00001000;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_REPLICATED_LEFT  = 0x00002000;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_REPLICATED_RIGHT = 0x00004000;
const unsigned long NVA_FX_GLOBAL_MIX_MAP_RESERVED         = 0xffff8000;

//
// These constants should be used to route mono FX paths to more than a single
// global output.
//
const unsigned long
NVA_FX_LOCAL_MIX_MAP_STEREO =
    NVA_FX_GLOBAL_MIX_MAP_LEFT | NVA_FX_GLOBAL_MIX_MAP_RIGHT;

const unsigned long
NVA_FX_LOCAL_MIX_MAP_QUADRAPHONIC =
    NVA_FX_LOCAL_MIX_MAP_STEREO |
    NVA_FX_GLOBAL_MIX_MAP_REAR_LEFT | NVA_FX_GLOBAL_MIX_MAP_REAR_RIGHT;

const unsigned long
NVA_FX_LOCAL_MIX_MAP_5POINT1 =
    NVA_FX_LOCAL_MIX_MAP_QUADRAPHONIC |
    NVA_FX_GLOBAL_MIX_MAP_CENTER | NVA_FX_GLOBAL_MIX_MAP_LFE;

const unsigned long
NVA_FX_LOCAL_MIX_MAP_3D =
    NVA_FX_GLOBAL_MIX_MAP_3D_FRONT_LEFT | NVA_FX_GLOBAL_MIX_MAP_3D_FRONT_RIGHT |
    NVA_FX_GLOBAL_MIX_MAP_3D_REAR_LEFT | NVA_FX_GLOBAL_MIX_MAP_3D_REAR_RIGHT;

//
// This is used in the NVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR to specify
// parameters of each local mix.  The valid values for inputs:outputs
// are: 1:1, 1:2, 1:3, 1:4, 1:5, 1:6, 2:1, 2:2, 2:3, 2:4, 3:1, 4:1, 4:2,
// 5:1, and 6:1.
//
typedef struct
{
    //
    // The number of inputs to the local mix.
    //
    unsigned long Inputs;

    //
    // The number of outputs from the local mix.
    //
    unsigned long Outputs;

    //
    // The inputs that are mixed and mapped to the output global mixes.
    //
    unsigned long LocalMixToGlobalMixMap;

} NVFX_LOCAL_MIX_DESCRIPTOR, *PNVFX_LOCAL_MIX_DESCRIPTOR;

//
// This is the function used to verify the local mix descriptor.  It will
// return 1 if the parameters are valid, 0 if they are not.  If you specify
// more local to global mappings than outputs, only the "Outputs" number of
// connections will be made starting from the left global mix.
//
inline int ValidLocalMixDescriptor
(
    IN  PNVFX_LOCAL_MIX_DESCRIPTOR pDescriptor
)
{
    if(!pDescriptor)
    {
        return 0;
    }

    if(pDescriptor->LocalMixToGlobalMixMap & NVA_FX_GLOBAL_MIX_MAP_RESERVED)
    {
        return 0;
    }

    unsigned long inputs  = pDescriptor->Inputs;
    unsigned long outputs = pDescriptor->Outputs;

    // 1:1, 1:2, 1:3, 1:4, 1:5, 1:6
    if(inputs == 1 && (1 <= outputs && outputs <= 6))
    {
        return 1;
    }

    // 2:1, 2:2, 2:4
    if(inputs == 2 && (outputs == 1 || outputs == 2 || outputs == 4))
    {
        return 1;
    }

    // 3:1, 3:2
    if(inputs == 3 && (outputs == 1 || outputs == 2))
    {
        return 1;
    }

    // 4:1, 4:2
    if(inputs == 4 && (outputs == 1 || outputs == 2))
    {
        return 1;
    }

    // 5:1, 6:1
    if((inputs == 5 || inputs == 6) && outputs == 1)
    {
        return 1;
    }

    return 0;
}

//
// The data structure to pass for the NVPROPERTY_FXMANAGER_ALLOCATE property.
// The FX Manager ID returned by this property call should be used as the
// DirectSound instance data for all other property calls.
//
// Only one application can allocate the Default FX Manager at any time as it
// specifies the default routing of all voices.  If the Default FX Manager is
// not specified, this property specifies the local mixes to allocate when a
// Local FX Manager is created.  If no local mixes are specified, the Local FX
// Manager will connect an FX directly to the global mixes when the FX is
// routed.  If more than one FX path is needed through the Local FX Manager,
// local terminating mixes should be created as connections to the global mixes
// are limited.
//
typedef struct
{
    //
    // The FX Manager allocation flags.
    //
    unsigned long Flags;

    //
    // Number of local mixes.
    //
    unsigned long NumberOfLocalMixes;

    //
    // Array of local mix output to global mix input masks.  This structure
    // should be allocated dynamically based on the required number of
    // local mixes you wish to have.  For example:  For 2 local mixes routed
    // to left and right, you would allocate
    //
    //   totalSize = sizeof(NVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR) +
    //               sizeof(NVFX_LOCAL_MIX_DESCRIPTOR)
    //
    // bytes for the structure.  The first local mix is included in this
    // structure, additional ones need space allocated for them.
    //
    NVFX_LOCAL_MIX_DESCRIPTOR LocalMixDescriptor[1];

} NVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR,
    *PNVFXINST_FXMANAGER_ALLOCATE_DESCRIPTOR;

//
// The FX Manager allocation flags.
//
// If no flags are specified, a Local FX Manager will be allocated which will
// only be is available only to your application.  The Default Local FX Manager
// is the default path that any sound played will flow through.  The Global FX
// Manager contains the effects that are applied to the mix of all local paths.
//
const unsigned long
NVFXINST_FXMANAGER_ALLOCATE_FLAGS_DEFAULT_LOCAL_FX_MANAGER  = 0x00000001;
const unsigned long
NVFXINST_FXMANAGER_ALLOCATE_FLAGS_GLOBAL_FX_MANAGER         = 0x00000002;


typedef void* NVFXMANAGER_ID;
typedef NVFXMANAGER_ID* PNVFXMANAGER_ID;

typedef struct
{
    //
    // The ID of the FX Manager.
    //
    NVFXMANAGER_ID FxManagerId;

} NVFXINST_FXMANAGER_ID, *PNVFXINST_FXMANAGER_ID,
  NVFXDATA_FXMANAGER_ID, *PNVFXDATA_FXMANAGER_ID;


//*****************************************************************************
// NVFXPROPERTY_FXMANAGER_BATCH_MODE
//*****************************************************************************

//
// The data structure to pass for the NVFXPROPERTY_FXMANAGER_BATCH_MODE
// property.  Set to 0 to take the FX Manager out of batch mode, anything
// else places the FX Manager in batched mode.
//
typedef unsigned long NVFXDATA_FXMANAGER_BATCH;
typedef NVFXDATA_FXMANAGER_BATCH *PNVFXDATA_FXMANAGER_BATCH;


//*****************************************************************************
// NVFXPROPERTY_FXMANAGER_DOWNLOADDYNAMICFX
//*****************************************************************************

//
// Don't use this yet.
//
typedef struct
{
    //
    // The type of the custom effect.
    //
    GUID Type;

    //
    // The size in bytes of the entire downloaded Fx structure.
    //
    unsigned long Size;

    //
    //
    // The DSP code to implement the effect.
    //
    unsigned long Fx[1];

} NVFXDATA_FXMANAGER_DOWNLOADSTATICFX_DESCRIPTOR,
  *PNVFXDATA_NVFXMANAGER_DOWNLOADSTATICFX_DESCRIPTOR;


//*****************************************************************************
// NVFXPROPERTY_FXMANAGER_FXADD
//*****************************************************************************

//
// The instance structure to pass for the NVPROPERTY_FXMANAGER_FXADD property.
// The FX ID returned by this property call should be used for all future
// references for this FX.
//
typedef struct
{
    //
    // The ID of the FX Manager.
    //
    NVFXMANAGER_ID FxManagerId;

    //
    // The type of the FX.
    //
    GUID Type;

    //
    // Flags describing the FX.
    //
    unsigned long Flags;

    //
    // Number of inputs to create for effect.
    //
    unsigned long Inputs;

    //
    // Number of outputs to create for effect.
    //
    unsigned long Outputs;

} NVFXINST_FXMANAGER_FXADD_DESCRIPTOR, *PNVFXINST_FXMANAGER_FXADD_DESCRIPTOR;

//
// Supported FX Types:
//
// Standard DX8
//
//     Chorus                   {EFE6629C-81F7-4281-BD91-C9D604A95AF6}
//     GUID_DSFX_STANDARD_CHORUS
//
//     Compressor/Limiter       {EF011F79-4000-406D-87AF-BFFB3FC39D57}
//     GUID_DSFX_STANDARD_COMPRESSOR
//
//     Distortion               {EF114C90-CD1D-484E-96E5-09CFAF912A21}
//     GUID_DSFX_STANDARD_DISTORTION
//
//     Echo/Delay               {EF3E932C-D40B-4F51-8CCF-3F98F1B29D5D}
//     GUID_DSFX_STANDARD_ECHO
//
//     Flanger                  {EFCA3D92-DFD8-4672-A603-7420894BAD98}
//     GUID_DSFX_STANDARD_FLANGER
//
//     Gargle                   {DAFD8210-5711-4B91-9FE3-F75B7AE279BF}
//     GUID_DSFX_STANDARD_GARGLE
//
//     Reverb (Listener) Effect {EF985E71-D5C7-42D4-BA4D-2D073E2E96F4}
//     GUID_DSFX_STANDARD_I3DL2REVERB
//
//     Parametric Equalization  {120CED89-3BF4-4173-A132-3CB406CF3231}
//     GUID_DSFX_STANDARD_PARAMEQ
//
//
// NVIDIA
//
//     Equalizer                {7006D745-B8E2-448A-A75F-CECA1D22F934}
//     GUID_NVFX_Equalizer
//         This supports various equalizer property sets.
//
//     Level Meter              {AD00DCF4-D479-4AA0-9B9E-78375180247C}
//     GUID_NVFX_LevelMeter
//         This supports the NVIDIA Level Meter property set.
//
//     Mix                      {D92A7E7A-4293-4708-96EC-BC940B13CDC0}
//     GUID_NVFX_Mix
//         Voices must always be routed to either a Mix FX that has been
//         added with the NVFXDATA_FXMANAGER_FXADD_MIX_VOICE flag or to a
//         Submix FX.  A voice mix is allocated when the
//         NVFXDATA_FXMANAGER_FXADD_MIX_VOICE flag is set, otherwise a mix
//         FX is added.
//
//         Standard mix FXs (not ones allocated with the
//         NVFXDATA_FXMANAGER_FXADD_MIX_VOICE set) support the get and set of
//         the KSPROPERTY_AUDIO_MIX_LEVEL_CAPS property.  A set using this
//         property may only be used to change the default number of input
//         and output pin instances of the mix FX.  The number of inputs and
//         outputs may only be set while the FX is disabled and has no FXs
//         attached.  Voice mixes do not have limits on the number of inputs
//         and have a single output.
//
//         Mixes always use 0 for the source and destination pins.
//
//     Reverb                   {E5DCFF44-2301-4484-B157-E1B0DF55B237}
//     GUID_NVFX_Reverb
//         This uses I3DL2 properties to set the parameters.
//
//     Sample Rate Convertor    {D9C33EFB-7B99-4FA4-A982-6B0D36ADE91F}
//     GUID_NVFX_SampleRateConvertor,
//         This supports sample rate property sets.
//
//     Submix                   {50FBC5B2-4DD9-4A57-B13E-030C2BCC871B}
//     GUID_NVFX_Submix
//         The output of a submix may not be used as a source to another
//         submix.  The submix uses 0 for its source and destination pin.
//
//     Terminator               {CF1494DA-108A-4F7F-8969-A083705FD21A}
//     GUID_NVFX_Terminator
//         This is the last FX in the graph where channel blending occurs
//         based on the channel configuration.
//
//     XTalk                    {15D067EA-DA61-4A9E-A9DA-2D0FA53F84BC}
//     GUID_NVFX_XTalk
//         This can only be enabled and disabled.
//         TODO: Allow speaker geometry properties
//
//

DEFINE_GUID(GUID_NVFX_Equalizer,
0x7006d745, 0xb8e2, 0x448a, 0xa7, 0x5f, 0xce, 0xca, 0x1d, 0x22, 0xf9, 0x34);

DEFINE_GUID(GUID_NVFX_LevelMeter,
0xad00dcf4, 0xd479, 0x4aa0, 0x9b, 0x9e, 0x78, 0x37, 0x51, 0x80, 0x24, 0x7c);

DEFINE_GUID(GUID_NVFX_Mix,
0xd92a7e7a, 0x4293, 0x4708, 0x96, 0xec, 0xbc, 0x94, 0x0b, 0x13, 0xcd, 0xc0);

//
// Supported Mix FX Add Flags:
//
//     There are two type of Mix FXs: Standard and Voice.  The default is the
//     Standard Mix.  It will allocate a hardware Mix FX into which the output
//     of FXs may be mixed.  All inputs to the FX are mixed, and all the
//     outputs are identical.  This type may be used for splits in the audio
//     path.
//
//     NVFXDATA_FXMANAGER_FXADD_MIX_VOICE
//         This flag is only supported by the Mix FX.  It will allocate a
//         hardware mix buffer into which voices may be mixed.
//
//     NVFXDATA_FXMANAGER_FXADD_MIX_REUSE_INPUT_BUFFERS
//         Then this flag is set, the mix will reuse the input buffers.
//         Care should be used with this flag as the inputs must be attached
//         before the outputs and the mix must be deleted before the upstream
//         FXs that allocated the mix buffer to prevent downstream FXs from
//         using an invalid buffer.  Note that adding output wires will fail
//         until a respective input wire has been created or an output pin
//         that has allocated a buffer is found.
//
//
//     These flags are reserved and should not be used by applications:
//
//     NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_SYSTEM_VOICES
//         This flag will provide a separate volume control for all system
//         sounds or Microsoft Direct Sound software buffers.
//
//     NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_2D_VOICES
//         This flag will provide a separate volume control for all Direct
//         Sound 2D hardware buffers.
//
//     NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_3D_VOICES
//         This flag will provide a separate volume control for all Direct
//         Sound 3D hardware buffers.
//
//     NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_SYNTH_VOICES
//         This flag will provide a separate volume control for all synth
//         buffers.
//
//     NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_DIGITIZED_VOICES
//         This flag will provide a separate volume control for the digitized
//         input channel.
//

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_VOICE                        = 0x00000001;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_REUSE_INPUT_BUFFERS          = 0x00000002;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_SYSTEM_VOICES    = 0x00010000;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_2D_VOICES        = 0x00020000;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_3D_VOICES        = 0x00040000;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_SYNTH_VOICES     = 0x00080000;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_DIGITIZED_VOICES = 0x00100000;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_MIX_VOICE_TRACK_ALL              = 0x00ff0000;


const long NVA_MAX_VOLUME_LEVEL = 0;
const long NVA_MIN_VOLUME_LEVEL = -10000;

const float NVA_MAX_VOLUME_LEVEL_FLOAT = 0.0f;
const float NVA_MIN_VOLUME_LEVEL_FLOAT = -10000.0f;

typedef struct
{
    //
    // This is the base volume level for the FX.  The default value is
    // NVA_MAX_VOLUME_LEVEL_FLOAT.
    //
    float BaseVolume;

    //
    // These are the relative volume levels of the different inputs of
    // the global reverb in mBs.  System sounds also include MS DirectSound
    // software buffers.  To disable the input, set the level to
    // NVA_MIN_VOLUME_LEVEL_FLOAT.
    //
    float SystemSounds;
    float Wave2d;
    float Wave3d;
    float Synth;
    float DigitizedInput;

    //
    // This is the amount of headroom to give to the mix.
    //
    unsigned char HeadRoom;

} NVFXPROPERTY_VOICEMIX, *PNVFXPROPERTY_VOICEMIX;


DEFINE_GUID(GUID_NVFX_Reverb,
0xe5dcff44, 0x2301, 0x4484, 0xb1, 0x57, 0xe1, 0xb0, 0xdf, 0x55, 0xb2, 0x37);

DEFINE_GUID(GUID_NVFX_SampleRateConvertor,
0xd9c33efb, 0x7b99, 0x4fa4, 0xa9, 0x82, 0x6b, 0xd, 0x36, 0xad, 0xe9, 0x1f);

DEFINE_GUID(GUID_NVFX_Submix,
0x50fbc5b2, 0x4dd9, 0x4a57, 0xb1, 0x3e, 0x03, 0x0c, 0x2b, 0xcc, 0x87, 0x1b);

//
// Supported Submix FX Add Flags:
//
//     NVFXDATA_FXMANAGER_FXADD_SUBMIX_3D
//         The Submix will support one input and will support 3D and I3DL2
//         buffer properties.
//
//     NVFXDATA_FXMANAGER_FXADD_SUBMIX_STEREO
//         The submix FX will support two inputs and four stereo pairs for
//         outputs.  The default is mono input and 8 mono outputs.
//
//     NVFXDATA_FXMANAGER_FXADD_SUBMIX_USE_TMP_MIX_BUFFERS
//         This flag is only supported by the submix FX.  Submixes that are
//         allocated with this flag will use the temporary multipass buffers.
//         Only two mix buffers are explicity specified as multipass submixes.
//         Therefore, only mono and stereo voices may be routed to the
//         temporary multipass buffers.  Voices that are in the on state may
//         not be rerouted to use the temporary submix buffers.  This flag is
//         ORed with either NVFXDATA_FXMANAGER_FXADD_SUBMIX_STEREO or
//         NVFXDATA_FXMANAGER_FXADD_SUBMIX_3D.
//
//

const unsigned long
NVFXDATA_FXMANAGER_FXADD_SUBMIX_3D                  = 0x00000001;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_SUBMIX_STEREO              = 0x00000002;

const unsigned long
NVFXDATA_FXMANAGER_FXADD_SUBMIX_USE_TMP_MIX_BUFFERS = 0x00000004;


DEFINE_GUID(GUID_NVFX_Terminator,
0xcf1494da, 0x108a, 0x4f7f, 0x89, 0x69, 0xa0, 0x83, 0x70, 0x5f, 0xd2, 0x1a);

DEFINE_GUID(GUID_NVFX_XTalk,
0x15d067ea, 0xda61, 0x4a9e, 0xa9, 0xda, 0x2d, 0xf, 0xa5, 0x3f, 0x84, 0xbc);



//
// Inputs and outputs for each FX type.
//

// Any effect.
const unsigned long MAX_NVFX_INPUTS         = 6;
const unsigned long MAX_NVFX_OUTPUTS        = 6;

// Chorus inputs cannot be greater than the outputs.
const unsigned long MIN_CHORUS_INPUTS       = 1;
const unsigned long MAX_CHORUS_INPUTS       = 2;
const unsigned long MIN_CHORUS_OUTPUTS      = 1;
const unsigned long MAX_CHORUS_OUTPUTS      = 2;

// Compressor inputs and outputs must be equal.
const unsigned long MIN_COMPRESSOR_INPUTS   = 1;
const unsigned long MAX_COMPRESSOR_INPUTS   = 2;
const unsigned long MIN_COMPRESSOR_OUTPUTS  = 1;
const unsigned long MAX_COMPRESSOR_OUTPUTS  = 2;

// Distortion inputs and outputs are always one.
const unsigned long MIN_DISTORTION_INPUTS   = 1;
const unsigned long MAX_DISTORTION_INPUTS   = 1;
const unsigned long MIN_DISTORTION_OUTPUTS  = 1;
const unsigned long MAX_DISTORTION_OUTPUTS  = 1;

// Echo inputs cannot be greater than the outputs.
const unsigned long MIN_ECHO_INPUTS         = 1;
const unsigned long MAX_ECHO_INPUTS         = 2;
const unsigned long MIN_ECHO_OUTPUTS        = 1;
const unsigned long MAX_ECHO_OUTPUTS        = 2;

// Flanger inputs cannot be greater than the outputs.
const unsigned long MIN_FLANGER_INPUTS      = 1;
const unsigned long MAX_FLANGER_INPUTS      = 2;
const unsigned long MIN_FLANGER_OUTPUTS     = 1;
const unsigned long MAX_FLANGER_OUTPUTS     = 2;

// Gargle inputs are always one.
const unsigned long MIN_GARGLE_INPUTS       = 1;
const unsigned long MAX_GARGLE_INPUTS       = 1;
const unsigned long MIN_GARGLE_OUTPUTS      = 1;
const unsigned long MAX_GARGLE_OUTPUTS      = 2;

// I3DL2 Reverb inputs cannot be greater than the outputs.
const unsigned long MIN_I3DL2REVERB_INPUTS  = 1;
const unsigned long MAX_I3DL2REVERB_INPUTS  = 1;
const unsigned long MIN_I3DL2REVERB_OUTPUTS = 4;
const unsigned long MAX_I3DL2REVERB_OUTPUTS = 4;

// Parametric EQ inputs and outputs are always one.
const unsigned long MIN_PARAMEQ_INPUTS      = 1;
const unsigned long MAX_PARAMEQ_INPUTS      = 1;
const unsigned long MIN_PARAMEQ_OUTPUTS     = 1;
const unsigned long MAX_PARAMEQ_OUTPUTS     = 1;

// Ignored if NVFXDATA_FXMANAGER_FXADD_MIX_VOICE flag is set.
const unsigned long MIN_MIX_INPUTS          = 1;
const unsigned long MAX_MIX_INPUTS          = 6;
const unsigned long MIN_MIX_OUTPUTS         = 1;
const unsigned long MAX_MIX_OUTPUTS         = 6;

// Sample Rate Convertors are always 1:1.
const unsigned long MIN_SRC_INPUTS          = 1;
const unsigned long MAX_SRC_INPUTS          = 1;
const unsigned long MIN_SRC_OUTPUTS         = 1;
const unsigned long MAX_SRC_OUTPUTS         = 1;


// Reverb inputs cannot be greater than the outputs.
// Values allowed are 1:1, 1:2, 1:4, and 2:2.
//
const unsigned long MIN_REVERB_INPUTS       = 1;
const unsigned long MAX_REVERB_INPUTS       = 2;
const unsigned long MIN_REVERB_OUTPUTS      = 1;
const unsigned long MAX_REVERB_OUTPUTS      = 4;

//
// This is the function used to verify the reverb descriptor.  It will
// return 1 if the parameters are valid, 0 if they are not.
//
inline int ValidReverbDescriptor
(
    IN  PNVFXINST_FXMANAGER_FXADD_DESCRIPTOR pDescriptor
)
{
    if(!pDescriptor)
    {
        return 0;
    }

    unsigned long inputs  = pDescriptor->Inputs;
    unsigned long outputs = pDescriptor->Outputs;

    // 1:1, 1:2, and 1:4
    if(inputs == 1 && (outputs == 1 || outputs == 2 || outputs == 4))
    {
        return 1;
    }

    // 2:2
    if(inputs == 2 && (outputs == 1 || outputs == 2 || outputs == 4))
    {
        return 1;
    }

    return 0;
}


//
// The Default Local FX IDs.  These are used along with the voice mixes in the
// Global FX Manager to create a custom FX path.
//
typedef enum
{
    //
    // These FX should not be removed from the Default Local FX Manager as they
    // are used to wire voices.  If they are removed, the voice will be wired to
    // to the first two voice mixes found, or else they will not be routed.
    //
    NVA_FX_ID_DEFAULT_VOICEMIX_CUSTOM_LEFT = 0,
    NVA_FX_ID_DEFAULT_VOICEMIX_CUSTOM_RIGHT,

    NVA_FX_ID_DEFAULT_INVALID = 0xffffffff

} NVA_FX_ID_DEFAULT, *PNVA_FX_ID_DEFAULT;

const unsigned long NUMBER_OF_DEFAULT_FXS = NVA_FX_ID_DEFAULT_VOICEMIX_CUSTOM_RIGHT + 1;


//
// These are default FX IDs of the Global FX Manager.  When querying the FX
// list, these are some handy defines that will help determine what is what.
//
typedef enum
{
    NVA_FX_ID_GLOBAL_VOICEMIX_LEFT = 0,
    NVA_FX_ID_GLOBAL_VOICEMIX_RIGHT,
    NVA_FX_ID_GLOBAL_VOICEMIX_CENTER,
    NVA_FX_ID_GLOBAL_VOICEMIX_LFE,
    NVA_FX_ID_GLOBAL_VOICEMIX_REAR_LEFT,
    NVA_FX_ID_GLOBAL_VOICEMIX_REAR_RIGHT,
    NVA_FX_ID_GLOBAL_VOICEMIX_3D_FRONT_LEFT,
    NVA_FX_ID_GLOBAL_VOICEMIX_3D_FRONT_RIGHT,
    NVA_FX_ID_GLOBAL_VOICEMIX_3D_REAR_LEFT,
    NVA_FX_ID_GLOBAL_VOICEMIX_3D_REAR_RIGHT,
    NVA_FX_ID_GLOBAL_VOICEMIX_REVERB,
    NVA_FX_ID_GLOBAL_VOICEMIX_CHORUS_LEFT,
    NVA_FX_ID_GLOBAL_VOICEMIX_CHORUS_RIGHT,
    NVA_FX_ID_GLOBAL_VOICEMIX_REPLICATED_LEFT,
    NVA_FX_ID_GLOBAL_VOICEMIX_REPLICATED_RIGHT,

    //
    // The two temporary multipass FXs are reserved for internal use and always feed back
    // into a voice and so are not shown as part of any FX graph.  They are used when
    // allocating a submix with the NVFXDATA_FXMANAGER_FXADD_SUBMIX_USE_TMP_MIX_BUFFERS
    // flag set.
    //
    NVA_FX_ID_GLOBAL_TEMPORARY_MULTIPASS_LEFT,
    NVA_FX_ID_GLOBAL_TEMPORARY_MULTIPASS_RIGHT,

    NVA_FX_ID_GLOBAL_MIX_REVERB,
    NVA_FX_ID_GLOBAL_REVERB,

    NVA_FX_ID_GLOBAL_MIX_CHORUS_LEFT,
    NVA_FX_ID_GLOBAL_MIX_CHORUS_RIGHT,
    NVA_FX_ID_GLOBAL_CHORUS,

    NVA_FX_ID_GLOBAL_MIX_3D_FRONT_LEFT,
    NVA_FX_ID_GLOBAL_MIX_3D_FRONT_RIGHT,
    NVA_FX_ID_GLOBAL_MIX_3D_REAR_LEFT,
    NVA_FX_ID_GLOBAL_MIX_3D_REAR_RIGHT,

    NVA_FX_ID_GLOBAL_XTALK_FRONT,
    NVA_FX_ID_GLOBAL_XTALK_REAR,

    NVA_FX_ID_GLOBAL_MIX_REPLICATED_LEFT,
    NVA_FX_ID_GLOBAL_MIX_REPLICATED_RIGHT,

    NVA_FX_ID_GLOBAL_MIX_LEFT,
    NVA_FX_ID_GLOBAL_MIX_RIGHT,
    NVA_FX_ID_GLOBAL_MIX_CENTER,
    NVA_FX_ID_GLOBAL_MIX_LFE,
    NVA_FX_ID_GLOBAL_MIX_REAR_LEFT,
    NVA_FX_ID_GLOBAL_MIX_REAR_RIGHT,

    NVA_FX_ID_GLOBAL_EQ,
    NVA_FX_ID_GLOBAL_TERMINATOR,
    NVA_FX_ID_GLOBAL_LEVEL_METER,

    NVA_FX_ID_GLOBAL_INVALID = 0xffffffff

} NVA_FX_ID_GLOBAL, *PNVA_FX_ID_GLOBAL;


//
// Some Global FX counts...
//
const unsigned long NUMBER_OF_3D_MIXES = 4;
const unsigned long NUMBER_OF_XTALK_FXS = 2;
const unsigned long NUMBER_OF_GLOBALEQ_FXS = 1;
const unsigned long NUMBER_OF_LEVELMETER_FXS = 1;
const unsigned long NUMBER_OF_GLOBAL_FXS = NVA_FX_ID_GLOBAL_LEVEL_METER + 1;

//
// This is the instance data type of the FX ID.  This returned as data
// from the NVFXPROPERTY_FXMANAGER_FXADD property.
//
typedef void* NVFXDATA_FX_ID;
typedef NVFXDATA_FX_ID* PNVFXDATA_FX_ID;
typedef NVFXDATA_FX_ID  NVFXINST_FX_ID;
typedef PNVFXDATA_FX_ID PNVFXINST_FX_ID;

//
// FX chains are always built from the source to destination.  This
// structure specifies the source and destination IDs for the
// NVFXPROPERTY_FXMANAGER_ATTACH property.
//
// To attach a voice to an effect, the source ID should be
// NVFX_SOURCE_IS_VOICE.  To determine the correct FX source, this property
// must be passed on the DirectSound stream or buffer to attach to the FX.
// By default, the voice will be attached to the standard output path.  The
// output path for a mono 2D voice is by the channel configuration set when
// the voice was created.  A "standard" mono voice is routed to the left and
// right channels on pins 0 and 1.  Non-standard mono voices, the parts of
// the 3D voice that are attached to the HRTF filter, and voices with greater
// than 2 channels may only be modified by global send effects.  The pin count
// on a voice is determined by submitting a KSPROPERTY_AUDIO_MIX_LEVEL_CAPS on
// the DirectSound buffer.  KSPROPERTY_AUDIO_MIX_LEVEL_TABLE should be used to
// set the volumes on the voice.  This applies to submixes as well.
//
// Initially all 2D voices are attached to the global mixes by soft links.
// Once an FX is attached, all soft link paths will be muted.  To unmute the
// path, the voice must be attached to an FX.  By default, a voice is attached
// to the global mixes as follows with unused channels muted:
//
//       2D Stereo                     3D
//    +------+                      +------+
//    |     0|-> Front Left         |     0|-> 3D Front Left   (Reserved)
//    |  V  1|-> Front Right        |  V  1|-> 3D Front Right  (Reserved)
//    |  o  2|-> (Unassigned)       |  o  2|-> 3D Back Left    (Reserved)
//    |  i  3|-> (Unassigned)       |  i  3|-> 3D Back Right   (Reserved)
//    |  c  4|-> (Unassigned)       |  c  4|-> I3DL2 Direct Path
//    |  e  5|-> (Unassigned)       |  e  5|-> I3DL2 Reverb Path (Reserved)
//    |     6|-> (Unassigned)       |     6|-> I3DL2 Direct Path
//    |     7|-> (Unassigned)       |     7|-> I3DL2 Reverb Path
//    +------+                      +------+
//
// To improve resource usage effeciency, if you wish to use the default path,
// it may be unmuted by sending a NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR with
// NVFX_SOURCE_IS_VOICE as the SourceId and NVFX_DESTINATION_IS_LOCAL_MIX as
// the DestinationId.  A connection descriptor with these settings will cause
// the Local FX Manager to ignore the channel mask specified when it was
// created and attach the voice directly to the global mix.  The I3DL2 Reverb
// connect to pin 5 should be treated as hard link and should not be detached.
//
// To terminate the audio path, the last FX (or voice) should be connected
// to a local mix.  The local mixes of all Local FX Manager will then be
// combined and routed to the standard output.  To connect the last FX (or
// voice) to the local mix, the destination ID should be set to
// NVFX_DESTINATION_IS_FINAL_MIX.  The number of pins is determined when the
// Local FX Manager is created if using a local terminating mix or is one
// of the global mix pins below if the FX Manager has no local mixes and
// connects directly to the global mixes.
//
// When is is desirable to detach an FX from a source FX, delete the
// downstreams FXs.  To detach a voice from a down stream FX, either mute
// the output going to the FX or attach the voice to another FX.
//
const NVFXDATA_FX_ID NVFX_SOURCE_IS_VOICE = 0;
const NVFXDATA_FX_ID NVFX_DESTINATION_IS_FINAL_MIX = 0;

//
// These are the pin IDs to attach the FX to the pin of the local mix.
//
typedef unsigned long NVA_FX_PIN;
typedef NVA_FX_PIN* PNVA_FX_PIN;

//
// These are the valid pins on the global mix.  They should only be used if
// connecting directly to the global mixes.  Otherwise, the local mix number
// should be used for the ID.
//
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_LEFT             = 0;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_RIGHT            = 1;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_CENTER           = 2;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_LFE              = 3;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_REAR_LEFT        = 4;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_REAR_RIGHT       = 5;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_3D_FRONT_LEFT    = 6;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_3D_FRONT_RIGHT   = 7;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_3D_REAR_LEFT     = 8;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_3D_REAR_RIGHT    = 9;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_REVERB           = 10;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_CHORUS_LEFT      = 11;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_CHORUS_RIGHT     = 12;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_REPLICATED_LEFT  = 13;
const NVA_FX_PIN NVA_FX_PIN_GLOBAL_MIX_REPLICATED_RIGHT = 14;

const unsigned long NUMBER_OF_GLOBAL_MIXES = NVA_FX_PIN_GLOBAL_MIX_REPLICATED_RIGHT + 1;
const NVA_FX_PIN NVA_FX_PIN_INVALID                     = -1;

//
// This pin is reserved on a source 3D voice for the center and reverb.
//
const NVA_FX_PIN NVA_FX_PIN_VOICE_RESERVED_3D_CENTER       = 4;
const NVA_FX_PIN NVA_FX_PIN_VOICE_RESERVED_3D_I3DL2_REVERB = 5;



//*****************************************************************************
// NVFXPROPERTY_FXMANAGER_ATTACH
// NVFXPROPERTY_FXMANAGER_DETACH
//*****************************************************************************

typedef struct
{
    //
    // The ID of the source.
    //
    NVFXDATA_FX_ID SourceId;

    //
    // The source pin on the source FX.
    //
    NVA_FX_PIN SourcePin;

    //
    // The ID of the destination.
    //
    NVFXDATA_FX_ID DestinationId;

    //
    // The destination pin on the destination FX.
    //
    NVA_FX_PIN DestinationPin;

} NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR, *PNVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR;


//*****************************************************************************
// NVFXPROPERTY_FXMANAGER_FXLIST
//*****************************************************************************

//
// The data for the NVFXPROPERTY_FXMANAGER_FXLIST property.
//
typedef struct
{
    NVFXDATA_FX_ID Id;
    NVFXINST_FXMANAGER_FXADD_DESCRIPTOR AddDescriptor;
    unsigned long IsEnabled;

} NVFXLIST_DESCRIPTOR, *PNVFXLIST_DESCRIPTOR;

typedef struct
{
    //
    // The FX information.
    //
    NVFXLIST_DESCRIPTOR Data[1];

} NVFXDATA_FXMANAGER_FXLIST_DESCRIPTOR, *PNVFXDATA_FXMANAGER_FXLIST_DESCRIPTOR;

//
// This is the maximum number of FXs allowed in an FX list.
//
const long MAX_NUMBER_OF_FXS = 50;


//*****************************************************************************
// NVFXPROPERTY_FXMANAGER_WIRELIST
//*****************************************************************************

//
// The data for the NVFXPROPERTY_FXMANAGER_WIRELIST property.
//
typedef struct
{
    //
    // The FX information.
    //
    NVFXDATA_FXMANAGER_CNNXN_DESCRIPTOR Data[1];

} NVFXDATA_FXMANAGER_WIRELIST_DESCRIPTOR, *PNVFXDATA_FXMANAGER_WIRELIST_DESCRIPTOR;



//*****************************************************************************
//*****************************************************************************
//
// NVIDIA FX Property Set
//
//*****************************************************************************
//*****************************************************************************

//
// NVPROPSETID_NvAudioFx
// {4D6C3344-6352-420d-99C8-EB903EF5775C}
//
DEFINE_GUID(NVPROPSETID_NvAudioFx,
0x4d6c3344, 0x6352, 0x420d, 0x99, 0xc8, 0xeb, 0x90, 0x3e, 0xf5, 0x77, 0x5c);

//
//
// Use this version when going through the Microsoft DSOUND API or
// prepending a PCPROPERTY_DSOUND_INSTANCE structure to your property.
//
// {D84EB93F-EA66-4830-8CB0-EA81FB260086}
//
DEFINE_GUID(NVPROPSETID_NvAudioFx_DirectSound,
0xd84eb93f, 0xea66, 0x4830, 0x8c, 0xb0, 0xea, 0x81, 0xfb, 0x26, 0x0, 0x86);


//
// Supported FX Properties:
//
//     NVFXPROPERTY_FX_ENABLE
//         This property is used to add and remove an FX to and from the
//         execution list of the FX processor.  The FX will be added in the
//         order it was allocated as is disabled by default.  To enable or
//         disable a large number of FXs, batch mode should be set on the FX
//         Manager.  The property is set by passing the NVFXINST_FX_DESCRIPTOR
//         structure as the instance data and NVFXDATA_FX_ENABLE as the data
//         parameter.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVFXPROPERTY_FX_FXPROPERTY
//         This is used to get or set a property supported by an FX.  The
//         instance data for this property is a NVFXINST_FXPROPERTY_DESCRIPTOR
//         structure.  The data is variable, dependent on desired property on
//         the FX.
//
//         This property supports IKsPropertySet::Get, IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
//
//     NVFXPROPERTY_FX_RESOURCE_USAGE
//         This property set outlines the resources required for the FX with the
//         current parameters.  Changing any parameter or enabling/disabling the
//         FX may change these values.  The instance data for this property is a
//         NVFXINST_FX_DESCRIPTOR structure and
//         NVFXDATA_AUDIO_RESOURCES_DESCRIPTOR as the data parameter.
//
//         This property supports IKsPropertySet::Get and
//         IKsPropertySet::QuerySupport methods only.
//
//
typedef enum
{
    NVFXPROPERTY_FX_ENABLE = 0,
    NVFXPROPERTY_FX_FXPROPERTY,
    NVFXPROPERTY_FX_RESOURCE_USAGE

} NVFXPROPERTY_FX, *PNVFXPROPERTY_FX;


//
// The general instance structure for FX property calls.
//
typedef struct
{
    //
    // The ID of the FX Manager.
    //
    NVFXMANAGER_ID FxManagerId;

    //
    // The ID of the audio FX.
    //
    NVFXDATA_FX_ID FxId;

} NVFXINST_FX_DESCRIPTOR, *PNVFXINST_FX_DESCRIPTOR;


//
// The data structure to pass for the NVFXPROPERTY_FX_ENABLE property.  Set
// to 0 to disable the FX, anything else enables the FX.
//
typedef unsigned long NVFXDATA_FX_ENABLE;
typedef NVFXDATA_FX_ENABLE *PNVFXDATA_FX_ENABLE;


//
// The instance structure for the NVFXPROPERTY_FX_FXPROPERTY property call.
//
typedef struct
{
    //
    // The instance of the FX.
    //
    NVFXINST_FX_DESCRIPTOR FxInstance;

    //
    // The type of property.  (Do not use the DirectSound version of the GUID
    // for KS or NVIDIA properties.)
    //
    GUID PropertyId;

    //
    // The property to set.
    //
    int Property;

} NVFXINST_FXPROPERTY_DESCRIPTOR, *PNVFXINST_FXPROPERTY_DESCRIPTOR;



//*****************************************************************************
//*****************************************************************************
//
// NVIDIA Voice Property Set
//
//*****************************************************************************
//*****************************************************************************

//
// NVPROPSETID_NvAudioVoice
// {7562951C-FD2F-4EF0-97AA-D02665C92F5D}
//
DEFINE_GUID(NVPROPSETID_NvAudioVoice,
0x7562951c, 0xfd2f, 0x4ef0, 0x97, 0xaa, 0xd0, 0x26, 0x65, 0xc9, 0x2f, 0x5d);

//
// Use this version when going through the Microsoft DSOUND API or
// prepending a PCPROPERTY_DSOUND_INSTANCE structure to your property.
//
// {3769F91E-36D9-43e6-BAF7-16FA71A5BFF2}
//
DEFINE_GUID(NVPROPSETID_NvAudioVoice_DirectSound,
0x3769f91e, 0x36d9, 0x43e6, 0xba, 0xf7, 0x16, 0xfa, 0x71, 0xa5, 0xbf, 0xf2);

//
// Supported Voice Properties:
//
//     In addition to the standard DirectSound, KS, EAX 1.0, EAX 2.0, and I3DL2
//     properties, the voice support these additional properties.
//
//     NVVOICEPROPERTY_VOLUMEPIN
//         This property allows the volume to be set on a specific output pin
//         for a voice.  The pin must be connected to an output path or its
//         output will not be heard.  The number of voice pins and which
//         channel they control is dependent upon the format of the sound
//         being played.  This volume is independent of the master voice
//         volume set with a mixer application.
//
//         This property supports IKsPropertySet::Get, IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
//     NVVOICEPROPERTY_VOLUMEPINTABLE
//         If setting more than one voice pin volume, a table should be used
//         to set the volumes more synchronously.
//
//         This property supports IKsPropertySet::Get, IKsPropertySet::Set and
//         IKsPropertySet::QuerySupport methods only.
//
//
typedef enum
{
    NVVOICEPROPERTY_VOLUMEPIN = 0,
    NVVOICEPROPERTY_VOLUMEPINTABLE

} NVVOICEPROPERTY, *PNVVOICEPROPERTY;

//
// The instance data structure to get/set the pin volume is the desired pin.
//
typedef struct
{
    //
    // The pin to set the volume on.
    //
    NVA_FX_PIN Pin;

} NVVOICEINST_VOLUMEPIN_PIN, *PNVVOICEINST_VOLUMEPIN_PIN;

//
// The data structure to get/set the pin volume is the volume.  For sets, the
// volume range is 0 to -6400 mB.
//
typedef long NVVOICEDATA_VOLUMEPIN_VOLUME;
typedef NVVOICEDATA_VOLUMEPIN_VOLUME* PNVVOICEDATA_VOLUMEPIN_VOLUME;


//
// The instance structure to pass to get/set the pin volume table.  The Pin
// field should be an array of NVVOICEINST_VOLUMEPIN_PIN items specifying pins.
// The data structure should be an array of NVVOICEDATA_VOLUMEPIN_VOLUME items
// with PinCount entries.
//
typedef struct
{
    //
    // The number of pins.
    //
    unsigned long PinCount;

    //
    // The first element of the pin array.
    //
    NVA_FX_PIN Pin[1];

} NVVOICEINST_VOLUMEPINTABLE, *PNVVOICEINST_VOLUMEPINTABLE;



#ifdef __cplusplus
}
#endif // __cplusplus


#endif
