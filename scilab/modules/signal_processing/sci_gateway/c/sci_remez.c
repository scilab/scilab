/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
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

#include "gw_signal.h"
#include "api_scilab.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"

extern void C2F(remez) (int *ngrid, int *nc, int *iext,
                        double *ad, double *x, double *y, float *des, float *grid, float *wt, double *a, double *p, double *q, double *alpha);

/* allocates required buffers and calls remez */
int remez_buffered(int ngrid, int nc, int *iext, float *des, float *grid, float *wt, double *output);

int sci_remez(char *fname, void *pvApiCtx)
{
    /************************************************
     * Warning : bug 4189                           *
     * The remez function returns                   *
     * an array which last element is meaningless.  *
     * -> sementic of the fortran gw preserved      *
     * -> watch the curious nc's cooking            *
     ************************************************/

    int rows, cols, length, ngrid = 0, nc = 0, error = 0;
    double *output = NULL, *argument = NULL;
    float *des = NULL, *grid = NULL, *wt = NULL;
    int *iext;
    int *p;

    CheckRhs(4, 4);
    CheckLhs(1, 1);

    // GetRhsVarMatrixDouble(1, &rows, &cols, &argument);
    getVarAddressFromPosition(pvApiCtx, 1, &p);
    getMatrixOfDouble(pvApiCtx, p, &rows, &cols, &argument);
    iext = (int *)argument;
    nc = cols * rows;
    C2F(entier) (&nc, argument, iext);

    // GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
    getVarAddressFromPosition(pvApiCtx, 2, &p);
    getMatrixOfDouble(pvApiCtx, p, &rows, &cols, &argument);
    des = (float *)argument;
    ngrid = cols * rows;
    length = rows;
    C2F(simple) (&ngrid, argument, des);

    // GetRhsVarMatrixDouble(3, &rows, &cols, &argument);
    getVarAddressFromPosition(pvApiCtx, 3, &p);
    getMatrixOfDouble(pvApiCtx, p, &rows, &cols, &argument);
    grid = (float *)argument;
    C2F(simple) (&ngrid, argument, grid);

    // GetRhsVarMatrixDouble(4, &rows, &cols, &argument);
    getVarAddressFromPosition(pvApiCtx, 4, &p);
    getMatrixOfDouble(pvApiCtx, p, &rows, &cols, &argument);
    wt = (float *)argument;
    C2F(simple) (&ngrid, argument, wt);

    // iAllocMatrixOfDouble(Rhs + 1, rows, nc - 1, &output);
    allocMatrixOfDouble(pvApiCtx, Rhs + 1, rows, nc - 1, &output);
    //createMatrixOfDouble(pvApiCtx, Rhs + 1, rows, nc - 1, output);

    error = remez_buffered(ngrid, nc - 2, iext, des, grid, wt, output);
    if (error)
    {
        Scierror(999, _("%s : Memory allocation error.\n"), fname);
        return 1;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}

int remez_buffered(int ngrid, int nc, int *iext, float *des, float *grid, float *wt, double *output)
{
    int one = 1;
    double *buffer0 = NULL, *buffer1 = NULL, *buffer2 = NULL, *buffer3 = NULL, *buffer4 = NULL, *buffer5 = NULL, *buffer6 = NULL;

    buffer0 = (double *)MALLOC((nc + 2) * sizeof(double));
    buffer1 = (double *)MALLOC((nc + 2) * sizeof(double));
    buffer2 = (double *)MALLOC((nc + 2) * sizeof(double));
    buffer3 = (double *)MALLOC((nc + 2) * sizeof(double));
    buffer4 = (double *)MALLOC((nc + 2) * sizeof(double));
    buffer5 = (double *)MALLOC((nc + 2) * sizeof(double));
    buffer6 = (double *)MALLOC((nc + 2) * sizeof(double));
    if (buffer0 == NULL || buffer1 == NULL || buffer2 == NULL || buffer3 == NULL || buffer4 == NULL || buffer5 == NULL || buffer6 == NULL)
    {
        FREE(buffer0);
        FREE(buffer1);
        FREE(buffer2);
        FREE(buffer3);
        FREE(buffer4);
        FREE(buffer5);
        FREE(buffer6);
        return 1;
    }

    C2F(remez) (&ngrid, &nc, iext, buffer1, buffer2, buffer3, des, grid, wt, buffer4, buffer5, buffer6, buffer0);
    nc++;
    C2F(dcopy) (&nc, buffer0, &one, output, &one);

    FREE(buffer0);
    FREE(buffer6);
    FREE(buffer5);
    FREE(buffer4);
    FREE(buffer3);
    FREE(buffer2);
    FREE(buffer1);

    return 0;
}
