/*
Programmer: AK2
Programm: T07ANIM
Date: 10.06.2016
*/

typedef struct tagak2UNIT ak2UNIT;
struct tagak2UNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       AK2UNIT *Uni;
   *   - animation context:
   *       AK2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Init)( ak2UNIT *Uni, ak2ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       AK2UNIT *Uni;
   *   - animation context:
   *       AK2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( ak2UNIT *Uni, ak2ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       AK2UNIT *Uni;
   *   - animation context:
   *       AK2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( ak2UNIT *Uni, ak2ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       AK2UNIT *Uni;
   *   - animation context:
   *       AK2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( ak2UNIT *Uni, ak2ANIM *Ani );
};

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