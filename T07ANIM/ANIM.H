/*
Programmer: AK2
Programm: T07ANIM
Date: 10.06.2016
*/

#ifndef __ANIM_H_
#define __ANIM_H_

#define AK2_MAX_UNITS 1000

#include "def.h"

typedef struct tagak2UNIT ak2UNIT;
typedef struct tagak2ANIM
{
  HWND hWnd;
  HDC hDC;  
  INT W, H; 
  HBITMAP hFrame; 
  ak2UNIT *Units[AK2_MAX_UNITS]; 
  INT NumOfUnits; 
} ak2ANIM;
struct tagak2UNIT
{
  VOID (*Init)( ak2UNIT *Uni, ak2ANIM *Ani );
  VOID (*Close)( ak2UNIT *Uni, ak2ANIM *Ani );
  VOID (*Response)( ak2UNIT *Uni, ak2ANIM *Ani );
  VOID (*Render)( ak2UNIT *Uni, ak2ANIM *Ani );
};

#endif /* __ANIM_H_ */

