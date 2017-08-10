/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
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
        FREE(piDims);
        return TRUE;
    }

    types::Struct* pStruct = new types::Struct(iRank, piDims);

    matvar_t** allData = (matvar_t**)(matVariable->data);

    wchar_t* temp;
    int iSizeStruct = Mat_VarGetNumberOfFields(matVariable);
    for (int i = 0; i < iSizeStruct; i++)
    {
        temp = to_wide_string((char*)allData[i]->name);
        std::wstring wstField(temp);
        FREE(temp);
        pStruct->addField(wstField);
    }

    for (int i = 0; i < iSize; i++)
    {
        for (int j = 0; j < iSizeStruct; j++)
        {
            temp = to_wide_string((char*)allData[j]->name);
            std::wstring wstField(temp);
            FREE(temp);
            pStruct->get(i)->set(wstField, CreateMatlabTreeVariable(allData[i * iSizeStruct + j]));
        }
    }

    out[rhs - 1] = pStruct;

    FREE(piDims);

    return TRUE;
}

