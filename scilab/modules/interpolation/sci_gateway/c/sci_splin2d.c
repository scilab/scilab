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
#include "gw_interpolation.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(bicubicsubspline)(double *x, double *y, double *z, int *nx,
                                 int *ny, double *C, double *p, double *q, double *r, int *spline_type);
extern int C2F(bicubicspline)(double *x, double *y, double *u, int *nx, int *ny,
                              double *C, double *p, double *q, double *r, double *A_d, double *A_sd,
                              double *d, double *ll, double *qdu, double *u_temp, int *spline_type);
/*--------------------------------------------------------------------------*/
#define NB_SPLINE_TYPE 7
static TableType SplineTable[NB_SPLINE_TYPE] =
{
    { "not_a_knot"   , NOT_A_KNOT    },
    { "natural"      , NATURAL       },
    { "clamped"      , CLAMPED       },
    { "periodic"     , PERIODIC      },
    { "monotone"     , MONOTONE      },
    { "fast"         , FAST          },
    { "fast_periodic", FAST_PERIODIC }
};
/*--------------------------------------------------------------------------*/
int intsplin2d(char *fname, unsigned long fname_len)
{
    /*    interface pour splin2d :
    *
    *    C = splin2d(x, y, z [, type])
    *
    */

    int minrhs = 3, maxrhs = 4, minlhs = 1, maxlhs = 1;

    int mx = 0, nx = 0, lx = 0, my = 0, ny = 0, ly = 0, mz = 0, nz = 0;
    int lz = 0, ns = 0, mc = 0, nc = 0, lc = 0, lp = 0, lq = 0, lr = 0;
    int spline_type = 0, *str_spline_type = NULL;
    int one = 1;
    double *x = NULL, *y = NULL;
    int i = 0;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
    GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mz, &nz, &lz);

    for (i = 1; i <= minrhs; i++)
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

    if ( mx != 1 || my != 1 || mz != nx || nz != ny || nx < 2 || ny < 2)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d, #%d or #%d.\n"), fname, 1, 2, 3);
        return 0;
    }

    /* verify strict increasing order for x and y */
    x = stk(lx);
    y = stk(ly);
    if ( !good_order(x, nx) || !good_order(y, ny))
    {
        Scierror(999, _("%s: x and/or y are not in strict increasing order (or +-inf detected)\n"), fname);
        return 0;
    }

    /* get the spline type */
    if ( Rhs == 4 )
    {
        GetRhsScalarString(4, &ns, &str_spline_type);
        spline_type = get_type(SplineTable, NB_SPLINE_TYPE, str_spline_type, ns);
        if ( spline_type == UNDEFINED || spline_type == CLAMPED )
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Unsupported '%s' type.\n"), fname, 4, "spline");
            return 0;
        };
    }
    else
    {
        spline_type = NOT_A_KNOT;
    }

    /* memory for the big C array */
    mc = 16 * (nx - 1) * (ny - 1);
    nc = 1;
    CreateVar( Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &mc,  &nc, &lc);
    /* memory for work arrays  */
    CreateVar( Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &nx, &ny, &lp);
    CreateVar( Rhs + 3, MATRIX_OF_DOUBLE_DATATYPE, &nx, &ny, &lq);
    CreateVar( Rhs + 4, MATRIX_OF_DOUBLE_DATATYPE, &nx, &ny, &lr);

    if (spline_type == MONOTONE || spline_type == FAST || spline_type == FAST_PERIODIC)
    {
        C2F(bicubicsubspline)(x, y, stk(lz), &nx, &ny, stk(lc),
                              stk(lp), stk(lq), stk(lr), &spline_type);
    }

    else   /*  spline */
    {
        int lA_d, lA_sd, ld, lqdu, lutemp, nxy, nxym1, nxym2, lll;

        nxy = Max(nx, ny);
        nxym1 = nxy - 1;
        nxym2 = nxy - 2;

        CreateVar( Rhs + 5, MATRIX_OF_DOUBLE_DATATYPE, &nxy,   &one, &lA_d);
        CreateVar( Rhs + 6, MATRIX_OF_DOUBLE_DATATYPE, &nxym1, &one, &lA_sd);
        CreateVar( Rhs + 7, MATRIX_OF_DOUBLE_DATATYPE, &ny,    &one, &ld);
        CreateVar( Rhs + 8, MATRIX_OF_DOUBLE_DATATYPE, &nxy,   &one, &lqdu);
        CreateVar( Rhs + 9, MATRIX_OF_DOUBLE_DATATYPE, &ny,    &one, &lutemp);

        if (spline_type == PERIODIC)
        {
            CreateVar(Rhs + 10, MATRIX_OF_DOUBLE_DATATYPE, &nxym2, &one, &lll);
        }
        else
        {
            lll = lA_sd ;    /* bidon ... */
        }
        C2F(bicubicspline)(x, y, stk(lz), &nx, &ny, stk(lc), stk(lp), stk(lq), stk(lr),
                           stk(lA_d), stk(lA_sd), stk(ld), stk(lll), stk(lqdu),
                           stk(lutemp), &spline_type);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/


