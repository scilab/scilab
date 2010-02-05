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
#include "MALLOC.h"
#include "gw_signal.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "../../../statistics/src/c/sci_string_matrix.h"

extern int C2F(scicorr)(char *id,unsigned long fname_len );

int corr_default(char *fname, int* _piKey);
int corr_updates(char *fname, int* _piKey);
int corr_fft(char *fname, int* _piKey);

/* dispatch to specialized gateways according to mode (default, 'updt', 'fft') */
int sci_corr(char *fname, int* _piKey)
{
  char *name;
  int type;
  int *arg;

  CheckRhs(2,5);
  CheckLhs(1,2);
  getVarAddressFromPosition(_piKey, 1, &arg);
  getVarType(_piKey, arg, &type);
	if (type == sci_strings) {
    name = create_string(_piKey, 1);
    switch (name[0]) {
    case 'f': /* fft */
      CheckRhs(4,5);
      corr_fft(fname, _piKey);
      break;
    case 'u': /* updates */
      CheckRhs(3,5);
      corr_updates(fname, _piKey);
      break;
    default:
      Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 1, "'fft', 'update'");
      return 1;
    }
    destroy_string(name);
  }
  else { /* default */
    CheckRhs(2,3);
    corr_default(fname, _piKey);
  }
  return 0;
}

int corr_default(char *fname, int* _piKey) {

  extern void C2F(tscccf)(double *x, double *y, int *length,
			  double *cxy, double *xymean, int *lag, int *error);

  int length, lags_number, error, rows1, cols1, rows2, cols2, rows3, cols3;
  double *x, *y, *xvar /* crossvariance */, *mean, *data;
  int *arg;
  int type;

  getVarAddressFromPosition(_piKey, 1, &arg);
  getVarDimension(_piKey, arg, &rows1, &cols1);
  length = rows1 * cols1;
  getVarType(_piKey, arg, &type);
  if ((type != sci_matrix) ||
      (rows1 != 1 && cols1 != 1)) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), fname, 1);
    return 1;
  }
  getMatrixOfDouble(_piKey, arg, &rows1, &cols1, &x);

  /* interpret 2nd argument according to number of arguments */
  getVarAddressFromPosition(_piKey, 2, &arg);
  getMatrixOfDouble(_piKey, arg, &rows2, &cols2, &data);
  if (Rhs == 2) {
    if (rows2 != 1 || cols2 != 1) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
      return 1;
    }
    y = x;
    lags_number = (int)data[0];
  }
  else {
    if (rows1 != rows2 || cols1 != cols2) {
      Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, 1, 2);
      return 1;
    }
    y = data;
    getVarAddressFromPosition(_piKey, 3, &arg);
    getMatrixOfDouble(_piKey, arg, &rows3, &cols3, &data);
    lags_number = (int)data[0];
  }
  allocMatrixOfDouble(_piKey, Rhs + 1, 1, lags_number, &xvar);
  allocMatrixOfDouble(_piKey, Rhs + 2, 1, (x == y) ? 1 : 2, &mean);
  C2F(tscccf)(x, y, &length, xvar, mean, &lags_number, &error);
  LhsVar(1) = Rhs + 1;
  LhsVar(2) = Rhs + 2;
  PutLhsVar();
  return error;
}

int corr_fft(char *fname, int* _piKey) {

  extern void C2F(cmpse2)(int *m, int *n, int *mode,
			  void (*bgetx)(double *, int *, int *),
			  void (*bgety)(double *, int *, int *),
			  double *xa, double *xr, double *xi,
			  double *zr, double *zi, int *error);
  extern void C2F(bgetx)(double *x, int *increment, int *start_idx);
  extern void C2F(bgety)(double *x, int *increment, int *start_idx);
  extern void C2F(setdgetx)(char *name, int *rep);
  extern void C2F(setdgety)(char *name, int *rep);

  enum {self = 2, cross = 3} mode = self;
  int error, rows, cols, n, mm, lag;
  double *r;
  double *xa, *xr, *xi, *zr, *zi, *data;
  int return2_length, one = 1;
  int *arg;

  /* taken from DllmainSignal_processing.c */
  /* the very f*** global referred to underneath */
  extern struct {
    int kgxtop, kgytop, ksec, kisc;
  } C2F(corradr);

  getVarAddressFromPosition(_piKey, Rhs, &arg);
  getMatrixOfDouble(_piKey, arg, &rows, &cols, &data);
  lag = (int)(*data);
  mm = 2 * lag;
  getVarAddressFromPosition(_piKey, Rhs - 1, &arg);
  getMatrixOfDouble(_piKey, arg, &rows, &cols, &data);
  n = (int)(*data);
  mode = (Rhs == 4 ? self : cross);

  /** @todo add code to manage calls to compiled functions */

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
  {
    int imode = mode; /* enum* not equiv to int* :'( */
    C2F(cmpse2)(&mm, &n, &imode,
		C2F(bgetx), C2F(bgety),
		xa, xr, xi, zr, zi, &error);
  }
  allocMatrixOfDouble(_piKey, Rhs + 1, 1, lag, &r);
  C2F(dcopy)(&lag, xa, &one, r, &one);

  LhsVar(1) = Rhs + 1;
  return2_length = Rhs - 3;
  allocMatrixOfDouble(_piKey, Rhs + 2, 1, return2_length, &r);
  C2F(dcopy)(&return2_length, xr, &one, r, &one);
  FREE(zi);
  FREE(zr);
  FREE(xi);
  FREE(xr);
  FREE(xa);
  LhsVar(2) = Rhs + 2;
  PutLhsVar();
  return 0;
}

/* assert: RhsVar must have been checked to hold 4 to 5 elements */
int corr_updates(char *fname, int* _piKey) {

  extern void C2F(cmpse3)(int *m, int *n, int *mode, double *x, double *y,
			  double *xr, double *xi,
			  double *zr, double *zi,
			  int *error, int *ichaud, int *nbx);

  enum {self = 0, cross = 1} mode = self;
  int update = 0;
  int mfft, wpos;
  int length, error, rows, cols;
  double *x, *y,
    *zr, *zi, *wr, *wi, *x_real,
    *rr, *ri, y_value;
  int *arg;
  int one = 1, nbx = 0;
  double zero = 0L;

  /* x */
  getVarAddressFromPosition(_piKey, 2, &arg);
  getMatrixOfDouble(_piKey, arg, &rows, &cols, &x);
  length = rows * cols;
  getVarAddressFromPosition(_piKey, 3, &arg);
  getVarDimension(_piKey, arg, &rows, &cols);
  if (length == rows * cols) {
    mode = cross;
  }
  if ((mode == cross && Rhs == 5) ||
      (mode == self && Rhs == 4)) {
    update = 1;
  }

  /* y */
  if (mode == cross) {
    CheckRhs(4,5);
    if (isVarComplex(_piKey, arg)) {
      Scierror(999, _("%s: Input argument #%d must be real.\n"), fname, 3);
      return 1;
    }
    getMatrixOfDouble(_piKey, arg, &rows, &cols, &y);
  }
  else {
    y_value = 0;
    y = &y_value;
  }

  /* w */
  wpos = (mode == self) ? 3 : 4;
  /* dimension ought to be a power of 2 (could be checked here) */
  /* GetVarDimension(wpos, &rows, &cols); */
  getVarAddressFromPosition(_piKey, wpos, &arg);
  if (isVarComplex(_piKey, arg)) {
    getComplexMatrixOfDouble(_piKey, arg, &rows, &cols, &zr, &zi);
    mfft = rows * cols;
  }
  else {
    getMatrixOfDouble(_piKey, arg, &rows, &cols, &zr);
    mfft = rows * cols;
    zi = (double *)MALLOC(mfft * sizeof(double));
    C2F(dset)(&mfft, &zero, zi, &one);
  }

  /* xu */
  wr = (double *)MALLOC(mfft * sizeof(double));
  wi = (double *)MALLOC(mfft * sizeof(double));
  C2F(dset)(&mfft, &zero, wr, &one);
  C2F(dset)(&mfft, &zero, wi, &one);
  if (update) { /* if xu provided, update it */
    getVarAddressFromPosition(_piKey, wpos + 1, &arg);
    getMatrixOfDouble(_piKey, arg, &rows, &cols, &x_real);
    nbx = rows * cols;
    if (nbx > mfft) {
      /* TODO: make error msg more accurate (2nd dim >= dim(arg)) */
      Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 5, 1, 999);
      return 1;
    }
    C2F(dcopy)(&nbx, x_real, &one, wr, &one);
  }
  else {
    nbx = 0;
  }

  C2F(cmpse3)(&mfft, &length, (int *) &mode, x, y,
	      wr, wi, zr, zi,
	      &error, (int *) &update, &nbx);

  if (error) {
    Scierror(999, _("%s: Wrong size for input argument #%d: Should be a power of 2"), fname, wpos);
    return error;
  }
  /** @warning x built of data on the stack that's reaffected by matrix allocation
     -> copied to keep it safe for lhs2 output */
  C2F(dcopy)(&nbx, &(x[length-nbx]), &one, wr, &one);
  allocComplexMatrixOfDouble(_piKey, Rhs + 1, 1, mfft, &rr, &ri);
  C2F(dcopy)(&mfft, zr, &one, rr, &one);
  C2F(dcopy)(&mfft, zi, &one, ri, &one);
  LhsVar(1) = Rhs + 1;
  if (Lhs == 2) {
    allocMatrixOfDouble(_piKey, Rhs + 2, 1, nbx, &rr);
    C2F(dcopy)(&nbx, wr, &one, rr, &one);
    LhsVar(2) = Rhs + 2;
  }
  PutLhsVar();
  FREE(wr);
  FREE(wi);
  return error;
}
