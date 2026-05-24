//************************************************************************
// File: SimpleOrbiter.cpp
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

#define STRICT

#include "stdafx.h"
#include "Simple6Chand3D.h"
#include "HwInterface.h"

//************************************************************************
// Name: COrbitor
// Desc: Constructor
//************************************************************************
COrbiter::COrbiter( )
{
    m_hDlg        = 0;
    m_pKsPropSet  = NULL;
    m_pDSListener = NULL;
    m_pDSB        = NULL;

    m_bAllowMovementTimer = FALSE;
    m_UseMouse = 0;
    m_OrbitRadius = ORBIT_MAX_RADIUS;
    m_fx = 0.0f;
    m_fy = 0.0f;
    m_fz = 0.0f;

    memset( &m_DsBufferParams, 0, sizeof(DS3DBUFFER *) );
    memset( &m_DsListenerParams, 0, sizeof(DS3DLISTENER) );
    memset( &m_vPosition, 0, sizeof(DS3DVECTOR) );
    memset( &m_vVelocity, 0, sizeof(DS3DVECTOR) );

    // Initialize the Pixels, originally static from Grid routines.
    DWORD i;
    for(i = 0; i < 5; i++)
    {
        m_lPixel[i] = CLR_INVALID;
    }

    m_lX = 0;
    m_lY = 0;

    InitializeCriticalSection(&m_CriticalSectionTimer);
}

    
//************************************************************************
// Name: ~COrbitor
// Desc: Destructor
//************************************************************************
COrbiter::~COrbiter( )
{
    KillInternalTimer();

    m_UseMouse = 0;
    m_bAllowMovementTimer = FALSE;
}


//************************************************************************
// Name: Initialize
// Desc: Initialize all internal variables used by this class.
//************************************************************************
HRESULT COrbiter::Initialize
( 
    HWND                        hDlg,
    LPKSPROPERTYSET             pKsPropSet,
    LPDIRECTSOUND3DLISTENER     pDSListener
)
{
    m_UseMouse = 0;
    m_fx = 0;
    m_fy = 0;
    m_fz = 0;

    m_hDlg = hDlg;
    m_pKsPropSet  = pKsPropSet;
    m_pDSListener = pDSListener;
 
    // Get listener parameters
    m_DsListenerParams.dwSize = sizeof(DS3DLISTENER);
    pDSListener->GetAllParameters( &m_DsListenerParams );

    // Create a timer to periodically move the 3D object around
    SetTimer( hDlg, TIMER_EVENT, 0, NULL );

    return S_OK;
}


//************************************************************************
// Name: SetDSBuffer
// Desc: Load the DS Buffer pointer and enable orbiting if Id not zero.
//************************************************************************
VOID COrbiter::SetDSBuffer( LPDIRECTSOUNDBUFFER pDSB ) 
{ 
    m_pDSB = pDSB; 

    if(pDSB)
        m_bAllowMovementTimer = TRUE;
    else
        m_bAllowMovementTimer = FALSE;
}


//************************************************************************
// Name: OnMovementTimer
// Desc: Periodically updates the position of the object 
//************************************************************************
VOID COrbiter::OnMovementTimer( HWND hDlg ) 
{
    FLOAT fXScale;
    FLOAT fYScale;
    FLOAT fZScale;

    EnterCriticalSection(&m_CriticalSectionTimer);

    if( !m_bAllowMovementTimer )
    {
        LeaveCriticalSection(&m_CriticalSectionTimer);
        return;
    }

    HWND hAutoMove = GetDlgItem( hDlg, IDC_ORBIT );
    long AutoMove = SendMessage( hAutoMove, BM_GETCHECK, 0, 0);

    fXScale = 1.0f;
    fYScale = 0.0f;
    fZScale = 1.0f;
    FLOAT t = timeGetTime()/1000.0f;

    fYScale *= -1.0f;
    fZScale *= -1.0f;

    float Speed = 10.0f;
    float SpeedSquared = Speed * Speed;
    float SpeedValue = t * SpeedSquared / 250;
    float tx = SpeedValue;
    float ty = 0; //SpeedValue;
    float tz = SpeedValue;

    // Move the sound object around the listener. The maximum radius of the
    // orbit is 27.5 units.
    if(AutoMove == BST_CHECKED)
    {
        DS3DVECTOR vNewPos;
        
        vNewPos.x = m_OrbitRadius * fXScale * (FLOAT)sin(tx);
        vNewPos.y = m_OrbitRadius * fYScale * (FLOAT)sin(ty);
        vNewPos.z = m_OrbitRadius * fZScale * (FLOAT)cos(tz);

        m_vVelocity.x = (SMOOTHFACTOR * m_vVelocity.x + 50 * (vNewPos.x - m_vPosition.x)) / (SMOOTHFACTOR+1);
        m_vVelocity.y = 0; // (SMOOTHFACTOR * m_vVelocity.y + 50 * (vNewPos.y - m_vPosition.y)) / (SMOOTHFACTOR+1);
        m_vVelocity.z = (SMOOTHFACTOR * m_vVelocity.z + 50 * (vNewPos.z - m_vPosition.z)) / (SMOOTHFACTOR+1);

        m_vPosition.x = vNewPos.x;
        m_vPosition.y = vNewPos.y;
        m_vPosition.z = vNewPos.z;
    }
    else
    {
        if(m_UseMouse)
        {
            m_vVelocity.x = (SMOOTHFACTOR * m_vVelocity.x + 50 * (m_fx - m_vPosition.x)) / (SMOOTHFACTOR+1);
            m_vVelocity.y = 0; // (SMOOTHFACTOR * m_vVelocity.y + 50 * (m_fy - m_vPosition.y)) / (SMOOTHFACTOR+1);
            m_vVelocity.z = (SMOOTHFACTOR * m_vVelocity.z + 50 * (m_fz - m_vPosition.z)) / (SMOOTHFACTOR+1);
 
            m_vPosition.x = m_fx;
            m_vPosition.y = m_fy;
            m_vPosition.z = m_fz;
        }
        else
        {
            m_vVelocity.x = 0.0f;
            m_vVelocity.y = 0.0f;
            m_vVelocity.z = 0.0f;
        }
    }

    // Show the object's position on the dialog's grid control
    UpdateGrid( hDlg, m_vPosition.x, m_vPosition.z );

    // Set the sound buffer velocity and position
    SetObjectProperties( &m_vPosition, &m_vVelocity );

    LeaveCriticalSection(&m_CriticalSectionTimer);
}

//************************************************************************
// Name: KillInternalTimer
// Desc: Disable the current timer for this instance of this class.
//************************************************************************
VOID COrbiter::KillInternalTimer()
{
    EnterCriticalSection(&m_CriticalSectionTimer);

    m_bAllowMovementTimer = FALSE;
    m_UseMouse = 0;

    if(m_hDlg)
        KillTimer( m_hDlg, TIMER_EVENT );

    LeaveCriticalSection(&m_CriticalSectionTimer);
}

//************************************************************************
// Name: UpdateGrid
// Desc: Draws a red dot in the dialog's grid bitmap at the x,y coordinate.
//************************************************************************
VOID COrbiter::UpdateGrid( HWND hDlg, FLOAT x, FLOAT y )
{
    HWND hWndGrid = GetDlgItem( hDlg, IDC_GRID );
    HDC  hDC      = GetDC( hWndGrid );
    RECT rc;

    // Don't update the grid if a WM_PAINT will be called soon
    BOOL bUpdateInProgress = GetUpdateRect(hDlg,NULL,FALSE);
    if( bUpdateInProgress )
        return;

    if( m_lPixel[0] != CLR_INVALID ) 
    {
        // Replace pixels from that were overdrawn last time
        SetPixel( hDC, m_lX-1, m_lY+0, m_lPixel[0] );
        SetPixel( hDC, m_lX+0, m_lY-1, m_lPixel[1] );
        SetPixel( hDC, m_lX+0, m_lY+0, m_lPixel[2] );
        SetPixel( hDC, m_lX+0, m_lY+1, m_lPixel[3] );
        SetPixel( hDC, m_lX+1, m_lY+0, m_lPixel[4] );   
    }

    // Convert the world space x,y coordinates to pixel coordinates
    GetClientRect( hWndGrid, &rc );
    m_lX = (LONG)( ( x/m_OrbitRadius + 1 ) * ( rc.left + rc.right ) / 2 );
    m_lY = (LONG)( (-y/m_OrbitRadius + 1 ) * ( rc.top + rc.bottom ) / 2 );

    // Save the pixels before drawing the cross hair
    m_lPixel[0] = GetPixel( hDC, m_lX-1, m_lY+0 );
    m_lPixel[1] = GetPixel( hDC, m_lX+0, m_lY-1 );
    m_lPixel[2] = GetPixel( hDC, m_lX+0, m_lY+0 );
    m_lPixel[3] = GetPixel( hDC, m_lX+0, m_lY+1 );
    m_lPixel[4] = GetPixel( hDC, m_lX+1, m_lY+0 );

    // Draw a crosshair object in red pixels
    SetPixel( hDC, m_lX-1, m_lY+0, 0x000000ff );
    SetPixel( hDC, m_lX+0, m_lY-1, 0x000000ff );
    SetPixel( hDC, m_lX+0, m_lY+0, 0x000000ff );
    SetPixel( hDC, m_lX+0, m_lY+1, 0x000000ff );
    SetPixel( hDC, m_lX+1, m_lY+0, 0x000000ff );

    ReleaseDC( hWndGrid, hDC );
}


//************************************************************************
// Name: SetObjectProperties
// Desc: Sets the position and velocity on the 3D buffer
//************************************************************************
VOID COrbiter::SetObjectProperties( DS3DVECTOR* pvPosition, DS3DVECTOR* pvVelocity )
{
    // Every change to 3-D sound buffer and listener settings causes 
    // DirectSound to remix, at the expense of CPU cycles. 
    // To minimize the performance impact of changing 3-D settings, 
    // use the DS3D_DEFERRED flag in the dwApply parameter of any of 
    // the IDirectSound3DListener or IDirectSound3DBuffer methods that 
    // change 3-D settings. Then call the IDirectSound3DListener::CommitDeferredSettings 
    // method to execute all of the deferred commands at once.
    memcpy( &m_DsBufferParams.vPosition, pvPosition, sizeof(DS3DVECTOR) );
    memcpy( &m_DsBufferParams.vVelocity, pvVelocity, sizeof(DS3DVECTOR) );

    // Note..doing the entire structure may be slower than 2 calls (unknown)
    // however, there is a bug in sending the lConeOutsideVolume at this point
    // so lets just send what we want to change.

    if(m_pDSB)
    {
        HRESULT hr;
        if( FAILED( hr = Set3DParameters( m_pDSB,
                                 KSPROPERTY_DIRECTSOUND3DBUFFER_POSITION, 
                                 sizeof(DS3DVECTOR), &m_DsBufferParams.vPosition, 0)))
        {
            m_bAllowMovementTimer = FALSE;
            return;
        }

        if( FAILED( hr = Set3DParameters( m_pDSB,
                                 KSPROPERTY_DIRECTSOUND3DBUFFER_VELOCITY, 
                                 sizeof(DS3DVECTOR), &m_DsBufferParams.vVelocity, 0)))
        {
            m_bAllowMovementTimer = FALSE;
            return;
        }
    }
}


//************************************************************************
// Name: OnMouseMovement
// Desc: Process when the mouse moves
//************************************************************************
VOID COrbiter::OnMouseMovement( HWND hDlg )
{
    if(m_UseMouse)
    {
        long curX, curY, xPos, yPos;

        HWND hWndGrid = GetDlgItem( hDlg, IDC_GRID );
        HDC  hDC = GetDC( hWndGrid );

        RECT Rect;
        GetWindowRect(hWndGrid, &Rect);

        POINT point;
        GetCursorPos(&point);

        curX = point.x;
        curY = point.y;

        if((curX >= Rect.left) && (curX <= Rect.right) && 
           (curY >= Rect.top) && (curY <= Rect.bottom))
        {
            xPos = curX - Rect.left;
            yPos = curY - Rect.top;

            m_fx = (float)(xPos * 200 / (Rect.right - Rect.left) - 100);
            m_fz = (float)(yPos * 200 / (Rect.bottom - Rect.top) - 100);
            m_fx = m_fx / 19;
            m_fz = -m_fz / 19;
        }
    }
}


//************************************************************************
// End of File: SimpleOrbiter.cpp
//************************************************************************
