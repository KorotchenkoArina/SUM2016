/* Programm: T07ANIM
 * PROGRAMMER: AK2
 * DATE: 08.06.2016
 */

#include <windows.h>

#include "vec.h"

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
/*INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  MATR M;

  M = MatrIdentity();
  M = MatrTranslate(VecSet(1, 2, 3));
  M = MatrScale(VecSet(1, 2, 3));
  M = MatrRotateX(30);
  M = MatrRotateY(30);
  M = MatrRotateZ(30);
  M = MatrRotate(30, VecSet(1, 2, 3));
  M = MatrMulMatr(MatrRotateX(90), MatrScale(VecSet(2, 2, 2)));
  M = MatrTranspose(M);
  M = MatrInverse(M);
  M = MatrView(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  M = MatrOrtho(-1, 1, -1, 1, -1, 1);
  M = MatrFrustum(-1, 1, -1, 1, 1, 1000);

} /* End of 'WinMain' function */

/* END OF 'T07ANIM.C' FILE */
