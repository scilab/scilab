/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "CallJuigetfile.hxx"
#include "GiwsException.hxx"
#include "BOOL.h"

extern "C"
{
#include <stdio.h>
#include "gw_gui.h"
#include "PATH_MAX.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
}

using namespace org_scilab_modules_gui_filechooser;

/*--------------------------------------------------------------------------*/
int sci_uigetdir(char *fname, unsigned long l)
{
    SciErr sciErr;

    int nbRow = 0, nbCol = 0;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;

    char* title = NULL;
    char* initialDirectory = NULL;
    char** userSelection = NULL;
    char* expandedpath = NULL;

    CheckInputArgument(pvApiCtx, 0, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) >= 1)
    {
        /* First argument is initial directory */
        if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddr1, &initialDirectory))
            {
                printError(&sciErr, 0);
                return 1;
            }

            expandedpath = expandPathVariable(initialDirectory);
            freeAllocatedSingleString(initialDirectory);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 1;
        }
    }

    if (nbInputArgument(pvApiCtx) == 2)
    {
        /* Second argument is title */
        if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
            if (sciErr.iErr)
            {
                FREE(expandedpath);
                printError(&sciErr, 0);
                return 1;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddr2, &title))
            {
                FREE(expandedpath);
                printError(&sciErr, 0);
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            FREE(expandedpath);
            return 1;
        }
    }

    try
    {
        switch (nbInputArgument(pvApiCtx))
        {
                /* Initial path is given */
            case 1:
                CallJuigetfileForDirectoryWithInitialdirectory(expandedpath);
                FREE(expandedpath);
                break;
                /* Initial path and title are given */
            case 2:
                CallJuigetfileForDirectoryWithInitialdirectoryAndTitle(expandedpath, title);
                FREE(expandedpath);
                freeAllocatedSingleString(title);
                break;
                /* Default call with default path and title */
            default:
                CallJuigetfileForDirectoryWithoutInput();
                break;
        }

        /* Read the size of the selection, if 0 then no file selected */
        nbRow = getJuigetfileSelectionSize();
        /* Read the selection */
        userSelection = getJuigetfileSelection();
    }
    catch (const GiwsException::JniCallMethodException & exception)
    {
        FREE(expandedpath);
        FREE(title);
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 1;
    }
    catch (const GiwsException::JniException & e)
    {
        FREE(expandedpath);
        FREE(title);
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return 1;
    }

    if (nbRow != 0)
    {
        /* The user selected a file --> returns the files names */
        nbCol = 1;

        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, userSelection);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        if (userSelection)
        {
            for (int i = 0; i < nbRow; i++)
            {
                if (userSelection[i])
                {
                    delete userSelection[i];
                    userSelection[i] = NULL;
                }
            }
            delete[]userSelection;
            userSelection = NULL;
        }
    }
    else
    {
        char* empty = "";
        /* The user canceled the selection --> returns an empty string */
        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &empty);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    returnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
