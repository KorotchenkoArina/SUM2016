/* file name: U_BOAT.C
 *progremmer: ak2
 *date: 15.06.2016
 */
#include <time.h>                   



#include "anim.h"

MATR
  AK2_RndMatrWorld, /* World (object) transformation matrix */
  AK2_RndMatrView,  /* Camera view transform */
  AK2_RndMatrProj;  /* Projection transform */

/* Boat unit representation type */
typedef struct
{
  ak2UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed; 
  ak2OBJ Pr;  /* Timer speed value*/
  ak2OBJ Lh;  /* Timer speed value*/
} ak2UNIT_CUBE;

/* Cube primitive */

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_CUBE *Uni;                                       
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitInit( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 5, Rnd0() * 10, Rnd0() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;             /* !!!!!!!!!!!!!!!! */
  AK2_RndObjLoad(&Uni->Pr, "models\\boat.g3d");
  AK2_RndObjLoad(&Uni->Lh, "models\\mount.g3d");
} /* End of 'AK2_UnitInit' function */

/* Unit cube deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_CUBE *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitClose( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
  AK2_RndObjFree(&Uni->Pr);
} /* End of 'AK2_UnitClose' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_CUBE *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitResponse( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
 /* if (Ani ->Keys[VK_SPACE])
    AK2_AnimAddUnit(AK2_UnitCreateBall()); */
 /* if (Ani ->Keys"C")
    AK2_AnimAddUnit(AK2_UnitCreateCube());   */

} /* End of 'AK2_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_CUBE *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitRender( ak2UNIT_CUBE *Uni, ak2ANIM *Ani )
{
  AK2_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.002, 0.002, 0.002)),MatrTranslate(VecAddVec(Uni->Pos,VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3))));
  AK2_RndObjDraw(&Uni->Pr);

  AK2_RndMatrWorld = MatrScale(VecSet(20, 3, 20));
  AK2_RndObjDraw(&Uni->Lh);
} /* End of 'AK2_UnitRender' function */

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ak2UNIT *) pointer to created unit.
 */
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