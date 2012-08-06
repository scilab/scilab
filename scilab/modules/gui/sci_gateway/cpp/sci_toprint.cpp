/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
*/

/*--------------------------------------------------------------------------*/
#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "api_scilab.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "Scierror.h"
#include "FileExist.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "gw_gui.h"
#include "getScilabJavaVM.h"
#include "getFullFilename.h"
#include "loadOnUseClassPath.h"
#include "warningmode.h"
#include "FigureList.h"
}
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;

/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
static int sci_toprint_one_rhs(void* _pvCtx, const char *fname);
static int sci_toprint_two_rhs(void* _pvCtx, const char *fname);

/*--------------------------------------------------------------------------*/
int sci_toprint(char *fname, unsigned long l)
{
    CheckRhs(1, 2);
    CheckLhs(0, 1);

    if (!loadedDep)
    {
        loadOnUseClassPath("pdf_ps_eps_graphic_export");
        loadedDep = TRUE;
    }

    if (Rhs == 1)
    {
        return sci_toprint_one_rhs(pvApiCtx, fname);
    }
    else
    {
        return sci_toprint_two_rhs(pvApiCtx, fname);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
static int sci_toprint_one_rhs(void* _pvCtx, const char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    sciErr = getVarAddressFromPosition(_pvCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(_pvCtx, piAddressVarOne))
    {
        char *fileName = NULL;
        BOOL iRet = FALSE;

        if (getAllocatedSingleString(_pvCtx, piAddressVarOne, &fileName) == 0)
        {
            char *fullName = getFullFilename(fileName);

            freeAllocatedSingleString(fileName);
            if (fullName)
            {
                if (FileExist(fullName))
                {
                    try
                    {
                        iRet = booltoBOOL(CallScilabBridge::printFile(getScilabJavaVM(), fullName));
                    }
                    catch (const GiwsException::JniException & e)
                    {
                        FREE(fullName);
                        fullName = NULL;
                        Scierror(999, _("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(),
                                 e.getJavaExceptionName().c_str());
                        return 0;
                    }
                }
                else
                {
                    if (getWarningMode())
                    {
                        sciprint(_("%s: The file %s does not exist.\n"), fname, fullName);
                    }
                    iRet = FALSE;
                }
                FREE(fullName);
                fullName = NULL;
            }

            createScalarBoolean(_pvCtx, nbInputArgument(_pvCtx) + 1, iRet);
            AssignOutputVariable(_pvCtx, 1) = nbInputArgument(_pvCtx) + 1;
            ReturnArguments(_pvCtx);
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
        }
    }
    else if (isDoubleType(pvApiCtx, piAddressVarOne))
    {
        if (isScalar(pvApiCtx, piAddressVarOne))
        {
            double dValue = 0.;

            if (!getScalarDouble(_pvCtx, piAddressVarOne, &dValue))
            {
                int num_win = (int)dValue;
                BOOL iRet = FALSE;

                if (num_win < 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Positive integers expected.\n"), fname);
                    return 0;
                }

                if (getFigureFromIndex((int) num_win) == NULL)
                {
                    Scierror(999, "%s: Figure with figure_id %d does not exist.\n", fname, (int)num_win);
                    return 0;
                }

                try
                {
                    iRet = booltoBOOL(CallScilabBridge::printFigure(getScilabJavaVM(), getFigureFromIndex((int) num_win), FALSE, FALSE));
                }
                catch (const GiwsException::JniException & e)
                {
                    Scierror(999, _("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
                    return 0;
                }

                createScalarBoolean(_pvCtx, nbInputArgument(_pvCtx) + 1, iRet);
                AssignOutputVariable(_pvCtx, 1) = nbInputArgument(_pvCtx) + 1;
                ReturnArguments(_pvCtx);
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Positive integer expected.\n"), fname, 1);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
static int sci_toprint_two_rhs(void* _pvCtx, const char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    sciErr = getVarAddressFromPosition(_pvCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);

        return 0;
    }

    sciErr = getVarAddressFromPosition(_pvCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isStringType(_pvCtx, piAddressVarOne))
    {
        if (isScalar(_pvCtx, piAddressVarTwo))
        {
            if (isStringType(_pvCtx, piAddressVarTwo))
            {
                char *pageHeader = NULL;
                char **pStVarOne = NULL;
                int *lenStVarOne = NULL;
                int mOne = 0, nOne = 0;
                int mnOne = 0;
                int lenLineToPrint = 0;
                int i = 0;
                char *lines = NULL;

                sciErr = getMatrixOfString(_pvCtx, piAddressVarOne, &mOne, &nOne, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                if (!((mOne == 1) || (nOne == 1)))
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"), fname, 1);
                    return 0;
                }

                mnOne = mOne * nOne;

                lenStVarOne = (int *)MALLOC(sizeof(int) * mnOne);
                if (lenStVarOne == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                sciErr = getMatrixOfString(_pvCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                for (i = 0; i < mnOne; i++)
                {
                    lenLineToPrint = lenLineToPrint + lenStVarOne[i] + (int)strlen("\n") + 1;
                }

                pStVarOne = (char **)MALLOC(sizeof(char *) * mnOne);
                if (pStVarOne == NULL)
                {
                    FREE(lenStVarOne);
                    lenStVarOne = NULL;
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                for (i = 0; i < mnOne; i++)
                {
                    pStVarOne[i] = (char *)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
                    if (pStVarOne[i] == NULL)
                    {
                        freeArrayOfString(pStVarOne, i);
                        if (lenStVarOne)
                        {
                            FREE(lenStVarOne);
                            lenStVarOne = NULL;
                        }
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }
                }

                sciErr = getMatrixOfString(_pvCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, pStVarOne);
                if (lenStVarOne)
                {
                    FREE(lenStVarOne);
                    lenStVarOne = NULL;
                }
                if (sciErr.iErr)
                {
                    freeArrayOfString(pStVarOne, mnOne);
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                lines = (char *)MALLOC((lenLineToPrint + 1) * sizeof(char));
                if (lines == NULL)
                {
                    freeArrayOfString(pStVarOne, mnOne);
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                for (i = 0; i < mnOne; i++)
                {
                    if (i == 0)
                    {
                        sprintf(lines, "%s\n", pStVarOne[i]);
                    }
                    else
                    {
                        sprintf(lines, "%s%s\n", lines, pStVarOne[i]);
                    }
                }
                freeArrayOfString(pStVarOne, mnOne);

                if (getAllocatedSingleString(_pvCtx, piAddressVarTwo, &pageHeader) == 0)
                {
                    BOOL iRet = FALSE;

                    try
                    {
                        iRet = booltoBOOL(CallScilabBridge::printString(getScilabJavaVM(), lines, pageHeader));
                    }

                    catch (const GiwsException::JniException & e)
                    {
                        freeAllocatedSingleString(pageHeader);
                        if (lines)
                        {
                            FREE(lines);
                            lines = NULL;
                        }
                        Scierror(999, _("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(),
                                 e.getJavaExceptionName().c_str());
                        return 0;
                    }

                    freeAllocatedSingleString(pageHeader);
                    if (lines)
                    {
                        FREE(lines);
                        lines = NULL;
                    }

                    createScalarBoolean(_pvCtx, nbInputArgument(_pvCtx) + 1, iRet);
                    AssignOutputVariable(_pvCtx, 1) = nbInputArgument(_pvCtx) + 1;
                    ReturnArguments(_pvCtx);
                }
                else
                {
                    if (lines)
                    {
                        FREE(lines);
                        lines = NULL;
                    }
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 2);
            return 0;
        }
    }
    else if (isDoubleType(_pvCtx, piAddressVarOne))
    {
        if (isScalar(_pvCtx, piAddressVarOne))
        {
            int num_win = 0;
            double dValue = 0.;

            if (!getScalarDouble(_pvCtx, piAddressVarOne, &dValue))
            {
                num_win = (int)dValue;

                if (num_win < 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Positive integers expected.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            if (!sciIsExistingFigure((int)num_win))
            {
                Scierror(999, "%s: Figure with figure_id %d does not exist.\n", fname, (int)num_win);
                return 0;
            }

            if (isStringType(_pvCtx, piAddressVarTwo))
            {
                BOOL iRet = FALSE;

                if (isScalar(_pvCtx, piAddressVarTwo))
                {
                    char *outputType = NULL;

                    if (getAllocatedSingleString(_pvCtx, piAddressVarTwo, &outputType) == 0)
                    {
                        if ((strcmp(outputType, "pos") == 0) || (strcmp(outputType, "gdi") == 0))
                        {
                            try
                            {
                                if (strcmp(outputType, "pos") == 0)
                                {
                                    iRet = booltoBOOL(CallScilabBridge::printFigure(getScilabJavaVM(), getFigureFromIndex(num_win), TRUE, FALSE));
                                }
                                else
                                {
                                    iRet = booltoBOOL((int)CallScilabBridge::printFigure(getScilabJavaVM(), getFigureFromIndex(num_win), FALSE, FALSE));
                                }
                            }
                            catch (const GiwsException::JniException & e)
                            {
                                Scierror(999, _("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(),
                                         e.getJavaExceptionName().c_str());
                                freeAllocatedSingleString(outputType);
                                return 0;
                            }

                            createScalarBoolean(_pvCtx, nbInputArgument(_pvCtx) + 1, iRet);
                            AssignOutputVariable(_pvCtx, 1) = nbInputArgument(_pvCtx) + 1;
                            ReturnArguments(_pvCtx);
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "pos", "gdi");
                        }
                        freeAllocatedSingleString(outputType);
                    }
                    else
                    {
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: 'pos' or 'gdi' value expected.\n"), fname, 2);
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 2);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Positive integer expected.\n"), fname, 1);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
