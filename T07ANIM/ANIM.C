/*
Programmer: AK2
Programm: T07ANIM
Date: 10.06.2016
*/
#include <stdio.h>
#include "anim.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

// Joystick axis value obtain macro function
#define AK2_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

// Global mouse wheel delta value
INT AK2_MouseWheel;

// Global animation context
ak2ANIM AK2_Anim;

// Timer local data
static UINT64
  AK2_StartTime,
  AK2_OldTime,
  AK2_OldTimeFPS,
  AK2_PauseTime,
  AK2_TimePerSec,
  AK2_FrameCounter;

VOID AK2_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;

  memset(&AK2_Anim, 0, sizeof(ak2ANIM));
  AK2_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  AK2_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);    

  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  AK2_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  AK2_StartTime = AK2_OldTime = AK2_OldTimeFPS = t.QuadPart;
  AK2_PauseTime = 0;

  AK2_RndProjDist = 0.1;
  AK2_RndFarClip = 100;
  AK2_RndProjSize = 0.1;

  AK2_RndMatrView = MatrView(VecSet(8, 7, 6), VecSet(0, 0, 0), VecSet(0, 1, 0));
}
VOID AK2_AnimClose( VOID )
{
  INT i;
  for (i = 0; i < AK2_Anim.NumOfUnits; i++)
  {
    AK2_Anim.Units[i]->Close(AK2_Anim.Units[i], &AK2_Anim);
    free(AK2_Anim.Units[i]);
  }
  AK2_Anim.NumOfUnits = 0;
  DeleteDC(AK2_Anim.hDC);
  DeleteObject(AK2_Anim.hFrame);
  memset(&AK2_Anim, 0, sizeof(ak2ANIM));
}
VOID AK2_AnimResize( DBL W, DBL H )
{
  HDC hDC;
  AK2_Anim.W = W;
  AK2_Anim.H = H;
  if (AK2_Anim.hFrame != NULL)
    DeleteObject(AK2_Anim.hFrame);
  hDC = GetDC(AK2_Anim.hWnd);
  AK2_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);

  ReleaseDC(AK2_Anim.hWnd, hDC);
  SelectObject(AK2_Anim.hDC, AK2_Anim.hFrame); 

  AK2_RndSetProj();
}
VOID AK2_AnimCopyFrame( HDC hDC)
{
  BitBlt(hDC, 0, 0, AK2_Anim.W, AK2_Anim.H, AK2_Anim.hDC, 0, 0, SRCCOPY);
}
VOID AK2_AnimRender( VOID )
{
  INT i;
  POINT pt;
  LARGE_INTEGER t; 
  // Handle timer
  AK2_FrameCounter++;
  QueryPerformanceCounter(&t);
  // Global time
  AK2_Anim.GlobalTime = (DBL)(t.QuadPart - AK2_StartTime) / AK2_TimePerSec;
  AK2_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - AK2_OldTime) / AK2_TimePerSec;
  // Time with pause
  if(AK2_Anim.IsPause)
  {
    AK2_Anim.DeltaTime = 0;
    AK2_PauseTime += t.QuadPart - AK2_OldTime;
  }
  else
  {
     AK2_Anim.DeltaTime = AK2_Anim.GlobalDeltaTime;
     AK2_Anim.Time = (DBL)(t.QuadPart - AK2_PauseTime - AK2_StartTime) / AK2_TimePerSec;
  }
  // FPS
  if(t.QuadPart - AK2_OldTimeFPS > AK2_TimePerSec )
  {
    CHAR str[100];

    AK2_Anim.FPS = AK2_FrameCounter * AK2_TimePerSec / (DBL)(t.QuadPart - AK2_OldTimeFPS);
    AK2_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS: %.5F", AK2_Anim.FPS);
    SetWindowText(AK2_Anim.hWnd, str);
    AK2_FrameCounter = 0;
  }
  AK2_OldTime = t.QuadPart;
  // Keyboard
  GetKeyboardState(AK2_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AK2_Anim.Keys[i] >>= 7;
    if (!AK2_Anim.OldKeys[i] && AK2_Anim.Keys[i])
      AK2_Anim.KeysClick[i] = TRUE;
    else
      AK2_Anim.KeysClick[i] = FALSE;
  }
  memcpy(AK2_Anim.OldKeys, AK2_Anim.Keys, 256);
  // Mouse
  GetCursorPos(&pt);
  ScreenToClient(AK2_Anim.hWnd, &pt);
  AK2_Anim.Mdx = pt.x - AK2_Anim.Mx;
  AK2_Anim.Mdy = pt.y - AK2_Anim.My;
  AK2_Anim.Mx = pt.x;
  AK2_Anim.My = pt.y;
  // Joystick
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;
    // Get joystick info
    if(joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        // Buttons
        for (i = 0; i < 32; i++)
          AK2_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
        // Axes
        AK2_Anim.JX = AK2_GET_JOYSTICK_AXIS(X);
        AK2_Anim.JY = AK2_GET_JOYSTICK_AXIS(Y);
        AK2_Anim.JZ = AK2_GET_JOYSTICK_AXIS(Z);
        AK2_Anim.JR = AK2_GET_JOYSTICK_AXIS(R);
        // Point of view
        AK2_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + i;
      }
    }
  }

  for (i = 0; i < AK2_Anim.NumOfUnits; i++)
    AK2_Anim.Units[i]->Response(AK2_Anim.Units[i], &AK2_Anim);
  SelectObject(AK2_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(AK2_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(AK2_Anim.hDC, RGB(255, 0, 197));
  Rectangle(AK2_Anim.hDC, 0, 0, AK2_Anim.W + 1, AK2_Anim.H + 1);

  for (i = 0; i < AK2_Anim.NumOfUnits; i++)
  {
    SelectObject(AK2_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(AK2_Anim.hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(AK2_Anim.hDC, RGB(0, 0, 0));
    SetDCBrushColor(AK2_Anim.hDC, RGB(255,255, 255));

    AK2_RndMatrWorld = MatrIdentity();
    AK2_Anim.Units[i]->Render(AK2_Anim.Units[i], &AK2_Anim);
  }                        
}
VOID AK2_AnimAddUnit( ak2UNIT *Uni )
{
  if (AK2_Anim.NumOfUnits < AK2_MAX_UNITS)
  {
    AK2_Anim.Units[AK2_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &AK2_Anim); 
  }
}

VOID AK2_AnimDoExit( VOID )
{
  static BOOL IsExit = FALSE;
  if (IsExit)
    return;
  IsExit = TRUE;
  PostMessage(AK2_Anim.hWnd, WM_CLOSE, 0, 0);
}

VOID AK2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen= FALSE;
  static RECT SaveRect;
  if (IsFullScreen)
  {
    SetWindowPos(AK2_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    // Set fullscreen size to window
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    // Store window old size
    GetWindowRect(AK2_Anim.hWnd, &SaveRect);
    // Get nearest monitor
    hmon = MonitorFromWindow(AK2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    // Obtain monitor info 
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    // Set window new size 
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(AK2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AK2_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
}