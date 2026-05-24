//*****************************************************************************
//*****************************************************************************
//
// encode.cpp
//
//
// Description:
// Encodes a WAV file to IMA ADPCM with 64 samples per block.
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




#include <windows.h>

#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imaadpcm.h"

//
// Standard IMA uses this for the block size and alignment from the apps I
// tried (GoldWave and SoundForge).
//
//#define SAMPLES_PER_BLOCK 2041
//#define BLOCKALIGN        1024

//
// This are the values for the ADPCM version supported by NVIDIA.
//
#define SAMPLES_PER_BLOCK 64
#define BLOCKALIGN        36

#define FAIL_ERR(str) \
  {                   \
    printf(str);      \
    return -1;        \
  }

#define FAIL_ERR_RESULT(str, result) \
  {                                  \
    printf(str, result);             \
    return -1;                       \
  }

int main(int argc, char *argv[])
  {
    char fname_i[256] = "";
    char fname_o[256] = "";

   // validate arguments
    switch (argc)
      {
        case 3:
          strcpy(fname_i, argv[1]);
          strcpy(fname_o, argv[2]);
          break;

        default:
          FAIL_ERR("Syntax: %s <infile.wav> <outfile.wav> \n");
      }

   // open files
    HMMIO fin = NULL;
    HMMIO fout = NULL;

    fin  = mmioOpen(fname_i, NULL, MMIO_READ);
    fout = mmioOpen(fname_o, NULL, MMIO_WRITE|MMIO_CREATE);

    if(!fin)
        FAIL_ERR("Error: can't open input file \n");

    if(!fout)
      FAIL_ERR("Error: can't create output file \n");

   // descend into RIFF chunk of input file
    MMCKINFO mmckRIFF;
    mmckRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(fin, &mmckRIFF, NULL, MMIO_FINDRIFF))
      FAIL_ERR("Error: can't find RIFF WAVE chunk \n");

   // find RIFF format chunk
    MMCKINFO mmckFMT;
    mmckFMT.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(fin, &mmckFMT, &mmckFMT, MMIO_FINDCHUNK))
      FAIL_ERR("Error: can't find fmt chunk \n");

   // load format chunk
    int s = sizeof(WAVEFORMATEX);
    WAVEFORMATEXTENSIBLE* pwfxIn = (WAVEFORMATEXTENSIBLE*)malloc(mmckFMT.cksize);
    if(!pwfxIn)
      FAIL_ERR("Error: can't allocate memory for fmt chunk \n");

    mmioRead(fin, (char *)pwfxIn, mmckFMT.cksize);

   // verify we have a PCM file
    if(!(pwfxIn->Format.wFormatTag == WAVE_FORMAT_PCM ||
         (pwfxIn->Format.wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
          pwfxIn->SubFormat == KSDATAFORMAT_SUBTYPE_PCM)))
      FAIL_ERR("Error: not a PCM file \n");

   // ascend out of format chunk
    mmioAscend(fin, &mmckFMT, 0);

   // find the data chunk
    MMCKINFO mmckDATA;
    mmckDATA.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(fin, &mmckDATA, &mmckRIFF, MMIO_FINDCHUNK))
      FAIL_ERR("Error: can't find data chunk \n");

   // load data
    DWORD cbSrc = mmckDATA.cksize;
    BYTE *pbSrc = (BYTE *)malloc(cbSrc);
    if(!pbSrc)
      FAIL_ERR("Error: can't find allocate memory for data chunk \n");

    mmioRead(fin, (char *)pbSrc, mmckDATA.cksize);

   // create RIFF WAVE chunk in output file
    MMCKINFO ckOutRIFF;
    ckOutRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioCreateChunk(fout, &ckOutRIFF, MMIO_CREATERIFF))
      FAIL_ERR("Error: can't create RIFF chunk in output file \n");


    //
    // This creates a "standard" IMA ADPCM file.
    //
/*
    IMAADPCMWAVEFORMAT wfOut;
    MMCKINFO ckOut;
    ckOut.ckid = mmioFOURCC('f', 'm', 't', ' ');
    ckOut.cksize = sizeof(wfOut);
    if (mmioCreateChunk(fout, &ckOut, 0) != 0)
      FAIL_ERR("Error: can't create fmt chunk in output file \n");

    wfOut.wfx.wFormatTag      = WAVE_FORMAT_IMA_ADPCM;
    wfOut.wfx.wBitsPerSample  = 4;
    wfOut.wfx.nSamplesPerSec  = pwfxIn->Format.nSamplesPerSec;
    wfOut.wfx.nChannels       = pwfxIn->Format.nChannels;
    wfOut.wfx.nBlockAlign     = (BLOCKALIGN * wfOut.wfx.nChannels);//512;
    wfOut.wfx.nAvgBytesPerSec = wfOut.wfx.nSamplesPerSec*wfOut.wfx.nBlockAlign/SAMPLES_PER_BLOCK;
    wfOut.wfx.cbSize          = sizeof(wfOut.wSamplesPerBlock);
    wfOut.wSamplesPerBlock    = SAMPLES_PER_BLOCK + 1;//1017;
    mmioWrite(fout, (HPSTR)&wfOut, sizeof(wfOut));
*/

    //
    // Setup the custom format.
    //
    WAVEFORMATEXTENSIBLE wfxxx;

    MMCKINFO ckOut;
    ckOut.ckid = mmioFOURCC('f', 'm', 't', ' ');
    ckOut.cksize = sizeof(wfxxx);
    if (mmioCreateChunk(fout, &ckOut, 0) != 0)
      FAIL_ERR("Error: can't create fmt chunk in output file \n");

    wfxxx.Format.cbSize = (WORD)sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
    wfxxx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    wfxxx.Format.nSamplesPerSec = pwfxIn->Format.nSamplesPerSec;
    wfxxx.Format.wBitsPerSample = 4;
    wfxxx.Format.nChannels = (WORD)pwfxIn->Format.nChannels;
    wfxxx.Format.nBlockAlign = (WORD)(BLOCKALIGN * wfxxx.Format.nChannels);
    wfxxx.Format.nAvgBytesPerSec = wfxxx.Format.nSamplesPerSec*wfxxx.Format.nBlockAlign/SAMPLES_PER_BLOCK;

    wfxxx.Samples.wValidBitsPerSample = 4;
    wfxxx.dwChannelMask = 3;
    wfxxx.SubFormat = NVDATAFORMAT_SUBTYPE_ADPCM;
    mmioWrite(fout, (HPSTR)&wfxxx, sizeof(wfxxx));


    mmioAscend(fout, &ckOut, 0);

   // xxxxxx - we're supposed to put in an info chunk here

   // create data chunk
    ckOut.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioCreateChunk(fout, &ckOut, 0))
      FAIL_ERR("Error: can't create data chunk in output file \n");

   // allocate destination buffer
//    int  cbDst = (cbSrc/pwfxIn->nBlockAlign / wfOut.wSamplesPerBlock  + 1) * wfOut.wfx.nBlockAlign;
    int cbDst = cbSrc; // xxxx - this is a waste of memory
    BYTE *pbDst = (BYTE *)malloc(cbDst);

   // pick conversion type
    DWORD (*fnEncode)(HPBYTE,DWORD,HPBYTE,UINT,UINT,int*,int*) = 0;
    if(pwfxIn->Format.nChannels == 1 && pwfxIn->Format.wBitsPerSample == 8)
      fnEncode = imaadpcmEncode4Bit_M08;
    else if (pwfxIn->Format.nChannels == 1 && pwfxIn->Format.wBitsPerSample == 16)
      fnEncode = imaadpcmEncode4Bit_M16;
    else if (pwfxIn->Format.nChannels == 2 && pwfxIn->Format.wBitsPerSample == 8)
      fnEncode = imaadpcmEncode4Bit_S08;
    else if (pwfxIn->Format.nChannels == 2 && pwfxIn->Format.wBitsPerSample == 16)
      fnEncode = imaadpcmEncode4Bit_S16;
    if(!fnEncode)
      FAIL_ERR("Error: Unsupported input format \n");

   // encode
    int nStepIndexL = 0;
    int nStepIndexR = 0;

    int cbDstUsed;
    cbDstUsed = fnEncode(
                         pbSrc,
                         cbSrc,
                         pbDst,
                         -1, SAMPLES_PER_BLOCK,
                         &nStepIndexL, &nStepIndexR
                        );

    if (cbDstUsed > cbDst)
      FAIL_ERR("Error: Output buffer overflow \n");

   // write output to file
    mmioWrite(fout, (HPSTR)pbDst, cbDstUsed);

   // finish up chunks in output file
    mmioAscend(fout, &ckOut, 0);
    mmioAscend(fout, &ckOutRIFF, 0);

   // close files
    mmioClose(fin,  0);
    mmioClose(fout, 0);

   // free buffers
    free(pbSrc);
    free(pbDst);

    return 0;
  }
