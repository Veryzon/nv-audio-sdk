#ifndef _OPENAL32_H_
#define _OPENAL32_H_


#include "al\al.h"
#include "al\alc.h"
#include "alu.h"
#include "alBuffer.h"
#include "alError.h"
#include "alListener.h"
#include "alSource.h"
#include "alState.h"
#include "windows.h"
#include "dsound.h"
#include "alList.h"


//*****************************************************************************
// Defines from alctypes.h.
//*****************************************************************************

//
// These values should be determined by querying alcGetEnumValue.
//

/* Bad value. */
#define ALC_INVALID                              (-1)

/** Errors: No Error. */
#define ALC_NO_ERROR                             0

#define ALC_MAJOR_VERSION                        0x1000
#define ALC_MINOR_VERSION                        0x1001
#define ALC_ATTRIBUTES_SIZE                      0x1002
#define ALC_ALL_ATTRIBUTES                       0x1003

#define ALC_DEFAULT_DEVICE_SPECIFIER             0x1004
#define ALC_DEVICE_SPECIFIER                     0x1005
#define ALC_EXTENSIONS                           0x1006

#define ALC_FREQUENCY                            0x1007
#define ALC_REFRESH                              0x1008
#define ALC_SYNC                                 0x1009

/**
 * The device argument does not name a valid device.
 */
#define ALC_INVALID_DEVICE                       0xA001

/**
 * The context argument does not name a valid context.
 */
#define ALC_INVALID_CONTEXT                      0xA002

/**
 * A function was called at inappropriate time,
 *  or in an inappropriate way, causing an illegal state.
 * This can be an incompatible ALenum, object ID,
 *  and/or function.
 */
#define ALC_INVALID_ENUM                         0xA003

/**
 * Illegal value passed as an argument to an AL call.
 * Applies to parameter values, but not to enumerations.
 */
#define ALC_INVALID_VALUE                        0xA004

/**
 * A function could not be completed,
 * because there is not enough memory available.
 */
#define ALC_OUT_OF_MEMORY                        0xA005


//*****************************************************************************
// Defines from altypes.h.
//*****************************************************************************

/* Bad value. */
#define AL_INVALID                               (-1)

/* Disable value. */
#define AL_NONE                                  0

/**
  * Indicate the type of AL_SOURCE.
  * Sources can be spatialized
  */
#define AL_SOURCE_TYPE                           0x200

/** Indicate source has absolute coordinates. */
#define AL_SOURCE_ABSOLUTE                       0x201

/** Indicate Source has listener relative coordinates. */
#define AL_SOURCE_RELATIVE                       0x202

/**
 * Directional source, inner cone angle, in degrees.
 * Range:    [0-360]
 * Default:  360
 */
#define AL_CONE_INNER_ANGLE                      0x1001

/**
 * Directional source, outer cone angle, in degrees.
 * Range:    [0-360]
 * Default:  360
 */
#define AL_CONE_OUTER_ANGLE                      0x1002

/**
 * Specify the pitch to be applied, either at source,
 *  or on mixer results, at listener.
 * Range:        [0.5-2.0]
 * Default:  1.0
 */
#define AL_PITCH                                 0x1003

/**
 * Specify the current location in three dimensional space.
 * OpenAL, like OpenGL, uses a right handed coordinate system,
 *  where in a frontal default view X (thumb) points right,
 *  Y points up (index finger), and Z points towards the
 *  viewer/camera (middle finger).
 * To switch from a left handed coordinate system, flip the
 *  sign on the Z coordinate.
 * Listener position is always in the world coordinate system.
 */
#define AL_POSITION                              0x1004

/** Specify the current direction as forward vector. */
#define AL_DIRECTION                             0x1005

/** Specify the current velocity in three dimensional space. */
#define AL_VELOCITY                              0x1006

/**
 * Indicate whether source has to loop infinite.
 * Type: ALboolean
 * Range:    [AL_TRUE, AL_FALSE]
 * Default:  AL_FALSE
 */
#define AL_LOOPING                               0x1007

/**
 * Indicate the buffer to provide sound samples.
 * Type: ALuint.
 * Range: any valid Buffer id.
 */
#define AL_BUFFER                                0x1009

/**
 * Indicate the gain (volume amplification) applied.
 * Type:     ALfloat.
 * Range:    ]0.0-  ]
 * A value of 1.0 means un-attenuated/unchanged.
 * Each division by 2 equals an attenuation of -6dB.
 * Each multiplicaton with 2 equals an amplification of +6dB.
 * A value of 0.0 is meaningless with respect to a logarithmic
 *  scale; it is interpreted as zero volume - the channel
 *  is effectively disabled.
 */
#define AL_GAIN                                  0x100A

/**
 * Indicate minimum source attenuation.
 * Type:     ALfloat
 * Range:        [0.0 - 1.0]
 */
#define AL_MIN_GAIN                              0x100D

/**
 * Indicate maximum source attenuation.
 * Type:         ALfloat
 * Range:        [0.0 - 1.0]
 */
#define AL_MAX_GAIN                              0x100E

/**
 * Specify the current orientation.
 * Type:         ALfv6 (at/up)
 * Range:        N/A
 */
#define AL_ORIENTATION                           0x100F

/* byte offset into source (in canon format).  -1 if source
 * is not playing.  Don't set this, get this.
 *
 * Type:     ALfloat
 * Range:    [0.0 - ]
 * Default:  1.0
 */
#define AL_REFERENCE_DISTANCE                    0x1020

 /**
 * Indicate the rolloff factor for the source.
 * Type: ALfloat
 * Range:    [0.0 - ]
 * Default:  1.0
 */
#define AL_ROLLOFF_FACTOR                        0x1021

/**
 * Indicate the gain (volume amplification) applied.
 * Type:     ALfloat.
 * Range:    ]0.0-  ]
 * A value of 1.0 means un-attenuated/unchanged.
 * Each division by 2 equals an attenuation of -6dB.
 * Each multiplicaton with 2 equals an amplification of +6dB.
 * A value of 0.0 is meaningless with respect to a logarithmic
 *  scale; it is interpreted as zero volume - the channel
 *  is effectively disabled.
 */
#define AL_CONE_OUTER_GAIN                       0x1022

/**
 * Specify the maximum distance.
 * Type:         ALfloat
 * Range:        [0.0 - ]
 */
#define AL_MAX_DISTANCE                          0x1023

/**
 * Specify the channel mask. (Creative)
 * Type:         ALuint
 * Range:        [0 - 255]
 */
#define AL_CHANNEL_MASK                          0x3000

/**
 * Source state information
 */
#define AL_SOURCE_STATE                          0x1010
#define AL_INITIAL                               0x1011
#define AL_PLAYING                               0x1012
#define AL_PAUSED                                0x1013
#define AL_STOPPED                               0x1014

/**
 * Buffer Queue params
 */
#define AL_BUFFERS_QUEUED                        0x1015
#define AL_BUFFERS_PROCESSED                     0x1016

/** Sound buffers: format specifier. */
#define AL_FORMAT_MONO8                          0x1100
#define AL_FORMAT_MONO16                         0x1101
#define AL_FORMAT_STEREO8                        0x1102
#define AL_FORMAT_STEREO16                       0x1103

/**
 * Sound buffers: frequency, in units of Hertz [Hz].
 * This is the number of samples per second. Half of the
 *  sample frequency marks the maximum significant
 *  frequency component.
 */
#define AL_FREQUENCY                             0x2001
#define AL_BITS                                  0x2002
#define AL_CHANNELS                              0x2003
#define AL_SIZE                                  0x2004
#define AL_DATA                                  0x2005

/**
 * Buffer state.
 *
 * Not supported for public use (yet).
 */
#define AL_UNUSED                                0x2010
#define AL_PENDING                               0x2011
#define AL_PROCESSED                             0x2012

/** Errors: No Error. */
#define AL_NO_ERROR                              0

/**
 * Illegal name passed as an argument to an AL call.
 */
#define AL_INVALID_NAME                          0xA001

/**
 * Illegal enum passed as an argument to an AL call.
 */
#define AL_INVALID_ENUM                          0xA002
/**
 * Illegal value passed as an argument to an AL call.
 * Applies to parameter values, but not to enumerations.
 */
#define AL_INVALID_VALUE                         0xA003

/**
 * A function was called at inappropriate time,
 *  or in an inappropriate way, causing an illegal state.
 * This can be an incompatible ALenum, object ID,
 *  and/or function.
 */
#define AL_INVALID_OPERATION                     0xA004

/**
 * A function could not be completed,
 * because there is not enough memory available.
 */
#define AL_OUT_OF_MEMORY                         0xA005



/** Context strings: Vendor Name. */
#define AL_VENDOR                                0xB001
#define AL_VERSION                               0xB002
#define AL_RENDERER                              0xB003
#define AL_EXTENSIONS                            0xB004

/** Global tweakage. */

/**
 * Doppler scale.  Default 1.0
 */
#define AL_DOPPLER_FACTOR                        0xC000

/**
 * Doppler velocity.  Default 1.0
 */
#define AL_DOPPLER_VELOCITY                      0xC001

/**
 * Distance model.  Default AL_INVERSE_DISTANCE_CLAMPED
 */
#define AL_DISTANCE_MODEL                        0xD000

/** Distance models. */

#define AL_INVERSE_DISTANCE                      0xD001
#define AL_INVERSE_DISTANCE_CLAMPED              0xD002



//*****************************************************************************
// Additional Defines
//*****************************************************************************

#define AL_MAX_CHANNELS         4
#define AL_MAX_SOURCES          32
#define NUMWAVEBUFFERS  4

typedef struct ALdevice_struct
{
    //
    // These variables must always be initialized.
    //
    ALenum                      LastError;
    ALint                       InUse;

    // Support for 3rd party OpenAL implementations.
    HINSTANCE                   Dll;
    ALAPI_FXN_TABLE             AlApi;
    ALCAPI_FXN_TABLE            AlcApi;
    struct ALdevice_struct*     DllDevice;

    //
    // Implementation specific variables.
    //
    ALuint          Frequency;
    ALuint          Channels;
    ALenum          Format;

    ALint           MajorVersion;
    ALint           MinorVersion;

    ALubyte         szDeviceName[256];

    // Maximum number of sources that can be created
    ALuint          MaxNoOfSources;

    // Set to DSBCAPS_LOCHARDWARE or DSBCAPS_LOCSOFTWARE based on the number of voices supported
    // by the hardware (more than 15 required for Hardware support).
    ALuint          CreationFlag;

    //DirectSound
    LPDIRECTSOUND DShandle;
    LPDIRECTSOUNDBUFFER DSpbuffer;
    LPDIRECTSOUNDBUFFER DSsbuffer;
    LPDIRECTSOUND3DLISTENER DS3dlistener;

    //waveOut
    HWAVEOUT        handle;
    WAVEHDR         buffer[NUMWAVEBUFFERS];
    ALint           BuffersCommitted;
    HANDLE          hWaveHdrEvent;
    HANDLE          hWaveThreadEvent;
    HANDLE          hThread;
    ALuint          ThreadID;
    ALboolean       bWaveShutdown;

    //mmTimer
    MMRESULT        timer;

} ALdevice;

typedef struct ALcontext_struct
{
    //
    // These variables are alway initialized.
    //
    ALlistEntry                 ListEntry;
    ALboolean                   Suspended;
    ALenum                      LastError;
    ALdevice*                   Device;
    struct ALcontext_struct*    DllContext;
    CRITICAL_SECTION            Lock;

    //
    // Implementation specific variables.
    //
    ALlistener      Listener;

    ALsource*       Source;
    ALuint          SourceCount;

    ALuint          Frequency;
    ALuint          Channels;
    ALenum          Format;

    ALenum          DistanceModel;

    ALfloat         DopplerFactor;
    ALfloat         DopplerVelocity;

    //
    // The buffers for this context.
    //
    ALbuffer* BufferList;
    ALuint BufferCount;

} ALcontext;


ALvoid alcUpdateContext(ALcontext* context, ALuint type, ALuint name);
ALint LinearGainToDB(float flGain);

extern ALlist* alcContextList;
extern ALcontext* alcCurrentContext;



#endif

