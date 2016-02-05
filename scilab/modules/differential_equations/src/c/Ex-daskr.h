/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
*
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
/*--------------------------------------------------------------------------*/

#ifndef __EX_DASKR_H__
#define __EX_DASKR_H__

extern void   C2F(dgefa) (double *A, int *lead_dim_A, int *n, int *ipivots, int *info);
extern void   C2F(dgesl) (double *A, int *lead_dim_A, int *n, int *ipivots, double *B, int *job);

typedef void (*resfunc)(double*, double*, double*, double*, int*, double*, int*);

void pjac1( resfunc res, int *ires, int *nequations, double *tOld, double *actual, double *actualP,
            double *rewt, double *savr, double *wk, double *h, double *cj, double *wp, int *iwp,
            int *ier, double *rpar, int *ipar);

void psol1( int *nequations, double *tOld, double *actual, double *actualP,
            double *savr, double *wk, double *cj, double *wght, double *wp,
            int *iwp, double *b, double *eplin, int *ier, double *dummy1, int *dummy2);

#endif // __EX_DASKR_H__
