/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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

#include "GetMatlabVariable.hxx"
#include "ConvertSciVarToMatVar.hxx"

extern "C"
{
#include <string.h>
#include "api_scilab.h"
#include "sci_types.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "localization.h"
}

matvar_t *GetCharVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{
    types::GatewayStruct* pGS = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pGS->m_pIn;

    if (in[iVar - 1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: string expected.\n"), "GetCharVariable");
        return NULL;
    }

    types::String* pStr = in[iVar - 1]->getAs<types::String>();

    return GetCharMatVar(pStr, name);
}

matvar_t* GetCharMatVar(types::String* pStr, const char* name)
{
    int Dims = pStr->getDims();
    int* pDims = pStr->getDimsArray();
    matvar_t * pMatVarOut = NULL;
    int iLen = 0;

    if (Dims > 2)
    {
        Scierror(999, _("%s: 2D array of strings saving is not implemented.\n"), "GetCharMatVar");
        return NULL;
    }

    if (pDims[1] != 1)
    {
        if (pDims[0] != 1)
        {
            Scierror(999, _("%s: 2D array of strings saving is not implemented.\n"), "GetCharMatVar");
        }
        else
        {
            Scierror(999, _("%s: Row array of strings saving is not implemented.\n"), "GetCharMatVar");
        }
        return NULL;
    }


    char* pcName = NULL;
    iLen = wcslen(pStr->get(0));

    for (int i = 1; i < pStr->getSize(); ++i)
    {
        pcName = wide_string_to_UTF8(pStr->get(i));
        if (iLen != strlen(pcName))
        {
            Scierror(999, _("%s: Column array of strings with different lengths saving is not implemented.\n"), "GetCharMatVar");
            FREE(pcName);
            return NULL;
        }
        FREE(pcName);
    }

    size_t* psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (psize_t == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCharMatVar");
        return NULL;
    }

    /* Reorder characters */
    char* pstMatData = NULL;
    if (iLen != 0)
    {

        char** ppcName = (char**)MALLOC(sizeof(char*) * pDims[0] * pDims[1]);
        if (ppcName == NULL)
        {
            FREE(psize_t);
            Scierror(999, _("%s: No more memory.\n"), "GetCharMatVar");
            return NULL;
        }

        pstMatData = (char*)MALLOC(sizeof(char) * pDims[0] * iLen);
        if (pstMatData == NULL)
        {
            FREE(ppcName);
            FREE(psize_t);
            Scierror(999, _("%s: No more memory.\n"), "GetCharMatVar");
            return NULL;
        }

        for (int i = 0; i < pDims[0]; ++i)
        {
            ppcName[i] = wide_string_to_UTF8(pStr->get(i));
            if (ppcName[i] == NULL)
            {
                for (int idelete = 0; idelete < i; ++idelete)
                {
                    FREE(ppcName[idelete]);
                }
                FREE(ppcName);
                FREE(pstMatData);
                FREE(psize_t);
                Scierror(999, _("%s: No more memory.\n"), "GetCharMatVar");
                return NULL;
            }
        }

        for (int i = 0; i < pDims[0]; ++i)
        {
            for (int j = 0; j < iLen; ++j)
            {
                pstMatData[i + j * pDims[0]] = ppcName[i][j];
            }
        }

        for (int i = 0; i < pDims[0]; ++i)
        {
            FREE(ppcName[i]);
        }

        FREE(ppcName);
    }

    /* Save the variable */
    psize_t[0] = pDims[0];
    psize_t[1] = iLen;

    pMatVarOut = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, Dims, psize_t, pstMatData, 0);

    FREE(pstMatData);
    FREE(psize_t);

    return pMatVarOut;
}

