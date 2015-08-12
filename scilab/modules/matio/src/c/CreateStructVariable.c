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

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "CreateMatlabVariable.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

int CreateStructVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
{
    char **fieldNames = NULL;
    int nbFields = 0;
    int fieldIndex = 0;
    int K = 0;
    int prodDims = 0;
    int valueIndex = 0;
    matvar_t *fieldMatVar = NULL;
    matvar_t ** allData = NULL;
    int * cell_addr = NULL;
    int * cell_entry_addr = NULL;
    int type;
    SciErr sciErr;
    int *piDims = NULL;
    int i = 0;

    /* Fields of the struct */
    nbFields = 2; /* "st" "dims" */
    nbFields += Mat_VarGetNumberOfFields(matVariable);

    fieldNames = (char**) MALLOC(sizeof(char*) * nbFields);
    if (fieldNames == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
        return FALSE;
    }

    fieldNames[0] = strdup("st");
    if (fieldNames[0] == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
        return FALSE;
    }
    fieldNames[1] = strdup("dims");
    if (fieldNames[1] == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
        return FALSE;
    }

    for (fieldIndex = 1; fieldIndex < nbFields - 1; fieldIndex++)
    {
        fieldMatVar = Mat_VarGetStructField(matVariable, &fieldIndex, MAT_BY_INDEX, 0);
        fieldNames[fieldIndex + 1] = strdup(fieldMatVar->name);
        if (fieldNames[fieldIndex + 1] == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "CreateStructVariable");
            return FALSE;
        }
    }

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
    sciErr = createMatrixOfStringInList(pvApiCtx, iVar, cell_addr, 1, 1, nbFields, fieldNames);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* SECOND LIST ENTRY: Dimensions (int32 type) */
    if (nbFields == 2) /* Empty struct must have size 0x0 in Scilab */
    {
        matVariable->dims[0] = 0;
        matVariable->dims[1] = 0;
    }

    piDims = (int *) MALLOC(matVariable->rank * sizeof(int));
    for (i = 0 ; i < matVariable->rank ; ++i)
    {
        piDims[i] = (int)matVariable->dims[i];
    }

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

    if (prodDims == 1) /* Scalar struct */
    {
        for (fieldIndex = 0; fieldIndex < nbFields - 2; fieldIndex++)
        {
            /* Create list entry in the stack */
            if (!CreateMatlabVariable(pvApiCtx, iVar, allData[fieldIndex], cell_addr, fieldIndex + 3)) /* Could not Create Variable */
            {
                if (allData[fieldIndex]->class_type != 0) /* class is 0 for not initialized fields */
                {
                    sciprint("Do not know how to read a variable of class %d.\n", allData[fieldIndex]->class_type);
                }
            }
        }
    }
    else
    {
        for (fieldIndex = 0; fieldIndex < nbFields - 2; fieldIndex++)
        {
            sciErr = createListInList(pvApiCtx, iVar, cell_addr, fieldIndex + 3, prodDims, &cell_entry_addr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            for (valueIndex = 0; valueIndex < prodDims; valueIndex++)
            {
                /* Create list entry in the stack */
                if (!CreateMatlabVariable(pvApiCtx, iVar, allData[(fieldIndex) + (nbFields - 2)*valueIndex], cell_entry_addr, valueIndex + 1)) /* Could not Create Variable */
                {
                    if (allData[(fieldIndex) + (nbFields - 2)*valueIndex]->class_type != 0) /* class is 0 for not initialized fields */
                    {
                        sciprint("Do not know how to read a variable of class %d.\n", allData[(fieldIndex) + (nbFields - 2)*valueIndex]->class_type);
                    }
                }
            }
        }
    }

    freeArrayOfString(fieldNames, nbFields);

    return TRUE;
}

