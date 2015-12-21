/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


#include "gatewaystruct.hxx"
#include "cell.hxx"
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

int CreateCellVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
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

    if ((iRank == 2) && ((piDims[0] * piDims[1]) <= 0))
    {
        types::Cell* pCell = new types::Cell();
        out[rhs - 1] = pCell;
        return TRUE;
    }

    types::Cell* pCell = new types::Cell(iRank, piDims);

    matvar_t** allData = (matvar_t**)(matVariable->data);
    types::InternalType** ppIT = new types::InternalType*[matVariable->data_size];
    for (int i = 0; i < iSize; i++)
    {
        ppIT[i] = CreateMatlabTreeVariable(allData[i]);
    }

    pCell->set(ppIT);

    out[rhs - 1] = pCell;

    FREE(piDims);

    return TRUE;
}
