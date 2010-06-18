/*
 * Scilab( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright(C) INRIA
 * Copyright(C) DIGITEO - 2010 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __ELEMENTARY_FUNCTIONS_H__
#define __ELEMENTARY_FUNCTIONS_H__

#include "machine.h" /* C2F */

/* these functions as fortran subroutines of elementary_functions */

int C2F(dmcopy)(double *a, int *na, double *b, int *nb, int *m, int *n);
int C2F(dset)(int *n, double *dx, double *dy, int *incy);
int C2F(wwrdiv)(double *ar, double *ai, int *ia, double *br, double *bi, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr);
int C2F(dmmul)(double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n);
int C2F(dsort)(double *count, int *n, int *index);

#endif /* __ELEMENTARY_FUNCTIONS_H__ */

