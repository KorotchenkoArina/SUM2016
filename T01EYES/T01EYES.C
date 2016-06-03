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
     "Title", 
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
   {
     TranslateMessage(&msg);
     DispatchMessage(&msg);
   }
   return msg.wParam;
}

/* Draw eye function */
VOID DrawEye( HWND hWnd, INT X, INT Y, INT R, INT R1 )
{
  HDC hDC;
  POINT pt;
 /* Get screen cursor position */
  GetCursorPos(&pt);
 /* Convert to window coordinate system */
  ScreenToClient(hWnd, &pt);
 /* Draw image */
  MoveToEx(hDC, X, Y, NULL);
  LineTo(hDC, pt.x, pt.y);
  MoveToEx(hDC, X + R + R, Y , NULL);
  LineTo(hDC, pt.x, pt.y);
  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  Ellipse(hDC, X + R, Y - R, X + R + R + R, Y + R);
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(12, 0, 0));
  SetDCBrushColor(hDC, RGB(12, 0, 0));
  Ellipse(hDC, X - R1, Y - R1, X + R1, Y + R1);
  Ellipse(hDC, X + R + R - R1, Y - R1, X + R + R + R1, Y + R1);  
}               

/* Window message handle function */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
    static HBITMAP hBm;

    case WM_SIZE:
      w = LOWORD(lParam);
      h = HIWORD(lParam);
      if(hBm != NULL)
        DeleteObject(hBm);
      HDC hDC = GetDC(hWnd);
      hBm = CreateEomponableBitmap(hDC, w, h);
      ReleaseDC(hWnd, hDC);
      SelectObject(hMemDC, hBm);
      return 0;
    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      DrawEye(hWnd, hDC, 100, 100, 50, 20);
      EndPaint(hWnd, &ps);
      InvalidateRect(hWnd, NULL, TRUE);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_MOUSEMOVE:
      InvalidateRect(hWnd, NULL, TRUE);
    case WM_CREATE:

  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
