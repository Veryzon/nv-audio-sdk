//************************************************************************
// SimpleFxData : A place to store our data.
//
//************************************************************************

 /***************************************************************************\
|*                                                                           *|
|*            Copyright NVIDIA Corporation.  All rights reserved.            *|
|*                                                                           *|
|*     NOTICE TO USER:   The source code  is copyrighted under  U.S. and     *|
|*     international laws.  Users and possessors of this source code are     *|
|*     hereby granted a nonexclusive,  royalty-free copyright license to     *|
|*     use this code in individual and commercial software.                  *|
|*                                                                           *|
|*     Any use of this source code must include,  in the user documenta-     *|
|*     tion and  internal comments to the code,  notices to the end user     *|
|*     as follows:                                                           *|
|*                                                                           *|
|*     NVIDIA CORPORATION MAKES NO REPRESENTATION ABOUT THE  SUITABILITY     *|
|*     OF  THIS SOURCE  CODE  FOR ANY PURPOSE.  IT IS  PROVIDED  "AS IS"     *|
|*     WITHOUT EXPRESS OR IMPLIED WARRANTY OF  ANY KIND.  NVIDIA CORPOR-     *|
|*     ATION DISCLAIMS ALL WARRANTIES  WITH REGARD  TO THIS SOURCE CODE,     *|
|*     INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGE-     *|
|*     MENT,  AND FITNESS  FOR A PARTICULAR PURPOSE.   IN NO EVENT SHALL     *|
|*     NVIDIA CORPORATION  BE LIABLE FOR  ANY SPECIAL,  INDIRECT,  INCI-     *|
|*     DENTAL, OR CONSEQUENTIAL DAMAGES,  OR ANY DAMAGES  WHATSOEVER RE-     *|
|*     SULTING FROM LOSS OF USE,  DATA OR PROFITS,  WHETHER IN AN ACTION     *|
|*     OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT OF     *|
|*     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.     *|
|*                                                                           *|
|*     U.S. Government  End  Users.   This source code  is a "commercial     *|
|*     item,"  as that  term is  defined at  48 C.F.R. 2.101 (OCT 1995),     *|
|*     consisting  of "commercial  computer  software"  and  "commercial     *|
|*     computer  software  documentation,"  as such  terms  are  used in     *|
|*     48 C.F.R. 12.212 (SEPT 1995)  and is provided to the U.S. Govern-     *|
|*     ment only as  a commercial end item.   Consistent with  48 C.F.R.     *|
|*     12.212 and  48 C.F.R. 227.7202-1 through  227.7202-4 (JUNE 1995),     *|
|*     all U.S. Government End Users  acquire the source code  with only     *|
|*     those rights set forth herein.                                        *|
|*                                                                           *|
|*                                                                    Rev A  *|
 \***************************************************************************/

#include "stdafx.h"
#include "SimpleFx.h"

//
// Number of parameters for each effect.
//
DWORD m_NumOfParms[MAX_EFFECTS] = { 0, 7, 6, 5, 5, 7, 2, 3, 12 };

//
// The size of each effects parameter structure (in bytes).
//
DWORD m_FxSize[MAX_EFFECTS] = 
{
    0,
    sizeof(DSFXChorus),
    sizeof(DSFXCompressor),
    sizeof(DSFXDistortion),
    sizeof(DSFXEcho),
    sizeof(DSFXFlanger),
    sizeof(DSFXGargle),
    sizeof(DSFXParamEq),
    sizeof(I3DL2_LISTENERPROPERTIES)
};

//
// The name we display for each effect
//
char m_FxNames[MAX_EFFECTS][EFFECT_STRING_MAX] = 
{
    "None Selected",
    "Chorus",
    "Compressor",
    "Distortion",
    "Echo",
    "Flanger",
    "Gargle",
    "ParamEq",
    "Reverb"
};

//
// Define each type of effect as STEREO or MONO.
//
DWORD m_FxType[MAX_EFFECTS] = 
{ 
    FX_NONE, 
    FX_STEREO, 
    FX_STEREO, 
    FX_MONO, 
    FX_STEREO, 
    FX_STEREO, 
    FX_MONO, 
    FX_MONO, 
    FX_STEREO 
};

//
// Define the GUID for each effect.
//
GUID m_FxGuid[MAX_EFFECTS];
GUID m_DataGuid[MAX_EFFECTS];


//
// String names in same order as D.S. parameter list.
//

char m_ChorusNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Wet/Dry Mix",
    "Depth",
    "Feedback",
    "Frequency",
    "Waveform",
    "Delay",
    "Phase"
};

char m_CompressorNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Gain",
    "Attack",
    "Release",
    "Threshold",
    "Ratio",
    "Predelay",
};

char m_DistortionNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Gain",
    "Edge",
    "Post EQ Frequency",
    "Post EQ Bandwidth",
    "Pre Lowpass Cutoff"
};

char m_EchoNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Wet/Dry Mix",
    "Feedback",
    "Left Delay",
    "Right Delay",
    "Pan Delay"
};

char m_FlangerNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Wet/Dry Mix",
    "Depth",
    "Feedback",
    "Frequency",
    "Waveform",
    "Delay",
    "Phase"
};

char m_GargleNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Rate",
    "Wave Shape"
};

char m_ParamEqNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Center",
    "Bandwidth",
    "Gain"
};

char m_ReverbNames[][MAX_PARAMS_NAME_SIZE] = 
{
    "Room",
    "Room HF",
    "Room Rolloff Factor",
    "Decay Time",
    "Decay HF Ratio",
    "Reflections",
    "Reflections Delay",
    "Reverb",
    "Reverb Delay",
    "Diffusion",
    "Density",
    "HF Reference"
};




char m_ChorusUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_PERCENT, 
    IDS_STRING_UNIT_PERCENT,
    IDS_STRING_UNIT_PERCENT, 
    IDS_STRING_UNIT_HZ,
    IDS_STRING_UNIT_NULL2, 
    IDS_STRING_UNIT_MSEC, 
    IDS_STRING_UNIT_NULL1
};

char m_CompressorUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_DB, 
    IDS_STRING_UNIT_MSEC,
    IDS_STRING_UNIT_MSEC, 
    IDS_STRING_UNIT_DB,
    IDS_STRING_UNIT_RATIO, 
    IDS_STRING_UNIT_MSEC 
};

char m_DistortionUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_DB, 
    IDS_STRING_UNIT_PERCENT, 
    IDS_STRING_UNIT_HZ,
    IDS_STRING_UNIT_HZ, 
    IDS_STRING_UNIT_HZ 
};

char m_EchoUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_PERCENT,
    IDS_STRING_UNIT_PERCENT,
    IDS_STRING_UNIT_MSEC, 
    IDS_STRING_UNIT_MSEC, 
    IDS_STRING_UNIT_NULL4 
};

char m_FlangerUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_PERCENT, 
    IDS_STRING_UNIT_PERCENT,
    IDS_STRING_UNIT_PERCENT, 
    IDS_STRING_UNIT_HZ,
    IDS_STRING_UNIT_NULL2, 
    IDS_STRING_UNIT_MSEC, 
    IDS_STRING_UNIT_NULL1 
};

char m_GargleUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_RATE, 
    IDS_STRING_UNIT_NULL3 
};

char m_ParamEqUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_HZ, 
    IDS_STRING_UNIT_HZ, 
    IDS_STRING_UNIT_DB 
};

char m_ReverbUnits[][MAX_PARAMS_UNIT_SIZE] = 
{
    IDS_STRING_UNIT_MB, 
    IDS_STRING_UNIT_MB, 
    IDS_STRING_UNIT_FACTOR, 
    IDS_STRING_UNIT_SEC,
    IDS_STRING_UNIT_RATIO, 
    IDS_STRING_UNIT_MB, 
    IDS_STRING_UNIT_SEC, 
    IDS_STRING_UNIT_MB,
    IDS_STRING_UNIT_SEC, 
    IDS_STRING_UNIT_PERCENT, 
    IDS_STRING_UNIT_PERCENT, 
    IDS_STRING_UNIT_HZ 
};

char m_LookupPhase[][MAX_PARAMS_UNIT_SIZE] = 
{ 
    IDS_STRING_PHASE1, 
    IDS_STRING_PHASE2, 
    IDS_STRING_PHASE3,
    IDS_STRING_PHASE4, 
    IDS_STRING_PHASE5 
};

char m_LookupWaveform1[][MAX_PARAMS_UNIT_SIZE] = 
{ 
    IDS_STRING_WAVEFORM1, 
    IDS_STRING_WAVEFORM2 
};

char m_LookupWaveform2[][MAX_PARAMS_UNIT_SIZE] = 
{ 
    IDS_STRING_WAVEFORM1, 
    IDS_STRING_WAVEFORM3 
};

char m_LookupOnOff[][MAX_PARAMS_UNIT_SIZE] = 
{ 
    IDS_STRING_ONOFF1, 
    IDS_STRING_ONOFF2 
};


//
// Look up tables for Parameters, Units and Waveforms
//
char * m_EParams[MAX_EFFECTS] = 
{ 
    0, 
    &m_ChorusNames[0][0],
    &m_CompressorNames[0][0], 
    &m_DistortionNames[0][0], 
    &m_EchoNames[0][0],
    &m_FlangerNames[0][0], 
    &m_GargleNames[0][0], 
    &m_ParamEqNames[0][0],
    &m_ReverbNames[0][0]
};

char * m_EUnits[MAX_EFFECTS] = 
{
    0, 
    &m_ChorusUnits[0][0],
    &m_CompressorUnits[0][0], 
    &m_DistortionUnits[0][0], 
    &m_EchoUnits[0][0],
    &m_FlangerUnits[0][0], 
    &m_GargleUnits[0][0], 
    &m_ParamEqUnits[0][0],
    &m_ReverbUnits[0][0]
};

char * m_ELookup[] = 
{ 
    &m_LookupPhase[0][0], 
    &m_LookupWaveform1[0][0], 
    &m_LookupWaveform2[0][0], 
    &m_LookupOnOff[0][0] 
};

//
// Easier to use numbers than the defines for initialization.
// Defines: CONV_LONG 0,  CONV_FLOAT 1,  CONV_UNUSED 2
//
DWORD m_ParamConv[MAX_EFFECTS][MAX_PARAMS_PER_EFFECT] = 
{
    {2,2,2,2,2,2,2,2,2,2,2,2},      // Unused
    {1,1,1,1,0,1,0,2,2,2,2,2},      // Chorus
    {1,1,1,1,1,1,2,2,2,2,2,2},      // Compressor
    {1,1,1,1,1,2,2,2,2,2,2,2},      // Distortion
    {1,1,1,1,0,2,2,2,2,2,2,2},      // Echo
    {1,1,1,1,0,1,0,2,2,2,2,2},      // Flanger
    {0,0,2,2,2,2,2,2,2,2,2,2},      // Gargle
    {1,1,1,2,2,2,2,2,2,2,2,2},      // ParamEq
    {0,0,1,1,1,0,1,0,1,1,1,1}      // Reverb
};

//
// Parameter data structures (indirect access only).
//
DSFXChorus              ChorusParms;             // Structure to hold Chorus Parameters
DSFXCompressor          CompressorParms;         // Structure to hold Compressor Parameters
DSFXDistortion          DistortionParms;         // Structure to hold Distortion Parameters
DSFXEcho                EchoParms;               // Structure to hold Echo Parameters
DSFXFlanger             FlangerParms;            // Structure to hold Flanger Parameters
DSFXGargle              GargleParms;             // Structure to hold Gargle Parameters
DSFXParamEq             ParamEqParms;            // Structure to hold ParamEq Parameters
I3DL2_LISTENERPROPERTIES ReverbParms;            // Structure to hold Reverb Parameters

LPVOID g_pParams[MAX_EFFECTS] = { NULL, &ChorusParms, &CompressorParms, &DistortionParms, 
                                 &EchoParms, &FlangerParms, &GargleParms, 
                                 &ParamEqParms, &ReverbParms, };



//************************************************************************
// End of File: SimpleFxData.h
//************************************************************************
