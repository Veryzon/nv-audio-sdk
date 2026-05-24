#ifndef _ALC_H_
#define _ALC_H_

#include "altypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
 #ifdef _OPENAL32LIB
  #define ALCAPI __declspec(dllexport)
 #else
  #ifndef ALC_DYNAMIC_LINK
   #define ALCAPI __declspec(dllimport)
  #else
   #define ALCAPI
  #endif
 #endif

typedef struct ALdevice_struct ALdevice;
typedef struct ALcontext_struct ALcontext;

 #define ALCAPIENTRY __cdecl
 #define ALC_CALLBACK __cdecl
#else
 #ifdef TARGET_OS_MAC
  #if TARGET_OS_MAC
   #pragma export on
  #endif
 #endif
 #define ALCAPI
 #define ALCAPIENTRY __cdecl
 #define ALC_CALLBACK __cdecl
#endif


// Callback Prototypes
typedef ALboolean (ALC_CALLBACK *ALenumerateDeviceCallback)(const ALubyte* deviceName, ALvoid* context);


// ALCAPI
typedef ALCAPI ALubyte*   (ALCAPIENTRY *ALCAPI_GET_STRING)(ALdevice* device, ALenum param);
typedef ALCAPI ALvoid     (ALCAPIENTRY *ALCAPI_GET_INTEGERV)(ALdevice* device, ALenum param, ALsizei size, ALint* data);

typedef ALCAPI ALdevice*  (ALCAPIENTRY *ALCAPI_OPEN_DEVICE)(ALubyte* deviceName);
typedef ALCAPI ALvoid     (ALCAPIENTRY *ALCAPI_CLOSE_DEVICE)(ALdevice* device);
typedef ALCAPI ALboolean  (ALCAPIENTRY *ALCAPI_ENUMERATE_DEVICES)(ALenumerateDeviceCallback callback, ALvoid* context);

typedef ALCAPI ALcontext* (ALCAPIENTRY *ALCAPI_CREATE_CONTEXT)(ALdevice* device, ALint* attrList);
typedef ALCAPI ALboolean  (ALCAPIENTRY *ALCAPI_MAKE_CONTEXT_CURRENT)(ALcontext* context);
typedef ALCAPI ALvoid     (ALCAPIENTRY *ALCAPI_PROCESS_CONTEXT)(ALcontext* context);
typedef ALCAPI ALcontext* (ALCAPIENTRY *ALCAPI_GET_CURRENT_CONTEXT)(ALvoid);
typedef ALCAPI ALdevice*  (ALCAPIENTRY *ALCAPI_GET_CONTEXTS_DEVICE)(ALcontext* context);
typedef ALCAPI ALvoid     (ALCAPIENTRY *ALCAPI_SUSPEND_CONTEXT)(ALcontext* context);
typedef ALCAPI ALvoid     (ALCAPIENTRY *ALCAPI_DESTROY_CONTEXT)(ALcontext* context);

typedef ALCAPI ALenum     (ALCAPIENTRY *ALCAPI_GET_ERROR)(ALdevice* device);

typedef ALCAPI ALboolean  (ALCAPIENTRY *ALCAPI_IS_EXTENSION_PRESENT)(ALdevice* device, ALubyte* eName);
typedef ALCAPI ALvoid*    (ALCAPIENTRY *ALCAPI_GET_PROC_ADDRESS)(ALdevice* device, ALubyte* fName);
typedef ALCAPI ALenum     (ALCAPIENTRY *ALCAPI_GET_ENUM_VALUE)(ALdevice* device, ALubyte* eName);


typedef struct ALCAPI_FXN_TABLE_STRUCT
{
    ALCAPI_GET_STRING           alcGetString;
    ALCAPI_GET_INTEGERV         alcGetIntegerv;

    ALCAPI_OPEN_DEVICE          alcOpenDevice;
    ALCAPI_CLOSE_DEVICE         alcCloseDevice;
    ALCAPI_ENUMERATE_DEVICES    alcEnumerateDevices;

    ALCAPI_CREATE_CONTEXT       alcCreateContext;
    ALCAPI_MAKE_CONTEXT_CURRENT alcMakeContextCurrent;
    ALCAPI_PROCESS_CONTEXT      alcProcessContext;
    ALCAPI_GET_CURRENT_CONTEXT  alcGetCurrentContext;
    ALCAPI_GET_CONTEXTS_DEVICE  alcGetContextsDevice;
    ALCAPI_SUSPEND_CONTEXT      alcSuspendContext;
    ALCAPI_DESTROY_CONTEXT      alcDestroyContext;

    ALCAPI_GET_ERROR            alcGetError;

    ALCAPI_IS_EXTENSION_PRESENT alcIsExtensionPresent;
    ALCAPI_GET_PROC_ADDRESS     alcGetProcAddress;
    ALCAPI_GET_ENUM_VALUE       alcGetEnumValue;

} ALCAPI_FXN_TABLE;



#ifndef ALC_NO_PROTOTYPES

ALCAPI ALubyte*    ALCAPIENTRY alcGetString(ALdevice* device, ALenum param);
ALCAPI ALvoid      ALCAPIENTRY alcGetIntegerv(ALdevice* device, ALenum param, ALsizei size, ALint* data);

ALCAPI ALdevice*   ALCAPIENTRY alcOpenDevice(ALubyte* deviceName);
ALCAPI ALvoid      ALCAPIENTRY alcCloseDevice(ALdevice* device);
ALCAPI ALboolean   ALCAPIENTRY alcEnumerateDevices(ALenumerateDeviceCallback callback, ALvoid* context);

ALCAPI ALcontext*  ALCAPIENTRY alcCreateContext(ALdevice* device, ALint* attrList);
ALCAPI ALboolean   ALCAPIENTRY alcMakeContextCurrent(ALcontext* context);
ALCAPI ALvoid      ALCAPIENTRY alcProcessContext(ALcontext* context);
ALCAPI ALcontext*  ALCAPIENTRY alcGetCurrentContext(ALvoid);
ALCAPI ALdevice*   ALCAPIENTRY alcGetContextsDevice(ALcontext* context);
ALCAPI ALvoid      ALCAPIENTRY alcSuspendContext(ALcontext* context);
ALCAPI ALvoid      ALCAPIENTRY alcDestroyContext(ALcontext* context);

ALCAPI ALenum      ALCAPIENTRY alcGetError(ALdevice* device);

ALCAPI ALboolean   ALCAPIENTRY alcIsExtensionPresent(ALdevice* device, ALubyte* ename);
ALCAPI ALvoid*     ALCAPIENTRY alcGetProcAddress(ALdevice* device, ALubyte* fname);
ALCAPI ALenum      ALCAPIENTRY alcGetEnumValue(ALdevice* device, ALubyte* ename);

#else /* ALC_NO_PROTOTYPES */

    #ifdef ALCAPI_DEFINE_GLOBAL_VARS

        ALCAPI_GET_STRING                  alcGetString          = 0;
        ALCAPI_GET_INTEGERV                alcGetIntegerv        = 0;

        ALCAPI_OPEN_DEVICE                 alcOpenDevice         = 0;
        ALCAPI_CLOSE_DEVICE                alcCloseDevice        = 0;
        ALCAPI_ENUMERATE_DEVICES           alcEnumerateDevices   = 0;

        ALCAPI_CREATE_CONTEXT              alcCreateContext      = 0;
        ALCAPI_MAKE_CONTEXT_CURRENT        alcMakeContextCurrent = 0;
        ALCAPI_PROCESS_CONTEXT             alcProcessContext     = 0;
        ALCAPI_GET_CURRENT_CONTEXT         alcGetCurrentContext  = 0;
        ALCAPI_GET_CONTEXTS_DEVICE         alcGetContextsDevice  = 0;
        ALCAPI_SUSPEND_CONTEXT             alcSuspendContext     = 0;
        ALCAPI_DESTROY_CONTEXT             alcDestroyContext     = 0;

        ALCAPI_GET_ERROR                   alcGetError           = 0;

        ALCAPI_IS_EXTENSION_PRESENT        alcIsExtensionPresent = 0;
        ALCAPI_GET_PROC_ADDRESS            alcGetProcAddress     = 0;
        ALCAPI_GET_ENUM_VALUE              alcGetEnumValue       = 0;

    #else

        extern ALCAPI_GET_STRING           alcGetString;
        extern ALCAPI_GET_INTEGERV         alcGetIntegerv;

        extern ALCAPI_OPEN_DEVICE          alcOpenDevice;
        extern ALCAPI_CLOSE_DEVICE         alcCloseDevice;
        extern ALCAPI_ENUMERATE_DEVICES    alcEnumerateDevices;

        extern ALCAPI_CREATE_CONTEXT       alcCreateContext;
        extern ALCAPI_MAKE_CONTEXT_CURRENT alcMakeContextCurrent;
        extern ALCAPI_PROCESS_CONTEXT      alcProcessContext;
        extern ALCAPI_GET_CURRENT_CONTEXT  alcGetCurrentContext;
        extern ALCAPI_GET_CONTEXTS_DEVICE  alcGetContextsDevice;
        extern ALCAPI_SUSPEND_CONTEXT      alcSuspendContext;
        extern ALCAPI_DESTROY_CONTEXT      alcDestroyContext;

        extern ALCAPI_GET_ERROR            alcGetError;

        extern ALCAPI_IS_EXTENSION_PRESENT alcIsExtensionPresent;
        extern ALCAPI_GET_PROC_ADDRESS     alcGetProcAddress;
        extern ALCAPI_GET_ENUM_VALUE       alcGetEnumValue;

    #endif

#endif /* ALC_NO_PROTOTYPES */


#ifdef TARGET_OS_MAC
 #if TARGET_OS_MAC
  #pragma export off
 #endif
#endif

#ifdef __cplusplus
}
#endif

#endif
