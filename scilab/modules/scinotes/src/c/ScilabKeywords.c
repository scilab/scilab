/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "getvariablesname.h"
#include "commandwords.h"
#include "getfunctionslist.h"
#include "getmacroslist.h"
#include "BOOL.h"
#include "ScilabKeywords.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
char **GetVariablesName(void)
{
        int returnedArraySize = 0;
        char **VarsName = getVariablesName(&returnedArraySize, TRUE);
        /* SWIG char array must be finished by NULL */
        if (VarsName)
        {
                VarsName = (char**)REALLOC(VarsName,sizeof(char*)*(returnedArraySize+1));
                VarsName[returnedArraySize] = NULL;
        }
        return VarsName;
}
/*--------------------------------------------------------------------------*/
char **GetCommandsName(void)
{
        int returnedArraySize = 0;
        char **CommandKeywords = getcommandkeywords(&returnedArraySize);
        if (CommandKeywords)
        {
                CommandKeywords = (char**)REALLOC(CommandKeywords, sizeof(char*)*(returnedArraySize+1));
                CommandKeywords[returnedArraySize] = NULL;
        }
        return CommandKeywords;
}
/*--------------------------------------------------------------------------*/
char **GetFunctionsName(void)
{
        int returnedArraySize = 0;
        char **FunctionsName = GetFunctionsList(&returnedArraySize);
        if (FunctionsName)
        {
                FunctionsName = (char**)REALLOC(FunctionsName, sizeof(char*)*(returnedArraySize+1));
                FunctionsName[returnedArraySize] = NULL;
        }
        return FunctionsName;
}
/*--------------------------------------------------------------------------*/
char **GetMacrosName(void)
{
        int returnedArraySize = 0;
        char **MacrosName = getmacroslist(&returnedArraySize);
        if (MacrosName)
        {
                MacrosName = (char**)REALLOC(MacrosName, sizeof(char*)*(returnedArraySize+1));
                MacrosName[returnedArraySize] = NULL;
        }
        return MacrosName;
}
/*--------------------------------------------------------------------------*/
char **GetFieldsName(char *var)
{
        SciErr sciErr;
        int *piAddr = NULL;
        int *piLen = NULL;
        int piType = 0;
        int nbItem = 0;
        int rows = 0;
        int cols = 0;
        int rc = 0;
        int i;
        char **pstData = NULL;

        sciErr = getNamedVarType(pvApiCtx, var, &piType);
        if (sciErr.iErr && piType != sci_mlist && piType != sci_tlist)
        {
                return NULL;
        }

        getVarAddressFromName(pvApiCtx, var, &piAddr);
        if (sciErr.iErr)
        {
                return NULL;
        }

        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &rows, &cols, NULL, NULL);
        if (sciErr.iErr)
        {
                return NULL;
        }

        rc = rows * cols;
        piLen = (int*)MALLOC(sizeof(int) * rc);
        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &rows, &cols, piLen, NULL);
        if (sciErr.iErr)
        {
                FREE(piLen);
                return NULL;
        }

        pstData = (char**)MALLOC(sizeof(char*) * (rc + 1));
        for (i = 0 ; i < rc ; i++)
        {
                pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));
        }

        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &rows, &cols, piLen, pstData);
        if (sciErr.iErr)
        {
                FREE(piLen);
                FREE(pstData);
                return NULL;
        }

        pstData[rc] = NULL;
        FREE(piLen);

        return pstData;
}
/*--------------------------------------------------------------------------*/
