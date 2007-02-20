
#include "../../routines/machine.h"

int F2C(ext1c)(n, a, b, c)
     int *n;
     double *a, *b, *c;
{   
  int k;
  for (k = 0; k < *n; ++k) 
      c[k] = a[k] + b[k];
  return(0);
}

