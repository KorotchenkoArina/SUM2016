/*
Programmer: AK2
Programm: T07ANIM
File: U_CUBE.C
Date: 12.06.2016
*/

#include <time.h>                   

#include "anim.h"
#include "render.h"

/* Cube unit representation type */
typedef struct
{
  ak2UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  ak2PRIM Pr;      /* Timer speed value*/
} ak2UNIT_CUBE;

VEC CubeP[] =
{
 {-1, -1,  1},
 { 1, -1,  1},
 { 1, -1, -1},
 {-1, -1, -1},
 {-1,  1,  1},
 { 1,  1,  1},
 { 1,  1, -1},
 {-1,  1, -1}
};

INT CubeE[][2] =
{
 {0, 1}, {1, 2}, {2, 3}, {3, 0},
 {4, 5}, {5, 6}, {6, 7}, {7, 4},
 {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

ak2PRIM Cube =
{
 CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
 CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

static VOID AK2_UnitInit( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 5, Rnd0() * 5, Rnd0() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
} /* End of 'AK2_UnitInit' function */

/* Unit cube deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       AK2UNIT_CUBE *Uni;
 *   - animation context:
 *       AK2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitClose( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
  AK2_RndPrimFree(&Uni->Pr);
} /* End of 'AK2_UnitClose' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       AK2UNIT_CUBE *Uni;
 *   - animation context:
 *       AK2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitResponse( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
} /* End of 'AK2_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       AK2UNIT_CUBE *Uni;
 *   - animation context:
 *       AK2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitRender( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
  AK2_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.1, 0.1, 0.1)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                             MatrTranslate(VecAddVec(Uni->Pos,
                                                                    VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3)))));
  AK2_RndPrimDraw(&Cube);  /* !!! */
} /* End of 'AK2_UnitRender' function */

ak2UNIT * AK2_UnitCreateCube( VOID )
{
  ak2UNIT_CUBE *Uni;

  if ((Uni = (ak2UNIT_CUBE *)AK2_AnimUnitCreate(sizeof(ak2UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AK2_UnitInit;
  Uni->Close = (VOID *)AK2_UnitClose;
  Uni->Response = (VOID *)AK2_UnitResponse;
  Uni->Render = (VOID *)AK2_UnitRender;
  return (ak2UNIT *)Uni;
} /* End of 'AK2_UnitCreateCube' function */

/* END OF 'U_CUBE.C' FILE */
 