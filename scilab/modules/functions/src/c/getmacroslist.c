/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include <stdlib.h>
#include "api_scilab.h"
#include "sci_types.h"
#include "getmacroslist.h"
#include "librarieslist.h"
#include "libraryinfo.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getvariablesname.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static char **getmacrosonstacklist(int *sizearray);
static char **getmacrosinlibrarieslist(int *sizearray);
static int getsizemacrosinlibrarieslist(void);
static void RemoveDuplicateStrings(char **Strings, int *SizeStrings);
static int cmpstr( const void *a , const void *b);
/*--------------------------------------------------------------------------*/
char **getmacroslist(int *sizearray)
{
    char **macroslist = NULL;

    char **macrosonstack = NULL;
    int sizemacrosonstack = 0;
    char **macrosinlibraries = NULL;
    int sizemacrosinlibraries = 0;

    *sizearray = 0;

    macrosonstack = getmacrosonstacklist(&sizemacrosonstack);
    macrosinlibraries = getmacrosinlibrarieslist(&sizemacrosinlibraries);

    if (macrosinlibraries || macrosonstack)
    {
        int nbMacros = sizemacrosonstack + sizemacrosinlibraries;
        if (sizemacrosonstack)
        {
            macroslist = (char**)REALLOC(macrosinlibraries, sizeof(char*) * nbMacros);
            if (macroslist)
            {
                int i = 0;
                int j = 0;
                for (i = sizemacrosinlibraries; i < nbMacros; i++)
                {
                    macroslist[i] = strdup(macrosonstack[j++]);
                }

                qsort(macroslist, nbMacros, sizeof(char*), cmpstr);
                RemoveDuplicateStrings(macroslist, &nbMacros);
                macroslist = (char**)REALLOC(macroslist, sizeof(char*) * nbMacros);
                *sizearray = nbMacros;
            }
            else
            {
                macroslist = macrosinlibraries;
                *sizearray = sizemacrosinlibraries;
            }
        }
        else
        {
            macroslist = macrosinlibraries;
            *sizearray = sizemacrosinlibraries;
        }
    }

    if (macrosonstack)
    {
        freeArrayOfString(macrosonstack, sizemacrosonstack);
        macrosonstack = NULL;
    }
    return macroslist;
}
/*--------------------------------------------------------------------------*/
static char **getmacrosonstacklist(int *sizearray)
{
    char **macroslist = NULL;

    char **localvariables = NULL;
    int sizelocalvariables = 0;

    *sizearray = 0;
    localvariables = getLocalVariablesName(&sizelocalvariables, FALSE);
    if (localvariables)
    {
        int i = 0;
        int nbMacros = 0;

        for (i = 0; i < sizelocalvariables; i++)
        {
            int iType = 0;
            SciErr sciErr = getNamedVarType(pvApiCtx, localvariables[i], &iType);
            if (!sciErr.iErr)
            {
                if (iType == sci_c_function)
                {
                    nbMacros++;
                }
            }
        }

        if (nbMacros)
        {
            macroslist = (char**)MALLOC(sizeof(char*) * nbMacros);
        }

        if (macroslist)
        {
            nbMacros = 0;
            for (i = 0; i < sizelocalvariables; i++)
            {
                int iType = 0;
                SciErr sciErr = getNamedVarType(pvApiCtx, localvariables[i], &iType);
                if (!sciErr.iErr)
                {
                    if (iType == sci_c_function)
                    {
                        macroslist[nbMacros++] = strdup(localvariables[i]);
                    }
                }
            }
            *sizearray = nbMacros;
        }

        freeArrayOfString(localvariables, sizelocalvariables);
        localvariables = NULL;
    }
    return macroslist;
}
/*--------------------------------------------------------------------------*/
static char **getmacrosinlibrarieslist(int *sizearray)
{
    char **dictionary = NULL;
    int sizedictionary = getsizemacrosinlibrarieslist();

    *sizearray = 0;

    if (sizedictionary > 0)
    {
        int sizelibraries = 0;
        char **libraries = getlibrarieslist(&sizelibraries);

        dictionary = (char**)MALLOC(sizeof(char*) * sizedictionary);

        if (dictionary)
        {
            int m = 0;
            if (libraries)
            {
                int i = 0;
                for (i = 0; i < sizelibraries; i++)
                {
                    int j = 0;
                    char **macros = NULL;
                    int sizemacros = 0;

                    macros = getlistmacrosfromlibrary(libraries[i], &sizemacros);

                    if (macros)
                    {
                        for (j = 0; j < sizemacros; j++)
                        {
                            dictionary[m] = macros[j];
                            m++;
                        }
                        FREE(macros);
                        macros = NULL;
                    }
                }
                *sizearray = sizedictionary;
            }
        }
        else
        {
            *sizearray = 0;
        }
        freeArrayOfString(libraries, sizelibraries);
    }
    return dictionary;
}
/*--------------------------------------------------------------------------*/
static int getsizemacrosinlibrarieslist(void)
{
    int sizedictionary = 0;
    int sizelibraries = 0;
    char **libraries = getlibrarieslist(&sizelibraries);

    if (libraries)
    {
        int i = 0;
        for (i = 0; i < sizelibraries; i++)
        {
            if (libraries[i])
            {
                char **macros = NULL;
                int sizemacros = 0;

                macros = getlistmacrosfromlibrary(libraries[i], &sizemacros);
                if (macros)
                {
                    sizedictionary = sizedictionary + sizemacros;

                    freeArrayOfString(macros, sizemacros);
                }

                FREE(libraries[i]);
                libraries[i] = NULL;
            }

        }
        FREE(libraries);
        libraries = NULL;
    }
    return sizedictionary;
}
/*--------------------------------------------------------------------------*/
static void RemoveDuplicateStrings(char **Strings, int *SizeStrings)
{
    int fin = 0, i = 0;
    int newsize = *SizeStrings;

    for (fin = *SizeStrings - 1; fin > 0; fin--)
    {
        int Sorted = FALSE;

        for (i = 0; i < fin; i++)
        {
            if (Strings[i])
            {
                if (strcmp(Strings[i], Strings[i + 1]) == 0)
                {
                    FREE(Strings[i + 1]);
                    Strings[i + 1] = NULL;
                    Sorted = TRUE;
                    newsize--;
                }
            }
            else
            {
                Strings[i] = Strings[i + 1];
                Strings[i + 1] = NULL;
                Sorted = TRUE;
            }
        }

        if (!Sorted)
        {
            break;
        }
    }
    *SizeStrings = newsize;
}
/*--------------------------------------------------------------------------*/
static int cmpstr( const void *a , const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b );
}
/*--------------------------------------------------------------------------*/
