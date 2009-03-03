/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <string.h>
#include "gw_signal.h"
#include "stack-c.h"
#include "MALLOC.h"
/****************************************************************/
//TODO: check adequate prototype to remez
extern void C2F(remez)(int *ngr, int *nfc, int *iext,
		       double *ad, double *x, double *y,
		       float *des, float *grid, float *wt,
		       double *a, double *p, double *q, double *alpha);
/****************************************************************/
int sci_remez(char *fname, unsigned long fname_len)
{
  int
    nrows1, ncols1,
    nrows2, ncols2,
    nrows3, ncols3,
    nrows4, ncols4;
  double
    *pdblArg1 = NULL,
    *pdblArg2 = NULL,
    *pdblArg3 = NULL,
    *pdblArg4 = NULL,
    *pdblArgOut = NULL;
  int *piArg4 = NULL;
  double
    *lw1 = NULL,
    *lw2 = NULL,
    *lw3 = NULL,
    *lw4 = NULL,
    *lw5 = NULL,
    *lw6 = NULL,
    *lw = NULL;
  int ngrid = 0;
  int nc = 0;
  int iOne = 1;
  
  CheckRhs(4,4);
  CheckLhs(1,1);
  
  GetRhsVarMatrixDouble(4, &nrows1, &ncols1, &pdblArg1);
  ngrid = ncols1 * nrows1;
  C2F(simple)(&ngrid, pdblArg1, (float *)pdblArg1);

  GetRhsVarMatrixDouble(3, &nrows2, &ncols2, &pdblArg2);
  C2F(simple)(&ngrid, pdblArg2, (float *)pdblArg2);
  
  GetRhsVarMatrixDouble(2, &nrows3, &ncols3, &pdblArg3);
  C2F(simple)(&ngrid, pdblArg3, (float *)pdblArg3);

  GetRhsVarMatrixDouble(1, &nrows4, &ncols4, &pdblArg4);
  nc = ncols4 * nrows4;
  piArg4 = (int *)pdblArg4;
  C2F(entier)(&nc, pdblArg4, piArg4);
  nc -= 2;

  iAllocMatrixOfDouble(Rhs + 1, nrows1, nc + 1, &pdblArgOut);

  lw = (double *)MALLOC((nc + 2) * sizeof(double));
  lw1 = (double *)MALLOC((nc + 2) * sizeof(double));
  lw2 = (double *)MALLOC((nc + 2) * sizeof(double));
  lw3 = (double *)MALLOC((nc + 2) * sizeof(double));
  lw4 = (double *)MALLOC((nc + 2) * sizeof(double));
  lw5 = (double *)MALLOC((nc + 2) * sizeof(double));
  lw6 = (double *)MALLOC((nc + 2) * sizeof(double));
  if (lw == NULL  ||
      lw1 == NULL ||
      lw2 == NULL ||
      lw3 == NULL ||
      lw4 == NULL ||
      lw5 == NULL ||
      lw6 == NULL)
    {
      printf("memory allocation error in sci_remez.c\n");
      return 1;
    }

  C2F(remez)(&ngrid, &nc, piArg4, lw1, lw2, lw3,
	     pdblArg3, pdblArg2, pdblArg1, lw4,
	     lw5, lw6, lw);
  nc++;
  C2F(dcopy)(&nc, lw, &iOne, pdblArgOut, &iOne);

  LhsVar(1) = Rhs + 1;
  PutLhsVar();

  free(lw);
  free(lw6);
  free(lw5);
  free(lw4);
  free(lw3);
  free(lw2);
  free(lw1);

  return 0;
}
/****************************************************************/
