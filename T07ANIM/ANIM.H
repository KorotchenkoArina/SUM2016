/*
Programmer: AK2
Programm: T07ANIM
Date: 12.06.2016
*/

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* Maximum Unit Count */
#define AK2_MAX_UNITS 1000

/* Forward reference to unit type */
typedef struct tagak2ANIM ak2ANIM;
typedef struct tagak2UNIT ak2UNIT;
/* Unit type */
struct tagak2UNIT
{
  VOID (*Init)( ak2UNIT *Uni, ak2ANIM *Ani );
  VOID (*Close)( ak2UNIT *Uni, ak2ANIM *Ani );
  VOID (*Response)( ak2UNIT *Uni, ak2ANIM *Ani );
  VOID (*Render)( ak2UNIT *Uni, ak2ANIM *Ani );
};
/* Animation type */
struct tagak2ANIM
{
  HWND hWnd;                     // Frame window      
  HDC hDC;                       // Drawing context      
  INT W, H;                      // Frame size      
  HBITMAP hFrame;                // Frame image      
  ak2UNIT *Units[AK2_MAX_UNITS]; // Animation units array      
  INT NumOfUnits;                // Number of animation units  
  // Timer Data
  DBL
    GlobalTime, GlobalDeltaTime,  // Global time and interframe interval
    Time, DeltaTime,              // Time with pause and interframe interval
    FPS;                          // Frames per second value
  BOOL IsPause;                   // Pause Flag
  // Input system
  INT
    Mx, My,                       // Mouse cursor position
    Mz,                           // Mouse wheel absolute value
    Mdx, Mdy, Mdz;                // Delta values of mouse axes
  BYTE
    Keys[256],                    // Current key states
    OldKeys[256],                 // Previous frame key states
    KeysClick[256];               // Click key flags
  BYTE
    JBut[32];                     // Joystick button state
  INT JPov;                       // Joystick point-of-view control [0, 1,..., 8]
  DBL
    JX, JY, JZ, JR;               // Joystick axes
};

__inline DBL Rnd0( VOID )
{
  return (DBL)rand() / RAND_MAX;
} 
__inline DBL Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
}

extern MATR 
      AK2_RndMatrWorld,
      AK2_RndMatrView,
      AK2_RndMatrProj;
extern DBL
  AK2_RndProjDist, /* Near clip plane */
  AK2_RndFarClip,  /* Far clip plane */
  AK2_RndProjSize; /* Project plane size */


// Global mouse wheel delta value
extern INT AK2_MouseWheel;

// Global animation context
extern ak2ANIM AK2_Anim;

/* Primitive representation type */
typedef struct
{
 VEC *P; /* Primitive points array */
 INT NumOfP; /* Point array size */
 INT (*Edges)[2]; /* Edges array - array of point pairs */
 INT NumOfE; /* Edges array size */
} ak2PRIM;

VOID AK2_AnimInit( HWND hWnd ); 
VOID AK2_AnimClose( VOID ); 
VOID AK2_AnimResize( DBL W, DBL H );
VOID AK2_AnimResponse( INT W, INT H ); 
VOID AK2_AnimCopyFrame( HDC hDC ); 
VOID AK2_AnimRender( VOID ); 
VOID AK2_AnimAddUnit( ak2UNIT *Uni );
VOID AK2_AnimDoExit( VOID );
VOID AK2_AnimFullSCreen( VOID );
ak2UNIT * AK2_AnimUnitCreate( INT Size );
VOID AK2_RndPrimDraw( ak2PRIM *Pr );
#endif /* __ANIM_H_ */

