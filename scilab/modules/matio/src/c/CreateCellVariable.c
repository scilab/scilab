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

#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

int CreateCellVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
{
    static const char *fieldNames[] = {"ce", "dims", "entries"};
    int nbFields = 3;
    int K = 0;
    int prodDims = 0;
    int valueIndex = 0, type;
    int * cell_addr = NULL;
    int * cell_entry_addr = NULL;
    matvar_t ** allData = NULL;
    SciErr sciErr;

    int *piDims = NULL;
    int i = 0;

    /* Returned mlist initialization */
    if (parent == NULL)
    {
        sciErr = createMList(pvApiCtx, iVar, nbFields, &cell_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        sciErr = createMListInList(pvApiCtx, iVar, parent, item_position, nbFields, &cell_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }

    /* FIRST LIST ENTRY: fieldnames */
    sciErr = createMatrixOfStringInList(pvApiCtx, iVar, cell_addr, 1, 1, nbFields, (const char **)fieldNames);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    piDims = (int *) MALLOC(matVariable->rank * sizeof(int));
    for (i = 0 ; i < matVariable->rank ; ++i)
    {
        piDims[i] = (int)matVariable->dims[i]; // Copy dims to make size_t values fit int
    }

    /* SECOND LIST ENTRY: Dimensions (int32 type) */
    if (matVariable->rank == 2) /* Two dimensions */
    {
        sciErr = createMatrixOfInteger32InList(pvApiCtx, iVar, cell_addr, 2, 1, matVariable->rank, piDims);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else /* 3 or more dimensions -> Scilab HyperMatrix */
    {
        type = I_INT32;
        CreateHyperMatrixVariable(pvApiCtx, iVar, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,
                                  &type, &matVariable->rank, piDims, (double*)matVariable->data,
                                  NULL, cell_addr, 2);
    }

    FREE(piDims);

    /* ALL OTHER ENTRIES: Fields data */
    prodDims = 1;
    for (K = 0; K < matVariable->rank; K++)
    {
        prodDims *= (int)matVariable->dims[K];
    }

    allData = (matvar_t**) (matVariable->data);

    if (prodDims == 1) /* Scalar cell */
    {
        /* Create list entry in the stack */
        if (!CreateMatlabVariable(pvApiCtx, iVar, allData[0], cell_addr, 3)) /* Could not Create Variable */
        {
            sciprint("Do not know how to read a variable of class %d.\n", allData[0]->class_type);
        }
    }
    else
    {
        sciErr = createListInList(pvApiCtx, iVar, cell_addr, 3, prodDims, &cell_entry_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
        {
            /* Create list entry in the stack */
            if (!CreateMatlabVariable(pvApiCtx, iVar, allData[valueIndex], cell_entry_addr, valueIndex + 1)) /* Could not Create Variable */
            {
                sciprint("Do not know how to read a variable of class %d.\n", allData[valueIndex]->class_type);
            }
        }
    }

    return TRUE;
}
