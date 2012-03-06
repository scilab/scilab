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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_interpolation.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(bicubicinterp)(double *x, double *y, double *C, int *nx,
    int *ny,double *x_eval, double *y_eval, double *z_eval, int *m,int *outmode);
extern int C2F(bicubicinterpwithgradandhes)();
extern int C2F(bicubicinterpwithgrad)();
/*--------------------------------------------------------------------------*/
#define NB_OUTMODE 6
static TableType OutModeTable[NB_OUTMODE] = {
    { "C0"        , C0         },
    { "by_zero"   , BY_ZERO    },
    { "natural"   , NATURAL    },
    { "periodic"  , PERIODIC   },
    { "by_nan"    , BY_NAN     },
    { "linear"    , LINEAR     }
};
/*--------------------------------------------------------------------------*/
int intinterp2d(char *fname,unsigned long fname_len)
{
    /*    interface pour interp2d :
    *
    *    [zp [, dzdxp, dzdyp [, d2zdx2, d2zdxy, d2zdy2]]] = interp2d(xp, yp, x, y, C[, outmode])
    */

    int minrhs = 5, maxrhs = 6, minlhs = 1, maxlhs = 6;

    int mxp = 0, nxp = 0, lxp = 0, myp = 0, nyp = 0, lyp = 0, mx = 0, nx = 0, lx = 0, my = 0, ny = 0, ly = 0;
    int mc = 0, nc = 0, lc = 0, ns = 0;
    int *str_outmode = NULL;
    int lzp = 0, ldzdxp = 0, ldzdyp = 0, ld2zdx2p = 0, ld2zdxyp = 0, ld2zdy2p = 0;
    int outmode = 0, m = 0;
    int i = 0;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lxp);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &myp, &nyp, &lyp);
    GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
    GetRhsVar(4, MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
    GetRhsVar(5, MATRIX_OF_DOUBLE_DATATYPE, &mc, &nc, &lc);

    for (i = 1; i <= minrhs; i++)
    {
        SciErr sciErr;
        int *piAddressVar = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddressVar);
        if(sciErr.iErr)
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

    if ( mxp != myp || nxp != nyp || mx != 1 || my != 1 || nc != 1 || nx < 2 || ny < 2
        || mc != 16*(nx-1)*(ny-1) )
    {
        Scierror(999, _("%s: Wrong value for input arguments #%d and #%d: Same sizes expected.\n"), fname, 1, 2);
        return 0;
    }

    /* get the outmode */
    if ( Rhs == 6 )
    {
        GetRhsScalarString(6, &ns, &str_outmode);
        outmode =  get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
        if ( outmode == UNDEFINED || outmode == LINEAR )
        {
            Scierror(999,_("%s: Wrong values for input argument #%d: Unsupported '%s' type.\n"), fname, 6, "outmode");
            return 0;
        }
    }
    else
    {
        outmode = C0;
    }

    /* memory for zp */
    CreateVar(Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &lzp);
    m = mxp * nxp;

    if ( Lhs == 1 )
    {
        /*   subroutine BiCubicInterp(x, y, C, nx, ny, x_eval, y_eval, z_eval, m, outmode)
        *     int nx, ny, m, outmode
        *     double precision x(nx), y(ny), C(4,4,nx-1,ny-1), x_eval(m), y_eval(m), z_eval(m)
        */
        C2F(bicubicinterp)(stk(lx), stk(ly), stk(lc), &nx, &ny, stk(lxp), stk(lyp), stk(lzp),
            &m, &outmode);
        LhsVar(1) = Rhs+1;
    }
    else   /* got also the derivatives */
    {
        /* memory for dzdxp and dzdyp */
        CreateVar(Rhs + 2,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ldzdxp);
        CreateVar(Rhs + 3,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ldzdyp);

        if ( Lhs <= 3 )
        {
            C2F(bicubicinterpwithgrad)(stk(lx), stk(ly), stk(lc), &nx, &ny, stk(lxp),
                stk(lyp), stk(lzp), stk(ldzdxp), stk(ldzdyp),
                &m, &outmode);
            LhsVar(1) = Rhs + 1;
            LhsVar(2) = Rhs + 2;
            LhsVar(3) = Rhs + 3;
        }
        else /* got also 2d derivatives */
        {
            CreateVar(Rhs + 4,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ld2zdx2p);
            CreateVar(Rhs + 5,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ld2zdxyp);
            CreateVar(Rhs + 6,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ld2zdy2p);
            C2F(bicubicinterpwithgradandhes)(stk(lx), stk(ly), stk(lc), &nx, &ny, stk(lxp),
                stk(lyp), stk(lzp), stk(ldzdxp), stk(ldzdyp),
                stk(ld2zdx2p), stk(ld2zdxyp), stk(ld2zdy2p),
                &m, &outmode);
            LhsVar(1) = Rhs + 1;
            LhsVar(2) = Rhs + 2;
            LhsVar(3) = Rhs + 3;
            LhsVar(4) = Rhs + 4;
            LhsVar(5) = Rhs + 5;
            LhsVar(6) = Rhs + 6;
        }
    }
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/

