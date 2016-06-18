/* FILENAME: U_CONTROL.C
 * PROGRAMMER: AK2
 * DATE: 15.06.2016
 * PURPOSE: Sprite unit drawing module
 */

#include <string.h>

#include "anim.h"
#include "render.h"

typedef struct
{
  ak2UNIT;
  VEC Pos;
} ak2UNIT_CONTROL;

/* Global system info on display */
BOOL AK2_IsSysInfo = 0, AK2_IsKeyInfo = 0;

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_BALL *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitInit( ak2UNIT_CONTROL *Uni, ak2ANIM *Ani )
{
  Uni->Pos = VecSet(10, 0, 10);

} /* End of 'AK2_UnitInit' function */

static VOID AK2_UnitResponse( ak2UNIT_CONTROL *Uni, ak2ANIM *Ani )
{
  /*CHAR StrBuf[200];*/
  FLT x = 0, y = 0, z = 0;
  DBL r;
  static FLT jx = 0, jy = 0, jz = 0;
  static VEC tr = {0, 0, 3}, at = {0, 0, 0}, up1 = {0, 1, 0};

  if (Ani->Keys[VK_MENU] && Ani->KeysClick[VK_RETURN])
    AK2_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    PostMessage(Ani->hWnd, WM_CLOSE, 0, 0);
  if (Ani->KeysClick[VK_F2])
    AK2_IsSysInfo = !AK2_IsSysInfo;
  if (Ani->KeysClick[VK_F3])
    AK2_IsKeyInfo = !AK2_IsKeyInfo;
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick['R'])
  {
    AK2_RndShaderFree(AK2_RndPrg);
    AK2_RndPrg = AK2_RndShaderLoad("a");
  }
  /*if (Ani->JPov == 1 || Ani->JPov == 2 || Ani->JPov == 8)
    z = -1;
  else if (Ani->JPov == 4 || Ani->JPov == 5 || Ani->JPov == 6)
    z = 1;
  if (Ani->JPov == 3 || Ani->JPov == 2 || Ani->JPov == 4)
    x = 1;
  else if (Ani->JPov == 7 || Ani->JPov == 8 || Ani->JPov == 6)
    x = -1;*/
  jx = Ani->JX;    
  jy = Ani->JY; 
  /*MatrTranslate(Ani->JX, Ani->JY, Ani->JZ);                                  /*!!!!!*/
  /*AK2_RndMatrView = MatrView(tr, VecSet(0, 0, 0), VecSet(0, 1, 0));*/
  /*AK2_RndMatrWorld = MatrMulMatr(AK2_RndMatrWorld, MatrMulMatr(, MatrixRotateX(jy)));*/
  
  Uni->Pos = VecAddVec(Uni->Pos, VecSet(x, y, z));
  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime;*/ 
  Uni->Pos = VecTransform43(Uni->Pos, MatrRotateY(30 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = VecTransform43(Uni->Pos, MatrRotateX(30 * Ani->JX * Ani->GlobalDeltaTime));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = VecTransform43(Uni->Pos, MatrRotateY(30 * Ani->Mdy * Ani->GlobalDeltaTime));  /* !!! */
    Uni->Pos = VecTransform43(Uni->Pos, MatrRotate(30 * Ani->Mdx * Ani->GlobalDeltaTime,    /* !!! */
                 VecNormalize(VecCrossVec(VecNormalize(VecSubVec(at, Uni->Pos)), up1))));
  }

  Uni->Pos = VecTransform43(Uni->Pos, MatrRotateY(60 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));      /*!!!*/
  Uni->Pos = VecTransform43(Uni->Pos, MatrRotateY(-60 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));       

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + -Ani->Mdz * Ani->GlobalDeltaTime) / r);

  AK2_RndMatrView = MatrView(Uni->Pos, at, up1);

} /* End of 'AK2_ControlModule' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitClose( ak2UNIT_CONTROL *Uni, ak2ANIM *Ani )
{
} /* End of 'AK2_UnitClose' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ak2UNIT_BALL *Uni;
 *   - animation context:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitRender( ak2UNIT_CONTROL *Uni, ak2ANIM *Ani )
{

} /* End of 'AK2_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ak2UNIT *) pointer to created unit.
 */
ak2UNIT * AK2_UnitCreateControl( VOID )
{
  ak2UNIT_CONTROL *Uni;

  if ((Uni = (ak2UNIT_CONTROL *)AK2_AnimUnitCreate(sizeof(ak2UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AK2_UnitInit;
  Uni->Response = (VOID *)AK2_UnitResponse;
  return (ak2UNIT *)Uni;
} /* End of 'AK2_UnitCreateControl' function */


/* END OF 'SPRITE.C' FILE */