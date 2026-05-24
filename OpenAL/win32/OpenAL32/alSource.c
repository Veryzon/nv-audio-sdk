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
#include <math.h>
#include <al\alc.h>
#include "OpenAL32.h"
#include "alError.h"
#include "alSource.h"


//*****************************************************************************
//*****************************************************************************
//
// Global Functions
//
//*****************************************************************************
//*****************************************************************************

ALboolean alIsSourceSafe(ALcontext* context, ALuint source)
{
    ALboolean result = AL_FALSE;
    ALsource* alSource;
    unsigned int i;

    if(!context)
        return result;

    // To determine if this is a valid alSource name, look through the list of generated Sources
    alSource = context->Source;
    for(i = 0; i < context->SourceCount; i++)
    {
        if((ALuint)alSource == source)
        {
            result = AL_TRUE;
            break;
        }

        if(!alSource)
        {
            break;
        }

        alSource = alSource->next;
    }

    return result;
}


ALvoid alSourcePlaySafe(ALcontext* context, ALsource* source)
{
    if(source->state != AL_PAUSED)
    {
        source->state = AL_PLAYING;
        source->inuse = AL_TRUE;
        source->play  = AL_TRUE;
        source->position = 0;
        source->position_fraction = 0;
        source->BuffersProcessed = 0;
        source->BuffersAddedToDSBuffer = 0;
        if(source->queue)
            source->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = source->queue->buffer;
    }

    else
    {
        source->state = AL_PLAYING;
        source->inuse = AL_TRUE;
        source->play  = AL_TRUE;
    }

    source->update1 |= STATE;
    alcUpdateContext(context, ALSOURCE, (ALuint)source);
}


ALvoid alSourcePauseSafe(ALcontext* context, ALsource* source)
{
    if(source->state == AL_PLAYING)
    {
        source->state = AL_PAUSED;
        source->inuse = AL_FALSE;
    }

    source->update1 = STATE;
    alcUpdateContext(context, ALSOURCE, (ALuint)source);
}


ALvoid alSourceStopSafe(ALcontext* context, ALsource* source)
{
    ALbufferlistitem* alBufferListItem;

    if(source->state != AL_INITIAL)
    {
        source->state = AL_STOPPED;
        source->inuse = AL_FALSE;
        source->BuffersProcessed = source->BuffersInQueue;
        alBufferListItem = source->queue;
        while(alBufferListItem != NULL)
        {
            alBufferListItem->bufferstate = PROCESSED;
            alBufferListItem = alBufferListItem->next;
        }
    }

    source->update1 |= STATE;
    alcUpdateContext(context, ALSOURCE, (ALuint)source);
}


ALvoid alSourceRewindSafe(ALcontext* context, ALsource* source)
{
    ALbufferlistitem* alBufferListItem;

    if(source->state != AL_INITIAL)
    {
        source->state = AL_INITIAL;
        source->inuse = AL_FALSE;
        source->position = 0;
        source->position_fraction = 0;
        source->BuffersProcessed  = 0;
        alBufferListItem = source->queue;
        while(alBufferListItem != NULL)
        {
            alBufferListItem->bufferstate = PROCESSED;
            alBufferListItem = alBufferListItem->next;
        }

        if(source->queue)
            source->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = source->queue->buffer;

        source->update1 |= STATE;
        alcUpdateContext(context, ALSOURCE, (ALuint)source);
    }
}



//*****************************************************************************
//*****************************************************************************
//
// AL API Entry Points
//
//*****************************************************************************
//*****************************************************************************

ALAPI ALvoid ALAPIENTRY alGenSources(ALsizei n, ALuint* sources)
{
    ALcontext*  alContext;
    ALsource*   alSourcesStart;
    ALsource*   alSource;
    ALdevice*   alDevice;
    ALsizei i;


    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alGenSources(n, sources);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    // Request to generate 0 Sources is a valid NOP
    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadWritePtr(sources, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    // Check that the requested number of sources can be generated
    alDevice = alContext->Device;
    if(alContext->SourceCount + n > alDevice->MaxNoOfSources)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    // Allocate the first source.
    alSourcesStart = (ALsource*)malloc(sizeof(ALsource));
    if(!alSourcesStart)
    {
        alContext->LastError = AL_OUT_OF_MEMORY;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    memset(alSourcesStart, 0, sizeof(ALsource));

    // Allocate the remaining sources.  It is done this way so we can back out if we need to.
    alSource = alSourcesStart;
    for(i = 1; i < n; i++)
    {
        alSource->next = (ALsource*)malloc(sizeof(ALsource));
        if(!alSource->next)
        {
            while(alSourcesStart)
            {
                alSource = alSourcesStart->next;
                free(alSourcesStart);
                alSourcesStart = alSource;
            }

            alContext->LastError = AL_OUT_OF_MEMORY;
            LeaveCriticalSection(&alContext->Lock);
            return;
        }

        memset(alSource->next, 0, sizeof(ALsource));
        alSource->next->previous = alSource;
        alSource = alSource->next;
    }

    // Check if we have an existing list.
    if(!alContext->Source)
    {
        alContext->Source = alSourcesStart;
    }

    else
    {
        alSource = alContext->Source;
        while(alSource->next)
        {
            alSource = alSource->next;
        }

        alSource->next = alSourcesStart;
    }

    i = 0;
    alSource = alSourcesStart;
    while(alSource)
    {
        alSource->valid = AL_TRUE;
        alSource->update1 |= SGENERATESOURCE;

        alSource->param[AL_CONE_INNER_ANGLE-AL_CONE_INNER_ANGLE].f = 360.0f;
        alSource->param[AL_CONE_OUTER_ANGLE-AL_CONE_INNER_ANGLE].f = 360.0f;
        alSource->param[AL_PITCH-AL_CONE_INNER_ANGLE].f = 1.0f;

        alSource->param[AL_POSITION-AL_CONE_INNER_ANGLE].fv3[0] = 0.0f;
        alSource->param[AL_POSITION-AL_CONE_INNER_ANGLE].fv3[1] = 0.0f;
        alSource->param[AL_POSITION-AL_CONE_INNER_ANGLE].fv3[2] = 0.0f;

        alSource->param[AL_DIRECTION-AL_CONE_INNER_ANGLE].fv3[0] = 1.0f;
        alSource->param[AL_DIRECTION-AL_CONE_INNER_ANGLE].fv3[1] = 0.0f;
        alSource->param[AL_DIRECTION-AL_CONE_INNER_ANGLE].fv3[2] = 0.0f;

        alSource->param[AL_VELOCITY-AL_CONE_INNER_ANGLE].fv3[0] = 0.0f;
        alSource->param[AL_VELOCITY-AL_CONE_INNER_ANGLE].fv3[1] = 0.0f;
        alSource->param[AL_VELOCITY-AL_CONE_INNER_ANGLE].fv3[2] = 0.0f;

        alSource->param[AL_REFERENCE_DISTANCE-AL_CONE_INNER_ANGLE].f = 1.0f;

        alSource->param[AL_MAX_DISTANCE-AL_CONE_INNER_ANGLE].f = 1000000.0f;

        alSource->param[AL_ROLLOFF_FACTOR-AL_CONE_INNER_ANGLE].f = 1.0f;

        alSource->param[AL_LOOPING-AL_CONE_INNER_ANGLE].i = 0;

        alSource->param[AL_GAIN-AL_CONE_INNER_ANGLE].f = 1.0f;

        alSource->param[AL_MIN_GAIN-AL_CONE_INNER_ANGLE].f = 0.0f;

        alSource->param[AL_MAX_GAIN-AL_CONE_INNER_ANGLE].f = 1.0f;

        alSource->param[AL_CONE_OUTER_GAIN-AL_CONE_INNER_ANGLE].f = 1.0f;

        alSource->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = 0;

        alSource->state = AL_INITIAL;

        alSource->update1 |= CONEANGLES | FREQUENCY | POSITION | VELOCITY | ORIENTATION |
            MINDIST | MAXDIST | LOOPED | VOLUME | CONEOUTSIDEVOLUME | STATE;


        alContext->SourceCount++;
        alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
        sources[i++] = (ALuint)alSource;
        alSource = alSource->next;
    }

    LeaveCriticalSection(&alContext->Lock);
    return;
}


ALAPI ALvoid ALAPIENTRY alDeleteSources(ALsizei n, ALuint* sources)
{
    ALcontext* alContext;
    ALsource*  alSource;
    ALsizei i;
    ALbufferlistitem* alBufferListItem;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alDeleteSources(n, sources);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    // NOP
    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(sources, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n > alContext->SourceCount)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(alIsSourceSafe(alContext, sources[i]))
        {
            alSource = (ALsource*)sources[i];

            // Make sure its stopped.
            if(alSource->state != AL_INITIAL)
            {
                alSource->state = AL_STOPPED;
                alSource->inuse = AL_FALSE;
                alSource->BuffersProcessed = alSource->BuffersInQueue;
                alBufferListItem = alSource->queue;
                while(alBufferListItem != NULL)
                {
                    alBufferListItem->bufferstate = PROCESSED;
                    alBufferListItem = alBufferListItem->next;
                }

                alSource->update1 |= STATE;
                alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
            }

            // For each buffer in the source's queue, decrement its reference counter and remove it
            while(alSource->queue != NULL)
            {
                alBufferListItem = alSource->queue;

                // Decrement buffer's reference counter
                if(alBufferListItem->buffer != 0)
                    ((ALbuffer*)(alBufferListItem->buffer))->refcount--;

                // Update queue to point to next element in list
                alSource->queue = alBufferListItem->next;

                // Release memory allocated for buffer list item
                free(alBufferListItem);
            }

            // Call alcUpdateContext with SDELETE flag to perform context specific deletion of source
            alSource->update1 = SDELETE;
            alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);

            // Remove alSource from list of Sources
            if(alSource->previous)
                alSource->previous->next = alSource->next;
            else
                alContext->Source = alSource->next;

            if(alSource->next)
                alSource->next->previous = alSource->previous;

            memset(alSource, 0, sizeof(ALsource));
            alContext->SourceCount--;
            free(alSource);
        }

        else alContext->LastError = AL_INVALID_NAME;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALboolean ALAPIENTRY alIsSource(ALuint source)
{
    ALboolean result = AL_FALSE;
    ALcontext* alContext;
    ALsource*  alSource;
    unsigned int i;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return result;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        result = alContext->Device->AlApi.alIsSource(source);
        LeaveCriticalSection(&alContext->Lock);
        return result;
    }

    result = alIsSourceSafe(alContext, source);

    LeaveCriticalSection(&alContext->Lock);
    return result;
}


ALAPI ALvoid ALAPIENTRY alSourcef(ALuint source, ALenum param, ALfloat value)
{
    ALcontext* alContext;
    ALsource*  alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourcef(source, param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        switch(param)
        {
            case AL_PITCH:
                if(value >= 0.0f && value <= 2.0f)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].f = value;
                    alSource->update1 |= FREQUENCY;   // Property to update
                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            case AL_CONE_INNER_ANGLE:
                if(0.0f <= value && value <= 360.0f && value <= alSource->param[AL_CONE_OUTER_ANGLE-AL_CONE_INNER_ANGLE].f)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].f = value;
                    alSource->update1 |= CONEANGLES;
                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            case AL_CONE_OUTER_ANGLE:
                if(0 <= value && value <= 360 && alSource->param[AL_CONE_INNER_ANGLE-AL_CONE_INNER_ANGLE].f <= value)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].f = value;
                    alSource->update1 |= CONEANGLES;
                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            case AL_GAIN:
            case AL_MAX_DISTANCE:
            case AL_ROLLOFF_FACTOR:
            case AL_REFERENCE_DISTANCE:
                if(value >= 0.0f)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].f = value;

                    if(param == AL_GAIN)
                        alSource->update1 |= VOLUME;
                    else if(param == AL_MAX_DISTANCE)
                        alSource->update1 |= MAXDIST;
                    else if(param == AL_REFERENCE_DISTANCE)
                        alSource->update1 |= MINDIST;

                    // ROLLOFF_FACTOR ignored at this time !

                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            case AL_MIN_GAIN:
            case AL_MAX_GAIN:
            case AL_CONE_OUTER_GAIN:
                if(value >= 0.0f && value <= 1.0f)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].f = value;

                    // MIN_GAIN and MAX_GAIN unsupported at this time

                    alSource->update1 |= CONEOUTSIDEVOLUME;
                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;
        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourcefv(ALuint source, ALenum param, ALfloat* values)
{
    ALcontext* alContext;
    ALsource*  alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourcefv(source, param, values);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(values, 3 * sizeof(ALfloat)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        switch(param)
        {
            case AL_POSITION:
            case AL_VELOCITY:
            case AL_DIRECTION:
                alSource->param[param-AL_CONE_INNER_ANGLE].fv3[0] = values[0];
                alSource->param[param-AL_CONE_INNER_ANGLE].fv3[1] = values[1];
                alSource->param[param-AL_CONE_INNER_ANGLE].fv3[2] = values[2];

                if(param == AL_POSITION)
                    alSource->update1 |= POSITION;
                else if(param == AL_VELOCITY)
                    alSource->update1 |= VELOCITY;
                else if(param == AL_DIRECTION)
                    alSource->update1 |= ORIENTATION;

                alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;
        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSource3f(ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3)
{
    ALcontext* alContext;
    ALsource*  alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSource3f(source, param, v1, v2, v3);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        switch(param)
        {
            case AL_POSITION:
            case AL_VELOCITY:
            case AL_DIRECTION:
                alSource->param[param-AL_CONE_INNER_ANGLE].fv3[0] = v1;
                alSource->param[param-AL_CONE_INNER_ANGLE].fv3[1] = v2;
                alSource->param[param-AL_CONE_INNER_ANGLE].fv3[2] = v3;

                if(param == AL_POSITION)
                    alSource->update1 |= POSITION;
                else if(param == AL_VELOCITY)
                    alSource->update1 |= VELOCITY;
                else if(param == AL_DIRECTION)
                    alSource->update1 |= ORIENTATION;

                alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;
        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourcei(ALuint source, ALenum param, ALint value)
{
    ALcontext* alContext;
    ALsource*  alSource;
    ALbufferlistitem* alBufferListItem;
    ALbufferlistitem* newAlBufferListItem;
    ALint Counter = 0;
    ALint DataSize = 0;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourcei(source, param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        switch(param)
        {
            case AL_SOURCE_RELATIVE:
                alSource->relative = value == AL_TRUE;
                alSource->update1 |= MODE;
                alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                break;

            case AL_CONE_INNER_ANGLE:
                if(0 <= value && value <= 360 && (ALfloat)value <= alSource->param[AL_CONE_OUTER_ANGLE-AL_CONE_INNER_ANGLE].f)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].f = (ALfloat)value;
                    alSource->update1 |= CONEANGLES;
                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            case AL_CONE_OUTER_ANGLE:
                if(0 <= value && value <= 360 && alSource->param[AL_CONE_INNER_ANGLE-AL_CONE_INNER_ANGLE].f <= (ALfloat)value)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].f = (ALfloat)value;
                    alSource->update1 |= CONEANGLES;
                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            case AL_LOOPING:
                if(value == AL_FALSE || value == AL_TRUE)
                {
                    alSource->param[param-AL_CONE_INNER_ANGLE].i = value;
                    alSource->update1 |= LOOPED;
                    alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                }

                else
                    alContext->LastError = AL_INVALID_VALUE;

                break;

            case AL_BUFFER:
                if(alSource->state == AL_STOPPED || alSource->state == AL_INITIAL)
                {
                    if(alIsBufferSafe(alContext, value) || value == 0)
                    {
                        // Allocate the new buffer list item.
                        if(value)
                        {
                            newAlBufferListItem = malloc(sizeof(ALbufferlistitem));
                            if(!newAlBufferListItem)
                            {
                                alContext->LastError = AL_OUT_OF_MEMORY;
                                break;
                            }
                        }

                        // Remove all elements in the queue
                        while(alSource->queue != NULL)
                        {
                            alBufferListItem = alSource->queue;
                            alSource->queue  = alBufferListItem->next;

                            if(alBufferListItem->buffer)
                            {
                                // Decrement reference counter for buffer
                                ((ALbuffer*)(alBufferListItem->buffer))->refcount--;

                                // Record size of buffer
                                DataSize += ((ALbuffer*)alBufferListItem->buffer)->size;
                            }

                            // Increment the number of buffers removed from queue
                            Counter++;

                            // Release memory for buffer list item
                            free(alBufferListItem);

                            // Decrement the number of buffers in the queue
                            alSource->BuffersInQueue--;
                        }

                        // Update variables required by the SUNQUEUE routine in UpdateContext
                        alSource->NumBuffersRemovedFromQueue = Counter;
                        alSource->SizeOfBufferDataRemovedFromQueue = DataSize;
                        alSource->update1 |= SUNQUEUE;
                        alcUpdateContext(alContext, ALSOURCE, source);

                        // Add the buffer to the queue (as long as it is NOT the NULL buffer, AND it is
                        // more than 0 bytes long)
                        if(value)
                        {
                            // Add the selected buffer to the queue
                            alBufferListItem = newAlBufferListItem;
                            alBufferListItem->buffer = value;
                            alBufferListItem->bufferstate = PENDING;
                            alBufferListItem->flag = 0;
                            alBufferListItem->next = NULL;

                            alSource->queue = alBufferListItem;
                            alSource->BuffersInQueue = 1;

                            DataSize = ((ALbuffer*)value)->size;

                            // Increment reference counter for buffer
                            ((ALbuffer*)value)->refcount++;

                            alSource->SizeOfBufferDataAddedToQueue = DataSize;
                            alSource->NumBuffersAddedToQueue = 1;
                            alSource->update1 |= SQUEUE;
                            alcUpdateContext(alContext, ALSOURCE, source);
                        }

                        // Set Buffers Processed
                        alSource->BuffersProcessed = 0;

                        // Update AL_BUFFER parameter
                        alSource->param[param-AL_CONE_INNER_ANGLE].i = value;
                    }

                    else
                        alContext->LastError = AL_INVALID_VALUE;

                }

                else
                    alContext->LastError = AL_INVALID_OPERATION;

                break;

            case AL_SOURCE_STATE:
                alSource->state = value;
                alSource->update1 |= STATE;
                alcUpdateContext(alContext, ALSOURCE, (ALuint)alSource);
                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;

        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetSourcef(ALuint source, ALenum param, ALfloat* value)
{
    ALcontext* alContext;
    ALsource* alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alGetSourcef(source, param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadWritePtr(value, sizeof(ALfloat)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        switch(param)
        {
            case AL_PITCH:
            case AL_GAIN:
            case AL_MIN_GAIN:
            case AL_MAX_GAIN:
            case AL_MAX_DISTANCE:
            case AL_ROLLOFF_FACTOR:
            case AL_CONE_OUTER_GAIN:
            case AL_CONE_INNER_ANGLE:
            case AL_CONE_OUTER_ANGLE:
            case AL_REFERENCE_DISTANCE:
                *value = alSource->param[param-AL_CONE_INNER_ANGLE].f;
                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;
        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetSource3f(ALuint source, ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3)
{
    ALcontext* alContext;
    ALsource* alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alGetSource3f(source, param, v1, v2, v3);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadWritePtr(v1, sizeof(ALfloat)) ||
       IsBadWritePtr(v2, sizeof(ALfloat)) ||
       IsBadWritePtr(v3, sizeof(ALfloat)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource=(ALsource*)source;
        switch(param)
        {
            case AL_POSITION:
            case AL_VELOCITY:
            case AL_DIRECTION:
                *v1 = alSource->param[param-AL_CONE_INNER_ANGLE].fv3[0];
                *v2 = alSource->param[param-AL_CONE_INNER_ANGLE].fv3[1];
                *v3 = alSource->param[param-AL_CONE_INNER_ANGLE].fv3[2];
                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;
        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetSourcefv(ALuint source, ALenum param, ALfloat* values)
{
    ALcontext* alContext;
    ALsource* alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alGetSourcefv(source, param, values);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadWritePtr(values, sizeof(ALfloat) * 3))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        switch(param)
        {
            case AL_POSITION:
            case AL_VELOCITY:
            case AL_DIRECTION:
                values[0] = alSource->param[param-AL_CONE_INNER_ANGLE].fv3[0];
                values[1] = alSource->param[param-AL_CONE_INNER_ANGLE].fv3[1];
                values[2] = alSource->param[param-AL_CONE_INNER_ANGLE].fv3[2];
                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;
        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetSourcei(ALuint source, ALenum param, ALint* value)
{
    ALcontext* alContext;
    ALsource* alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alGetSourcei(source, param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadWritePtr(value, sizeof(ALint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        switch(param)
        {
            case AL_SOURCE_RELATIVE:
                *value = alSource->relative;
                break;

            case AL_CONE_INNER_ANGLE:
            case AL_CONE_OUTER_ANGLE:
                *value = (ALint)alSource->param[param-AL_CONE_INNER_ANGLE].f;
                break;

            case AL_LOOPING:
                *value = alSource->param[param-AL_CONE_INNER_ANGLE].i;
                break;

            case AL_BUFFER:
                // Call UpdateContext to retrieve up-to-date information about the current Buffer
                alSource->update1 |= SUPDATEBUFFERS;
                alcUpdateContext(alContext, ALSOURCE, source);
                *value = alSource->param[param-AL_CONE_INNER_ANGLE].i;
                break;

            case AL_SOURCE_STATE:
                // Call UpdateContext to retrieve up-to-date information about the state of the alSource
                alSource->update1 |= SUPDATEBUFFERS;
                alcUpdateContext(alContext, ALSOURCE, source);
                *value = alSource->state;
                break;

            case AL_BUFFERS_QUEUED:
                *value = alSource->BuffersInQueue;
                break;

            case AL_BUFFERS_PROCESSED:
                // Call UpdateContext to retrieve up-to-date information about the number of Buffers processed
                alSource->update1 |= SUPDATEBUFFERS;
                alcUpdateContext(alContext, ALSOURCE, source);
                *value = alSource->BuffersProcessed;
                break;

            default:
                alContext->LastError = AL_INVALID_ENUM;
                break;
        }
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourcePlay(ALuint source)
{
    ALcontext* alContext;
    ALsource* alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourcePlay(source);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        alSourcePlaySafe(alContext, alSource);
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourcePlayv(ALsizei n, ALuint* sources)
{
    ALcontext* alContext;
    ALsource* alSource;
    ALsizei i;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourcePlayv(n, sources);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(sources, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n > alContext->SourceCount)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(alIsSourceSafe(alContext, sources[i]))
        {
            alSource = (ALsource*)sources[i];
            alSourcePlaySafe(alContext, alSource);
        }

        else alContext->LastError = AL_INVALID_NAME;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourcePause(ALuint source)
{
    ALcontext* alContext;
    ALsource* alSource;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourcePause(source);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        alSourcePauseSafe(alContext, alSource);
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}

ALAPI ALvoid ALAPIENTRY alSourcePausev(ALsizei n, ALuint* sources)
{
    ALcontext* alContext;
    ALsource* alSource;
    ALsizei i;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourcePausev(n, sources);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(sources, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n > alContext->SourceCount)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(alIsSourceSafe(alContext, sources[i]))
        {
            alSource = (ALsource*)sources[i];
            alSourcePauseSafe(alContext, alSource);
        }

        else alContext->LastError = AL_INVALID_NAME;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourceStop(ALuint source)
{
    ALcontext* alContext;
    ALsource* alSource;
    ALbufferlistitem* alBufferListItem;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourceStop(source);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        alSourceStopSafe(alContext, alSource);
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}

ALAPI ALvoid ALAPIENTRY alSourceStopv(ALsizei n, ALuint* sources)
{
    ALcontext* alContext;
    ALsource* alSource;
    ALsizei i;
    ALbufferlistitem* alBufferListItem;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourceStopv(n, sources);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(sources, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n > alContext->SourceCount)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(alIsSourceSafe(alContext, sources[i]))
        {
            alSource = (ALsource*)sources[i];
            alSourceStopSafe(alContext, alSource);
        }

        else alContext->LastError = AL_INVALID_NAME;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourceRewind(ALuint source)
{
    ALcontext* alContext;
    ALsource* alSource;
    ALbufferlistitem* alBufferListItem;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourceRewind(source);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(alIsSourceSafe(alContext, source))
    {
        alSource = (ALsource*)source;
        alSourceRewindSafe(alContext, alSource);
    }

    else alContext->LastError = AL_INVALID_NAME;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourceRewindv(ALsizei n, ALuint* sources)
{
    ALcontext* alContext;
    ALsource* alSource;
    ALsizei i;
    ALbufferlistitem* alBufferListItem;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourceRewindv(n, sources);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(sources, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n > alContext->SourceCount)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(alIsSourceSafe(alContext, sources[i]))
        {
            alSource = (ALsource*)sources[i];
            alSourceRewindSafe(alContext, alSource);
        }

        else alContext->LastError = AL_INVALID_NAME;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourceQueueBuffers(ALuint source, ALsizei n, ALuint* buffers)
{
    ALcontext* alContext;
    ALsource* alSource;
    ALsizei i;
    ALbufferlistitem* alBufferList;
    ALbufferlistitem* alBufferListStart;
    ALuint DataSize = 0;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourceQueueBuffers(source, n, buffers);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(buffers, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n > alContext->BufferCount)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    alSource = (ALsource*)source;

    // Check that all buffers are valid or zero and that the source is valid
    if(!alIsSourceSafe(alContext, source))
    {
        alContext->LastError = AL_INVALID_NAME;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    for(i = 0; i < n; i++)
    {
        if(!alIsBufferSafe(alContext, buffers[i]) && buffers[i] != 0)
        {
            alContext->LastError = AL_INVALID_NAME;
            LeaveCriticalSection(&alContext->Lock);
            return;
        }
    }


    // Allocate the first buffer list item.
    alBufferListStart = (ALbufferlistitem*)malloc(sizeof(ALbufferlistitem));
    if(!alBufferListStart)
    {
        alContext->LastError = AL_OUT_OF_MEMORY;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    memset(alBufferListStart, 0, sizeof(ALbufferlistitem));

    // Allocate the remaining buffer list items.  It is done this way so we can back out if we need to.
    alBufferList = alBufferListStart;
    for(i = 1; i < n; i++)
    {
        alBufferList->next = (ALbufferlistitem*)malloc(sizeof(ALbufferlistitem));
        if(!alBufferList->next)
        {
            while(alBufferListStart)
            {
                alBufferList = alBufferListStart->next;
                free(alBufferListStart);
                alBufferListStart = alBufferList;
            }

            alContext->LastError = AL_OUT_OF_MEMORY;
            LeaveCriticalSection(&alContext->Lock);
            return;
        }

        memset(alBufferList->next, 0, sizeof(ALbufferlistitem));
        alBufferList = alBufferList->next;
    }


    i = 0;
    alBufferList = alBufferListStart;
    while(alBufferList)
    {
        alBufferList->buffer = buffers[i];
        alBufferList->bufferstate = PENDING;
        alBufferList->flag = 0;

        if(buffers[i])
            DataSize += ((ALbuffer*)buffers[i])->size;

        // Increment reference counter for buffer
        if(buffers[i])
            ((ALbuffer*)buffers[i])->refcount++;

        alBufferList = alBufferList->next;
    }


    if(alSource->queue == NULL)
    {
        alSource->queue = alBufferListStart;

        // Update Current Buffer
        alSource->param[AL_BUFFER - AL_CONE_INNER_ANGLE].i = alBufferListStart->buffer;
    }

    else
    {
        // Find end of queue
        alBufferList = alSource->queue;
        while(alBufferList->next != NULL)
        {
            alBufferList = alBufferList->next;
        }

        alBufferList->next = alBufferListStart;
    }

    // Update number of buffers in queue
    alSource->BuffersInQueue += n;

    // Record the amount of data added to the queue
    alSource->SizeOfBufferDataAddedToQueue = DataSize;
    alSource->NumBuffersAddedToQueue = n;
    alSource->update1 |= SQUEUE;
    alcUpdateContext(alContext, ALSOURCE, source);

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alSourceUnqueueBuffers(ALuint source, ALsizei n, ALuint* buffers)
{
    ALcontext* alContext;
    ALsource*  alSource;
    ALsizei i;
    ALbufferlistitem* alBufferList;
    ALuint DataSize = 0;
    ALuint BufferID;
    ALboolean bBuffersProcessed;

    bBuffersProcessed = AL_TRUE;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        alContext->Device->AlApi.alSourceUnqueueBuffers(source, n, buffers);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n == 0)
    {
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(IsBadReadPtr(buffers, n * sizeof(ALuint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    if(n > alContext->BufferCount)
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    // Check that all buffers are valid or zero and that the source is valid
    if(!alIsSourceSafe(alContext, source))
    {
        alContext->LastError = AL_INVALID_NAME;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    // Check that all 'n' buffers have been processed
    alSource = (ALsource*)source;
    alBufferList = alSource->queue;
    for(i = 0; i < n; i++)
    {
        if(alBufferList && alBufferList->bufferstate == PROCESSED)
        {
            alBufferList = alBufferList->next;
        }

        else
        {
            alContext->LastError = AL_INVALID_VALUE;
            LeaveCriticalSection(&alContext->Lock);
            return;
        }
    }

    // If all 'n' buffers have been processed, remove them from the queue
    for(i = 0; i < n; i++)
    {
        alBufferList = alSource->queue;
        alSource->queue = alBufferList->next;

        // Record name of buffer
        buffers[i] = alBufferList->buffer;

        // Decrement buffer reference counter
        if(alBufferList->buffer)
            ((ALbuffer*)(alBufferList->buffer))->refcount--;

        // Record size of buffer
        if(alBufferList->buffer)
            DataSize += ((ALbuffer*)alBufferList->buffer)->size;

        // Release memory for buffer list item
        free(alBufferList);
        alSource->BuffersInQueue--;
        alSource->BuffersProcessed--;
    }

    if(alSource->state != AL_PLAYING)
    {
        if(alSource->queue)
            BufferID = alSource->queue->buffer;
        else
            BufferID = 0;

        alSource->param[AL_BUFFER-AL_CONE_INNER_ANGLE].i = BufferID;
    }

    alSource->NumBuffersRemovedFromQueue = n;
    alSource->SizeOfBufferDataRemovedFromQueue = DataSize;
    alSource->BuffersAddedToDSBuffer -= alSource->NumBuffersRemovedFromQueue;
    alSource->update1 |= SUNQUEUE;
    alcUpdateContext(alContext, ALSOURCE, source);

    LeaveCriticalSection(&alContext->Lock);
}