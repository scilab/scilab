/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "api_scilab.h"
#include "api_oldstack.h"

/*--------------------------------------------------------------------------*/
int sci_rand(char *fname, int* _piKey)
{
    SciErr sciErr;
    //save state of rand function
    static int siRandType = 0;
    static int siRandSave = 0;
    static int iForceInit	= 0;
    int iRet							= 0;
    int iRowsOut					= 0;
    int iColsOut					= 0;

    int iRandSave					= -1;
    int* piAddr						= NULL;

    if (Rhs > 3)
    {
        //TODO YaSp : %hm_rand
        return 0;
    }

    if (Rhs == 0)
    {
        //rand() or rand
        double dblReal = 0;

        dblReal = getNextRandValue(siRandType, &siRandSave, 0);
        iRet = createScalarDouble(_piKey, Rhs + 1, dblReal);
        if (iRet)
        {
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isDoubleType(_piKey, piAddr))
    {
        int *piAddrLast	= NULL;

        sciErr = getVarAddressFromPosition(_piKey, Rhs, &piAddrLast);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isStringType(_piKey, piAddrLast))
        {
            char* pstDataLast			= NULL;

            iRet = getAllocatedSingleString(_piKey, piAddrLast, &pstDataLast);
            if (iRet)
            {
                freeAllocatedSingleString(pstDataLast);
                return 0;
            }

            //memorize that the law has been changed
            iRandSave = siRandType;
            //change the law temporarily
            siRandType = setRandType(pstDataLast[0]);
            freeAllocatedSingleString(pstDataLast);
        }

        if (Rhs == 1 || (Rhs == 2 && iRandSave != -1)) //rand(A) or rand(A, "law")
        {
            if (isVarMatrixType(_piKey, piAddr) == 0)
            {
                OverLoad(1);
                return 0;
            }

            sciErr = getVarDimension(_piKey, piAddr, &iRowsOut, &iColsOut);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else //rand(m,n) or rand(m,n,"law")
        {
            int* piAddr2	= NULL;
            sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = getDimFromVar(_piKey, piAddr, &iRowsOut);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = getDimFromVar(_piKey, piAddr2, &iColsOut);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

        if (iRowsOut == 0 || iColsOut == 0)
        {
            iRet = createEmptyMatrix(_piKey, Rhs + 1);
            if (iRet)
            {
                return 1;
            }
        }
        else //generate
        {
            int i;
            double* pdblReal	= NULL;
            double* pdblImg		= NULL;

            if (Rhs == 1 && isVarComplex(_piKey, piAddr))
            {
                sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRowsOut, iColsOut, &pdblReal, &pdblImg);
            }
            else
            {
                sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRowsOut, iColsOut, &pdblReal);
            }

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            for (i = 0 ; i < iRowsOut * iColsOut ; i++)
            {
                pdblReal[i] = getNextRandValue(siRandType, &siRandSave, iForceInit);
                iForceInit = 0;
            }

            if (pdblImg)
            {
                for (i = 0 ; i < iRowsOut * iColsOut ; i++)
                {
                    pdblImg[i] = getNextRandValue(siRandType, &siRandSave, 0);
                }
            }
            if (iRandSave != -1)
            {
                //restore old law
                siRandType = iRandSave;
            }
        }
    }
    else if (isStringType(_piKey, piAddr))
    {
        char* pstData				= NULL;

        iRet = getAllocatedSingleString(_piKey, piAddr, &pstData);
        if (iRet)
        {
            return 0;
        }

        if (strcmp(pstData, g_pstConfigSeed) == 0) //seed
        {
            if (Rhs == 1) //get
            {
                iRet = createScalarDoubleFromInteger(_piKey, Rhs + 1, siRandSave);
                if (iRet)
                {
                    return 1;
                }
            }
            else if (Rhs == 2) //set
            {
                int* piAddr2			= NULL;
                double dblReal2		= 0;

                sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if (!isScalar(_piKey, piAddr2) || isVarComplex(_piKey, piAddr2))
                {
                    return 1;
                }

                iRet = getScalarDouble(_piKey, piAddr2, &dblReal2);
                if (iRet)
                {
                    return 1;
                }

                siRandSave = (int)Max(dblReal2, 0);
                iForceInit = 1;
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if (strcmp(pstData, g_pstConfigInfo) == 0) //info
        {
            char* pstDataOut;
            if (Rhs > 1)
            {
                //error
                return 0;
            }

            if (siRandType == 0)
            {
                pstDataOut = (char*)g_pstTypeUniform;
            }
            else
            {
                pstDataOut = (char*)g_pstTypeNormal;
            }

            iRet = createSingleString(_piKey, Rhs + 1, pstDataOut);
            if (iRet)
            {
                return 1;
            }
        }
        else //uniform or normal or 'g' ( 'u', 'n', 'g' )
        {
            siRandType = setRandType(pstData[0]);
            return 0;
        }
    }
    else
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
