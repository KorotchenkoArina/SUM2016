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

  if (Pos == N)
    return;
  else
  {
    for (i = Pos; i < N; i++)
    {
      if (Pos != i)
        Swap(&P[Pos], &P[i]), Parity = !Parity;
      Go(Pos + 1);
      if (Pos != i)
        Swap(&P[Pos], &P[i]), Parity = !Parity;
    }
  }
}

double EvalDeterminant( char *FileName )
{
  int i;
  double prod = 1;

  LoadMatrix(FileName);
  for(i = 0; i < N; i++, prod *= A[i][P[i]])
  { 
    P[i] = i;
    if (Parity == 0)
      sum += prod;
    else
      sum -= prod;
  }
  Go(0);
  return sum;
}

void main( void )
{
  int i;
  char *M[] =
  {
    "m.txt", "m1.txt", "m4.txt" 
  } ;

  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterminant(M[i]));
}