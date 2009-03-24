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

#include <math.h>
#include "MALLOC.h"
#include "gw_signal.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
extern int C2F(scicorr)(char *id,unsigned long fname_len );
/*--------------------------------------------------------------------------*/
int sci_corr0(char *fname,unsigned long fname_len)
{
	C2F(scicorr)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
/****************************************************************/

/****************************************************************/
extern void C2F(tscccf)(double *x, double *y, int*length,
			double *cxy, double *xymean, int *lag, int *error);
extern void C2F(cmpse2)(int *m, int *n, int *mode, double *x, double *y,
			double *xa, double *xr, double *xi,
			double *zr, double *zi, int *error);
extern void C2F(cmpse3)(int *m, int *n, int *mode, double *x, double *y,
			double *xr, double *xi,
			double *zr, double *zi, int *error, int ichaud, int nbx);

/****************************************************************/
int sci_corr(char *fname, unsigned long fname_len)
{
  enum {usual, fft, updates} procedure;
  int rows, cols, i, length;
  char *procedure_name;
  double argument;

  CheckRhs(2,5);
  CheckLhs(1,2);
  
  if (GetType(1) == sci_strings) {
    GetRhsVar(1, STRING_DATATYPE, &rows, &cols, &i);
    procedure_name = cstk(i);
    switch (procedure_name[0]) {
    case 'f':
      procedure = fft;
      break;
    case 'u':
      procedure = updates;
      break;
    default:
      Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 1, "'fft', 'update'");
      return 1;
    }
  }
  else {
    procedure = usual;
  }

  switch (procedure) {
  case usual:
    CheckRhs(2,3);
    corr_usual();
    break;
  case fft:
    CheckRhs(4,5);
    corr_fft();
    break;
  case updates:
    CheckRhs(3,5);
    corr_updates();
    break;
  default:
    return 1;
  }
  
  return 0;
}

/* assert: RhsVar must have been checked to hold 2 to 3 elements */
int corr_usual() {
  int length, lags_number, error, rows, cols;
  double *x, *y, *crossvariance, *mean, *argument;

  // check inputs
  GetRhsVarMatrixDouble(1, &rows, &cols, &argument);
  x = argument;
  length = rows * cols;
  GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
  if (Rhs == 2) {
    y = x;
    lags_number = (int)argument[0];
  }
  else {
    y = argument;
    GetRhsVarMatrixDouble(3, &rows, &cols, &argument);
    lags_number = (int)argument[0];
  }

  iAllocMatrixOfDouble(Rhs + 1, 1, lags_number, &crossvariance);
  iAllocMatrixOfDouble(Rhs + 2, 1, (x == y) ? 1 : 2, &mean);
  
  C2F(tscccf)(x, y, &length, crossvariance, mean, &lags_number, &error);

  LhsVar(1) = Rhs + 1;
  LhsVar(2) = Rhs + 2;
  PutLhsVar();
  
  return error;
}

int corr_fft() {
  int length, lags_number, error, rows, cols;
  double *x, *y, *crossvariance, *mean, *argument;

  printf("fft\n");
  //cmpse2()
}

int corr_updates() {
  printf("updates\n");
  //cmpse3()
}
