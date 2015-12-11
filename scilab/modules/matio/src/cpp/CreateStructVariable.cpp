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

#include "gatewaystruct.hxx"
#include "struct.hxx"
#include "string.hxx"
#include "context.hxx"
#include "CreateMatlabTreeVariable.hxx"

extern "C"
{
#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
}

int CreateStructVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
{
    types::GatewayStruct* pStr = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int  iSize = 1;

    int rhs = iVar - *getNbInputArgument(pvApiCtx);

    int iRank = matVariable->rank;
    int* piDims = (int *)MALLOC(iRank * sizeof(int));

    for (int i = 0; i < iRank; ++i)
    {
        piDims[i] = (int)matVariable->dims[i];
        iSize *= piDims[i];
    }

    if (matVariable->data == NULL)
    {
        types::Struct* pStruct = new types::Struct();
        out[rhs - 1] = pStruct;
        return TRUE;
    }

    types::Struct* pStruct = new types::Struct(iRank, piDims);

    matvar_t** allData = (matvar_t**)(matVariable->data);

    int iSizeStruct = Mat_VarGetNumberOfFields(matVariable);
    for (int i = 0; i < iSizeStruct; i++)
    {
        std::wstring wstField(to_wide_string((char*)allData[i]->name));
        pStruct->addField(wstField);
    }

    for (int i = 0; i < iSize; i++)
    {
        for (int j = 0; j < iSizeStruct; j++)
        {
            std::wstring wstField(to_wide_string((char*)allData[j]->name));
            pStruct->get(i)->set(wstField, CreateMatlabTreeVariable(allData[i * iSizeStruct + j]));
        }
    }

    out[rhs - 1] = pStruct;

    FREE(piDims);

    return TRUE;
}

