/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/
#include <string.h>
#include "gw_interpolation.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "interpolation.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern double C2F(db3val)(double *xval, double *yval, double *zval, int *idx, int *idy, int *idz,
    double *tx, double *ty, double *tz, int *nx, int *ny, int *nz,
    int *kx, int *ky, int *kz, double *bcoef, double *work);
/*--------------------------------------------------------------------------*/
int intbsplin3val(char *fname, unsigned long fname_len)
{
    /*
    *   [fp] = bsplin3val(xp, yp, zp, tlcoef, der)
    */

    int minrhs = 5, maxrhs = 5, minlhs = 1, maxlhs = 1;

    int mxp = 0, nxp = 0, lxp = 0, myp = 0, nyp = 0, lyp = 0, mzp = 0, nzp = 0, lzp = 0;
    int  mt = 0, nt = 0, lt = 0, m1 = 0, n1 = 0, np = 0;
    int one = 1, kx = 0, ky = 0, kz = 0;
    int nx = 0, ny = 0, nz = 0, nxyz = 0, mtx = 0, mty = 0, mtz = 0, m = 0, n = 0, ltx;
    int lty = 0, ltz = 0, lbcoef = 0, mwork = 0, lwork = 0, lfp = 0;
    int i = 0, mder = 0, nder = 0, lder = 0, ox = 0, oy = 0, oz = 0;
    double *fp = NULL, *xp = NULL, *yp = NULL, *zp = NULL, *der = NULL;
    SciIntMat Order;
    int *order = NULL;
    char **Str = NULL;;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lxp);
    xp = stk(lxp);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &myp, &nyp, &lyp);
    yp = stk(lyp);
    GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mzp, &nzp, &lzp);
    zp = stk(lzp);

    for (i = 1; i <= 3; i++)
    {
        SciErr sciErr;
        int *piAddressVar = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddressVar);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddressVar))
        {
            Scierror(202, _("%s: Wrong type for argument %d: Real matrix expected.\n"), fname, i);
            return 0;
        }
    }

    if ( mxp != myp  ||  nxp != nyp || mxp != mzp  ||  nxp != nzp)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d, #%d and #%d: Same sizes expected.\n"), fname, 1, 2, 3);
        return 0;
    }
    np = mxp * nxp;

    GetRhsVar(4, TYPED_LIST_DATATYPE, &mt, &nt, &lt);
    GetListRhsVar(4, 1, MATRIX_OF_STRING_DATATYPE, &m1,  &n1, &Str);

    if ( strcmp(Str[0], "tensbs3d") != 0)
    {
        /* Free Str */
        if (Str)
        {
            int li = 0;
            while (Str[li] != NULL)
            {
                FREE(Str[li]);
                li++;
            };
            FREE(Str);
            Str = NULL;
        }
        Scierror(999, _("%s: Wrong type for input argument #%d: %s tlist expected.\n"), fname, 4, "tensbs3d");
        return 0;
    }
    /* Free Str */
    if (Str)
    {
        int li = 0;
        while (Str[li] != NULL)
        {
            FREE(Str[li]);
            li++;
        };
        FREE(Str);
        Str = NULL;
    }

    GetListRhsVar(4, 2, MATRIX_OF_DOUBLE_DATATYPE, &mtx, &n,  &ltx);
    GetListRhsVar(4, 3, MATRIX_OF_DOUBLE_DATATYPE, &mty, &n,  &lty);
    GetListRhsVar(4, 4, MATRIX_OF_DOUBLE_DATATYPE, &mtz, &n,  &ltz);
    GetListRhsVar(4, 5, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m  , &n,  (int *)&Order);
    GetListRhsVar(4, 6, MATRIX_OF_DOUBLE_DATATYPE, &nxyz, &n,  &lbcoef);

    GetRhsVar(5, MATRIX_OF_DOUBLE_DATATYPE, &mder, &nder, &lder);
    der = stk(lder);
    if (   mder*nder != 3
        || der[0] != floor(der[0]) || der[0] < 0.0
        || der[1] != floor(der[1]) || der[1] < 0.0
        || der[2] != floor(der[2]) || der[2] < 0.0 )
    {
        Scierror(999, _("%s: Wrong values for input argument #%d.\n"), fname, 5);
        return 0;
    }
    ox = (int) der[0];
    oy = (int) der[1];
    oz = (int) der[2];

    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lfp);
    fp = stk(lfp);

    order = (int *)Order.D;
    kx = order[0];
    ky = order[1];
    kz = order[2];
    nx = mtx - kx;
    ny = mty - ky;
    nz = mtz - kz;

    mwork = ky * kz + 3 * Max(kx, Max(ky, kz)) + kz;
    CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &mwork, &one, &lwork);

    for (i = 0; i < np; i++)
    {
        fp[i] = C2F(db3val)(&(xp[i]), &(yp[i]), &(zp[i]), &ox, &oy, &oz,
            stk(ltx), stk(lty), stk(lty), &nx, &ny, &nz,
            &kx, &ky, &kz, stk(lbcoef), stk(lwork));
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
