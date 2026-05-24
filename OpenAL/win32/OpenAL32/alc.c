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




#pragma comment(lib, "winmm.lib")

#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <al/alc.h>
#include <stdio.h>
#include <tchar.h>
#include <assert.h>

#include "OpenAL32.h"
#include "alBuffer.h"
#include "alu.h"




//*****************************************************************************
//*****************************************************************************
//
// Defines
//
//*****************************************************************************
//*****************************************************************************

#define SPEEDOFSOUNDMETRESPERSEC        (343.3f)

typedef struct ALCextension_struct
{
    ALubyte* ename;
    ALvoid * address;

} ALCextension;

typedef struct ALCfunction_struct
{
    ALubyte* fname;
    ALvoid * address;

} ALCfunction;



//*****************************************************************************
//*****************************************************************************
//
// Forward Declarations
//
//*****************************************************************************
//*****************************************************************************

// Multimedia Timer Callback function prototype
void CALLBACK TimerCallback(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
DWORD WINAPI ThreadProc(LPVOID lpParameter);

// Update Context functions
void UpdateSource(ALcontext* context, ALsource* source);
void UpdateListener(ALcontext* context);

// Call to update a particular Source
void ProcessSource(ALcontext* context, ALsource* source);



//*****************************************************************************
//*****************************************************************************
//
// Global Vars
//
//*****************************************************************************
//*****************************************************************************

ALlist* alcContextList = 0;
ALcontext* alcCurrentContext = 0;


//*****************************************************************************
//*****************************************************************************
//
// Global Functions
//
//*****************************************************************************
//*****************************************************************************

ALint LinearGainToMB(float flGain)
/*
        Helper function to convert a floating point amplitude (range 0.0 to 1.0) to millibels
*/
{
    if(flGain > 0)
        return(long)(2000.0*log10(flGain));
    else
        return -10000;
}



//*****************************************************************************
//*****************************************************************************
//
// Local Vars
//
//*****************************************************************************
//*****************************************************************************

static ALCextension alcExtensions[] =
{
    { 0, (ALvoid*)0 }
};

static ALCfunction  alcFunctions[] =
{
    { 0, (ALvoid*)0 }
};

// Error strings
static ALubyte alcNoError[] = "No Error";
static ALubyte alcErrInvalidDevice[] = "Invalid Device";
static ALubyte alcErrInvalidContext[] = "Invalid Context";
static ALubyte alcErrInvalidEnum[] = "Invalid Enum";
static ALubyte alcErrInvalidValue[] = "Invalud Value";

// Context strings
static ALubyte alcDefaultDeviceSpecifier[] = "DirectSound3D";
static ALubyte alcExtensionList[] = "";

static ALint alcMajorVersion = 1;
static ALint alcMinorVersion = 0;

static ALuint nDeviceUsingTimer = 0;
static ALuint nTimerInterval = 100;
static ALuint nTimerID = 0;



//*****************************************************************************
//*****************************************************************************
//
// Local Functions
//
//*****************************************************************************
//*****************************************************************************

static void CALLBACK alcDirectSoundProc(UINT uID, UINT uReserved, DWORD dwUser, DWORD dwReserved1, DWORD dwReserved2)
{
    static DWORD OldWriteCursor=0;
    DWORD PlayCursor,WriteCursor;
    BYTE* WritePtr1,*WritePtr2;
    DWORD WriteCnt1,WriteCnt2;
    ALcontext* alContext;
    ALdevice* alDevice;
    DWORD BytesPlayed;
    HRESULT DSRes;

    alDevice=(ALdevice*)dwUser;
    IDirectSoundBuffer_GetCurrentPosition(alDevice->DSsbuffer,&PlayCursor,&WriteCursor);
    if(!OldWriteCursor) OldWriteCursor=WriteCursor-PlayCursor;
    BytesPlayed=((((long)WriteCursor-(long)OldWriteCursor)<0)?((long)32768+(long)WriteCursor-(long)OldWriteCursor):((DWORD)WriteCursor-(DWORD)OldWriteCursor));
    DSRes=IDirectSoundBuffer_Lock(alDevice->DSsbuffer,(OldWriteCursor+3528)&32767,BytesPlayed,&WritePtr1,&WriteCnt1,&WritePtr2,&WriteCnt2,0);
    if(DSRes==DSERR_BUFFERLOST)
    {
        IDirectSoundBuffer_Restore(alDevice->DSsbuffer);
        IDirectSoundBuffer_Play(alDevice->DSsbuffer,0,0,DSBPLAY_LOOPING);
        DSRes=IDirectSoundBuffer_Lock(alDevice->DSsbuffer,(OldWriteCursor+3528)&32767,BytesPlayed,&WritePtr1,&WriteCnt1,&WritePtr2,&WriteCnt2,0);
    }

    if(DSRes==DS_OK)
    {
        // TODO: To support mulitple contexts per DSOUND device, the device should maintain a list of contexts that are
        // assigned to it and mix the data from each non-suspended context into the device buffer.  For now, we will
        // only support the current context.
        alListAcquireLock(alcContextList);

        if(alcCurrentContext && alcCurrentContext->Device == alDevice)
        {
            alContext = alcCurrentContext;
            EnterCriticalSection(&alContext->Lock);
            alListReleaseLock(alcContextList);

            // Check if the context is suspended.
            if(!alContext->Suspended)
            {
                if(WritePtr1)
                    aluMixData(alContext, WritePtr1, WriteCnt1, alContext->Format);
                if(WritePtr2)
                    aluMixData(alContext, WritePtr2, WriteCnt2, alContext->Format);
            }

            else
            {
                if(WritePtr1)
                    memset(WritePtr1,0,WriteCnt1);
                if(WritePtr2)
                    memset(WritePtr2,0,WriteCnt2);
            }

            LeaveCriticalSection(&alcCurrentContext->Lock);
        }

        else
        {
            alListReleaseLock(alcContextList);
        }

        IDirectSoundBuffer_Unlock(alDevice->DSsbuffer,WritePtr1,WriteCnt1,WritePtr2,WriteCnt2);
    }

    OldWriteCursor=WriteCursor;
}


static void CALLBACK alcWaveOutProc(HWAVEOUT hDevice, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    ALdevice* alDevice;
    alDevice = (ALdevice*)dwInstance;
    if(uMsg == WOM_DONE)
    {
        // Decrement number of buffers in use
        alDevice->BuffersCommitted--;
        if(alDevice->bWaveShutdown == AL_FALSE)
        {
            // Notify Wave Processor Thread that a Wave Header has returned
            PostThreadMessage(alDevice->ThreadID, uMsg, 0, dwParam1);
        }

        else
        {
            if(alDevice->BuffersCommitted == 0)
            {
                // Signal Wave Buffers Returned event
                if(alDevice->hWaveHdrEvent)
                    SetEvent(alDevice->hWaveHdrEvent);

                // Post 'Quit' Message to Wave Processor Thread
                PostThreadMessage(alDevice->ThreadID, WM_QUIT, 0, 0);
            }
        }
    }
}


DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    ALcontext* alContext;
    ALdevice* alDevice;
    LPWAVEHDR WaveHdr;
    MSG msg;

    alDevice=(ALdevice*)lpParameter;
    while(GetMessage(&msg, 0, 0, 0))
    {
        if(msg.message == WOM_DONE && !alDevice->bWaveShutdown)
        {
            // TODO: To support mulitple contexts per WaveOut device, the device should maintain a list of contexts that are
            // assigned to it and mix the data from each non-suspended context into the device buffer.  For now, we will
            // only support the current context.
            alListAcquireLock(alcContextList);

            if(alcCurrentContext && alcCurrentContext->Device == alDevice)
            {
                alContext = alcCurrentContext;
                EnterCriticalSection(&alContext->Lock);
                alListReleaseLock(alcContextList);

                // Check if the context is suspended.
                WaveHdr=((LPWAVEHDR)msg.lParam);
                if(!alContext->Suspended)
                {
                    aluMixData(alContext, WaveHdr->lpData, WaveHdr->dwBufferLength, alContext->Format);
                }

                else
                {
                    memset(WaveHdr->lpData, 0, WaveHdr->dwBufferLength);
                }

                LeaveCriticalSection(&alcCurrentContext->Lock);

                waveOutWrite(alDevice->handle, WaveHdr, sizeof(WAVEHDR));
                alDevice->BuffersCommitted++;
            }

            else
            {
                alListReleaseLock(alcContextList);
            }
        }
    }

    // Signal Wave Thread completed event
    if(alDevice->hWaveThreadEvent)
        SetEvent(alDevice->hWaveThreadEvent);

    ExitThread(0);

    return 0;
}


ALvoid alcInitContext(ALcontext* context)
{
    //Initialize listener
    context->Listener.Gain = 1.0f;

    context->Listener.Position[0] =  0.0f;
    context->Listener.Position[1] =  0.0f;
    context->Listener.Position[2] =  0.0f;

    context->Listener.Velocity[0] =  0.0f;
    context->Listener.Velocity[1] =  0.0f;
    context->Listener.Velocity[2] =  0.0f;

    context->Listener.Forward[0]  =  0.0f;
    context->Listener.Forward[1]  =  0.0f;
    context->Listener.Forward[2]  = -1.0f;

    context->Listener.Up[0] = 0.0f;
    context->Listener.Up[1] = 1.0f;
    context->Listener.Up[2] = 0.0f;

    //Set output format
    context->Frequency = context->Device->Frequency;
    context->Channels  = context->Device->Channels;
    context->Format    = context->Device->Format;

    //Set globals
    context->DistanceModel = AL_INVERSE_DISTANCE_CLAMPED;
    context->DopplerFactor = 1.0f;
    context->DopplerVelocity = SPEEDOFSOUNDMETRESPERSEC;

    // Initialize update to set all the Listener parameters
    context->Listener.update1 = LPOSITION | LVELOCITY | LORIENTATION | LDOPPLERFACTOR | LDOPPLERVELOCITY | LDISTANCEMODEL;
    alcUpdateContext(context, ALLISTENER, 0);
}


ALvoid alcExitContext(ALcontext* context)
{
    unsigned int i;
    ALsource* alSource;
    ALsource* alTempSource;
#ifdef _DEBUG
    char szString[256];
#endif

#ifdef _DEBUG
    if(context->SourceCount>0)
    {
        sprintf(szString,"OpenAL32 : alcDestroyContext() %d Source(s) NOT deleted\n", context->SourceCount);
        OutputDebugString(szString);
    }
#endif

    // Free all the Sources still remaining
    alSource = context->Source;
    for(i = 0; i < context->SourceCount; i++)
    {
        if(alSource->uservalue1)
        {
            IDirectSoundBuffer_Stop((LPDIRECTSOUNDBUFFER)alSource->uservalue1);
            if(alSource->uservalue3)
            {
                IKsPropertySet_Release((LPKSPROPERTYSET)alSource->uservalue3);
                alSource->uservalue3 = 0;
            }
            if(alSource->uservalue2)
            {
                IDirectSound3DBuffer_Release((LPDIRECTSOUND3DBUFFER)alSource->uservalue2);
                alSource->uservalue2 = 0;
            }
            IDirectSoundBuffer_Release((LPDIRECTSOUNDBUFFER)alSource->uservalue1);
            alSource->uservalue1=0;
        }

        alTempSource = alSource->next;
        memset(alSource,0,sizeof(ALsource));
        free(alSource);
        alSource = alTempSource;
    }
}


void CALLBACK TimerCallback(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    ALuint i, loop;
    ALuint PlayCursor, WriteCursor, DataToLock;
    ALuint BufferID, DataSize;
    ALuint Part1Size, Part2Size;
    ALuint Data, BytesWritten, DataLeft;
    ALuint DataPlayed, DataCount;
    ALuint BytesPlayed, BufferSize;
    ALvoid* lpPart1, *lpPart2;
    ALsource* alSource;
    ALuint BuffersToSkip;
    ALcontext* alContext;
    ALbufferlistitem* ALBufferListItem;
    ALlistEntry* entry;

    // Loop through each context and update those that we own.
    alListAcquireLock(alcContextList);
    for(entry = alListIteratorReset(alcContextList); entry;
        entry = alListIteratorNext(alcContextList))
    {
        alContext = (ALcontext*)alListGetData(entry);
        if(alContext->DllContext)
        {
            continue;
        }

        EnterCriticalSection(&alContext->Lock);

        if(alContext->Suspended)
        {
            LeaveCriticalSection(&alContext->Lock);
            continue;
        }

        alSource = alContext->Source;

        // Process each playing source
        for(loop=0;loop < alContext->SourceCount;loop++)
        {
            if(alIsSourceSafe(alContext, (ALuint)alSource) && (alSource->state == AL_PLAYING))
            {
                // Valid playing source

                // Get position in DS Buffer
                IDirectSoundBuffer_GetCurrentPosition((LPDIRECTSOUNDBUFFER)alSource->uservalue1, &PlayCursor, &WriteCursor);

                // Lock buffer from Old Write cursor to current Play cursor
                if(alSource->OldWriteCursor > PlayCursor)
                    DataToLock = (88200 - alSource->OldWriteCursor) + PlayCursor;
                else
                    DataToLock = PlayCursor - alSource->OldWriteCursor;

                // Calculate amount of data played since last Timer event
                if(alSource->OldPlayCursor > PlayCursor)
                    alSource->BytesPlayed += ((88200 - alSource->OldPlayCursor) + PlayCursor);
                else
                    alSource->BytesPlayed += (PlayCursor - alSource->OldPlayCursor);

                // Update current buffer variable

                // Find position in queue
                BytesPlayed = alSource->BytesPlayed;
                if(BytesPlayed >= alSource->TotalBufferDataSize)
                {
                    if(alSource->TotalBufferDataSize == 0)
                        assert(alSource->TotalBufferDataSize != 0);
                    BytesPlayed = (BytesPlayed % alSource->TotalBufferDataSize);
                }

                ALBufferListItem = alSource->queue;
                DataSize = 0;
                while(ALBufferListItem != 0)
                {
                    if(ALBufferListItem->buffer)
                        BufferSize = ((ALbuffer*)(ALBufferListItem->buffer))->size;
                    else
                        BufferSize = 0;
                    DataSize += BufferSize;
                    if(DataSize > BytesPlayed)
                        break;
                    else
                        ALBufferListItem = ALBufferListItem->next;
                }

                // Record current BufferID
                BufferID = ALBufferListItem->buffer;

                // If we are not looping, decrement DataStillToPlay by the amount played since the last
                // Timer event, and check if any buffers in the queue have finished playing
                // Also check if the Source has now finished playing
                if(alSource->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i == AL_FALSE)
                {
                    if(PlayCursor >= alSource->OldPlayCursor)
                        alSource->DataStillToPlay -= (PlayCursor - alSource->OldPlayCursor);
                    else
                        alSource->DataStillToPlay -= ((88200 - alSource->OldPlayCursor) + PlayCursor);

                    if(alSource->DataStillToPlay < 0)
                        alSource->DataStillToPlay = 0;

                    // Check if any buffers in the queue have finished playing - if they have adjust
                    // their state to PROCESSED

                    DataPlayed = alSource->TotalBufferDataSize - alSource->DataStillToPlay;

                    DataCount = 0;
                    alSource->BuffersProcessed = 0;
                    ALBufferListItem = alSource->queue;
                    while(ALBufferListItem != 0)
                    {
                        if(ALBufferListItem->buffer)
                            DataSize = ((ALbuffer*)ALBufferListItem->buffer)->size;
                        else
                            DataSize = 0;

                        DataCount += DataSize;
                        if(DataCount <= DataPlayed)
                        {
                            // Buffer has been played
                            ALBufferListItem->bufferstate = PROCESSED;
                            alSource->BuffersProcessed++;
                            ALBufferListItem = ALBufferListItem->next;
                        }
                        else
                            ALBufferListItem = 0;
                    }



                    // Check if finished - if so stop source !
                    if(alSource->DataStillToPlay == 0)
                    {
                        IDirectSoundBuffer_Stop((LPDIRECTSOUNDBUFFER)alSource->uservalue1);
                        IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)alSource->uservalue1, 0);
                        alSource->state = AL_STOPPED;

                        // Reset variables
                        alSource->BufferPosition = 0;
                        alSource->BytesPlayed = 0;
                        alSource->FinishedQueue = AL_FALSE;
                        alSource->OldPlayCursor = 0;
                        alSource->OldWriteCursor = 0;
                        alSource->SilenceAdded = 0;

                        // Move on to next source
                        continue;
                    }


                }


                if(alSource->FinishedQueue)
                {
                    // Finished copying audio data into source, but data hasn't finished playing yet
                    // Therefore copy silence into DS Buffer
                    IDirectSoundBuffer_Lock((LPDIRECTSOUNDBUFFER)alSource->uservalue1, alSource->OldWriteCursor, DataToLock, &lpPart1, &Part1Size, &lpPart2, &Part2Size, 0);

                    if(lpPart1)
                    {
                        memset(lpPart1, 0, Part1Size);
                        alSource->SilenceAdded += Part1Size;
                        alSource->OldWriteCursor += Part1Size;
                        if(alSource->OldWriteCursor >= 88200)
                            alSource->OldWriteCursor -= 88200;
                    }

                    if(lpPart2)
                    {
                        memset(lpPart2, 0, Part2Size);
                        alSource->SilenceAdded += Part2Size;
                        alSource->OldWriteCursor += Part2Size;
                        if(alSource->OldWriteCursor >= 88200)
                            alSource->OldWriteCursor -= 88200;
                    }

                    IDirectSoundBuffer_Unlock((LPDIRECTSOUNDBUFFER)alSource->uservalue1, lpPart1, Part1Size, lpPart2, Part2Size);

                    // Update Old Play Cursor
                    alSource->OldPlayCursor = PlayCursor;
                }
                else
                {
                    IDirectSoundBuffer_Lock((LPDIRECTSOUNDBUFFER)alSource->uservalue1, alSource->OldWriteCursor, DataToLock, &lpPart1, &Part1Size, &lpPart2, &Part2Size, 0);

                    if(lpPart1 != 0)
                    {
                        // Find position in buffer queue
                        BuffersToSkip = alSource->BuffersAddedToDSBuffer;
                        if(BuffersToSkip >= alSource->BuffersInQueue)
                            BuffersToSkip = BuffersToSkip % alSource->BuffersInQueue;

                        ALBufferListItem = alSource->queue;
                        for(i = 0; i < BuffersToSkip; i++)
                        {
                            ALBufferListItem = ALBufferListItem->next;
                        }

                        BytesWritten = 0;
                        BufferID = ALBufferListItem->buffer;

                        while(AL_TRUE)
                        {
                            // Copy audio data from Open AL Buffer(s) into DS buffer

                            // Find out how much data is left in current Open AL Buffer
                            if(BufferID)
                            {
                                Data = (ALint)(((ALbuffer*)BufferID)->data);
                                DataSize = ((ALbuffer*)BufferID)->size;
                            }
                            else
                            {
                                Data = 0;
                                DataSize = 0;
                            }

                            if(DataSize == 0)
                                DataLeft = 0;
                            else
                                DataLeft = DataSize - alSource->BufferPosition;

                            if(DataLeft > (Part1Size - BytesWritten))
                            {
                                // Copy (Part1Size - BytesWritten) bytes to Direct Sound buffer
                                memcpy((ALubyte*)lpPart1 + BytesWritten, (ALubyte*)Data + alSource->BufferPosition, Part1Size - BytesWritten);
                                alSource->FinishedQueue = AL_FALSE;     // More data to follow ...
                                alSource->BufferPosition += (Part1Size - BytesWritten);         // Record position in buffer data
                                BytesWritten += (Part1Size - BytesWritten);
                                break;
                            }
                            else
                            {
                                // Not enough data in buffer to fill DS buffer so just copy as much data as possible
                                if((Data) && (DataLeft > 0))
                                    memcpy((ALubyte*)lpPart1 + BytesWritten, (ALubyte*)Data + alSource->BufferPosition, DataLeft);

                                BytesWritten += DataLeft;

                                alSource->BuffersAddedToDSBuffer++;

                                alSource->BufferPosition = 0;

                                // Get next valid buffer ID
                                ALBufferListItem = ALBufferListItem->next;

                                if(ALBufferListItem == 0)
                                {
                                    // No more buffers - check for looping flag
                                    if(alSource->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i == AL_TRUE)
                                    {
                                        // Looping
                                        ALBufferListItem = alSource->queue;
                                    }
                                    else
                                    {
                                        // Not looping and no more buffers
                                        alSource->FinishedQueue = AL_TRUE;
                                        break;
                                    }
                                }

                                BufferID = ALBufferListItem->buffer;
                            }
                        }

                        if(BytesWritten < Part1Size)
                        {
                            // Fill the rest of the buffer with silence
                            memset((ALubyte*)lpPart1 + BytesWritten, 0, Part1Size - BytesWritten);
                            alSource->SilenceAdded += (Part1Size - BytesWritten);
                        }

                        alSource->OldWriteCursor += Part1Size;
                        if(alSource->OldWriteCursor >= 88200)
                            alSource->OldWriteCursor -= 88200;
                    }


                    if(lpPart2 != 0)
                    {
                        if(alSource->FinishedQueue)
                        {
                            // Fill Part 2 with silence
                            memset(lpPart2, 0, Part2Size);
                        }
                        else
                        {
                            // Find position in buffer queue
                            BuffersToSkip = alSource->BuffersAddedToDSBuffer;
                            if(BuffersToSkip >= alSource->BuffersInQueue)
                                BuffersToSkip = BuffersToSkip % alSource->BuffersInQueue;

                            ALBufferListItem = alSource->queue;
                            for(i = 0; i < BuffersToSkip; i++)
                            {
                                ALBufferListItem = ALBufferListItem->next;
                            }

                            BytesWritten = 0;
                            BufferID = ALBufferListItem->buffer;

                            while(AL_TRUE)
                            {
                                // Copy audio data from Open AL Buffer(s) into DS buffer

                                // Find out how much data is left in current Open AL Buffer
                                if(BufferID)
                                {
                                    Data = (ALint)((ALbuffer*)BufferID)->data;
                                    DataSize = ((ALbuffer*)BufferID)->size;
                                }
                                else
                                {
                                    Data = 0;
                                    DataSize = 0;
                                }

                                if(DataSize == 0)
                                    DataLeft = 0;
                                else
                                    DataLeft = DataSize - alSource->BufferPosition;

                                if(DataLeft > (Part2Size - BytesWritten))
                                {
                                    // Copy (Part1Size - BytesWritten) bytes to Direct Sound buffer
                                    memcpy((ALubyte*)lpPart2 + BytesWritten, (ALubyte*)Data + alSource->BufferPosition, Part2Size - BytesWritten);
                                    alSource->FinishedQueue = AL_FALSE;     // More data to follow ...
                                    alSource->BufferPosition += (Part2Size - BytesWritten);         // Record position in buffer data
                                    BytesWritten += (Part2Size - BytesWritten);
                                    break;
                                }
                                else
                                {
                                    // Not enough data in buffer to fill DS buffer so just copy as much data as possible
                                    if((Data) && (DataLeft > 0))
                                        memcpy((ALubyte*)lpPart2 + BytesWritten, (ALubyte*)Data + alSource->BufferPosition, DataLeft);

                                    BytesWritten += DataLeft;

                                    alSource->BuffersAddedToDSBuffer++;

                                    alSource->BufferPosition = 0;

                                    // Get next valid buffer ID
                                    ALBufferListItem = ALBufferListItem->next;

                                    if(ALBufferListItem == 0)
                                    {
                                        // No more buffers - check for looping flag
                                        if(alSource->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i == AL_TRUE)
                                        {
                                            // Looping
                                            ALBufferListItem = alSource->queue;
                                        }
                                        else
                                        {
                                            // Not looping and no more buffers
                                            alSource->FinishedQueue = AL_TRUE;
                                            break;
                                        }
                                    }

                                    BufferID = ALBufferListItem->buffer;
                                }
                            }

                            if(BytesWritten < Part2Size)
                            {
                                // Fill the rest of the buffer with silence
                                memset((ALubyte*)lpPart2 + BytesWritten, 0, Part2Size - BytesWritten);
                                alSource->SilenceAdded += (Part2Size - BytesWritten);
                            }

                            alSource->OldWriteCursor += Part2Size;
                            if(alSource->OldWriteCursor >= 88200)
                                alSource->OldWriteCursor -= 88200;
                        }
                    }

                    alSource->OldPlayCursor = PlayCursor;

                    IDirectSoundBuffer_Unlock((LPDIRECTSOUNDBUFFER)alSource->uservalue1, lpPart1, Part1Size, lpPart2, Part2Size);
                } // end else

                // If we are still playing (we may have reached the end) then update current buffer
                if(alSource->state == AL_PLAYING)
                    alSource->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = BufferID;
            }

            alSource = alSource->next;
        }

        LeaveCriticalSection(&alContext->Lock);
    }

    alListReleaseLock(alcContextList);
    return;
}


ALvoid alcUpdateContext(ALcontext* context, ALuint type, ALuint name)
{
    ALcontext* alContext;
    ALsource* alSource;

    alContext = context;
    alSource = (ALsource*)name;

    if(!alContext->Suspended && alContext->Device)
    {
        //Platform specific context updating
        if((alContext->Device->DShandle)&&(alContext->Device->DS3dlistener))
        {
            // Check if we need to update a Source
            if(type == ALSOURCE && alIsSourceSafe(alContext, name) && alSource->update1)
            {
                // First check for any Open AL Updates (e.g Position, Velocity, Looping etc ...)
                if(alSource->update1)
                    UpdateSource(alContext, alSource);

                // If we need to actually start playing the sound, do it now
                if(alSource->play)
                {
                    if(alSource->uservalue1)
                    {
                        // Start playing the DS Streaming buffer (always looping)
                        IDirectSoundBuffer_Play((LPDIRECTSOUNDBUFFER)alSource->uservalue1,0,0,DSBPLAY_LOOPING);
                        alSource->play=AL_FALSE;
                    }
                }
            }

            // Check for Listener related updates
            if(type == ALLISTENER && alContext->Device->DS3dlistener)
            {
                // Update any Open AL Listener Properties (e.g Position, Velocity, Orientation etc ...)
                if(alContext->Listener.update1)
                    UpdateListener(alContext);
            }
        }
    }
}


void ProcessSource(ALcontext* context, ALsource* source)
/*
    ProcessSource(ALcontext, ALsource)

    Obtain up-to-date status information for the given Source - e.g. Number of Buffers Processed,
    Current Buffer, and the current State.  If this function detects that the Source has finished playing
    it will stop the DS Buffer, and reset all the control variables.

    Note : This function must not update any of the global control variables of the Source (except when
    stopping the Source), otherwise the Timer callback function will perform incorrectly.

    Note 2 : Some problems seem to occur on some soundcard drivers if this function is called too
    often ( > 100 times a second).
*/
{
    ALuint PlayCursor, WriteCursor;
    ALuint BufferID, DataSize;
    ALuint DataPlayed, DataCount;
    ALuint BytesPlayed, BufferSize;
    ALint  DataStillToPlay;
    ALbufferlistitem* ALBufferListItem;
    ALuint  curTime;
    static ALuint lastTime = 0;

    // Only update the state of the Source at the most 100 times a second !
    curTime = GetTickCount();
    if(curTime < (lastTime + 10))
        return;
    else
        lastTime = curTime;

    if(source->state == AL_PLAYING)
    {
        // Get position in DS Buffer

        PlayCursor = source->OldPlayCursor;
        if(IDirectSoundBuffer_GetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1, &PlayCursor, &WriteCursor) != DS_OK)
            return;

        // Calculate amount of data played since last update
        BytesPlayed = source->BytesPlayed;

        if(source->OldPlayCursor > PlayCursor)
            BytesPlayed += ((88200 - source->OldPlayCursor) + PlayCursor);
        else
            BytesPlayed += (PlayCursor - source->OldPlayCursor);

        // Check for wrap-around (for looping Sources)
        if(BytesPlayed >= source->TotalBufferDataSize)
        {
            if(source->TotalBufferDataSize == 0)
                assert(source->TotalBufferDataSize != 0);
            BytesPlayed = (BytesPlayed % source->TotalBufferDataSize);
        }

        // Determine current Buffer
        DataSize = 0;
        ALBufferListItem = source->queue;
        while(ALBufferListItem != 0)
        {
            if(ALBufferListItem->buffer)
                BufferSize = ((ALbuffer*)(ALBufferListItem->buffer))->size;
            else
                BufferSize = 0;
            DataSize += BufferSize;
            if(DataSize > BytesPlayed)
                break;
            else
                ALBufferListItem = ALBufferListItem->next;
        }

        // Record current BufferID
        BufferID = ALBufferListItem->buffer;

        // If we are not looping, decrement DataStillToPlay by the amount played since the last
        // Timer event, and check if any buffers in the queue have finished playing
        DataStillToPlay = source->DataStillToPlay;
        if(source->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i == AL_FALSE)
        {
            if(PlayCursor >= source->OldPlayCursor)
                DataStillToPlay -= (PlayCursor - source->OldPlayCursor);
            else
                DataStillToPlay -= ((88200 - source->OldPlayCursor) + PlayCursor);

            if(DataStillToPlay < 0)
                DataStillToPlay = 0;

            // Check if any buffers in the queue have finished playing - if they have adjust
            // their state to PROCESSED

            DataPlayed = source->TotalBufferDataSize - DataStillToPlay;

            DataCount = 0;
            source->BuffersProcessed = 0;
            ALBufferListItem = source->queue;
            while(ALBufferListItem != 0)
            {
                if(ALBufferListItem->buffer)
                    DataSize = ((ALbuffer*)ALBufferListItem->buffer)->size;
                else
                    DataSize = 0;
                DataCount += DataSize;
                if(DataCount <= DataPlayed)
                {
                    // Buffer has been played
                    ALBufferListItem->bufferstate = PROCESSED;
                    source->BuffersProcessed++;
                    ALBufferListItem = ALBufferListItem->next;
                }
                else
                    ALBufferListItem = 0;
            }

            // Check if finished - if so stop source !
            if(DataStillToPlay <= 0)
            {
                IDirectSoundBuffer_Stop((LPDIRECTSOUNDBUFFER)source->uservalue1);
                IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1, 0);
                source->state = AL_STOPPED;

                // Reset variables
                source->BufferPosition = 0;
                source->BytesPlayed = 0;
                source->DataStillToPlay = 0;
                source->FinishedQueue = AL_FALSE;
                source->OldPlayCursor = 0;
                source->OldWriteCursor = 0;
                source->SilenceAdded = 0;
            }
        }

        // If we are still playing (we may have reached the end) then update current buffer
        if(source->state == AL_PLAYING)
            source->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = BufferID;
    }

    return;
}


void UpdateSource(ALcontext* context, ALsource* source)
{
    WAVEFORMATEX OutputType;
    DSBUFFERDESC DSBDescription;
    ALfloat Dir[3], Pos[3], Vel[3];
    ALuint  BytesWritten, DataLeft, BuffersToSkip, DataPlayed, DataCount, TotalDataSize;
    ALint   BufferSize, DataCommitted;
    ALint   Relative;
    ALuint  Data, Freq, State, Channels, outerAngle, innerAngle;
    ALfloat Pitch, outerGain, maxDist, minDist, Gain;
    ALvoid* lpPart1;
    ALuint  Part1Size, DataSize;
    ALuint PlayCursor, WriteCursor;
    ALuint BufferID, status, Loop, i;
    ALbufferlistitem* ALBufferListItem;
    ALbufferlistitem* ALBufferListTemp;
    ALint   volume;

    if(source->update1 & SUPDATEBUFFERS)
    {
        // Force an update to recalculate the number of buffers processed, and the current buffer
        // (and the state of the Source)
        ProcessSource(context, source);

        source->update1 &= ~SUPDATEBUFFERS;
        if(source->update1 == 0)
            return;
    }

    // Check if the Source is being Destroyed
    if(source->update1 == SDELETE)
    {
        // Destroy source
        if(source->uservalue1)
        {
            IDirectSoundBuffer_Stop((LPDIRECTSOUNDBUFFER)source->uservalue1);
            if(source->uservalue3)
            {
                IKsPropertySet_Release((LPKSPROPERTYSET)source->uservalue3);
                source->uservalue3 = 0;
            }
            if(source->uservalue2)
            {
                IDirectSound3DBuffer_Release((LPDIRECTSOUND3DBUFFER)source->uservalue2);
                source->uservalue2 = 0;
            }
            IDirectSoundBuffer_Release((LPDIRECTSOUNDBUFFER)source->uservalue1);
            source->uservalue1=0;

            source->update1 &= ~SDELETE;
            if(source->update1 == 0)
                return;
        }
    }

    // Check if we need to generate a new Source
    if(source->update1 & SGENERATESOURCE)
    {
        // Create a streaming DS buffer - 16bit mono 44.1KHz, 1 second in length
        memset(&DSBDescription,0,sizeof(DSBUFFERDESC));
        DSBDescription.dwSize=sizeof(DSBUFFERDESC);
        DSBDescription.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRL3D|DSBCAPS_GLOBALFOCUS|
                               DSBCAPS_GETCURRENTPOSITION2|context->Device->CreationFlag;
        DSBDescription.dwBufferBytes=88200;
        DSBDescription.lpwfxFormat=&OutputType;
        memset(&OutputType,0,sizeof(WAVEFORMATEX));
        OutputType.wFormatTag=WAVE_FORMAT_PCM;
        OutputType.nChannels=1;
        OutputType.wBitsPerSample=16;
        OutputType.nBlockAlign=2;
        OutputType.nSamplesPerSec=44100;
        OutputType.nAvgBytesPerSec=88200;
        OutputType.cbSize=0;
        if(IDirectSound_CreateSoundBuffer(context->Device->DShandle,&DSBDescription,&(LPDIRECTSOUNDBUFFER)source->uservalue1,0)==DS_OK)
        {
            IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1,0);

            // Set Volume
            Gain = source->param[AL_GAIN-AL_CONE_INNER_ANGLE].f;
            Gain = (Gain * context->Listener.Gain);
            volume = LinearGainToMB(Gain);
            IDirectSoundBuffer_SetVolume((LPDIRECTSOUNDBUFFER)source->uservalue1, volume);

            // Get 3D Interface
            if(IDirectSoundBuffer_QueryInterface((LPDIRECTSOUNDBUFFER)source->uservalue1,&IID_IDirectSound3DBuffer,(LPUNKNOWN*)&source->uservalue2)==DS_OK)
            {
                // Get Property Set Interface
                IDirectSound3DBuffer_QueryInterface((LPDIRECTSOUND3DBUFFER)source->uservalue2,&IID_IKsPropertySet,(LPUNKNOWN*)&source->uservalue3);
            }
        }

        source->update1 &= ~SGENERATESOURCE;
        if(source->update1 == 0)
            return;
    }

    // Check if we need to Stop, Start, Pause, or Resume a Source
    if(source->update1 & STATE)
    {
        State = source->state;

        switch(State)
        {
            case AL_INITIAL:
                if(source->uservalue1)
                    IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1,0);
                break;
            case AL_PLAYING:
                if(source->play)
                {
                    // Check if this Source is already playing
                    IDirectSoundBuffer_GetStatus((LPDIRECTSOUNDBUFFER)source->uservalue1,&status);

                    // If already playing, stop source, set position to zero, mark all items in
                    // queue as PENDING ready to start playback
                    if(status & DSBSTATUS_PLAYING)
                    {
                        IDirectSoundBuffer_Stop((LPDIRECTSOUNDBUFFER)source->uservalue1);
                        IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1,0);

                        // Mark all buffers in the queue as PENDING
                        ALBufferListItem = source->queue;
                        while(ALBufferListItem != 0)
                        {
                            ALBufferListItem->bufferstate = PENDING;
                            ALBufferListItem = ALBufferListItem->next;
                        }

                        source->BuffersProcessed = 0;
                        source->BuffersAddedToDSBuffer = 0;
                        source->BufferPosition = 0;
                        source->BytesPlayed = 0;
                        source->DataStillToPlay = 0;
                        source->FinishedQueue = AL_FALSE;
                        source->OldPlayCursor = 0;
                        source->OldWriteCursor = 0;
                    }

                    // Check that we have some data to play
                    if(source->BuffersInQueue == 0)
                    {
                        source->play = AL_FALSE;

                        // temp
                        source->state = AL_STOPPED;

                        break;
                    }

                    if(source->TotalBufferDataSize == 0)
                    {
                        // Nothing to play (the buffers must be of length 0) - mark them as processed

                        // Mark all buffers in queue as PROCESSED
                        source->BuffersProcessed = source->BuffersInQueue;

                        ALBufferListItem= source->queue;
                        while(ALBufferListItem != 0)
                        {
                            ALBufferListItem->bufferstate = PROCESSED;
                            ALBufferListItem = ALBufferListItem->next;
                        }

                        // No need to start playing !
                        source->play = AL_FALSE;

                        // temp
                        source->state = AL_STOPPED;

                        break;
                    }

                    // If looping has been enabled, make sure that all buffers are PENDING
                    Loop = source->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i;

                    if(Loop == AL_TRUE)
                    {
                        ALBufferListItem = source->queue;
                        while(ALBufferListItem != 0)
                        {
                            ALBufferListItem->bufferstate = PENDING;
                            ALBufferListItem = ALBufferListItem->next;
                        }
                        source->BuffersProcessed = 0;
                    }

                    // Find position in buffer queue
                    BuffersToSkip = source->BuffersAddedToDSBuffer;
                    if(BuffersToSkip > source->BuffersInQueue)
                    {
                        BuffersToSkip = BuffersToSkip % source->BuffersInQueue;
                    }

                    ALBufferListItem = source->queue;
                    for(i = 0; i < BuffersToSkip; i++)
                    {
                        ALBufferListItem = ALBufferListItem->next;
                    }

                    // Mark any buffers at the start of the list as processed if they have bufferID == 0, or
                    // if they have length 0 bytes
                    while(ALBufferListItem)
                    {
                        if(ALBufferListItem->buffer)
                            BufferSize = ((ALbuffer*)ALBufferListItem->buffer)->size;
                        else
                            BufferSize = 0;
                        if(BufferSize == 0)
                        {
                            // Skip over this buffer (and mark as processed)
                            ALBufferListItem->bufferstate = PROCESSED;
                            source->BuffersProcessed++;
                            ALBufferListItem = ALBufferListItem->next;
                            source->BuffersAddedToDSBuffer++;
                        }
                        else
                        {
                            // Found a valid buffer
                            break;
                        }
                    }

                    // Check if we still have something to play !
                    if(ALBufferListItem == 0)
                    {
                        // No buffers to play - remove play flag
                        source->play = AL_FALSE;

                        // temp
                        source->state = AL_STOPPED;

                        break;
                    }

                    // Update current buffer variable
                    BufferID = ALBufferListItem->buffer;
                    source->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = BufferID;

                    // Update variables
                    source->BytesPlayed = 0;
                    source->OldWriteCursor = 0;
                    source->OldPlayCursor = 0;

                    // Calculate how much data still to play
                    DataSize = ((ALbuffer*)BufferID)->size;
                    source->DataStillToPlay = DataSize - source->BufferPosition;

                    ALBufferListTemp = ALBufferListItem;

                    while(ALBufferListTemp->next != 0)
                    {
                        if(ALBufferListTemp->next->buffer)
                            DataSize = ((ALbuffer*)ALBufferListTemp->next->buffer)->size;
                        else
                            DataSize = 0;
                        source->DataStillToPlay += DataSize;
                        ALBufferListTemp = ALBufferListTemp->next;
                    }

                    // Check if the buffer is stereo
                    Channels = (((((ALbuffer*)BufferID)->format==AL_FORMAT_MONO8)||(((ALbuffer*)BufferID)->format==AL_FORMAT_MONO16))?1:2);

                    if((Channels == 2) && (source->SourceType == SOURCE3D))
                    {
                        // Playing a stereo buffer

                        // Need to destroy the DS Streaming Mono 3D Buffer and create a Stereo 2D buffer
                        if(source->uservalue3)
                        {
                            IKsPropertySet_Release((LPKSPROPERTYSET)source->uservalue3);
                            source->uservalue3 = 0;
                        }
                        if(source->uservalue2)
                        {
                            IDirectSound3DBuffer_Release((LPDIRECTSOUND3DBUFFER)source->uservalue2);
                            source->uservalue2 = 0;
                        }
                        IDirectSoundBuffer_Release((LPDIRECTSOUNDBUFFER)source->uservalue1);
                        source->uservalue1=0;

                        // Set Caps
                        memset(&DSBDescription,0,sizeof(DSBUFFERDESC));
                        DSBDescription.dwSize=sizeof(DSBUFFERDESC);
                        DSBDescription.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_GLOBALFOCUS|
                                               DSBCAPS_GETCURRENTPOSITION2|context->Device->CreationFlag;
                        DSBDescription.dwBufferBytes=88200;
                        DSBDescription.lpwfxFormat=&OutputType;
                        memset(&OutputType,0,sizeof(WAVEFORMATEX));
                        OutputType.wFormatTag=WAVE_FORMAT_PCM;
                        OutputType.nChannels=2;
                        OutputType.wBitsPerSample=16;
                        OutputType.nBlockAlign=4;
                        OutputType.nSamplesPerSec=44100;
                        OutputType.nAvgBytesPerSec=176400;
                        OutputType.cbSize=0;
                        if(IDirectSound_CreateSoundBuffer(context->Device->DShandle,&DSBDescription,&(LPDIRECTSOUNDBUFFER)source->uservalue1,0)==DS_OK)
                        {
                            IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1,0);
                        }

                        // Set correct volume for new DS Buffer
                        if(source->uservalue1)
                        {
                            Gain = source->param[AL_GAIN-AL_CONE_INNER_ANGLE].f;
                            Gain = (Gain * context->Listener.Gain);
                            volume = LinearGainToMB(Gain);
                            IDirectSoundBuffer_SetVolume((LPDIRECTSOUNDBUFFER)source->uservalue1, volume);
                        }

                        source->SourceType = SOURCE2D;
                    }
                    else if((Channels == 1) && (source->SourceType == SOURCE2D))
                    {
                        // Playing a (3D) Mono buffer

                        // Need to destroy the stereo streaming buffer and create a 3D mono one instead
                        if(source->uservalue1)
                        {
                            IDirectSoundBuffer_Release((LPDIRECTSOUNDBUFFER)source->uservalue1);
                            source->uservalue1=0;
                        }

                        // Set Caps
                        memset(&DSBDescription,0,sizeof(DSBUFFERDESC));
                        DSBDescription.dwSize=sizeof(DSBUFFERDESC);
                        DSBDescription.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRL3D|DSBCAPS_GLOBALFOCUS|
                                               DSBCAPS_GETCURRENTPOSITION2|context->Device->CreationFlag;
                        DSBDescription.dwBufferBytes=88200;
                        DSBDescription.lpwfxFormat=&OutputType;
                        memset(&OutputType,0,sizeof(WAVEFORMATEX));
                        OutputType.wFormatTag=WAVE_FORMAT_PCM;
                        OutputType.nChannels=1;
                        OutputType.wBitsPerSample=16;
                        OutputType.nBlockAlign=2;
                        OutputType.nSamplesPerSec=44100;
                        OutputType.nAvgBytesPerSec=88200;
                        OutputType.cbSize=0;

                        if(IDirectSound_CreateSoundBuffer(context->Device->DShandle,&DSBDescription,&(LPDIRECTSOUNDBUFFER)source->uservalue1,0)==DS_OK)
                        {
                            IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1,0);

                            // Get 3D Interface
                            if(IDirectSoundBuffer_QueryInterface((LPDIRECTSOUNDBUFFER)source->uservalue1,&IID_IDirectSound3DBuffer,(LPUNKNOWN*)&source->uservalue2)==DS_OK)
                            {
                                // Get Property Set Interface
                                IDirectSound3DBuffer_QueryInterface((LPDIRECTSOUND3DBUFFER)source->uservalue2,&IID_IKsPropertySet,(LPUNKNOWN*)&source->uservalue3);
                            }
                        }


                        // Set correct volume for new DS Buffer
                        if(source->uservalue1)
                        {
                            Gain = source->param[AL_GAIN-AL_CONE_INNER_ANGLE].f;
                            Gain = (Gain * context->Listener.Gain);
                            volume = LinearGainToMB(Gain);
                            IDirectSoundBuffer_SetVolume((LPDIRECTSOUNDBUFFER)source->uservalue1, volume);
                        }

                        source->SourceType = SOURCE3D;
                    }

                    // Set Direct Sound buffer to frequency of current Open AL buffer multiplied by desired Pitch
                    Freq = ((ALbuffer*)BufferID)->frequency;
                    Pitch = source->param[AL_PITCH-AL_CONE_INNER_ANGLE].f;

                    IDirectSoundBuffer_SetFrequency((LPDIRECTSOUNDBUFFER)source->uservalue1, (long)(Freq*Pitch));

                    // Lock the whole DS buffer
                    IDirectSoundBuffer_Lock((LPDIRECTSOUNDBUFFER)source->uservalue1,0,0,&lpPart1,&Part1Size,0,0,DSBLOCK_ENTIREBUFFER);

                    BytesWritten = 0;

                    while(AL_TRUE)
                    {
                        // Copy audio data from Open AL Buffer into DS buffer

                        // Find out how much data is left in current Open AL Buffer
                        Data = (ALint)(((ALbuffer*)BufferID)->data);
                        DataSize = ((ALbuffer*)BufferID)->size;

                        if(DataSize == 0)
                            DataLeft = 0;
                        else
                            DataLeft = DataSize - source->BufferPosition;

                        if(DataLeft > (88200 - BytesWritten))
                        {
                            // Copy (88200 - BytesWritten) bytes to Direct Sound buffer
                            memcpy((ALubyte*)lpPart1 + BytesWritten, (ALubyte*)Data + source->BufferPosition, 88200 - BytesWritten);
                            source->FinishedQueue = AL_FALSE;     // More data to follow ...
                            source->BufferPosition += (88200 - BytesWritten);             // Record position in buffer data
                            BytesWritten += (88200 - BytesWritten);
                            break;
                        }
                        else
                        {
                            // Not enough data in buffer to fill DS buffer so just copy as much data as possible
                            memcpy((ALubyte*)lpPart1 + BytesWritten, (ALubyte*)Data + source->BufferPosition, DataLeft);

                            BytesWritten += DataLeft;

                            source->BuffersAddedToDSBuffer++;

                            source->BufferPosition = 0;

                            // Get next valid buffer ID
                            ALBufferListItem = ALBufferListItem->next;

                            if(ALBufferListItem == 0)
                            {
                                // No more buffers - check for looping flag
                                if(source->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i == AL_TRUE)
                                {
                                    // Looping
                                    ALBufferListItem = source->queue;
                                }
                                else
                                {
                                    // Not looping and no more buffers
                                    break;
                                }
                            }

                            BufferID = ALBufferListItem->buffer;
                        }
                    }

                    if(BytesWritten < 88200)
                    {
                        // Fill the rest of the buffer with silence
                        memset((ALubyte*)lpPart1 + BytesWritten, 0, 88200 - BytesWritten);
                        source->SilenceAdded = 88200 - BytesWritten;
                        source->FinishedQueue = AL_TRUE;              // Set this to true to indicate no more data needs to be copied into buffer
                    }

                    IDirectSoundBuffer_Unlock((LPDIRECTSOUNDBUFFER)source->uservalue1,lpPart1,Part1Size,0,0);
                }
                break;

            case AL_PAUSED:
                if(source->uservalue1)
                {
                    IDirectSoundBuffer_Stop((LPDIRECTSOUNDBUFFER)source->uservalue1);
                    source->paused = AL_TRUE;
                }
                break;
            case AL_STOPPED:
                if(source->uservalue1)
                {
                    IDirectSoundBuffer_Stop((LPDIRECTSOUNDBUFFER)source->uservalue1);
                    IDirectSoundBuffer_SetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1,0);
                    // Re-set variables
                    source->BufferPosition = 0;
                    source->BytesPlayed = 0;
                    source->DataStillToPlay = 0;
                    source->FinishedQueue = AL_FALSE;
                    source->OldPlayCursor = 0;
                    source->OldWriteCursor = 0;
                    source->SilenceAdded = 0;
                }

                // Mark all buffers in queue as PROCESSED
                source->BuffersProcessed = source->BuffersInQueue;

                ALBufferListItem= source->queue;
                while(ALBufferListItem != 0)
                {
                    ALBufferListItem->bufferstate = PROCESSED;
                    ALBufferListItem = ALBufferListItem->next;
                }

                break;
        }

        // End of STATE update
        source->update1 &= ~STATE;
        if(source->update1 == 0)
            return;
    }


    // Check if we need to update the 3D Position of the Source
    if(source->update1 & POSITION)
    {
        if(source->uservalue2)
        {
            Pos[0] = source->param[AL_POSITION-AL_CONE_INNER_ANGLE].fv3[0];
            Pos[1] = source->param[AL_POSITION-AL_CONE_INNER_ANGLE].fv3[1];
            Pos[2] = -source->param[AL_POSITION-AL_CONE_INNER_ANGLE].fv3[2];
            IDirectSound3DBuffer_SetPosition((LPDIRECTSOUND3DBUFFER)source->uservalue2,Pos[0],Pos[1],Pos[2],DS3D_IMMEDIATE);
            source->update1 &= ~POSITION;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we need to adjust the velocity of the Source
    if(source->update1 & VELOCITY)
    {
        if(source->uservalue2)
        {
            Vel[0] = source->param[AL_VELOCITY-AL_CONE_INNER_ANGLE].fv3[0];
            Vel[1] = source->param[AL_VELOCITY-AL_CONE_INNER_ANGLE].fv3[1];
            Vel[2] = -source->param[AL_VELOCITY-AL_CONE_INNER_ANGLE].fv3[2];
            IDirectSound3DBuffer_SetVelocity((LPDIRECTSOUND3DBUFFER)source->uservalue2,Vel[0],Vel[1],Vel[2],DS3D_IMMEDIATE);
            source->update1 &= ~VELOCITY;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we need to adjust the Orientation of the Source
    if(source->update1 & ORIENTATION)
    {
        if(source->uservalue2)
        {
            Dir[0] = source->param[AL_DIRECTION-AL_CONE_INNER_ANGLE].fv3[0];
            Dir[1] = source->param[AL_DIRECTION-AL_CONE_INNER_ANGLE].fv3[1];
            Dir[2] = -source->param[AL_DIRECTION-AL_CONE_INNER_ANGLE].fv3[2];
            IDirectSound3DBuffer_SetConeOrientation((LPDIRECTSOUND3DBUFFER)source->uservalue2,Dir[0],Dir[1],Dir[2],DS3D_IMMEDIATE);
            source->update1 &= ~ORIENTATION;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if any Buffers have been added to this Source's queue
    if(source->update1 & SQUEUE)
    {
        if((source->uservalue1) && (source->state == AL_PLAYING))
        {
            // Some buffer(s) have been added to the queue

            // If silence has been added, then we need to overwrite the silence with new audio
            // data from the buffers in the queue
            if(source->SilenceAdded > 0)
            {
                // Find position of end of valid data
                if(source->OldWriteCursor < source->SilenceAdded)
                    source->OldWriteCursor += 88200;

                // Set new Write position to the end of the valid data
                source->OldWriteCursor -= source->SilenceAdded;

                // Read position from next buffer should be set to 0
                source->BufferPosition = 0;

                // We have overwritten the silent data, so reset variable
                source->SilenceAdded = 0;

                // Make sure that the we haven't finished processing the queue !
                source->FinishedQueue = AL_FALSE;
            }
            // Update DataStillToPlay
            source->DataStillToPlay += source->SizeOfBufferDataAddedToQueue;
        }
        source->TotalBufferDataSize += source->SizeOfBufferDataAddedToQueue;
        source->SizeOfBufferDataAddedToQueue = 0;
        source->NumBuffersAddedToQueue = 0;
        source->update1 &= ~SQUEUE;
        if(source->update1 == 0)
            return;
    }

    // Check if any Buffers have been removed from this Source's Queue
    if(source->update1 & SUNQUEUE)
    {
        // Some number of buffers have been removed from the queue

        // We need to update some variables to correctly reflect the new queue

        // The number of BuffersAddedToDSBuffers must be decreased by the number of buffers
        // removed from the queue (or else the Timer function will think we are further through
        // the list than we are)

        // The amount of DataPlayed must be decreased by the total size of the data in the buffers
        // removed from the queue (or the amount of data still to play (TotalDataSize - DataPlayed)
        // will be incorrect)
        if((source->uservalue1) && (source->state == AL_PLAYING))
        {
            source->BytesPlayed -= source->SizeOfBufferDataRemovedFromQueue;
        }
        source->TotalBufferDataSize -= source->SizeOfBufferDataRemovedFromQueue;
        source->NumBuffersRemovedFromQueue = 0;
        source->SizeOfBufferDataRemovedFromQueue = 0;

        // If we're not playing then reset current buffer (it may have changed)
        if(source->state != AL_PLAYING)
        {
            if(source->queue)
                BufferID = source->queue->buffer;
            else
                BufferID = 0;

            source->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = BufferID;
        }

        source->update1 &= ~SUNQUEUE;
        if(source->update1 == 0)
            return;
    }

    // Check if we need to adjust the volume of the Source
    if(source->update1 & VOLUME)
    {
        if(source->uservalue1)
        {
            Gain = source->param[AL_GAIN-AL_CONE_INNER_ANGLE].f;
            Gain = (Gain * context->Listener.Gain);
            volume = LinearGainToMB(Gain);
            IDirectSoundBuffer_SetVolume((LPDIRECTSOUNDBUFFER)source->uservalue1, volume);
            source->update1 &= ~VOLUME;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we need to adjust the frequency of the Source
    if(source->update1 & FREQUENCY)
    {
        if(source->uservalue1)
        {
            BufferID = source->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i;
            if(BufferID == 0)
                Freq = 44100;
            else
                Freq = ((ALbuffer*)BufferID)->frequency;

            Pitch = source->param[AL_PITCH-AL_CONE_INNER_ANGLE].f;
            IDirectSoundBuffer_SetFrequency((LPDIRECTSOUNDBUFFER)source->uservalue1,(long)(Freq*Pitch));
            source->update1 &= ~FREQUENCY;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we need to adjust the Min Distance of the Source
    if(source->update1 & MINDIST)
    {
        if(source->uservalue2)
        {
            minDist = source->param[AL_REFERENCE_DISTANCE-AL_CONE_INNER_ANGLE].f;
            IDirectSound3DBuffer_SetMinDistance((LPDIRECTSOUND3DBUFFER)source->uservalue2,minDist,DS3D_IMMEDIATE);
            source->update1 &= ~MINDIST;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we need to adjust the Max Distance of the Source
    if(source->update1 & MAXDIST)
    {
        if(source->uservalue2)
        {
            maxDist = source->param[AL_MAX_DISTANCE-AL_CONE_INNER_ANGLE].f;
            IDirectSound3DBuffer_SetMaxDistance((LPDIRECTSOUND3DBUFFER)source->uservalue2,maxDist,DS3D_IMMEDIATE);
            source->update1 &= ~MAXDIST;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we need to adjust the Cone Outside Volume of the Source
    if(source->update1 & CONEOUTSIDEVOLUME)
    {
        if(source->uservalue2)
        {
            outerGain = source->param[AL_CONE_OUTER_GAIN-AL_CONE_INNER_ANGLE].f;
            volume = LinearGainToMB(outerGain);

            IDirectSound3DBuffer_SetConeOutsideVolume((LPDIRECTSOUND3DBUFFER)source->uservalue2, volume,DS3D_IMMEDIATE);

            source->update1 &= ~CONEOUTSIDEVOLUME;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we need to update the 3D Processing Mode (Head Relative)
    if(source->update1 & MODE)
    {
        if(source->uservalue2)
        {
            Relative = source->relative ? DS3DMODE_HEADRELATIVE : DS3DMODE_NORMAL;
            IDirectSound3DBuffer_SetMode((LPDIRECTSOUND3DBUFFER)source->uservalue2,Relative,DS3D_IMMEDIATE);
            source->update1 &= ~MODE;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if we ne need to update the Cone Angles for the Source
    if(source->update1 & CONEANGLES)
    {
        if(source->uservalue2)
        {
            innerAngle = (ALuint)source->param[AL_CONE_INNER_ANGLE-AL_CONE_INNER_ANGLE].f;
            outerAngle = (ALuint)source->param[AL_CONE_OUTER_ANGLE-AL_CONE_INNER_ANGLE].f;

            IDirectSound3DBuffer_SetConeAngles((LPDIRECTSOUND3DBUFFER)source->uservalue2,innerAngle,outerAngle,DS3D_IMMEDIATE);
            source->update1 &= ~CONEANGLES;
            if(source->update1 == 0)
                return;
        }
    }


    // Check if Looping has been enabled / disabled
    if(source->update1 & LOOPED)
    {
        // Only has an effect if the Source is playing
        if((source->uservalue1) && (source->state == AL_PLAYING))
        {
            // Find out whether Looping has been enabled or disabled
            Loop = source->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i;

            if(Loop == AL_TRUE)
            {
                // Looping enabled !

                // All buffers in queue will be needed again, so their state needs to be upgraded
                // to PENDING, and the number of buffers processed set to 0
                source->BuffersProcessed = 0;

                // While calculating the total size of the data (by summing the datasize of each
                // buffer in the queue), set all Buffer states to PENDING
                ALBufferListTemp = source->queue;
                source->DataStillToPlay = 0;

                while(ALBufferListTemp != 0)
                {
                    DataSize = ((ALbuffer*)ALBufferListTemp->buffer)->size;
                    source->DataStillToPlay += DataSize;
                    ALBufferListTemp->bufferstate = PENDING;
                    ALBufferListTemp = ALBufferListTemp->next;
                }

                // If we have added silence after the valid data, then we need to set the new
                // write position back to the end of the valid data
                if(source->SilenceAdded > 0)
                {
                    if(source->OldWriteCursor < source->SilenceAdded)
                        source->OldWriteCursor += 88200;

                    source->OldWriteCursor -= source->SilenceAdded;

                    source->BufferPosition = 0;
                }

                source->FinishedQueue = AL_FALSE;
            }
            else
            {
                // Looping disabled !

                // We need to calculate how much data is still to be played
                IDirectSoundBuffer_GetCurrentPosition((LPDIRECTSOUNDBUFFER)source->uservalue1, &PlayCursor, &WriteCursor);

                // Calculate amount of data played
                if(source->OldPlayCursor > PlayCursor)
                    source->BytesPlayed += ((88200 - source->OldPlayCursor) + PlayCursor);
                else
                    source->BytesPlayed += (PlayCursor - source->OldPlayCursor);

                source->OldPlayCursor = PlayCursor;

                // Calculate how much data is left to play for the current iteration of the looping data

                TotalDataSize = 0;
                ALBufferListTemp = source->queue;

                while(ALBufferListTemp != 0)
                {
                    DataSize = ((ALbuffer*)ALBufferListTemp->buffer)->size;
                    TotalDataSize += DataSize;
                    ALBufferListTemp = ALBufferListTemp->next;
                }

                source->DataStillToPlay = TotalDataSize - (source->BytesPlayed % TotalDataSize);

                if(WriteCursor > PlayCursor)
                    DataCommitted = WriteCursor - PlayCursor;
                else
                    DataCommitted = (88200 - PlayCursor) + WriteCursor;

                if(DataCommitted > source->DataStillToPlay)
                {
                    // Data for the next iteration of the loop has already been committed
                    // Therefore increment DataStillToPlay by the total loop size
                    source->DataStillToPlay += TotalDataSize;
                }
                else
                {
                    DataPlayed = TotalDataSize - source->DataStillToPlay;
                    DataCount = 0;
                    source->BuffersProcessed = 0;

                    ALBufferListItem = source->queue;
                    while(ALBufferListItem != 0)
                    {
                        DataSize = ((ALbuffer*)ALBufferListItem->buffer)->size;
                        DataCount += DataSize;
                        if(DataCount < DataPlayed)
                        {
                            ALBufferListItem->bufferstate = PROCESSED;
                            ALBufferListItem = ALBufferListItem->next;
                            source->BuffersProcessed++;
                        }
                        else
                            ALBufferListItem = 0;
                    }
                }

                if(source->DataStillToPlay < 88200)
                {
                    // Need to move Write Cursor to end of valid data (so silence can be added
                    // after it)
                    source->OldWriteCursor = PlayCursor + source->DataStillToPlay;

                    if(source->OldWriteCursor >= 88200)
                        source->OldWriteCursor -= 88200;

                    source->FinishedQueue = AL_TRUE;
                }
            }
        }
        source->update1 &= ~LOOPED;
        if(source->update1 == 0)
            return;
    }

    return;
}


void UpdateListener(ALcontext* context)
/*
        Update Open AL Listener Properties
*/
{
    ALfloat         Pos[3], Vel[3], Ori[6];
    ALfloat         flDistanceFactor;
    ALfloat         flRollOffFactor;
    ALsource*       alSource;
    ALfloat         Gain;
    ALuint          i;
    ALint           volume;

    if(context->Listener.update1 & LVOLUME)
    {
        // Setting the volume of the Primary buffer has the effect of setting the volume
        // of the Wave / Direct Sound Mixer Line, so we can't do that

        // Instead we adjust the Gain of every Source

        alSource = context->Source;

        for(i = 0; i < context->SourceCount; i++)
        {
            if(alSource->uservalue1)
            {
                // Get current gain for source
                Gain = alSource->param[AL_GAIN-AL_CONE_INNER_ANGLE].f;
                Gain = (Gain * context->Listener.Gain);
                volume = LinearGainToMB(Gain);
                IDirectSoundBuffer_SetVolume((LPDIRECTSOUNDBUFFER)alSource->uservalue1, volume);
            }
            alSource = alSource->next;
        }

        context->Listener.update1 &= ~LVOLUME;
        if(context->Listener.update1 == 0)
            return;
    }

    if(context->Listener.update1 & LPOSITION)
    {
        Pos[0] = context->Listener.Position[0];
        Pos[1] = context->Listener.Position[1];
        Pos[2] = -context->Listener.Position[2];
        IDirectSound3DListener_SetPosition(context->Device->DS3dlistener, Pos[0], Pos[1], Pos[2],DS3D_IMMEDIATE);
        context->Listener.update1 &= ~LPOSITION;
        if(context->Listener.update1 == 0)
            return;
    }

    if(context->Listener.update1 & LVELOCITY)
    {
        Vel[0] = context->Listener.Velocity[0];
        Vel[1] = context->Listener.Velocity[1];
        Vel[2] = -context->Listener.Velocity[2];
        IDirectSound3DListener_SetVelocity(context->Device->DS3dlistener, Vel[0], Vel[1], Vel[2],DS3D_IMMEDIATE);
        context->Listener.update1 &= ~LVELOCITY;
        if(context->Listener.update1 == 0)
            return;
    }

    if(context->Listener.update1 & LORIENTATION)
    {
        Ori[0] = context->Listener.Forward[0];
        Ori[1] = context->Listener.Forward[1];
        Ori[2] = -context->Listener.Forward[2];
        Ori[3] = context->Listener.Up[0];
        Ori[4] = context->Listener.Up[1];
        Ori[5] = -context->Listener.Up[2];
        IDirectSound3DListener_SetOrientation(context->Device->DS3dlistener, Ori[0], Ori[1], Ori[2], Ori[3], Ori[4], Ori[5], DS3D_IMMEDIATE);
        context->Listener.update1 &= ~LORIENTATION;
        if(context->Listener.update1 == 0)
            return;
    }

    if(context->Listener.update1 & LDOPPLERFACTOR)
    {
        IDirectSound3DListener_SetDopplerFactor(context->Device->DS3dlistener,context->DopplerFactor,DS3D_IMMEDIATE);
        context->Listener.update1 &= ~LDOPPLERFACTOR;
        if(context->Listener.update1 == 0)
            return;
    }

    if(context->Listener.update1 & LDOPPLERVELOCITY)
    {
        // Doppler Velocity is used to set the speed of sound in units per second
        // DS3D uses Distance Factor to relate units to real world coordinates (metres)
        // Therefore need to convert Doppler Velocity into DS3D Distance Factor
        flDistanceFactor = SPEEDOFSOUNDMETRESPERSEC / context->DopplerVelocity;
        IDirectSound3DListener_SetDistanceFactor(context->Device->DS3dlistener, flDistanceFactor, DS3D_IMMEDIATE);
        context->Listener.update1 &= ~LDOPPLERVELOCITY;
        if(context->Listener.update1 == 0)
            return;
    }

    if(context->Listener.update1 & LDISTANCEMODEL)
    {
        if(context->DistanceModel == AL_NONE)
            flRollOffFactor = 0.0f;
        else
            flRollOffFactor = 1.0f;

        IDirectSound3DListener_SetRolloffFactor (context->Device->DS3dlistener, flRollOffFactor, DS3D_IMMEDIATE);
        context->Listener.update1 &= ~LDISTANCEMODEL;
        if(context->Listener.update1 == 0)
            return;
    }

    return;
}


ALboolean ALC_CALLBACK EnumerateDevicesCallback(const ALubyte* deviceName, ALvoid* context)
{
    if(strcmp((char*)context, deviceName)==0)
    {
        return AL_TRUE;
    }

    return AL_FALSE;
}


ALboolean FillOut3rdPartyFunctionTable(ALdevice* device)
{
    ALboolean  alFxns = FALSE;
    ALboolean alcFxns = FALSE;
    ALCAPI_FXN_TABLE* alcApi = &device->AlcApi;
    ALAPI_FXN_TABLE*   alApi = &device->AlApi;

    //
    // Get the OpenAL 1.0 Entry points.
    //

    // AL:
    alApi->alEnable               = (ALAPI_ENABLE)GetProcAddress(device->Dll, "alEnable");
    alApi->alDisable              = (ALAPI_DISABLE)GetProcAddress(device->Dll, "alDisable");
    alApi->alIsEnabled            = (ALAPI_IS_ENABLED)GetProcAddress(device->Dll, "alIsEnabled");

    alApi->alGetBoolean           = (ALAPI_GET_BOOLEAN)GetProcAddress(device->Dll, "alGetBoolean");
    alApi->alGetInteger           = (ALAPI_GET_INTEGER)GetProcAddress(device->Dll, "alGetInteger");
    alApi->alGetFloat             = (ALAPI_GET_FLOAT)GetProcAddress(device->Dll, "alGetFloat");
    alApi->alGetDouble            = (ALAPI_GET_DOUBLE)GetProcAddress(device->Dll, "alGetDouble");
    alApi->alGetBooleanv          = (ALAPI_GET_BOOLEANV)GetProcAddress(device->Dll, "alGetBooleanv");
    alApi->alGetIntegerv          = (ALAPI_GET_INTEGERV)GetProcAddress(device->Dll, "alGetIntegerv");
    alApi->alGetFloatv            = (ALAPI_GET_FLOATV)GetProcAddress(device->Dll, "alGetFloatv");
    alApi->alGetDoublev           = (ALAPI_GET_DOUBLEV)GetProcAddress(device->Dll, "alGetDoublev");
    alApi->alGetString            = (ALAPI_GET_STRING)GetProcAddress(device->Dll, "alGetString");

    alApi->alGetError             = (ALAPI_GET_ERROR)GetProcAddress(device->Dll, "alGetError");

    alApi->alIsExtensionPresent   = (ALAPI_IS_EXTENSION_PRESENT)GetProcAddress(device->Dll, "alIsExtensionPresent");
    alApi->alGetProcAddress       = (ALAPI_GET_PROC_ADDRESS)GetProcAddress(device->Dll, "alGetProcAddress");
    alApi->alGetEnumValue         = (ALAPI_GET_ENUM_VALUE)GetProcAddress(device->Dll, "alGetEnumValue");

    alApi->alListeneri            = (ALAPI_LISTENERI)GetProcAddress(device->Dll, "alListeneri");
    alApi->alListenerf            = (ALAPI_LISTENERF)GetProcAddress(device->Dll, "alListenerf");
    alApi->alListener3f           = (ALAPI_LISTENER3F)GetProcAddress(device->Dll, "alListener3f");
    alApi->alListenerfv           = (ALAPI_LISTENERFV)GetProcAddress(device->Dll, "alListenerfv");
    alApi->alGetListeneri         = (ALAPI_GET_LISTENERI)GetProcAddress(device->Dll, "alGetListeneri");
    alApi->alGetListenerf         = (ALAPI_GET_LISTENERF)GetProcAddress(device->Dll, "alGetListenerf");
    alApi->alGetListener3f        = (ALAPI_GET_LISTENER3F)GetProcAddress(device->Dll, "alGetListener3f");
    alApi->alGetListenerfv        = (ALAPI_GET_LISTENERFV)GetProcAddress(device->Dll, "alGetListenerfv");

    alApi->alGenSources           = (ALAPI_GEN_SOURCES)GetProcAddress(device->Dll, "alGenSources");
    alApi->alDeleteSources        = (ALAPI_DELETE_SOURCES)GetProcAddress(device->Dll, "alDeleteSources");
    alApi->alIsSource             = (ALAPI_IS_SOURCE)GetProcAddress(device->Dll, "alIsSource");
    alApi->alSourcei              = (ALAPI_SOURCEI)GetProcAddress(device->Dll, "alSourcei");
    alApi->alSourcef              = (ALAPI_SOURCEF)GetProcAddress(device->Dll, "alSourcef");
    alApi->alSource3f             = (ALAPI_SOURCE3F)GetProcAddress(device->Dll, "alSource3f");
    alApi->alSourcefv             = (ALAPI_SOURCEFV)GetProcAddress(device->Dll, "alSourcefv");
    alApi->alGetSourcei           = (ALAPI_GET_SOURCEI)GetProcAddress(device->Dll, "alGetSourcei");
    alApi->alGetSourcef           = (ALAPI_GET_SOURCEF)GetProcAddress(device->Dll, "alGetSourcef");
    alApi->alGetSource3f          = (ALAPI_GET_SOURCE3F)GetProcAddress(device->Dll, "alGetSource3f");
    alApi->alGetSourcefv          = (ALAPI_GET_SOURCEFV)GetProcAddress(device->Dll, "alGetSourcefv");
    alApi->alSourcePlayv          = (ALAPI_SOURCE_PLAYV)GetProcAddress(device->Dll, "alSourcePlayv");
    alApi->alSourcePausev         = (ALAPI_SOURCE_PAUSEV)GetProcAddress(device->Dll, "alSourcePausev");
    alApi->alSourceStopv          = (ALAPI_SOURCE_STOPV)GetProcAddress(device->Dll, "alSourceStopv");
    alApi->alSourceRewindv        = (ALAPI_SOURCE_REWINDV)GetProcAddress(device->Dll, "alSourceRewindv");
    alApi->alSourcePlay           = (ALAPI_SOURCE_PLAY)GetProcAddress(device->Dll, "alSourcePlay");
    alApi->alSourcePause          = (ALAPI_SOURCE_PAUSE)GetProcAddress(device->Dll, "alSourcePause");
    alApi->alSourceStop           = (ALAPI_SOURCE_STOP)GetProcAddress(device->Dll, "alSourceStop");
    alApi->alSourceRewind         = (ALAPI_SOURCE_STOP)GetProcAddress(device->Dll, "alSourceRewind");

    alApi->alGenBuffers           = (ALAPI_GEN_BUFFERS)GetProcAddress(device->Dll, "alGenBuffers");
    alApi->alDeleteBuffers        = (ALAPI_DELETE_BUFFERS)GetProcAddress(device->Dll, "alDeleteBuffers");
    alApi->alIsBuffer             = (ALAPI_IS_BUFFER)GetProcAddress(device->Dll, "alIsBuffer");
    alApi->alBufferData           = (ALAPI_BUFFER_DATA)GetProcAddress(device->Dll, "alBufferData");
    alApi->alGetBufferi           = (ALAPI_GET_BUFFERI)GetProcAddress(device->Dll, "alGetBufferi");
    alApi->alGetBufferf           = (ALAPI_GET_BUFFERF)GetProcAddress(device->Dll, "alGetBufferf");

    alApi->alSourceQueueBuffers   = (ALAPI_SOURCE_QUEUE_BUFFERS)GetProcAddress(device->Dll, "alSourceQueueBuffers");
    alApi->alSourceUnqueueBuffers = (ALAPI_SOURCE_UNQUEUE_BUFFERS)GetProcAddress(device->Dll, "alSourceUnqueueBuffers");

    alApi->alDistanceModel        = (ALAPI_DISTANCE_MODEL)GetProcAddress(device->Dll, "alDistanceModel");
    alApi->alDopplerFactor        = (ALAPI_DOPPLER_FACTOR)GetProcAddress(device->Dll, "alDopplerFactor");
    alApi->alDopplerVelocity      = (ALAPI_DOPPLER_VELOCITY)GetProcAddress(device->Dll, "alDopplerVelocity");


    alFxns = (alApi->alEnable               &&
              alApi->alDisable              &&
              alApi->alIsEnabled            &&

              alApi->alGetBoolean           &&
              alApi->alGetInteger           &&
              alApi->alGetFloat             &&
              alApi->alGetDouble            &&
              alApi->alGetBooleanv          &&
              alApi->alGetIntegerv          &&
              alApi->alGetFloatv            &&
              alApi->alGetDoublev           &&
              alApi->alGetString            &&

              alApi->alGetError             &&

              alApi->alIsExtensionPresent   &&
              alApi->alGetProcAddress       &&
              alApi->alGetEnumValue         &&

              alApi->alListeneri            &&
              alApi->alListenerf            &&
              alApi->alListener3f           &&
              alApi->alListenerfv           &&
              alApi->alGetListeneri         &&
              alApi->alGetListenerf         &&
              alApi->alGetListener3f        &&
              alApi->alGetListenerfv        &&

              alApi->alGenSources           &&
              alApi->alDeleteSources        &&
              alApi->alIsSource             &&
              alApi->alSourcei              &&
              alApi->alSourcef              &&
              alApi->alSource3f             &&
              alApi->alSourcefv             &&
              alApi->alGetSourcei           &&
              alApi->alGetSourcef           &&
              alApi->alGetSource3f          &&
              alApi->alGetSourcefv          &&
              alApi->alSourcePlayv          &&
              alApi->alSourcePausev         &&
              alApi->alSourceStopv          &&
              alApi->alSourceRewindv        &&
              alApi->alSourcePlay           &&
              alApi->alSourcePause          &&
              alApi->alSourceStop           &&
              alApi->alSourceRewind         &&

              alApi->alGenBuffers           &&
              alApi->alDeleteBuffers        &&
              alApi->alIsBuffer             &&
              alApi->alBufferData           &&
              alApi->alGetBufferi           &&
              alApi->alGetBufferf           &&

              alApi->alSourceQueueBuffers   &&
              alApi->alSourceUnqueueBuffers &&

              alApi->alDistanceModel        &&
              alApi->alDopplerFactor        &&
              alApi->alDopplerVelocity);



    // ALC:
    alcApi->alcGetString          = (ALCAPI_GET_STRING)GetProcAddress(device->Dll, "alcGetString");
    alcApi->alcGetIntegerv        = (ALCAPI_GET_INTEGERV)GetProcAddress(device->Dll, "alcGetIntegerv");

    alcApi->alcOpenDevice         = (ALCAPI_OPEN_DEVICE)GetProcAddress(device->Dll, "alcOpenDevice");
    alcApi->alcCloseDevice        = (ALCAPI_CLOSE_DEVICE)GetProcAddress(device->Dll, "alcCloseDevice");
    alcApi->alcEnumerateDevices   = (ALCAPI_ENUMERATE_DEVICES)GetProcAddress(device->Dll, "alcEnumerateDevices");

    alcApi->alcCreateContext      = (ALCAPI_CREATE_CONTEXT)GetProcAddress(device->Dll, "alcCreateContext");
    alcApi->alcMakeContextCurrent = (ALCAPI_MAKE_CONTEXT_CURRENT)GetProcAddress(device->Dll, "alcMakeContextCurrent");
    alcApi->alcProcessContext     = (ALCAPI_PROCESS_CONTEXT)GetProcAddress(device->Dll, "alcProcessContext");
    alcApi->alcGetCurrentContext  = (ALCAPI_GET_CURRENT_CONTEXT)GetProcAddress(device->Dll, "alcGetCurrentContext");
    alcApi->alcGetContextsDevice  = (ALCAPI_GET_CONTEXTS_DEVICE)GetProcAddress(device->Dll, "alcGetContextsDevice");
    alcApi->alcSuspendContext     = (ALCAPI_SUSPEND_CONTEXT)GetProcAddress(device->Dll, "alcSuspendContext");
    alcApi->alcDestroyContext     = (ALCAPI_DESTROY_CONTEXT)GetProcAddress(device->Dll, "alcDestroyContext");

    alcApi->alcGetError           = (ALCAPI_GET_ERROR)GetProcAddress(device->Dll, "alcGetError");

    alcApi->alcIsExtensionPresent = (ALCAPI_IS_EXTENSION_PRESENT)GetProcAddress(device->Dll, "alcIsExtensionPresent");
    alcApi->alcGetProcAddress     = (ALCAPI_GET_PROC_ADDRESS)GetProcAddress(device->Dll, "alcGetProcAddress");
    alcApi->alcGetEnumValue       = (ALCAPI_GET_ENUM_VALUE)GetProcAddress(device->Dll, "alcGetEnumValue");


    alcFxns = (alcApi->alcGetString          &&
               alcApi->alcGetIntegerv        &&
               alcApi->alcOpenDevice         &&
               alcApi->alcCloseDevice        &&
               alcApi->alcEnumerateDevices   &&

               alcApi->alcCreateContext      &&
               alcApi->alcMakeContextCurrent &&
               alcApi->alcProcessContext     &&
               alcApi->alcGetCurrentContext  &&
               alcApi->alcGetContextsDevice  &&
               alcApi->alcSuspendContext     &&
               alcApi->alcDestroyContext     &&

               alcApi->alcGetError           &&

               alcApi->alcIsExtensionPresent &&
               alcApi->alcGetProcAddress     &&
               alcApi->alcGetEnumValue);


    return alcFxns && alFxns;
}



//*****************************************************************************
//*****************************************************************************
//
// ALC API Entry Points
//
//*****************************************************************************
//*****************************************************************************

ALCAPI ALvoid ALCAPIENTRY alcSuspendContext(ALcontext* context)
{
    alListAcquireLock(alcContextList);
    if(!context && !alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    if(!context)
    {
        context = alcCurrentContext;
    }

    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    context->Suspended = TRUE;

    if(context->DllContext)
    {
        context->Device->AlcApi.alcSuspendContext(context->DllContext);
    }

    LeaveCriticalSection(&context->Lock);
    return;
}


ALCAPI ALvoid ALCAPIENTRY alcProcessContext(ALcontext* context)
{
    alListAcquireLock(alcContextList);
    if(!context && !alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    if(!context)
    {
        context = alcCurrentContext;
    }

    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    if(context->DllContext)
    {
        context->Device->AlcApi.alcProcessContext(context->DllContext);
    }

    context->Suspended = FALSE;

    LeaveCriticalSection(&context->Lock);
    return;
}


ALCAPI ALenum ALCAPIENTRY alcGetError(ALdevice* device)
{
    // Could they have stuck error codes in anymore places?  Why pass in a device?  Why?  Why???
    ALenum errorCode = AL_NO_ERROR;

    alListAcquireLock(alcContextList);

    // Try to get a valid device.
    if(!device && alcCurrentContext)
    {
        device = alcCurrentContext->Device;
    }

    // Check if its a 3rd party device.
    if(device && device->DllDevice)
    {
        //
        // If we want to keep checking for errors, then we must enumerate the 3rd party DLL
        // AL_NO_ERROR value.
        //
        errorCode = device->AlcApi.alcGetError(device->DllDevice);
        alListReleaseLock(alcContextList);
        return errorCode;
    }

    // Check the device for errors.
    if(device && device->LastError != AL_NO_ERROR)
    {
        errorCode = device->LastError;
        device->LastError = AL_NO_ERROR;
        if(alcCurrentContext && alcCurrentContext->Device == device)
        {
            alcCurrentContext->LastError = AL_NO_ERROR;
        }

        alListReleaseLock(alcContextList);
        return errorCode;
    }

    // Check the current context.
    if(alcCurrentContext)
    {
        errorCode = alcCurrentContext->LastError;
        alcCurrentContext->LastError = AL_NO_ERROR;
        alListReleaseLock(alcContextList);
        return errorCode;
    }

    // Guess we don't have a current context so how can we possibly find an error.
    alListReleaseLock(alcContextList);
    return AL_NO_ERROR;
}


ALCAPI ALubyte* ALCAPIENTRY alcGetString(ALdevice* device, ALenum param)
{
    ALubyte* value = 0;

    if(device && device->DllDevice)
    {
        return device->AlcApi.alcGetString(device->DllDevice, param);
    }

    switch(param)
    {
        case ALC_NO_ERROR:
            value = alcNoError;
            break;

        case ALC_INVALID_ENUM:
            value = alcErrInvalidEnum;
            break;

        case ALC_INVALID_VALUE:
            value = alcErrInvalidValue;
            break;

        case ALC_INVALID_DEVICE:
            value = alcErrInvalidDevice;
            break;

        case ALC_INVALID_CONTEXT:
            value = alcErrInvalidContext;
            break;

        case ALC_DEFAULT_DEVICE_SPECIFIER:
            value = alcDefaultDeviceSpecifier;
            break;

        case ALC_DEVICE_SPECIFIER:
            if(device)
                value = device->szDeviceName;
            break;

        case ALC_EXTENSIONS:
            value = alcExtensionList;
            break;

        default:
            if(device)
            {
                device->LastError = ALC_INVALID_ENUM;
            }
            break;
    }

    return value;
}


ALCAPI ALvoid ALCAPIENTRY alcGetIntegerv(ALdevice* device, ALenum param, ALsizei size, ALint* data)
{
    if(device && device->DllDevice)
    {
        device->AlcApi.alcGetIntegerv(device->DllDevice, param, size, data);
        return;
    }

    if((size == 0) || (!data))
        return;

    switch(param)
    {
        case ALC_MAJOR_VERSION:
            if(size < sizeof(ALint) && device)
                device->LastError = ALC_INVALID_VALUE;
            else
                *data = alcMajorVersion;
            break;

        case ALC_MINOR_VERSION:
            if(size < sizeof(ALint) && device)
                device->LastError = ALC_INVALID_VALUE;
            else
                *data = alcMinorVersion;
            break;

        case ALC_ATTRIBUTES_SIZE:
            if(size < sizeof(ALint) && device)
                device->LastError = ALC_INVALID_VALUE;
            else
                *data = strlen(alcExtensionList);
            break;

        case ALC_ALL_ATTRIBUTES:
            if(size < strlen(alcExtensionList) && device)
                device->LastError = ALC_INVALID_VALUE;
            else
                strcpy((char*)data, alcExtensionList);
            break;

        default:
            if(device)
            {
                device->LastError = ALC_INVALID_ENUM;
            }
            break;
    }
}


ALCAPI ALcontext* ALCAPIENTRY alcCreateContext(ALdevice* device, ALint* attrList)
{
    ALcontext* alcContext = 0;

    if(!device)
    {
        return 0;
    }


    // Allocate the initial context.
    alcContext = malloc(sizeof(ALcontext));
    if(!alcContext)
    {
        return 0;
    }

    memset(alcContext, 0, sizeof(ALcontext));
    alcContext->Device = device;
    alcContext->Suspended = FALSE;
    alcContext->LastError = AL_NO_ERROR;
    InitializeCriticalSection(&alcContext->Lock);

    if(!device->DllDevice)
    {
        alcInitContext(alcContext);
    }

    else
    {
        alcContext->DllContext = device->AlcApi.alcCreateContext(device->DllDevice, attrList);
        if(!alcContext->DllContext)
        {
            DeleteCriticalSection(&alcContext->Lock);
            free(alcContext);
            alcContext = 0;
            return 0;
        }
    }

    device->InUse++;

    // Add it to the context list.
    alListInitializeEntry(&alcContext->ListEntry, alcContext);
    alListAcquireLock(alcContextList);
    alListAddEntry(alcContextList, &alcContext->ListEntry);
    alListReleaseLock(alcContextList);
    return alcContext;
}


ALCAPI ALvoid ALCAPIENTRY alcDestroyContext(ALcontext* context)
{
    ALcontext* listData = 0;

    if(!context)
    {
        return;
    }

    // Remove the entry from the context list.
    alListAcquireLock(alcContextList);
    listData = (ALcontext*)alListRemoveEntry(alcContextList, &context->ListEntry);
    if(!listData)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    if(context == alcCurrentContext)
    {
        alcCurrentContext = 0;
    }

    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    context->Device->InUse--;

    // Clean up the context.
    if(context->DllContext)
    {
        context->Device->AlcApi.alcDestroyContext(context->DllContext);
    }

    else
    {
        alcExitContext(context);
    }

    LeaveCriticalSection(&context->Lock);
    DeleteCriticalSection(&context->Lock);
    free(context);
}


ALCAPI ALcontext* ALCAPIENTRY alcGetCurrentContext(ALvoid)
{
    return alcCurrentContext;
}


ALCAPI ALdevice* ALCAPIENTRY alcGetContextsDevice(ALcontext* context)
{
    ALdevice* alDevice = 0;

    alListAcquireLock(alcContextList);
    if(alListMatchData(alcContextList, context))
    {
        alDevice = context->Device;
    }

    alListReleaseLock(alcContextList);

    return alDevice;
}


ALCAPI ALboolean ALCAPIENTRY alcMakeContextCurrent(ALcontext* context)
{
    ALcontext* lockedContext;
    ALboolean ContextSwitched = TRUE;

    // context must be a valid Context or 0
    alListAcquireLock(alcContextList);
    if(!alListMatchData(alcContextList, context) && context != 0)
    {
        alListReleaseLock(alcContextList);
        return AL_FALSE;
    }

    // Notify a third party OpenAL implementation of a change in contexts.
    if(alcCurrentContext && alcCurrentContext->DllContext)
    {
        if(context)
        {
            ContextSwitched = alcCurrentContext->Device->AlcApi.alcMakeContextCurrent(context->DllContext);
        }

        else
        {
            ContextSwitched = alcCurrentContext->Device->AlcApi.alcMakeContextCurrent(0);
        }
    }

    // Try the new context.  If both are 0, then we really aren't changing the context are we.
    else if(context && context->DllContext)
    {
        ContextSwitched = context->Device->AlcApi.alcMakeContextCurrent(context->DllContext);
    }


    // Set the context states if the switch was successful.
    if(ContextSwitched)
    {
        alcCurrentContext = context;
    }

    alListReleaseLock(alcContextList);
    return ContextSwitched;
}


ALCAPI ALdevice* ALCAPIENTRY alcOpenDevice(ALubyte* deviceName)
{
    DSBUFFERDESC DSBDescription;
    WAVEFORMATEX OutputType;
    ALdevice* device = 0;
    ALint vmode = 1;
    ALint i;
    DSCAPS dsCaps;
    ALvoid* lpPart1, *lpPart2;
    ALuint dwSize1, dwSize2;
    ALboolean bUseDS   = AL_FALSE;
    ALboolean bUseDS3D = AL_FALSE;
    int error = 1;


    // Initialise the OpenAL device structure
    device=malloc(sizeof(ALdevice));
    if(!device)
    {
        return 0;
    }

    memset(device,0,sizeof(ALdevice));

    // Validate device
    device->LastError = AL_NO_ERROR;
    device->InUse = 0;

    // find the device.
    while(TRUE)
    {
        if(!deviceName || strcmp(deviceName,"DirectSound3D")==0)
        {
            bUseDS3D = AL_TRUE;
            break;
        }

        if(strcmp(deviceName, "DirectSound")==0)
        {
            bUseDS = AL_TRUE;
            break;
        }

        if(strcmp(deviceName, "MMSYSTEM")==0)
        {
            break;
        }

        // Begin the search for other DLLs.
        {
            WIN32_FIND_DATA findData;
            HANDLE searchHandle = INVALID_HANDLE_VALUE;
            TCHAR searchName[MAX_PATH + 1];
            TCHAR module[MAX_PATH + 1];
            TCHAR fileDrive[MAX_PATH + 1];
            TCHAR fileDir[MAX_PATH + 1];
            TCHAR fileName[MAX_PATH + 1];
            TCHAR fileExt[MAX_PATH + 1];

            TCHAR dir[3][MAX_PATH + 1];
            DWORD dirSize = 0;
            int i;
            ALboolean found = AL_FALSE;
            HINSTANCE dll = 0;
            ALCAPI_ENUMERATE_DEVICES alcEnumerateDevicesFxn = 0;
            ALCAPI_OPEN_DEVICE alcOpenDeviceFxn = 0;

            // Construct our search paths.  We will search the current directory, the app directory, and then
            // the system directory.
            dirSize = GetCurrentDirectory(MAX_PATH, dir[0]);
            _tcscat(dir[0], _T("\\"));

            GetModuleFileName(0, module, MAX_PATH);
            _splitpath(module, fileDrive, fileDir, fileName, fileExt);
            _tcscpy(dir[1], fileDrive);
            _tcscat(dir[1], fileDir);
            _tcscat(fileName, fileExt);

            dirSize = GetSystemDirectory(dir[2], MAX_PATH);
            _tcscat(dir[2], _T("\\"));

            // Get the original filename as we don't what to enumerate its devices.
            GetModuleFileName(GetModuleHandle("OpenAL32.dll"), module, MAX_PATH);

            // Begin searching for additional OpenAL implementations.  Start with the current directory, but check if
            // the current directory is the same as the application directory so we don't search it twice.
            for(i = _tcsicmp(dir[0], dir[1]) != 0 ? 0 : 1; i < 3 && !found; i++)
            {
                _tcscpy(searchName, dir[i]);
                _tcscat(searchName, _T("*OpenAL*.dll"));
                searchHandle = FindFirstFile(searchName, &findData);
                if(searchHandle != INVALID_HANDLE_VALUE)
                {
                    while(TRUE)
                    {
                        // Check if this is our module.
                        _tcscpy(searchName, dir[i]);
                        _tcscat(searchName, findData.cFileName);
                        if(_tcsicmp(searchName, module))
                        {
                            dll = LoadLibrary(searchName);
                            if(dll)
                            {
                                alcEnumerateDevicesFxn = (ALCAPI_ENUMERATE_DEVICES)GetProcAddress(dll, "alcEnumerateDevices");
                                if(alcEnumerateDevicesFxn)
                                {
                                    found = alcEnumerateDevicesFxn(EnumerateDevicesCallback, deviceName);
                                }

                                if(found)
                                {
                                    device->Dll = dll;
                                    break;
                                }

                                else
                                {
                                    FreeLibrary(dll);
                                }
                            }
                        }

                        if(!FindNextFile(searchHandle, &findData))
                        {
                            if(GetLastError() == ERROR_NO_MORE_FILES)
                                break;
                        }
                    }

                    FindClose(searchHandle);
                    searchHandle = INVALID_HANDLE_VALUE;
                }
            }

            if(found)
            {
                // Fill out the other functions
                if(FillOut3rdPartyFunctionTable(device))
                {
                    // Try to open the device
                    device->DllDevice = device->AlcApi.alcOpenDevice(deviceName);
                    if(device->DllDevice)
                    {
                        error = 0;
                    }
                }
            }

            if(error)
            {
                FreeLibrary(dll);
                free(device);
                device = 0;
            }

            return device;
        }
    }


    // Increment the number of devices using the multimedia timer.  We don't want to create this on a per context basis
    // so we can support multiple device contexts.
    if(!nDeviceUsingTimer)
    {
        TIMECAPS timeCaps;

        // Get the Timer Capabilities of the system
        timeGetDevCaps(&timeCaps, sizeof(TIMECAPS));

        // We want 100ms accuracy, if this is not possible, then just go with the best that we have
        if(timeCaps.wPeriodMin > nTimerInterval)
            nTimerInterval = timeCaps.wPeriodMin;

        // Begin Time Period
        timeBeginPeriod(nTimerInterval);

        // Start multimedia timer (if not already in progress)
        nTimerID = timeSetEvent(nTimerInterval, 0, &TimerCallback, 0, TIME_CALLBACK_FUNCTION | TIME_PERIODIC);
    }

    nDeviceUsingTimer++;


    // Set output format
    device->Frequency = 22050;
    device->Channels = 2;
    device->Format = AL_FORMAT_STEREO16;

    // Platform specific
    memset(&OutputType, 0, sizeof(WAVEFORMATEX));
    OutputType.wFormatTag=WAVE_FORMAT_PCM;
    assert(device->Channels < 0x10000); // Windows limits number of channels to a word size.
    OutputType.nChannels = (WORD)device->Channels;
    OutputType.wBitsPerSample = (device->Format == AL_FORMAT_MONO16 || (device->Format == AL_FORMAT_STEREO16) ? 16 : 8);
    OutputType.nBlockAlign = OutputType.nChannels * OutputType.wBitsPerSample / 8;
    OutputType.nSamplesPerSec = device->Frequency;
    OutputType.nAvgBytesPerSec = OutputType.nSamplesPerSec * OutputType.nBlockAlign;
    OutputType.cbSize = 0;


    //Initialise requested device
    if(bUseDS3D)
    {
        //Init COM
        CoInitialize(0);
        //DirectSound Init code
        if(CoCreateInstance(&CLSID_DirectSound,0,CLSCTX_INPROC_SERVER,&IID_IDirectSound,&(device->DShandle))==S_OK)
        {
            if(IDirectSound_Initialize(device->DShandle,0)==DS_OK)
            {
                if(IDirectSound_SetCooperativeLevel(device->DShandle,GetForegroundWindow(),DSSCL_PRIORITY)==DS_OK)
                {
                    memset(&DSBDescription,0,sizeof(DSBUFFERDESC));
                    DSBDescription.dwSize=sizeof(DSBUFFERDESC);
                    DSBDescription.dwFlags=DSBCAPS_PRIMARYBUFFER|DSBCAPS_CTRL3D|DSBCAPS_CTRLVOLUME;
                    if(IDirectSound_CreateSoundBuffer(device->DShandle,&DSBDescription,&device->DSpbuffer,0)==DS_OK)
                    {
                        if(IDirectSoundBuffer_SetFormat(device->DSpbuffer,&OutputType)==DS_OK)
                        {
                            if(IDirectSoundBuffer_QueryInterface(device->DSpbuffer,&IID_IDirectSound3DListener,&device->DS3dlistener)==DS_OK)
                            {
                                memset(&dsCaps, 0, sizeof(DSCAPS));
                                dsCaps.dwSize = sizeof(DSCAPS);
                                if(IDirectSound_GetCaps(device->DShandle, &dsCaps) == DS_OK)
                                {
                                    // Check that is an accelerated DS device
                                    if(!(dsCaps.dwFlags & DSCAPS_EMULDRIVER))
                                    {
                                        // Check how many Hardware 3D Streaming buffers can be created
                                        device->MaxNoOfSources = dsCaps.dwFreeHw3DStreamingBuffers;
                                        // Some soundcards incorrectly report the number of voices so we are going to
                                        // clamp the number of Sources to an upper limit of 31
                                        if(device->MaxNoOfSources > 31)
                                        {
                                            device->MaxNoOfSources = 31;
                                        }

                                        if(device->MaxNoOfSources > 15)
                                        {
                                            device->CreationFlag = DSBCAPS_LOCHARDWARE;
                                        }
                                        else
                                        {
                                            device->MaxNoOfSources = 16;
                                            device->CreationFlag = DSBCAPS_LOCSOFTWARE;
                                        }
                                        strcpy(device->szDeviceName, "DirectSound3D");
                                        return device;
                                    }
                                }

                                IDirectSound3DListener_Release(device->DS3dlistener);
                                device->DS3dlistener=0;
                            }
                        }
                        IDirectSoundBuffer_Release(device->DSpbuffer);
                        device->DSpbuffer=0;
                    }
                }
                IDirectSound_Release(device->DShandle);
                device->DShandle=0;
            }
        }
        // Failed to initialize DirectSound3D device - so fall back to DirectSound device
        bUseDS = AL_TRUE;
    }

    if(bUseDS)
    {
        //Init COM
        CoInitialize(0);
        //DirectSound Init code
        if(CoCreateInstance(&CLSID_DirectSound,0,CLSCTX_INPROC_SERVER,&IID_IDirectSound,&device->DShandle)==S_OK)
        {
            if(IDirectSound_Initialize(device->DShandle,0)==DS_OK)
            {
                if(IDirectSound_SetCooperativeLevel(device->DShandle,GetForegroundWindow(),DSSCL_PRIORITY)==DS_OK)
                {
                    memset(&DSBDescription,0,sizeof(DSBUFFERDESC));
                    DSBDescription.dwSize=sizeof(DSBUFFERDESC);
                    DSBDescription.dwFlags=DSBCAPS_PRIMARYBUFFER;
                    if(IDirectSound_CreateSoundBuffer(device->DShandle,&DSBDescription,&device->DSpbuffer,0)==DS_OK)
                    {
                        if(IDirectSoundBuffer_SetFormat(device->DSpbuffer,&OutputType)==DS_OK)
                        {
                            memset(&DSBDescription,0,sizeof(DSBUFFERDESC));
                            DSBDescription.dwSize=sizeof(DSBUFFERDESC);
                            DSBDescription.dwFlags=DSBCAPS_GLOBALFOCUS|DSBCAPS_GETCURRENTPOSITION2;
                            DSBDescription.dwBufferBytes=32768;
                            DSBDescription.lpwfxFormat=&OutputType;
                            if(IDirectSound_CreateSoundBuffer(device->DShandle,&DSBDescription,&device->DSsbuffer,0)==DS_OK)
                            {
                                // Check that is a hardware accelerated DS device
                                memset(&dsCaps, 0, sizeof(DSCAPS));
                                dsCaps.dwSize = sizeof(DSCAPS);
                                if(IDirectSound_GetCaps(device->DShandle, &dsCaps) == DS_OK)
                                {
                                    // Check that is an accelerated DS device
                                    if(!(dsCaps.dwFlags & DSCAPS_EMULDRIVER))
                                    {
                                        if(IDirectSoundBuffer_Lock(device->DSsbuffer,0,0,&lpPart1, &dwSize1, &lpPart2, &dwSize2, DSBLOCK_ENTIREBUFFER ) == DS_OK)
                                        {
                                            memset(lpPart1, 0, dwSize1);
                                            IDirectSoundBuffer_Unlock(device->DSsbuffer,lpPart1, dwSize1, lpPart1, dwSize2);
                                        }
                                        if(IDirectSoundBuffer_Play(device->DSsbuffer,0,0,DSBPLAY_LOOPING)==DS_OK)
                                        {
                                            device->timer=timeSetEvent(25,0,alcDirectSoundProc,(DWORD)device,TIME_CALLBACK_FUNCTION|TIME_PERIODIC);
                                            device->MaxNoOfSources = 32;

                                            strcpy(device->szDeviceName, "DirectSound");
                                            return device;
                                        }
                                    }
                                }
                                IDirectSoundBuffer_Release(device->DSsbuffer);
                                device->DSsbuffer=0;
                            }
                        }
                        IDirectSoundBuffer_Release(device->DSpbuffer);
                        device->DSpbuffer=0;
                    }
                }
                IDirectSound_Release(device->DShandle);
                device->DShandle=0;
            }
        }
    }

    // Fallback to WaveOut code
    if(waveOutOpen(&device->handle,WAVE_MAPPER,&OutputType,0,0,WAVE_FORMAT_DIRECT_QUERY)==MMSYSERR_NOERROR)
    {
        if(waveOutOpen(&device->handle,WAVE_MAPPER,&OutputType,(DWORD_PTR)&alcWaveOutProc,(DWORD_PTR)device,CALLBACK_FUNCTION)==MMSYSERR_NOERROR)
        {
            device->hWaveHdrEvent = CreateEvent(0, AL_TRUE, AL_FALSE, "WaveOutAllHeadersReturned");
            if(device->hWaveHdrEvent != 0)
            {
                device->hWaveThreadEvent = CreateEvent(0, AL_TRUE, AL_FALSE, "WaveOutThreadDestroyed");
                if(device->hWaveThreadEvent != 0)
                {
                    device->hThread=CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, device, 0, &device->ThreadID);
                    if(device->hThread != 0)
                    {
                        device->MaxNoOfSources = 32;
                        // Setup Windows Multimedia driver buffers and start playing
                        for(i=0;i<NUMWAVEBUFFERS;i++)
                        {
                            memset(&device->buffer[i],0,sizeof(WAVEHDR));
                            device->buffer[i].lpData = malloc(((OutputType.nAvgBytesPerSec/16)&0xfffffff0));
                            device->buffer[i].dwBufferLength = ((OutputType.nAvgBytesPerSec/16)&0xfffffff0);
                            device->buffer[i].dwFlags = 0;
                            device->buffer[i].dwLoops = 0;
                            waveOutPrepareHeader(device->handle,&device->buffer[i],sizeof(WAVEHDR));
                            if(waveOutWrite(device->handle,&device->buffer[i],sizeof(WAVEHDR))!=MMSYSERR_NOERROR)
                            {
                                waveOutUnprepareHeader(device->handle,&device->buffer[i],sizeof(WAVEHDR));
                                free(device->buffer[i].lpData);
                            }
                            else
                                device->BuffersCommitted++;
                        }
                        strcpy(device->szDeviceName, "MMSYSTEM");
                        return device;
                    }
                    CloseHandle(device->hWaveThreadEvent);
                    device->hWaveThreadEvent = 0;
                }
                CloseHandle(device->hWaveHdrEvent);
                device->hWaveHdrEvent = 0;
            }
            waveOutClose(device->handle);
            device->handle = 0;
        }
    }

    // No suitable output device found
    free(device);
    device = 0;
    return 0;
}


ALCAPI ALvoid ALCAPIENTRY alcCloseDevice(ALdevice* device)
{
    ALint i;

    if(device)
    {
        // Check if its linked to a context.
        if(device->InUse)
        {
            // Not all of the contexts using the device have been destroyed.
            assert(0);
            return;
        }

        // Check if its a 3rd party native driver
        if(device->Dll)
        {
            if(device->AlcApi.alcCloseDevice)
            {
                device->AlcApi.alcCloseDevice(device->DllDevice);
            }

            FreeLibrary(device->Dll);
            free(device);
            return;
        }

        nDeviceUsingTimer--;
        if(!nDeviceUsingTimer)
        {
            // Stop the multimedia timer
            if(nTimerID != 0)
                timeKillEvent(nTimerID);

            // End Timer Period
            timeEndPeriod(nTimerInterval);
        }

        // Release timer
        if(device->timer)
            timeKillEvent(device->timer);

        // Platform specific exit
        if(device->DShandle)
        {
            if(device->DS3dlistener)
                IDirectSound3DListener_Release(device->DS3dlistener);
            if(device->DSsbuffer)
                IDirectSoundBuffer_Release(device->DSsbuffer);
            if(device->DSpbuffer)
                IDirectSoundBuffer_Release(device->DSpbuffer);
            if(device->DShandle)
                IDirectSound_Release(device->DShandle);
            // Deinit COM
            CoUninitialize();
        }

        else
        {
            device->bWaveShutdown = AL_TRUE;

            // Wait for signal that all Wave Buffers have returned
            WaitForSingleObjectEx(device->hWaveHdrEvent, 5000, FALSE);

            // Wait for signal that Wave Thread has been destroyed
            WaitForSingleObjectEx(device->hWaveThreadEvent, 5000, FALSE);

            // Call waveOutReset to shutdown wave device
            waveOutReset(device->handle);

            // Release them all
            for(i=0;i<NUMWAVEBUFFERS;i++)
            {
                waveOutUnprepareHeader(device->handle,&device->buffer[i],sizeof(WAVEHDR));
                free(device->buffer[i].lpData);
            }

            // Close the Wave device
            waveOutClose(device->handle);
            device->handle = 0;


            CloseHandle(device->hThread);
            device->hThread = 0;

            if(device->hWaveHdrEvent)
            {
                CloseHandle(device->hWaveHdrEvent);
                device->hWaveHdrEvent = 0;
            }

            if(device->hWaveThreadEvent)
            {
                CloseHandle(device->hWaveThreadEvent);
                device->hWaveThreadEvent = 0;
            }
        }

        memset(device,0,sizeof(ALdevice));
        free(device);
    }
}


ALCAPI ALboolean ALCAPIENTRY alcEnumerateDevices(ALenumerateDeviceCallback callback, ALvoid* context)
{
    WIN32_FIND_DATA findData;
    HANDLE searchHandle = INVALID_HANDLE_VALUE;
    TCHAR searchName[MAX_PATH + 1];
    TCHAR module[MAX_PATH + 1];
    TCHAR fileDrive[MAX_PATH + 1];
    TCHAR fileDir[MAX_PATH + 1];
    TCHAR fileName[MAX_PATH + 1];
    TCHAR fileExt[MAX_PATH + 1];

    TCHAR dir[3][MAX_PATH + 1];
    DWORD dirSize = 0;
    int i;
    ALboolean found = AL_FALSE;
    HINSTANCE dll = 0;
    ALCAPI_ENUMERATE_DEVICES alcEnumerateDevicesFxn = 0;

    // Start with the default devices
    if(callback("DirectSound3D", context))
       return AL_TRUE;

    if(callback("DirectSound", context))
       return AL_TRUE;

    if(callback("MMSYSTEM", context))
       return AL_TRUE;

    // Construct our search paths.  We will search the current directory, the app directory, and then
    // the system directory.
    dirSize = GetCurrentDirectory(MAX_PATH, dir[0]);
    _tcscat(dir[0], _T("\\"));

    GetModuleFileName(0, module, MAX_PATH);
    _splitpath(module, fileDrive, fileDir, fileName, fileExt);
    _tcscpy(dir[1], fileDrive);
    _tcscat(dir[1], fileDir);
    _tcscat(fileName, fileExt);

    dirSize = GetSystemDirectory(dir[2], MAX_PATH);
    _tcscat(dir[2], _T("\\"));

    // Get the original filename as we don't what to enumerate its devices.
    GetModuleFileName(GetModuleHandle("OpenAL32.dll"), module, MAX_PATH);

    // Begin searching for additional OpenAL implementations.  Start with the current directory, but check if
    // the current directory is the same as the application directory so we don't search it twice.
    for(i = _tcsicmp(dir[0], dir[1]) != 0 ? 0 : 1; i < 3 && !found; i++)
    {
        _tcscpy(searchName, dir[i]);
        _tcscat(searchName, _T("*OpenAL*.dll"));
        searchHandle = FindFirstFile(searchName, &findData);
        if(searchHandle != INVALID_HANDLE_VALUE)
        {
            while(TRUE)
            {
                // Check if this is our module.
                _tcscpy(searchName, dir[i]);
                _tcscat(searchName, findData.cFileName);
                if(_tcsicmp(searchName, module))
                {
                    dll = LoadLibrary(searchName);
                    if(dll)
                    {
                        alcEnumerateDevicesFxn = (ALCAPI_ENUMERATE_DEVICES)GetProcAddress(dll, "alcEnumerateDevices");
                        if(alcEnumerateDevicesFxn)
                        {
                            found = alcEnumerateDevicesFxn(callback, context);
                        }

                        FreeLibrary(dll);
                    }

                    if(found)
                    {
                        break;
                    }
                }

                if(!FindNextFile(searchHandle, &findData))
                {
                    if(GetLastError() == ERROR_NO_MORE_FILES)
                        break;
                }
            }

            FindClose(searchHandle);
            searchHandle = INVALID_HANDLE_VALUE;
        }
    }

    return found;
}


ALCAPI ALboolean ALCAPIENTRY alcIsExtensionPresent(ALdevice* device, ALubyte* ename)
/*
        Determine if there is an extension called extName available
*/
{
    ALsizei i=0;

    if(device && device->DllDevice)
    {
        return device->AlcApi.alcIsExtensionPresent(device, ename);
    }

    while((alcExtensions[i].ename)&&(strcmp((char*)alcExtensions[i].ename,(char*)ename)))
        i++;
    return(alcExtensions[i].ename!=0?AL_TRUE:AL_FALSE);
}


ALCAPI ALvoid* ALCAPIENTRY alcGetProcAddress(ALdevice* device, ALubyte* fname)
/*
        Get the function address for given function extension
*/
{
    ALsizei i=0;

    if(device && device->DllDevice)
    {
        return device->AlcApi.alcGetProcAddress(device, fname);
    }

    while((alcFunctions[i].fname)&&(strcmp((char*)alcFunctions[i].fname,(char*)fname)))
        i++;
    return alcFunctions[i].address;
}


ALCAPI ALenum ALCAPIENTRY alcGetEnumValue(ALdevice* device, ALubyte* ename)
/*
        Get the value for a particular ALC Enumerated Value
        Calls alGetEnumValue in alExtension.c to process request
*/
{
    if(device && device->DllDevice)
    {
        return device->AlcApi.alcGetEnumValue(device, ename);
    }

    return alGetEnumValue(ename);
}

