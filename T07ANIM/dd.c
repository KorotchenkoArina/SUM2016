    
#define P(X, Y) ((X) + (Y))

...
  int a, b;

  a = P(a, b); --> a = ((a) + (b));


#define P(X, Y) (X ## Y)

...
  int a, b;

  a = P(a, b); --> a = (ab);



    Anin.JX = (ji.dwX - jc.dXMin);
    Anin.JY = (ji.dwY - jc.dYMin);
    Anin.JZ = (ji.dwZ - jc.dZMin);

#define GET_AXIS(A) (ji.dw ## A - jc.d ## A ## Min)

    Anin.JX = GET_AXIS(X); --> (ji.dwX - jc.dXMin)
    Anin.JY = GET_AXIS(Y); --> (ji.dwY - jc.dXMin)
    Anin.JZ = GET_AXIS(Z); --> (ji.dwZ - jc.dZMin);
