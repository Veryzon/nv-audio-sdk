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

#include <al/alc.h>
#include "OpenAL32.h"
#include "alError.h"
#include "alEax.h"
#include "alListener.h"
#include <objbase.h>


// EAX 2.0 GUIDs
const GUID DSPROPSETID_EAX20_ListenerProperties
= { 0x306a6a8, 0xb224, 0x11d2, { 0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22}};

const GUID DSPROPSETID_EAX20_BufferProperties
= { 0x306a6a7, 0xb224, 0x11d2, {0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22}};


/*
        Test for support of appropriate EAX Version
*/
ALboolean CheckEAXSupport()
{
    ALcontext*              alContext;
    ALdevice*               alDevice;
    ALsource*               alSource;
    ALsource                newSource;
    LPKSPROPERTYSET         lpPropertySet;
    GUID                    ListenerGuid, BufferGuid;
    ALuint                  ListenerProperty, BufferProperty;
    ALuint                  i, ulSupport;
    ALuint                  property, size;
    ALint                   value = 0xFFFFFFFF;
    ALfloat                 fvalue;
    ALboolean               bSourceGenerated = AL_FALSE;
    ALboolean               bEAXSupported = AL_FALSE;

    ListenerGuid = DSPROPSETID_EAX20_ListenerProperties;
    BufferGuid = DSPROPSETID_EAX20_BufferProperties;
    ListenerProperty = 1;           // LISTENER_ALL
    BufferProperty = 1;             // BUFFER_ALL
    property = 2;                   // ROOM
    size = sizeof(ALint);
    value = -10000;

    // At this point we can assume the current alContext is valid and is locked.
    alContext = alcCurrentContext;
    alDevice = alcCurrentContext->Device;
    alSource = alContext->Source;
    lpPropertySet = 0;

    // See if a alSource has already been created.
    for(i = 0; i < alContext->SourceCount; i++)
    {
        if(alSource->uservalue3)
        {
            lpPropertySet = alSource->uservalue3;
            break;
        }

        alSource = alSource->next;
    }

    // If we didn't find a valid alSource, create one now
    if(!lpPropertySet && alDevice->DShandle)
    {
        DSBUFFERDESC DSBDescription;
        WAVEFORMATEX OutputType;

        memset(&newSource, 0, sizeof(ALsource));

        // Create a streaming DS buffer - 16bit mono 44.1KHz, 1 second in length
        memset(&DSBDescription, 0, sizeof(DSBUFFERDESC));
        DSBDescription.dwSize = sizeof(DSBUFFERDESC);
        DSBDescription.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D | DSBCAPS_GLOBALFOCUS |
                               DSBCAPS_GETCURRENTPOSITION2 | alDevice->CreationFlag;
        DSBDescription.dwBufferBytes = 88200;
        DSBDescription.lpwfxFormat = &OutputType;

        memset(&OutputType, 0, sizeof(WAVEFORMATEX));
        OutputType.wFormatTag = WAVE_FORMAT_PCM;
        OutputType.nChannels = 1;
        OutputType.wBitsPerSample = 16;
        OutputType.nBlockAlign = 2;
        OutputType.nSamplesPerSec = 44100;
        OutputType.nAvgBytesPerSec = 88200;
        OutputType.cbSize = 0;
        if(IDirectSound_CreateSoundBuffer(alDevice->DShandle, &DSBDescription, &(LPDIRECTSOUNDBUFFER)newSource.uservalue1, 0) == DS_OK)
        {
            // Get 3D Interface
            if(IDirectSoundBuffer_QueryInterface((LPDIRECTSOUNDBUFFER)newSource.uservalue1, &IID_IDirectSound3DBuffer,
                                                 (LPUNKNOWN*)&newSource.uservalue2) == DS_OK)
            {
                // Get Property Set Interface
                IDirectSound3DBuffer_QueryInterface((LPDIRECTSOUND3DBUFFER)newSource.uservalue2, &IID_IKsPropertySet,
                                                    (LPUNKNOWN*)&newSource.uservalue3);
            }
        }

        lpPropertySet = newSource.uservalue3;

    }

    // If Property Set Interface hasn't been obtained, EAX support is not available
    if(!lpPropertySet)
    {
        if(newSource.uservalue1)
        {
            if(newSource.uservalue3)
            {
                IKsPropertySet_Release((LPKSPROPERTYSET)newSource.uservalue3);
            }

            if(newSource.uservalue2)
            {
                IDirectSound3DBuffer_Release((LPDIRECTSOUND3DBUFFER)newSource.uservalue2);
            }

            IDirectSoundBuffer_Release((LPDIRECTSOUNDBUFFER)newSource.uservalue1);
        }

        return AL_FALSE;
    }

    if(SUCCEEDED(IKsPropertySet_QuerySupport(lpPropertySet, &ListenerGuid, ListenerProperty, &ulSupport)))
    {
        if((ulSupport & KSPROPERTY_SUPPORT_GET) && (ulSupport & KSPROPERTY_SUPPORT_SET))
        {
            if(SUCCEEDED(IKsPropertySet_QuerySupport(lpPropertySet, &BufferGuid, BufferProperty, &ulSupport)))
            {
                if((ulSupport & KSPROPERTY_SUPPORT_GET) && (ulSupport & KSPROPERTY_SUPPORT_SET))
                {
                    // Fully supported !
                    bEAXSupported = AL_TRUE;

                    // Set Default Property
                    if(value != 0xFFFFFFFF)
                        IKsPropertySet_Set(lpPropertySet, &ListenerGuid, property, NULL, 0, &value, sizeof(ALint));
                    else
                        IKsPropertySet_Set(lpPropertySet, &ListenerGuid, property, NULL, 0, &fvalue, sizeof(ALfloat));
                }
            }
        }
    }

    if(newSource.uservalue1)
    {
        if(newSource.uservalue3)
        {
            IKsPropertySet_Release((LPKSPROPERTYSET)newSource.uservalue3);
        }

        if(newSource.uservalue2)
        {
            IDirectSound3DBuffer_Release((LPDIRECTSOUND3DBUFFER)newSource.uservalue2);
        }

        IDirectSoundBuffer_Release((LPDIRECTSOUNDBUFFER)newSource.uservalue1);
    }

    return bEAXSupported;
}


/*
        EAXGet(propertySetID, property, source, value, size)

        propertySetID : GUID of EAX Property Set (defined in eax.h files)
        property          : Property in Property Set to retrieve (enumerations defined in eax.h files)
        source            : Source to use to retrieve EAX affect (this can be NULL for Listener Property Sets)
        value             : Pointer to memory location to retrieve value
        size              : Size of data pointed to by value

        Returns AL_INVALID_NAME if a valid Source name was required but not given
        Returns AL_INVALID_OPERATION if the Source is 2D
        Returns AL_INVALID_VALUE if the GUID is not recognized
*/
ALenum EAXGet(const GUID* propertySetID, ALuint property, ALuint source, ALvoid* value, ALuint size)
{
    ALsource*  alSource;
    ALcontext* alContext;
    ALuint     ulBytes;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return AL_INVALID_OPERATION;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);


    if(IsBadWritePtr(value, sizeof(size)))
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_OPERATION;
    }

    // Make sure we are using Direct Sound.
    if(!alContext->Device->DShandle)
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_OPERATION;
    }

    // Determine which EAX Property is being used
    if(!IsEqualGUID(propertySetID, &DSPROPSETID_EAX20_BufferProperties) &&
       !IsEqualGUID(propertySetID, &DSPROPSETID_EAX20_ListenerProperties))
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_VALUE;
    }

    // Process EAX related calls.  We don't separate our the buffer calls from the listener calls because
    // listener calls should only be made on 3D buffers and if we don't have a 3D buffer, creating one won't
    // help because as soon as we close the listener, the reverb on the listener would go away!
    if(!alIsSourceSafe(alContext, source))
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_NAME;
    }

    alSource = (ALsource*)source;
    if(!alSource->uservalue3)
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_OPERATION;
    }

    IKsPropertySet_Get((LPKSPROPERTYSET)alSource->uservalue3, propertySetID, property, NULL, 0,
                       value, size, &ulBytes);
    LeaveCriticalSection(&alContext->Lock);
    return AL_NO_ERROR;
}


/*
        EAXSet(propertySetID, property, source, value, size)

        propertySetID : GUID of EAX Property Set (defined in eax.h files)
        property          : Property in Property Set to affect (enumerations defined in eax.h files)
        source            : Source to apply EAX affects to (this can be NULL for Listener Property Sets)
        value             : Pointer to value to set
        size              : Size of data pointed to by value

        Returns AL_INVALID_NAME if a valid Source name was required but not given
        Returns AL_INVALID_OPERATION if the Source is 2D
        Returns AL_INVALID_VALUE if the GUID is not recognized
*/
ALenum EAXSet(const GUID* propertySetID, ALuint property, ALuint source, ALvoid* value, ALuint size)
{
    ALsource*  alSource;
    ALcontext* alContext;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return AL_INVALID_OPERATION;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);


    if(IsBadReadPtr(value, sizeof(size)))
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_OPERATION;
    }

    // Make sure we are using Direct Sound.
    if(!alContext->Device->DShandle)
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_OPERATION;
    }

    // Determine which EAX Property is being used
    if(!IsEqualGUID(propertySetID, &DSPROPSETID_EAX20_BufferProperties) &&
       !IsEqualGUID(propertySetID, &DSPROPSETID_EAX20_ListenerProperties))
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_VALUE;
    }

    // Process EAX related calls.  We don't separate our the buffer calls from the listener calls because
    // listener calls should only be made on 3D buffers and if we don't have a 3D buffer, creating one won't
    // help because as soon as we close the listener, the reverb on the listener would go away!
    if(!alIsSourceSafe(alContext, source))
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_NAME;
    }

    alSource = (ALsource*)source;
    if(!alSource->uservalue3)
    {
        LeaveCriticalSection(&alContext->Lock);
        return AL_INVALID_OPERATION;
    }

    IKsPropertySet_Set((LPKSPROPERTYSET)alSource->uservalue3, propertySetID, property, NULL, 0, value, size);
    LeaveCriticalSection(&alContext->Lock);
    return AL_NO_ERROR;
}
