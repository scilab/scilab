#include "machine.h"

#define Min(a,b) (((a)<= (b))? (a) : (b) )


int C2F(fsom)(a, n, b, m, c)
     float *a;
     integer *n;
     float *b;
     integer *m;
     float *c;
{
  int k;
  for (k = 0 ; k < *n ; ++k) 
    {
      c[k] = (float) 1.0;
    };
  for (k = 0 ; k  < Min(*n,*m) ; ++k) 
    {
      c[k] +=  b[k] + a[k];
    };
  return 0;
}

