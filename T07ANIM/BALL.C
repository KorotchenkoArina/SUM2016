#include "anim.h"

#include <math.h>

#define N 15
#define M 30
#define R 300
#define PI 3.14159265357989323846

typedef struct
{
  ak2UNIT;
  VEC Pos;
} ak2UNIT_BALL;

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

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_BALL *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitInit( ak2UNIT_BALL *Uni, ak2ANIM *Ani )
{
  (*Uni).Pos = VecSet(0, 1, 0);
} /* End of 'AK2_UnitInit' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_BALL *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitRender( ak2UNIT_BALL *Uni, ak2ANIM *Ani )
{
  DrawSphere(VecAddVec(Uni->Pos,
                       VecMulNum(VecSet(0, 1, 0), sin(Ani->Time)); 
} /* End of 'AK2_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ak2UNIT *) pointer to created unit.
 */
ak2UNIT * AK2_UnitCreateBall( VOID )
{
  ak2UNIT *Uni;

  if ((Uni = AK2_AnimUnitCreate(sizeof(ak2UNIT_BALL))) == NULL)
    return NULL;
  /* Setup unit methods */
  (Uni*).Init = AK2_UnitInit;
  (Uni*).Render = AK2_UnitRender;
  return Uni;
} /* End of 'AK2_UnitCreateBall' function */