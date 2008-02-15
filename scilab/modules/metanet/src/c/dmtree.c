
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <string.h>
#include <stdlib.h>

extern void cerro();

#include "machine.h"
#include "MALLOC.h"
#include "localization.h"

#ifdef _MSC_VER
extern int F2C(arbor)();
#endif

void C2F(dmtree)(int *i0, int *la1, int *lp1, int *ls1, int *m, int *n, int *pred, int *w)
{
  int i,nndim;
  int *alphi,*beta,*f,*g,*ind,*pred1;
  double *z,*zsom;
  int isize,dsize;
  isize = sizeof(int); dsize = sizeof(double);

  nndim = 2 * *n;
  if ((alphi = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((beta = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((f = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((g = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((ind = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((pred1 = (int *)MALLOC(nndim * isize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((z = (double *)MALLOC(nndim * dsize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if ((zsom = (double *)MALLOC(nndim * dsize)) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }

  F2C(arbor)(alphi,beta,f,g,i0,ind,la1,lp1,ls1,m,n,&nndim,pred1,w,z,zsom);

  for (i = 0; i < *n; i++) pred[i] = pred1[i];

  FREE(alphi); FREE(beta); FREE(f); FREE(g); FREE(ind);
  FREE(pred1); FREE(z); FREE(zsom);
}
