/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
#include "struct.hxx"
#include "context.hxx"
#include "ConvertSciVarToMatVar.hxx"

extern "C"
{
#include "GetMatlabVariable.h"
#include "sci_types.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "charEncoding.h"
}

matvar_t *GetStructVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position)
//(void *pvApiCtx, int iVar, const char *name, int matfile_version, char **fieldNames, int nbFields, int * parent, int item_position)
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

    matvar_t **structEntries = NULL;
    int * var_addr = NULL;
    Struct* pStruct = in[iVar - 1]->getAs<Struct>();

    Dims = pStruct->getDims();
    pDims = pStruct->getDimsArray();
    prodDims = pStruct->getSize();

    /* OTHERS LIST ENTRIES: ALL CELL VALUES */

    pszDims = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (pszDims == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetStructVariable");
        return NULL;
    }

    /* Total number of entries */
    for (K = 0; K < Dims; K++)
    {
        pszDims[K] = ((int*)pDims)[K];
    }

    String* pFieldNames = pStruct->getFieldNames();
    wchar_t** ppwchFieldNames = pFieldNames->get();
    int isizeFieldNames = pFieldNames->getSize();

    structEntries = (matvar_t **)MALLOC(sizeof(matvar_t*) * prodDims * isizeFieldNames + 1);
    if (structEntries == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetStructVariable");
        return NULL;
    }

    for (int K = 0; K < prodDims * isizeFieldNames + 1; K++)
    {
        structEntries[K] = NULL;
    }

    SingleStruct** ppSingleStruct = pStruct->get();
    for (int i = 0; i < prodDims; i++)
    {
        for (int j = 0; j < isizeFieldNames; j++)
        {
            structEntries[i * isizeFieldNames + j] = ConvertSciVarToMatVar(ppSingleStruct[i]->get(pFieldNames->get(j)), wide_string_to_UTF8(pFieldNames->get(j)));
            if (structEntries[i * isizeFieldNames + j] == NULL)
            {
                FREE(structEntries);
                FREE(pszDims);
                return NULL;
            }
        }
    }

    return Mat_VarCreate(name, MAT_C_STRUCT, MAT_T_STRUCT, prodDims * isizeFieldNames, pszDims, structEntries, 0);
}
