//*****************************************************************************
//*****************************************************************************
//
// OpenALHelp.cpp
//
//
// Description:
// This file defines some common functions to help setup OpenAL.
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




#include "windows.h"
#define AL_NO_PROTOTYPES
#define AL_DYNAMIC_LINK
#define ALC_NO_PROTOTYPES
#define ALC_DYNAMIC_LINK
#include <al/alc.h>
#include <al/alut.h>
#include <al/nvalext.h>


//*****************************************************************************
//*****************************************************************************
//
// Forward Declarations
//
//*****************************************************************************
//*****************************************************************************

#define AL_FALSE                                 0
#define AL_TRUE                                  1

//
// AL Enumerations
//
extern ALenum AL_INVALID;
extern ALenum AL_NONE;
extern ALenum AL_NO_ERROR;
extern ALenum AL_SOURCE_RELATIVE;
extern ALenum AL_CONE_INNER_ANGLE;
extern ALenum AL_CONE_OUTER_ANGLE;
extern ALenum AL_PITCH;
extern ALenum AL_POSITION;
extern ALenum AL_DIRECTION;
extern ALenum AL_VELOCITY;
extern ALenum AL_LOOPING;
extern ALenum AL_BUFFER;
extern ALenum AL_GAIN;
extern ALenum AL_MIN_GAIN;
extern ALenum AL_MAX_GAIN;
extern ALenum AL_ORIENTATION;
extern ALenum AL_SOURCE_STATE;
extern ALenum AL_INITIAL;
extern ALenum AL_PLAYING;
extern ALenum AL_PAUSED;
extern ALenum AL_STOPPED;
extern ALenum AL_BUFFERS_QUEUED;
extern ALenum AL_BUFFERS_PROCESSED;
extern ALenum AL_REFERENCE_DISTANCE;
extern ALenum AL_ROLLOFF_FACTOR;
extern ALenum AL_CONE_OUTER_GAIN;
extern ALenum AL_MAX_DISTANCE;
extern ALenum AL_FORMAT_MONO8;
extern ALenum AL_FORMAT_MONO16;
extern ALenum AL_FORMAT_STEREO8;
extern ALenum AL_FORMAT_STEREO16;
extern ALenum AL_FREQUENCY;
extern ALenum AL_BITS;
extern ALenum AL_CHANNELS;
extern ALenum AL_SIZE;
extern ALenum AL_DATA;
extern ALenum AL_INVALID_NAME;
extern ALenum AL_INVALID_ENUM;
extern ALenum AL_INVALID_VALUE;
extern ALenum AL_INVALID_OPERATION;
extern ALenum AL_OUT_OF_MEMORY;
extern ALenum AL_VENDOR;
extern ALenum AL_VERSION;
extern ALenum AL_RENDERER;
extern ALenum AL_EXTENSIONS;
extern ALenum AL_DOPPLER_FACTOR;
extern ALenum AL_DOPPLER_VELOCITY;
extern ALenum AL_DISTANCE_MODEL;
extern ALenum AL_INVERSE_DISTANCE;
extern ALenum AL_INVERSE_DISTANCE_CLAMPED;

//
// ALC Enumerations
//
extern ALenum ALC_INVALID;
extern ALenum ALC_NO_ERROR;
extern ALenum ALC_MAJOR_VERSION;
extern ALenum ALC_MINOR_VERSION;
extern ALenum ALC_ATTRIBUTES_SIZE;
extern ALenum ALC_ALL_ATTRIBUTES;
extern ALenum ALC_DEFAULT_DEVICE_SPECIFIER;
extern ALenum ALC_DEVICE_SPECIFIER;
extern ALenum ALC_EXTENSIONS;
extern ALenum ALC_INVALID_DEVICE;
extern ALenum ALC_INVALID_CONTEXT;
extern ALenum ALC_INVALID_ENUM;
extern ALenum ALC_INVALID_VALUE;

//
// NVIDIA Extensions
//
extern ALenum AL_FORMAT_PCM_NV;
extern ALenum AL_FORMAT_NVADPCM_NV;

extern ALuint AL_CHANNELMASK_NONE_NV;
extern ALuint AL_CHANNELMASK_FRONT_LEFT_NV;
extern ALuint AL_CHANNELMASK_FRONT_RIGHT_NV;
extern ALuint AL_CHANNELMASK_FRONT_CENTER_NV;
extern ALuint AL_CHANNELMASK_LOW_FREQUENCY_NV;
extern ALuint AL_CHANNELMASK_BACK_LEFT_NV;
extern ALuint AL_CHANNELMASK_BACK_RIGHT_NV;

extern ALuint AL_CHANNELMASK_STEREO_NV;
extern ALuint AL_CHANNELMASK_QUAD_NV;
extern ALuint AL_CHANNELMASK_5POINT1_NV;



//*****************************************************************************
// InitOpenAL
//*****************************************************************************
// Returns AL_TRUE if OpenAL was successfully setup.
//
ALboolean InitOpenAL
(
    IN  HINSTANCE Dll
);


//*****************************************************************************
// InitOpenALStandardEnums
//*****************************************************************************
// Initializes the OpenAL enumerations for the current context.
// Return AL_TRUE is the enumerations were successfully initialized.
//
ALboolean InitOpenALStandardEnums
(
    IN  ALdevice* Device
);

//*****************************************************************************
// InitOpenALNvidiaExtension
//*****************************************************************************
// Initializes the OpenAL NVIDIA extension variables for the current context.
// Return AL_TRUE is the extension variables were successfully initialized.
//
ALboolean InitOpenALNvidiaExtension();

