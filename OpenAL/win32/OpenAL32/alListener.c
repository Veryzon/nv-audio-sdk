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




#include <al\alc.h>
#include "OpenAL32.h"
#include "alError.h"
#include "alListener.h"


ALAPI ALvoid ALAPIENTRY alListenerf(ALenum param, ALfloat value)
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
        alContext->Device->AlApi.alListenerf(param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(param)
    {
        case AL_GAIN:
            if(value >= 0.0f)
            {
                alContext->Listener.Gain = value;
                alContext->Listener.update1 |= LVOLUME;
                alcUpdateContext(alContext, ALLISTENER, 0);
            }

            else
                alContext->LastError = AL_INVALID_VALUE;

            break;

        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alListener3f(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3)
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
        alContext->Device->AlApi.alListener3f(param, v1, v2, v3);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(param)
    {
        case AL_POSITION:
            alContext->Listener.Position[0] = v1;
            alContext->Listener.Position[1] = v2;
            alContext->Listener.Position[2] = v3;
            alContext->Listener.update1 |= LPOSITION;
            alcUpdateContext(alContext, ALLISTENER, 0);
            break;

        case AL_VELOCITY:
            alContext->Listener.Velocity[0] = v1;
            alContext->Listener.Velocity[1] = v2;
            alContext->Listener.Velocity[2] = v3;
            alContext->Listener.update1 |= LVELOCITY;
            alcUpdateContext(alContext, ALLISTENER, 0);
            break;

        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alListenerfv(ALenum param, ALfloat* values)
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
        alContext->Device->AlApi.alListenerfv(param, values);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(param)
    {
        case AL_POSITION:
            if(IsBadReadPtr(values, sizeof(ALfloat) * 3))
            {
                alContext->LastError = AL_INVALID_VALUE;
                LeaveCriticalSection(&alContext->Lock);
                return;
            }

            alContext->Listener.Position[0] = values[0];
            alContext->Listener.Position[1] = values[1];
            alContext->Listener.Position[2] = values[2];
            alContext->Listener.update1 |= LPOSITION;
            alcUpdateContext(alContext, ALLISTENER, 0);
            break;

        case AL_VELOCITY:
            if(IsBadReadPtr(values, sizeof(ALfloat) * 3))
            {
                alContext->LastError = AL_INVALID_VALUE;
                LeaveCriticalSection(&alContext->Lock);
                return;
            }

            alContext->Listener.Velocity[0] = values[0];
            alContext->Listener.Velocity[1] = values[1];
            alContext->Listener.Velocity[2] = values[2];
            alContext->Listener.update1 |= LVELOCITY;
            alcUpdateContext(alContext, ALLISTENER, 0);
            break;

        case AL_ORIENTATION:
            // AT then UP
            if(IsBadReadPtr(values, sizeof(ALfloat) * 6))
            {
                alContext->LastError = AL_INVALID_VALUE;
                LeaveCriticalSection(&alContext->Lock);
                return;
            }

            alContext->Listener.Forward[0] = values[0];
            alContext->Listener.Forward[1] = values[1];
            alContext->Listener.Forward[2] = values[2];
            alContext->Listener.Up[0] = values[3];
            alContext->Listener.Up[1] = values[4];
            alContext->Listener.Up[2] = values[5];
            alContext->Listener.update1 |= LORIENTATION;
            alcUpdateContext(alContext, ALLISTENER, 0);
            break;

        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alListeneri(ALenum param, ALint value)
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
        alContext->Device->AlApi.alListeneri(param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(param)
    {
        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetListenerf(ALenum param, ALfloat* value)
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
        alContext->Device->AlApi.alGetListenerf(param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(IsBadWritePtr(value, sizeof(ALfloat)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    switch(param)
    {
        case AL_GAIN:
            *value = alContext->Listener.Gain;
            break;

        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetListener3f(ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3)
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
        alContext->Device->AlApi.alGetListener3f(param, v1, v2, v3);
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

    switch(param)
    {
        case AL_POSITION:
            *v1 = alContext->Listener.Position[0];
            *v2 = alContext->Listener.Position[1];
            *v3 = alContext->Listener.Position[2];
            break;

        case AL_VELOCITY:
            *v1 = alContext->Listener.Velocity[0];
            *v2 = alContext->Listener.Velocity[1];
            *v3 = alContext->Listener.Velocity[2];
            break;

        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetListenerfv(ALenum param, ALfloat* values)
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
        alContext->Device->AlApi.alGetListenerfv(param, values);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    switch(param)
    {
        case AL_POSITION:
            if(IsBadWritePtr(values, sizeof(ALfloat) * 3))
            {
                alContext->LastError = AL_INVALID_VALUE;
                LeaveCriticalSection(&alContext->Lock);
                return;
            }

            values[0] = alContext->Listener.Position[0];
            values[1] = alContext->Listener.Position[1];
            values[2] = alContext->Listener.Position[2];
            break;

        case AL_VELOCITY:
            if(IsBadWritePtr(values, sizeof(ALfloat) * 3))
            {
                alContext->LastError = AL_INVALID_VALUE;
                LeaveCriticalSection(&alContext->Lock);
                return;
            }

            values[0] = alContext->Listener.Velocity[0];
            values[1] = alContext->Listener.Velocity[1];
            values[2] = alContext->Listener.Velocity[2];
            break;

        case AL_ORIENTATION:
            // AT then UP
            if(IsBadWritePtr(values, sizeof(ALfloat) * 6))
            {
                alContext->LastError = AL_INVALID_VALUE;
                LeaveCriticalSection(&alContext->Lock);
                return;
            }

            values[3] = alContext->Listener.Up[0];
            values[4] = alContext->Listener.Up[1];
            values[5] = alContext->Listener.Up[2];
            values[0] = alContext->Listener.Forward[0];
            values[1] = alContext->Listener.Forward[1];
            values[2] = alContext->Listener.Forward[2];
            break;

        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}


ALAPI ALvoid ALAPIENTRY alGetListeneri(ALenum param, ALint* value)
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
        alContext->Device->AlApi.alGetListeneri(param, value);
        LeaveCriticalSection(&alContext->Lock);
        return;
    }


    if(IsBadWritePtr(value, sizeof(ALint)))
    {
        alContext->LastError = AL_INVALID_VALUE;
        LeaveCriticalSection(&alContext->Lock);
        return;
    }

    switch(param)
    {
        default:
            alContext->LastError = AL_INVALID_OPERATION;
            break;
    }

    LeaveCriticalSection(&alContext->Lock);
}
