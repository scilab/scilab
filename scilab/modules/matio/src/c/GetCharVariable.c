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
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "MALLOC.h"
#include "localization.h"

matvar_t *GetCharVariable(int iVar, const char *name, int * parent, int item_position)
{
    char * dataAdr = NULL;
    int rank = 0, i = 0, j = 0;
    int *dims = NULL;
    matvar_t *createdVar = NULL;
    int* piLen = NULL;
    char** pstData = NULL;
    char* pstMatData = NULL;
    int * piAddr = NULL;
    int * item_addr = NULL;
    int var_type;
    int saveDim = 0; /* Used to save old dimension before restoring it */
    SciErr sciErr;

    if (parent==NULL)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, iVar, &piAddr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
        sciErr = getVarType(pvApiCtx, piAddr, &var_type);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
    }
    else
    {
        sciErr = getListItemAddress(pvApiCtx, parent, item_position, &item_addr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
        sciErr = getVarType(pvApiCtx, item_addr, &var_type);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return NULL;
        }
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
            // First call to retrieve dimensions
            sciErr = getMatrixOfString(pvApiCtx, piAddr, &dims[0], &dims[1], NULL, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            piLen = (int *)MALLOC(dims[0] * dims[1] * sizeof(int));
            // Second call to retrieve length of each string
            sciErr = getMatrixOfString(pvApiCtx, piAddr, &dims[0], &dims[1], piLen, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            pstData = (char**)MALLOC(sizeof(char*) * dims[0] * dims[1]);
            for(i = 0 ; i < dims[0] * dims[1] ; i++)
            {
                pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1)); //+ 1 for null termination
            }
            // Third call to retrieve data
            sciErr = getMatrixOfString(pvApiCtx, piAddr, &dims[0], &dims[1], piLen, pstData);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            dataAdr = strdup(pstData[0]);
        }
        else
        {
            // First call to retrieve dimensions
            sciErr  = getMatrixOfStringInList(pvApiCtx, parent, item_position, &dims[0], &dims[1], NULL, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return NULL;
            }
            piLen = (int *)MALLOC(dims[0] * dims[1] * sizeof(int));
            // Second call to retrieve length of each string
            sciErr = getMatrixOfStringInList(pvApiCtx, parent, item_position, &dims[0], &dims[1], piLen, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return NULL;
            }
            pstData = (char**)MALLOC(sizeof(char*) * dims[0] * dims[1]);
            for(i = 0 ; i < dims[0] * dims[1] ; i++)
            {
                pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1)); //+ 1 for null termination
            }
            // Third call to retrieve data
            sciErr = getMatrixOfStringInList(pvApiCtx, parent, item_position, &dims[0], &dims[1], piLen, pstData);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return NULL;
            }
            dataAdr = strdup(pstData[0]);
        }

        if (dims[0] == 0) /* Empty character string */
        {
            createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, pstData[0], 0);
        }
        else if (dims[0]*dims[1] == 1) /* Scalar character string */
        {
            saveDim = dims[1];
            dims[1] = piLen[0];
            createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, pstData[0], 0);
            dims[1] = saveDim;
        }
        else /* More than one character string -> save as a Cell */
        {
            if (dims[0] == 1)
            {
                /* TODO: Should be saved as a cell */
                Scierror(999, _("%s: Row array of strings saving is not implemented.\n"), "GetCharVariable");
                freeArrayOfString(pstData, dims[0]*dims[1]);
                FREE(dims);
                FREE(dataAdr);
                FREE(piLen);
                return NULL;
            }
            else if (dims[1] == 1)
            {
                /* Check that all strings have the same length */
                for (i = 0 ; i < dims[0] ; i++)
                {
                    if (piLen[0] != piLen[i])
                    {
                        /* TODO: Should be saved as a cell */
                        Scierror(999, _("%s: Column array of strings with different lengths saving is not implemented.\n"), "GetCharVariable");
                        freeArrayOfString(pstData, dims[0]*dims[1]);
                        FREE(dims);
                        FREE(dataAdr);
                        FREE(piLen);
                        return NULL;
                    }
                }

                /* Reorder characters */
                pstMatData = (char*)MALLOC(sizeof(char) * dims[0] * piLen[0]);
                for (i = 0 ; i < dims[0] ; i++)
                {
                    for (j = 0 ; j < piLen[0] ; j++)
                    {
                        pstMatData[i+j*dims[0]] = pstData[i][j];
                    }
                }

                /* Save the variable */
                saveDim = dims[1];
                dims[1] = piLen[0];
                createdVar = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, rank, dims, pstMatData, 0);
                dims[1] = saveDim;

                freeArrayOfString(pstData, dims[0]*dims[1]); /* FREE now because dimensions are changed just below */
                FREE(pstMatData);
                FREE(dims);
                FREE(dataAdr);
                FREE(piLen);
            }
            else
            {
                /* TODO: Should be saved as a cell */
                Scierror(999, _("%s: 2D array of strings saving is not implemented.\n"), "GetCharVariable");
                freeArrayOfString(pstData, dims[0]*dims[1]);
                FREE(dims);
                FREE(dataAdr);
                FREE(piLen);
                return NULL;
            }
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
