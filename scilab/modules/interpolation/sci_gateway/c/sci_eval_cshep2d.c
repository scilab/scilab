/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/
#include <string.h>
#include "api_scilab.h"
#include "stack-c.h"
#include "gw_interpolation.h"
#include "interpolation.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern double C2F(cs2val)(double *px, double *py, int *n, double *x, double *y,
                          double *f, int *nr, int *lcell, int *lnext, double *xmin, double *ymin,
                          double *dx, double *dy, double *rmax, double *rw, double *a);
extern int C2F(cs2grd) (double *px, double *py, int *n, double *x, double *y,
                        double *f, int *nr, int *lcell, int *lnext,  double *xmin, double *ymin,
                        double *dx, double *dy, double *rmax, double *rw, double *a, double *c,
                        double *cx, double *cy, int *ier);
extern int C2F(cs2hes)  (double *px, double *py, int *n, double *x, double *y,
                         double *f, int *nr, int *lcell, int *lnext,  double *xmin, double *ymin,
                         double *dx, double *dy, double *rmax, double *rw, double *a, double *c,
                         double *cx, double *cy,  double *cxx, double *cxy, double *cyy, int *ier);
/*--------------------------------------------------------------------------*/
int inteval_cshep2d(char *fname, unsigned long fname_len)
{
    /*
    *   [f [,dfdx, dfdy [, dffdxx, dffdxy, dffdyy]]] = eval_cshep2d(xp, yp, tlcoef)
    */

    int minrhs = 3, maxrhs = 3, minlhs = 1, maxlhs = 6;
    int mx = 0, nx = 0, lx = 0, my = 0, ny = 0, ly = 0, mt = 0, nt = 0, lt = 0;
    char **Str = NULL;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0, m4 = 0, n4 = 0, m5 = 0, n5 = 0, m6 = 0, n6 = 0, m7 = 0, n7 = 0, m8 = 0, n8 = 0;
    int lxyz = 0, lgrid = 0, lrmax = 0, lrw = 0, la = 0;
    double *xp = NULL, *yp = NULL, *xyz = NULL, *grid = NULL, *f = NULL, *dfdx = NULL, *dfdy = NULL, *dffdxx = NULL, *dffdyy = NULL, *dffdxy = NULL;
    int i = 0, ier = 0, n = 0, np = 0, nr = 0, lf = 0, ldfdx = 0, ldfdy = 0, ldffdxx = 0, ldffdyy = 0, ldffdxy = 0;
    SciIntMat Cell, Next;
    int *cell = NULL, *next = NULL;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);

    for (i = 1; i <= minrhs - 1; i++)
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
            Scierror(202, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), fname, i);
            return 0;
        }
    }

    if ( mx != my  ||  nx != ny )
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same sizes expected.\n"), fname, 1, 2);
        return 0;
    }

    GetRhsVar(3, TYPED_LIST_DATATYPE, &mt, &nt, &lt);
    GetListRhsVar(3, 1, MATRIX_OF_STRING_DATATYPE, &m1,  &n1, &Str);   /* m1 = 1, n1 = 8 ? a verifier */
    if ( strcmp(Str[0], "cshep2d") != 0)
    {
        /* Free Str */
        if (Str)
        {
            int li = 0;
            while ( Str[li] != NULL)
            {
                FREE(Str[li]);
                Str[li] = NULL;
                li++;
            };
            FREE(Str);
            Str = NULL;
        }
        Scierror(999, _("%s: Wrong type for input argument #%d: %s tlist expected.\n"), fname, 2, "cshep2d");
        return 0;
    }
    /* Free Str */
    if (Str)
    {
        int li = 0;
        while ( Str[li] != NULL)
        {
            FREE(Str[li]);
            Str[li] = NULL;
            li++;
        };
        FREE(Str);
        Str = NULL;
    }
    GetListRhsVar(3, 2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2,  &lxyz);  /* m2 = n , n2 = 3  */
    GetListRhsVar(3, 3, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m3, &n3,  (int *)&Cell); /* m3 = nr, n3 = nr */
    GetListRhsVar(3, 4, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m4, &n4,  (int *)&Next); /* m4 = 1 , n4 = n  */
    GetListRhsVar(3, 5, MATRIX_OF_DOUBLE_DATATYPE, &m5, &n5,  &lgrid); /* m5 = 1 , n5 = 4  */
    GetListRhsVar(3, 6, MATRIX_OF_DOUBLE_DATATYPE, &m6, &n6,  &lrmax); /* m6 = 1 , n6 = 1  */
    GetListRhsVar(3, 7, MATRIX_OF_DOUBLE_DATATYPE, &m7, &n7,  &lrw);   /* m7 = 1 , n7 = n  */
    GetListRhsVar(3, 8, MATRIX_OF_DOUBLE_DATATYPE, &m8, &n8,  &la);    /* m8 = 9 , n8 = n  */

    cell = (int *)Cell.D;
    next = (int *)Next.D;
    xp = stk(lx);
    yp = stk(ly);
    np = mx * nx;
    n = m2;
    nr = m3;
    xyz = stk(lxyz);
    grid = stk(lgrid);

    CreateVar(4, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lf);
    f = stk(lf);
    if ( Lhs > 1 )
    {
        CreateVar(5, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldfdx);
        dfdx = stk(ldfdx);
        CreateVar(6, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldfdy);
        dfdy = stk(ldfdy);
    }
    if ( Lhs > 3 )
    {
        CreateVar(7, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldffdxx);
        dffdxx = stk(ldffdxx);
        CreateVar(8, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldffdxy);
        dffdyy = stk(ldffdxy);
        CreateVar(9, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldffdyy);
        dffdxy = stk(ldffdyy);
    }

    switch ( Lhs )
    {
        case ( 1 ) :
            for ( i = 0 ; i < np ; i++ )
                /*            DOUBLE PRECISION FUNCTION CS2VAL (PX,PY,N,X,Y,F,NR,
                *                          LCELL,LNEXT,XMIN,YMIN,DX,DY,RMAX,RW,A)
                */
                f[i] = C2F(cs2val)(&xp[i], &yp[i], &n, xyz, &xyz[n], &xyz[2 * n], &nr,
                                   cell, next, grid, &grid[1], &grid[2], &grid[3],
                                   stk(lrmax), stk(lrw), stk(la));
            LhsVar(1) = 4;
            break;

        case ( 2 ) :
        case ( 3 ) :
            for ( i = 0 ; i < np ; i++ )
                /*      SUBROUTINE CS2GRD (PX,PY,N,X,Y,F,NR,LCELL,LNEXT,XMIN,
                *.                   YMIN,DX,DY,RMAX,RW,A, C,CX,CY,IER)
                */
                C2F(cs2grd) (&xp[i], &yp[i], &n, xyz, &xyz[n], &xyz[2 * n], &nr,
                             cell, next, grid, &grid[1], &grid[2], &grid[3],
                             stk(lrmax), stk(lrw), stk(la), &f[i], &dfdx[i], &dfdy[i], &ier);
            LhsVar(1) = 4;
            LhsVar(2) = 5;
            LhsVar(3) = 6;
            break;

        case ( 4 ) :
        case ( 5 ) :
        case ( 6 ) :
            for ( i = 0 ; i < np ; i++ )
            {
                /*   SUBROUTINE CS2HES (PX,PY,N,X,Y,F,NR,LCELL,LNEXT,XMIN,
                *.                     YMIN,DX,DY,RMAX,RW,A, C,CX,CY,CXX,CXY,CYY,IER)
                */
                C2F(cs2hes) (&xp[i], &yp[i], &n, xyz, &xyz[n], &xyz[2 * n], &nr,
                             cell, next, grid, &grid[1], &grid[2], &grid[3],
                             stk(lrmax), stk(lrw), stk(la), &f[i], &dfdx[i], &dfdy[i],
                             &dffdxx[i], &dffdxy[i], &dffdyy[i], &ier);
            }
            LhsVar(1) = 4;
            LhsVar(2) = 5;
            LhsVar(3) = 6;
            LhsVar(4) = 7;
            LhsVar(5) = 8;
            LhsVar(6) = 9;
            break;
    }
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
