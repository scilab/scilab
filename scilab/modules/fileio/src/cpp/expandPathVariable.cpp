/*--------------------------------------------------------------------------*/
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include <wchar.h>
#include "context.hxx"
#include "string.hxx"
#include "expandPathVariable.h"

extern "C"
{
#include "charEncoding.h"
#include "sci_malloc.h"
#include "PATH_MAX.h"
#include "api_scilab.h"
#include "getlongpathname.h"
#include "os_string.h"
}
/*--------------------------------------------------------------------------*/
struct VARIABLEALIAS
{
    const char *Alias;
    const char *VariableName;
    symbol::Variable* var;
};
/*--------------------------------------------------------------------------*/
#define NB_ALIAS 7
static struct VARIABLEALIAS VARIABLES_words[NB_ALIAS] =
{
    {"SCIHOME", "SCIHOME", NULL},
    {"WSCI", "WSCI", NULL},
    {"SCI", "SCI", NULL},
    {"~", "home", NULL},
    {"HOME", "home", NULL},
    {"home", "home", NULL},
    {"TMPDIR", "TMPDIR", NULL}
};
/*--------------------------------------------------------------------------*/
static char *getVariableValueDefinedInScilab(VARIABLEALIAS* var);
static char *convertFileSeparators(char *str);
/*--------------------------------------------------------------------------*/
char *expandPathVariable(const char* str)
{
    char *expanded = NULL;
    if (str)
    {
        int i = 0;
        int lenStr = (int)strlen(str);

        for (i = 0; i < NB_ALIAS; i++)
        {
            int lenAlias = 0;

            /* input is ALIAS without subdirectory */
            if (strcmp(VARIABLES_words[i].Alias, str) == 0)
            {
                char* expanded = getVariableValueDefinedInScilab(&VARIABLES_words[i]);
                if (expanded)
                {
                    return convertFileSeparators(expanded);
                }
            }

            lenAlias = (int)strlen(VARIABLES_words[i].Alias);

            if (lenStr > lenAlias)
            {
                char* begin = (char *)MALLOC(sizeof(char) * (lenAlias + 1));
                if (begin)
                {
                    strncpy(begin, str, lenAlias);
                    begin[lenAlias] = 0;

                    if (strcmp(begin, VARIABLES_words[i].Alias) == 0 )
                    {
                        if ( (str[lenAlias] == '/') || (str[lenAlias] == '\\') )
                        {
                            char* newBegin = getVariableValueDefinedInScilab(&VARIABLES_words[i]);
                            if (newBegin)
                            {
                                int lengthnewBegin = (int)strlen(newBegin);
                                expanded = (char*)MALLOC(sizeof(char) * (lengthnewBegin + (int)strlen(&str[lenAlias]) + 1));
                                if (expanded)
                                {
                                    strcpy(expanded, newBegin);
                                    strcat(expanded, &str[lenAlias]);
                                    FREE(begin);
                                    begin = NULL;
                                    free(newBegin);
                                    newBegin = NULL;
                                    return convertFileSeparators(expanded);
                                }
                                FREE(newBegin);
                                newBegin = NULL;
                            }
                        }
                    }
                    FREE(begin);
                    begin = NULL;
                }
            }
        }

        /* Variables not founded returns a copy of input */
        expanded = (char*)MALLOC(sizeof(char) * ((int)strlen(str) + 1));
        if (expanded)
        {
            strcpy(expanded, str);
            return convertFileSeparators(expanded);
        }
    }
    return expanded;
}
/*--------------------------------------------------------------------------*/
char *getVariableValueDefinedInScilab(VARIABLEALIAS* _var)
{
    if (_var)
    {
        if (_var->var == NULL)
        {
            _var->var = symbol::Context::getInstance()->getOrCreate(symbol::Symbol(_var->VariableName));
        }

        types::InternalType *pIT = _var->var->get();
        if (pIT == NULL || pIT->isString() == false)
        {
            return NULL;
        }

        types::String* pS = pIT->getAs<types::String>();
        return os_strdup(pS->get(0));
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
void resetVariableValueDefinedInScilab(void)
{
    for (int i = 0; i < NB_ALIAS; i++)
    {
        VARIABLES_words[i].var = NULL;
    }
}
/*--------------------------------------------------------------------------*/
char* convertFileSeparators(char *str)
{
    if (str)
    {
        int k = 0;
        int len = (int)strlen(str);

#ifdef _MSC_VER
        for (k = 0; k < len; k++) if (str[k] == '/')
        {
            str[k] = '\\';
        }
#else
        for (k = 0 ; k < len ; k++) if (str[k] == '\\')
        {
            str[k] = '/';
        }
#endif
    }
    return str;
}
/*--------------------------------------------------------------------------*/
