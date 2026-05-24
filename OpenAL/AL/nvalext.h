//*****************************************************************************
//*****************************************************************************
//
// nvalext.h
//
//
// Description:
// This file NVIDIA's extensions to OpenAL.
//
//
//
//            Copyright NVIDIA Corporation.  All rights reserved.
//
//     NOTICE TO USER:   The source code  is copyrighted under  U.S. and
//     international laws.  Users and possessors of this source code are
//     hereby granted a nonexclusive,  royalty-free copyright license to
//     use this code in individual and commercial software.
//
//     Any use of this source code must include,  in the user documenta-
//     tion and  internal comments to the code,  notices to the end user
//     as follows:
//
//     NVIDIA CORPORATION MAKES NO REPRESENTATION ABOUT THE  SUITABILITY
//     OF  THIS SOURCE  CODE  FOR ANY PURPOSE.  IT IS  PROVIDED  "AS IS"
//     WITHOUT EXPRESS OR IMPLIED WARRANTY OF  ANY KIND.  NVIDIA CORPOR-
//     ATION DISCLAIMS ALL WARRANTIES  WITH REGARD  TO THIS SOURCE CODE,
//     INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGE-
//     MENT,  AND FITNESS  FOR A PARTICULAR PURPOSE.   IN NO EVENT SHALL
//     NVIDIA CORPORATION  BE LIABLE FOR  ANY SPECIAL,  INDIRECT,  INCI-
//     DENTAL, OR CONSEQUENTIAL DAMAGES,  OR ANY DAMAGES  WHATSOEVER RE-
//     SULTING FROM LOSS OF USE,  DATA OR PROFITS,  WHETHER IN AN ACTION
//     OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT OF
//     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
//
//     U.S. Government  End  Users.   This source code  is a "commercial
//     item,"  as that  term is  defined at  48 C.F.R. 2.101 (OCT 1995),
//     consisting  of "commercial  computer  software"  and  "commercial
//     computer  software  documentation,"  as such  terms  are  used in
//     48 C.F.R. 12.212 (SEPT 1995)  and is provided to the U.S. Govern-
//     ment only as  a commercial end item.   Consistent with  48 C.F.R.
//     12.212 and  48 C.F.R. 227.7202-1 through  227.7202-4 (JUNE 1995),
//     all U.S. Government End Users  acquire the source code  with only
//     those rights set forth herein.
//
//                                                                    Rev A
//
//
//*****************************************************************************
//*****************************************************************************



#ifndef __NVALEXT_H__
#define __NVALEXT_H__


#include "al.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


//*****************************************************************************
//*****************************************************************************
//
// Defines
//
//*****************************************************************************
//*****************************************************************************

//
// This describes the NVIDIA buffer format structure.
//
typedef struct
{
    //
    // The supported format enumerated types are:
    //
    //     AL_FORMAT_MONO8
    //     AL_FORMAT_MONO16
    //     AL_FORMAT_STEREO8
    //     AL_FORMAT_STEREO16
    //     AL_FORMAT_PCM_NV
    //     AL_FORMAT_NVADPCM_NV
    //
    // On NVIDIA hardware when using NVADPCM, only mono or stereo is supported.  Formats
    // that explictly imply the channels and bit resolutions are not required to fill the
    // remaining entries to this structure (e.g. AL_FORMAT_MONO8).
    //
    ALenum FormatEnum;

    //
    // On NVIDIA hardware, the sample rate must be between 0Hz - 181440Hz.
    // Other implemenations that choose to support this extension may not be so limited.
    //
    ALuint SampleRate;

    //
    // On NVIDIA hardware, this number must be less than the number of bits in the BitsPerSample.
    //
    ALuint ValidBitsPerSample;

    //
    // On NVIDIA hardware, the number of bits per sample is limited to 8, 16, 24, or 32 for
    // AL_FORMAT_PCM_NV.  AL_FORMAT_NVADPCM_NV is limited to 4 bits per sample.
    // Other implementations that choose to support this extension may not be so limited.
    //
    ALuint BitsPerSample;

    //
    // On NVIDIA hardware, the number of interleaved channels is limited to 1, 2, 4, or 6.
    // Other implementations that choose to support this extension may not be so limited to the numbers
    // of interleaved channels.  The only implementation requirement for this extension is that for 3D
    // positional audio, only mono buffers may be specified.
    //
    ALuint NumberOfChannels;

    //
    // For buffers with a single channel, the supported channel masks that may be OR'd to
    // position that channel out of a given speaker (assuming it exists) are:
    //
    //     AL_CHANNELMASK_NONE_NV       <-- Indicates the sound is to be positioned in 3D space accordingly
    //     AL_CHANNELMASK_FRONT_LEFT_NV
    //     AL_CHANNELMASK_FRONT_RIGHT_NV
    //     AL_CHANNELMASK_FRONT_CENTER_NV
    //     AL_CHANNELMASK_LOW_FREQUENCY_NV
    //     AL_CHANNELMASK_BACK_LEFT_NV
    //     AL_CHANNELMASK_BACK_RIGHT_NV
    //
    // On NVIDIA hardware where the number of supported interleaved channels are 2, 4, or 6, the
    // supported channel masks are listed below along with their respective interleaved channel ordering.
    // Buffers with 2-channel interleaved data also support the AL_CHANNELMASK_QUAD_NV channel mask to
    // replicate the stereo data into the rear channels.
    //
    //     AL_CHANNELMASK_STEREO_NV     <-- FRONT LEFT and FRONT RIGHT
    //     AL_CHANNELMASK_QUAD_NV       <-- FRONT LEFT, FRONT RIGHT, BACK LEFT, and BACK RIGHT
    //     AL_CHANNELMASK_5POINT1_NV    <-- FRONT LEFT, FRONT RIGHT, CENTER, LFE, BACK LEFT, and BACK RIGHT
    //
    // Other implementations that choose to support this extension may not be so limited
    // and may support any combination of channels and masks.
    //
    ALuint ChannelMask;

} ALbufferFormatNV;



//*****************************************************************************
// bufferDataNV
//*****************************************************************************
// An implemenation is free to convert these formats as needed to any internal
// format they wish.  If this function is used, only buffers that are mono
// and have a channel mask of CHANNEL_NONE_NV can support 3D properties once
// assigned to a source.  Any source that has had 3D properties set prior to
// buffer being assigned, will ignore 3D properties.
//
// If the format is not supported in any form, alcGetError will return an error.
//
typedef ALvoid (ALAPIENTRY *ALAPI_BUFFER_DATA_NV)(ALuint bufferName, ALbufferFormatNV* bufferFormat, ALvoid* data, ALsizei size);



#ifndef AL_NO_PROTOTYPES
    ALvoid ALAPIENTRY alBufferDataNV(ALuint bufferName, ALbufferFormatNV* bufferFormat, ALvoid* data, ALsizei size);
#else /* AL_NO_PROTOTYPES */

    #ifdef ALAPI_DEFINE_GLOBAL_VARS
        ALAPI_BUFFER_DATA_NV        alBufferDataNV = 0;
    #else
        extern ALAPI_BUFFER_DATA_NV alBufferDataNV;
    #endif

#endif /* AL_NO_PROTOTYPES */




#ifdef __cplusplus
}
#endif // __cplusplus


#endif
