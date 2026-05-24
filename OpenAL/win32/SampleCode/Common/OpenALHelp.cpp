//*****************************************************************************
//*****************************************************************************
//
// OpenALHelp.cpp
//
//
// Description:
// This file implements some common functions to help setup OpenAL.
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




#include "StdAfx.h"
#define ALCAPI_DEFINE_GLOBAL_VARS
#define ALAPI_DEFINE_GLOBAL_VARS
#include "OpenALHelp.h"

//*****************************************************************************
//*****************************************************************************
//
// Globals
//
//*****************************************************************************
//*****************************************************************************

//
// AL Enumerations
//
ALenum AL_INVALID                      = 0;
ALenum AL_NONE                         = 0;
ALenum AL_NO_ERROR                     = 0;
ALenum AL_SOURCE_RELATIVE              = 0;
ALenum AL_CONE_INNER_ANGLE             = 0;
ALenum AL_CONE_OUTER_ANGLE             = 0;
ALenum AL_PITCH                        = 0;
ALenum AL_POSITION                     = 0;
ALenum AL_DIRECTION                    = 0;
ALenum AL_VELOCITY                     = 0;
ALenum AL_LOOPING                      = 0;
ALenum AL_BUFFER                       = 0;
ALenum AL_GAIN                         = 0;
ALenum AL_MIN_GAIN                     = 0;
ALenum AL_MAX_GAIN                     = 0;
ALenum AL_ORIENTATION                  = 0;
ALenum AL_SOURCE_STATE                 = 0;
ALenum AL_INITIAL                      = 0;
ALenum AL_PLAYING                      = 0;
ALenum AL_PAUSED                       = 0;
ALenum AL_STOPPED                      = 0;
ALenum AL_BUFFERS_QUEUED               = 0;
ALenum AL_BUFFERS_PROCESSED            = 0;
ALenum AL_REFERENCE_DISTANCE           = 0;
ALenum AL_ROLLOFF_FACTOR               = 0;
ALenum AL_CONE_OUTER_GAIN              = 0;
ALenum AL_MAX_DISTANCE                 = 0;
ALenum AL_FORMAT_MONO8                 = 0;
ALenum AL_FORMAT_MONO16                = 0;
ALenum AL_FORMAT_STEREO8               = 0;
ALenum AL_FORMAT_STEREO16              = 0;
ALenum AL_FREQUENCY                    = 0;
ALenum AL_BITS                         = 0;
ALenum AL_CHANNELS                     = 0;
ALenum AL_SIZE                         = 0;
ALenum AL_DATA                         = 0;
ALenum AL_INVALID_NAME                 = 0;
ALenum AL_INVALID_ENUM                 = 0;
ALenum AL_INVALID_VALUE                = 0;
ALenum AL_INVALID_OPERATION            = 0;
ALenum AL_OUT_OF_MEMORY                = 0;
ALenum AL_VENDOR                       = 0;
ALenum AL_VERSION                      = 0;
ALenum AL_RENDERER                     = 0;
ALenum AL_EXTENSIONS                   = 0;
ALenum AL_DOPPLER_FACTOR               = 0;
ALenum AL_DOPPLER_VELOCITY             = 0;
ALenum AL_DISTANCE_MODEL               = 0;
ALenum AL_INVERSE_DISTANCE             = 0;
ALenum AL_INVERSE_DISTANCE_CLAMPED     = 0;

//
// ALC Enumerations
//
ALenum ALC_INVALID                     = 0;
ALenum ALC_NO_ERROR                    = 0;
ALenum ALC_MAJOR_VERSION               = 0;
ALenum ALC_MINOR_VERSION               = 0;
ALenum ALC_ATTRIBUTES_SIZE             = 0;
ALenum ALC_ALL_ATTRIBUTES              = 0;
ALenum ALC_DEFAULT_DEVICE_SPECIFIER    = 0;
ALenum ALC_DEVICE_SPECIFIER            = 0;
ALenum ALC_EXTENSIONS                  = 0;
ALenum ALC_INVALID_DEVICE              = 0;
ALenum ALC_INVALID_CONTEXT             = 0;
ALenum ALC_INVALID_ENUM                = 0;
ALenum ALC_INVALID_VALUE               = 0;

//
// NVIDIA Extensions
//
ALenum AL_FORMAT_PCM_NV                = 0;
ALenum AL_FORMAT_NVADPCM_NV            = 0;

ALuint AL_CHANNELMASK_NONE_NV          = 0;
ALuint AL_CHANNELMASK_FRONT_LEFT_NV    = 0;
ALuint AL_CHANNELMASK_FRONT_RIGHT_NV   = 0;
ALuint AL_CHANNELMASK_FRONT_CENTER_NV  = 0;
ALuint AL_CHANNELMASK_LOW_FREQUENCY_NV = 0;
ALuint AL_CHANNELMASK_BACK_LEFT_NV     = 0;
ALuint AL_CHANNELMASK_BACK_RIGHT_NV    = 0;

ALuint AL_CHANNELMASK_STEREO_NV        = 0;
ALuint AL_CHANNELMASK_QUAD_NV          = 0;
ALuint AL_CHANNELMASK_5POINT1_NV       = 0;


//*****************************************************************************
// InitOpenAL
//*****************************************************************************
//
ALboolean InitOpenAL
(
    IN  HINSTANCE Dll
)
{
    ALboolean  alFxns = FALSE;
    ALboolean alcFxns = FALSE;


    //
    // Get the OpenAL 1.0 Entry points.
    //

    // AL:
    alEnable               = (ALAPI_ENABLE)GetProcAddress(Dll, "alEnable");
    alDisable              = (ALAPI_DISABLE)GetProcAddress(Dll, "alDisable");
    alIsEnabled            = (ALAPI_IS_ENABLED)GetProcAddress(Dll, "alIsEnabled");

    alGetBoolean           = (ALAPI_GET_BOOLEAN)GetProcAddress(Dll, "alGetBoolean");
    alGetInteger           = (ALAPI_GET_INTEGER)GetProcAddress(Dll, "alGetInteger");
    alGetFloat             = (ALAPI_GET_FLOAT)GetProcAddress(Dll, "alGetFloat");
    alGetDouble            = (ALAPI_GET_DOUBLE)GetProcAddress(Dll, "alGetDouble");
    alGetBooleanv          = (ALAPI_GET_BOOLEANV)GetProcAddress(Dll, "alGetBooleanv");
    alGetIntegerv          = (ALAPI_GET_INTEGERV)GetProcAddress(Dll, "alGetIntegerv");
    alGetFloatv            = (ALAPI_GET_FLOATV)GetProcAddress(Dll, "alGetFloatv");
    alGetDoublev           = (ALAPI_GET_DOUBLEV)GetProcAddress(Dll, "alGetDoublev");
    alGetString            = (ALAPI_GET_STRING)GetProcAddress(Dll, "alGetString");

    alGetError             = (ALAPI_GET_ERROR)GetProcAddress(Dll, "alGetError");

    alIsExtensionPresent   = (ALAPI_IS_EXTENSION_PRESENT)GetProcAddress(Dll, "alIsExtensionPresent");
    alGetProcAddress       = (ALAPI_GET_PROC_ADDRESS)GetProcAddress(Dll, "alGetProcAddress");
    alGetEnumValue         = (ALAPI_GET_ENUM_VALUE)GetProcAddress(Dll, "alGetEnumValue");

    alListeneri            = (ALAPI_LISTENERI)GetProcAddress(Dll, "alListeneri");
    alListenerf            = (ALAPI_LISTENERF)GetProcAddress(Dll, "alListenerf");
    alListener3f           = (ALAPI_LISTENER3F)GetProcAddress(Dll, "alListener3f");
    alListenerfv           = (ALAPI_LISTENERFV)GetProcAddress(Dll, "alListenerfv");
    alGetListeneri         = (ALAPI_GET_LISTENERI)GetProcAddress(Dll, "alGetListeneri");
    alGetListenerf         = (ALAPI_GET_LISTENERF)GetProcAddress(Dll, "alGetListenerf");
    alGetListener3f        = (ALAPI_GET_LISTENER3F)GetProcAddress(Dll, "alGetListener3f");
    alGetListenerfv        = (ALAPI_GET_LISTENERFV)GetProcAddress(Dll, "alGetListenerfv");

    alGenSources           = (ALAPI_GEN_SOURCES)GetProcAddress(Dll, "alGenSources");
    alDeleteSources        = (ALAPI_DELETE_SOURCES)GetProcAddress(Dll, "alDeleteSources");
    alIsSource             = (ALAPI_IS_SOURCE)GetProcAddress(Dll, "alIsSource");
    alSourcei              = (ALAPI_SOURCEI)GetProcAddress(Dll, "alSourcei");
    alSourcef              = (ALAPI_SOURCEF)GetProcAddress(Dll, "alSourcef");
    alSource3f             = (ALAPI_SOURCE3F)GetProcAddress(Dll, "alSource3f");
    alSourcefv             = (ALAPI_SOURCEFV)GetProcAddress(Dll, "alSourcefv");
    alGetSourcei           = (ALAPI_GET_SOURCEI)GetProcAddress(Dll, "alGetSourcei");
    alGetSourcef           = (ALAPI_GET_SOURCEF)GetProcAddress(Dll, "alGetSourcef");
    alGetSourcefv          = (ALAPI_GET_SOURCEFV)GetProcAddress(Dll, "alGetSourcefv");
    alSourcePlayv          = (ALAPI_SOURCE_PLAYV)GetProcAddress(Dll, "alSourcePlayv");
    alSourceStopv          = (ALAPI_SOURCE_STOPV)GetProcAddress(Dll, "alSourceStopv");
    alSourcePlay           = (ALAPI_SOURCE_PLAY)GetProcAddress(Dll, "alSourcePlay");
    alSourcePause          = (ALAPI_SOURCE_PAUSE)GetProcAddress(Dll, "alSourcePause");
    alSourceStop           = (ALAPI_SOURCE_STOP)GetProcAddress(Dll, "alSourceStop");

    alGenBuffers           = (ALAPI_GEN_BUFFERS)GetProcAddress(Dll, "alGenBuffers");
    alDeleteBuffers        = (ALAPI_DELETE_BUFFERS)GetProcAddress(Dll, "alDeleteBuffers");
    alIsBuffer             = (ALAPI_IS_BUFFER)GetProcAddress(Dll, "alIsBuffer");
    alBufferData           = (ALAPI_BUFFER_DATA)GetProcAddress(Dll, "alBufferData");
    alGetBufferi           = (ALAPI_GET_BUFFERI)GetProcAddress(Dll, "alGetBufferi");
    alGetBufferf           = (ALAPI_GET_BUFFERF)GetProcAddress(Dll, "alGetBufferf");

    alSourceQueueBuffers   = (ALAPI_SOURCE_QUEUE_BUFFERS)GetProcAddress(Dll, "alSourceQueueBuffers");
    alSourceUnqueueBuffers = (ALAPI_SOURCE_UNQUEUE_BUFFERS)GetProcAddress(Dll, "alSourceUnqueueBuffers");

    alDistanceModel        = (ALAPI_DISTANCE_MODEL)GetProcAddress(Dll, "alDistanceModel");
    alDopplerFactor        = (ALAPI_DOPPLER_FACTOR)GetProcAddress(Dll, "alDopplerFactor");
    alDopplerVelocity      = (ALAPI_DOPPLER_VELOCITY)GetProcAddress(Dll, "alDopplerVelocity");


    alFxns = (alEnable               &&
              alDisable              &&
              alIsEnabled            &&

              alGetBoolean           &&
              alGetInteger           &&
              alGetFloat             &&
              alGetDouble            &&
              alGetBooleanv          &&
              alGetIntegerv          &&
              alGetFloatv            &&
              alGetDoublev           &&
              alGetString            &&

              alGetError             &&

              alIsExtensionPresent   &&
              alGetProcAddress       &&
              alGetEnumValue         &&

              alListeneri            &&
              alListenerf            &&
              alListener3f           &&
              alListenerfv           &&
              alGetListeneri         &&
              alGetListenerf         &&
              alGetListener3f        &&
              alGetListenerfv        &&

              alGenSources           &&
              alDeleteSources        &&
              alIsSource             &&
              alSourcei              &&
              alSourcef              &&
              alSource3f             &&
              alSourcefv             &&
              alGetSourcei           &&
              alGetSourcef           &&
              alGetSourcefv          &&
              alSourcePlayv          &&
              alSourceStopv          &&
              alSourcePlay           &&
              alSourcePause          &&
              alSourceStop           &&

              alGenBuffers           &&
              alDeleteBuffers        &&
              alIsBuffer             &&
              alBufferData           &&
              alGetBufferi           &&
              alGetBufferf           &&

              alSourceQueueBuffers   &&
              alSourceUnqueueBuffers &&

              alDistanceModel        &&
              alDopplerFactor        &&
              alDopplerVelocity);



    // ALC:
    alcGetString          = (ALCAPI_GET_STRING)GetProcAddress(Dll, "alcGetString");
    alcGetIntegerv        = (ALCAPI_GET_INTEGERV)GetProcAddress(Dll, "alcGetIntegerv");

    alcOpenDevice         = (ALCAPI_OPEN_DEVICE)GetProcAddress(Dll, "alcOpenDevice");
    alcCloseDevice        = (ALCAPI_CLOSE_DEVICE)GetProcAddress(Dll, "alcCloseDevice");
    alcEnumerateDevices   = (ALCAPI_ENUMERATE_DEVICES)GetProcAddress(Dll, "alcEnumerateDevices");

    alcCreateContext      = (ALCAPI_CREATE_CONTEXT)GetProcAddress(Dll, "alcCreateContext");
    alcMakeContextCurrent = (ALCAPI_MAKE_CONTEXT_CURRENT)GetProcAddress(Dll, "alcMakeContextCurrent");
    alcProcessContext     = (ALCAPI_PROCESS_CONTEXT)GetProcAddress(Dll, "alcProcessContext");
    alcGetCurrentContext  = (ALCAPI_GET_CURRENT_CONTEXT)GetProcAddress(Dll, "alcGetCurrentContext");
    alcGetContextsDevice  = (ALCAPI_GET_CONTEXTS_DEVICE)GetProcAddress(Dll, "alcGetContextsDevice");
    alcSuspendContext     = (ALCAPI_SUSPEND_CONTEXT)GetProcAddress(Dll, "alcSuspendContext");
    alcDestroyContext     = (ALCAPI_DESTROY_CONTEXT)GetProcAddress(Dll, "alcDestroyContext");

    alcGetError           = (ALCAPI_GET_ERROR)GetProcAddress(Dll, "alcGetError");

    alcIsExtensionPresent = (ALCAPI_IS_EXTENSION_PRESENT)GetProcAddress(Dll, "alcIsExtensionPresent");
    alcGetProcAddress     = (ALCAPI_GET_PROC_ADDRESS)GetProcAddress(Dll, "alcGetProcAddress");
    alcGetEnumValue       = (ALCAPI_GET_ENUM_VALUE)GetProcAddress(Dll, "alcGetEnumValue");


    alcFxns = (alcGetString          &&
               alcGetIntegerv        &&
               alcOpenDevice         &&
               alcCloseDevice        &&
               alcEnumerateDevices   &&

               alcCreateContext      &&
               alcMakeContextCurrent &&
               alcProcessContext     &&
               alcGetCurrentContext  &&
               alcGetContextsDevice  &&
               alcSuspendContext     &&
               alcDestroyContext     &&

               alcGetError           &&

               alcIsExtensionPresent &&
               alcGetProcAddress     &&
               alcGetEnumValue);


    return alcFxns && alFxns;
}


//*****************************************************************************
// InitOpenALStandardEnums
//*****************************************************************************
//
ALboolean InitOpenALStandardEnums
(
    ALdevice* Device
)
{
    //
    // AL Enumerations
    //
    AL_INVALID                   = alGetEnumValue((ALubyte*)"AL_INVALID");
    AL_NONE                      = alGetEnumValue((ALubyte*)"AL_NONE");
    AL_NO_ERROR                  = alGetEnumValue((ALubyte*)"AL_NO_ERROR");
    AL_SOURCE_RELATIVE           = alGetEnumValue((ALubyte*)"AL_SOURCE_RELATIVE");
    AL_CONE_INNER_ANGLE          = alGetEnumValue((ALubyte*)"AL_CONE_INNER_ANGLE");
    AL_CONE_OUTER_ANGLE          = alGetEnumValue((ALubyte*)"AL_CONE_OUTER_ANGLE");
    AL_PITCH                     = alGetEnumValue((ALubyte*)"AL_PITCH");
    AL_POSITION                  = alGetEnumValue((ALubyte*)"AL_POSITION");
    AL_DIRECTION                 = alGetEnumValue((ALubyte*)"AL_DIRECTION");
    AL_VELOCITY                  = alGetEnumValue((ALubyte*)"AL_VELOCITY");
    AL_LOOPING                   = alGetEnumValue((ALubyte*)"AL_LOOPING");
    AL_BUFFER                    = alGetEnumValue((ALubyte*)"AL_BUFFER");
    AL_GAIN                      = alGetEnumValue((ALubyte*)"AL_GAIN");
    AL_MIN_GAIN                  = alGetEnumValue((ALubyte*)"AL_MIN_GAIN");
    AL_MAX_GAIN                  = alGetEnumValue((ALubyte*)"AL_MAX_GAIN");
    AL_ORIENTATION               = alGetEnumValue((ALubyte*)"AL_ORIENTATION");
    AL_SOURCE_STATE              = alGetEnumValue((ALubyte*)"AL_SOURCE_STATE");
    AL_INITIAL                   = alGetEnumValue((ALubyte*)"AL_INITIAL");
    AL_PLAYING                   = alGetEnumValue((ALubyte*)"AL_PLAYING");
    AL_PAUSED                    = alGetEnumValue((ALubyte*)"AL_PAUSED");
    AL_STOPPED                   = alGetEnumValue((ALubyte*)"AL_STOPPED");
    AL_BUFFERS_QUEUED            = alGetEnumValue((ALubyte*)"AL_BUFFERS_QUEUED");
    AL_BUFFERS_PROCESSED         = alGetEnumValue((ALubyte*)"AL_BUFFERS_PROCESSED");
    AL_REFERENCE_DISTANCE        = alGetEnumValue((ALubyte*)"AL_REFERENCE_DISTANCE");
    AL_ROLLOFF_FACTOR            = alGetEnumValue((ALubyte*)"AL_ROLLOFF_FACTOR");
    AL_CONE_OUTER_GAIN           = alGetEnumValue((ALubyte*)"AL_CONE_OUTER_GAIN");
    AL_MAX_DISTANCE              = alGetEnumValue((ALubyte*)"AL_MAX_DISTANCE");
    AL_FORMAT_MONO8              = alGetEnumValue((ALubyte*)"AL_FORMAT_MONO8");
    AL_FORMAT_MONO16             = alGetEnumValue((ALubyte*)"AL_FORMAT_MONO16");
    AL_FORMAT_STEREO8            = alGetEnumValue((ALubyte*)"AL_FORMAT_STEREO8");
    AL_FORMAT_STEREO16           = alGetEnumValue((ALubyte*)"AL_FORMAT_STEREO16");
    AL_FREQUENCY                 = alGetEnumValue((ALubyte*)"AL_FREQUENCY");
    AL_BITS                      = alGetEnumValue((ALubyte*)"AL_BITS");
    AL_CHANNELS                  = alGetEnumValue((ALubyte*)"AL_CHANNELS");
    AL_SIZE                      = alGetEnumValue((ALubyte*)"AL_SIZE");
    AL_DATA                      = alGetEnumValue((ALubyte*)"AL_DATA");
    AL_INVALID_NAME              = alGetEnumValue((ALubyte*)"AL_INVALID_NAME");
    AL_INVALID_ENUM              = alGetEnumValue((ALubyte*)"AL_INVALID_ENUM");
    AL_INVALID_VALUE             = alGetEnumValue((ALubyte*)"AL_INVALID_VALUE");
    AL_INVALID_OPERATION         = alGetEnumValue((ALubyte*)"AL_INVALID_OPERATION");
    AL_OUT_OF_MEMORY             = alGetEnumValue((ALubyte*)"AL_OUT_OF_MEMORY");
    AL_VENDOR                    = alGetEnumValue((ALubyte*)"AL_VENDOR");
    AL_VERSION                   = alGetEnumValue((ALubyte*)"AL_VERSION");
    AL_RENDERER                  = alGetEnumValue((ALubyte*)"AL_RENDERER");
    AL_EXTENSIONS                = alGetEnumValue((ALubyte*)"AL_EXTENSIONS");
    AL_DOPPLER_FACTOR            = alGetEnumValue((ALubyte*)"AL_DOPPLER_FACTOR");
    AL_DOPPLER_VELOCITY          = alGetEnumValue((ALubyte*)"AL_DOPPLER_VELOCITY");
    AL_DISTANCE_MODEL            = alGetEnumValue((ALubyte*)"AL_DISTANCE_MODEL");
    AL_INVERSE_DISTANCE          = alGetEnumValue((ALubyte*)"AL_INVERSE_DISTANCE");
    AL_INVERSE_DISTANCE_CLAMPED  = alGetEnumValue((ALubyte*)"AL_INVERSE_DISTANCE_CLAMPED");
    if(alGetError() != AL_NO_ERROR)
    {
        return AL_FALSE;
    }

    //
    // ALC Enumerations
    //
    ALC_INVALID                  = alcGetEnumValue(Device, (ALubyte*)"ALC_INVALID");
    ALC_NO_ERROR                 = alcGetEnumValue(Device, (ALubyte*)"ALC_NO_ERROR");
    ALC_MAJOR_VERSION            = alcGetEnumValue(Device, (ALubyte*)"ALC_MAJOR_VERSION");
    ALC_MINOR_VERSION            = alcGetEnumValue(Device, (ALubyte*)"ALC_MINOR_VERSION");
    ALC_ATTRIBUTES_SIZE          = alcGetEnumValue(Device, (ALubyte*)"ALC_ATTRIBUTES_SIZE");
    ALC_ALL_ATTRIBUTES           = alcGetEnumValue(Device, (ALubyte*)"ALC_ALL_ATTRIBUTES");
    ALC_DEFAULT_DEVICE_SPECIFIER = alcGetEnumValue(Device, (ALubyte*)"ALC_DEFAULT_DEVICE_SPECIFIER");
    ALC_DEVICE_SPECIFIER         = alcGetEnumValue(Device, (ALubyte*)"ALC_DEVICE_SPECIFIER");
    ALC_EXTENSIONS               = alcGetEnumValue(Device, (ALubyte*)"ALC_EXTENSIONS");
    ALC_INVALID_DEVICE           = alcGetEnumValue(Device, (ALubyte*)"ALC_INVALID_DEVICE");
    ALC_INVALID_CONTEXT          = alcGetEnumValue(Device, (ALubyte*)"ALC_INVALID_CONTEXT");
    ALC_INVALID_ENUM             = alcGetEnumValue(Device, (ALubyte*)"ALC_INVALID_ENUM");
    ALC_INVALID_VALUE            = alcGetEnumValue(Device, (ALubyte*)"ALC_INVALID_VALUE");
    if(alGetError() != AL_NO_ERROR)
    {
        return AL_FALSE;
    }

    return AL_TRUE;
}


//*****************************************************************************
// InitOpenALNvidiaExtension
//*****************************************************************************
//
ALboolean InitOpenALNvidiaExtension()
{
    alBufferDataNV                  = (ALAPI_BUFFER_DATA_NV)alGetProcAddress((ALubyte*)"alBufferDataNV");
    if(!alBufferDataNV)
    {
        return AL_FALSE;
    }

    AL_FORMAT_PCM_NV                = alGetEnumValue((ALubyte*)"AL_FORMAT_PCM_NV");
    AL_FORMAT_NVADPCM_NV            = alGetEnumValue((ALubyte*)"AL_FORMAT_NVADPCM_NV");

    AL_CHANNELMASK_NONE_NV          = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_NONE_NV");
    AL_CHANNELMASK_FRONT_LEFT_NV    = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_FRONT_LEFT_NV");
    AL_CHANNELMASK_FRONT_RIGHT_NV   = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_FRONT_RIGHT_NV");
    AL_CHANNELMASK_FRONT_CENTER_NV  = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_FRONT_CENTER_NV");
    AL_CHANNELMASK_LOW_FREQUENCY_NV = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_LOW_FREQUENCY_NV");
    AL_CHANNELMASK_BACK_LEFT_NV     = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_BACK_LEFT_NV");
    AL_CHANNELMASK_BACK_RIGHT_NV    = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_BACK_RIGHT_NV");

    AL_CHANNELMASK_STEREO_NV        = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_STEREO_NV");
    AL_CHANNELMASK_QUAD_NV          = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_QUAD_NV");
    AL_CHANNELMASK_5POINT1_NV       = alGetEnumValue((ALubyte*)"AL_CHANNELMASK_5POINT1_NV");

    if(alGetError() != AL_NO_ERROR)
    {
        return AL_FALSE;
    }

    return TRUE;
}

