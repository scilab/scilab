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

    char* piInt8 = NULL;
    char** ppiInt8 = NULL;

    unsigned char* piUInt8 = NULL;
    short** ppiUInt8 = NULL;

    short* piInt16 = NULL;
    short** ppiInt16 = NULL;

    unsigned short* piUInt16 = NULL;
    int** ppiUInt16 = NULL;

    int* piInt32 = NULL;
    int** ppiInt32 = NULL;

    unsigned int* piUInt32 = NULL;
    long long int** ppiUInt32 = NULL;

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
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

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
    if(m1 != 1 || n1 != 1)
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    /* get lengths */
    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
    if(sciErr.iErr)
    {
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

    if(strcmp(pStVarOne, "ans") == 0)
    {
        Scierror(999,_("%s: ans cannot be edited.\n"), fname);
        FREE(pStVarOne);
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
    if(sciErr.iErr)
    {
        FREE(pStVarOne);
        printError(&sciErr, 0);
        return 0;
    }

    switch(iType)
    {
    case sci_matrix :

        /* get complexity */
        iComplex    = isVarComplex(pvApiCtx, piAddr);

        /* check complexity */
        if(iComplex)
        {
            /* get size and data from Scilab memory */
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
            if(sciErr.iErr)
            {
                FREE(pStVarOne);
                printError(&sciErr, 0);
                return 0;
            }

            ppdblRealMatrix = new double*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppdblRealMatrix[i] = &pdblReal[i * iCols];
            }

            ppdblImgMatrix = new double*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppdblImgMatrix[i] = &pdblImg[i * iCols];
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorComplex(getScilabJavaVM(),
                                                   ppdblRealMatrix,
                                                   iRows,
                                                   iCols,
                                                   ppdblImgMatrix,
                                                   iRows,
                                                   iCols,
                                                   pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            delete(ppdblRealMatrix);
            delete(ppdblImgMatrix);
        }
        else
        {
            /* get size and data from Scilab memory */
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
            if(sciErr.iErr)
            {
                FREE(pStVarOne);
                printError(&sciErr, 0);
                return 0;
            }
            /*
             * we need this to make the links between the API (which return a double*)
             * and the JNI which needs a double**
             */
            ppdblRealMatrix = new double*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppdblRealMatrix[i] = &pdblReal[i * iCols];
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorDouble(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            delete(ppdblRealMatrix);
        }
        break;

    case sci_strings :

        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
        if(sciErr.iErr)
        {
            FREE(pStVarOne);
            printError(&sciErr, 0);
            return 0;
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
        if(sciErr.iErr)
        {
            FREE(pStVarOne);
            FREE(piLen);
            printError(&sciErr, 0);
            return 0;
        }

        pstData = (char**)calloc(iRows * iCols, sizeof(char*));
        for(int i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }
        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
        if(sciErr.iErr)
        {
            FREE(pStVarOne);
            FREE(piLen);
            freeArrayOfString(pstData, iRows * iCols);
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
            ppstData[i] = &pstData[i * iCols];
        }
        /* Launch Java Variable Editor through JNI */
        try
        {
            EditVar::openVariableEditorString(getScilabJavaVM(), ppstData, iRows, iCols, pStVarOne);
        } catch (GiwsException::JniCallMethodException ex1)
        {
            Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
        } catch (GiwsException::JniBadAllocException ex2)
        {
            Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
        }

        FREE(piLen);
        freeArrayOfString(pstData, iRows * iCols);
        delete(ppstData);

        break;

    case sci_boolean :
        //get size and data from Scilab memory
        sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &iRows, &iCols, &piBool);
        if(sciErr.iErr)
        {
            FREE(pStVarOne);
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
            ppiBool[i] = &piBool[i * iCols];
        }
        /* Launch Java Variable Editor through JNI */
        try
        {
            EditVar::openVariableEditorBoolean(getScilabJavaVM(), ppiBool, iRows, iCols, pStVarOne);
        } catch (GiwsException::JniCallMethodException ex1)
        {
            Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
        } catch (GiwsException::JniBadAllocException ex2)
        {
            Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
        }

        delete(ppiBool);

        break;

    case sci_ints :
        //get size and data from Scilab memory
        int prec;
        sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr, &prec);
        if (sciErr.iErr)
        {
            FREE(pStVarOne);
            printError(&sciErr, 0);
            return 0;
        }

        switch (prec)
        {
        case SCI_INT8 :
            sciErr = getMatrixOfInteger8(pvApiCtx, piAddr, &iRows, &iCols, &piInt8);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return -1;
            }
            ppiInt8 = new char*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppiInt8[i] = &piInt8[i * iCols];
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorInteger8(getScilabJavaVM(), reinterpret_cast<byte**>(ppiInt8), iRows, iCols, pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            delete(ppiInt8);

            break;

        case SCI_UINT8 :
            sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddr, &iRows, &iCols, &piUInt8);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return -1;
            }

            ppiUInt8 = new short*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppiUInt8[i] = new short[iCols];
                for (int j = 0; j < iCols; ++j)
                {
                    ppiUInt8[i][j] = piUInt8[i * iCols + j];
                }
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorUInteger8(getScilabJavaVM(), ppiUInt8, iRows, iCols, pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            for (int i = 0; i < iRows; ++i)
            {
                delete(ppiUInt8[i]);
            }
            delete(ppiUInt8);

            break;

        case SCI_INT16 :
            sciErr = getMatrixOfInteger16(pvApiCtx, piAddr, &iRows, &iCols, &piInt16);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return -1;
            }
            ppiInt16 = new short*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppiInt16[i] = &piInt16[i * iCols];
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorInteger16(getScilabJavaVM(), ppiInt16, iRows, iCols, pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            delete(ppiInt16);

            break;

        case SCI_UINT16 :
            sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddr, &iRows, &iCols, &piUInt16);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return -1;
            }

            ppiUInt16 = new int*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppiUInt16[i] = new int[iCols];
                for (int j = 0; j < iCols; ++j)
                {
                    ppiUInt16[i][j] = piUInt16[i * iCols + j];
                }
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorUInteger16(getScilabJavaVM(), ppiUInt16, iRows, iCols, pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            for (int i = 0; i < iRows; ++i)
            {
                delete(ppiUInt16[i]);
            }
            delete(ppiUInt16);

            break;

        case SCI_INT32 :
            sciErr = getMatrixOfInteger32(pvApiCtx, piAddr, &iRows, &iCols, &piInt32);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return -1;
            }
            ppiInt32 = new int*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppiInt32[i] = &piInt32[i * iCols];
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorInteger32(getScilabJavaVM(), ppiInt32, iRows, iCols, pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            delete(ppiInt32);

            break;

        case SCI_UINT32 :
            sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddr, &iRows, &iCols, &piUInt32);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return -1;
            }

            ppiUInt32 = new long long int*[iRows];
            for (int i = 0; i < iRows; ++i)
            {
                ppiUInt32[i] = new long long int[iCols];
                for (int j = 0; j < iCols; ++j)
                {
                    ppiUInt32[i][j] = piUInt32[i * iCols + j];
                }
            }

            /* Launch Java Variable Editor through JNI */
            try
            {
                EditVar::openVariableEditorUInteger32(getScilabJavaVM(), ppiUInt32, iRows, iCols, pStVarOne);
            } catch (GiwsException::JniCallMethodException ex1)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            } catch (GiwsException::JniBadAllocException ex2)
            {
                Scierror(999, _("%s: Java memory problem, cannot edit %s\n"), fname, pStVarOne);
            }

            for (int i = 0; i < iRows; ++i)
            {
                delete(ppiUInt32[i]);
            }
            delete(ppiUInt32);

            break;

        default :

            Scierror(42, _("%s: Type not handle yet"), fname);
            return 0;
        }

        break;

    default:

        Scierror(42, _("%s: Type not handle yet"), fname);
        return 0;
    }

    FREE(pStVarOne);

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
