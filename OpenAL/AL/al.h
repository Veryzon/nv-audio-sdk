#ifndef _AL_H_
#define _AL_H_

/**
 * OpenAL cross platform audio library
 * Copyright (C) 1999-2000 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA  02111-1307, USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */
#include "altypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
 #ifdef _OPENAL32LIB
  #define ALAPI __declspec(dllexport)
 #else
  #ifndef AL_DYNAMIC_LINK
   #define ALAPI __declspec(dllimport)
  #else
   #define ALAPI
  #endif
#endif
 #define ALAPIENTRY __cdecl
 #define AL_CALLBACK
#else
 #ifdef TARGET_OS_MAC
  #if TARGET_OS_MAC
   #pragma export on
  #endif
 #endif
 #define ALAPI
 #define ALAPIENTRY __cdecl
 #define AL_CALLBACK
#endif

#define OPENAL

typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_ENABLE)(ALenum capability);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_DISABLE)(ALenum capability);
typedef ALAPI ALboolean (ALAPIENTRY *ALAPI_IS_ENABLED)(ALenum capability);

typedef ALAPI ALboolean (ALAPIENTRY *ALAPI_GET_BOOLEAN)(ALenum param);
typedef ALAPI ALint     (ALAPIENTRY *ALAPI_GET_INTEGER)(ALenum param);
typedef ALAPI ALfloat   (ALAPIENTRY *ALAPI_GET_FLOAT)(ALenum param);
typedef ALAPI ALdouble  (ALAPIENTRY *ALAPI_GET_DOUBLE)(ALenum param);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_BOOLEANV)(ALenum param, ALboolean* data);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_INTEGERV)(ALenum param, ALint* data);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_FLOATV)(ALenum param, ALfloat* data);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_DOUBLEV)(ALenum param, ALdouble* data);
typedef ALAPI ALubyte*  (ALAPIENTRY *ALAPI_GET_STRING)(ALenum param);

typedef ALAPI ALenum    (ALAPIENTRY *ALAPI_GET_ERROR)(ALvoid);

typedef ALAPI ALboolean (ALAPIENTRY *ALAPI_IS_EXTENSION_PRESENT)(ALubyte* ename);
typedef ALAPI ALvoid*   (ALAPIENTRY *ALAPI_GET_PROC_ADDRESS)(ALubyte* fname);
typedef ALAPI ALenum    (ALAPIENTRY *ALAPI_GET_ENUM_VALUE)(ALubyte* ename);

typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_LISTENERI)(ALenum param, ALint value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_LISTENERF)(ALenum param, ALfloat value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_LISTENER3F)(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_LISTENERFV)(ALenum param, ALfloat* values);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_LISTENERI)(ALenum param, ALint* value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_LISTENERF)(ALenum param, ALfloat* value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_LISTENER3F)(ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_LISTENERFV)(ALenum param, ALfloat* values);

typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GEN_SOURCES)(ALsizei n, ALuint* sourceNames);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_DELETE_SOURCES)(ALsizei n, ALuint* sourceNames);
typedef ALAPI ALboolean (ALAPIENTRY *ALAPI_IS_SOURCE)(ALuint id);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCEI)(ALuint sourceName, ALenum param, ALint value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCEF)(ALuint sourceName, ALenum param, ALfloat value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE3F)(ALuint sourceName, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCEFV)(ALuint sourceName, ALenum param, ALfloat* values);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_SOURCEI)(ALuint sourceName, ALenum param, ALint* value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_SOURCEF)(ALuint sourceName, ALenum param, ALfloat* value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_SOURCE3F)(ALuint sourceName, ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_SOURCEFV)(ALuint sourceName, ALenum param, ALfloat* values);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_PLAYV)(ALsizei n, ALuint* sources);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_PAUSEV)(ALsizei n, ALuint* sources);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_STOPV)(ALsizei n, ALuint* sources);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_REWINDV)(ALsizei n, ALuint* sources);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_PLAY)(ALuint sourceName);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_PAUSE)(ALuint sourceName);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_STOP)(ALuint sourceName);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_REWIND)(ALuint sourceName);

typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GEN_BUFFERS)(ALsizei n, ALuint* bufferNames);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_DELETE_BUFFERS)(ALsizei n, ALuint* bufferNames);
typedef ALAPI ALboolean (ALAPIENTRY *ALAPI_IS_BUFFER)(ALuint bufferName);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_BUFFER_DATA)(ALuint bufferName, ALenum format, ALvoid* data, ALsizei size, ALuint freq);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_BUFFERI)(ALuint bufferName, ALenum param, ALint* value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_GET_BUFFERF)(ALuint bufferName, ALenum param, ALfloat* value);

typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_QUEUE_BUFFERS)(ALuint sourceName, ALsizei n, ALuint* bufferNames);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_SOURCE_UNQUEUE_BUFFERS)(ALuint sourceName, ALsizei n, ALuint* bufferNames);

typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_DISTANCE_MODEL)(ALenum value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_DOPPLER_FACTOR)(ALfloat value);
typedef ALAPI ALvoid    (ALAPIENTRY *ALAPI_DOPPLER_VELOCITY)(ALfloat value);


typedef struct ALAPI_FXN_TABLE_STRUCT
{
    ALAPI_ENABLE                 alEnable;
    ALAPI_DISABLE                alDisable;
    ALAPI_IS_ENABLED             alIsEnabled;

    ALAPI_GET_BOOLEAN            alGetBoolean;
    ALAPI_GET_INTEGER            alGetInteger;
    ALAPI_GET_FLOAT              alGetFloat;
    ALAPI_GET_DOUBLE             alGetDouble;
    ALAPI_GET_BOOLEANV           alGetBooleanv;
    ALAPI_GET_INTEGERV           alGetIntegerv;
    ALAPI_GET_FLOATV             alGetFloatv;
    ALAPI_GET_DOUBLEV            alGetDoublev;
    ALAPI_GET_STRING             alGetString;

    ALAPI_GET_ERROR              alGetError;

    ALAPI_IS_EXTENSION_PRESENT   alIsExtensionPresent;
    ALAPI_GET_PROC_ADDRESS       alGetProcAddress;
    ALAPI_GET_ENUM_VALUE         alGetEnumValue;

    ALAPI_LISTENERI              alListeneri;
    ALAPI_LISTENERF              alListenerf;
    ALAPI_LISTENER3F             alListener3f;
    ALAPI_LISTENERFV             alListenerfv;
    ALAPI_GET_LISTENERI          alGetListeneri;
    ALAPI_GET_LISTENERF          alGetListenerf;
    ALAPI_GET_LISTENER3F         alGetListener3f;
    ALAPI_GET_LISTENERFV         alGetListenerfv;

    ALAPI_GEN_SOURCES            alGenSources;
    ALAPI_DELETE_SOURCES         alDeleteSources;
    ALAPI_IS_SOURCE              alIsSource;
    ALAPI_SOURCEI                alSourcei;
    ALAPI_SOURCEF                alSourcef;
    ALAPI_SOURCE3F               alSource3f;
    ALAPI_SOURCEFV               alSourcefv;
    ALAPI_GET_SOURCEI            alGetSourcei;
    ALAPI_GET_SOURCEF            alGetSourcef;
    ALAPI_GET_SOURCE3F           alGetSource3f;
    ALAPI_GET_SOURCEFV           alGetSourcefv;
    ALAPI_SOURCE_PLAYV           alSourcePlayv;
    ALAPI_SOURCE_PAUSEV          alSourcePausev;
    ALAPI_SOURCE_STOPV           alSourceStopv;
    ALAPI_SOURCE_REWINDV         alSourceRewindv;
    ALAPI_SOURCE_PLAY            alSourcePlay;
    ALAPI_SOURCE_PAUSE           alSourcePause;
    ALAPI_SOURCE_STOP            alSourceStop;
    ALAPI_SOURCE_REWIND          alSourceRewind;

    ALAPI_GEN_BUFFERS            alGenBuffers;
    ALAPI_DELETE_BUFFERS         alDeleteBuffers;
    ALAPI_IS_BUFFER              alIsBuffer;
    ALAPI_BUFFER_DATA            alBufferData;
    ALAPI_GET_BUFFERI            alGetBufferi;
    ALAPI_GET_BUFFERF            alGetBufferf;

    ALAPI_SOURCE_QUEUE_BUFFERS   alSourceQueueBuffers;
    ALAPI_SOURCE_UNQUEUE_BUFFERS alSourceUnqueueBuffers;

    ALAPI_DISTANCE_MODEL         alDistanceModel;
    ALAPI_DOPPLER_FACTOR         alDopplerFactor;
    ALAPI_DOPPLER_VELOCITY       alDopplerVelocity;

} ALAPI_FXN_TABLE;



#ifndef AL_NO_PROTOTYPES

    /**
     * OpenAL Maintenance Functions
     * Initialization and exiting.
     * State Management and Query.
     * Error Handling.
     * Extension Support.
     */

    /** State management. */
    ALAPI ALvoid    ALAPIENTRY alEnable(ALenum capability);
    ALAPI ALvoid    ALAPIENTRY alDisable(ALenum capability);
    ALAPI ALboolean ALAPIENTRY alIsEnabled(ALenum capability);

    /** State retrieval. */
    ALAPI ALboolean ALAPIENTRY alGetBoolean(ALenum param);
    ALAPI ALint     ALAPIENTRY alGetInteger(ALenum param);
    ALAPI ALfloat   ALAPIENTRY alGetFloat(ALenum param);
    ALAPI ALdouble  ALAPIENTRY alGetDouble(ALenum param);
    ALAPI ALvoid    ALAPIENTRY alGetBooleanv(ALenum param, ALboolean* data);
    ALAPI ALvoid    ALAPIENTRY alGetIntegerv(ALenum param, ALint* data);
    ALAPI ALvoid    ALAPIENTRY alGetFloatv(ALenum param, ALfloat* data);
    ALAPI ALvoid    ALAPIENTRY alGetDoublev(ALenum param, ALdouble* data);
    ALAPI ALubyte*  ALAPIENTRY alGetString(ALenum param);

    /**
     * Error support.
     * Obtain the most recent error generated in the AL state machine.
     */
    ALAPI ALenum    ALAPIENTRY alGetError(ALvoid);


    /**
     * Extension support.
     * Obtain the address of a function (usually an extension)
     *  with the name fname. All addresses are context-independent.
     */
    ALAPI ALboolean ALAPIENTRY alIsExtensionPresent(ALubyte* ename);


    /**
     * Extension support.
     * Obtain the address of a function (usually an extension)
     *  with the name fname. All addresses are context-independent.
     */
    ALAPI ALvoid*   ALAPIENTRY alGetProcAddress(ALubyte* fname);


    /**
     * Extension support.
     * Obtain the integer value of an enumeration (usually an extension) with the name ename.
     */
    ALAPI ALenum    ALAPIENTRY alGetEnumValue(ALubyte* ename);




    /**
     * LISTENER
     * Listener is the sample position for a given context.
     * The multi-channel (usually stereo) output stream generated
     *  by the mixer is parametrized by this Listener object:
     *  its position and velocity relative to Sources, within
     *  occluder and reflector geometry.
     */



    /**
     *
     * Listener Environment:  default 0.
     */
    ALAPI ALvoid    ALAPIENTRY alListeneri(ALenum param, ALint value);


    /**
     *
     * Listener Gain:  default 1.0f.
     */
    ALAPI ALvoid    ALAPIENTRY alListenerf(ALenum param, ALfloat value);


    /**
     *
     * Listener Position.
     * Listener Velocity.
     */
    ALAPI ALvoid    ALAPIENTRY alListener3f(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);


    /**
     *
     * Listener Position:        ALfloat[3]
     * Listener Velocity:        ALfloat[3]
     * Listener Orientation:     ALfloat[6]  (forward and up vector).
     */
    ALAPI ALvoid    ALAPIENTRY alListenerfv(ALenum param, ALfloat* values);

    ALAPI ALvoid    ALAPIENTRY alGetListeneri(ALenum param, ALint* value);
    ALAPI ALvoid    ALAPIENTRY alGetListenerf(ALenum param, ALfloat* value);
    ALAPI ALvoid    ALAPIENTRY alGetListener3f(ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3);
    ALAPI ALvoid    ALAPIENTRY alGetListenerfv(ALenum param, ALfloat* values);


    /**
     * SOURCE
     * Source objects are by default localized. Sources
     *  take the PCM data provided in the specified Buffer,
     *  apply Source-specific modifications, and then
     *  submit them to be mixed according to spatial
     *  arrangement etc.
     */



    /** Create Source objects. */
    ALAPI ALvoid    ALAPIENTRY alGenSources(ALsizei n, ALuint* sources);

    /** Delete Source objects. */
    ALAPI ALvoid    ALAPIENTRY alDeleteSources(ALsizei n, ALuint* sources);

    /** Verify a handle is a valid Source. */
    ALAPI ALboolean ALAPIENTRY alIsSource(ALuint sourceName);

    /** Set an integer parameter for a Source object. */
    ALAPI ALvoid    ALAPIENTRY alSourcei(ALuint sourceName, ALenum param, ALint value);
    ALAPI ALvoid    ALAPIENTRY alSourcef(ALuint sourceName, ALenum param, ALfloat value);
    ALAPI ALvoid    ALAPIENTRY alSource3f(ALuint sourceName, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
    ALAPI ALvoid    ALAPIENTRY alSourcefv(ALuint sourceName, ALenum param, ALfloat* values);

    /** Get an integer parameter for a Source object. */
    ALAPI ALvoid    ALAPIENTRY alGetSourcei(ALuint sourceName, ALenum param, ALint* value);
    ALAPI ALvoid    ALAPIENTRY alGetSourcef(ALuint sourceName, ALenum param, ALfloat* value);
    ALAPI ALvoid    ALAPIENTRY alGetSource3f(ALuint sourceName, ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3);
    ALAPI ALvoid    ALAPIENTRY alGetSourcefv(ALuint sourceName, ALenum param, ALfloat* values);

    /** Activate a source, start replay. */
    ALAPI ALvoid    ALAPIENTRY alSourcePlay(ALuint sourceName);
    ALAPI ALvoid    ALAPIENTRY alSourcePlayv(ALsizei n, ALuint* sourceNames);

    /**
     * Pause a source,
     *  temporarily remove it from the mixer list.
     */
    ALAPI ALvoid    ALAPIENTRY alSourcePause(ALuint sourceName);
    ALAPI ALvoid    ALAPIENTRY alSourcePausev(ALsizei n, ALuint* sourceNames);

    /**
     * Stop a source,
     *  temporarily remove it from the mixer list,
     *  and reset its internal state to pre-Play.
     * To remove a Source completely, it has to be
     *  deleted following Stop, or before Play.
     */
    ALAPI ALvoid    ALAPIENTRY alSourceStop(ALuint sourceName);
    ALAPI ALvoid    ALAPIENTRY alSourceStopv(ALsizei n, ALuint* sourceNames);

    /**
     * Rewinds a source,
     *  temporarily remove it from the mixer list,
     *  and reset its internal state to pre-Play.
     */
    ALAPI ALvoid    ALAPIENTRY alSourceRewind(ALuint sourceName);
    ALAPI ALvoid    ALAPIENTRY alSourceRewindv(ALsizei n, ALuint* sourceNames);

    /**
     * Queue stuff
     */
    ALAPI ALvoid    ALAPIENTRY alSourceQueueBuffers(ALuint sourceName, ALsizei n, ALuint* bufferNames);
    ALAPI ALvoid    ALAPIENTRY alSourceUnqueueBuffers(ALuint sourceName, ALsizei n, ALuint* bufferNames);



    /**
     * BUFFER
     * Buffer objects are storage space for sample data.
     * Buffers are referred to by Sources. There can be more than
     *  one Source using the same Buffer data. If Buffers have
     *  to be duplicated on a per-Source basis, the driver has to
     *  take care of allocation, copying, and deallocation as well
     *  as propagating buffer data changes.
     */

    /** Buffer object generation. */
    ALAPI ALvoid    ALAPIENTRY alGenBuffers(ALsizei n, ALuint* bufferNames);
    ALAPI ALvoid    ALAPIENTRY alDeleteBuffers(ALsizei n, ALuint* bufferNames);
    ALAPI ALboolean ALAPIENTRY alIsBuffer(ALuint bufferName);

    /**
     * Specify the data to be filled into a buffer.
     */
    ALAPI ALvoid    ALAPIENTRY alBufferData(ALuint bufferName, ALenum format, ALvoid* data, ALsizei size, ALuint freq);
    ALAPI ALvoid    ALAPIENTRY alGetBufferi(ALuint bufferName, ALenum param, ALint*   value);
    ALAPI ALvoid    ALAPIENTRY alGetBufferf(ALuint bufferName, ALenum param, ALfloat* value);



    /**
     * Knobs and dials
     */
    ALAPI ALvoid    ALAPIENTRY alDistanceModel(ALenum value);
    ALAPI ALvoid    ALAPIENTRY alDopplerFactor(ALfloat value);
    ALAPI ALvoid    ALAPIENTRY alDopplerVelocity(ALfloat value);

#else /* AL_NO_PROTOTYPES */

    #ifdef ALAPI_DEFINE_GLOBAL_VARS

        ALAPI_ENABLE                 alEnable               = 0;
        ALAPI_DISABLE                alDisable              = 0;
        ALAPI_IS_ENABLED             alIsEnabled            = 0;

        ALAPI_GET_BOOLEAN            alGetBoolean           = 0;
        ALAPI_GET_INTEGER            alGetInteger           = 0;
        ALAPI_GET_FLOAT              alGetFloat             = 0;
        ALAPI_GET_DOUBLE             alGetDouble            = 0;
        ALAPI_GET_BOOLEANV           alGetBooleanv          = 0;
        ALAPI_GET_INTEGERV           alGetIntegerv          = 0;
        ALAPI_GET_FLOATV             alGetFloatv            = 0;
        ALAPI_GET_DOUBLEV            alGetDoublev           = 0;
        ALAPI_GET_STRING             alGetString            = 0;

        ALAPI_GET_ERROR              alGetError             = 0;

        ALAPI_IS_EXTENSION_PRESENT   alIsExtensionPresent   = 0;
        ALAPI_GET_PROC_ADDRESS       alGetProcAddress       = 0;
        ALAPI_GET_ENUM_VALUE         alGetEnumValue         = 0;

        ALAPI_LISTENERI              alListeneri            = 0;
        ALAPI_LISTENERF              alListenerf            = 0;
        ALAPI_LISTENER3F             alListener3f           = 0;
        ALAPI_LISTENERFV             alListenerfv           = 0;
        ALAPI_GET_LISTENERI          alGetListeneri         = 0;
        ALAPI_GET_LISTENERF          alGetListenerf         = 0;
        ALAPI_GET_LISTENER3F         alGetListener3f        = 0;
        ALAPI_GET_LISTENERFV         alGetListenerfv        = 0;

        ALAPI_GEN_SOURCES            alGenSources           = 0;
        ALAPI_DELETE_SOURCES         alDeleteSources        = 0;
        ALAPI_IS_SOURCE              alIsSource             = 0;
        ALAPI_SOURCEI                alSourcei              = 0;
        ALAPI_SOURCEF                alSourcef              = 0;
        ALAPI_SOURCE3F               alSource3f             = 0;
        ALAPI_SOURCEFV               alSourcefv             = 0;
        ALAPI_GET_SOURCEI            alGetSourcei           = 0;
        ALAPI_GET_SOURCEF            alGetSourcef           = 0;
        ALAPI_GET_SOURCE3F           alGetSource3f          = 0;
        ALAPI_GET_SOURCEFV           alGetSourcefv          = 0;
        ALAPI_SOURCE_PLAYV           alSourcePlayv          = 0;
        ALAPI_SOURCE_PAUSEV          alSourcePausev         = 0;
        ALAPI_SOURCE_STOPV           alSourceStopv          = 0;
        ALAPI_SOURCE_REWINDV         alSourceRewindv        = 0;
        ALAPI_SOURCE_PLAY            alSourcePlay           = 0;
        ALAPI_SOURCE_PAUSE           alSourcePause          = 0;
        ALAPI_SOURCE_STOP            alSourceStop           = 0;
        ALAPI_SOURCE_REWIND          alSourceRewind         = 0;

        ALAPI_GEN_BUFFERS            alGenBuffers           = 0;
        ALAPI_DELETE_BUFFERS         alDeleteBuffers        = 0;
        ALAPI_IS_BUFFER              alIsBuffer             = 0;
        ALAPI_BUFFER_DATA            alBufferData           = 0;
        ALAPI_GET_BUFFERI            alGetBufferi           = 0;
        ALAPI_GET_BUFFERF            alGetBufferf           = 0;

        ALAPI_SOURCE_QUEUE_BUFFERS   alSourceQueueBuffers   = 0;
        ALAPI_SOURCE_UNQUEUE_BUFFERS alSourceUnqueueBuffers = 0;

        ALAPI_DISTANCE_MODEL         alDistanceModel        = 0;
        ALAPI_DOPPLER_FACTOR         alDopplerFactor        = 0;
        ALAPI_DOPPLER_VELOCITY       alDopplerVelocity      = 0;

    #else

        extern ALAPI_ENABLE                 alEnable;
        extern ALAPI_DISABLE                alDisable;
        extern ALAPI_IS_ENABLED             alIsEnabled;

        extern ALAPI_GET_BOOLEAN            alGetBoolean;
        extern ALAPI_GET_INTEGER            alGetInteger;
        extern ALAPI_GET_FLOAT              alGetFloat;
        extern ALAPI_GET_DOUBLE             alGetDouble;
        extern ALAPI_GET_BOOLEANV           alGetBooleanv;
        extern ALAPI_GET_INTEGERV           alGetIntegerv;
        extern ALAPI_GET_FLOATV             alGetFloatv;
        extern ALAPI_GET_DOUBLEV            alGetDoublev;
        extern ALAPI_GET_STRING             alGetString;

        extern ALAPI_GET_ERROR              alGetError;

        extern ALAPI_IS_EXTENSION_PRESENT   alIsExtensionPresent;
        extern ALAPI_GET_PROC_ADDRESS       alGetProcAddress;
        extern ALAPI_GET_ENUM_VALUE         alGetEnumValue;

        extern ALAPI_LISTENERI              alListeneri;
        extern ALAPI_LISTENERF              alListenerf;
        extern ALAPI_LISTENER3F             alListener3f;
        extern ALAPI_LISTENERFV             alListenerfv;
        extern ALAPI_GET_LISTENERI          alGetListeneri;
        extern ALAPI_GET_LISTENERF          alGetListenerf;
        extern ALAPI_GET_LISTENER3F         alGetListener3f;
        extern ALAPI_GET_LISTENERFV         alGetListenerfv;

        extern ALAPI_GEN_SOURCES            alGenSources;
        extern ALAPI_DELETE_SOURCES         alDeleteSources;
        extern ALAPI_IS_SOURCE              alIsSource;
        extern ALAPI_SOURCEI                alSourcei;
        extern ALAPI_SOURCEF                alSourcef;
        extern ALAPI_SOURCE3F               alSource3f;
        extern ALAPI_SOURCEFV               alSourcefv;
        extern ALAPI_GET_SOURCEI            alGetSourcei;
        extern ALAPI_GET_SOURCEF            alGetSourcef;
        extern ALAPI_GET_SOURCE3F           alGetSource3f;
        extern ALAPI_GET_SOURCEFV           alGetSourcefv;
        extern ALAPI_SOURCE_PLAYV           alSourcePlayv;
        extern ALAPI_SOURCE_PAUSEV          alSourcePausev;
        extern ALAPI_SOURCE_STOPV           alSourceStopv;
        extern ALAPI_SOURCE_PLAY            alSourcePlay;
        extern ALAPI_SOURCE_PAUSE           alSourcePause;
        extern ALAPI_SOURCE_STOP            alSourceStop;

        extern ALAPI_GEN_BUFFERS            alGenBuffers;
        extern ALAPI_DELETE_BUFFERS         alDeleteBuffers;
        extern ALAPI_IS_BUFFER              alIsBuffer;
        extern ALAPI_BUFFER_DATA            alBufferData;
        extern ALAPI_GET_BUFFERI            alGetBufferi;
        extern ALAPI_GET_BUFFERF            alGetBufferf;

        extern ALAPI_SOURCE_QUEUE_BUFFERS   alSourceQueueBuffers;
        extern ALAPI_SOURCE_UNQUEUE_BUFFERS alSourceUnqueueBuffers;

        extern ALAPI_DISTANCE_MODEL         alDistanceModel;
        extern ALAPI_DOPPLER_FACTOR         alDopplerFactor;
        extern ALAPI_DOPPLER_VELOCITY       alDopplerVelocity;

    #endif
#endif /* AL_NO_PROTOTYPES */

#ifdef TARGET_OS_MAC
 #if TARGET_OS_MAC
  #pragma export off
 #endif
#endif

#ifdef __cplusplus
}
#endif

#endif
