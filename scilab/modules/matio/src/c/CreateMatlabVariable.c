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

int CreateMatlabVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
{
    SciErr sciErr;

    /* To be sure isComplex is 0 or 1 */
    matVariable->isComplex =  matVariable->isComplex != 0;
    switch (matVariable->class_type)
    {
        case MAT_C_CELL: /* 1 */
            CreateCellVariable(pvApiCtx, iVar, matVariable, parent, item_position);
            break;
        case MAT_C_STRUCT: /* 2 */
            CreateStructVariable(pvApiCtx, iVar, matVariable, parent, item_position);
            break;
        case MAT_C_CHAR: /* 4 */
            CreateCharVariable(pvApiCtx, iVar, matVariable, parent, item_position);
            break;
        case MAT_C_SPARSE: /* 5 */
            CreateSparseVariable(pvApiCtx, iVar, matVariable, parent, item_position);
            break;
        case MAT_C_DOUBLE: /* 6 */
        case MAT_C_SINGLE: /* 7 */
            CreateDoubleVariable(pvApiCtx, iVar, matVariable, parent, item_position);
            break;
        case MAT_C_INT8: /* 8 */
            CreateIntegerVariable(pvApiCtx, iVar, SCI_INT8, matVariable, parent, item_position);
            break;
        case MAT_C_UINT8: /* 9 */
            if (matVariable->isLogical != 0)
            {
                CreateBooleanVariable(pvApiCtx, iVar, matVariable, parent, item_position);
            }
            else
            {
                CreateIntegerVariable(pvApiCtx, iVar, SCI_UINT8, matVariable, parent, item_position);
            }
            break;
        case MAT_C_INT16: /* 10 */
            CreateIntegerVariable(pvApiCtx, iVar, SCI_INT16, matVariable, parent, item_position);
            break;
        case MAT_C_UINT16: /* 11 */
            CreateIntegerVariable(pvApiCtx, iVar, SCI_UINT16, matVariable, parent, item_position);
            break;
        case MAT_C_INT32: /* 12 */
            CreateIntegerVariable(pvApiCtx, iVar, SCI_INT32, matVariable, parent, item_position);
            break;
        case MAT_C_UINT32: /* 13 */
            CreateIntegerVariable(pvApiCtx, iVar, SCI_UINT32, matVariable, parent, item_position);
            break;
#ifdef __SCILAB_INT64__
        case MAT_C_INT64: /* 14 */
            CreateIntegerVariable(pvApiCtx, iVar, SCI_UINT64, matVariable, parent, item_position);
            break;
        case MAT_C_UINT64: /* 15 */
            CreateIntegerVariable(pvApiCtx, iVar, SCI_UINT64, matVariable, parent, item_position);
            break;
#else
        case MAT_C_INT64: /* 14: no Scilab equivalent */
        case MAT_C_UINT64: /* 15: no Scilab equivalent */
#endif
        case MAT_C_OBJECT: /* 3 to be written */
        case MAT_C_FUNCTION: /* 16 to be written */
        default:
            /* Empty matrix returned */
            if (parent == NULL)
            {
                createEmptyMatrix(pvApiCtx, iVar);
            }
            else
            {
                sciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, 0, 0, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
            return FALSE;
    }

    return TRUE;
}
