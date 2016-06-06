/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <string.h>
#include <stdlib.h>
#include "completion.h"
#include "sci_malloc.h"
#include "getvariablesname.h"
#include "commandwords.h"
#include "getfunctionsname.h"
#include "getmacrosname.h"
#include "completion_generic.h"
#include "getfilesdictionary.h"
#include "getfieldsdictionary.h"
#include "getDictionarySetProperties.h"
#include "getDictionaryGetProperties.h"
#include "toolsdictionary.h"
#include "os_string.h"
#include "stdio.h"
/*--------------------------------------------------------------------------*/
char **completionOnDictionary(char **dictionary, int sizedictionary, const char *somechars, int *sizearrayreturned);
/*--------------------------------------------------------------------------*/
char **completion(const char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;

    char **dictionary = NULL;
    int sizedictionary = 0;

    int sizecompletionfunctions = 0;
    char **completionfunctions = completionOnFunctions(somechars, &sizecompletionfunctions);

    int sizecompletioncommandwords = 0;
    char **completioncommandwords = completionOnCommandWords(somechars, &sizecompletioncommandwords);

    int sizecompletionmacros = 0;
    char **completionmacros =  completionOnMacros(somechars, &sizecompletionmacros);

    int sizecompletionvariables = 0;
    char **completionvariables = completionOnVariables(somechars, &sizecompletionvariables);

    int sizecompletionhandlegraphicsproperties = 0;
    char **completionhandlegraphicsproperties = completionOnHandleGraphicsProperties(somechars, &sizecompletionhandlegraphicsproperties);

    int sizecompletionfiles = 0;
    char **completionfiles = completionOnFiles(somechars, &sizecompletionfiles);

    *sizeArrayReturned = 0;

    sizedictionary = sizecompletionfunctions + sizecompletioncommandwords + sizecompletionmacros
                     + sizecompletionvariables + sizecompletionhandlegraphicsproperties + sizecompletionfiles;


    if ( (completionfiles) && (sizedictionary == sizecompletionfiles) )
    {
        ListWords = completionfiles;
        *sizeArrayReturned = sizecompletionfiles;
    }
    else
    {
        if (sizedictionary > 0)
        {
            dictionary = (char**)MALLOC(sizeof(char*) * sizedictionary);
        }

        if (dictionary)
        {
            int i = 0;
            appendDictionary(&dictionary, &i, &completionfunctions, &sizecompletionfunctions);
            appendDictionary(&dictionary, &i, &completioncommandwords, &sizecompletioncommandwords);
            appendDictionary(&dictionary, &i, &completionmacros, &sizecompletionmacros);
            appendDictionary(&dictionary, &i, &completionvariables, &sizecompletionvariables);
            appendDictionary(&dictionary, &i, &completionhandlegraphicsproperties, &sizecompletionhandlegraphicsproperties);
            appendDictionary(&dictionary, &i, &completionfiles, &sizecompletionfiles);

            dictionary = SortDictionary(dictionary, i);
            dictionary = RemoveDuplicateDictionary(dictionary, &i);

            sizedictionary = i;
        }
        ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
        freePointerDictionary(dictionary, sizedictionary);
    }
    freePointerDictionary(completionvariables, sizecompletionvariables);
    freePointerDictionary(completionmacros, sizecompletionmacros);
    freePointerDictionary(completionhandlegraphicsproperties, sizecompletionhandlegraphicsproperties);
    freePointerDictionary(completionfunctions, sizecompletionfunctions);

    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnFunctions(const char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getFunctionsName(&sizedictionary);

    if (dictionary)
    {
        ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
        freePointerDictionary(dictionary, sizedictionary);
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnCommandWords(const char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getcommandkeywords(&sizedictionary);

    if (dictionary)
    {
        dictionary = SortDictionary(dictionary, sizedictionary);
        ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
        freePointerDictionary(dictionary, sizedictionary);
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnMacros(const char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getMacrosName(&sizedictionary);

    if (dictionary)
    {
        ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
        freePointerDictionary(dictionary, sizedictionary);
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnVariables(const char *somechars, int *sizeArrayReturned)
{
    int i = 0;
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;
    dictionary = getVariablesName(&sizedictionary, TRUE);
    ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
    freePointerDictionary(dictionary, sizedictionary);

    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnVariablesWithoutMacros(const char *somechars, int *sizeArrayReturned)
{
    int i = 0;
    int j = 0;
    int nbWordsAlreadyInMacros = 0;

    char **ListWords = NULL;
    int sizeListWords = 0;

    char **dictionaryVariables = NULL;
    int sizedictionaryVariables = 0;

    dictionaryVariables = completionOnVariables(somechars, &sizedictionaryVariables);

    if (sizedictionaryVariables)
    {
        char **dictionaryMacros = NULL;
        int sizedictionaryMacros = 0;

        dictionaryMacros = getMacrosName(&sizedictionaryMacros);

        /* Search if we have more than one definition */
        for ( i = 0; i < sizedictionaryVariables; i++)
        {
            for ( j = 0; j < sizedictionaryMacros; j++)
            {
                if ( strcmp(dictionaryVariables[i], dictionaryMacros[j]) == 0 )
                {
                    nbWordsAlreadyInMacros++;
                }
            }
        }

        if (nbWordsAlreadyInMacros)
        {
            sizeListWords = sizedictionaryVariables - nbWordsAlreadyInMacros;
            if (sizeListWords > 0)
            {
                char **ListWordsTmp = (char**)MALLOC(sizeof(char*) * sizedictionaryVariables);
                if (ListWordsTmp)
                {
                    int k = 0;

                    /* do a copy of dictionary of Variables */
                    for ( i = 0; i < sizedictionaryVariables; i++)
                    {
                        ListWordsTmp[i] = os_strdup(dictionaryVariables[i]);
                    }

                    for ( i = 0; i < sizedictionaryVariables; i++)
                    {
                        for ( j = 0; j < sizedictionaryMacros; j++)
                        {
                            if ( strcmp(dictionaryVariables[i], dictionaryMacros[j]) == 0 )
                            {
                                FREE(ListWordsTmp[i]);
                                ListWordsTmp[i] = NULL;
                            }
                        }
                    }

                    ListWords = (char**)MALLOC(sizeof(char*) * (sizeListWords + 1));
                    if (ListWords)
                    {
                        for ( i = 0; i < sizedictionaryVariables; i++)
                        {
                            if (ListWordsTmp[i])
                            {
                                ListWords[k] = os_strdup(ListWordsTmp[i]);
                                if (k <= sizeListWords)
                                {
                                    k++;
                                }
                            }
                        }
                        /* Add a NULL element at the end (to get number of items from JNI) */
                        ListWords[sizeListWords] = NULL;

                        *sizeArrayReturned = sizeListWords;
                    }
                    else
                    {
                        ListWords = NULL;
                        *sizeArrayReturned = 0;
                    }
                }
                else
                {
                    ListWords = NULL;
                    *sizeArrayReturned = 0;
                }
            }
            else
            {
                ListWords = NULL;
                *sizeArrayReturned = 0;
            }
        }
        else
        {
            ListWords = (char**)MALLOC(sizeof(char*) * (sizedictionaryVariables + 1));
            for ( i = 0; i < sizedictionaryVariables; i++ )
            {
                ListWords[i] = os_strdup(dictionaryVariables[i]);
            }
            ListWords[sizedictionaryVariables] = 0x0;
            *sizeArrayReturned = sizedictionaryVariables;
        }

        freePointerDictionary(dictionaryMacros, sizedictionaryMacros);
    }
    else
    {
        ListWords = NULL;
        *sizeArrayReturned = 0;
    }
    freePointerDictionary(dictionaryVariables, sizedictionaryVariables);

    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnFiles(const char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getfilesdictionary(somechars, &sizedictionary, FALSE);

    if (dictionary)
    {
        ListWords = dictionary;
        *sizeArrayReturned = sizedictionary;
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnFields(char *lineBeforeCaret, char *pattern, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    if (lineBeforeCaret && pattern)
    {
        dictionary = getfieldsdictionary(lineBeforeCaret, pattern, &sizedictionary);
    }

    if (dictionary)
    {
        ListWords = dictionary;
        *sizeArrayReturned = sizedictionary;
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnHandleGraphicsProperties(const char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    int sizeHandleGraphicsGetPropertiesDictionary = 0;
    char **HandleGraphicsGetPropertiesDictionary = getDictionaryGetProperties(&sizeHandleGraphicsGetPropertiesDictionary);

    int sizeHandleGraphicsSetPropertiesDictionary = 0;
    char **HandleGraphicsSetPropertiesDictionary = getDictionarySetProperties(&sizeHandleGraphicsSetPropertiesDictionary);

    *sizeArrayReturned = 0;

    sizedictionary = sizeHandleGraphicsGetPropertiesDictionary + sizeHandleGraphicsSetPropertiesDictionary;

    if (sizedictionary > 0)
    {
        dictionary = (char**)MALLOC(sizeof(char*) * sizedictionary);

        if (dictionary)
        {
            int i = 0;

            appendDictionary(&dictionary, &i, &HandleGraphicsGetPropertiesDictionary, &sizeHandleGraphicsGetPropertiesDictionary);
            appendDictionary(&dictionary, &i, &HandleGraphicsSetPropertiesDictionary, &sizeHandleGraphicsSetPropertiesDictionary);
            sizedictionary = i;
        }

        if (dictionary)
        {
            dictionary = SortDictionary(dictionary, sizedictionary);
            dictionary = RemoveDuplicateDictionary(dictionary, &sizedictionary);
            ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
            freePointerDictionary(dictionary, sizedictionary);
        }
        else
        {
            *sizeArrayReturned = 0;
        }
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    freePointerDictionary(HandleGraphicsSetPropertiesDictionary, sizeHandleGraphicsSetPropertiesDictionary);
    freePointerDictionary(HandleGraphicsGetPropertiesDictionary, sizeHandleGraphicsGetPropertiesDictionary);
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnDictionary(char **dictionary, int sizedictionary, const char *somechars, int *sizearrayreturned)
{
    char **ListWords = NULL;

    if (dictionary)
    {
        ListWords = completion_generic(dictionary, sizedictionary, somechars, sizearrayreturned);
        if (ListWords == NULL)
        {
            *sizearrayreturned = 0;
        }
    }
    else
    {
        *sizearrayreturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
