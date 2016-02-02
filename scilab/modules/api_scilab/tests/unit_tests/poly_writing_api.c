/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

int write_poly(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    //output variable info : polinomial matrix 2 x 4
    //[ x + 2                   x^2 - 4x + 5    4x^3 - 14x^2 + 18 ;
    //  2x^3 - 12x^2 + 64       1               8x^5 + 32x^3]
    int iRows               = 2;
    int iCols               = 3;
    //varname "x"
    char pstVarName[2]      = {"x"};
    //coeficient array
    int piNbCoef[6]         = {2, 4, 3, 1, 4, 6};
    //data array
    double *pdblReal[6]     = {0};
    double pdblPoly0[2]     = {2, 1};
    double pdblPoly1[4]     = {64, 0, -12, 2};
    double pdblPoly2[3]     = {5, -4, 1};
    double pdblPoly3[1]     = {1};
    double pdblPoly4[4]     = {18, 0, -14, 4};
    double pdblPoly5[6]     = {0, 0, 0, 32, 0, 8};
    pdblReal[0]             = pdblPoly0;
    pdblReal[1]             = pdblPoly1;
    pdblReal[2]             = pdblPoly2;
    pdblReal[3]             = pdblPoly3;
    pdblReal[4]             = pdblPoly4;
    pdblReal[5]             = pdblPoly5;

    sciErr = createMatrixOfPoly(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstVarName, iRows, iCols, piNbCoef, pdblReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //assign allocated variables to Lhs position
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
