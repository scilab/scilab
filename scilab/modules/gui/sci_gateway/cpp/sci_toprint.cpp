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
extern "C"
{
#include "api_scilab.h"
#include "stack-c.h"
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
#include "WindowList.h"
#include "warningmode.h"
}
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;
/*--------------------------------------------------------------------------*/
static int sci_toprint_one_rhs(const char *fname);
static int sci_toprint_two_rhs(const char *fname);
/*--------------------------------------------------------------------------*/
int sci_toprint(char *fname,unsigned long l)
{
    CheckRhs(1,2);
    CheckLhs(0,1);

    if (!loadedDep)
    {
        loadOnUseClassPath("pdf_ps_eps_graphic_export");
        loadedDep = TRUE;
    }

    if (Rhs == 1)
    {
        return sci_toprint_one_rhs(fname);
    }
    else
    {
        return sci_toprint_two_rhs(fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_toprint_one_rhs(const char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne))
    {
        char *fileName = NULL;
        BOOL iRet = FALSE;

        if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &fileName) == 0)
        {
            char *fullName = getFullFilename(fileName);
            freeAllocatedSingleString(fileName);
            if (fullName)
            {
                if (FileExist(fullName))
                {
                    try
                    {
                        iRet = (int)CallScilabBridge::printFile(getScilabJavaVM(), fullName);
                    }
                    catch (const GiwsException::JniException& e)
                    {
                        FREE(fullName);
                        fullName = NULL;
                        Scierror(999,_("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
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

            createScalarBoolean(pvApiCtx, Rhs + 1, iRet);
            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
        }
    }
    else if (isDoubleType(pvApiCtx, piAddressVarOne))
    {
        if (isScalar(pvApiCtx, piAddressVarOne))
        {
            double dValue = 0.;
            if (!getScalarDouble(pvApiCtx, piAddressVarOne, &dValue))
            {
                int num_win = (int)dValue;
                BOOL iRet = FALSE;

                if (num_win < 0)
                {
                    Scierror(999,_("%s: Wrong value for input argument #%d: Non-negative integers expected.\n"), fname);
                    return 0;
                }

                if (!sciIsExistingFigure((int) num_win))
                {
                    Scierror(999, "%s: Figure with figure_id %d does not exist.\n",fname, (int) num_win);
                    return 0;
                }

                try
                {
                    iRet = (int)CallScilabBridge::printFigure(getScilabJavaVM(), num_win, FALSE, FALSE);
                }
                catch (const GiwsException::JniException& e)
                {
                    Scierror(999,_("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
                    return 0;
                }

                createScalarBoolean(pvApiCtx, Rhs + 1, iRet);
                LhsVar(1) = Rhs + 1;
                PutLhsVar();
            }
            else
            {
                Scierror(999,_("%s: Memory allocation error.\n"), fname);
            }
        }
        else
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: Non-negative integer expected.\n"),fname,1);
        }
    }
    else
    {
        Scierror(999,_("%s: Wrong type for input argument #%d.\n"), fname, 1);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_toprint_two_rhs(const char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne))
    {
        if (isScalar(pvApiCtx, piAddressVarTwo))
        {
            if (isStringType(pvApiCtx, piAddressVarTwo))
            {
                char *pageHeader = NULL;
                char **pStVarOne = NULL;
                int *lenStVarOne = NULL;
                int mOne = 0, nOne = 0;
                int mnOne = 0;
                int lenLineToPrint = 0;
                int i = 0;
                char *lines = NULL;


                sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne, &nOne, NULL, NULL);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if ( !((mOne == 1) || (nOne == 1)) )
                {
                    Scierror(999,_("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"), fname, 1);
                    return 0;
                }

                mnOne = mOne * nOne;

                lenStVarOne = (int*)MALLOC(sizeof(int) * mnOne);
                if (lenStVarOne == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne, &nOne, lenStVarOne, NULL);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                for (i = 0; i <  mnOne; i++)
                {
                    lenLineToPrint = lenLineToPrint + lenStVarOne[i] + (int)strlen("\n") + 1;
                }

                pStVarOne = (char**) MALLOC(sizeof(char*) * mnOne);
                if (pStVarOne == NULL)
                {
                    FREE(lenStVarOne);
                    lenStVarOne = NULL;
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                for (i = 0; i < mnOne; i++)
                {
                    pStVarOne[i] = (char*)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
                    if (pStVarOne[i] == NULL)
                    {
                        freeArrayOfString(pStVarOne, i);
                        if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }
                }

                sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, pStVarOne);
                if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
                if(sciErr.iErr)
                {
                    freeArrayOfString(pStVarOne, mnOne);
                    printError(&sciErr, 0);
                    return 0;
                }

                lines = (char*)MALLOC((lenLineToPrint + 1) * sizeof(char));
                if (lines == NULL)
                {
                    freeArrayOfString(pStVarOne, mnOne);
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                for (i = 0; i < mnOne; i++)
                {
                    if (i==0)
                    {
                        sprintf(lines, "%s\n", pStVarOne[i]);
                    }
                    else
                    {
                        sprintf(lines, "%s%s\n", lines,  pStVarOne[i]);
                    }
                }
                freeArrayOfString(pStVarOne, mnOne);

                if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &pageHeader) == 0)
                {
                    BOOL iRet = FALSE;
                    try
                    {
                        iRet = (BOOL)CallScilabBridge::printString(getScilabJavaVM(), lines, pageHeader);
                    }

                    catch (const GiwsException::JniException& e)
                    {
                        freeAllocatedSingleString(pageHeader);
                        if (lines)
                        {
                            FREE(lines);
                            lines = NULL;
                        }
                        Scierror(999,_("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
                        return 0;
                    }

                    freeAllocatedSingleString(pageHeader);
                    if (lines)
                    {
                        FREE(lines);
                        lines = NULL;
                    }

                    createScalarBoolean(pvApiCtx, Rhs + 1, iRet);
                    LhsVar(1) = Rhs + 1;
                    PutLhsVar();
                }
                else
                {
                    if (lines)
                    {
                        FREE(lines);
                        lines = NULL;
                    }
                    Scierror(999,_("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: String expected.\n"), fname, 2);
            return 0;
        }
    }
    else if (isDoubleType(pvApiCtx, piAddressVarOne))
    {
        if (isScalar(pvApiCtx, piAddressVarOne))
        {
            int num_win = 0;
            double dValue = 0.;

            if (!getScalarDouble(pvApiCtx, piAddressVarOne, &dValue))
            {
                num_win = (int)dValue;

                if (num_win < 0)
                {
                    Scierror(999,_("%s: Wrong value for input argument #%d: Non-negative integers expected.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999,_("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            if (!sciIsExistingFigure((int) num_win))
            {
                Scierror(999, "%s: Figure with figure_id %d does not exist.\n",fname, (int) num_win);
                return 0;
            }

            if (isStringType(pvApiCtx, piAddressVarTwo))
            {
                BOOL iRet = FALSE;

                if (isScalar(pvApiCtx, piAddressVarTwo))
                {
                    char *outputType = NULL;
                    if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &outputType) == 0)
                    {
                        if ((strcmp(outputType, "pos") == 0) || (strcmp(outputType, "gdi") == 0))
                        {
                            try
                            {
                                if (strcmp(outputType, "pos") == 0)
                                {
                                    iRet = (BOOL)CallScilabBridge::printFigure(getScilabJavaVM(), num_win, TRUE, FALSE);
                                }
                                else
                                {
                                    iRet = (BOOL)(int)CallScilabBridge::printFigure(getScilabJavaVM(), num_win, FALSE, FALSE);
                                }
                            }
                            catch (const GiwsException::JniException& e)
                            {
                                Scierror(999,_("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
                                freeAllocatedSingleString(outputType);
                                return 0;
                            }

                            createScalarBoolean(pvApiCtx, Rhs + 1, iRet);
                            LhsVar(1) = Rhs + 1;
                            PutLhsVar();
                        }
                        else
                        {
                            Scierror(999,_("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "pos", "gdi");
                        }
                        freeAllocatedSingleString(outputType);
                    }
                    else
                    {
                        Scierror(999,_("%s: Memory allocation error.\n"), fname);
                    }
                }
                else
                {
                    Scierror(999,_("%s: Wrong size for input argument #%d: 'pos' or 'gdi' value expected.\n"), fname, 2);
                }
            }
            else
            {
                Scierror(999,_("%s: Wrong type for input argument #%d.\n"), fname, 2);
            }
        }
        else
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: Non-negative integer expected.\n"), fname, 1);
        }
    }
    else
    {
        Scierror(999,_("%s: Wrong type for input argument #%d.\n"), fname, 1);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
