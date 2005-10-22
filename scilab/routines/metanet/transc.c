#include <string.h>
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif


#include "../machine.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

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

  if ((*lpft = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*lsft = (int *)MALLOC(mftdim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }

  if ((cfcl = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((cfcp = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((cw = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((inf = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((nfcomp = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((nn = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((num = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((p = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((p1 = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((pile = (int *)MALLOC(*n * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((s = (int *)MALLOC(sdim * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((som = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((suc = (int *)MALLOC(*m * isize)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  
  F2C(frmtrs)(cfcl,cfcp,cw,inf,lp1,*lpft,ls1,*lsft,m,
	  &mftdim,n,&nc,nfcomp,nn,num,p,p1,pile,s,
	  &sdim,som,suc);

  FREE(inf); FREE(cw); FREE(nfcomp);FREE(cfcp); 
  FREE(cfcl); FREE(nn); FREE(num); FREE(p); FREE(p1); 
  FREE(pile); FREE(s); FREE(som); FREE(suc);

  *lpftdim = n1;
  *lsftdim = (*lpft)[*n] - 1;
}
