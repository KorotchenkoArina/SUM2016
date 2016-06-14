/* Setup projection function.
* ARGUMENTS: None.
* RETURNS: None.
*/

#include "anim.h"

/* Global transformation matrices */
MATR
  AK2_RndMatrWorld, /* World (object) transformation matrix */
  AK2_RndMatrView,  /* Camera view transform */
  AK2_RndMatrProj;  /* Projection transform */
/* Projection parameters */
DBL
  AK2_RndProjDist,  /* Near clip plane */
  AK2_RndFarClip,   /* Far clip plane */
  AK2_RndProjSize;  /* Project plane size */

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK2_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (AK2_Anim.W >= AK2_Anim.H)
  ratio_x = (DBL)AK2_Anim.W / AK2_Anim.H;
  else
  ratio_y = (DBL)AK2_Anim.H / AK2_Anim.W;
  AK2_RndMatrProj = MatrFrustum(-ratio_x * AK2_RndProjSize / 2,
  ratio_x * AK2_RndProjSize / 2,
  -ratio_y * AK2_RndProjSize / 2,
  ratio_y * AK2_RndProjSize / 2,
  AK2_RndProjDist, AK2_RndFarClip);
} /* End of 'AK2_RndSetProj' function */VOID AK2_RndPrimDraw( ak2PRIM *Pr )
{
  INT i;
  MATR M;
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(AK2_RndMatrWorld,
    MatrMulMatr(AK2_RndMatrView, AK2_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = VecMulMatr(Pr->P[i], M);

    pts[i].x = (p.X + 1) * AK2_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * AK2_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(AK2_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(AK2_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'AK2_RndPrimDraw' function */VOID AK2_RndPrimFree( ak2PRIM *Pr )
{
  if (Pr->P != NULL)
    free(Pr->P);
  if (Pr->Edges != NULL)
    free(Pr->Edges);
  memset(Pr, 0, sizeof(ak2PRIM));
} /* End of 'AK2_RndPrimFree' function *//* END OF 'RENDER.C' FILE */