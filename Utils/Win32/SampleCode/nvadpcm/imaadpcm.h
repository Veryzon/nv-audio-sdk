//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1992-1994 Microsoft Corporation
//
//--------------------------------------------------------------------------;
//
//  imaadpcm.h
//
//  Description:
//      This file contains prototypes for the filtering routines.
//
//
//==========================================================================;

#ifndef _IMAADPCM_H_
#define _IMAADPCM_H_

//////// begin porting hack
#define HPBYTE PBYTE
//typedef unsigned char *PBYTE;

#define FNGLOBAL
#define HUGE_T

#include <assert.h>

#define ASSERT(x) assert(x)

void DPF(int x, char *y,...) { }
//////// end porting hack

#ifndef RC_INVOKED
#pragma pack(1)                     // assume byte packing throughout
#endif

#ifndef EXTERN_C
#ifdef __cplusplus
    #define EXTERN_C extern "C"
#else
    #define EXTERN_C extern 
#endif
#endif




// Definitions added by JDW
#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct {
    WAVEFORMATEX    Format;
    union {
        WORD wValidBitsPerSample;       /* bits of precision  */
        WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
        WORD wReserved;                 /* If neither applies, set to zero. */
    } Samples;
    DWORD           dwChannelMask;      /* which channels are */
                                        /* present in stream  */
    GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

#if !defined(WAVE_FORMAT_EXTENSIBLE)
#define  WAVE_FORMAT_EXTENSIBLE                 0xFFFE
#endif // !defined(WAVE_FORMAT_EXTENSIBLE)

#if defined(__cplusplus) && _MSC_VER >= 1100
#define DEFINE_GUIDSTRUCT(g, n) struct __declspec(uuid(g)) n
#define DEFINE_GUIDNAMED(n) __uuidof(struct n)
#else // !defined(__cplusplus)
#define DEFINE_GUIDSTRUCT(g, n) DEFINE_GUIDEX(n)
#define DEFINE_GUIDNAMED(n) n
#endif // !defined(__cplusplus)

#define STATIC_KSDATAFORMAT_SUBTYPE_PCM\
    DEFINE_WAVEFORMATEX_GUID(WAVE_FORMAT_PCM)
DEFINE_GUIDSTRUCT("00000001-0000-0010-8000-00aa00389b71", KSDATAFORMAT_SUBTYPE_PCM);
#define KSDATAFORMAT_SUBTYPE_PCM DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_PCM)


#define STATIC_KSDATAFORMAT_SUBTYPE_ADPCM\
    DEFINE_WAVEFORMATEX_GUID(WAVE_FORMAT_ADPCM)
DEFINE_GUIDSTRUCT("00000002-0000-0010-8000-00aa00389b71", KSDATAFORMAT_SUBTYPE_ADPCM);
#define KSDATAFORMAT_SUBTYPE_ADPCM DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_ADPCM)



DEFINE_GUIDSTRUCT("89896454-A38A-4B32-9516-D8D82515BA1C", NVDATAFORMAT_SUBTYPE_ADPCM);
#define NVDATAFORMAT_SUBTYPE_ADPCM DEFINE_GUIDNAMED(NVDATAFORMAT_SUBTYPE_ADPCM)



#ifdef __cplusplus
extern "C"                          // assume C declarations for C++
{
#endif


//
//
//
//
#define IMAADPCM_MAX_CHANNELS       2
#define IMAADPCM_BITS_PER_SAMPLE    4
#define IMAADPCM_WFX_EXTRA_BYTES    (sizeof(IMAADPCMWAVEFORMAT) - sizeof(WAVEFORMATEX))
#define IMAADPCM_HEADER_LENGTH      4    // In bytes, per channel.

#ifdef IMAADPCM_USECONFIG
#define IMAADPCM_CONFIGTESTTIME     4   // seconds of PCM data for test.
#define IMAADPCM_CONFIG_DEFAULT                             0x0000
#define IMAADPCM_CONFIG_DEFAULT_MAXRTENCODESETTING          5
#define IMAADPCM_CONFIG_DEFAULT_MAXRTDECODESETTING          6
#define IMAADPCM_CONFIG_UNCONFIGURED                        0x0999
#define IMAADPCM_CONFIG_DEFAULT_PERCENTCPU	        	    50
#define IMAADPCM_CONFIG_TEXTLEN                             80
#define IMAADPCM_CONFIG_DEFAULTKEY                          HKEY_CURRENT_USER
#endif


//
//  Conversion function prototypes.
//
DWORD FNGLOBAL imaadpcmDecode4Bit_M08
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);

DWORD FNGLOBAL imaadpcmDecode4Bit_M16
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);

DWORD FNGLOBAL imaadpcmDecode4Bit_S08
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);

DWORD FNGLOBAL imaadpcmDecode4Bit_S16
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);

DWORD FNGLOBAL imaadpcmEncode4Bit_M08
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);

DWORD FNGLOBAL imaadpcmEncode4Bit_M16
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);

DWORD FNGLOBAL imaadpcmEncode4Bit_S08
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);

DWORD FNGLOBAL imaadpcmEncode4Bit_S16
(
    HPBYTE                  pbSrc,
    DWORD                   cbSrcLength,
    HPBYTE                  pbDst,
    UINT                    nBlockAlignment,
    UINT                    cSamplesPerBlock,
    int                 *   pnStepIndexL,
    int                 *   pnStepIndexR
);


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ; 
//
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ; 

#ifndef RC_INVOKED
#pragma pack()                      // revert to default packing
#endif

#ifdef __cplusplus
}                                   // end of extern "C" { 
#endif

#endif // _IMAADPCM_H_

