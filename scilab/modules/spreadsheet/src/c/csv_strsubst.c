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
#ifndef _MSC_VER
#include <stddef.h>
#endif
#include "csv_strsubst.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
// =============================================================================
char *csv_strsubst(const char *input_string, const char *string_to_search, const char *replacement_string)
{
    char *result = NULL, *r = NULL;
    const char *p = NULL, *q = NULL;

    size_t string_to_searchlen = 0;
    size_t count = 0, resultlen = 0, replacement_stringlen = 0;

    if (input_string == NULL)
    {
        return NULL;
    }
    if (string_to_search == NULL)
    {
        return strdup(input_string);
    }
    if (replacement_string == NULL)
    {
        return strdup(input_string);
    }

    string_to_searchlen = strlen(string_to_search);
    replacement_stringlen = strlen(replacement_string);

    if (string_to_searchlen != replacement_stringlen)
    {
        for (count = 0, p = input_string; (q = strstr(p, string_to_search)) != NULL; p = q + string_to_searchlen)
        {
            count++;
        }
        resultlen = p - input_string + strlen(p) + count * (replacement_stringlen - string_to_searchlen);
    }
    else
    {
        resultlen = strlen(input_string);
    }

    if (resultlen + 1 > 0)
    {
        result = (char*)CALLOC(resultlen + 1, sizeof(char));
    }
    if (result)
    {
        for (r = result, p = input_string; (q = strstr(p, string_to_search)) != NULL; p = q + string_to_searchlen)
        {

            ptrdiff_t l = q - p;
            memcpy(r, p, l);
            r += l;
            memcpy(r, replacement_string, replacement_stringlen);
            r += replacement_stringlen;
        }
        strcpy(r, p);
    }

    return result;
}
// =============================================================================
