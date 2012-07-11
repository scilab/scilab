/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "GetMatlabVariable.h"
#include "api_scilab.h"
#include "sci_types.h"
#include "freeArrayOfString.h"
#include "os_strdup.h"
#include "localization.h"

#define MATIO_ERROR if(_SciErr.iErr)            \
    {                                           \
        printError(&_SciErr, 0);                \
        return 0;                               \
    }

matvar_t *GetCharVariable(void* pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{
    char * dataAdr = NULL;
    int rank = 0, i = 0, j = 0;
    int *dims = NULL;
    matvar_t *createdVar = NULL;
    int* piLen = NULL;
    char ** tmp_char = NULL;
    char** pstData = NULL;
    char* pstMatData = NULL;
    int * piAddr = NULL;
    int * item_addr = NULL;
    int var_type;
    int saveDim = 0; /* Used to save old dimension before restoring it */
    SciErr _SciErr;

    if (parent==NULL)
    {
        _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &piAddr);
        MATIO_ERROR;
        _SciErr = getVarType(pvApiCtx, piAddr, &var_type);
        MATIO_ERROR;
    }
    else
    {
        _SciErr = getListItemAddress(pvApiCtx, parent, item_position, &item_addr);
        MATIO_ERROR;
        _SciErr = getVarType(pvApiCtx, item_addr, &var_type);
        MATIO_ERROR;
    }

    if(var_type == sci_strings) /* 2-D array */
    {
        rank = 2;
        if ((dims = (int*)MALLOC(sizeof(int) * rank)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetCharVariable");
            return NULL;
        }

        if (parent==NULL)
        {
            getAllocatedSingleString(pvApiCtx, piAddr, &dataAdr);
            _SciErr = getVarDimension(pvApiCtx, piAddr, &dims[1], &dims[0]);
            MATIO_ERROR;
        }
        else
        {
            _SciErr  = getMatrixOfStringInList(pvApiCtx, parent, item_position, &dims[1], &dims[0], NULL, NULL);
            piLen    = (int *)MALLOC(dims[1]*dims[0]*sizeof(int));
            tmp_char = (char **)MALLOC(dims[1]*dims[0]*sizeof(char *));
            _SciErr  = getMatrixOfStringInList(pvApiCtx, parent, item_position, &dims[1], &dims[0], piLen, NULL);
            for(i=0;i<dims[1]*dims[0];i++)
            {
                tmp_char[i] = (char *)MALLOC((piLen[i]+1)*sizeof(char));
            }
            _SciErr = getMatrixOfStringInList(pvApiCtx, parent, item_position, &dims[1], &dims[0], piLen, tmp_char);
            dataAdr = os_strdup(tmp_char[0]);
            freeArrayOfString(tmp_char, dims[1]*dims[0]);
            FREE(piLen);
        }

        if (dims[0] == 0) /* Empty character string */
        {
            createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, dataAdr, 0);
        }
        else if (dims[0] == 1) /* Scalar character string */
        {
            createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, dataAdr, 0);
        }
        else /* More than one character string -> save as a Cell */
        {
            /* TODO */
            Scierror(999, _("%s: Scilab string matrix saved as Matlab Cell.\n"), "GetCharVariable");
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: String matrix expected.\n"), "GetCharVariable");
        freeArrayOfString(pstData, dims[0]*dims[1]);
        FREE(dims);
        FREE(dataAdr);
        FREE(piLen);
        return NULL;
    }

    return createdVar;
}
