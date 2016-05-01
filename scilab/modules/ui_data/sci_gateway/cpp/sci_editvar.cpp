/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

#include <iostream>
#include "EditVar.hxx"
#include "GiwsException.hxx"
//#include "ScilabToJava.hxx"

extern "C"
{
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "sciprint.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include <time.h>
}

using namespace org_scilab_modules_ui_data;
/*--------------------------------------------------------------------------*/
template <typename T, typename U>
T** wrap(U * x, int r, int c)
{
    T ** xx = new T*[r];
    for (int i = 0; i < r; i++)
    {
        xx[i] = new T[c];
        for (int j = 0; j < c; j++)
        {
            xx[i][j] = static_cast<T>(x[j * r + i]);
        }
    }

    return xx;
}
/*--------------------------------------------------------------------------*/
/* For example if the matrix is [1 2 0 0 0;0 1 2 0 0;0 2 1 3 0 0;0 0 0 0 0;0 0 0 0 1], then
   r=5, c=5, nbItem=8, nbItemRow=[2 2 3 0 1], colPos=[1 2 2 3 2 3 4 5] and x=[1 2 1 2 2 1 3 1] */
double ** wrapSparse(int nbItem, int * nbItemRow, int * colPos, int r, int c, double * x)
{
    double ** xx = new double*[r];
    int prev = 0;
    for (int i = 0; i < r; i++)
    {
        xx[i] = new double[c];
        memset(xx[i], 0, c * sizeof(double));
        for (int j = 0; j < nbItemRow[i]; j++)
        {
            xx[i][colPos[prev + j] - 1] = x[prev + j];
        }
        prev += nbItemRow[i];
    }

    return xx;
}
/*--------------------------------------------------------------------------*/
/* boolean version */
int ** wrapSparse(int nbItem, int * nbItemRow, int * colPos, int r, int c)
{
    int ** xx = new int*[r];
    int prev = 0;
    for (int i = 0; i < r; i++)
    {
        xx[i] = new int[c];
        memset(xx[i], 0, c * sizeof(int));
        for (int j = 0; j < nbItemRow[i]; j++)
        {
            xx[i][colPos[prev + j] - 1] = 1;
        }
        prev += nbItemRow[i];
    }

    return xx;
}
/*--------------------------------------------------------------------------*/
template <typename T>
void clearWrap(T ** x, int r)
{
    for (int i = 0; i < r; i++)
    {
        delete [] x[i];
    }

    delete[] x;
}
/*--------------------------------------------------------------------------*/
int sci_editvar(char * fname, void* pvApiCtx)
{
    CheckRhs(1, 4); /* TODO change this in the future */
    CheckLhs(0, 1);

    SciErr sciErr;

    int m1 = 0, n1 = 0;

    int * piAddr = NULL;
    int iType = 0;
    int iCols = 0;
    int iRows = 0;

    int * piBool = NULL;
    int ** ppiBool = NULL;

    char * piInt8 = NULL;
    char ** ppiInt8 = NULL;

    unsigned char * piUInt8 = NULL;
    short ** ppiUInt8 = NULL;

    short * piInt16 = NULL;
    short ** ppiInt16 = NULL;

    unsigned short * piUInt16 = NULL;
    int ** ppiUInt16 = NULL;

    int * piInt32 = NULL;
    int ** ppiInt32 = NULL;

    unsigned int * piUInt32 = NULL;
    long long int ** ppiUInt32 = NULL;

    int iComplex = 0;
    double * pdblReal = NULL;
    double * pdblImg = NULL;
    double ** ppdblRealMatrix = NULL;
    double ** ppdblImgMatrix = NULL;

    char ** pstData = NULL;
    char *** ppstData = NULL;

    int * piAddressVarOne = NULL;
    char * pStVarOne = NULL;
    int lenStVarOne = 0;

    int * addr = NULL;
    double * rowsIndex = NULL;
    int nbRowsIndex = 0;
    double * colsIndex = NULL;
    int nbColsIndex = 0;

    int piNbItem = 0;
    int * piNbItemRow = NULL;
    int * piColPos = NULL;

    if (Rhs == 3)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"), fname, 1, 2, 4);
        return 0;
    }

    /* get address */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType != sci_strings)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), fname, 1);
        return 0;
    }

    /* get variable name */
    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (strcmp(pStVarOne, "ans") == 0)
    {
        Scierror(999, _("%s: ans cannot be edited.\n"), fname);
        freeAllocatedSingleString(pStVarOne);
        return 0;
    }

    /* get address of the variable*/
    sciErr = getVarAddressFromName(pvApiCtx, pStVarOne, &piAddr);
    if (sciErr.iErr)
    {
        Scierror(4, _("%s: Undefined variable: %s.\n"), fname, pStVarOne);
        freeAllocatedSingleString(pStVarOne);
        return 0;
    }

    /* Workaround to check for permanent variable.*/

    if (strcmp(pStVarOne, "$")			    == 0 ||
            strcmp(pStVarOne, "%e")		    == 0 ||
            strcmp(pStVarOne, "%eps")   	== 0 ||
            strcmp(pStVarOne, "%fftw")  	== 0 ||
            strcmp(pStVarOne, "%f")		    == 0 ||
            strcmp(pStVarOne, "%F")		    == 0 ||
            strcmp(pStVarOne, "%gui")		== 0 ||
            strcmp(pStVarOne, "%i")		    == 0 ||
            strcmp(pStVarOne, "%io")		== 0 ||
            strcmp(pStVarOne, "%inf")		== 0 ||
            strcmp(pStVarOne, "%nan")		== 0 ||
            strcmp(pStVarOne, "%pi")		== 0 ||
            strcmp(pStVarOne, "%s")	    	== 0 ||
            strcmp(pStVarOne, "%tk")		== 0 ||
            strcmp(pStVarOne, "%t")	    	== 0 ||
            strcmp(pStVarOne, "%T")	    	== 0 ||
            strcmp(pStVarOne, "%z")	    	== 0 ||
            strcmp(pStVarOne, "evoid")	    == 0 ||
            strcmp(pStVarOne, "home")		== 0 ||
            strcmp(pStVarOne, "PWD")		== 0 ||
            strcmp(pStVarOne, "SCI")		== 0 ||
            strcmp(pStVarOne, "SCIHOME")	== 0 ||
            strcmp(pStVarOne, "TMPDIR") 	== 0 )
    {
        Scierror(13, _("Redefining permanent variable.\n"), fname);
        freeAllocatedSingleString(pStVarOne);
        return 0;
    }

    if (Rhs == 1)
    {
        /* get address of the variable*/
        sciErr = getVarAddressFromName(pvApiCtx, pStVarOne, &piAddr);
        if (sciErr.iErr)
        {
            Scierror(4, _("%s: Undefined variable: %s.\n"), fname, pStVarOne);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }
    }
    else
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }
    }

    if (Rhs == 4)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }

        if (!isDoubleType(pvApiCtx, addr))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Double expected.\n"), fname, 3);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, addr, &iRows, &iCols, &rowsIndex);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }

        nbRowsIndex = iRows * iCols;

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }

        if (!isDoubleType(pvApiCtx, addr))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Double expected.\n"), fname, 4);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, addr, &iRows, &iCols, &colsIndex);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }

        nbColsIndex = iRows * iCols;
    }

    //org_modules_commons::ScilabToJava::sendVariable(std::string(pStVarOne), true, pvApiCtx);

    /* get type of the named variable */
    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        freeAllocatedSingleString(pStVarOne);
        return 0;
    }

    switch (iType)
    {
        case sci_matrix :

            /* get complexity */
            iComplex    = isVarComplex(pvApiCtx, piAddr);

            /* check complexity */
            if (iComplex)
            {
                /* get size and data from Scilab memory */
                sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    freeAllocatedSingleString(pStVarOne);
                    return 0;
                }

                ppdblRealMatrix = wrap<double, double>(pdblReal, iRows, iCols);
                ppdblImgMatrix = wrap<double, double>(pdblImg, iRows, iCols);

                try
                {
                    if (nbRowsIndex == 0 || nbColsIndex == 0)
                    {
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
                        /* Launch Java Variable Editor through JNI */
                        EditVar::refreshVariableEditorComplex(getScilabJavaVM(),
                                                              ppdblRealMatrix,
                                                              iRows,
                                                              iCols,
                                                              ppdblImgMatrix,
                                                              iRows,
                                                              iCols,
                                                              rowsIndex,
                                                              nbRowsIndex,
                                                              colsIndex,
                                                              nbColsIndex,
                                                              pStVarOne);
                    }
                }
                catch (const GiwsException::JniException & e)
                {
                    Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                }

                clearWrap<double>(ppdblRealMatrix, iRows);
                clearWrap<double>(ppdblImgMatrix, iRows);
            }
            else
            {
                /* get size and data from Scilab memory */
                sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    freeAllocatedSingleString(pStVarOne);
                    return 0;
                }
                /*
                 * we need this to make the links between the API (which return a double*)
                 * and the JNI which needs a double**
                 */
                ppdblRealMatrix = wrap<double, double>(pdblReal, iRows, iCols);

                /* Launch Java Variable Editor through JNI */
                try
                {
                    if (nbRowsIndex == 0 || nbColsIndex == 0)
                    {
                        EditVar::openVariableEditorDouble(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, pStVarOne);
                    }
                    else
                    {
                        EditVar::refreshVariableEditorDouble(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                    }
                }
                catch (const GiwsException::JniException & e)
                {
                    Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                }

                clearWrap<double>(ppdblRealMatrix, iRows);
            }
            break;

        case sci_strings :

            //retrieve data
            if (getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &pstData))
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                freeAllocatedSingleString(pStVarOne);
                return 0;
            }

            /*
             * we need this to make the links between the API (which return a char**)
             * and the JNI which needs a 2-dims char*
             */
            ppstData = wrap<char *, char *>(pstData, iRows, iCols);

            /* Launch Java Variable Editor through JNI */
            try
            {
                if (nbRowsIndex == 0 || nbColsIndex == 0)
                {
                    EditVar::openVariableEditorString(getScilabJavaVM(), ppstData, iRows, iCols, pStVarOne);
                }
                else
                {
                    EditVar::refreshVariableEditorString(getScilabJavaVM(), ppstData, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                }
            }
            catch (const GiwsException::JniException & e)
            {
                Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
            }

            freeAllocatedMatrixOfString(iRows, iCols, pstData);

            clearWrap<char *>(ppstData, iRows);

            break;

        case sci_boolean :
            //get size and data from Scilab memory
            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &iRows, &iCols, &piBool);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                freeAllocatedSingleString(pStVarOne);
                return 0;
            }

            /*
             * we need this to make the links between the API (which return a int*)
             * and the JNI which needs a int**
             */
            ppiBool = wrap<int, int>(piBool, iRows, iCols);

            /* Launch Java Variable Editor through JNI */
            try
            {
                if (nbRowsIndex == 0 || nbColsIndex == 0)
                {
                    EditVar::openVariableEditorBoolean(getScilabJavaVM(), ppiBool, iRows, iCols, pStVarOne);
                }
                else
                {
                    EditVar::refreshVariableEditorBoolean(getScilabJavaVM(), ppiBool, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                }
            }
            catch (const GiwsException::JniException & e)
            {
                Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
            }

            clearWrap<int>(ppiBool, iRows);

            break;

        case sci_ints :
            //get size and data from Scilab memory
            int prec;
            sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr, &prec);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                freeAllocatedSingleString(pStVarOne);
                return 0;
            }

            switch (prec)
            {
                case SCI_INT8 :
                    sciErr = getMatrixOfInteger8(pvApiCtx, piAddr, &iRows, &iCols, &piInt8);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        freeAllocatedSingleString(pStVarOne);
                        return -1;
                    }

                    ppiInt8 = wrap<char, char>(piInt8, iRows, iCols);

                    /* Launch Java Variable Editor through JNI */
                    try
                    {
                        if (nbRowsIndex == 0 || nbColsIndex == 0)
                        {
                            EditVar::openVariableEditorInteger8(getScilabJavaVM(), reinterpret_cast<byte**>(ppiInt8), iRows, iCols, pStVarOne);
                        }
                        else
                        {
                            EditVar::refreshVariableEditorInteger8(getScilabJavaVM(), reinterpret_cast<byte**>(ppiInt8), iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                        }
                    }
                    catch (const GiwsException::JniException & e)
                    {
                        Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                    }

                    clearWrap<char>(ppiInt8, iRows);

                    break;

                case SCI_UINT8 :
                    sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddr, &iRows, &iCols, &piUInt8);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        freeAllocatedSingleString(pStVarOne);
                        return -1;
                    }

                    ppiUInt8 = wrap<short, unsigned char>(piUInt8, iRows, iCols);

                    /* Launch Java Variable Editor through JNI */
                    try
                    {
                        if (nbRowsIndex == 0 || nbColsIndex == 0)
                        {
                            EditVar::openVariableEditorUInteger8(getScilabJavaVM(), ppiUInt8, iRows, iCols, pStVarOne);
                        }
                        else
                        {
                            EditVar::refreshVariableEditorUInteger8(getScilabJavaVM(), ppiUInt8, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                        }
                    }
                    catch (const GiwsException::JniException & e)
                    {
                        Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                    }

                    clearWrap<short>(ppiUInt8, iRows);

                    break;

                case SCI_INT16 :
                    sciErr = getMatrixOfInteger16(pvApiCtx, piAddr, &iRows, &iCols, &piInt16);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        freeAllocatedSingleString(pStVarOne);
                        return -1;
                    }

                    ppiInt16 = wrap<short, short>(piInt16, iRows, iCols);

                    /* Launch Java Variable Editor through JNI */
                    try
                    {
                        if (nbRowsIndex == 0 || nbColsIndex == 0)
                        {
                            EditVar::openVariableEditorInteger16(getScilabJavaVM(), ppiInt16, iRows, iCols, pStVarOne);
                        }
                        else
                        {
                            EditVar::refreshVariableEditorInteger16(getScilabJavaVM(), ppiInt16, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                        }
                    }
                    catch (const GiwsException::JniException & e)
                    {
                        Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                    }

                    clearWrap<short>(ppiInt16, iRows);

                    break;

                case SCI_UINT16 :
                    sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddr, &iRows, &iCols, &piUInt16);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        freeAllocatedSingleString(pStVarOne);
                        return -1;
                    }

                    ppiUInt16 = wrap<int, unsigned short>(piUInt16, iRows, iCols);

                    /* Launch Java Variable Editor through JNI */
                    try
                    {
                        if (nbRowsIndex == 0 || nbColsIndex == 0)
                        {
                            EditVar::openVariableEditorUInteger16(getScilabJavaVM(), ppiUInt16, iRows, iCols, pStVarOne);
                        }
                        else
                        {
                            EditVar::refreshVariableEditorUInteger16(getScilabJavaVM(), ppiUInt16, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                        }
                    }
                    catch (const GiwsException::JniException & e)
                    {
                        Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                    }

                    clearWrap<int>(ppiUInt16, iRows);

                    break;

                case SCI_INT32 :
                    sciErr = getMatrixOfInteger32(pvApiCtx, piAddr, &iRows, &iCols, &piInt32);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        freeAllocatedSingleString(pStVarOne);
                        return -1;
                    }

                    ppiInt32 = wrap<int>(piInt32, iRows, iCols);

                    /* Launch Java Variable Editor through JNI */
                    try
                    {
                        if (nbRowsIndex == 0 || nbColsIndex == 0)
                        {
                            EditVar::openVariableEditorInteger32(getScilabJavaVM(), ppiInt32, iRows, iCols, pStVarOne);
                        }
                        else
                        {
                            EditVar::refreshVariableEditorInteger32(getScilabJavaVM(), ppiInt32, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                        }
                    }
                    catch (const GiwsException::JniException & e)
                    {
                        Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                    }

                    clearWrap<int>(ppiInt32, iRows);

                    break;

                case SCI_UINT32 :
                    sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddr, &iRows, &iCols, &piUInt32);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        freeAllocatedSingleString(pStVarOne);
                        return -1;
                    }

                    ppiUInt32 = wrap<long long int, unsigned int>(piUInt32, iRows, iCols);

                    /* Launch Java Variable Editor through JNI */
                    try
                    {
                        if (nbRowsIndex == 0 || nbColsIndex == 0)
                        {
                            EditVar::openVariableEditorUInteger32(getScilabJavaVM(), ppiUInt32, iRows, iCols, pStVarOne);
                        }
                        else
                        {
                            EditVar::refreshVariableEditorUInteger32(getScilabJavaVM(), ppiUInt32, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                        }
                    }
                    catch (const GiwsException::JniException & e)
                    {
                        Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                    }

                    clearWrap<long long int>(ppiUInt32, iRows);

                    break;

                default :
                    Scierror(42, _("%s: Type not handle yet"), fname);
                    freeAllocatedSingleString(pStVarOne);
                    return 0;
            }

            break;

        case sci_boolean_sparse :
            sciErr = getBooleanSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &piNbItem, &piNbItemRow, &piColPos);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                freeAllocatedSingleString(pStVarOne);
                return 0;
            }

            ppiBool = wrapSparse(piNbItem, piNbItemRow, piColPos, iRows, iCols);

            /* Launch Java Variable Editor through JNI */
            try
            {
                if (nbRowsIndex == 0 || nbColsIndex == 0)
                {
                    EditVar::openVariableEditorBooleanSparse(getScilabJavaVM(), ppiBool, iRows, iCols, pStVarOne);
                }
                else
                {
                    EditVar::refreshVariableEditorBooleanSparse(getScilabJavaVM(), ppiBool, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                }
            }
            catch (const GiwsException::JniException & e)
            {
                Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
            }

            clearWrap<int>(ppiBool, iRows);
            break;
        case sci_sparse :
            if (isVarComplex(pvApiCtx, piAddr))
            {
                sciErr = getComplexSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    freeAllocatedSingleString(pStVarOne);
                    return 0;
                }

                ppdblRealMatrix = wrapSparse(piNbItem, piNbItemRow, piColPos, iRows, iCols, pdblReal);
                ppdblImgMatrix = wrapSparse(piNbItem, piNbItemRow, piColPos, iRows, iCols, pdblImg);

                /* Launch Java Variable Editor through JNI */
                try
                {
                    if (nbRowsIndex == 0 || nbColsIndex == 0)
                    {
                        EditVar::openVariableEditorComplexSparse(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, ppdblImgMatrix, iRows, iCols, pStVarOne);
                    }
                    else
                    {
                        EditVar::refreshVariableEditorComplexSparse(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, ppdblImgMatrix, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                    }
                }
                catch (const GiwsException::JniException & e)
                {
                    Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                }

                clearWrap<double>(ppdblRealMatrix, iRows);
                clearWrap<double>(ppdblImgMatrix, iRows);
            }
            else
            {
                sciErr = getSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &piNbItem, &piNbItemRow, &piColPos, &pdblReal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    freeAllocatedSingleString(pStVarOne);
                    return 0;
                }

                ppdblRealMatrix = wrapSparse(piNbItem, piNbItemRow, piColPos, iRows, iCols, pdblReal);

                /* Launch Java Variable Editor through JNI */
                try
                {
                    if (nbRowsIndex == 0 || nbColsIndex == 0)
                    {
                        EditVar::openVariableEditorSparse(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, pStVarOne);
                    }
                    else
                    {
                        EditVar::refreshVariableEditorSparse(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols, rowsIndex, nbRowsIndex, colsIndex, nbColsIndex, pStVarOne);
                    }
                }
                catch (const GiwsException::JniException & e)
                {
                    Scierror(999, _("%s: Java exception arisen:\n%s\n"), fname, e.what());
                }

                clearWrap<double>(ppdblRealMatrix, iRows);
            }
            break;
        default:
            Scierror(42, _("%s: Type not handle yet"), fname);
            freeAllocatedSingleString(pStVarOne);
            return 0;
    }

    freeAllocatedSingleString(pStVarOne);
    FREE(piColPos);
    FREE(piNbItemRow);

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
