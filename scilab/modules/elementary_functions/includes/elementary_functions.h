/*
 * Scilab( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright(C) INRIA
 * Copyright(C) DIGITEO - 2010 - Allan CORNET

 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __ELEMENTARY_FUNCTIONS_H__
#define __ELEMENTARY_FUNCTIONS_H__

#include "machine.h" /* C2F */

/* these functions as fortran subroutines of elementary_functions */

int C2F(dmcopy)(double *a, int *na, double *b, int *nb, int *m, int *n);
//int C2F(dset)(int *n, double *dx, double *dy, int *incy);
int C2F(wwrdiv)(double *ar, double *ai, int *ia, double *br, double *bi, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr);
int C2F(dmmul)(double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n);
int C2F(dsort)(double *count, int *n, int *index);

#endif /* __ELEMENTARY_FUNCTIONS_H__ */
