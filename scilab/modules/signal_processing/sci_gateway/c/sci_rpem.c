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
#include "sci_mem_alloc.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
extern int C2F(scirpem)(char *id,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_rpem(char *fname,unsigned long fname_len)
{
	C2F(scirpem)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
/****************************************************************/
extern void C2F(rpem)(double theta[], double p[],
		      int *n,
		      double *u, double *y, double *lambda, double *k,
		      double *c,
		      int *istab2,
		      double *v, double *eps, double *eps1,
		      int *idim,
		      double phi[], double psi[], double tstab[],
		      double work[], double f[], double g[], double l[]);
/****************************************************************/
int sci_rpem0(char *fname, unsigned long fname_len)
{
  double *argument, *theta, *p,u, y, lambda, k, v, eps, eps1, c,
    *phi, *psi, *tstab, *work, *f, *g, *l;
  int rows, cols, n, istab2, idim;

  CheckRhs(3,6);
  CheckLhs(1,2);

  /* arg1 : w0 */
  GetVarDimension(1, &rows, &cols);
  if (GetType(1) != sci_list || cols != 5) {
    Scierror(999, _("%s: Wrong size for input argument #%d: An %d elements list expected.\n"), fname, 1, 5);
    return 1;
  }
  // GetList


  /* arg2 : u0 */
  /* arg3 : y0 */
  /* arg4 : lambda */
  /* arg5 : k */
  /* arg6 : c */

  C2F(rpem)(theta, p, &n, &u, &y, &lambda, &k, &c, &istab2, &v, &eps, &eps1,
		      &idim, phi, psi, tstab,work, f, g, l);

  LhsVar(1) = Rhs + 1; // + ?
  PutLhsVar();

  return 0;
}
