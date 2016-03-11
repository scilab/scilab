/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Cong WU
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

/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "sciprint.h"
#include "configvariable_interface.h"
#include "Sciwarning.h"

/*--------------------------------------------------------------------------*/
int sci_log10(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i = 0;
    int complex = 0;
    int check_ieee = 0;

    int* piAddr = NULL;
    int iType = 0;
    int iRows = 0;
    int iCols = 0;
    double* pIn = NULL;
    double* pOutR = NULL;
    double* pOutI = NULL;

    // Wolfram Alpha : Pi / log(10)
    const double imag = 1.364376353841841347485783625431355770210127483723925399900;
    // Wolfram Alpha : 1 / log(10)
    const double inverseLog10 = 0.434294481903251827651128918916605082294397005803666566114;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType != sci_matrix || isVarComplex(pvApiCtx, piAddr))
    {
        //manage complex log10 via %s_log macro
        OverLoad(1);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pIn);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //check values
    for (i = 0 ; i < iRows * iCols ; ++i)
    {
        double d = pIn[i];
        if (d < 0)
        {
            complex = 1;
        }

        if (d == 0)
        {
            check_ieee = 1;
        }
    }

    if (check_ieee)
    {
        if (getieee() == 0)
        {
            Scierror(999, "Singularity of log or tan function.\n");
            return 0;
        }

        if (getieee() == 1)
        {
            Sciwarning(_("Warning : singularity of 'log' or 'tan' function.\n"));
        }
    }

    if (complex)
    {
        sciErr = allocComplexMatrixOfDouble(pvApiCtx, iRhs + 1, iRows, iCols, &pOutR, &pOutI);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        memset(pOutI, 0x00, iRows * iCols * sizeof(double));
    }
    else
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, iRhs + 1, iRows, iCols, &pOutR);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }


    for (i = 0 ; i < iRows * iCols ; ++i)
    {
        if (pIn[i] < 0)
        {
            // log10 = log * 1/log(10)
            pOutR[i] = log(-pIn[i]) * inverseLog10;
            pOutI[i] = imag;
        }
        else
        {
            // log10 = log * 1/log(10)
            pOutR[i] = log(pIn[i]) * inverseLog10;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

