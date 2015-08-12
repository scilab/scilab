/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.h"
#include "sci_types.h"
#include "api_scilab.h"

matvar_t * GetDoubleVariable(void *pvApiCtx, int iVar, const char* name, int matfile_version, int * parent, int item_position)
{
    double * realDataAdr = NULL, * complexDataAdr = NULL;
    int i = 0;
    int rank = 0;
    int *piDims = NULL;
    size_t *pszDims = NULL;
    int isComplex = 0;
    struct mat_complex_split_t mat5ComplexData;
    matvar_t *createdVar = NULL;
    int * var_addr = NULL;
    int * item_addr = NULL;
    int var_type;
    SciErr sciErr;

    if (parent == NULL)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getVarType(pvApiCtx, var_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        isComplex = isVarComplex(pvApiCtx, var_addr);
    }
    else
    {
        sciErr = getListItemAddress(pvApiCtx, parent, item_position, &item_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getVarType(pvApiCtx, item_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        isComplex = isVarComplex(pvApiCtx, item_addr);
    }

    if (var_type == sci_matrix) /* 2-D array */
    {
        rank = 2;
        if ((piDims = (int*)MALLOC(sizeof(int) * rank)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetDoubleVariable");
            return NULL;
        }
        if ((pszDims = (size_t*)MALLOC(sizeof(size_t) * rank)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetDoubleVariable");
            return NULL;
        }

        if (isComplex)
        {
            if (parent == NULL)
            {
                sciErr = getComplexMatrixOfDouble(pvApiCtx, var_addr, &piDims[0], &piDims[1], &realDataAdr, &complexDataAdr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
            else
            {
                sciErr = getComplexMatrixOfDoubleInList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1],
                                                        &realDataAdr, &complexDataAdr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
        }
        else
        {
            if (parent == NULL)
            {
                sciErr = getMatrixOfDouble(pvApiCtx, var_addr, &piDims[0], &piDims[1], &realDataAdr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
            else
            {
                sciErr = getMatrixOfDoubleInList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1], &realDataAdr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
        }

        for (i = 0; i < rank; i++)
        {
            pszDims[i] = piDims[i];
        }

        if (isComplex == 0)
        {
            createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, pszDims, realDataAdr, 0);
        }
        else
        {
            if (matfile_version == MAT_FT_MAT4) /* MATLAB4: data is a table of value */
            {
                createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, pszDims, realDataAdr, MAT_F_COMPLEX);
            }
            else /* MATLAB5 file: data is a mat_complex_split_t */
            {
                mat5ComplexData.Re = realDataAdr;
                mat5ComplexData.Im = complexDataAdr;
                createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, pszDims, &mat5ComplexData, MAT_F_COMPLEX);
            }
            createdVar->isComplex = 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: Double matrix expected.\n"), "GetDoubleVariable");
    }

    FREE(piDims);
    FREE(pszDims);

    return createdVar;
}
