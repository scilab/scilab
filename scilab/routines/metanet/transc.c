#include <string.h>
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif


#include "../machine.h"

#ifdef WIN32
extern void cerro __PARAMS((char *str));
extern int F2C(frmtrs) ();
#endif

void C2F(transc)(lp1,lpft,ls1,lsft,m,lsftdim,lpftdim,n)
int *lp1,**lpft,*ls1,**lsft,*m,*lsftdim,*lpftdim,*n;
{
  int n1,sdim,mftdim;
  int *cfcl,*cfcp,*cw,*inf,nc,*nfcomp,*nn,*num,
  *p,*p1,*pile,*s,*som,*suc;
  int isize = sizeof(int);

  n1 = *n + 1;
  sdim = (*n * (*n - 1))/2 + 1;
  mftdim = (*n * (*n - 1)) + 1;

  if ((*lpft = (int *)malloc(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*lsft = (int *)malloc(mftdim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }

  if ((cfcl = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((cfcp = (int *)malloc(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((cw = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((inf = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((nfcomp = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((nn = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((num = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((p = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((p1 = (int *)malloc(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((pile = (int *)malloc(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((s = (int *)malloc(sdim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((som = (int *)malloc(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((suc = (int *)malloc(*m * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  
  F2C(frmtrs)(cfcl,cfcp,cw,inf,lp1,*lpft,ls1,*lsft,m,
	  &mftdim,n,&nc,nfcomp,nn,num,p,p1,pile,s,
	  &sdim,som,suc);

  free(inf); free(cw); free(nfcomp);free(cfcp); 
  free(cfcl); free(nn); free(num); free(p); free(p1); 
  free(pile); free(s); free(som); free(suc);

  *lpftdim = n1;
  *lsftdim = (*lpft)[*n] - 1;
}
