#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#pragma warning(disable: 4244)

SetDbgMemHooks();

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
   WNDCLASS wc;
   HWND hWnd;
   MSG msg;

   /* Create window class */
   wc.style = CS_VREDRAW | CS_HREDRAW;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hIcon = LoadIcon(NULL,IDI_ERROR);
   wc.lpszMenuName = NULL;
   wc.hInstance = hInstance;
   wc.lpfnWndProc = MyWindowFunc;
   wc.lpszClassName = WND_CLASS_NAME;

   if (!RegisterClass(&wc))
   {
     MessageBox(NULL, "Error register window class","ERROR", MB_OK | MB_ICONERROR);
     return 0;
   }

   /* Create window */
   hWnd =
     CreateWindow(WND_CLASS_NAME,
     "o Ball o", 
     WS_OVERLAPPEDWINDOW,
     CW_USEDEFAULT, CW_USEDEFAULT,
     CW_USEDEFAULT, CW_USEDEFAULT,
     NULL,
     NULL,
     hInstance,
     NULL);
   ShowWindow(hWnd, SW_SHOWNORMAL);
   UpdateWindow(hWnd);

   /* Run message loop */
   while (GetMessage(&msg, NULL, 0, 0))
     DispatchMessage(&msg);
   return msg.wParam;
} /* End of MinMain function */

/* Window message handle function */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;  
  DOUBLE a, r;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 11, 2, NULL);
    AK2_AnimInit(hWnd);
    hDC = GetDC(hWnd);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 11);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'MAIN.C' FILE */