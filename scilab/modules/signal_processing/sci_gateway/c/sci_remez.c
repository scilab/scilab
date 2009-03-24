/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "MALLOC.h"
#include "gw_signal.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

/****************************************************************/
extern void C2F(remez)(int *ngrid, int *nc, int *iext,
		       double *ad, double *x, double *y,
		       float *des, float *grid, float *wt,
		       double *a, double *p, double *q, double *alpha);
/****************************************************************/
/* allocates required buffers and calls remez */
int remez_buffered(int ngrid, int nc, int *iext,
		   float *des, float *grid, float *wt, double *output);
/****************************************************************/
int sci_remez(char *fname, unsigned long fname_len)
{
  /************************************************
   * Warning : bug 4189                           *
   * The remez function returns                   *
   * an array which last element is meaningless.  *
   * -> sementic of the fortran gw preserved      *
   * -> watch the curious nc's cooking            *
   ************************************************/
  
  int rows, cols, length, ngrid = 0, nc = 0, error = 0;
  double *output = NULL, *argument = NULL;
  float *des = NULL, *grid = NULL, *wt = NULL;
  int *iext;
  
  CheckRhs(4,4);
  CheckLhs(1,1);
  
  GetRhsVarMatrixDouble(1, &rows, &cols, &argument);
  iext = (int *)argument;
  nc = cols * rows;
  C2F(entier)(&nc, argument, iext);

  GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
  des = (float *)argument;
  ngrid = cols * rows;
  length = rows;
  C2F(simple)(&ngrid, argument, des);

  GetRhsVarMatrixDouble(3, &rows, &cols, &argument);
  grid = (float *)argument;
  C2F(simple)(&ngrid, argument, grid);

  GetRhsVarMatrixDouble(4, &rows, &cols, &argument);
  wt = (float *)argument;
  C2F(simple)(&ngrid, argument, wt);

  iAllocMatrixOfDouble(Rhs + 1, rows, nc - 1, &output);

  error = remez_buffered(ngrid, nc - 2, iext, des, grid, wt, output);
  if (error) {
    Scierror(999, _("%s : Memory allocation error.\n"), fname);
    return 1;
  }

  LhsVar(1) = Rhs + 1;
  PutLhsVar();

  return 0;
}

int remez_buffered(int ngrid, int nc, int *iext,
		   float *des, float *grid, float *wt, double *output) {
  int one = 1;
  double *buffer0 = NULL,
    *buffer1 = NULL, *buffer2 = NULL,
    *buffer3 = NULL, *buffer4 = NULL,
    *buffer5 = NULL, *buffer6 = NULL;

  buffer0 = (double *)MALLOC((nc + 2) * sizeof(double));
  buffer1 = (double *)MALLOC((nc + 2) * sizeof(double));
  buffer2 = (double *)MALLOC((nc + 2) * sizeof(double));
  buffer3 = (double *)MALLOC((nc + 2) * sizeof(double));
  buffer4 = (double *)MALLOC((nc + 2) * sizeof(double));
  buffer5 = (double *)MALLOC((nc + 2) * sizeof(double));
  buffer6 = (double *)MALLOC((nc + 2) * sizeof(double));
  if (buffer0 == NULL ||
      buffer1 == NULL || buffer2 == NULL ||
      buffer3 == NULL || buffer4 == NULL ||
      buffer5 == NULL || buffer6 == NULL) {
    return 1;
  }

  C2F(remez)(&ngrid, &nc, iext, buffer1, buffer2, buffer3,
	     des, grid, wt,
	     buffer4, buffer5, buffer6, buffer0);
  nc++;
  C2F(dcopy)(&nc, buffer0, &one, output, &one);

  free(buffer0);
  free(buffer6);
  free(buffer5);
  free(buffer4);
  free(buffer3);
  free(buffer2);
  free(buffer1);

  return 0;
}
