/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
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
}

using namespace org_scilab_modules_ui_data;
/*--------------------------------------------------------------------------*/
int sci_updateEditvarValue(char * fname, int *_piKey)
{
    CheckRhs(5,5); 
    CheckLhs(0,1);    
    SciErr sciErr; 
    
    int iComplex = 0;
   
    int *piAddressVarOne = NULL;
    char *pStVarOne = NULL;
    int lenStVarOne = 0;
    int m1 = 0, n1 = 0;

    int *piAddressVarTwo = NULL;
    double* pdblVarTwo = NULL;
    int m2 = 0, n2 = 0;
    
    int *piAddressVarThree = NULL;
    double* pdblVarThree = NULL;
    int m3 = 0, n3 = 0;

    int *piAddressVarFive = NULL;
    double* pdblVarFive = NULL;
    int m5 = 0, n5 = 0;


    int* piLen = NULL;

    /* First parameters must be a single string */

    /* get address */
    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* get dimensions */
    sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* we only want a single string */
    if (m1 !=1 || n1 != 1) {
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
        return 0;
    } 

    /* get lengths */
    sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pStVarOne = (char*)MALLOC(sizeof(char*) * (lenStVarOne + 1));

    /* get variable name to edit */
    sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
    if(sciErr.iErr)
    {
        FREE(pStVarOne);
        printError(&sciErr, 0);
        return 0;
    }


    /* Second parameters must be a single int */

    /* get address */
    sciErr = getVarAddressFromPosition(_piKey, 2, &piAddressVarTwo);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* get dimensions */
    sciErr = getMatrixOfDouble(_piKey, piAddressVarTwo, &m2, &n2, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* we only want a single int */
    if (m2 !=1 || n2 != 1) {
        Scierror(999,_("%s: Wrong size for input argument #%d: An integer expected.\n"),fname,2);
        return 0;
    } 

    /* get value */
    sciErr = getMatrixOfDouble(_piKey, piAddressVarTwo, &m2, &n2, &pdblVarTwo);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* Third parameters must be a single int */

    /* get address */
    sciErr = getVarAddressFromPosition(_piKey, 3, &piAddressVarThree);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* get dimensions */
    sciErr = getMatrixOfDouble(_piKey, piAddressVarThree, &m3, &n3, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* we only want a single int */
    if (m3 !=1 || n3 != 1) {
        Scierror(999,_("%s: Wrong size for input argument #%d: An integer expected.\n"),fname,3);
        return 0;
    } 

    /* get value */
    sciErr = getMatrixOfDouble(_piKey, piAddressVarThree, &m3, &n3, &pdblVarThree);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* Fith parameters must be a single int */

    /* get address */
    sciErr = getVarAddressFromPosition(_piKey, 5, &piAddressVarFive);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* get dimensions */
    sciErr = getMatrixOfDouble(_piKey, piAddressVarFive, &m5, &n5, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* we only want a single int */
    if (m5 !=1 || n5 != 1) {
        Scierror(999,_("%s: Wrong size for input argument #%d: An integer expected.\n"),fname,5);
        return 0;
    } 

    /* get value */
    sciErr = getMatrixOfDouble(_piKey, piAddressVarFive, &m5, &n5, &pdblVarFive);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* Fourth parameters */

    int iType = 0;
    void* pVarFour = NULL;
    void* pImgVarFour = NULL;
    int *piAddressVarFour = NULL;
    int m4 = 0, n4 = 0;
    /* get address */
    sciErr = getVarAddressFromPosition(_piKey, 4, &piAddressVarFour);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* get type of the named variable */
    sciErr = getVarType(_piKey, piAddressVarFour, &iType);

    switch (iType)
    {
    case sci_matrix:     

        /* get complexity */
        iComplex    = isVarComplex(_piKey, piAddressVarFour);

        /* check complexity */
        if(iComplex)
        {
            /* get size and data from Scilab memory */
            sciErr = getComplexMatrixOfDouble(_piKey,
                                            piAddressVarFour,
                                            &m4,
                                            &n4,
                                            (double**)&pVarFour,
                                            (double**)&pImgVarFour);

            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            /* Launch Java Variable Editor through JNI */
            EditVar::updateVariableEditorComplex(getScilabJavaVM(),
                                          pStVarOne,
                                          (int)pdblVarTwo[0],
                                          (int)pdblVarThree[0],
                                          ((double*)pVarFour)[0],
                                          ((double*)pImgVarFour)[0],
                                          (int)pdblVarFive[0]);
                   
        }
        else
        {
            /* get size and data from Scilab memory */
            sciErr = getMatrixOfDouble(_piKey, piAddressVarFour, &m4, &n4, (double**)&pVarFour);

            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            /* Launch Java Variable Editor through JNI */
            EditVar::updateVariableEditorDouble(getScilabJavaVM(),
                                          pStVarOne,
                                          (int)pdblVarTwo[0],
                                          (int)pdblVarThree[0],
                                          ((double*)pVarFour)[0],
                                          (int)pdblVarFive[0]);
            
        }



        break;

    case sci_strings :


        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(_piKey, piAddressVarFour, &m4, &n4, NULL, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * m4 * n4);
        //second call to retrieve length of each string
        sciErr = getMatrixOfString(_piKey, piAddressVarFour, &m4, &n4, piLen, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        pVarFour = (char**)malloc(sizeof(char*) * m4 * n4);
        for(int i = 0 ; i < m4 * n4 ; i++)
        {
            ((char**)pVarFour)[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }
        //third call to retrieve data
        sciErr = getMatrixOfString(_piKey, piAddressVarFour, &m4, &n4, piLen, (char**)pVarFour);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        /* Launch Java Variable Editor through JNI */
        EditVar::updateVariableEditorString(getScilabJavaVM(),
                                      pStVarOne,
                                      (int)pdblVarTwo[0],
                                      (int)pdblVarThree[0],
                                      ((char**)pVarFour)[0],
                                      (int)pdblVarFive[0]);


        break;

    case sci_boolean:
        /* get size and data from Scilab memory */
        sciErr = getMatrixOfBoolean(_piKey, piAddressVarFour, &m4, &n4, (int**)&pVarFour);

        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        /* Launch Java Variable Editor through JNI */
        EditVar::updateVariableEditorBoolean(getScilabJavaVM(),
                                      pStVarOne,
                                      (int)pdblVarTwo[0],
                                      (int)pdblVarThree[0],
                                      ((int*)pVarFour)[0],
                                      (int)pdblVarFive[0]);


        break;

    default :
        Scierror(42,"Type not handle by editvar yet");
        return 0;
    }
    




    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/

