//*****************************************************************************
//*****************************************************************************
//
// wavefile.cpp
//
//
// Description:
// This file defines some common functions to help setup OpenAL.
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




#ifndef _WAVEFILE_H_
#define _WAVEFILE_H_


#include <windows.h>
#include <mmsystem.h>


//*****************************************************************************
// CWaveFile
//*****************************************************************************
//
class CWaveFile
{

public:

    CWaveFile();
    ~CWaveFile();
    HRESULT LoadFile(CHAR* szFilename);
    DWORD GetFileSize()         { return m_dwFileSize; }
    DWORD GetFormatSize()       { return m_dwFormatSize; }
    DWORD GetWaveFormat()       { return m_dwWaveFormat; }
    DWORD GetSampleRate()       { return m_dwSampleRate; }
    DWORD GetBitsPerSample()    { return m_dwBitsPerSample; }
    DWORD GetChannels()         { return m_dwChannels; }
    DWORD GetAvgBytesPerSec()   { return m_dwAvgBytesPerSec; }
    DWORD GetBlockAlign()       { return m_dwBlockAlign; }
    VOID* GetDataBuffer()       { return m_pDataBuffer; }
    DWORD GetDataSize()         { return m_dwDataSize; }
    char* GetFormatBuffer()     { return m_pFormatBuffer; }
    DWORD GetExtraSize()        { return m_dwExtraSize; }
    GUID  GetSubFormat()        { return m_guidSubFormat; }


private:

    DWORD m_dwFileSize;
    DWORD m_dwWaveFormat;
    DWORD m_dwSampleRate;
    DWORD m_dwBitsPerSample;
    DWORD m_dwChannels;
    DWORD m_dwAvgBytesPerSec;
    DWORD m_dwBlockAlign;
    DWORD m_dwExtraSize;

    char* m_pFormatBuffer;
    DWORD m_dwFormatSize;
    char* m_pDataBuffer;
    DWORD m_dwDataSize;

    GUID  m_guidSubFormat;

};

#endif
