/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "predef.h"
#include "api_scilab.h"
#include "stackinfo.h"
#include "stack-def.h"
#include "core_math.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int getNumberPredefVariablesProtected(void)
{
    return (C2F(vstk).isiz - C2F(vstk).bbot);
}
/*--------------------------------------------------------------------------*/
void setNumberPredefVariablesProtected(int nb)
{
    int new_value = Max( C2F(vstk).bot , Min( (C2F(vstk).isiz - nb), C2F(vstk).bot0 ) );
    if ( C2F(vstk).bbot != new_value ) C2F(vstk).bbot = new_value;
}
/*--------------------------------------------------------------------------*/
void clearPredef(void)
{
    if ( C2F(vstk).bbot != C2F(vstk).bot0 ) C2F(vstk).bbot = C2F(vstk).bot0;
}
/*--------------------------------------------------------------------------*/
void predefAll(void)
{
    if (C2F(vstk).bbot != C2F(vstk).bot)
    {
        /* bug 3773 */
        /* "ans" must not be protected */
        char *FirstNameVariableOnStack = getLocalNamefromId(1);

        if (FirstNameVariableOnStack)
        {
            if (strcmp(FirstNameVariableOnStack, "ans") == 0)
            {
                /* protect variable after "ans" */
                C2F(vstk).bbot = C2F(vstk).bot + 1;
            }
            else
            {
                C2F(vstk).bbot = C2F(vstk).bot;
            }
            FREE(FirstNameVariableOnStack);
        }
        else
        {
            C2F(vstk).bbot = C2F(vstk).bot;
        }
    }
}
/*--------------------------------------------------------------------------*/
BOOL isPredefinedVariable(const char *variablename)
{
    if (!isNamedVarExist(pvApiCtx, variablename))
    {
        int nbPredefined = getNumberPredefVariablesProtected() - 1; // remove ans
        if (nbPredefined > 0)
        {
            int i = 0;
            int Ltotal = 0;
            int Lused = 0;

            C2F(getvariablesinfo)(&Ltotal, &Lused);

            for(i = 0; i < nbPredefined; i++)
            {
                char *varname = getLocalNamefromId(Lused - i - 1);
                if (varname)
                {
                    if (strcmp(variablename, varname) == 0)
                    {
                        FREE(varname);
                        varname = NULL;
                        return TRUE;
                    }
                    FREE(varname);
                    varname = NULL;
                }
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
char **getPredefinedVariablesName(int *iReturnedSize)
{
    char **variablesName = NULL;
    int nbPredefined = 0;

    *iReturnedSize = 0;

    nbPredefined = getNumberPredefVariablesProtected() - 1; // remove ans
    if (nbPredefined > 0)
    {
        int Ltotal = 0;
        int Lused = 0;

        C2F(getvariablesinfo)(&Ltotal, &Lused);

        variablesName = (char**)MALLOC(sizeof(char*) * nbPredefined);
        if (variablesName)
        {
            int i = 0;

            for(i = 0; i < nbPredefined; i++)
            {
                variablesName[i] = getLocalNamefromId(Lused - i - 1);
            }
            *iReturnedSize = nbPredefined;
        }
    }
    return variablesName;
}
/*--------------------------------------------------------------------------*/
