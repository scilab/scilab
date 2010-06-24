
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static int cx1 = 1;
static int cx0 = 0;
static double c_b40 = 0.;
/*--------------------------------------------------------------------------*/
extern int C2F(vfinite)(int *n, double *v);
extern int C2F(wbdiag)();
extern int C2F(bdiag)();
/*--------------------------------------------------------------------------*/
/* [Ab [,X [,bs]]]=bdiag(A [,rMax]) */
/*--------------------------------------------------------------------------*/
int C2F(intbdiagr)(char *fname, long unsigned int fname_len)
{
    int ix1, ix2;
    double dx1;
    int fail;
    double rMax;
    int ix, j, k, m, n;
    double t;
    int nbloc, lrMax;
    int m1, n1, la, le, lj, it;
    int lw, lx ;
    int lai, lib, lbs, lxi, lxr;

    CheckRhs(1,2);
    CheckLhs(1,3);

    GetRhsCVar(1,MATRIX_OF_DOUBLE_DATATYPE, &it, &m, &n, &la, &lai);
    CheckSquare(1,m,n);

    if (n == 0) {
	CreateVar(2,MATRIX_OF_DOUBLE_DATATYPE, &cx0, &cx0, &lx);
	CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE, &cx0, &cx0, &lbs);
	LhsVar(1) = 1;
	LhsVar(2) = 2;
	LhsVar(3) = 3;
	return 0;
    }
    ix1 = (it + 1) * m * n;
    if (C2F(vfinite)(&ix1, stk(la )) == 0) {
	Err = 1;
	SciError(264);
	return 0;
    }
    if (Rhs == 2) {
	GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &n1, &m1, &lrMax);
	CheckScalar(2,n1,m1);
	rMax = *stk(lrMax );
    } else {
	rMax = 1.;
	lj = la - 1;
	ix1 = n;
	for (j = 1; j <= ix1; ++j) {
	    t = 0.;
	    ix2 = n;
	    for (ix = 1; ix <= ix2; ++ix) {
		t += (dx1 = *stk(lj + ix ), Abs(dx1));
	    }
	    if (t > rMax) {
		rMax = t;
	    }
	    lj += n;
	}
    }
    CreateCVar(2,MATRIX_OF_DOUBLE_DATATYPE, &it, &n, &n, &lxr, &lxi);
    ix1 = n << 1;
    CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE, &cx1, &ix1, &le);
    CreateVar(4,MATRIX_OF_INTEGER_DATATYPE, &cx1, &n, &lib);
    CreateVar(5,MATRIX_OF_DOUBLE_DATATYPE, &cx1, &n, &lw);
    if (it == 0) {
      /*     subroutine bdiag(lda,n,a,epsshr,rMax,er,ei,bs,x,xi,scale,job,fail) */
      C2F(bdiag)(&n, &n, stk(la ), &c_b40, &rMax, stk(le ), stk(le + n ),
		 istk(lib ), stk(lxr ), stk(lxi ), stk(lw ), &cx0, &fail);
    } else {
	C2F(wbdiag)(&n, &n, stk(la ), stk(la + n * n ), &rMax, stk(le ),
		    stk(le + n ), istk(lib ), stk(lxr ), stk(lxi ), &t, &t, stk(lw ), &cx0, &fail);
    }

    if (fail) {
      Scierror(24,_("%s: Non convergence in QR steps.\n"),fname);
      return 0;
    }
    if (Lhs == 3) {
      nbloc = 0;
      for (k = 1; k <= n; ++k)
	if (*istk(lib + k - 2 +1) >= 0)  ++nbloc;
      CreateVar(6,MATRIX_OF_DOUBLE_DATATYPE, &nbloc, &cx1, &lbs);
      ix = 0;
      for (k = 1; k <= n; ++k) {
	if (*istk(lib + k - 2 +1) >= 0) {
	  *stk(lbs + ix ) = (double) *istk(lib + k - 2 +1);
	  ++ix;
	}
      }
    }
    LhsVar(1) = 1;
    LhsVar(2) = 2;
    LhsVar(3) = 6;
    return 0;
} /* intbdiagr_ */

/*--------------------------------------------------------------------------*/
