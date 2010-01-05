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

#include "api_scilab.h"
#include "gw_signal.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

extern void C2F(rpem)(double theta[], double p[],
		      int *order,
		      double *u, double *y, double *lambda, double *k, double *c,
		      int *istab2,
		      double *v, double *eps, double *eps1,
		      int *dimension,
		      double phi[], double psi[], double tstab[],
		      double work[], double f[], double g[], double l[]);

int sci_rpem(char *fname, unsigned long fname_len)
{
  double *data, *theta, *p, *l, *phi, *psi, *u, *y, *tstab, *work, *f, *g;
  double v, eps, eps1;
  double lambda = 0.950l, alpha = 0.990l, beta = 0.01l, kappa = 0.000l, mu = 0.980l, nu = 0.020l, c = 1000.0l;
  int rows, cols, type, order, dimension, istab2, u_length, i;
  int *arg, *it, *res;


  /*** arguments capture ***/

  CheckRhs(3,6);
  CheckLhs(1,2);

  /* arg1: w0 = list(theta, p, l, phi, psi) */
  getVarAddressFromPosition(pvApiCtx, 1, &arg);
  getListItemNumber(pvApiCtx, arg, &rows);
	getVarType(pvApiCtx, arg, &type);
  if (type != sci_list || rows != 5) {
    Scierror(999, _("%s: Wrong size for input argument #%d: %d-element list expected.\n"), fname, 1, 5);
    return 1;
  }

  /** @todo type of each matrix in w0 should be checked to real
   sci_matrix (better placed in matrix retrieval functions) */
  /** @todo error messages should be more accurate */

  /* theta: 3n real ranged row vector */
  getListItemAddress(pvApiCtx, arg, 1, &it);
  getMatrixOfDouble(pvApiCtx, it, &rows, &cols, &theta);
  if (rows != 1) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), fname, 1);
    return 1;
  }
  if (cols % 3 != 0) {
    Scierror(999, _("%s: Wrong size for input argument #%d: Size must be multiple of %d.\n"), fname, 1, 3);
    return 1;
  }
  dimension = cols;
  order = dimension / 3;

  /* p: 3n x 3n real ranged matrix */
  getListItemAddress(pvApiCtx, arg, 2, &it);
  getMatrixOfDouble(pvApiCtx, it, &rows, &cols, &p);
  if (rows != dimension || cols != dimension) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, 1, 1);
    return 1;
  }
  /* l: 3n real ranged row vector */
  getListItemAddress(pvApiCtx, arg, 3, &it);
  getMatrixOfDouble(pvApiCtx, it, &rows, &cols, &l);
  if (rows != 1 || cols != dimension) {
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, 1, 1);
    return 1;
  }
  /* phi: 3n real ranged row vector */
  getListItemAddress(pvApiCtx, arg, 4, &it);
  getMatrixOfDouble(pvApiCtx, it, &rows, &cols, &phi);
  if (rows != 1 || cols != dimension) {
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, 1, 1);
    return 1;
  }
  /* psi: 3n real ranged row vector */
  getListItemAddress(pvApiCtx, arg, 5, &it);
  getMatrixOfDouble(pvApiCtx, it, &rows, &cols, &psi);
  if (rows != 1 || cols != dimension) {
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, 1, 1);
    return 1;
  }

  /* arg2: u0: real ranged row vector */
  getVarAddressFromPosition(pvApiCtx, 2, &arg);
  getMatrixOfDouble(pvApiCtx, arg, &rows, &cols, &u);
  getVarType(pvApiCtx, arg, &type);
  if((type != sci_matrix) || (rows != 1)) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), fname, 2);
    return 1;
  }
  u_length = cols;

  /* arg3: y0: real ranged row vector of same length as u0 */
  getVarAddressFromPosition(pvApiCtx, 3, &arg);
  getMatrixOfDouble(pvApiCtx, arg, &rows, &cols, &y);
  getVarType(pvApiCtx, arg, &type);
  if ((type != sci_matrix) || (rows != 1) || (cols != u_length)) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), fname, 3);
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"), fname, 2, 3);
    return 1;
  }

  /* optional arguments */
  switch (Rhs) {
  case 6: /* c */
    getVarAddressFromPosition(pvApiCtx, 6, &arg);
    getMatrixOfDouble(pvApiCtx, arg, &rows, &cols, &data);
    getVarType(pvApiCtx,  arg, &type);
    if (type != sci_matrix || rows != 1 || cols != 1) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 6);
      return 1;
    }
    c = *data;
  case 5: /* [kappa, mu, nu] */
    getVarAddressFromPosition(pvApiCtx, 5, &arg);
    getMatrixOfDouble(pvApiCtx, arg, &rows, &cols, &data);
    getVarType(pvApiCtx,  arg, &type);
    if (type != sci_matrix || rows != 1 || cols != 3) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 5, 1, 3);
      return 1;
    }
    kappa = data[0];
    mu = data[1];
    nu = data[2];
  case 4: /* [lambda, alpha, beta] */
    getVarAddressFromPosition(pvApiCtx, 4, &arg);
    getMatrixOfDouble(pvApiCtx, arg, &rows, &cols, &data);
    getVarType(pvApiCtx,  arg, &type);
    if (type != sci_matrix || rows != 1 || cols != 3) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 4, 1, 3);
      return 1;
    }
		lambda = data[0];
		alpha = data[1];
		beta = data[2];
  }

  /*** algorithm call ***/

  /* references provided to justify allocation with code relying on it */
  f = (double *) MALLOC((dimension) * sizeof(double));		/* rpem.f l.168 */
  g = (double *) MALLOC((dimension) * sizeof(double)); 		/* rpem.f l.169  */
  tstab = (double *) MALLOC((order + 1) * sizeof(double)); 	/* rpem.f l.105 */
  work = (double *) MALLOC((2 * order + 2) * sizeof(double)); 	/* nstabl.f */
  /* (tip: bound variables to determine required memory: nk1 <= ordre + 1) */

  for (i = 1 ; i < u_length ; ++i) {
    C2F(rpem)(theta, p, &order, &(u[i-1]), &(y[i]), &lambda, &kappa, &c,
	      &istab2, &v, &eps, &eps1, //output
	      &dimension, phi, psi,
	      tstab,work, //output
	      f, g, l);
    lambda = alpha * lambda + beta;
    kappa = mu * kappa + nu;
  }

  FREE(work);
  FREE(tstab);
  FREE(g);
  FREE(f);

  /*** output formatting ***/

  createList(pvApiCtx, Rhs + 1, 5, &res);
  createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, res, 1, 1, dimension, theta);
  createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, res, 2, dimension, dimension, p);
  createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, res, 3, 1, dimension, l);
  createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, res, 4, 1, dimension, phi);
  createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, res, 5, 1, dimension, psi);
  LhsVar(1) = Rhs + 1;
  if (Lhs == 2) {
    //allocMatrixOfDouble(pvApiCtx, Rhs + 2, 1, 1, &data);
    createMatrixOfDouble(pvApiCtx, Rhs + 2, 1, 1, &v);
    LhsVar(2) = Rhs + 2;
  }
  PutLhsVar();
  return 0;
}
