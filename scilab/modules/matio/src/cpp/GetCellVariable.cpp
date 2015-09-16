/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetMatlabVariable.hxx"
#include "gatewaystruct.hxx"
#include "context.hxx"
#include "ConvertSciVarToMatVar.hxx"

extern "C"
{
#include "sci_types.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
}

matvar_t *GetCellVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position)
{
    types::GatewayStruct* pGS = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pGS->m_pIn;

    if (in[iVar - 1]->isCell() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: String matrix expected.\n"), "GetCellVariable");
        return NULL;
    }

    types::Cell* pCell = in[iVar - 1]->getAs<types::Cell>();

    return GetCellMatVar(pCell, name, matfile_version);
}

matvar_t* GetCellMatVar(types::Cell* pCell, const char* name, int matfile_version)
{
    matvar_t **cellEntries = NULL;

    int Dims = pCell->getDims();
    int* pDims = pCell->getDimsArray();
    int prodDims = pCell->getSize();

    matvar_t* pMatVarOut = NULL;

    /* OTHERS LIST ENTRIES: ALL CELL VALUES */

    size_t* pszDims = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (pszDims == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCellMatVar");
        return NULL;
    }

    /* Total number of entries */
    for (int K = 0; K < Dims; ++K)
    {
        pszDims[K] = ((int*)pDims)[K];
    }

    cellEntries = (matvar_t **)MALLOC(sizeof(matvar_t*) * prodDims);
    if (cellEntries == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCellMatVar");
        return NULL;
    }


    types::InternalType** ppIT = pCell->get();
    for (int K = 0; K < prodDims; ++K)
    {
        cellEntries[K] = ConvertSciVarToMatVar(ppIT[K], name, matfile_version);
        if (cellEntries[K] == NULL)
        {
            FREE(cellEntries);
            FREE(pszDims);
            return NULL;
        }
    }

    pMatVarOut = Mat_VarCreate(name, MAT_C_CELL, MAT_T_CELL, Dims, pszDims, cellEntries, 0);

    FREE(pszDims);

    return pMatVarOut;
}
