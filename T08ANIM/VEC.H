/*
Programmer: AK2
Programm: T07ANIM
Date: 08.06.2016
*/
#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>
#include <stdlib.h>

#include <windows.h>



#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)

/* Basic double type */
typedef double DBL;
/* Basic float type */
typedef float FLT;

/* type for vec at spase */
typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagVEC2
{
  FLT X, Y;
} VEC2;

typedef struct tagVEC4
{
  FLT X, Y, Z, R;
} VEC4;

/* matrix type */
typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

__inline FLT Rnd0( VOID )
{
  return (FLT)rand() / RAND_MAX;
}

__inline FLT Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
}

// Vecs:
__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC r = {X, Y, Z};

  return r;
}  /* End of 'VecSet' function */

__inline VEC2 Vec2Set( FLT X, FLT Y )
{
  VEC2 r = {X, Y};

  return r;
}  /* End of 'Vec2Set' function */

__inline VEC4 Vec4Set( FLT X, FLT Y, FLT Z, FLT R )
{
  VEC4 r = {X, Y, Z, R};

  return r;
}  /* End of 'Vec4Set' function */

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}
// End of "VecAddVec" function
__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}
// End of "VecSubVec" function
__inline VEC VecMulNum( VEC A, FLT N )
{
  return VecSet(A.X * N, A.Y * N, A.Z * N);
}
// End of "VecMulNum" function
__inline VEC VecDivNum( VEC A, FLT N )
{
  return VecSet(A.X / N, A.Y / N, A.Z / N);
}
// End of "VecDivNum" function
__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}
// End of "VecNeg" function
__inline FLT VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z; 
}
// End of "VecDotVec" function
__inline VEC VecCrossVec( VEC A, VEC B )
{
  return VecSet(A.Y * B.Z - A.Z * B.Y,
                A.Z * B.X - A.X * B.Z,
                A.X * B.Y - A.Y * B.X);
}
// End of "VecCrossVec" function
__inline FLT VecLen2( VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;
  return len;
}
// End of "VecLen2" function
__inline FLT VecLen( VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 1 && len != 0)
    return sqrt(len);
  return len;
}
// End of "VecLen" function
__inline VEC VecNormalize( VEC V ) 
{
  FLT len = VecDotVec(V, V);

  if (len != 1 && len != 0)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
} /* End of 'VecNormalize' function */

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
// End of "PointTransform" function
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}
// End of "VectorTransform" function
__inline VEC PointTransform4( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}
// End of "PointTransform4" function

__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}  

// Matrix

__inline MATR MatrTranslate( VEC D )
{
  MATR M =
  {
    {
      {  1,   0,   0,  0},
      {  0,   1,   0,  0},
      {  0,   0,   1,  0},
      {D.X, D.Y, D.Z,  1},
    }
  };

  return M;
}
// End of "MatrTranslate" function
//VEC V = PointTransform(VecSet(1, 2, 3), MatrTranslate(VecSet(4, 0, 0)));

__inline MATR MatrScale( VEC S )
{
  MATR M =
  {
    {
      {S.X,    0,    0,  0},
      {   0, S.Y,    0,  0},
      {   0,    0, S.Z,  0},
      {   0,    0,    0,  1}
    }
  };
  return M;
}
// End of "MatrScale" function
__inline MATR MatrRotateX( FLT AngleDegree )
{
  FLT A = D2R(AngleDegree), si = sin(A), co = cos(A);
  MATR M = 
  {
    {
      {  1,   0,   0, 0},
      {  0,  co,  si, 0},
      {  0, -si,  co, 0},
      {  0,   0,   0, 1},
    }
  };
  return M;
}
// End of "MatrRotateX" function
__inline MATR MatrRotateY( FLT AngleDegree )
{
  FLT A = D2R(AngleDegree), si = sin(A), co = cos(A);
  MATR M = 
  {
    {
      {co,  0, -si, 0},
      { 0,  1,   0, 0},
      {si,  0,  co, 0},
      { 0,  0,   0, 1},
    }
  };
  return M;
}
// End of "MatrRotateY" function
__inline MATR MatrRotateZ( FLT AngleDegree )
{
  FLT A = D2R(AngleDegree), si = sin(A), co = cos(A);
  MATR M = 
  {
    {
      { co, si, 0, 0},
      {-si, co, 0, 0},
      {  0,  0, 1, 0},
      {  0,  0, 0, 1},
    }
  };
  return M;
}
// End of "MatrRotateZ" function
__inline MATR MatrRotate( FLT AngleDegree, VEC R )
{
  FLT A = D2R(AngleDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),        V.X * V.Y * (1 - co) - V.Z * si,  V.X * V.Z * (1 - co) + V.Y * si, 0},
      {V.Y * V.X * (1 - co) + V.Z * si,  co + V.Y * V.Y * (1 - co),        V.Y * V.Z * (1 - co) - V.X * si, 0},
      {V.Z * V.X * (1 - co) - V.Y * si,  V.Z * V.Y * (1 - co) + V.X * si,  co + V.Z * V.Z * (1 - co),       0},
      {0,                                0,                                0,                               1}
    }
  };

  return M;
}
// End of "MatrRotate" function
__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}
// End of "MatrIdentity" function
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0, r.A[i][j] = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}
// End of "MatrMulMatr" function
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
}

__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                             FLT A21, FLT A22, FLT A23,
                             FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}
// End of "MatrTranspose" function

__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}
// End of "MatrDeterm" function
__inline MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return M;
  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                             M.A[2][1], M.A[2][2], M.A[2][3],
                             M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                             M.A[2][0], M.A[2][1], M.A[2][3],
                             M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2]) / det;
}
// End of "MatrInverse" function

__inline MATR MatrFrustum( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f )
{
  MATR M =
  {
    {
      {(2 * n) / (r - l),                 0,                      0,  0},
      {                0, (2 * n) / (t - b),                      0,  0},
      {(r + l) / (r - l), (t + b) / (t - b),     -(f + n) / (f - n), -1},
      {                0,                 0, -(2 * n * f) / (f - n),  0}
    }
  };

  return M;
}

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR M = 
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };
  return M;
}

__inline VEC VecTransform43( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'VecTransform43' function */

__inline VEC NormalTransform( VEC V, MATR InvM )
{
  int i, j;
  FLT w, tmp;
  VEC Vres;

  InvM = MatrInverse(InvM);
  for (i = 0; i < 4; i++)
    for (j = i + 1; j < 4; j++)
    {
      tmp = InvM.A[i][j];
      InvM.A[i][j] = InvM.A[j][i];
      InvM.A[j][i] = tmp;
    }
  w = V.X * InvM.A[0][3] + V.Y * InvM.A[1][3] + V.Z * InvM.A[2][3];

  Vres.X = (V.X * InvM.A[0][0] + V.Y * InvM.A[1][0] + V.Z * InvM.A[2][0]) / w;
  Vres.Y = (V.X * InvM.A[0][1] + V.Y * InvM.A[1][1] + V.Z * InvM.A[2][1]) / w;
  Vres.Z = (V.X * InvM.A[0][2] + V.Y * InvM.A[1][2] + V.Z * InvM.A[2][2]) / w;

return Vres;
} /* End of 'NormalTransform' function */

#endif /* __VEC_H_ */

// End of "Vec.H" file