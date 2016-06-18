/* file name: anim.c
 *progremmer: AK2
 *date: 15.06.2016
 */                                
#include <stdio.h>

#include "anim.h"
#include "shaders.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")


       
#define AK2_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

MATR
  AK2_RndMatrWorld, /* World (object) transformation matrix */
  AK2_RndMatrView,  /* Camera view transform */
  AK2_RndMatrProj;  /* Projection transform */
/* Projection parameters */
DBL
  AK2_RndProjDist,  /* Near clip plane */
  AK2_RndFarClip,   /* Far clip plane */
  AK2_RndProjSize;  /* Project plane size */

INT AK2_MouseWheel;   
ak2ANIM AK2_Anim;

static UINT64
  AK2_StartTime,
  AK2_OldTime,
  AK2_OldTimeFPS,
  AK2_PauseTime,
  AK2_TimePerSec,
  AK2_FrameCounter;

VOID AK2_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER t;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Store window and create memory device context */
  AK2_Anim.hWnd = hWnd;
  AK2_Anim.hDC = GetDC(hWnd);
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AK2_Anim.hDC, &pfd);
  DescribePixelFormat(AK2_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AK2_Anim.hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  AK2_Anim.hGLRC = wglCreateContext(AK2_Anim.hDC);
  wglMakeCurrent(AK2_Anim.hDC, AK2_Anim.hGLRC);

  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AK2_Anim.hGLRC);
    ReleaseDC(AK2_Anim.hWnd, AK2_Anim.hDC);
    exit(0);
  }
  QueryPerformanceFrequency(&t);
  AK2_TimePerSec = t. QuadPart;
  QueryPerformanceCounter(&t);
  AK2_StartTime = AK2_OldTime = AK2_OldTimeFPS = t.QuadPart;
  AK2_PauseTime = 0;

  
  AK2_RndMatrWorld  = MatrIdentity();
  AK2_RndMatrView = MatrView(VecSet(100, 100, 100), VecSet(0, 0, 0), VecSet(1, 1, 1));
  AK2_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);


  AK2_RndProjDist = 1;
  AK2_RndFarClip = 500;
  AK2_RndProjSize = 1;  

  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  AK2_RndPrg = AK2_RndShaderLoad("a");
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
}



VOID  AK2_AnimClose( VOID )
{
  INT i;
  for (i = 0; i < AK2_Anim.NumOfUnits; i++)
  {
    AK2_Anim.Units[i]->Close(AK2_Anim.Units[i], &AK2_Anim);
    free(AK2_Anim.Units[i]);
  }
  AK2_Anim.NumOfUnits = 0;

  AK2_RndShaderFree(AK2_RndPrg);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AK2_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(AK2_Anim.hWnd, AK2_Anim.hDC);

  memset(&AK2_Anim, 0, sizeof(ak2ANIM));  
   

}
VOID  AK2_AnimResize( INT W, INT H )
{
  AK2_Anim.W = W;
  AK2_Anim.H = H;
  glViewport(0, 0, W, H);

  AK2_RndSetProj();
}
VOID  AK2_AnimCopyFrame( VOID )
{
  SwapBuffers(AK2_Anim.hDC);
}
VOID  AK2_AnimRender(VOID)
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;

  AK2_FrameCounter++;

  /* Update timer */
  QueryPerformanceCounter(&t);
  AK2_Anim.GlobalTime = (DBL)(t.QuadPart - AK2_StartTime) / AK2_TimePerSec;
  AK2_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - AK2_OldTime) / AK2_TimePerSec;
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

  /* Update keyboard */
  GetKeyboardState(AK2_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AK2_Anim.Keys[i] >>= 7;
    if(!AK2_Anim.OldKeys[i] && AK2_Anim.Keys[i])
      AK2_Anim.KeysClick[i] = TRUE;
    else
      AK2_Anim.KeysClick[i] = FALSE;
  }
  memcpy(AK2_Anim.OldKeys, AK2_Anim.Keys, 256);

  /* Update mouse */
  GetCursorPos(&pt);
  ScreenToClient(AK2_Anim.hWnd, &pt);
  AK2_Anim.Mdx = pt.x - AK2_Anim.Mx;
  AK2_Anim.Mdy = pt.y - AK2_Anim.My;
  AK2_Anim.Mx = pt.x;
  AK2_Anim.My = pt.y;

  /* Update joystick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;
    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);                                         
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
         /* Buttons */
        for (i = 0; i < 32; i++)
          AK2_Anim.JBut[i] = (ji.dwButtons >> i) & 1;        
          

        /* Axes */                                                  
         AK2_Anim.JX = AK2_GET_JOYSTICK_AXIS(X);
         AK2_Anim.JY = AK2_GET_JOYSTICK_AXIS(Y);    
         AK2_Anim.JZ = AK2_GET_JOYSTICK_AXIS(Z);
         AK2_Anim.JR = AK2_GET_JOYSTICK_AXIS(R);
         AK2_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;       /* !!!!! */ 
      }
    }
  }
  /* Send response to all units */
  for (i = 0; i < AK2_Anim.NumOfUnits; i++)
    AK2_Anim.Units[i]->Response(AK2_Anim.Units[i], &AK2_Anim);

   /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*** Render all units ***/
  for (i = 0; i < AK2_Anim.NumOfUnits; i++)
  {
    AK2_RndMatrWorld = MatrIdentity();
    AK2_Anim.Units[i]->Render(AK2_Anim.Units[i], &AK2_Anim);
  }

  /* Finalize OpenGL drawing */
  glFinish();
}                          
VOID  AK2_AnimAddUnit(ak2UNIT *Uni) 
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
  if(IsExit)
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
