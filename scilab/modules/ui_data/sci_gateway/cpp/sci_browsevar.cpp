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

extern "C"
{
#include <string.h>
#include "localization.h"
#include "gw_ui_data.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "stackinfo.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}
using namespace org_scilab_modules_ui_data;
/*--------------------------------------------------------------------------*/
int sci_browsevar(char *fname,unsigned long fname_len)
{
    int iGlobalVariablesUsed = 0;
    int iGlobalVariablesTotal = 0;
    int iLocalVariablesUsed = 0;
    int iLocalVariablesTotal = 0;
    int i = 0;

    CheckRhs(0, 0);
    CheckLhs(0, 1);	

    // First get how many global / local variable we have.
    C2F(getvariablesinfo)(&iLocalVariablesTotal, &iLocalVariablesUsed);
    C2F(getgvariablesinfo)(&iGlobalVariablesTotal, &iGlobalVariablesUsed);

    char ** pstAllVariableNames = (char **) MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    char ** pstAllVariableVisibility = (char **) MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    int * piAllVariableBytes = (int *) MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));
    int * piAllVariableTypes = (int *) MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));

    // for each local variable get informations
    for (i = 0 ; i < iLocalVariablesUsed ; ++i)
    {
        // name
        pstAllVariableNames[i] = getLocalNamefromId(i+1);
        // type
        getNamedVarType(pvApiCtx, pstAllVariableNames[i], &piAllVariableTypes[i]);
        // Bytes used
        piAllVariableBytes[i] = getLocalSizefromId(i);
        // global / local ??
        pstAllVariableVisibility[i] = strdup("local");
    }

    // for each global variable get informations
    for (int j = 0 ; j < iGlobalVariablesUsed ; ++j, ++i)
    {
        // name
        pstAllVariableNames[i] = getGlobalNamefromId(j);
        // Bytes used
        piAllVariableBytes[i] = getGlobalSizefromId(j);
        // type
        // Calling "API Scilab": not yet implemented for global variable
        //getNamedVarType(pvApiCtx, pstAllVariableNames[i], &piAllVariableTypes[i]);
        // Using old stack operations...
        piAllVariableTypes[i] = GetType(C2F(vstk).isiz + 2 + j);
        // global / local ??
        pstAllVariableVisibility[i] = strdup("global");
    }

    char *pstColumnNames[] = {_("Icon"), 
        _("Name"), 
        //_("Value"),
        //_("Size"),
        _("Bytes"),
        _("Type"),
        //_("Min"),
        //_("Max"),
        //_("Range"),
        //_("Mean"),
        //_("Median"),
        //_("Mode"),
        //_("Var"),
        //_("Std"),
        _("Visibility")
    };

    // Launch Java Variable Browser through JNI
    BrowseVar::openVariableBrowser(getScilabJavaVM(), 
        pstColumnNames, 5,
        pstAllVariableNames, iLocalVariablesUsed + iGlobalVariablesUsed,
        piAllVariableBytes, iLocalVariablesUsed + iGlobalVariablesUsed,
        piAllVariableTypes, iLocalVariablesUsed + iGlobalVariablesUsed,
        pstAllVariableVisibility, iLocalVariablesUsed + iGlobalVariablesUsed
        );

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

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
