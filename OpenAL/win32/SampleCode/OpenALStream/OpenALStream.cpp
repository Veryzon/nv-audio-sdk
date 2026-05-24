//*****************************************************************************
//*****************************************************************************
//
// OpenALStream.cpp
//
//
// Description:
// This file demonstrates how to stream sounds with OpenAL.
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
#include "malloc.h"
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
#define PAGE_SIZE               4096
#define BREAK()                 _asm int 3;


//*****************************************************************************
//*****************************************************************************
//
// Locals
//
//*****************************************************************************
//*****************************************************************************

static int     error = 0;
static ALuint* bufferNames;
static ALuint  otherBuffer;
static ALint   numBuffers = 0;
static ALuint  sourceNames[1];
static ALint   done;
static DWORD   time, currenttime, newtime;


//*****************************************************************************
//*****************************************************************************
//
// Forward Declarations
//
//*****************************************************************************
//*****************************************************************************

void InitTime();
void PassTime(unsigned int n);

BOOL Test0();
BOOL Test1();
BOOL Test2();
BOOL Test3();
BOOL Test4();
BOOL Test5();
BOOL Test6();
BOOL Test7();
BOOL Test8();
BOOL Test9();
BOOL Test10();
BOOL Test11();
BOOL Test12();
BOOL Test13();
BOOL Test14();
BOOL Test15();
BOOL Test16();
BOOL Test17();
BOOL Test18();
BOOL Test19();



//*****************************************************************************
//*****************************************************************************
//
// Main Entry Point
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// main
//*****************************************************************************
//
int main(int argc, char* argv[])
{
    ALdevice*  device  = 0;
    ALcontext* context = 0;
    TCHAR path[MAX_PATH];
    HINSTANCE dll = 0;

    //
    // Load the wave file.
    //
    CWaveFile wave, otherWave;
    if(wave.LoadFile("WaveFiles\\human_footsteps.wav") != NOERROR ||
       otherWave.LoadFile("WaveFiles\\stereo.wav") != NOERROR)
    {
        printf("Failed to load the wave files!\n");
        goto on_error;
    }

    //
    // Load the library.
    //
    GetSystemDirectory(path, MAX_PATH);
    lstrcat(path, TEXT("\\OpenAL32.dll"));
    dll = LoadLibrary(path);
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
    // Setup the NVIDIA OpenAL device.
    //
    device = alcOpenDevice((ALubyte*)NVIDIA_DEVICE_NAME);
    if(!device)
    {
        printf("Failed to create \"NVIDIA nForce(TM)\"!\n");
        goto on_error;
    }

    context = alcCreateContext(device, 0);
    if(!context)
    {
        printf("Failed to create the NVIDIA device context!\n");
        goto on_error;
    }

    if(!alcMakeContextCurrent(context))
    {
        printf("Failed to make the NVIDIA device context current!\n");
        goto on_error;
    }

    if(!InitOpenALStandardEnums(device))
    {
        printf("Failed to initialize the OpenAL enumerations!");
        goto on_error;
    }

    //
    // Determine how many buffers we need.  I'm going to go with buffers that have 4 pages of data.
    //
    numBuffers = wave.GetDataSize() / (PAGE_SIZE * 4) + 1;
    bufferNames = (ALuint*)malloc(sizeof(ALuint) * numBuffers);
    if(!bufferNames)
    {
        printf("Failed to allocate the buffer names array!");
        goto on_error;
    }

    //
    // Setup the initial buffers to stream and our source.
    //
    alGenBuffers(numBuffers, bufferNames);
    ALint head;
    for(head = 0; head < numBuffers; head++)
    {
        if((head + 1) * PAGE_SIZE * 4 < (ALint)wave.GetDataSize())
        {
            alBufferData(bufferNames[head], AL_FORMAT_MONO16, (PUCHAR)wave.GetDataBuffer() + head * PAGE_SIZE * 4,
                         PAGE_SIZE * 4, wave.GetSampleRate());
        }

        else if(wave.GetDataSize() - head * PAGE_SIZE * 4 != 0)
        {
            //
            // Finish the last piece of the buffer.
            //
            alBufferData(bufferNames[head], AL_FORMAT_MONO16, (PUCHAR)wave.GetDataBuffer() + head * PAGE_SIZE * 4,
                         wave.GetDataSize() - head * PAGE_SIZE * 4, wave.GetSampleRate());
            break;
        }

        else
        {
            break;
        }
    }

    //
    // Setup one other buffer to verify swapping the format works.
    //
    alGenBuffers(1, &otherBuffer);
    alBufferData(otherBuffer, AL_FORMAT_STEREO16, otherWave.GetDataBuffer(), otherWave.GetDataSize(), otherWave.GetSampleRate());


    //
    // Make sure that no errors have occurred.
    //
    if(alGetError() != AL_NO_ERROR)
    {
        goto on_error;
    }



    //
    // Buffer queue tests.
    //
    Test0();
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();

    //
    // Streaming tests.
    //
    Test7();
    Test8();
    Test9();
    Test10();
    Test11();
    Test12();
    Test13();


    Test14();

    //
    // Verify positional abilities.
    //
    Test15();

    //
    // State transition tests.
    //
    Test16();
    Test17();
    Test18();

    //
    // Format transition test.
    //
    Test19();

    //
    // Close up shop.
    //
    alDeleteBuffers(1, &otherBuffer);
    alDeleteBuffers(numBuffers, bufferNames);
    alcMakeContextCurrent(0);
    goto no_error;

on_error:
    error = 1;

no_error:
    if(bufferNames)
    {
        free(bufferNames);
        bufferNames = 0;
    }

    if(context)
    {
        alcDestroyContext(context);
    }

    if(device)
    {
        alcCloseDevice(device);
    }

    if(dll)
    {
        FreeLibrary(dll);
    }

    return error;
}



//*****************************************************************************
//*****************************************************************************
//
// Other Functions
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// InitTime
//*****************************************************************************
//
void InitTime()
{
    time = timeGetTime();
    currenttime = time;
}


//*****************************************************************************
// PassTime
//*****************************************************************************
//
void PassTime(unsigned int n)
{
    //
    // Print out a "*" every n milliseconds.
    //
    newtime = timeGetTime();
    if(newtime - currenttime > n)
    {
        printf("*");
        currenttime = newtime;
    }
}



//*****************************************************************************
//*****************************************************************************
//
// Test Cases
//
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
// Test0
//*****************************************************************************
// Verify queue and unqueue NULL buffers.
//
BOOL Test0()
{
    printf("Test 0: Verify queue and unqueue NULL buffers.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 0: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 0: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test1
//*****************************************************************************
// Verify queue and unqueue NORMAL buffers.
//
BOOL Test1()
{
    printf("Test 1: Verify queue and unqueue NORMAL buffers.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 1: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 1: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test2
//*****************************************************************************
// Verify queue and unqueue a mix of NULL and NORMAL buffers.
//
BOOL Test2()
{
    printf("Test 2: Verify queue and unqueue a mix of NULL and NORMAL buffers.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    //
    // Try out-of-order unqueueing.
    //
    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() == AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Out of order unqueue failed as expected.\n");

    //
    // Unqueue the buffers in the correct order.
    //
    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 2: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 2: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test3
//*****************************************************************************
// Verify queue and unqueue a mix of NORMAL and NULL buffers.
//
BOOL Test3()
{
    printf("Test 3: Verify queue and unqueue a mix of NORMAL and NULL buffers.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    //
    // Try out-of-order unqueueing.
    //
    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() == AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Out of order unqueue failed as expected.\n");

    //
    // Unqueue the buffers in the correct order.
    //
    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 3: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 3: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test4
//*****************************************************************************
// Verify queue and unqueue NORMAL buffers in looping mode.
//
BOOL Test4()
{
    printf("Test 4: Verify queue and unqueue NORMAL buffers in looping mode.\n");

    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    alSourcei(sourceNames[0], AL_LOOPING, TRUE);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourcei(sourceNames[0], AL_LOOPING, FALSE);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 4: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 4: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test5
//*****************************************************************************
// Verify queue and unqueue NULL buffers in looping mode.
//
BOOL Test5()
{
    printf("Test 5: Verify queue and unqueue NULL buffers in looping mode.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourcei(sourceNames[0], AL_LOOPING, FALSE);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 5: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 5: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test6
//*****************************************************************************
// Verify switch back-and-forth between NULL buffers and NULL streams.
//
BOOL Test6()
{
    printf("Test 6: Verify switch back-and-forth between NULL buffers and NULL streams.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourcei(sourceNames[0], AL_BUFFER, 0);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 6: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 6: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test7
//*****************************************************************************
// Queue up all the buffers and play the sound.
//
BOOL Test7()
{
    printf("Test 7: Queue up all the buffers and play the sound.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 7: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 7: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test8
//*****************************************************************************
// Queue up some buffers, play the sound, queue the rest.
//
BOOL Test8()
{
    printf("Test 8: Queue up some buffers, play the sound, queue the rest.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing first set of buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 8, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing remaining buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers - 8, &bufferNames[8]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 8: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 8: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test9
//*****************************************************************************
// Queue up some buffers, play the sound, queue the rest and some NULL buffers.
//
BOOL Test9()
{
    printf("Test 9: Queue up some buffers, play the sound, queue the rest and some NULL buffers.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing NULL buffers.\n");
    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing first set of buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 8, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Queueing NULL buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing remaining buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers - 8, &bufferNames[8]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing NULL buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers + 15)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 8, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], numBuffers - 8, &bufferNames[8]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 9: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 9: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test10
//*****************************************************************************
// Test looping mode for 10 seconds.
//
BOOL Test10()
{
    printf("Test 10: Test looping mode for 10 seconds.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing NULL buffers.\n");
    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing first set of buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 8, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourcei(sourceNames[0], AL_LOOPING, TRUE);

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Queueing NULL buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing remaining buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers - 8, &bufferNames[8]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing NULL buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 10000)
        {
            break;
        }
    };

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 8, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], numBuffers - 8, &bufferNames[8]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 10: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 10: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test11
//*****************************************************************************
// Switch to looping mode on/off while playing.
//
BOOL Test11()
{
    printf("Test 11: Switch to looping mode on/off while playing.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }


    printf("Queueing NULL buffers.\n");
    ALuint nullBuffers[5] = {0, 0, 0, 0, 0};
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing first set of buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 8, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing NULL buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing remaining buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers - 8, &bufferNames[8]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Queueing NULL buffers.\n");
    alSourceQueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Play for 1 second.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 1000)
        {
            break;
        }
    };

    printf("\n");

    printf("Turning on looping mode.  Loop for 10 more seconds.\n");
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);

    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 10000)
        {
            break;
        }
    };

    printf("\n");

    printf("Turning off looping mode.  Play until done.\n");
    alSourcei(sourceNames[0], AL_LOOPING, FALSE);

    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers + 15)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 8, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], numBuffers - 8, &bufferNames[8]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], 5, nullBuffers);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 11: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 11: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test12
//*****************************************************************************
// Queue up some buffers, starve the stream, queue more buffers.
//
BOOL Test12()
{
    printf("Test 12: Queue up some buffers, starve the stream, queue more buffers.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing first set of buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    printf("Stream should be starved.  Pausing 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    printf("Queueing more buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers * 2)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 12: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 12: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test13
//*****************************************************************************
// Switch to looping mode on while starved.
//
BOOL Test13()
{
    printf("Test 13: Switch to looping mode on while starved.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    printf("Stream should be starved.  Pausing 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");

    printf("Turning on looping mode.  Loop for 10 more seconds.\n");
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);

    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 10000)
        {
            break;
        }
    };

    printf("\n");

    printf("Turning off looping mode.  Play until done.\n");
    alSourcei(sourceNames[0], AL_LOOPING, FALSE);

    alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
    while(done != numBuffers)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        PassTime(100);
    }

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Test 13: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 13: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test14
//*****************************************************************************
// Verify unqueue while playing.
//
BOOL Test14()
{
    printf("Test 14: Verify unqueue while playing.\n");
    BOOL result = TRUE;
    int index = 0;
    int i;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    for(i = 0; i < numBuffers; i++)
    {
        printf("Queued buffer 0x%08x.\n", bufferNames[i]);
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    InitTime();
    while(index != numBuffers)
    {
        alGetSourcei(sourceNames[0], AL_BUFFERS_PROCESSED, &done);
        if(done)
        {
            for(i = 0; i < done; i++)
            {
                printf("Unqueueing buffer 0x%08x.\n", bufferNames[index + i]);
            }

            alSourceUnqueueBuffers(sourceNames[0], done, &bufferNames[index]);
            if(alGetError() != AL_NO_ERROR)
            {
                BREAK();
                goto on_error;
            }

            index += done;
        }
    }

    printf("\n");

    alSourceStop(sourceNames[0]);

    printf("Test 14: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 14: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test15
//*****************************************************************************
// Verify 3D position on streams.
//
BOOL Test15()
{
    printf("Test 15: Verify 3D position on streams.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Turning on looping mode.\n");
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Sound should be coming from front center for 5 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 5000)
        {
            break;
        }
    };

    printf("\n");

    printf("Sound should be coming from front left for 5 seconds.\n");
    alSource3f(sourceNames[0], AL_POSITION, -1.0, 0.0, -1.0);
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 5000)
        {
            break;
        }
    };

    printf("\n");

    printf("Sound should be coming from front right for 5 seconds.\n");
    alSource3f(sourceNames[0], AL_POSITION, 1.0, 0.0, -1.0);
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 5000)
        {
            break;
        }
    };

    printf("\n");

    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 15: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 15: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test16
//*****************************************************************************
// Verify Play-Pause-Play.
//
BOOL Test16()
{
    printf("Test 16: Verify Play-Pause-Play.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Turning on looping mode.\n");
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);


    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    printf("Pausing sound.\n");
    alSourcePause(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Pausing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 16: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 16: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test17
//*****************************************************************************
// Verify Play-Stop-Play.
//
BOOL Test17()
{
    printf("Test 17: Verify Play-Stop-Play.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Turning on looping mode.\n");
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);


    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    printf("Stopping sound.\n");
    alSourceStop(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Stopping for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 17: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 17: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test18
//*****************************************************************************
// Verify Play-Play.
//
BOOL Test18()
{
    printf("Test 18: Verify Play-Play.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Queueing buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Turning on looping mode.\n");
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);


    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 18: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 18: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}


//*****************************************************************************
// Test19
//*****************************************************************************
// Verify format change.
//
BOOL Test19()
{
    printf("Test 19: Verify format change.\n");
    BOOL result = TRUE;

    alGenSources(1, sourceNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        return FALSE;
    }

    printf("Turning on looping mode.\n");
    alSourcei(sourceNames[0], AL_LOOPING, TRUE);


    printf("Queueing buffers.\n");
    alSourceQueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    printf("Stopping sound.\n");
    alSourceStop(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Unqueueing all buffers.\n");
    alSourceUnqueueBuffers(sourceNames[0], numBuffers, bufferNames);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Queueing buffer with new format.\n");
    alSourceQueueBuffers(sourceNames[0], 1, &otherBuffer);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing sound.\n");
    alSourcePlay(sourceNames[0]);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }

    printf("Playing for 3 seconds.\n");
    InitTime();
    while(TRUE)
    {
        PassTime(100);
        if(newtime - time > 3000)
        {
            break;
        }
    };

    printf("\n");


    alSourceStop(sourceNames[0]);

    alSourceUnqueueBuffers(sourceNames[0], 1, &otherBuffer);
    if(alGetError() != AL_NO_ERROR)
    {
        BREAK();
        goto on_error;
    }


    printf("Test 19: PASS.\n\n");
    result = TRUE;
    goto no_error;


on_error:
    printf("Test 19: FAIL.\n\n");
    result = FALSE;

no_error:
    alDeleteSources(1, sourceNames);
    return result;
}

