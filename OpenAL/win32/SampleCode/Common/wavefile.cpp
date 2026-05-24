//*****************************************************************************
//*****************************************************************************
//
// wavefile.cpp
//
//
// Description:
// This file implements a class to help setup wave files.
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
#include "wavefile.h"
#include <stdio.h>


//*****************************************************************************
// CWaveFile
//*****************************************************************************
//
CWaveFile::CWaveFile()
{
    m_dwFileSize = 0;
    m_dwWaveFormat = 0;
    m_dwSampleRate = 0;
    m_dwBitsPerSample = 0;
    m_dwChannels = 0;
    m_dwAvgBytesPerSec = 0;
    m_dwBlockAlign = 0;
    m_pFormatBuffer = 0;
    m_dwFormatSize = 0;
    m_pDataBuffer = 0;
    m_dwDataSize = 0;
    m_dwExtraSize = 0;
    memset(&m_guidSubFormat, 0, sizeof(GUID));
}


//*****************************************************************************
// ~CWaveFile
//*****************************************************************************
//
CWaveFile::~CWaveFile()
{
    if(m_pDataBuffer)
    {
        GlobalFree(m_pDataBuffer);
        m_pDataBuffer = 0;
    }

    if(m_pFormatBuffer)
    {
        GlobalFree(m_pFormatBuffer);
        m_pFormatBuffer = NULL;
    }
}


//*****************************************************************************
// LoadFile
//*****************************************************************************
//
HRESULT CWaveFile::LoadFile(CHAR* szFilename)
{
    HMMIO       hmmio;              // file handle for open file
    MMCKINFO    mmckinfoParent;     // parent chunk information
    MMCKINFO    mmckinfoSubchunk;   // subchunk information structure

    FILE* fin;
    fpos_t pos;

    if(NULL == (fin = fopen(szFilename, "rb")))
    {
        return E_FAIL;
    }

    fseek(fin, 0, SEEK_END);
    fgetpos(fin, &pos);
    fseek(fin, 0, SEEK_SET);

    m_dwFileSize = (DWORD)pos;

    fclose(fin);

    // Open the file for reading with buffered I/O by using the default internal buffer
    if(!(hmmio = mmioOpen(szFilename, NULL, MMIO_READ | MMIO_ALLOCBUF)))
    {
        return E_FAIL;
    }

    // Locate a "RIFF" chunk with a "WAVE" form type to make sure the file is a waveform-audio file.
    mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if(mmioDescend(hmmio, (LPMMCKINFO) &mmckinfoParent, NULL, MMIO_FINDRIFF))
    {
        mmioClose(hmmio, 0);
        return E_FAIL;
    }

    // Find the "FMT" chunk (form type "FMT"); it must be  a subchunk of the "RIFF" chunk.
    mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, MMIO_FINDCHUNK))
    {
        mmioClose(hmmio, 0);
        return E_FAIL;
    }

    // Get the size of the "FMT" chunk. Allocate and lock memory for it.
    m_dwFormatSize = mmckinfoSubchunk.cksize;

    // allocate a buffer for the format chunk
    m_pFormatBuffer = (char*)GlobalAlloc(GPTR, m_dwFormatSize);

    // Read the "FMT" chunk.
    if (mmioRead(hmmio, (HPSTR)m_pFormatBuffer, m_dwFormatSize) != (long)m_dwFormatSize)
    {
        mmioClose(hmmio, 0);
        return E_FAIL;
    }

    // Ascend out of the "FMT" subchunk.
    mmioAscend(hmmio, &mmckinfoSubchunk, 0);

    // Find the data subchunk.
    mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, MMIO_FINDCHUNK))
    {
        mmioClose(hmmio, 0);
        return E_FAIL;
    }

    // Get the size of the data subchunk.
    m_dwDataSize = mmckinfoSubchunk.cksize;

    if (m_dwDataSize == 0L)
    {
        mmioClose(hmmio, 0);
        return E_FAIL;
    }

    // Allocate and lock memory for the waveform-audio data.
    m_pDataBuffer = (char*)GlobalAlloc(GPTR, m_dwDataSize);

    // Read the waveform-audio data subchunk.
    if(mmioRead(hmmio, (HPSTR) m_pDataBuffer, m_dwDataSize) != (long)m_dwDataSize)
    {
        mmioClose(hmmio, 0);
        return E_FAIL;
    }

    // Close the file.
    mmioClose(hmmio, 0);

    m_dwWaveFormat     = *(WORD*) (m_pFormatBuffer + 0);
    m_dwChannels       = *(WORD*) (m_pFormatBuffer + 2);
    m_dwSampleRate     = *(DWORD*)(m_pFormatBuffer + 4);
    m_dwAvgBytesPerSec = *(DWORD*)(m_pFormatBuffer + 8);
    m_dwBlockAlign     = *(WORD*) (m_pFormatBuffer + 12);
    m_dwBitsPerSample  = *(WORD*) (m_pFormatBuffer + 14);
    m_dwExtraSize      = *(WORD*) (m_pFormatBuffer + 16);

    // Save the SubFormat if extensible
    if(m_dwWaveFormat == 0x0fffe)
        memcpy(&m_guidSubFormat, m_pFormatBuffer+24, sizeof(GUID));

    return NOERROR;
}