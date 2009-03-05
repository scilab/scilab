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

#include "gw_signal.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

/****************************************************************/
extern int C2F(amell)(double *du, double *dk, double *dsn2, int *n);
/****************************************************************/
int sci_amell(char *fname,unsigned long fname_len)
{

  int input_rows, input_cols, output_length, rows, cols;
  double *argument, *input_data, *output_data;
  double parameter;

  CheckRhs(2,2);
  CheckLhs(1,1);

  if (iIsComplex(1)) {
    Scierror(999, _("%s: Input argument #%d must be real.\n"), fname, 1);
    return 1;
  }
  GetRhsVarMatrixDouble(1, &input_rows, &input_cols, &input_data);
  if (iIsComplex(2)) {
    Scierror(999, _("%s: Input argument #%d must be real.\n"), fname, 2);
    return 1;
  }
  GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
  parameter = *argument;
  if (rows != 1 || cols != 1) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
    return 1;
  }
  if (parameter < 0 || parameter > 1) {
    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, "0", "1");
    return 1;
  }
  iAllocMatrixOfDouble(Rhs + 1, input_rows, input_cols, &output_data);
  output_length = input_rows * input_cols;
  C2F(amell)(input_data, &parameter, output_data, &output_length);
  LhsVar(1) = Rhs + 1;
  PutLhsVar();
  return 0;
}
/****************************************************************/
