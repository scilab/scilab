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

#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "sci_malloc.h"

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

        CreateHyperMatrixVariable(pvApiCtx, iVar, matVariable->class_type, &matVariable->isComplex, &matVariable->rank,
                                  piDims, matVariable, parent, item_position);

        FREE(piDims);
    }
    return TRUE;
}
