/*
Programm: T06SPHR
Programmer: AK2
Date: 06.06.2016
*/

#include <stdio.h>
#include <math.h>

#include "sphere.h"

#define N 15
#define M 30
#define R 300

/*Start of drawing sphere function*/
VOID DrawSphere( HDC hDC, INT Xc, INT Yc )
{
  INT i, j;
  DOUBLE theta, phi, x, y, z, phase = clock() / 5000.0;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(00, 255, 230));
  SetDCBrushColor(hDC, RGB(00, 00, 00));
  for (i = 0; i <= N; i++ )
  {
    theta = i * 3.14159265358979323846 / N;    
    for (j = 0; j <= M; j++)
    {
      phi = j * 2 * 3.14159265358979323846 / M + phase;
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      z = R * sin(theta) * cos(phi);
      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);
    }
  }

    /*Horizontal lines*/
  for (i = 0; i <= N; i++)
  {
    theta = i * 3.14159265358979323846 / N;
    x = Xc + R * sin(theta) * cos(phase);
    y = Yc - R * cos(theta);
    MoveToEx(hDC,x ,y , NULL);
    for (j = 0; j <= M; j++)
    {
      phi = j * 2 * 3.14159265358979323846 / M + phase;
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      LineTo(hDC, x, y);
    }
  }

    /*Vertical lines*/
  for (j = 0; j <= M; j++ )
  {
    phi = j * 2 * 3.14159265358979323846 / M + phase;
    x = Xc;
    y = Yc - R;
    MoveToEx(hDC, x, y, NULL);
    for (i = 0; i <= N; i++)
    {
      theta = i * 3.14159265358979323846 / N; 
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      LineTo(hDC, x, y);
    }
  }
  
}/*End of drawing sphere function*/


/* END OF 'SPHERE.C' FILE */