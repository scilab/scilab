/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009-2009 - Antoine ELIAS <antoine.elias@scilab.org>
 * Copyright (C) DIGITEO - 2009-2010 - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "callxcos.h"
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
int
sci_xcosDiagramToHDF5(char *fname, unsigned long fname_len)
{
    CheckRhs(3, 3);
    CheckLhs(0, 1);

    SciErr sciErr;

    int iRows1 = 0;
    int iCols1 = 0;
    int iLen1 = 0;
    int* piAddr1 = NULL;
    char* pstXcosFile = NULL;

    int iVarType2 = 0;

    int iRows2 = 0;
    int iCols2 = 0;
    int iLen2 = 0;
    int* piAddr2 = NULL;
    char* pstH5File = NULL;

    int iRows3 = 0;
    int iCols3 = 0;
    int* piAddr3 = NULL;
    int* piForceWrite = NULL;

    bool bForceWrite = false;

    //get xcos filename
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //get xcos filename matrix dimension
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iRows1 != 1 || iCols1 != 1)
    {
        //Error String matrix 1,1 excepted
    }

    //get xcos filename length
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, &iLen1,
            NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstXcosFile = (char*) MALLOC(sizeof(char*) * (iLen1 + 1));//+ 1 for null termination
    //get xcos filename
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, &iLen1,
            &pstXcosFile);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //get h5 filename
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //get h5 filename matrix dimension
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows2, &iCols2, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iRows2 != 1 || iCols2 != 1)
    {
        //Error String matrix 1,1 excepted
    }

    //get xcos filename length
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows2, &iCols2, &iLen2,
            NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstH5File = (char*) MALLOC(sizeof(char*) * (iLen2 + 1));//+ 1 for null termination
    //get xcos filename
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows2, &iCols2, &iLen2,
            &pstH5File);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //get force writing flag
    //get h5 filename
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarDimension(pvApiCtx, piAddr3, &iRows3, &iCols3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfBoolean(pvApiCtx, piAddr3, &iRows3, &iCols3,
            &piForceWrite);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iRows3 != 1 || iCols3 != 1)
    {
        //Error boolean matrix 1,1 excepted
    }

    bForceWrite = piForceWrite[0] ? true : false;

    /*
     * Call the Java implementation
     */
    int iRet = 0;
    try
    {
        iRet = Xcos::xcosDiagramToHDF5(getScilabJavaVM(), pstXcosFile,
                pstH5File, bForceWrite);
    }
    catch (GiwsException::JniCallMethodException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.what());
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
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
