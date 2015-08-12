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

#include "GetMatlabVariable.h"
#include "sci_types.h"
#include "api_scilab.h"

matvar_t *GetIntegerVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{
    int rank = 0;
    size_t *pszDims = NULL;
    int *piDims = NULL;
    matvar_t *createdVar = NULL;
    int * var_addr = NULL;
    int i;
    int var_type;
    int integerType;
    SciErr sciErr;
    char * tmp_int8 = NULL;
    short * tmp_int16 = NULL;
    int * tmp_int32 = NULL;
    int * item_addr = NULL;
    unsigned char * tmp_uint8 = NULL;
    unsigned short * tmp_uint16 = NULL;
    unsigned int * tmp_uint32 = NULL;
#ifdef __SCILAB_INT64__
    long long * tmp_int64 = NULL;
    unsigned long long * tmp_uint64 = NULL;
#endif

    if (parent == NULL)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getVarType(pvApiCtx, var_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        sciErr = getListItemAddress(pvApiCtx, parent, item_position, &item_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getVarType(pvApiCtx, item_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }

    if (var_type == sci_ints) /* 2-D array */
    {
        rank = 2;
        if ((pszDims = (size_t*)MALLOC(sizeof(size_t) * rank)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetIntegerVariable");
            return NULL;
        }
        if ((piDims = (int*)MALLOC(sizeof(int) * rank)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetIntegerVariable");
            return NULL;
        }

        if (parent == NULL)
        {
            sciErr = getMatrixOfIntegerPrecision(pvApiCtx, var_addr, &integerType);
        }
        else
        {
            sciErr = getMatrixOfIntegerPrecision(pvApiCtx, item_addr, &integerType);
        }

        switch (integerType)
        {
            case SCI_INT8: /* INT8 */
                if (parent == NULL)
                {
                    sciErr = getMatrixOfInteger8(pvApiCtx, var_addr, &piDims[0], &piDims[1], &tmp_int8);
                }
                else
                {
                    sciErr = getMatrixOfInteger8InList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1], &tmp_int8);
                }

                for (i = 0; i < rank; i++)
                {
                    pszDims[i] = piDims[i];
                }

                createdVar = Mat_VarCreate(name, MAT_C_INT8, MAT_T_INT8, rank, pszDims, tmp_int8, 0);
                break;
            case SCI_INT16: /* INT16 */
                if (parent == NULL)
                {
                    sciErr = getMatrixOfInteger16(pvApiCtx, var_addr, &piDims[0], &piDims[1], &tmp_int16);
                }
                else
                {
                    sciErr = getMatrixOfInteger16InList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1], &tmp_int16);
                }

                for (i = 0; i < rank; i++)
                {
                    pszDims[i] = piDims[i];
                }

                createdVar = Mat_VarCreate(name, MAT_C_INT16, MAT_T_INT16, rank, pszDims, tmp_int16, 0);
                break;
            case SCI_INT32: /* INT32 */
                if (parent == NULL)
                {
                    sciErr = getMatrixOfInteger32(pvApiCtx, var_addr, &piDims[0], &piDims[1], &tmp_int32);
                }
                else
                {
                    sciErr = getMatrixOfInteger32InList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1], &tmp_int32);
                }

                for (i = 0; i < rank; i++)
                {
                    pszDims[i] = piDims[i];
                }

                createdVar = Mat_VarCreate(name, MAT_C_INT32, MAT_T_INT32, rank, pszDims, tmp_int32, 0);
                break;
            case SCI_UINT8: /* UINT8 */
                if (parent == NULL)
                {
                    sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, var_addr, &piDims[0], &piDims[1], &tmp_uint8);
                }
                else
                {
                    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1], &tmp_uint8);
                }

                for (i = 0; i < rank; i++)
                {
                    pszDims[i] = piDims[i];
                }

                createdVar = Mat_VarCreate(name, MAT_C_UINT8, MAT_T_UINT8, rank, pszDims, tmp_uint8, 0);
                break;
            case SCI_UINT16: /* UINT16 */
                if (parent == NULL)
                {
                    sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, var_addr, &piDims[0], &piDims[1], &tmp_uint16);
                }
                else
                {
                    sciErr = getMatrixOfUnsignedInteger16InList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1], &tmp_uint16);
                }

                for (i = 0; i < rank; i++)
                {
                    pszDims[i] = piDims[i];
                }

                createdVar = Mat_VarCreate(name, MAT_C_UINT16, MAT_T_UINT16, rank, pszDims, tmp_uint16, 0);
                break;
            case SCI_UINT32: /* UINT32 */
                if (parent == NULL)
                {
                    sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, var_addr, &piDims[0], &piDims[1], &tmp_uint32);
                }
                else
                {
                    sciErr = getMatrixOfUnsignedInteger32InList(pvApiCtx, parent, item_position, &piDims[0], &piDims[1], &tmp_uint32);
                }

                for (i = 0; i < rank; i++)
                {
                    pszDims[i] = piDims[i];
                }

                createdVar = Mat_VarCreate(name, MAT_C_UINT32, MAT_T_UINT32, rank, pszDims, tmp_uint32, 0);
                break;
            default:
                createdVar = NULL;
                break;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: Integer matrix expected.\n"), "GetIntegerVariable");
    }

    FREE(pszDims);
    FREE(piDims);

    return createdVar;
}
