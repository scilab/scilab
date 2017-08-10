/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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
#include <stdio.h>
#include "splitLine.h"
#include "strsubst.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"

#define EMPTYFIELD "__EMPTY_FIELD_CSV__"
#define DOUBLE_QUOTE '"'

// Add the token (string) to the array of tokens,
// and applies post processing (escape double quotes,...)
static int addToken(char **tokens, int *tokenIdx, const char* tokenValue, int tokenLen)
{
    char *token = (char *) MALLOC((sizeof(char) * tokenLen) + 1);

    if (token)
    {
        char *token2;
        const char *c, *c_end;
        char *c2;

        memcpy(token, tokenValue, tokenLen);
        token[tokenLen] = 0;

        if (strcmp(token, EMPTYFIELD) == 0)
        {
            strcpy(token, "");
        }

        // Escape double quotes, and remove simple quotes
        token2 = (char *) MALLOC((sizeof(char) * tokenLen) + 1);
        c = token;
        c_end = c + tokenLen;
        c2 = token2;
        while (c < c_end)
        {
            if (*c == DOUBLE_QUOTE)
            {
                c++;
                if (*c == DOUBLE_QUOTE)
                {
                    *c2 = DOUBLE_QUOTE;
                    c++;
                    c2++;
                }
            }
            else
            {
                *c2 = *c;
                c++;
                c2++;
            }
        }
        *c2 = 0;

        // Add token
        tokens[*tokenIdx] = token2;
        (*tokenIdx)++;

        FREE(token);

        return TRUE;
    }
    return FALSE;
}

/* ==================================================================== */
char **splitLineCSV(const char *str, const char *sep, int *toks)
{
    char **retstr = NULL;
    const char *idx = NULL;
    const char *end = NULL;
    const char *sep_end = NULL;
    const char *sep_idx = NULL;
    int len = 0;
    int curr_str = 0;
    int inDoubleQuote = 0;

    /* Usually, it should be ,, or ;; */
    char tokenstring_to_search[64] = "";
    /* Previous item will be replaced by ;__EMPTY_FIELD_CSV__; */
    char tokenreplacement_string[64] = "";
    char *substitutedstring = NULL;

    sprintf(tokenstring_to_search, "%s%s", sep, sep);
    sprintf(tokenreplacement_string, "%s%s%s", sep, EMPTYFIELD, sep);
    substitutedstring = strsub(str, tokenstring_to_search, tokenreplacement_string);
    /* in a string like foo;bar;;;, replace all the ;;, not only the first and last one */
    while (strstr(substitutedstring, tokenstring_to_search) != NULL)
    {
        substitutedstring = strsub(substitutedstring, tokenstring_to_search, tokenreplacement_string);
    }

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

    idx = substitutedstring;

    if (strstr(substitutedstring, sep) == NULL)
    {
        *toks = 0;
        FREE(substitutedstring);
        return NULL;
    }

    retstr = (char **) MALLOC((sizeof(char *) * (int)strlen(substitutedstring)));
    if (retstr == NULL)
    {
        *toks = 0;
        FREE(substitutedstring);
        return NULL;
    }

    while (idx < end)
    {
        // If we are in a double quoted field, we do not plit on separators
        if (!inDoubleQuote)
        {
            sep_idx = sep;
            while (sep_idx < sep_end)
            {
                if ((*idx == *sep_idx))
                {
                    if (len > 0)
                    {
                        if (curr_str < (int)strlen(substitutedstring))
                        {
                            // New token (= field)
                            if (addToken(retstr, &curr_str, (char*)(idx - len), len))
                            {
                                // Reset for next field
                                len = 0;
                                idx++;
                            }
                            else
                            {
                                *toks = 0;
                                freeArrayOfString(retstr, (int)strlen(substitutedstring));
                                FREE(substitutedstring);
                                return NULL;
                            }
                        }

                        if (curr_str >= (int)strlen(substitutedstring))
                        {
                            *toks = curr_str + 1;
                            FREE(substitutedstring);
                            return retstr;
                        }
                    }
                    else
                    {
                        idx++;
                        len = 0;
                    }
                }
                else
                {
                    sep_idx++;
                }
            }
        }

        if (*idx == '"')
        {
            // Count number of consecutive double quotes
            int nbDoubleQuotes = 0;
            const char *idxTmp = idx;

            while (*idxTmp == '"')
            {
                idxTmp++;
            }
            nbDoubleQuotes = (int)(idxTmp - idx);

            // if it is odd, we enter or leave a double quoted field
            if (nbDoubleQuotes % 2 == 1)
            {
                inDoubleQuote = (inDoubleQuote == 0) ? 1 : 0;
            }
            len += nbDoubleQuotes;
            idx += nbDoubleQuotes;
        }
        else
        {
            len++;
            idx++;
        }
    }

    if (len > 0)
    {
        // New token (= field)
        if (!addToken(retstr, &curr_str, (char*)(idx - len), len))
        {
            *toks = 0;
            freeArrayOfString(retstr, (int)strlen(substitutedstring));
            FREE(substitutedstring);
            return NULL;
        }
    }

    *toks = curr_str;

    FREE(substitutedstring);

    return retstr;
}
/* ==================================================================== */
