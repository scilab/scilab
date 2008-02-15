
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "transc.h"
#include "localization.h"

void C2F(transc)(int *lp1,int **lpft,int *ls1,int **lsft,int *m,int *lsftdim,int *lpftdim,int *n)
{
  int n1,sdim,mftdim;
  int *cfcl,*cfcp,*cw,*inf,nc,*nfcomp,*nn,*num,
  *p,*p1,*pile,*s,*som,*suc;
  int isize = sizeof(int);

  n1 = *n + 1;
  sdim = (*n * (*n - 1))/2 + 1;
  mftdim = (*n * (*n - 1)) + 1;

  if ((*lpft = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*lsft = (int *)MALLOC(mftdim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }

  if ((cfcl = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((cfcp = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((cw = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((inf = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((nfcomp = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((nn = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((num = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((p = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((p1 = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((pile = (int *)MALLOC(*n * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((s = (int *)MALLOC(sdim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((som = (int *)MALLOC(n1 * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((suc = (int *)MALLOC(*m * isize)) == NULL) {
    cerro(_("Running out of memory"));
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
