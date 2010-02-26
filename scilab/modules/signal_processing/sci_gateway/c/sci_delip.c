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
 */

#include "api_scilab.h"
#include "gw_signal.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"

extern void C2F(delip)(int *length,
		       double *output_real, double *output_imaginary,
		       double *x, double *ck);

int sci_delip(char *fname, int* _piKey)
{
  enum {real, complex} return_type;
  int rows, cols, type, x_rows, x_cols, length, i;
  double *argument, *output_real, *output_imaginary, *x, ck;
  int *p;

  CheckRhs(2,2);
  CheckLhs(1,1);

  /* arg1: x */
  getVarAddressFromPosition(_piKey, 1, &p);
	getVarType(_piKey, p, &type);
  if (type != sci_matrix || isVarComplex(_piKey, p)) {
    Scierror(999, _("%s: Wrong type for argument %d: Real matrix expected.\n"), fname, 1);
    return 1;
  }
  // GetRhsVarMatrixDouble(1, &x_rows, &x_cols, &x);
  getMatrixOfDouble(_piKey, p, &x_rows, &x_cols, &x);
  length = x_cols * x_rows;
  for (i = 0, return_type = real ; i < length ; i++) {
    if (x[i] < 0) {
      Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, 1, "0", "+INF");
      return 1;
    }
    else if (x[i] > 1) {
      return_type = complex;
    }
  }

  /* arg2: ck */
  getVarAddressFromPosition(_piKey, 2, &p);
  // GetVarDimension(2, &rows, &cols);
  getVarDimension(_piKey, p, &rows, &cols);
  getVarType(_piKey, p, &type);

  if (type != sci_matrix || rows != 1 || cols != 1 || isVarComplex(_piKey, p)) {
    Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), fname, 2);
    return 1;
  }
  // GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
  getMatrixOfDouble(_piKey, p, &rows, &cols, &argument);
  ck = argument[0];
  if (ck < -1 || ck > 1) {
      Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, 2, "-1", "1");
      return 1;
  }

  switch (return_type) {
  case real:
    // iAllocMatrixOfDouble(Rhs + 1, x_rows, x_cols, &output_real);
    allocMatrixOfDouble(_piKey, Rhs + 1, x_rows, x_cols, &output_real);
    createMatrixOfDouble(_piKey, Rhs + 1, x_rows, x_cols, output_real);
    /* allocate a useless return buffer to securize delip's call */
    output_imaginary = (double *)MALLOC(length * sizeof(double));
    if (output_imaginary == NULL) {
      Scierror(999, _("%s : Memory allocation error.\n"), fname);
      return 1;
    }
    break;
  case complex:
    // iAllocComplexMatrixOfDouble(Rhs + 1, x_rows, x_cols, &output_real, &output_imaginary);
    allocComplexMatrixOfDouble(_piKey, Rhs + 1, x_rows, x_cols, &output_real, &output_imaginary);
    createComplexMatrixOfDouble(_piKey, Rhs + 1, x_rows, x_cols, output_real, output_imaginary);
    break;
  }
  C2F(delip)(&length, output_real, output_imaginary, x, &ck);
  if (return_type == real) {
    FREE(output_imaginary);
    output_imaginary = NULL;
  }
  LhsVar(1) = Rhs + 1;
  PutLhsVar();
  return 0;
}
