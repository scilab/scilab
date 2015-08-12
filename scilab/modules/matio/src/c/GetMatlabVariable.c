/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.h"
#include "sci_types.h"
#include "api_scilab.h"

matvar_t *GetMatlabVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position)
{
    int * var_addr = NULL;
    int var_type;
    SciErr sciErr;
    matvar_t * tmp_res = NULL;

    if (parent == NULL)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
        sciErr = getVarType(pvApiCtx, var_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
    }
    else
    {
        sciErr = getListItemAddress(pvApiCtx, parent, item_position, &var_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
        sciErr = getVarType(pvApiCtx, var_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
    }

    switch (var_type)
    {
        case sci_matrix:
            tmp_res = GetDoubleVariable(pvApiCtx, iVar, name, matfile_version, parent, item_position);
            break;
        case sci_strings:
            tmp_res = GetCharVariable(pvApiCtx, iVar, name, parent, item_position);
            break;
        case sci_ints:
            tmp_res = GetIntegerVariable(pvApiCtx, iVar, name, parent, item_position);
            break;
        case sci_mlist:
            /* Only cells structs and hypermatrices are managed */
            if (item_position > 0)
            {
                tmp_res = GetMlistVariable(pvApiCtx, iVar, name, matfile_version, parent, item_position);
            }
            else
            {
                tmp_res = GetMlistVariable(pvApiCtx, iVar, name, matfile_version, parent, -1);
            }
            break;
        case sci_sparse:
            if (item_position > 0)
            {
                tmp_res = GetSparseVariable(pvApiCtx, iVar, name, parent, item_position);
            }
            else
            {
                tmp_res = GetSparseVariable(pvApiCtx, iVar, name, parent, -1);
            }
            break;
        default:
            sciprint("Do not known how to get variable of type %d\n", var_type);
            tmp_res = NULL;
    }

    return tmp_res;
}
