#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

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
     MessageBox(NULL, "Error register window class","ERROR", MB_OK);
     return 0;
   }

   /* Create window */
   hWnd =
     CreateWindow(WND_CLASS_NAME,
     "~~MAGNITNOE POLE~~", 
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
} /* End of "MinMain" function */

  /*Handle drawing function*/
VOID DrawHandle( HDC hDC, INT x, INT y, DOUBLE a)
{
  INT i;
  DOUBLE rad = a * 3.14159265358979 / 180, si = sin(rad), co = cos(rad);
  static POINT pt[5] = 
  {
    {-10, 0}, {-10, 50}, {0, 150}, {10, 50}, {10, 0}
  };
  POINT pt1[sizeof(pt) / sizeof(pt[0])];

  for (i = 0; i < sizeof(pt) / sizeof(pt[0]); i++)
  {
    pt1[i].x = x + pt[i].x * co - pt[i].y * si;
    pt1[i].y = y - (pt[i].x * si + pt[i].y * co);
  }/*End of for cycle*/
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0xFF, 0, 0xFF));
  GetCursorPos(&pt1);
  ScreenToClient(hDC, &pt1);
  Polygon(hDC, pt1, sizeof(pt) / sizeof(pt[0]));
} /*End of Handle drawing function*/

/* Window message handle function */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
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
    DrawHandle(hMemDC, 90, 90, 200); 
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, FALSE);
  case WM_DESTROY:
    KillTimer(hWnd, 11);
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T03POLE.C' FILE */