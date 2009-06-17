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
#include "MALLOC.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

/****************************************************************/
extern void C2F(rpem)(double theta[], double p[],
		      int *order,
		      double *u, double *y, double *lambda, double *k, double *c,
		      int *istab2,
		      double *v, double *eps, double *eps1,
		      int *dimension,
		      double phi[], double psi[], double tstab[],
		      double work[], double f[], double g[], double l[]);
/****************************************************************/
int sci_rpem(char *fname, unsigned long fname_len)
{
  double *argument, *theta, *p, *l, *phi, *psi, *u, *y, *tstab, *work, *f, *g;
  double v, eps, eps1;
  double lambda = 0.950l, alpha = 0.990l, beta = 0.01l, kappa = 0.000l, mu = 0.980l, nu = 0.020l, c = 1000.0l;
  int rows, cols, order, dimension, istab2, u_length, i;

  /********************************
   *      arguments capture       *
   ********************************/

  CheckRhs(3,6);
  CheckLhs(1,2);

  /* arg1: w0 = list(theta, p, l, phi, psi) */
  GetVarDimension(1, &rows, &cols);
  if (GetType(1) != sci_list || rows != 5) {
    Scierror(999, _("%s: Wrong size for input argument #%d: An %d elements list expected.\n"), fname, 1, 5);
    return 1;
  }

  /* TODO: type of each matrix in w0 should be checked to real
   sci_matrix (left over since no type checking facility is provided
   for variables nested in a list) */
  /* TODO: error messages should be more accurate (but no standardized
   message referring to list elements is provided) */

  /** theta: 3n real ranged row vector **/
  iGetListItemDouble(1, 1, &rows, &cols, &theta, NULL);
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

  /** p: 3n x 3n real ranged matrix **/
  iGetListItemDouble(1, 2, &rows, &cols, &p, NULL);
  if (rows != dimension || cols != dimension) {
  }
  /** l: 3n real ranged row vector **/
  iGetListItemDouble(1, 3, &rows, &cols, &l, NULL);
  if (rows != 1 || cols != dimension) {
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, 1, 1);
    return 1;
  }
  /** phi: 3n real ranged row vector **/
  iGetListItemDouble(1, 4, &rows, &cols, &phi, NULL);
  if (rows != 1 || cols != dimension) {
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, 1, 1);
    return 1;
  }
  /** psi: 3n real ranged row vector **/
  iGetListItemDouble(1, 5, &rows, &cols, &psi, NULL);
  if (rows != 1 || cols != dimension) {
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, 1, 1);
    return 1;
  }

  /* arg2: u0: real ranged row vector */
  GetRhsVarMatrixDouble(2, &rows, &cols, &u);
  if ((GetType(2) != sci_matrix) || (rows != 1)) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), fname, 2);
    return 1;
  }
  u_length = cols;

  /* arg3: y0: real ranged row vector of same length as u0 */
  GetRhsVarMatrixDouble(3, &rows, &cols, &y);
  if ((GetType(3) != sci_matrix) || (rows != 1) || (cols != u_length)) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), fname, 3);
    Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"), fname, 2, 3);
    return 1;
  }

  /* optional arguments */
  switch (Rhs) {
  case 6: /* arg6 : c */
    GetRhsVarMatrixDouble(6, &rows, &cols, &argument);
    if (GetType(6) != sci_matrix || rows != 1 || cols != 1) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 6);
      return 1;
    }
    c = *argument;
  case 5: /* arg5 : [kappa, mu, nu] */
    GetRhsVarMatrixDouble(5, &rows, &cols, &argument);
    if (GetType(5) != sci_matrix || rows != 1 || cols != 3) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 5, 1, 3);
      return 1;
    }
    kappa = argument[0];
    mu = argument[1];
    nu = argument[2];
  case 4: /* arg4 : [lambda, alpha, beta] */
    GetRhsVarMatrixDouble(4, &rows, &cols, &argument);
    if (GetType(4) != sci_matrix || rows != 1 || cols != 3) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 4, 1, 3);
      return 1;
    }
    lambda = argument[0];
    alpha = argument[1];
    beta = argument[2];
  }

  /********************************
   *        algorithm call        *
   ********************************/

  /* references provided to justify allocation with code relying on it */
  f = (double *) MALLOC((dimension) * sizeof(double)); /* rpem.f l.168 */
  g = (double *) MALLOC((dimension) * sizeof(double)); /* rpem.f l.169  */
  tstab = (double *) MALLOC((order + 1) * sizeof(double)); /* rpem.f l.105 */
  work = (double *) MALLOC((2 * order + 2) * sizeof(double)); /* nstabl.f */
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

  /********************************
   *        result output         *
   ********************************/

  /* first call argument left onplace (modified) */
  LhsVar(1) = 1;

  /* overwrite second call argument */
  if (Lhs == 2) {
    iAllocMatrixOfDouble(2, 1, 1, &argument);
    *argument = v;
    LhsVar(2) = 2;
  }

  /* set top of stack according to the number of arguments recycled for output */
  Top = Top - Rhs + Lhs;

  return 0;
}
