
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef SCI_GRAND
#define SCI_GRAND

#include "dynlib_randlib.h"
#include "machine.h"

RANDLIB_IMPEXP int C2F(setgmn) (double *meanv, double *covm, int *ldcovm, int *p, double *parm, int *ierr);
RANDLIB_IMPEXP double C2F(genbet) (double *aa, double *bb);
RANDLIB_IMPEXP double C2F(genchi) (double *df);
RANDLIB_IMPEXP double C2F(genexp) (double *av);
RANDLIB_IMPEXP double C2F(genf) (double *dfn, double *dfd);
RANDLIB_IMPEXP double C2F(gengam) (double *a, double *r__);
RANDLIB_IMPEXP double C2F(gennch) (double *df, double *xnonc);
RANDLIB_IMPEXP double C2F(gennf) (double *dfn, double *dfd, double *xnonc);
RANDLIB_IMPEXP double C2F(gennor) (double *av, double *sd);
RANDLIB_IMPEXP double C2F(sdot) (int *n, double *sx, int *incx, double *sy, int *incy);
RANDLIB_IMPEXP double C2F(sexpo) (void);
RANDLIB_IMPEXP double C2F(sgamma) (double *a);
RANDLIB_IMPEXP double C2F(snorm) (void);
RANDLIB_IMPEXP double igngeom(double p);
RANDLIB_IMPEXP int C2F(genmn) (double *parm, double *x, double *work);
RANDLIB_IMPEXP int C2F(genmul) (int *n, double *p, int *ncat, int *ix);
RANDLIB_IMPEXP int C2F(genprm) (double *array, int *larray);
RANDLIB_IMPEXP int C2F(ignbin) (int *n, double *pp);
RANDLIB_IMPEXP int C2F(ignnbn) (int *n, double *p);
RANDLIB_IMPEXP int C2F(ignpoi) (double *mu);
RANDLIB_IMPEXP int C2F(phrtsd) (char *phrase, int *phrasel, int *seed1, int *seed2, int phrase_len);
RANDLIB_IMPEXP int C2F(spofa) (double *a, int *lda, int *n, int *info);
RANDLIB_IMPEXP double C2F(ranf)(void);
RANDLIB_IMPEXP double ignlgi(void);
RANDLIB_IMPEXP double C2F(ignuin)(double *a, double *b);

#endif /** SCI_GRAND   **/

