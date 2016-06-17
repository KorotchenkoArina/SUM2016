/*
Programmer: AK2
Programm: T07ANIM
Date: 10.06.2016
*/

#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       AK2UNIT *Uni;
 *   - animation context:
 *       AK2ANIM *Ani;                                            
 * RETURNS: None.                                                
 */                                                               
static VOID AK2_UnitInit( ak2UNIT *Uni, ak2ANIM *Ani )            
{                                                                 
} /* End of 'AK2_UnitInit' function */                            
                                                                  
/* Unit deinitialization function.                                
 * ARGUMENTS:                                                     
 *   - self-pointer to unit object:                               
 *       AK2UNIT *Uni;                                             
 *   - animation context:                                           
 *       AK2ANIM *Ani;                                              
 * RETURNS: None.                                                   
 */
static VOID AK2_UnitClose( ak2UNIT *Uni, ak2ANIM *Ani )
{
} /* End of 'AK2_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       AK2UNIT *Uni;
 *   - animation context:
 *       AK2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitResponse( ak2UNIT *Uni, ak2ANIM *Ani )
{
} /* End of 'AK2_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       AK2UNIT *Uni;
 *   - animation context:
 *       AK2ANIM *Ani;
 * RETURNS: None.
 */
static VOID AK2_UnitRender( ak2UNIT *Uni, ak2ANIM *Ani )
{
} /* End of 'AK2_UnitRender' function */

ak2UNIT * AK2_AnimUnitCreate( INT Size )
{
  ak2UNIT *Uni;

  // Memory allocation
  if (Size < sizeof (ak2UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  Uni->Init = AK2_UnitInit;
  Uni->Close = AK2_UnitClose;
  Uni->Response = AK2_UnitResponse;
  Uni->Render = AK2_UnitRender;
  return Uni;
}
// End of "Units" file