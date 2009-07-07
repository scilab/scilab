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
/****************************************************************/

int corr_usual(void);
int corr_updates(char *fname);
int corr_fft(char *fname, unsigned long fname_len);

int sci_corr(char *fname, unsigned long fname_len)
{
  int rows, cols, procedure_index;
  char *procedure_name;

  CheckRhs(2,5);
  CheckLhs(1,2);
  
  if (GetType(1) == sci_strings) {
    GetRhsVar(1, STRING_DATATYPE, &rows, &cols, &procedure_index);
    procedure_name = cstk(procedure_index);
    switch (procedure_name[0]) {
    case 'f': /** fft case **/
      CheckRhs(4,5);
      corr_fft(fname, fname_len);
      break;
    case 'u': /** updates case **/
      CheckRhs(3,5);
      corr_updates(fname);
      break;
    default:
      Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 1, "'fft', 'update'");
      return 1;
    }
  }
  else { /** usual case **/
    CheckRhs(2,3);
    corr_usual();
  }

  return 0;
}

/* assert: RhsVar must have been checked to hold 2 to 3 elements */
int corr_usual(void) {

  extern void C2F(tscccf)(double *x, double *y, int*length,
			  double *cxy, double *xymean, int *lag, int *error);

  int length, lags_number, error, rows, cols;
  double *x, *y, *crossvariance, *mean, *argument;

  /* TODO: check inputs */
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

/* assert: RhsVar must have been checked to hold 4 to 5 elements */
int corr_fft(char *fname, unsigned long fname_len) {

  extern void C2F(cmpse2)(int *m, int *n, int *mode,
			  void (*bgetx)(double *, int *, int *),
			  void (*bgety)(double *, int *, int *),
			  double *xa, double *xr, double *xi,
			  double *zr, double *zi, int *error);

  extern void C2F(bgetx)(double *x, int *increment, int *start_index);
  extern void C2F(bgety)(double *x, int *increment, int *start_index);

  extern void C2F(setdgetx)(char *name, int *rep);
  extern void C2F(setdgety)(char *name, int *rep);


  // enum {self = 3, cross = 4} correlation_mode = self;
  enum {self = 2, cross = 3} correlation_mode = self;
  int length, lags_number, error, rows, cols, n, mm, lag, res;
  double *xa, *xr, *xi, *zr, *zi, *crossvariance, *mean, *argument;
  void *getx, *gety;
  int return2_length, one = 1;

  char *namex, *namey;

  /* taken from DllmainSignal_processing.c */
  /* the very f*** global referred to underneath */
  extern struct { 
    int kgxtop, kgytop, ksec, kisc;
  } C2F(corradr);
  
  GetRhsVarMatrixDouble(Rhs, &rows, &cols, &argument);
  lag = (int)(*argument);
  mm = 2 * lag;
  GetRhsVarMatrixDouble(Rhs - 1, &rows, &cols, &argument);
  n = (int)(*argument);

  correlation_mode = (Rhs == 4 ? self : cross);

  /* TODO: add code to manage calls to compiled functions */

  /* setting the f*** (!@&£*% etc) global parameter for cmpse2 */
  C2F(corradr).kgxtop = Top - Rhs + 2;
  C2F(corradr).kgytop = Top - Rhs + 3; /* useless for self correlation but doesn't hurt */
  C2F(corradr).ksec = Top;
  C2F(corradr).kisc = Top + 1;

  xa = (double *)MALLOC(mm * sizeof(double));
  xr = (double *)MALLOC(mm * sizeof(double));
  xi = (double *)MALLOC(mm * sizeof(double));
  zr = (double *)MALLOC(mm * sizeof(double));
  zi = (double *)MALLOC(mm * sizeof(double));
  
  C2F(cmpse2)(&mm, &n, &correlation_mode,
	      C2F(bgetx), C2F(bgety),
	      xa, xr, xi, zr, zi, &error);

  iAllocMatrixOfDouble(Rhs + 1, 1, lag, &res);
  C2F(dcopy)(&lag, xa, &one, res, &one);
  LhsVar(1) = Rhs + 1;
  return2_length = Rhs - 3;
  iAllocMatrixOfDouble(Rhs + 2, 1, return2_length, &res);
  C2F(dcopy)(&return2_length, xr, &one, res, &one);
  LhsVar(2) = Rhs + 2;

  PutLhsVar();

  FREE(zi);
  FREE(zr);
  FREE(xi);
  FREE(xr);
  FREE(xa);

  return 0;
}

/* assert: RhsVar must have been checked to hold 4 to 5 elements */
int corr_updates(char *fname) {

  extern void C2F(cmpse3)(int *m, int *n, int *mode, double *x, double *y,
			  double *xr, double *xi,
			  double *zr, double *zi, int *error, int *ichaud, int *nbx);

  /* either self- or cross-correlation */
  enum {self = 0, cross = 1} correlation_mode = self;
  /* is xu provided or is computation started from scratch ? */
  enum {fresh = 0, update = 1}  update_mode = fresh;
  
  int mfft, i, w_position;
  int length, lags_number, error, rows, cols, one = 1, nbx = 0;
  double *x, *y, *crossvariance, *mean, *argument, zero = 0L,
    *z_real, *z_imaginary, *w_real, *w_imaginary, *x_real,
    *result_real, *result_imaginary, y_value;

  /* x ****************/
  GetRhsVarMatrixDouble(2, &rows, &cols, &x);
  length = rows * cols;

  /* detects if y provided by checking length(arg2) = length(arg3) */
  GetVarDimension(3, &rows, &cols);
  if (length == rows * cols) { 
    correlation_mode = cross;
  }
  if ((correlation_mode == cross && Rhs == 5) ||
      (correlation_mode == self && Rhs == 4)) {
    update_mode = update;
  }

  /* y ****************/
  if (correlation_mode == cross) {
    CheckRhs(4,5);
    if (iIsComplex(3)) {
      Scierror(999, _("%s: Input argument #%d must be real.\n"), fname, 3);
      return 1;
    }
    GetRhsVarMatrixDouble(3, &rows, &cols, &y);
  }
  else {
    y_value = 0;
    y = &y_value;
  }
  
  /* w ****************/
  w_position = (correlation_mode == self) ? 3 : 4;
  printf("w position: %d, corr mode: %s\n", w_position, (correlation_mode == self) ? "self" : "cross");
  /* dimension ought to be a power of 2 (could be checked here) */
  /* GetVarDimension(w_position, &rows, &cols); */
  if (iIsComplex(w_position)) {
    printf("complex\n");
    GetRhsVarMatrixComplex(w_position, &rows, &cols, &z_real, &z_imaginary);
    mfft = rows * cols;
  }
  else {
    printf("real\n");
    GetRhsVarMatrixDouble(w_position, &rows, &cols, &z_real);
    mfft = rows * cols;
    z_imaginary = (double *)MALLOC(mfft * sizeof(double));
    C2F(dset)(&mfft, &zero, z_imaginary, &one);
  }

  /* xu ****************/
  w_real = (double *)MALLOC(mfft * sizeof(double));
  w_imaginary = (double *)MALLOC(mfft * sizeof(double));
  C2F(dset)(&mfft, &zero, w_real, &one);
  C2F(dset)(&mfft, &zero, w_imaginary, &one);
  if (update_mode == update) {
    GetRhsVarMatrixDouble(w_position + 1, &rows, &cols, &x_real);
    nbx = rows * cols;
    if (nbx > mfft) {
      /* TODO: make error msg more accurate (2nd dim >= dim(arg)) */
      Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 5, 1, 999);
      return 1;
    }
    /* C2F(dcopy)(&nbx, w_real, &one, x_real, &one); // was so in the old code, useless, isn't it ? */
    C2F(dcopy)(&nbx, x_real, &one, w_real, &one);
  }
  else {
    nbx = 0;
  }

  /****************************************************************/
  C2F(cmpse3)(&mfft, &length, (int *) &correlation_mode, x, y,
	      w_real, w_imaginary, z_real, z_imaginary,
	      &error, (int *) &update_mode, &nbx);

  if (error) {
    Scierror(999, _("%s: Wrong size for input argument #%d: Should be a power of 2"), fname, w_position);
    return error;
  }
  /****************************************************************/

  /* WARNING: x built of data on the stack that's reaffected by matrix allocation
     -> copy to keep it safe for lhs2 output */
  C2F(dcopy)(&nbx, &(x[length-nbx]), &one, w_real, &one);

  iAllocComplexMatrixOfDouble(Rhs + 1, 1, mfft, &result_real, &result_imaginary);
  C2F(dcopy)(&mfft, z_real, &one, result_real, &one);
  C2F(dcopy)(&mfft, z_imaginary, &one, result_imaginary, &one);
  LhsVar(1) = Rhs + 1;

  if (Lhs == 2) {
    iAllocMatrixOfDouble(Rhs + 2, 1, nbx, &result_real);
    C2F(dcopy)(&nbx, w_real, &one, result_real, &one);
    LhsVar(2) = Rhs + 2;
  }

  PutLhsVar();
  
  FREE(w_real);
  FREE(w_imaginary);

  return error;
}
