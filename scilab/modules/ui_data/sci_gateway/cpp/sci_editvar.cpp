/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include "EditVar.hxx"

extern "C"
{
#include "gw_ui_data.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "sciprint.h"
#include "getScilabJavaVM.h"
#include "localization.h"
}

using namespace org_scilab_modules_ui_data;
/*--------------------------------------------------------------------------*/
int sci_editvar(char *fname,unsigned long fname_len)
{
    CheckRhs(1,1); /* TODO change this in the future */
    CheckLhs(0,1);
    SciErr sciErr;

    int m1 = 0, n1 = 0;

    int *piAddr = NULL;
    int iType = 0;
    int iCols = 0;
    int iRows = 0;

    int* piBool = NULL;
    int** ppiBool = NULL;

    int iComplex = 0;
    double *pdblReal = NULL;
    double *pdblImg = NULL;
    double **ppdblRealMatrix = NULL;
    double **ppdblImgMatrix = NULL;


    char** pstData = NULL;
    int* piLen = NULL;
    char ***ppstData = NULL;

    int *piAddressVarOne = NULL;
    char *pStVarOne = NULL;
    int lenStVarOne = 0;

    /*get input data*/
    if(Rhs != 1)
    {
        Scierror(999,_("%s: Wrong number of input argument(s): %d expected.\n"), fname, 1);
        return 0;
    }


    /* get address */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);

    if (iType != sci_strings)
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A String expected.\n"), fname, 1);
        return 0;
    }

    /* get dimensions */
    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* TODO maybe allow vectors in case someone wants to edit several variables in the same time? */
    if (m1 !=1 || n1 != 1) {
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
        return 0;
    }

    /* get lengths */
    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
    if(sciErr.iErr)
    {
        if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
        printError(&sciErr, 0);
        return 0;
    }

    pStVarOne = (char*)MALLOC(sizeof(char*) * (lenStVarOne + 1));

    /* get variable name to edit */
    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
    if(sciErr.iErr)
    {
        FREE(pStVarOne);
        printError(&sciErr, 0);
        return 0;
    }
    /* get address of the variable*/
    sciErr = getVarAddressFromName(pvApiCtx, pStVarOne, &piAddr);
    if(sciErr.iErr)
    {
        Scierror(4,_("%s: Undefined variable %s.\n"), fname, pStVarOne);
        FREE(pStVarOne);
        return 0;
    }

    /* get type of the named variable */
    sciErr = getVarType(pvApiCtx, piAddr, &iType);

    switch( iType)
    {
    case sci_matrix :

        /* get complexity */
        iComplex    = isVarComplex(pvApiCtx, piAddr);

        /* check complexity */
        if(iComplex)
        {
            /* get size and data from Scilab memory */
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);

            ppdblRealMatrix = new double*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppdblRealMatrix[i] = &pdblReal[i*iCols];

            }

            ppdblImgMatrix = new double*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppdblImgMatrix[i] = &pdblImg[i*iCols];

            }
            
            /* Launch Java Variable Editor through JNI */
            EditVar::openVariableEditorComplex(getScilabJavaVM(),
                                            ppdblRealMatrix,
                                            iRows,
                                            iCols,
                                            ppdblImgMatrix,
                                            iRows,
                                            iCols,
                                            pStVarOne);
        
        }
        else
        {
            /* get size and data from Scilab memory */
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);

            /*
             * we need this to make the links between the API (which return a double*)
             * and the JNI which needs a double**
             */
            ppdblRealMatrix = new double*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppdblRealMatrix[i] = &pdblReal[i*iCols];

            }

            /* Launch Java Variable Editor through JNI */
            EditVar::openVariableEditorDouble(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, pStVarOne);


        
        }

        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }





        break;

    case sci_strings :


        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);
        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
        for(int i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }
        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        /*
         * we need this to make the links between the API (which return a char**)
         * and the JNI which needs a char***
         */
        ppstData = new char**[iRows];
        for (int i = 0; i < iRows; ++i)
        {
            ppstData[i] = &pstData[i*iCols];

        }
        /* Launch Java Variable Editor through JNI */
        EditVar::openVariableEditorString(getScilabJavaVM(), ppstData, iRows, iCols, pStVarOne);
        break;

    case sci_boolean:
       //get size and data from Scilab memory
        sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &iRows, &iCols, &piBool);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        /*
         * we need this to make the links between the API (which return a int*)
         * and the JNI which needs a int**
         */
        ppiBool = new int*[iRows];
        for (int i = 0; i < iRows; ++i)
        {
            ppiBool[i] = &piBool[i*iCols];

        }
        /* Launch Java Variable Editor through JNI */
        EditVar::openVariableEditorBoolean(getScilabJavaVM(), ppiBool, iRows, iCols, pStVarOne);


        break;


    default:

        Scierror(42,"Type not handle by editvar yet");
        return 0;
    }
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
