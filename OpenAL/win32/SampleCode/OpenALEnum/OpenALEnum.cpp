//*****************************************************************************
//*****************************************************************************
//
// OpenALEnum.cpp
//
//
// Description:
// This file demonstrates the use of the alcEnumerateDevice function.
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




#include "stdafx.h"
#include "math.h"
#include "../common/OpenALHelp.h"
#include "../common/wavefile.h"


//*****************************************************************************
//*****************************************************************************
//
// Defines
//
//*****************************************************************************
//*****************************************************************************

#define NVIDIA_DEVICE_NAME      "NVIDIA® nForce(TM) Audio"



//*****************************************************************************
// EnumerateDevicesCallback
//*****************************************************************************
//
extern "C"
ALboolean ALC_CALLBACK EnumerateDevicesCallback(const ALubyte* deviceName, ALvoid* context)
{
    printf("%s\n", deviceName);
    return AL_FALSE;
}


//*****************************************************************************
// main
//*****************************************************************************
//
int main(int argc, char* argv[])
{
    int error = 0;
    ALdevice*  pnvDevice  = 0;
    ALcontext* pnvContext = 0;
    ALuint bufferNames[1];
    ALuint sourceNames[1];
    ALint done;

    //
    // Load the wave file.
    //
    CWaveFile footsteps;
    footsteps.LoadFile("WaveFiles\\human_footsteps.wav");

    //
    // Load the library.
    //
    TCHAR path[MAX_PATH];
    GetSystemDirectory(path, MAX_PATH);
    lstrcat(path, TEXT("\\OpenAL32.dll"));
    HINSTANCE dll = LoadLibrary(path);
    if(!dll)
    {
        printf("Failed to load the OpenAL!\n");
        goto on_error;
    }

    printf("Loaded OpenAL.\n");

    if(!InitOpenAL(dll))
    {
        printf("Failed to get OpenAL functions!\n");
        goto on_error;

    }

    //
    // See what devices are out there.
    //
    alcEnumerateDevices(EnumerateDevicesCallback, 0);

    //
    // Try to enumerate the NVIDIA device.
    //
    pnvDevice = alcOpenDevice((ALubyte*)NVIDIA_DEVICE_NAME);
    if(!pnvDevice)
    {
        printf("Failed to create \"NVIDIA nForce(TM)\"!\n");
        goto on_error;
    }

    //
    // Try to create a device context.
    //
    pnvContext = alcCreateContext(pnvDevice, 0);
    if(!pnvContext)
    {
        printf("Failed to create the NVIDIA device context!\n");
        goto on_error;
    }

    //
    // Try out some other functions.
    //
    if(!alcMakeContextCurrent(pnvContext))
    {
        return FALSE;
    }

    if(!InitOpenALStandardEnums(pnvDevice))
    {
        printf("Failed to initialize the OpenAL enumerations!");
        goto on_error;
    }

    alGenBuffers(1, bufferNames);

    alBufferData(bufferNames[0], AL_FORMAT_MONO16, footsteps.GetDataBuffer(), footsteps.GetDataSize(), footsteps.GetSampleRate());
    alGenSources(1, sourceNames);
    alSourcei(sourceNames[0], AL_BUFFER, bufferNames[0]);

    //
    // Play in the initial position.
    //
    alSourcePlay(sourceNames[0]);
    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(!done)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    }

    alSourceStop(sourceNames[0]);
    alDeleteSources(1, sourceNames);
    alDeleteBuffers(1, bufferNames);

    alcMakeContextCurrent(0);

    //
    // Close up shop.
    //
    goto no_error;

on_error:
    error = 1;

no_error:
    if(pnvContext)
    {
        alcDestroyContext(pnvContext);
    }

    if(pnvDevice)
    {
        alcCloseDevice(pnvDevice);
    }

    if(dll)
    {
        FreeLibrary(dll);
    }

    return error;
}

