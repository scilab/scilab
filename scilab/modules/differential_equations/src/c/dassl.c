/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
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

/* C driver over ddassl to handle longjump from xerhlt*/
#include "xerhlt.h"
#include "dynlib_differential_equations.h"


extern void C2F(ddassl)(void *res, int *neq, double *t, double *y, double *yprime,
                        double *tout, int *info, double *rtol, double *atol,
                        int *idid, double *rwork, int *lrw, int *iwork, int *liw,
                        double *rpar, int *ipar, void *jac);


DIFFERENTIAL_EQUATIONS_IMPEXP void  C2F(dassl)(void *res, int *neq, double *t, double *y, double *yprime, double *tout,
        int *info, double *rtol, double *atol, int *idid, double *rwork,
        int *lrw, int *iwork, int *liw, double *rpar, int *ipar, void *jac);

void  C2F(dassl)(void *res, int *neq, double *t, double *y, double *yprime, double *tout,
                 int *info, double *rtol, double *atol, int *idid, double *rwork,
                 int *lrw, int *iwork, int *liw, double *rpar, int *ipar, void *jac)
{
    int itemp[12], i;

    if (setjmp_slatec_jmp_env())
    {
        return;
    }

    /* Nest instruction is a patch for sun solaris 5.8,
    without this trick the call to ddassl hangs Scilab */
    for (i = 0; i < 11; i++)
    {
        itemp[i] = info[i];
    }

    C2F(ddassl)(res, neq, t, y, yprime, tout, itemp, rtol, atol, idid, rwork,
                lrw, iwork, liw, rpar, ipar, jac);
}

