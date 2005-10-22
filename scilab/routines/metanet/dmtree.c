#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

extern void cerro();

#include "../machine.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

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
  if ((alphi = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((beta = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((f = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((g = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((ind = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((pred1 = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((z = (double *)MALLOC(nndim * dsize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((zsom = (double *)MALLOC(nndim * dsize)) == NULL) {
    cerro("Running out of memory");
    return;
  }

  F2C(arbor)(alphi,beta,f,g,i0,ind,la1,lp1,ls1,m,n,&nndim,pred1,w,z,zsom);

  for (i = 0; i < *n; i++) pred[i] = pred1[i];

  FREE(alphi); FREE(beta); FREE(f); FREE(g); FREE(ind);
  FREE(pred1); FREE(z); FREE(zsom);
}
