/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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
        FREE(piDims);
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
