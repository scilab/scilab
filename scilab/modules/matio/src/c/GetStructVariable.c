/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"

matvar_t *GetStructVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, char **fieldNames, int nbFields, int * parent, int item_position)
{
    int fieldIndex = 0;
    int valueIndex = 0;
    int K = 0;
    size_t *pszDims = NULL;
    int prodDims = 1;
    matvar_t *dimensionsVariable = NULL;
    matvar_t **structEntries = NULL;
    int * var_addr = NULL;
    int * list_addr = NULL;
    SciErr sciErr;

    if (parent == NULL)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr);
    }
    else
    {
        sciErr = getListItemAddress(pvApiCtx, parent, item_position, &var_addr);
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return NULL;
    }

    /* FIRST LIST ENTRY: fieldnames --> NO NEED TO BE READ */

    /* SECOND LIST ENTRY: dimensions */
    /* Second input argument = "data" because we do not need to give the format because this variable is just temp */
    dimensionsVariable = GetMatlabVariable(pvApiCtx, iVar, "data", 0, var_addr, 2);

    pszDims = (size_t*) MALLOC(dimensionsVariable->rank * sizeof(size_t));
    if (pszDims == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetStructVariable");
        freeArrayOfString(fieldNames, nbFields);
        return NULL;
    }

    /* Total number of entries */
    for (K = 0; K < dimensionsVariable->rank; K++)
    {
        prodDims *= ((int*)dimensionsVariable->data)[K];
        pszDims[K] = ((int*)dimensionsVariable->data)[K];
    }

    /* OTHERS LIST ENTRIES: ALL STRUCT VALUES */
    structEntries = (matvar_t **) MALLOC (sizeof(matvar_t*) * (prodDims * (nbFields - 2) + 1));
    if (structEntries == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetStructVariable");
        freeArrayOfString(fieldNames, nbFields);
        return NULL;
    }
    for (K = 0; K < prodDims * (nbFields - 2) + 1; K++)
    {
        structEntries[K] = NULL;
    }

    if (prodDims == 1) /* Scalar struct array */
    {
        for (fieldIndex = 2; fieldIndex < nbFields; fieldIndex++)
        {
            structEntries[fieldIndex - 2] = GetMatlabVariable(pvApiCtx, iVar , fieldNames[fieldIndex], matfile_version, var_addr, fieldIndex + 1);
        }
    }
    else
    {
        /* All the values of each field are stored in a list */

        /* Read all entries */
        for (fieldIndex = 2; fieldIndex < nbFields; fieldIndex++)
        {
            sciErr = getListInList(pvApiCtx, var_addr, fieldIndex + 1, &list_addr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return NULL;
            }

            for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
            {
                structEntries[(fieldIndex - 1) + (nbFields - 2)*valueIndex] = GetMatlabVariable(pvApiCtx, iVar , fieldNames[fieldIndex], matfile_version, list_addr, valueIndex + 1);
            }
        }
    }

    return Mat_VarCreate(name, MAT_C_STRUCT, MAT_T_STRUCT, dimensionsVariable->rank, pszDims, structEntries, 0);
}
