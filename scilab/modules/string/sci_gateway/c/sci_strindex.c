/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : search position of a character string in an other string        */
/*        using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "gw_string.h"
#include "pcre.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "pcre_private.h"
#include "BOOL.h"
#include "pcre_error.h"
#include "Scierror.h"
#include "charEncoding.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*------------------------------------------------------------------------*/
#define CHAR_S "s"
#define CHAR_R "r"
/*------------------------------------------------------------------------*/
struct In
{
    int data;
    int position;
} ;
/*------------------------------------------------------------------------*/
int cmp( const void *a , const void *b)
{
    return (*(struct In *)a).data > (*(struct In *)b).data ? 1 : -1;
}
/*------------------------------------------------------------------------*/
int sci_strindex(char *fname, unsigned long fname_len)
{
    BOOL bStrindex_with_pattern = FALSE;
    int outIndex = 0;
    int numRow = 1;
    int *next = NULL;

    CheckRhs(2, 3);
    CheckLhs(1, 2);

    if (Rhs == 3)
    {
        int m3 = 0;
        int n3 = 0;
        char **Strings_Input3 = NULL;
        int m3n3 = 0; /* m3 * n3 */

        if (VarType(3) != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Character expected.\n"), fname, 3);
            return 0;
        }
        GetRhsVar(3, MATRIX_OF_STRING_DATATYPE, &m3, &n3, &Strings_Input3);
        m3n3 = m3 * n3;

        if (m3n3 != 1)
        {
            freeArrayOfString(Strings_Input3, m3n3);
            Scierror(999, _("%s: Wrong type for input argument #%d: Character expected.\n"), fname, 3);
            return 0;
        }

        if ( (strcmp(Strings_Input3[0], CHAR_R) == 0) || (strcmp(Strings_Input3[0], CHAR_S) == 0) )
        {
            if (strcmp(Strings_Input3[0], CHAR_R) == 0)
            {
                bStrindex_with_pattern = TRUE;
            }
            else
            {
                bStrindex_with_pattern = FALSE;
            }
            freeArrayOfString(Strings_Input3, m3n3);
        }
        else
        {
            freeArrayOfString(Strings_Input3, m3n3);
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 3, CHAR_S, CHAR_R);
            return 0;
        }
    }

    if (VarType(1) == sci_matrix)
    {
        int m1 = 0;
        int n1 = 0;
        int l1 = 0;

        GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
        if ((m1 == 0) && (n1 == 0))
        {
            CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
            LhsVar(1) = Rhs + 1 ;
            PutLhsVar();
            return 0;
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
            return 0;
        }
    }

    if ( (VarType(1) == sci_strings) && (VarType(2) == sci_strings) )
    {
        int m1 = 0, n1 = 0;
        char **Strings_Input1 = NULL;
        wchar_t *wStrings_Input1 = NULL;
        int m1n1 = 0; /* m1 * n1 */

        int m2 = 0, n2 = 0;
        char **Strings_Input2 = NULL;
        wchar_t **wStrings_Input2 = NULL;
        int m2n2 = 0; /* m2 * n2 */

        struct In *values = NULL;

        int nbValues = 0;
        int nbposition = 0;

        int i = 0;

        GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, &Strings_Input1);
        m1n1 = m1 * n1;

        if (m1n1 != 1)
        {
            freeArrayOfString(Strings_Input1, m1n1);
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        wStrings_Input1 = to_wide_string(Strings_Input1[0]);
        if (wStrings_Input1 == NULL)
        {
            /* string conversion fails */
            freeArrayOfString(Strings_Input1, m1n1);
            freeArrayOfString(Strings_Input2, m2n2);
            Scierror(999, _("%s: Wrong value for input argument #%d: A valid string expected (UTF-8 Encoding problem).\n"), fname, 1);
            return 0;
        }

        GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &m2, &n2, &Strings_Input2);
        m2n2 = m2 * n2;

        if ( (m2 != 1) && (n2 != 1) )
        {
            freeArrayOfString(Strings_Input1, m1n1);
            freeArrayOfString(Strings_Input2, m2n2);
            Scierror(999, _("%s: Wrong type for input argument #%d: Row vector of strings or column vector of strings expected.\n"), fname, 2);
            return 0;
        }

        wStrings_Input2 = (wchar_t**)MALLOC(m2n2 * sizeof(wchar_t*));
        for (i = 0 ; i < m2n2 ; i++)
        {
            wStrings_Input2[i] = to_wide_string(Strings_Input2[i]);
        }

        if ( (int)wcslen(wStrings_Input1) == 0 )
        {
            values = (struct In*)MALLOC(sizeof(struct In));
        }
        else
        {
            values = (struct In *)MALLOC( sizeof(struct In) * ( wcslen(wStrings_Input1) ) * m2n2);
        }

        if (bStrindex_with_pattern)
        {
            int x = 0;
            pcre_error_code w = PCRE_FINISHED_OK;

            int Output_Start = 0;
            int Output_End = 0;

            /* We use pcre library */
            for (x = 0; x < m2n2; ++x)
            {
                w = pcre_private(Strings_Input1[0], Strings_Input2[x], &Output_Start, &Output_End, NULL, NULL);
                if ( w == PCRE_FINISHED_OK)
                {
                    char *partStr = strdup(Strings_Input1[0]);
                    wchar_t *wcpartStr = NULL;
                    partStr[Output_Start] = '\0';
                    wcpartStr = to_wide_string(partStr);
                    values[nbValues++].data = (int)wcslen(wcpartStr) + 1; /* adding the answer into the outputmatrix */
                    values[nbposition++].position = x + 1;      /* The number according to the str2 matrix */

                    if (partStr)
                    {
                        FREE(partStr);
                        partStr = NULL;
                    }
                    if (wcpartStr)
                    {
                        FREE(wcpartStr);
                        wcpartStr = NULL;
                    }
                }
                else
                {
                    if (w != NO_MATCH)
                    {
                        freeArrayOfString(Strings_Input1, m1n1);
                        freeArrayOfString(Strings_Input2, m2n2);
                        pcre_error(fname, w);
                        return 0;
                    }
                    break;
                }
            }

            qsort(values, nbValues, sizeof(values[0]), cmp);

        }
        else
        {
            /* We don't use pcre library */
            int x = 0;

            for (x = 0; x < m2n2 ; ++x)
            {
                if ( wcslen(wStrings_Input2[x]) == 0 )
                {
                    freeArrayOfWideString(wStrings_Input2, m2n2);
                    freeArrayOfString(Strings_Input2, m2n2);
                    freeArrayOfString(Strings_Input1, m1n1);
                    if (next)
                    {
                        FREE(next);
                        next = NULL;
                    }
                    if (values)
                    {
                        FREE(values);
                        values = NULL;
                    }
                    Scierror(999, _("%s: Wrong size for input argument #%d: Non-empty string expected.\n"), fname, 2);
                    return 0;
                }
                if (Strings_Input2)
                {
                    wchar_t *pCur = wStrings_Input1;
                    do
                    {
                        pCur = wcsstr(pCur, wStrings_Input2[x]);
                        if (pCur != NULL)
                        {
                            pCur++;
                            values[nbValues++].data = (int)(pCur - wStrings_Input1);
                            values[nbposition++].position = x + 1;
                        }
                    }
                    while (pCur != NULL && *pCur != 0); //Plus tard

                    /* values are sorted */
                    qsort(values, nbValues, sizeof(values[0]), cmp);
                }
            }
        }

        FREE(wStrings_Input1);
        freeArrayOfWideString(wStrings_Input2, m2n2);
        freeArrayOfString(Strings_Input1, m1n1);
        freeArrayOfString(Strings_Input2, m2n2);

        numRow   = 1;
        outIndex = 0;
        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &numRow, &nbValues, &outIndex);
        for ( i = 0 ; i < nbValues ; i++ )
        {
            stk(outIndex)[i] = (double)values[i].data ;
        }
        LhsVar(1) = Rhs + 1 ;

        if (Lhs == 2)
        {
            numRow   = 1;
            outIndex = 0;
            CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &numRow, &nbposition, &outIndex);
            for ( i = 0 ; i < nbposition ; i++ )
            {
                stk(outIndex)[i] = (double)values[i].position ;
            }
            LhsVar(2) = Rhs + 2;
        }

        if (values)
        {
            FREE(values);
            values = NULL;
        }
        PutLhsVar();
    }
    else
    {
        if (VarType(1) != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Row vector of strings or column vector of strings expected.\n"), fname, 2);
        }
        return 0;
    }
    return 0;
}
/*------------------------------------------------------------------------*/
