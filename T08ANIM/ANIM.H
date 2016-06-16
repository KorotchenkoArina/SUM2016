 /* FILENAME: ANIM.H
 * PROGRAMMER: AK2
 * DATE: 15.06.2016
 * PURPOSE: Animation system difinitions
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "shaders.h"

#define AK2_MAX_UNITS 100

/* Forward reference to unit type*/
typedef struct tagak2UNIT ak2UNIT;

/* Animation type */
typedef struct
{
  HWND hWnd;                       /* window */
  HDC hDC;                         /* Drawing window context */
  HGLRC hGLRC;                     /* Rendering context */

  INT W, H;                        /* window size */
  ak2UNIT *Units[AK2_MAX_UNITS];   /* animation object array */
  INT NumOfUnits;                  /* is now number animation object */
  /* Timer data */
  DBL GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
      Time, DeltaTime,             /* Time with pause and interframe interval */
      FPS;                         /* Frames per seond value */
  BOOL IsPause;                    /* Pause flag */
  /* Input system */
  INT Mx, My,                      /* Mouse cursor position */
      Mz,                          /* Mouse wheel absolute value */
      Mdx, Mdy, Mdz;               /* Delta values of mouse axes */
  BYTE Keys[256],                  /* Current key state */
       OldKeys[256],               /* Previous frame key state */
       KeysClick[256],             /* Click key flags */
       JBut[32];                   /* Joystick button state */
  INT JPov;                        /* Joystick point-of-view control [0,1..8] */
  DBL JX, JY, JZ, JR;              /* Joystick axes */

} ak2ANIM;

struct tagak2UNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ak2UNIT *Uni;
   *   - animation context:
   *       ak2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Init)(ak2UNIT *Uni, ak2ANIM *Ani);
  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ak2UNIT *Uni;
   *   - animation context:
   *       ak2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Close)(ak2UNIT *Uni, ak2ANIM *Ani);
  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ak2UNIT *Uni;
   *   - animation context:
   *       ak2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Response)(ak2UNIT *Uni, ak2ANIM *Ani);
  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ak2UNIT *Uni;
   *   - animation context:
   *       ak2ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Render)(ak2UNIT *Uni, ak2ANIM *Ani);
};

/*** 
 * Animation system support 
 ***/
extern ak2ANIM AK2_Anim;

/* Global mouse wheel delta value */
extern INT AK2_MouseWheel;

/* Animation initialization function.
 * ARGUMENTS:
 *   - window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID AK2_AnimInit( HWND hWnd );
/* Animation deinitialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID AK2_AnimClose( VOID );
/* Animation initialization function.
 * ARGUMENTS:
 *   - width, height window size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID AK2_AnimResize( INT W, INT H );
/* Double buffer frame copy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK2_AnimCopyFrame( VOID );
/* Animation render function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID AK2_AnimRender( VOID );
/* Animation initialization function.
 * ARGUMENTS:
 *   - Animation parameters:
 *       ak2ANIM *Ani;
 * RETURNS: None.
 */
VOID AK2_AnimAddUnit( ak2UNIT *Uni );
/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (ak2UNIT *) pointer to created unit.
 */
ak2UNIT * AK2_AnimUnitCreate( INT Size );

/* Set/reset full screen mode function */
VOID AK2_AnimFlipFullScreen( VOID );

/*** 
 * Render
 ***/

typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} ak2VERTEX;

typedef struct
{
  INT VA;
  INT VBuf;
  INT IBuf; 
  INT NumOfI;
  MATR M;
  INT MtlNo; /* Material number */
} ak2PRIM;

typedef struct
{
  ak2PRIM *Prims;
  INT NumOfPrims;
} ak2OBJ;

/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} ak24MTL;

/* Global transformation matrices */
extern MATR
  AK2_RndMatrWorld, /* World (object) transformation matrix */
  AK2_RndMatrView,  /* Camera view transform */
  AK2_RndMatrProj;  /* Projection transform */
/* Projection parameters */
extern DBL
  AK2_RndProjDist,  /* Near clip plane */
  AK2_RndFarClip,   /* Far clip plane */
  AK2_RndProjSize;  /* Project plane size */

extern UINT AK2_RndPrg;

/* Materials array */
#define AK2_MAX_MATERIALS 10000
extern ak24MTL AK2_RndMaterials[AK2_MAX_MATERIALS];
extern INT AK2_RndNumOfMaterials;

VOID AK2_RndSetProj( VOID );
VOID AK2_RndPrimDraw( ak2PRIM *Pr );
VOID AK2_RndPrimFree( ak2PRIM *Pr );

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ak2OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL AK2_RndObjLoad( ak2OBJ *Obj, CHAR *FileName );

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ak2OBJ *Obj;
 * RETURNS: None.
 */
VOID AK2_RndObjFree( ak2OBJ *Obj );

VOID AK2_RndObjDraw( ak2OBJ *Obj );

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT AK2_RndFindMaterial( CHAR *Name );

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID AK2_RndLoadMaterials( CHAR *FileName );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */