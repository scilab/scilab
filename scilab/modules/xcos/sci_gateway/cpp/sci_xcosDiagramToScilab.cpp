/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009-2009 - Antoine ELIAS <antoine.elias@scilab.org>
 * Copyright (C) DIGITEO - 2009-2010 - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;

/*--------------------------------------------------------------------------*/
int sci_xcosDiagramToScilab(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    SciErr sciErr;

    int iRows1 = 0;
    int iCols1 = 0;
    int iLen1 = 0;
    int *piAddr1 = NULL;
    char *pstXcosFile = NULL;

    //get xcos filename
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    //get xcos filename matrix dimension
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iRows1 * iCols1 != 1)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
    }

    //get xcos filename length
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, &iLen1, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    pstXcosFile = (char *)MALLOC(sizeof(char *) * (iLen1 + 1)); //+ 1 for null termination
    //get xcos filename
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, &iLen1, &pstXcosFile);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    /*
     * Call the Java implementation
     */
    int iRet = 0;

    try
    {
        iRet = Xcos::xcosDiagramToScilab(getScilabJavaVM(), pstXcosFile);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
        return 0;
    }

    if (iRet != 0)
    {
        //manage error
        iRet = 1;
    }

    double dblResult = iRet;

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &dblResult);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
