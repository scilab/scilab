#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

extern void cerro();

#include "../machine.h"

#if WIN32
extern int F2C(arbor)();
#endif

void C2F(dmtree)(i0,la1,lp1,ls1,m,n,pred,w)
int *i0,*la1,*lp1,*ls1,*m,*n,*pred,*w;
{
  int i,nndim;
  int *alphi,*beta,*f,*g,*ind,*pred1;
  double *z,*zsom;
  int isize,dsize;
  isize = sizeof(int); dsize = sizeof(double);

  nndim = 2 * *n;
  if ((alphi = (int *)malloc(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((beta = (int *)malloc(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((f = (int *)malloc(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((g = (int *)malloc(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((ind = (int *)malloc(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((pred1 = (int *)malloc(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((z = (double *)malloc(nndim * dsize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((zsom = (double *)malloc(nndim * dsize)) == NULL) {
    cerro("Running out of memory");
    return;
  }

  F2C(arbor)(alphi,beta,f,g,i0,ind,la1,lp1,ls1,m,n,&nndim,pred1,w,z,zsom);

  for (i = 0; i < *n; i++) pred[i] = pred1[i];

  free(alphi); free(beta); free(f); free(g); free(ind);
  free(pred1); free(z); free(zsom);
}
