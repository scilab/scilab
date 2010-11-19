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
