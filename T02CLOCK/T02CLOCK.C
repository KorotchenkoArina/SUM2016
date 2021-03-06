#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#pragma warning(disable: 4244)

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
     "MyLovelyClocks <3", 
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
  CHAR s[30]; 
  HPEN hPen;
  HFONT hFnt;
  static SYSTEMTIME SystemTime;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 11, 2, NULL);
    hBmLogo = LoadImage(NULL, "CLOCKFACE.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDCLogo, hBmLogo);
    SelectObject(hMemDCLogo, hBmLogo);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);
    GetLocalTime(&SystemTime);

    /*Strelki*/
    a = (SystemTime.wSecond + SystemTime.wMilliseconds / 1000.0) * 2 * 3.1415926535 / 60;
    r = bm.bmWidth / 2.2;
    hPen = CreatePen(PS_SOLID, 3, RGB(0x80, 0, 0x80));
    SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    DeleteObject(hPen);

    a = SystemTime.wMinute * 2 * 3.1415926535 / 60;
    r = bm.bmWidth / 3.2;
    hPen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
    SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    DeleteObject(hPen);

    a = (SystemTime.wHour % 12 + SystemTime.wMinute / 60.0) * 2 * 3.1415926535 / 12;
    r = bm.bmWidth / 4.2;
    hPen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
    SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    DeleteObject(hPen);

    /*Day, Month, Year*/
    hFnt = CreateFont(50, 15, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_SCRIPT, "Bookman Old Style");
    SelectObject(hMemDC, hFnt);
    SetTextColor(hMemDC, RGB(255, 0, 255));
    TextOut(hMemDC, w / 2 - 78, h / 2 + h / 3, s, 
      sprintf(s, "%02i %02i %04i", SystemTime.wDay, SystemTime.wMonth, SystemTime.wYear));

    /* VOID WINAPI GetLocalTime(lpSystemTime); */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 11);
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T02CLOCK.C' FILE */
