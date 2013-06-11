/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*---------------------------------------------------------------------------*/
#include "symbol.hxx"
#include "internal.hxx"
#include "context.hxx"
extern "C"
{
#include "completionDictionary.h"
#include "charEncoding.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "os_strdup.h"
#include "commandwords.h" // to get old keywords
#include "getDictionarySetProperties.h" // to get old set properties
#include "getDictionaryGetProperties.h" // to get old get properties
#include "toolsdictionary.h"
}
/*---------------------------------------------------------------------------*/
char **getAllDictionary(int *sizeArrayReturned)
{
    char **pStrAllSymbols = NULL;
    *sizeArrayReturned = (int)symbol::Symbol::map_size();
    wchar_t** allSymbols = symbol::Symbol::get_all();

    pStrAllSymbols = static_cast<char**>(MALLOC(sizeof(char*) * (*sizeArrayReturned)));
    if (pStrAllSymbols)
    {
        for (int i = 0 ; i <  ( *sizeArrayReturned) ; i++)
        {
            pStrAllSymbols[i] = wide_string_to_UTF8(allSymbols[i]);
        }
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return pStrAllSymbols;
}
/*---------------------------------------------------------------------------*/
char **getFunctionsDictionary(int *sizeArrayReturned)
{
    char **pStrFunctions = NULL;
    *sizeArrayReturned = 0;
    int nbAllSymbols = 0;
    char **pStrAllSymbols = getAllDictionary(&nbAllSymbols);
    if (pStrAllSymbols)
    {
        int nbFunctions = 0;
        for (int i = 0; i < nbAllSymbols; i++)
        {
            wchar_t *pwStrSymbol = to_wide_string(pStrAllSymbols[i]);
            if (pwStrSymbol)
            {
                types::InternalType *pitReturn = symbol::Context::getInstance()->get(symbol::Symbol(pwStrSymbol));
                if (pitReturn)
                {
                    if (pitReturn->isFunction())
                    {
                        nbFunctions++;
                        if (pStrFunctions == NULL)
                        {
                            pStrFunctions = (char**) MALLOC(sizeof(char*) * nbFunctions);
                        }
                        else
                        {
                            pStrFunctions = (char**) REALLOC(pStrFunctions, sizeof(char*) * nbFunctions);
                        }
                        pStrFunctions[nbFunctions - 1] = os_strdup(pStrAllSymbols[i]);
                    }
                }
                FREE(pwStrSymbol);
                pwStrSymbol = NULL;
            }
        }
        *sizeArrayReturned  = nbFunctions;
        freeArrayOfString(pStrAllSymbols, nbAllSymbols);
        pStrAllSymbols = NULL;
    }
    return pStrFunctions;
}
/*---------------------------------------------------------------------------*/
char **getVariablesDictionary(int *sizeArrayReturned, int bWithoutMacros)
{
    char **pStrVariables = NULL;
    *sizeArrayReturned = 0;
    int nbAllSymbols = 0;
    char **pStrAllSymbols = getAllDictionary(&nbAllSymbols);
    if (pStrAllSymbols)
    {
        int nbVariables = 0;
        for (int i = 0; i < nbAllSymbols; i++)
        {
            wchar_t *pwStrSymbol = to_wide_string(pStrAllSymbols[i]);
            if (pwStrSymbol)
            {
                types::InternalType *pitReturn = symbol::Context::getInstance()->get(symbol::Symbol(pwStrSymbol));
                if (pitReturn)
                {
                    int bCheck = 0;

                    if (bWithoutMacros)
                    {
                        bCheck = (!pitReturn->isFunction()) && (!pitReturn->isMacro()) && (!pitReturn->isMacroFile());
                    }
                    else
                    {
                        bCheck = (!pitReturn->isFunction()) && (!pitReturn->isMacroFile());
                    }

                    if (bCheck)
                    {
                        nbVariables++;
                        if (pStrVariables == NULL)
                        {
                            pStrVariables = (char**) MALLOC(sizeof(char*) * nbVariables);
                        }
                        else
                        {
                            pStrVariables = (char**) REALLOC(pStrVariables, sizeof(char*) * nbVariables);
                        }
                        pStrVariables[nbVariables - 1] = os_strdup(pStrAllSymbols[i]);
                    }
                }
                FREE(pwStrSymbol);
                pwStrSymbol = NULL;
            }
        }
        *sizeArrayReturned = nbVariables;

        freeArrayOfString(pStrAllSymbols, nbAllSymbols);
        pStrAllSymbols = NULL;
    }
    return pStrVariables;
}
/*---------------------------------------------------------------------------*/
char **getMacrosFilesDictionary(int *sizeArrayReturned)
{
    char **pStrMacrosFiles = NULL;
    *sizeArrayReturned = 0;
    int nbAllSymbols = 0;
    char **pStrAllSymbols = getAllDictionary(&nbAllSymbols);
    if (pStrAllSymbols)
    {
        int nbMacrosFiles = 0;
        for (int i = 0; i < nbAllSymbols; i++)
        {
            wchar_t *pwStrSymbol = to_wide_string(pStrAllSymbols[i]);
            if (pwStrSymbol)
            {
                types::InternalType *pitReturn = symbol::Context::getInstance()->get(symbol::Symbol(pwStrSymbol));
                if (pitReturn)
                {
                    if (pitReturn->isMacroFile())
                    {
                        nbMacrosFiles++;
                        if (pStrMacrosFiles == NULL)
                        {
                            pStrMacrosFiles = (char**) MALLOC(sizeof(char*) * nbMacrosFiles);
                        }
                        else
                        {
                            pStrMacrosFiles = (char**) REALLOC(pStrMacrosFiles, sizeof(char*) * nbMacrosFiles);
                        }
                        pStrMacrosFiles[nbMacrosFiles - 1] = os_strdup(pStrAllSymbols[i]);
                    }
                }
            }
            FREE(pwStrSymbol);
            pwStrSymbol = NULL;
        }
        *sizeArrayReturned = nbMacrosFiles;

        freeArrayOfString(pStrAllSymbols, nbAllSymbols);
        pStrAllSymbols = NULL;
    }
    return pStrMacrosFiles;
}
/*---------------------------------------------------------------------------*/
char **getMacrosVariablesDictionary(int *sizeArrayReturned)
{
    char **pStrMacrosVariables = NULL;
    *sizeArrayReturned = 0;
    int nbAllSymbols = 0;
    char **pStrAllSymbols = getAllDictionary(&nbAllSymbols);
    if (pStrAllSymbols)
    {
        int nbMacrosVariables = 0;
        for (int i = 0; i < nbAllSymbols; i++)
        {
            wchar_t *pwStrSymbol = to_wide_string(pStrAllSymbols[i]);
            if (pwStrSymbol)
            {
                types::InternalType *pitReturn = symbol::Context::getInstance()->get(symbol::Symbol(pwStrSymbol));
                if (pitReturn)
                {
                    if (pitReturn->isMacro())
                    {
                        nbMacrosVariables++;
                        if (pStrMacrosVariables == NULL)
                        {
                            pStrMacrosVariables = (char**) MALLOC(sizeof(char*) * nbMacrosVariables);
                        }
                        else
                        {
                            pStrMacrosVariables = (char**) REALLOC(pStrMacrosVariables, sizeof(char*) * nbMacrosVariables);
                        }
                        pStrMacrosVariables[nbMacrosVariables - 1] = os_strdup(pStrAllSymbols[i]);
                    }
                }
            }
            FREE(pwStrSymbol);
            pwStrSymbol = NULL;
        }
        *sizeArrayReturned = nbMacrosVariables;

        freeArrayOfString(pStrAllSymbols, nbAllSymbols);
        pStrAllSymbols = NULL;
    }
    return pStrMacrosVariables;
}
/*---------------------------------------------------------------------------*/
char **getMacrosAllDictionary(int *sizeArrayReturned)
{
    char **pStrMacrosAll = NULL;
    *sizeArrayReturned = 0;
    int nbAllSymbols = 0;
    char **pStrAllSymbols = getAllDictionary(&nbAllSymbols);
    if (pStrAllSymbols)
    {
        int nbMacrosAll = 0;
        for (int i = 0; i < nbAllSymbols; i++)
        {
            wchar_t *pwStrSymbol = to_wide_string(pStrAllSymbols[i]);
            if (pwStrSymbol)
            {
                types::InternalType *pitReturn = symbol::Context::getInstance()->get(symbol::Symbol(pwStrSymbol));
                if (pitReturn)
                {
                    if (pitReturn->isMacro() || pitReturn->isMacroFile())
                    {
                        nbMacrosAll++;
                        if (pStrMacrosAll == NULL)
                        {
                            pStrMacrosAll = (char**) MALLOC(sizeof(char*) * nbMacrosAll);
                        }
                        else
                        {
                            pStrMacrosAll = (char**) REALLOC(pStrMacrosAll, sizeof(char*) * nbMacrosAll);
                        }
                        pStrMacrosAll[nbMacrosAll - 1] = os_strdup(pStrAllSymbols[i]);
                    }
                }
            }
            FREE(pwStrSymbol);
            pwStrSymbol = NULL;
        }
        *sizeArrayReturned = nbMacrosAll;

        freeArrayOfString(pStrAllSymbols, nbAllSymbols);
        pStrAllSymbols = NULL;
    }
    return pStrMacrosAll;
}
/*---------------------------------------------------------------------------*/
char **getGraphicPropertiesDictionary(int *sizeArrayReturned)
{
    // not yet plugged on YaSp
    // it uses old table of properties
    char **pStrGraphicHandles = NULL;
    int nbGraphicProperties = 0;
    *sizeArrayReturned = 0;

    int sizeHandleGraphicsGetPropertiesDictionary = 0;
    char **HandleGraphicsGetPropertiesDictionary = getDictionaryGetProperties(&sizeHandleGraphicsGetPropertiesDictionary);

    int sizeHandleGraphicsSetPropertiesDictionary = 0;
    char **HandleGraphicsSetPropertiesDictionary = getDictionarySetProperties(&sizeHandleGraphicsSetPropertiesDictionary);

    nbGraphicProperties = sizeHandleGraphicsGetPropertiesDictionary + sizeHandleGraphicsSetPropertiesDictionary;

    if (nbGraphicProperties > 0)
    {
        pStrGraphicHandles = (char**)MALLOC(sizeof(char*) * nbGraphicProperties);

        if (pStrGraphicHandles)
        {
            int i = 0;
            appendDictionary(&pStrGraphicHandles, &i, &HandleGraphicsGetPropertiesDictionary, &sizeHandleGraphicsGetPropertiesDictionary);
            appendDictionary(&pStrGraphicHandles, &i, &HandleGraphicsSetPropertiesDictionary, &sizeHandleGraphicsSetPropertiesDictionary);
            nbGraphicProperties = i;
        }
        *sizeArrayReturned = nbGraphicProperties;
    }
    return pStrGraphicHandles;
}
/*---------------------------------------------------------------------------*/
char **getKeywordsDictionary(int *sizeArrayReturned)
{
    // no yet plugged on YaSp
    // it uses old table of keywords
    char **pStrKeyWords = getcommandkeywords(sizeArrayReturned);
    if (pStrKeyWords)
    {
        pStrKeyWords = SortDictionary(pStrKeyWords, *sizeArrayReturned);
        pStrKeyWords = RemoveDuplicateDictionary(pStrKeyWords, sizeArrayReturned);
    }
    return pStrKeyWords;
}
/*---------------------------------------------------------------------------*/
