
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

int C2F(setgmn) (double *meanv, double *covm, int *ldcovm, int *p, double *parm, int *ierr);
double C2F(genbet) (double *aa, double *bb);
double C2F(genchi) (double *df);
double C2F(genexp) (double *av);
double C2F(genf) (double *dfn, double *dfd);
double C2F(gengam) (double *a, double *r__);
double C2F(gennch) (double *df, double *xnonc);
double C2F(gennf) (double *dfn, double *dfd, double *xnonc);
double C2F(gennor) (double *av, double *sd);
double C2F(sdot) (int *n, double *sx, int *incx, double *sy, int *incy);
double C2F(sexpo) (void);
double C2F(sgamma) (double *a);
double C2F(snorm) (void);
double igngeom(double p);
int C2F(genmn) (double *parm, double *x, double *work);
int C2F(genmul) (int *n, double *p, int *ncat, int *ix);
int C2F(genprm) (double *array, int *larray);
int C2F(ignbin) (int *n, double *pp);
int C2F(ignnbn) (int *n, double *p);
int C2F(ignpoi) (double *mu);
int C2F(phrtsd) (char *phrase, int *phrasel, int *seed1, int *seed2, int phrase_len);
int C2F(spofa) (double *a, int *lda, int *n, int *info);
double C2F(ranf)(void);
double ignlgi(void);
double C2F(ignuin)(double *a, double *b);

#endif /** SCI_GRAND   **/

