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

#include "gatewaystruct.hxx"
#include "cell.hxx"
#include "context.hxx"
#include "ConvertSciVarToMatVar.hxx"

extern "C"
{
#include "GetMatlabVariable.h"
#include "sci_types.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
}

using namespace types;

matvar_t *GetCellVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position)
{
    GatewayStruct* pStr = (GatewayStruct*)pvApiCtx;
    typed_list in = *pStr->m_pIn;
    InternalType** out = pStr->m_pOut;
    int  Dims = 1;
    int* pDims = NULL;
    int prodDims = 1;

    int nbFields = 0;
    int K = 0;
    size_t *pszDims = NULL;

    matvar_t **cellEntries = NULL;
    int * var_addr = NULL;
    int var_type;
    SciErr sciErr;

    Cell* pCell = in[iVar - 1]->getAs<Cell>();

    Dims = pCell->getDims();
    pDims = pCell->getDimsArray();
    prodDims = pCell->getSize();

    /* OTHERS LIST ENTRIES: ALL CELL VALUES */

    pszDims = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (pszDims == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCellVariable");
        return NULL;
    }

    /* Total number of entries */
    for (K = 0; K < Dims; K++)
    {
        pszDims[K] = ((int*)pDims)[K];
    }

    cellEntries = (matvar_t **) MALLOC(sizeof(matvar_t*) * prodDims);
    if (cellEntries == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCellVariable");
        return NULL;
    }


    types::InternalType** ppIT = pCell->get();
    for (K = 0; K < prodDims; K++)
    {
        cellEntries[K] = ConvertSciVarToMatVar(ppIT[K], name);
        if (cellEntries[K] == NULL)
        {
            FREE(cellEntries);
            FREE(pszDims);
            return NULL;
        }
    }

    return Mat_VarCreate(name, MAT_C_CELL, MAT_T_CELL, Dims, pszDims, cellEntries, 0);
}