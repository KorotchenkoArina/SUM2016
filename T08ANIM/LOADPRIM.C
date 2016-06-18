/* FILE NAME: LOADPRIM.C
 * PROGRAMMER: AK2
 * DATE: 14.06.2016
 */
#include <stdio.h>
#include "anim.h"

typedef struct
{
  VEC  P;   /*Vertex position*/
  VEC2 T;   /*Vertex texture coordinates*/ 
  VEC  N;   /*Normal at vertex*/ 
  ak2COLOR C;   /*Vertex color*/
} ak2VERTEX;

BOOL AK2_RndPrimLoad(ak2PRIM * Pr, CHAR *FileName)
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfF;
  CHAR Mtl[300];
  ak2VERTEX V;
  INT p, N0, N1, N2;


  memset(Pr, 0, sizeof(ak2PRIM));
  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;
    fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  for (p = 0; p < NumOfPrimitives; p++)
  {
    INT i;

    /* Read primitive info*/ 
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfF, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive*/ 
    if ((Pr->P = malloc(sizeof(VEC) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->Edges = malloc(sizeof(INT [2]) * NumOfF)) == NULL)
    {
      free(Pr->P);
      Pr->P = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfP = NumOfP;
    Pr->NumOfE = NumOfF;
    for (i = 0; i < NumOfP; i++)
    {
      fread(&V, sizeof(ak2VERTEX), 1, F);
      Pr->P[i] = V.P;
    }
    for (i = 0; i < NumOfF / 3; i++)
    {
      fread(&N0, 4, 1, F);
      fread(&N1, 4, 1, F);
      fread(&N2, 4, 1, F);
      Pr->Edges[i * 3 + 0][0] = N0;
      Pr->Edges[i * 3 + 0][1] = N1;
      Pr->Edges[i * 3 + 1][0] = N1;
      Pr->Edges[i * 3 + 1][1] = N2;
      Pr->Edges[i * 3 + 2][0] = N2;
      Pr->Edges[i * 3 + 2][1] = N0;
    }
    break;
  }

  fclose(F);
  return TRUE;
}