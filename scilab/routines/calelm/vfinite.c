#include "../machine.h"

/* checks if all entries of a vector are finite */

extern int finite __PARAMS( (double v));

int C2F(vfinite)(int *n, double *v)
{
  int i;
  for (i=0; i < *n; i++) 
    if (finite(v[i])==0) return 0;
  return 1;
}
