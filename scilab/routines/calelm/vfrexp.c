#include <math.h>
#include "../machine.h"
void C2F(vfrexp)(n,x,ix,y,iy,z,iz)
     double *x, *y;
     double *z;
     int *n, *ix, *iy, *iz;
{
  int ix1,iy1,iz1,i,j;
  ix1=0;
  iy1=0;
  iz1=0;

  if (*ix<0) ix1=-(*n-1)*(*ix);
  if (*iy<0) iy1=-(*n-1)*(*iy);
  if (*iz<0) iz1=-(*n-1)*(*iz);

  for (i=0;i<*n;i++) {
    y[iy1]=frexp(x[ix1],&j);
    z[iz1]=j;
    iy1 += *iy;
    ix1 += *ix;
    iz1 += *iz;
  }
}
