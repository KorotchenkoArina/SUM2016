/*
Programm: T05DET
Programmer: AK2
Date: 06.06.2016
*/
#define MAX 3

#include <stdio.h>

int N;
int P[MAX], Parity = 0;
double A[MAX][MAX], sum = 0;

/*start of "LoadMatrix" function*/
void LoadMatrix( char *FileName )
{
  int i, j;

  FILE *F;
  F = fopen(FileName, "r");
  if (F == NULL)
    return;
  fscanf(F, "%d", &N);
  for (i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
} /*end of "LoadMatrix" functoin*/
void Swap( int *A, int *B )
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}

/*void SavePerm( void )
{
  int i, prod = 1;

  FILE *F;
  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  fclose(F);
}*/

void Go( int Pos )
{
  int i;
  double prod = 1;

 if (Pos == N)
  {
    for(i = 0; i < N; i++) 
      prod *= A[i][P[i]];
    if (Parity == 0)
      sum += prod;
    else
      sum -= prod;
    return;
  }
 else
 {
   Go(Pos + 1);
   for (i = Pos + 1; i < N; i++)
   {
     Swap(&P[Pos], &P[i]);
     Parity = !Parity;
     Go(Pos + 1);
     Swap(&P[Pos], &P[i]);
     Parity = !Parity;
   }
 }
}

double EvalDeterminant( char *FileName )
{
  int i;

  LoadMatrix(FileName);
  sum = 0;
  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  return sum;
}

void main( void )
{
  int i;
  char *M[] =
  {
    "mat1.txt"
  } ;

  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterminant(M[i]));
}