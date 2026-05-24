#ifndef _ALTYPES_H_
#define _ALTYPES_H_

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


#ifdef __cplusplus
extern "C" {
#endif

/** OpenAL boolean type. */
typedef char ALboolean;
#define AL_FALSE        0
#define AL_TRUE         1

/** OpenAL 8bit signed byte. */
typedef char ALbyte;

/** OpenAL 8bit unsigned byte. */
typedef unsigned char ALubyte;

/** OpenAL 16bit signed short integer type. */
typedef short ALshort;

/** OpenAL 16bit unsigned short integer type. */
typedef unsigned short ALushort;

/** OpenAL 32bit unsigned integer type. */
typedef unsigned ALuint;

/** OpenAL 32bit signed integer type. */
typedef int ALint;

/** OpenAL 32bit floating point type. */
typedef float ALfloat;

/** OpenAL 64bit double point type. */
typedef double ALdouble;

/** OpenAL 32bit type. */
typedef unsigned int ALsizei;

/** OpenAL void type */
typedef void ALvoid;

/** OpenAL enumerations. */
typedef int ALenum;

/** OpenAL vector. */
typedef struct
{
    ALfloat x;
    ALfloat y;
    ALfloat z;

} ALvector;

//
// If you are wondering where the #define'd values went, you should not be
// using them directly but should be querying for the values using alGetEnumValue!
//


#ifdef __cplusplus
}
#endif

#endif
