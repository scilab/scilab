/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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

#include "GetMatlabVariable.hxx"
#include "ConvertSciVarToMatVar.hxx"

extern "C"
{
#include "sci_types.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "charEncoding.h"
}

matvar_t *GetStructVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position)
{
    types::GatewayStruct* pGS = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pGS->m_pIn;

    if (in[iVar - 1]->isStruct() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: string expected.\n"), "GetStructVariable");
        return NULL;
    }

    types::Struct* pStruct = in[iVar - 1]->getAs<types::Struct>();

    return GetStructMatVar(pStruct, name, matfile_version);
}

matvar_t* GetStructMatVar(types::Struct* pStruct, const char *name, int matfile_version)
{
    matvar_t **structEntries = NULL;

    int Dims = pStruct->getDims();
    int* pDims = pStruct->getDimsArray();
    int prodDims = pStruct->getSize();

    matvar_t* pMatVarOut = NULL;

    /* OTHERS LIST ENTRIES: ALL CELL VALUES */
    size_t* pszDims = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (pszDims == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetStructMatVar");
        return NULL;
    }

    types::String* pFieldNames = pStruct->getFieldNames();
    wchar_t** ppwchFieldNames = pFieldNames->get();
    int isizeFieldNames = pFieldNames->getSize();

    /* Total number of entries */
    for (int K = 0; K < Dims; ++K)
    {
        pszDims[K] = ((int*)pDims)[K];
    }

    structEntries = (matvar_t **)MALLOC(sizeof(matvar_t*) * prodDims * isizeFieldNames + 1);
    if (structEntries == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetStructMatVar");
        FREE(pszDims);
        return NULL;
    }

    for (int K = 0; K < prodDims * isizeFieldNames + 1; ++K)
    {
        structEntries[K] = NULL;
    }

    types::SingleStruct** ppSingleStruct = pStruct->get();

    for (int i = 0; i < prodDims; i++)
    {
        for (int j = 0; j < isizeFieldNames; j++)
        {
            structEntries[i * isizeFieldNames + j] = ConvertSciVarToMatVar(ppSingleStruct[i]->get(pFieldNames->get(j)), wide_string_to_UTF8(pFieldNames->get(j)), matfile_version);
            if (structEntries[i * isizeFieldNames + j] == NULL)
            {
                FREE(structEntries);
                FREE(pszDims);
                return NULL;
            }
        }
    }

    pMatVarOut = Mat_VarCreate(name, MAT_C_STRUCT, MAT_T_STRUCT, prodDims * isizeFieldNames, pszDims, structEntries, 0);

    FREE(pszDims);
    return pMatVarOut;
}
