/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "elem_common.h" // dlamch

extern void   C2F(dgefa) (double *A, int *lead_dim_A, int *n, int *ipivots, int *info);
extern void   C2F(dgesl) (double *A, int *lead_dim_A, int *n, int *ipivots, double *B, int *job);

typedef void (*resfunc)(double*, double*, double*, double*, int*, double*, int*);

void pjac1( resfunc res, int *ires, int *nequations, double *tOld, double *actual, double *actualP,
            double *rewt, double *savr, double *wk, double *h, double *cj, double *wp, int *iwp,
            int *ier, double *rpar, int *ipar);

void psol1( int *nequations, double *tOld, double *actual, double *actualP,
            double *savr, double *wk, double *cj, double *wght, double *wp,
            int *iwp, double *b, double *eplin, int *ier, double *dummy1, int *dummy2);

