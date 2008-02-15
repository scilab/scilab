
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#ifndef SCI_GRAND
#define SCI_GRAND

int C2F(setgmn) __PARAMS((double *meanv, double *covm, int *ldcovm, int *p, double *parm, int *ierr));
double C2F(genbet) __PARAMS((double *aa, double *bb));
double C2F(genchi) __PARAMS((double *df));
double C2F(genexp) __PARAMS((double *av));
double C2F(genf) __PARAMS((double *dfn, double *dfd));
double C2F(gengam) __PARAMS((double *a, double *r__));
double C2F(gennch) __PARAMS((double *df, double *xnonc));
double C2F(gennf) __PARAMS((double *dfn, double *dfd, double *xnonc));
double C2F(gennor) __PARAMS((double *av, double *sd));
double C2F(sdot) __PARAMS((int *n, double *sx, int *incx, double *sy, int *incy));
double C2F(sexpo) __PARAMS((void));
double C2F(sgamma) __PARAMS((double *a));
double C2F(snorm) __PARAMS((void));
double igngeom(double p);
int C2F(genmn) __PARAMS((double *parm, double *x, double *work));
int C2F(genmul) __PARAMS((int *n, double *p, int *ncat, int *ix));
int C2F(genprm) __PARAMS((double *array, int *larray));
int C2F(ignbin) __PARAMS((int *n, double *pp));
int C2F(ignnbn) __PARAMS((int *n, double *p));
int C2F(ignpoi) __PARAMS((double *mu));
int C2F(phrtsd) __PARAMS((char *phrase, int *phrasel, int *seed1, int *seed2, int phrase_len));
int C2F(spofa) __PARAMS((double *a, int *lda, int *n, int *info));
double C2F(ranf)(void);
double ignlgi(void);
double C2F(ignuin)(double *a, double *b);

#endif /** SCI_GRAND   **/

