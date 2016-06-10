#include "anim.h"

#include <math.h>

typedef struct
{
  ak2UNIT;
  VEC Pos;
} ak2UNIT_BALL;

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