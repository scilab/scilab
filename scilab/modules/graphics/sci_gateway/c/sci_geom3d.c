/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_geom3d.c                                                     */
/* desc : interface for geom3d routine                                    */
/*------------------------------------------------------------------------*/
#include "stack-c.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "HandleManagement.h"

#include "BuildObjects.h"

int geom3d(double *x, double *y, double *z, int n);

/*--------------------------------------------------------------------------*/
int geom3d(double *x, double *y, double *z, int n)
{
    int i = 0;
    double userCoords2d[2];

    char * psubwinUID = (char*)getOrCreateDefaultSubwin();

    for (i = 0; i < n; i++)
    {
        double userCoords[3] = {x[i], y[i], z[i]};
        sciGet2dViewCoordinate(psubwinUID, userCoords, userCoords2d);
        x[i] = userCoords2d[0];
        y[i] = userCoords2d[1];
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int sci_geom3d(char * fname, unsigned long fname_len)
{
    int ix1 = 0, m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0, m3 = 0, n3 = 0, l3 = 0;

    CheckRhs(3,3);
    CheckLhs(2,3);

    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
    CheckSameDims(1,2,m1,n1,m2,n2);
    CheckSameDims(2,3,m2,n2,m3,n3);
    if (m1 * n1 == 0)
    {
        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }

    ix1 = m1 * n1;
    geom3d(stk(l1), stk(l2), stk(l3), ix1);

    LhsVar(1) = 1;
    LhsVar(2) = 2;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
