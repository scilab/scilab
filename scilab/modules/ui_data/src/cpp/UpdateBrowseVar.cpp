/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include "BrowseVar.hxx"

#include <string>
#include <iterator>
using std::string;

#include <set>

extern "C"
{
#include <string.h>
#include "UpdateBrowseVar.h"
#include "localization.h"
#include "MALLOC.h"
#include "BOOL.h"
#include "stackinfo.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "sci_types.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}
using namespace org_scilab_modules_ui_data;

static std::set < string > createScilabDefaultVariablesSet();

/*--------------------------------------------------------------------------*/
void UpdateBrowseVar(BOOL update)
{
    SciErr err;
    int iGlobalVariablesUsed = 0;
    int iGlobalVariablesTotal = 0;
    int iLocalVariablesUsed = 0;
    int iLocalVariablesTotal = 0;
    int i = 0;

    if (update && !BrowseVar::isVariableBrowserOpened(getScilabJavaVM()))
    {
        return;
    }

    // First get how many global / local variable we have.
    C2F(getvariablesinfo) (&iLocalVariablesTotal, &iLocalVariablesUsed);
    C2F(getgvariablesinfo) (&iGlobalVariablesTotal, &iGlobalVariablesUsed);

    char **pstAllVariableNames = (char **)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    char **pstAllVariableVisibility = (char **)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    int *piAllVariableBytes = (int *)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));
    char **pstAllVariableSizes = (char **)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    int *piAllVariableTypes = (int *)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));
    bool *piAllVariableFromUser = (bool *) MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(BOOL));
    int nbRows, nbCols;
    char *sizeStr = NULL;

    std::set < string > scilabDefaultVariablesSet = createScilabDefaultVariablesSet();

    // for each local variable get informations
    for (; i < iLocalVariablesUsed; ++i)
    {
        // name
        pstAllVariableNames[i] = getLocalNamefromId(i + 1);
        // type
        err = getNamedVarType(pvApiCtx, pstAllVariableNames[i], &piAllVariableTypes[i]);
        if (!err.iErr)
        {
            piAllVariableBytes[i] = getLocalSizefromId(i);
            err = getNamedVarDimension(pvApiCtx, pstAllVariableNames[i], &nbRows, &nbCols);
        }

        if (err.iErr || nbRows * nbCols == 0)
        {
#define N_A "N/A"
            pstAllVariableSizes[i] = (char *)MALLOC((sizeof(N_A) + 1) * sizeof(char));
            strcpy(pstAllVariableSizes[i], N_A);
        }
        else
        {
            // 11 =strlen("2147483647")+1 (1 for security)
            sizeStr = (char *)MALLOC((11 + 11 + 1 + 1) * sizeof(char));
            sprintf(sizeStr, "%dx%d", nbRows, nbCols);
            pstAllVariableSizes[i] = strdup(sizeStr);
            FREE(sizeStr);
        }

        // global / local ??
        pstAllVariableVisibility[i] = strdup("local");

        if (scilabDefaultVariablesSet.find(string(pstAllVariableNames[i])) == scilabDefaultVariablesSet.end() && piAllVariableTypes[i] != sci_c_function    /*TODO: voir si je fais sauter ou pas */
                && piAllVariableTypes[i] != sci_lib)
        {
            piAllVariableFromUser[i] = TRUE;
        }
        else
        {
            piAllVariableFromUser[i] = FALSE;
        }
    }

    // for each global variable get informations
    for (int j = 0; j < iGlobalVariablesUsed; ++j, ++i)
    {
        // name
        pstAllVariableNames[i] = getGlobalNamefromId(j);
        // Bytes used
        piAllVariableBytes[i] = getGlobalSizefromId(j);
        // type
        // Calling "API Scilab": not yet implemented for global variable
        //getNamedVarType(pvApiCtx, pstAllVariableNames[i], &piAllVariableTypes[i]);
        // Using old stack operations...
        int pos = C2F(vstk).isiz + 2 + j;

        piAllVariableTypes[i] = C2F(gettype) (&pos);

        // Sizes of the variable
        getNamedVarDimension(pvApiCtx, pstAllVariableNames[i], &nbRows, &nbCols);
        // 11 =strlen("2147483647")+1 (1 for security)
        sizeStr = (char *)MALLOC((11 + 11 + 1 + 1) * sizeof(char));
        sprintf(sizeStr, "%dx%d", nbRows, nbCols);
        pstAllVariableSizes[i] = strdup(sizeStr);
        FREE(sizeStr);

        // global / local ??
        pstAllVariableVisibility[i] = strdup("global");

        if (scilabDefaultVariablesSet.find(string(pstAllVariableNames[i])) == scilabDefaultVariablesSet.end()
                && piAllVariableTypes[i] != sci_c_function && piAllVariableTypes[i] != sci_lib)
        {
            piAllVariableFromUser[i] = TRUE;
        }
        else
        {
            piAllVariableFromUser[i] = FALSE;
        }
    }

    // Launch Java Variable Browser through JNI
    BrowseVar::openVariableBrowser(getScilabJavaVM(),
                                   BOOLtobool(update),
                                   pstAllVariableNames, iLocalVariablesUsed + iGlobalVariablesUsed,
                                   piAllVariableBytes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                   piAllVariableTypes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                   pstAllVariableSizes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                   pstAllVariableVisibility, iLocalVariablesUsed + iGlobalVariablesUsed,
                                   piAllVariableFromUser, iLocalVariablesUsed + iGlobalVariablesUsed);

    freeArrayOfString(pstAllVariableNames, iLocalVariablesUsed + iGlobalVariablesUsed);
    freeArrayOfString(pstAllVariableVisibility, iLocalVariablesUsed + iGlobalVariablesUsed);
    if (piAllVariableBytes)
    {
        FREE(piAllVariableBytes);
        piAllVariableBytes = NULL;
    }

    if (piAllVariableTypes)
    {
        FREE(piAllVariableTypes);
        piAllVariableTypes = NULL;
    }

    if (pstAllVariableSizes)
    {
        FREE(pstAllVariableSizes);
        pstAllVariableSizes = NULL;
    }
}

/*--------------------------------------------------------------------------*/
static std::set < string > createScilabDefaultVariablesSet()
{
    string arr[] = { "home",
                     "PWD",
                     "%tk",
                     "%pvm",
                     "MSDOS",
                     "%F",
                     "%T",
                     "%f",
                     "%t",
                     "%e",
                     "%pi",
                     "%modalWarning",
                     "%nan",
                     "%inf",
                     "SCI",
                     "WSCI",
                     "SCIHOME",
                     "TMPDIR",
                     "%gui",
                     "%fftw",
                     "%helps",
                     "%eps",
                     "%io",
                     "%i",
                     "demolist",
                     "%z",
                     "%s",
                     "$",
                     "%toolboxes",
                     "%toolboxes_dir",
                     "TICTOC",
                     "%helps_modules",
                     "%_atoms_cache"
                   };
    int i = 0;

#define NBELEMENT 32
    std::set < string > ScilabDefaultVariables;

    for (i = 0; i <= NBELEMENT; i++)
    {
        ScilabDefaultVariables.insert(arr[i]);
    }

    return ScilabDefaultVariables;
}
