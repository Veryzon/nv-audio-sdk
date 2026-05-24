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




#include <stdlib.h>
#include <stdio.h>
#include <al\alc.h>
#include "OpenAL32.h"
#include "alError.h"
#include "alBuffer.h"


//*****************************************************************************
//*****************************************************************************
//
// Local Functions
//
//*****************************************************************************
//*****************************************************************************

ALboolean alIsBufferSafe(ALcontext* context, ALuint bufferName)
{
    ALboolean result = AL_FALSE;
    ALbuffer* buffer;
    unsigned int i;

    if(!bufferName)
    {
        return AL_FALSE;
    }

    buffer = context->BufferList;
    for(i = 0; i < context->BufferCount; i++)
    {
        if((ALuint)buffer == bufferName)
        {
            result = AL_TRUE;
            break;
        }

        if(!buffer)
        {
            break;
        }

        buffer = buffer->next;
    }

    return result;
}



//*****************************************************************************
//*****************************************************************************
//
// AL API Entry Points
//
//*****************************************************************************
//*****************************************************************************

ALAPI ALvoid ALAPIENTRY alGenBuffers(ALsizei n, ALuint* bufferNames)
{
    ALbuffer* buffer;
    ALbuffer* buffersStart;
    ALsizei i = 0;
    ALcontext* context;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    context = alcCurrentContext;
    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    if(context->DllContext)
    {
        context->Device->AlApi.alGenBuffers(n, bufferNames);
        LeaveCriticalSection(&context->Lock);
        return;
    }


    // Request to generate 0 Buffers is a valid NOP
    if(n == 0)
    {
        LeaveCriticalSection(&context->Lock);
        return;
    }

    if(IsBadWritePtr(bufferNames, n * sizeof(ALuint)))
    {
        context->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&context->Lock);
        return;
    }


    // Allocate the first source.
    buffersStart = (ALbuffer*)malloc(sizeof(ALbuffer));
    if(!buffersStart)
    {
        context->LastError = AL_OUT_OF_MEMORY;
        LeaveCriticalSection(&context->Lock);
        return;
    }

    memset(buffersStart, 0, sizeof(ALbuffer));

    // Allocate the remaining buffers.  It is done this way so we can back out if we need to.
    buffer = buffersStart;
    for(i = 1; i < n; i++)
    {
        buffer->next = (ALbuffer*)malloc(sizeof(ALbuffer));
        if(!buffer->next)
        {
            while(buffersStart)
            {
                buffer = buffersStart->next;
                free(buffersStart);
                buffersStart = buffer;
            }

            context->LastError = AL_OUT_OF_MEMORY;
            LeaveCriticalSection(&context->Lock);
            return;
        }

        memset(buffer->next, 0, sizeof(ALbuffer));
        buffer->next->previous = buffer;
        buffer = buffer->next;
    }


    // Check if we have an existing list.
    if(!context->BufferList)
    {
        context->BufferList = buffersStart;
    }

    else
    {
        buffer = context->BufferList;
        while(buffer->next)
        {
            buffer = buffer->next;
        }

        buffer->next = buffersStart;
    }


    i = 0;
    buffer = buffersStart;
    while(buffer)
    {
        context->BufferList->state = UNUSED;
        context->BufferCount++;
        bufferNames[i++] = (ALuint)buffer;
        buffer = buffer->next;
    }

    LeaveCriticalSection(&context->Lock);
}


ALAPI ALvoid ALAPIENTRY alDeleteBuffers(ALsizei n, ALuint* bufferNames)
{
    ALbuffer* buffer;
    ALsizei i;
    ALcontext* context;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    context = alcCurrentContext;
    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    if(context->DllContext)
    {
        context->Device->AlApi.alDeleteBuffers(n, bufferNames);
        LeaveCriticalSection(&context->Lock);
        return;
    }


    // Request to generate 0 Buffers is a valid NOP
    if(n == 0)
    {
        LeaveCriticalSection(&context->Lock);
        return;
    }

    if(n > context->BufferCount)
    {
        context->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&context->Lock);
        return;
    }

    if(IsBadReadPtr(bufferNames, n * sizeof(ALuint)))
    {
        context->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&context->Lock);
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(alIsBufferSafe(context, bufferNames[i]))
        {
            buffer = (ALbuffer*)bufferNames[i];

            // Check the reference counter for the buffer
            if(buffer->refcount == 0)
            {
                if(buffer->previous)
                    buffer->previous->next = buffer->next;
                else
                    context->BufferList = buffer->next;

                if(buffer->next)
                    buffer->next->previous = buffer->previous;

                // Release the memory used to store audio data
                if(buffer->data)
                    free(buffer->data);

                // Release buffer structure
                memset(buffer, 0, sizeof(ALbuffer));
                context->BufferCount--;
                free(buffer);
            }

            else
                context->LastError = AL_INVALID_OPERATION;
        }

        else
        {
            if(bufferNames[i] != 0)
                context->LastError = AL_INVALID_NAME;
        }
    }

    LeaveCriticalSection(&context->Lock);
}


ALAPI ALboolean ALAPIENTRY alIsBuffer(ALuint bufferName)
{
    ALboolean result = AL_FALSE;
    ALcontext* context;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return FALSE;
    }

    context = alcCurrentContext;
    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    result = alIsBufferSafe(context, bufferName);

    LeaveCriticalSection(&context->Lock);
    return result;
}


ALAPI ALvoid ALAPIENTRY alBufferData(ALuint bufferName, ALenum format, ALvoid* data, ALsizei size, ALsizei freq)
{
    ALbuffer* buffer;
    ALsizei i;
    ALcontext* context;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    context = alcCurrentContext;
    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    if(context->DllContext)
    {
        context->Device->AlApi.alBufferData(bufferName, format, data, size, freq);
        LeaveCriticalSection(&context->Lock);
        return;
    }


    if(IsBadReadPtr(data, size))
    {
        context->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&context->Lock);
        return;
    }

    if(alIsBufferSafe(context, bufferName))
    {
        buffer = (ALbuffer*)bufferName;
        if(buffer->state == UNUSED && data)
        {
            switch(format)
            {
                case AL_FORMAT_MONO8:
                    buffer->data = realloc(buffer->data,size/sizeof(ALubyte)*sizeof(ALshort));
                    if(buffer->data)
                    {
                        buffer->format = AL_FORMAT_MONO16;
                        for(i = 0; i < size / sizeof(ALubyte); i++)
                            buffer->data[i] = (ALshort)((((ALubyte*)data)[i]-128)<<8);

                        buffer->size = size / sizeof(ALubyte) * sizeof(ALshort);
                        buffer->frequency = freq;
                    }

                    else
                        context->LastError = AL_OUT_OF_MEMORY;

                    break;

                case AL_FORMAT_MONO16:
                    buffer->data = realloc(buffer->data, size / sizeof(ALshort) * sizeof(ALshort));
                    if(buffer->data)
                    {
                        buffer->format = AL_FORMAT_MONO16;
                        memcpy(buffer->data, data, size / sizeof(ALshort) * sizeof(ALshort));
                        buffer->size = size / sizeof(ALshort) * sizeof(ALshort);
                        buffer->frequency = freq;
                    }

                    else
                        context->LastError = AL_OUT_OF_MEMORY;

                    break;

                case AL_FORMAT_STEREO8:
                    buffer->data = realloc(buffer->data, size / sizeof(ALubyte) * sizeof(ALshort));
                    if(buffer->data)
                    {
                        buffer->format = AL_FORMAT_STEREO16;
                        for(i = 0; i < size / sizeof(ALubyte); i++)
                            buffer->data[i] = (ALshort)((((ALubyte*)data)[i] - 128) << 8);

                        buffer->size = size / sizeof(ALubyte) * sizeof(ALshort);
                        buffer->frequency = freq;
                    }

                    else
                        context->LastError = AL_OUT_OF_MEMORY;

                    break;

                case AL_FORMAT_STEREO16:
                    buffer->data = realloc(buffer->data, size / sizeof(ALshort) * sizeof(ALshort));
                    if(buffer->data)
                    {
                        buffer->format = AL_FORMAT_STEREO16;
                        memcpy(buffer->data, data, size / sizeof(ALshort) * sizeof(ALshort));
                        buffer->size = size / sizeof(ALshort) * sizeof(ALshort);
                        buffer->frequency = freq;
                    }

                    else
                        context->LastError = AL_OUT_OF_MEMORY;

                    break;

                default:
                    context->LastError = AL_INVALID_VALUE;
                    break;
            }
        }

        else context->LastError = AL_INVALID_VALUE;
    }

    else context->LastError = AL_INVALID_OPERATION;

    LeaveCriticalSection(&context->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetBufferf(ALuint bufferName, ALenum param, ALfloat* value)
{
    ALbuffer* buffer;
    ALcontext* context;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    context = alcCurrentContext;
    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    if(context->DllContext)
    {
        context->Device->AlApi.alGetBufferf(bufferName, param, value);
        LeaveCriticalSection(&context->Lock);
        return;
    }


    if(IsBadWritePtr(value, sizeof(ALfloat)))
    {
        context->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&context->Lock);
        return;
    }

    if(alIsBufferSafe(context, bufferName))
    {
        buffer = (ALbuffer*)bufferName;
        switch(param)
        {
            default:
                context->LastError = AL_INVALID_OPERATION;
                break;
        }
    }

    else context->LastError = AL_INVALID_OPERATION;

    LeaveCriticalSection(&context->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetBufferi(ALuint bufferName, ALenum param, ALint* value)
{
    ALbuffer*  buffer;
    ALcontext* context;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    context = alcCurrentContext;
    EnterCriticalSection(&context->Lock);
    alListReleaseLock(alcContextList);

    if(context->DllContext)
    {
        context->Device->AlApi.alGetBufferi(bufferName, param, value);
        LeaveCriticalSection(&context->Lock);
        return;
    }


    if(IsBadWritePtr(value, sizeof(ALint)))
    {
        context->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&context->Lock);
        return;
    }

    if(alIsBufferSafe(context, bufferName))
    {
        buffer = (ALbuffer*)bufferName;
        switch(param)
        {
            case AL_FREQUENCY:
                *value = buffer->frequency;
                break;

            case AL_BITS:
                *value = (buffer->format == AL_FORMAT_MONO8 || buffer->format == AL_FORMAT_STEREO8) ? 8 : 16;
                break;

            case AL_CHANNELS:
                *value= (buffer->format == AL_FORMAT_MONO8 || buffer->format == AL_FORMAT_MONO16) ? 1 : 2;
                break;

            case AL_SIZE:
                *value = buffer->size;
                break;

            default:
                context->LastError = AL_INVALID_OPERATION;
                break;
        }
    }

    else context->LastError = AL_INVALID_OPERATION;

    LeaveCriticalSection(&context->Lock);
}


/*
        Releasebuffers()

        Called by DLLMain on exit to destroy any buffers that still exist
*/
ALvoid Releasebuffers(ALvoid)
{
    ALlistEntry * alListEntry;
    ALcontext* context;
    ALbuffer* buffer;
    ALbuffer* bufferTemp;
    unsigned int i;
#ifdef _DEBUG
    char szString[256];
#endif

    // Check if we still have contexts.
    if(!alcContextList)
    {
        return;
    }

    alListAcquireLock(alcContextList);
    for(alListEntry = alListIteratorReset(alcContextList); alListEntry;
        alListEntry = alListIteratorNext(alcContextList))
    {
        context = (ALcontext*)alListGetData(alListEntry);

        // Let 3rd party guys take care of their own.
        if(context->DllContext)
        {
            continue;
        }

        EnterCriticalSection(&context->Lock);

#ifdef _DEBUG
        if(context->BufferCount > 0)
        {
            sprintf(szString, "OpenAL32 : DllMain() %d Buffer(s) NOT deleted\n", context->BufferCount);
            OutputDebugString(szString);
        }
#endif

        buffer = context->BufferList;
        for(i = 0; i < context->BufferCount; i++)
        {
            // Release sample data
            if(buffer->data)
                free(buffer->data);

            // Release Buffer structure
            bufferTemp = buffer;
            buffer = buffer->next;
            memset(bufferTemp, 0, sizeof(ALbuffer));
            free(bufferTemp);
        }

        LeaveCriticalSection(&context->Lock);
    }

    alListReleaseLock(alcContextList);
}