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

#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "MALLOC.h"

int CreateDoubleVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
{
    int nbRow = 0, nbCol = 0;
    mat_complex_split_t *mat5ComplexData = NULL;
    SciErr sciErr;
    int *piDims = NULL;
    int i = 0;

    if (matVariable->rank == 2) /* 2-D array */
    {
        nbRow = (int)matVariable->dims[0];
        nbCol = (int)matVariable->dims[1];
        if (matVariable->isComplex == 0)
        {
            if (parent == NULL)
            {
                sciErr = createMatrixOfDouble(pvApiCtx, iVar, nbRow, nbCol, (double*)matVariable->data);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
            else
            {
                sciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, (double*)matVariable->data);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
        }
        else
        {
            /* Since MATIO 1.3.2 data is a ComplexSplit for MAT4 and MAT5 formats */
            mat5ComplexData = matVariable->data;
            if (parent == NULL)
            {
                sciErr = createComplexMatrixOfDouble(pvApiCtx, iVar, nbRow, nbCol, (double*)mat5ComplexData->Re, (double*)mat5ComplexData->Im);
            }
            else
            {
                sciErr = createComplexMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol,
                         (double*)mat5ComplexData->Re, (double*)mat5ComplexData->Im);
            }
        }
    }
    else /* Multi-dimension array -> Scilab HyperMatrix */
    {
        piDims = (int *) MALLOC(matVariable->rank * sizeof(int));
        for (i = 0 ; i < matVariable->rank ; ++i)
        {
            piDims[i] = (int)matVariable->dims[i];
        }

        if (matVariable->isComplex == 0)
        {
            CreateHyperMatrixVariable(pvApiCtx, iVar, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &matVariable->rank,
                                      piDims, (double*)matVariable->data, NULL, parent, item_position);
        }
        else
        {
            mat5ComplexData = (mat_complex_split_t*)matVariable->data;
            CreateHyperMatrixVariable(pvApiCtx, iVar, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &matVariable->rank,
                                      piDims, (double*)mat5ComplexData->Re, (double*)mat5ComplexData->Im, parent, item_position);
        }

        FREE(piDims);
    }
    return TRUE;
}
