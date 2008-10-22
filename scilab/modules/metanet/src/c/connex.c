
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
 
#include "connex.h"
#include "localization.h"

void C2F(concom)(int *icomp, int *n, int *ncomp, int **ns, int *nsize)
{
  int i;
  *nsize = 0;
  if ((*ns = (int *)MALLOC(*n * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  for (i = 0; i < *n; i++){
    if (ncomp[i] == *icomp) {
      (*ns)[*nsize] = i + 1;
      (*nsize)++;
    }
  }
}
