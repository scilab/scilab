/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include <stdio.h>
#include "splitLine.h"
#include "csv_strsubst.h"
#include "MALLOC.h"
/* ==================================================================== */
char **splitLineCSV(const char *str, const char *sep, int *toks, char meta)
{
#define EMPTYFIELD "__EMPTY_FIELD_CSV__"
    char **retstr = NULL;
    const char *idx = NULL;
    const char *end = NULL;
    const char *sep_end = NULL;
    const char *sep_idx = NULL;
    int len = 0;
    int curr_str = 0;
    char last_char = 0xFF;

    char tokenstring_to_search[64] = ",,";
    char tokenreplacement_string[64] = ",,";
    char *substitutedstring = NULL;

    sprintf(tokenstring_to_search, "%s%s", sep, sep);
    sprintf(tokenreplacement_string, "%s%s%s", sep, EMPTYFIELD, sep);
    substitutedstring = csv_strsubst(str, tokenstring_to_search, tokenreplacement_string);
    if (strncmp(substitutedstring, sep, strlen(sep)) == 0)
    {
        char *tmp = NULL;
        size_t l = strlen(substitutedstring) + strlen(EMPTYFIELD) + strlen(sep) + 1;
        tmp = (char*)MALLOC(sizeof(char) * l);
        sprintf(tmp, "%s%s%s", EMPTYFIELD, sep, &substitutedstring[1]);
        FREE(substitutedstring);
        substitutedstring = tmp;
    }

    if (substitutedstring[strlen(substitutedstring) - 1] == sep[0])
    {
        char *tmp = NULL;
        size_t l = strlen(substitutedstring) + strlen(EMPTYFIELD) + 1;
        tmp = (char*)MALLOC(sizeof(char) * l);
        sprintf(tmp, "%s%s", substitutedstring, EMPTYFIELD);
        FREE(substitutedstring);
        substitutedstring = tmp;

    }

    sep_end = sep + strlen(sep);
    end = substitutedstring + strlen(substitutedstring);

    sep_idx = sep;
    idx = substitutedstring;

    if (strstr(substitutedstring, sep) == NULL)
    {
        *toks = 0;
        FREE(substitutedstring);
        substitutedstring = NULL;
        return NULL;
    }

    retstr = (char **) MALLOC((sizeof(char *) * (int)strlen(substitutedstring)));
    if (retstr == NULL)
    {
        *toks = 0;
        FREE(substitutedstring);
        substitutedstring = NULL;
        return NULL;
    }

    while (idx < end)
    {
        while (sep_idx < sep_end)
        {
            if ((*idx == *sep_idx) && (last_char != meta))
            {
                if (len > 0)
                {
                    if (curr_str < (int)strlen(substitutedstring))
                    {
                        retstr[curr_str] = (char *) MALLOC((sizeof(char) * len) + 1);

                        if (retstr[curr_str] == NULL)
                        {
                            *toks = 0;
                            FREE(substitutedstring);
                            substitutedstring = NULL;
                            return NULL;
                        }
                        memcpy(retstr[curr_str], (idx - len), len);
                        retstr[curr_str][len] = 0;
                        if (strcmp(retstr[curr_str], EMPTYFIELD) == 0)
                        {
                            strcpy(retstr[curr_str], "");
                        }
                        len = 0;
                        curr_str++;
                        last_char = *idx;
                        idx++;
                    }

                    if (curr_str >= (int)strlen(substitutedstring))
                    {
                        *toks = curr_str + 1;
                        FREE(substitutedstring);
                        substitutedstring = NULL;
                        return retstr;
                    }
                }
                else
                {
                    last_char = *idx;
                    idx++;
                    sep_idx = sep;
                    len = 0;
                }
            }
            else
            {
                sep_idx++;
            }
        }

        sep_idx = sep;
        len++;
        last_char = *idx;
        idx++;
    }

    if (len > 0)
    {
        retstr[curr_str] = (char *) MALLOC((sizeof(char) * len) + 1);

        if (retstr[curr_str] == NULL)
        {
            *toks = 0;
            if (substitutedstring)
            {
                FREE(substitutedstring);
                substitutedstring = NULL;
            }
            return NULL;
        }

        memcpy(retstr[curr_str], (idx - len), len);
        retstr[curr_str][len] = 0;
        if (strcmp(retstr[curr_str], EMPTYFIELD) == 0)
        {
            strcpy(retstr[curr_str], "");
        }

        *toks = curr_str + 1;
    }

    if (substitutedstring)
    {
        FREE(substitutedstring);
        substitutedstring = NULL;
    }

    return retstr;
}
/* ==================================================================== */
