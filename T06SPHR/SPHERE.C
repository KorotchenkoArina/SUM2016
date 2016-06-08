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
#define PI 3.14159265357989323846

typedef struct 
{
  DOUBLE X, Y, Z;
} VEC;

VEC Rot( VEC V, DOUBLE Angel)
{
  DOUBLE A = Angel * PI / 180;
  VEC r;

  r.X = V.X;
  r.Y = V.Y * cos(A) - V.Z * sin(A);
  r.Z = V.Y * sin(A) + V.Z * cos(A);
  return r;
}

/*Start of drawing sphere function*/
VOID DrawSphere( HDC hDC, INT Xc, INT Yc )
{
  INT i, j;
  DOUBLE theta, phi, X, Y, Z, phase = clock() / 5000.0;
  static VEC G[N][M];


  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(00, 255, 230));
  SetDCBrushColor(hDC, RGB(00, 00, 00));
  for (i = 0; i <= N; i++ )
  {
    theta = i * PI / N;    
    for (j = 0; j <= M; j++)
    {
      phi = j * 2 * PI / M + phase;
      G[i][j].X = R * sin(theta) * sin(phi);
      G[i][j].Y = R * cos(theta);
      G[i][j].Z = R * sin(theta) * cos(phi);

      G[i][j] = Rot(G[i][j], - 200 * sin(phase));
    }
  }

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M; j++)
    {
      X = Xc + G[i][j].X;
      Y = Yc - G[i][j].Y;
      Ellipse(hDC, X - 5, Y - 5, X + 5, Y + 5);
    }
  }

    /*Horizontal lines*/
  for (i = 0; i <= N; i++)
  {
    theta = i * PI / N;
    X = Xc + G[i][0].X;
    Y = Yc - G[i][0].Y;
    MoveToEx(hDC, X, Y, NULL);
    for (j = 0; j <= M; j++)
    {
      phi = j * 2 * PI / M + phase;
      X = Xc + G[i][j].X;
      Y = Yc - G[i][j].Y;
      LineTo(hDC, X, Y);
    }
  }

    /*Vertical lines*/
  for (j = 0; j <= M; j++ )
  {
    phi = j * 2 * PI / M + phase;
    X = Xc + G[0][j].X;
    Y = Yc - G[0][j].Y;
    MoveToEx(hDC, X, Y, NULL);
    for (i = 0; i <= N; i++)
    {
      theta = i * PI / N; 
      X = Xc + G[i][j].X;
      Y = Yc - G[i][j].Y;
      LineTo(hDC, X, Y);
    }
  }
  
}/*End of drawing sphere function*/


/* END OF 'SPHERE.C' FILE */