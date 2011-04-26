/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2011 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <string.h>
#include <stdlib.h>
#include "completion.h"
#include "completionDictionary.h"
#include "MALLOC.h"
#include "completion_generic.h"
#include "getfilesdictionary.h"
#include "getfieldsdictionary.h"
#include "toolsdictionary.h"
#include "os_strdup.h"
/*--------------------------------------------------------------------------*/
char **completionOnDictionary(char **dictionary,int sizedictionary,char *somechars,int *sizearrayreturned);
/*--------------------------------------------------------------------------*/
char **completion(char *somechars, int *sizeArrayReturned)
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
        if (sizedictionary > 0) dictionary = (char**)MALLOC(sizeof(char*)*sizedictionary);

        if (dictionary)
        {
            int i = 0;
            appendDictionary(&dictionary,&i,&completionfunctions,&sizecompletionfunctions);
            appendDictionary(&dictionary,&i,&completioncommandwords,&sizecompletioncommandwords);
            appendDictionary(&dictionary,&i,&completionmacros,&sizecompletionmacros);
            appendDictionary(&dictionary,&i,&completionvariables,&sizecompletionvariables);
            appendDictionary(&dictionary,&i,&completionhandlegraphicsproperties,&sizecompletionhandlegraphicsproperties);
            appendDictionary(&dictionary,&i,&completionfiles,&sizecompletionfiles);

            dictionary = SortDictionary(dictionary,i);	
            dictionary = RemoveDuplicateDictionary(dictionary,&i);	

            sizedictionary = i;
        }
        ListWords = completionOnDictionary(dictionary,sizedictionary,somechars,sizeArrayReturned);
        freePointerDictionary(dictionary,sizedictionary);
    }

    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnFunctions(char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getFunctionsDictionary(&sizedictionary);

    if (dictionary)
    {
        ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
        freePointerDictionary(dictionary,sizedictionary);
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnCommandWords(char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getKeywordsDictionary(&sizedictionary);

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
char **completionOnMacros(char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getMacrosAllDictionary(&sizedictionary);

    if (dictionary)
    {
        ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
        freePointerDictionary(dictionary,sizedictionary);
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnVariables(char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getVariablesDictionary(&sizedictionary, FALSE);

    ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
    freePointerDictionary(dictionary, sizedictionary);

    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnVariablesWithoutMacros(char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getVariablesDictionary(&sizedictionary, TRUE);

    ListWords = completionOnDictionary(dictionary, sizedictionary, somechars, sizeArrayReturned);
    freePointerDictionary(dictionary, sizedictionary);

    return ListWords;
}
/*--------------------------------------------------------------------------*/
char **completionOnFiles(char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getfilesdictionary(somechars,&sizedictionary,FALSE);

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

    dictionary = getfieldsdictionary(lineBeforeCaret, pattern, &sizedictionary);

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
char **completionOnHandleGraphicsProperties(char *somechars, int *sizeArrayReturned)
{
    char **ListWords = NULL;
    char **dictionary = NULL;
    int sizedictionary = 0;

    dictionary = getGraphicPropertiesDictionary(&sizedictionary);
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
char **completionOnDictionary(char **dictionary,int sizedictionary,char *somechars,int *sizearrayreturned)
{
    char **ListWords = NULL;

    if (dictionary)
    {
        ListWords = completion_generic(dictionary,sizedictionary,somechars,sizearrayreturned);
        if (ListWords == NULL) *sizearrayreturned = 0;
    }
    else
    {
        *sizearrayreturned = 0;
    }
    return ListWords;
}
/*--------------------------------------------------------------------------*/
