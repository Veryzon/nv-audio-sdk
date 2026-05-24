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
#include <al\alc.h>
#include "OpenAL32.h"
#include "alError.h"
#include "alState.h"

static ALubyte alVendor[] = "Standard Implementation";
static ALubyte alVersion[] = "OpenAL 1.0";
static ALubyte alRenderer[] = "Software";
static ALubyte alExtensions[] = "EAX 2.0";

// Error Messages
static ALubyte alNoError[] = "No Error";
static ALubyte alErrInvalidName[] = "Invalid Name";
static ALubyte alErrInvalidEnum[] = "Invalid Enum";
static ALubyte alErrInvalidValue[] = "Invalid Value";
static ALubyte alErrInvalidOp[] = "Invalid Operation";
static ALubyte alErrOutOfMemory[] = "Out of Memory";


ALAPI ALvoid ALAPIENTRY alEnable(ALenum capability)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alEnable(capability);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(capability)
    {
        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alDisable(ALenum capability)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alDisable(capability);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(capability)
    {
        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALboolean ALAPIENTRY alIsEnabled(ALenum capability)
{
    ALcontext* alContext;
    ALboolean result = AL_FALSE;

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
        result = alContext->Device->AlApi.alIsEnabled(capability);
        LeaveCriticalSection(&alContext->Lock);
        return result;
    }


    switch(capability)
    {
        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
    return result;
}


ALAPI ALboolean ALAPIENTRY alGetBoolean(ALenum param)
{
    ALcontext* alContext;
    ALboolean value = AL_FALSE;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return value;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        value = alContext->Device->AlApi.alGetBoolean(param);
        LeaveCriticalSection(&alContext->Lock);
        return value;
    }


    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            value = (alContext->DopplerFactor != 0.0f);
            break;

        case AL_DOPPLER_VELOCITY:
            value = (alContext->DopplerVelocity != 0.0f);
            break;

        case AL_DISTANCE_MODEL:
            value = (alContext->DistanceModel == AL_INVERSE_DISTANCE_CLAMPED);
            break;

        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
    return value;
}


ALAPI ALdouble ALAPIENTRY alGetDouble(ALenum param)
{
    ALcontext* alContext;
    ALdouble value = 0.0;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return value;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        value = alContext->Device->AlApi.alGetDouble(param);
        LeaveCriticalSection(&alContext->Lock);
        return value;
    }


    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            value = (double)alContext->DopplerFactor;
            break;

        case AL_DOPPLER_VELOCITY:
            value = (double)alContext->DopplerVelocity;
            break;

        case AL_DISTANCE_MODEL:
            value = (double)alContext->DistanceModel;
            break;

        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
    return value;
}


ALAPI ALfloat ALAPIENTRY alGetFloat(ALenum param)
{
    ALcontext* alContext;
    ALfloat value = 0.0f;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return value;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        value = alContext->Device->AlApi.alGetFloat(param);
        LeaveCriticalSection(&alContext->Lock);
        return value;
    }


    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            value = alContext->DopplerFactor;
            break;

        case AL_DOPPLER_VELOCITY:
            value = alContext->DopplerVelocity;
            break;

        case AL_DISTANCE_MODEL:
            value = (ALfloat)alContext->DistanceModel;
            break;

        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
    return value;
}


ALAPI ALint ALAPIENTRY alGetInteger(ALenum param)
{
    ALcontext* alContext;
    ALint value = 0;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return value;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        value = alContext->Device->AlApi.alGetInteger(param);
        LeaveCriticalSection(&alContext->Lock);
        return value;
    }


    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            value = (ALint)alContext->DopplerFactor;
            break;

        case AL_DOPPLER_VELOCITY:
            value = (ALint)alContext->DopplerVelocity;
            break;

        case AL_DISTANCE_MODEL:
            value= (ALint)alContext->DistanceModel;
            break;

        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
    return value;
}


ALAPI ALvoid ALAPIENTRY alGetBooleanv(ALenum param, ALboolean* data)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alGetBooleanv(param, data);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(IsBadWritePtr(data, sizeof(ALboolean)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            *data = (alContext->DopplerFactor != 0.0f);
            break;

        case AL_DOPPLER_VELOCITY:
            *data = (alContext->DopplerVelocity != 0.0f);
            break;

        case AL_DISTANCE_MODEL:
            *data = (alContext->DistanceModel == AL_INVERSE_DISTANCE_CLAMPED);
            break;

        default:
            *data = AL_FALSE;
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetDoublev(ALenum param, ALdouble* data)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alGetDoublev(param, data);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(IsBadWritePtr(data, sizeof(ALdouble)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            *data = (double)alContext->DopplerFactor;
            break;

        case AL_DOPPLER_VELOCITY:
            *data = (double)alContext->DopplerVelocity;
            break;

        case AL_DISTANCE_MODEL:
            *data = (double)alContext->DistanceModel;
            break;

        default:
            *data = 0.0;
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetFloatv(ALenum param, ALfloat* data)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alGetFloatv(param, data);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(IsBadWritePtr(data, sizeof(ALfloat)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            *data = alContext->DopplerFactor;
            break;

        case AL_DOPPLER_VELOCITY:
            *data = alContext->DopplerVelocity;
            break;

        case AL_DISTANCE_MODEL:
            *data = (ALfloat)alContext->DistanceModel;
            break;

        default:
            *data = 0.0f;
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetIntegerv(ALenum param, ALint* data)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alGetIntegerv(param, data);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(IsBadWritePtr(data, sizeof(ALint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    switch(param)
    {
        case AL_DOPPLER_FACTOR:
            *data = (ALint)alContext->DopplerFactor;
            break;

        case AL_DOPPLER_VELOCITY:
            *data = (ALint)alContext->DopplerVelocity;
            break;

        case AL_DISTANCE_MODEL:
            *data = (ALint)alContext->DistanceModel;
            break;

        default:
            *data = 0;
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALubyte* ALAPIENTRY alGetString(ALenum param)
{
    ALcontext* alContext;
    ALubyte* value = 0;

    alListAcquireLock(alcContextList);
    if(!alcCurrentContext)
    {
        alListReleaseLock(alcContextList);
        return value;
    }

    alContext = alcCurrentContext;
    EnterCriticalSection(&alContext->Lock);
    alListReleaseLock(alcContextList);

    if(alContext->DllContext)
    {
        value = alContext->Device->AlApi.alGetString(param);
        LeaveCriticalSection(&alContext->Lock);
        return value;
    }


    switch(param)
    {
        case AL_VENDOR:
            value = alVendor;
            break;

        case AL_VERSION:
            value = alVersion;
            break;

        case AL_RENDERER:
            value = alRenderer;
            break;

        case AL_EXTENSIONS:
            value = alExtensions;
            break;

        case AL_NO_ERROR:
            value = alNoError;
            break;

        case AL_INVALID_NAME:
            value = alErrInvalidName;
            break;

        case AL_INVALID_ENUM:
            value = alErrInvalidEnum;
            break;

        case AL_INVALID_VALUE:
            value = alErrInvalidValue;
            break;

        case AL_INVALID_OPERATION:
            value = alErrInvalidOp;
            break;

        case AL_OUT_OF_MEMORY:
            value = alErrOutOfMemory;
            break;

        default:
            alContext->LastError = AL_INVALID_ENUM;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
    return value;
}


ALAPI ALvoid ALAPIENTRY alDopplerFactor(ALfloat value)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alDopplerFactor(value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(value >= 0.0f)
    {
        alContext->DopplerFactor=value;
        alContext->Listener.update1 = LDOPPLERFACTOR;
        alcUpdateContext(alContext, ALLISTENER, 0);
    }

    else
        alContext->LastError = AL_INVALID_VALUE;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alDopplerVelocity(ALfloat value)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alDopplerVelocity(value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(value > 0.0f)
    {
        alContext->DopplerVelocity=value;
        alContext->Listener.update1 = LDOPPLERVELOCITY;
        alcUpdateContext(alContext, ALLISTENER, 0);
    }

    else
        alContext->LastError = AL_INVALID_VALUE;

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alDistanceModel(ALenum value)
{
    ALcontext* alContext;

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
        alContext->Device->AlApi.alDistanceModel(value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(value)
    {
        case AL_NONE:
        case AL_INVERSE_DISTANCE:
        case AL_INVERSE_DISTANCE_CLAMPED:
            alContext->DistanceModel = value;
            alContext->Listener.update1 = LDISTANCEMODEL;
            alcUpdateContext(alContext, ALLISTENER, 0);
            break;

        default:
            alContext->LastError = AL_INVALID_VALUE;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}
