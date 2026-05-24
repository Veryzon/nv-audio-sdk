#ifndef _ALUT_H_
#define _ALUT_H_

#include "al.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
 #ifdef _OPENAL32LIB
  #define ALUTAPI __declspec(dllexport)
 #else
  #ifndef AL_DYNAMIC_LINK
    #define ALUTAPI __declspec(dllimport)
  #else
   #define ALUTAPI
  #endif
 #endif

 typedef struct ALdevice_struct ALdevice;
 typedef struct ALcontext_struct ALcontext;

 #define ALUTAPIENTRY __cdecl
#else
 #ifdef TARGET_OS_MAC
  #if TARGET_OS_MAC
   #pragma export on
  #endif
 #endif
 #define ALUTAPI
 #define ALUTAPIENTRY __cdecl
#endif


typedef ALUTAPI ALvoid    (ALUTAPIENTRY *ALUTAPI_INIT)(ALint* argc, ALbyte** argv);
typedef ALUTAPI ALvoid    (ALUTAPIENTRY *ALUTAPI_EXIT)(ALvoid);
typedef ALUTAPI ALvoid    (ALUTAPIENTRY *ALUTAPI_LOAD_WAV_FILE)(ALbyte* file, ALenum* format, ALvoid** data, ALsizei* size, ALsizei*freq, ALboolean* loop);
typedef ALUTAPI ALvoid    (ALUTAPIENTRY *ALUTAPI_LOAD_WAV_MEMORY)(ALbyte* memory, ALenum* format, ALvoid** data, ALsizei* size, ALsizei* freq, ALboolean* loop);
typedef ALUTAPI ALvoid    (ALUTAPIENTRY *ALUTAPI_UNLOAD_WAV)(ALenum format, ALvoid* data, ALsizei size, ALsizei freq);

typedef struct ALUTAPI_FXN_TABLE_STRUCT
{
    ALUTAPI_INIT                alutInit;
    ALUTAPI_EXIT                alutExit;
    ALUTAPI_LOAD_WAV_FILE       alutLoadWAVFile;
    ALUTAPI_LOAD_WAV_MEMORY     alutLoadWAVMemory;
    ALUTAPI_UNLOAD_WAV          alutUnloadWAV;

} ALUTAPI_FXN_TABLE;


#ifndef ALUT_NO_PROTOTYPES

ALUTAPI ALvoid  ALUTAPIENTRY alutInit(ALint* argc, ALbyte** argv);
ALUTAPI ALvoid  ALUTAPIENTRY alutExit(ALvoid);
ALUTAPI ALvoid  ALUTAPIENTRY alutLoadWAVFile(ALbyte* file, ALenum* format, ALvoid** data, ALsizei* size, ALsizei*freq, ALboolean* loop);
ALUTAPI ALvoid  ALUTAPIENTRY alutLoadWAVMemory(ALbyte* memory, ALenum* format, ALvoid** data, ALsizei* size, ALsizei* freq, ALboolean* loop);
ALUTAPI ALvoid  ALUTAPIENTRY alutUnloadWAV(ALenum format, ALvoid* data, ALsizei size, ALsizei freq);

#else /* ALUT_NO_PROTOTYPES */

    #ifdef ALUTAPI_DEFINE_GLOBAL_VARS

        ALUTAPI_INIT                alutInit = 0;
        ALUTAPI_EXIT                alutExit = 0;
        ALUTAPI_LOAD_WAV_FILE       alutLoadWAVFile = 0;
        ALUTAPI_LOAD_WAV_MEMORY     alutLoadWAVMemory = 0;
        ALUTAPI_UNLOAD_WAV          alutUnloadWAV = 0;

    #else

        extern ALUTAPI_INIT             alutInit;
        extern ALUTAPI_EXIT             alutExit;
        extern ALUTAPI_LOAD_WAV_FILE    alutLoadWAVFile;
        extern ALUTAPI_LOAD_WAV_MEMORY  alutLoadWAVMemory = 0;
        extern ALUTAPI_UNLOAD_WAV       alutUnloadWAV = 0;

    #endif

#endif /* ALUT_NO_PROTOTYPES */

#ifdef TARGET_OS_MAC
 #if TARGET_OS_MAC
  #pragma export off
 #endif
#endif

#ifdef __cplusplus
}
#endif

#endif
