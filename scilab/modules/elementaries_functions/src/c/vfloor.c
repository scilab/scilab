#include <math.h>
#include "machine.h"
void C2F(vfloor)(double *x, double *y, int *n, double *ix, double *iy);
void C2F(vfloor)(double *x, double *y, int *n, double *ix, double *iy)
{
  int ix1,iy1,i;
  ix1=0;
  iy1=0;

  if (*ix<0) ix1=-(*n-1)*(*ix);
  if (*iy<0) iy1=-(*n-1)*(*iy);

  for (i=0;i<*n;i++) {
    y[iy1]=floor(x[ix1]);
    iy1 += *iy;
    ix1 += *ix;
  }
}
