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
#include <stdlib.h>
#include <string.h>
#include "completion_wrap_java.h"
#include "completion.h"
/*--------------------------------------------------------------------------*/
char **searchAllDictionaries(char *somechars)
{
    int nbrElements = 0;
    if ( (somechars) && (strcmp(somechars, "")) )
    {
        return completion(somechars, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchFunctionsDictionary(char *somechars)
{
    int nbrElements = 0;
    if ( (somechars) && (strcmp(somechars, "")) )
    {
        return completionOnFunctions(somechars, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchCommandsDictionary(char *somechars)
{
    int nbrElements = 0;
    if ( (somechars) && (strcmp(somechars, "")) )
    {
        return completionOnCommandWords(somechars, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchMacrosDictionary(char *somechars)
{
    int nbrElements = 0;
    if ( (somechars) && (strcmp(somechars, "")) )
    {
        return completionOnMacros(somechars, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchVariablesDictionary(char *somechars)
{
    int nbrElements = 0;
    if ( (somechars) && (strcmp(somechars, "")) )
    {
        return completionOnVariablesWithoutMacros(somechars, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchFilesDictionary(char *somechars)
{
    int nbrElements = 0;
    if ( (somechars) && (strcmp(somechars, "")) )
    {
        return completionOnFiles(somechars, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchFieldsDictionary(char *lineBeforeCaret, char *pattern)
{
    int nbrElements = 0;
    if ( (lineBeforeCaret) && (strcmp(lineBeforeCaret, "")) )
    {
        return completionOnFields(lineBeforeCaret, pattern, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchHandleGraphicsPropertiesDictionary(char *somechars)
{
    int nbrElements = 0;
    if ( (somechars) && (strcmp(somechars, "")) )
    {
        char *pattern = somechars;
        while (*(pattern++) == ' ')
        {
            ;
        }
        return completionOnHandleGraphicsProperties(--pattern, &nbrElements);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
