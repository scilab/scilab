/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "call_scilab.h"
#include "sciprint.h"

static int findOptional(void* _pvCtx, char *fname, rhs_opts opts[]);
static void printOptionalNames(void* _pvCtx, rhs_opts opts[]);

/**************************/
/*   optional functions   */
/**************************/
int getOptionals(void* _pvCtx, char* fname, rhs_opts opts[])
{
    int k, i = 0;
    char name[nlgh + 1];
    int nopt = NumOpt();        /* optional arguments on the stack */

    /* reset first field since opts is declared static in calling function */
    /* this could be avoided with ansi compilers by removing static in the
     * opts declaration */

    while (opts[i].pstName != NULL)
    {
        opts[i].iPos = -1;
        i++;
    }

    /* Walking through last arguments */

    for (k = Rhs - nopt + 1; k <= Rhs; k++)
    {
        if (IsOpt(k, name) == 0)
        {
            Scierror(999, _("%s: Optional arguments name=val must be at the end.\n"), fname);
            return 0;
        }
        else
        {
            int isopt = findOptional(_pvCtx, name, opts);

            if (isopt >= 0)
            {
                rhs_opts *ro = &opts[isopt];
                ro->iPos = k;
                getVarAddressFromPosition(_pvCtx, k, &ro->piAddr);
                getVarType(_pvCtx, ro->piAddr, &ro->iType);
                getVarDimension(_pvCtx, ro->piAddr, &ro->iRows, &ro->iCols);
            }
            else
            {
                sciprint(_("%s: Unrecognized optional arguments %s.\n"), fname, name);
                printOptionalNames(_pvCtx, opts);
                SciError(999);
                return 0;
            }
        }
    }
    return 1;
}

static int findOptional(void* _pvCtx, char *fname, rhs_opts opts[])
{
    int rep = -1, i = 0;

    while (opts[i].pstName != NULL)
    {
        int cmp;

        /* name is terminated by white space and we want to ignore them */
        if ((cmp = strcmp(fname, opts[i].pstName)) == 0)
        {
            rep = i;
            break;
        }
        else if (cmp < 0)
        {
            break;
        }
        else
        {
            i++;
        }
    }
    return rep;
}

void printOptionalNames(void* _pvCtx, rhs_opts opts[])
/* array of optinal names (in alphabetical order)
* the array is null terminated */
{
    int i = 0;

    if (opts[i].pstName == NULL)
    {
        sciprint(_("Optional argument list is empty.\n"));
        return;
    }
    sciprint(_("Optional arguments list: \n"));
    while (opts[i + 1].pstName != NULL)
    {
        sciprint("%s, ", opts[i].pstName);
        i++;
    }
    sciprint(_("and %s.\n"), opts[i].pstName);
}
