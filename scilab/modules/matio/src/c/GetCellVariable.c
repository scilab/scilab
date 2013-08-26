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

#include "freeArrayOfString.h"
#include "MALLOC.h"

matvar_t *GetCellVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position)
{
    int nbFields = 0;
    int K = 0;
    size_t *pszDims = NULL;
    int prodDims = 1;
    matvar_t *dimensionsVariable = NULL;
    matvar_t **cellEntries = NULL;
    int * var_addr = NULL;
    int var_type;
    SciErr sciErr;

    if ((parent == NULL) && (item_position == -1))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else if ((parent != NULL) && (item_position == -1))
    {
        var_addr = parent;
    }
    else
    {
        sciErr = getListItemAddress(pvApiCtx, parent, item_position, &var_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }

    sciErr = getVarType(pvApiCtx, var_addr, &var_type);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if (var_type != sci_mlist)
    {
        Scierror(999, _("%s: Wrong type for first input argument: Mlist expected.\n"), "GetCellVariable");
        return FALSE;
    }

    sciErr = getListItemNumber(pvApiCtx, var_addr, &nbFields);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* FIRST LIST ENTRY: fieldnames --> NO NEED TO BE READ */

    /* SECOND LIST ENTRY: dimensions */
    dimensionsVariable = GetMatlabVariable(pvApiCtx, iVar, "data", 0, var_addr, 2);

    /* OTHERS LIST ENTRIES: ALL CELL VALUES */

    pszDims = (size_t*) MALLOC(dimensionsVariable->rank * sizeof(size_t));
    if (pszDims == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCellVariable");
        return NULL;
    }

    /* Total number of entries */
    for (K = 0; K < dimensionsVariable->rank; K++)
    {
        prodDims *= ((int*)dimensionsVariable->data)[K];
        pszDims[K] = ((int*)dimensionsVariable->data)[K];
    }

    cellEntries = (matvar_t **) MALLOC(sizeof(matvar_t*) * prodDims);
    if (cellEntries == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCellVariable");
        return NULL;
    }

    if (prodDims == 1) /* Scalar cell array */
    {
        cellEntries[0] = GetMatlabVariable(pvApiCtx, iVar , "data", matfile_version, var_addr, 3);
    }
    else
    {
        /* Read all entries */
        for (K = 0; K < prodDims; K++)
        {
            cellEntries[K] = GetMatlabVariable(pvApiCtx, iVar , "data", matfile_version, var_addr, 3 + K);
        }
    }

    return Mat_VarCreate(name, MAT_C_CELL, MAT_T_CELL, dimensionsVariable->rank, pszDims, cellEntries, 0);
}
