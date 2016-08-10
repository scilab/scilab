/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2008 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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

/*------------------------------------------------------------------------*/
/* file: sci_uigetfile.cpp                                                */
/* desc : interface for uigetfile routine                                 */
/*------------------------------------------------------------------------*/
#include "CallJuigetfile.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_gui.h"
#include "PATH_MAX.h"
#include "api_scilab.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "BOOL.h"
}
/*--------------------------------------------------------------------------*/
#define freePointersUigetfile()                 \
    if (selection)                              \
    {                                           \
        for (int i = 0; i < selectionSize; i++) \
        {                                       \
            if (selection[i])                   \
            {                                   \
                delete selection[i];            \
                selection[i] = NULL;            \
            }                                   \
        }                                       \
        delete [] selection;                    \
        selection = NULL;                       \
    }                                           \
    if (selectionPathName)                      \
    {                                           \
        delete selectionPathName;               \
        selectionPathName = NULL;               \
    }                                           \
    if (selectionFileNames)                     \
    {                                           \
        for (int i = 0; i < selectionSize; i++) \
        {                                       \
            if (selectionFileNames[i])          \
            {                                   \
                delete selectionFileNames[i];   \
                selectionFileNames[i] = NULL;   \
            }                                   \
        }                                       \
        delete [] selectionFileNames;           \
        selectionFileNames = NULL;              \
    }                                           \
    if (menuCallback)                           \
    {                                           \
        delete menuCallback;                    \
        menuCallback = NULL;                    \
    }
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_filechooser;

/*--------------------------------------------------------------------------*/

int sci_uigetfile(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int nbRow  = 0, nbCol  = 0;
    int nbRow2 = 0, nbCol2 = 0;
    int nbRow3 = 0, nbCol3 = 0;
    int nbRow4 = 0, nbCol4 = 0;

    int nbRowOutSelection = 1, nbColOutSelection = 0;
    int nbRowOutFilterIndex = 1, nbColOutFilterIndex = 1;
    int nbRowOutPath = 1, nbColOutPath = 1;

    char** mask = NULL;
    char** description = NULL;

    char* titleBox = NULL;
    char* selectionPathName = NULL;
    char* initialDirectory = NULL;

    int multipleSelection = 0;

    char **selection = NULL;
    char **selectionFileNames = NULL;
    int selectionSize = 0;
    int filterIndex = 0;

    char *menuCallback = NULL;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;

    CheckInputArgument(pvApiCtx, 0, 4);
    CheckOutputArgument(pvApiCtx, 1, 3);

    //inputs checking
    /* call uigetfile with 1 arg */
    if (nbInputArgument(pvApiCtx) >= 1)
    {
        if (checkInputArgumentType(pvApiCtx, 1, sci_strings) == FALSE)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string matrix expected.\n"), fname, 1);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedMatrixOfString(pvApiCtx, piAddr1, &nbRow, &nbCol, &mask))
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (nbCol == 1)
        {
            // only masks of files are provided
            description = NULL;
        }
        else if (nbCol == 2)
        {
            // mask contains both the masks for files and the description of each mask
            // in the sequence [m1, m2,..., mn, d1, d2,...,dn].
            // So description is at the middle of the array.
            description = (char **)MALLOC(sizeof(char *) * nbRow);
            for (int i = 0; i < nbRow; i++)
            {
                description[i] = os_strdup(mask[nbRow + i]);
            }
        }
        else
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, mask);
            Scierror(999, _("%s: Wrong size for input argument #%d: A string matrix expected.\n"), fname, 1);
            return 1;
        }
    }

    /* call uigetfile with 2 arg */
    if (nbInputArgument(pvApiCtx) >= 2)
    {
        char *path = NULL;

        if (checkInputArgumentType(pvApiCtx, 2, sci_strings) == FALSE)
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, mask);
            freeArrayOfString(description, nbRow);
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, mask);
            freeArrayOfString(description, nbRow);
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddr2, &initialDirectory))
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, mask);
            freeArrayOfString(description, nbRow);
            printError(&sciErr, 0);
            return 1;
        }

        path = expandPathVariable(initialDirectory);
        freeAllocatedSingleString(initialDirectory);
        initialDirectory = path;
    }

    /* call uigetfile with 3 arg */
    if (nbInputArgument(pvApiCtx) >= 3)
    {
        if (checkInputArgumentType(pvApiCtx, 3, sci_strings) == FALSE)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
            freeAllocatedMatrixOfString(nbRow, nbCol, mask);
            freeArrayOfString(description, nbRow);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, mask);
            freeArrayOfString(description, nbRow);
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddr3, &titleBox))
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, mask);
            freeArrayOfString(description, nbRow);
            printError(&sciErr, 0);
            return 1;
        }
    }

    try
    {
        /* Call Java */
        switch (nbInputArgument(pvApiCtx))
        {
            case 0:
                CallJuigetfileWithoutInput();
                break;

            case 1:
                CallJuigetfileOnlyWithMask(mask, description, nbRow);
                break;

            case 2:
                CallJuigetfileWithMaskAndInitialdirectory(mask, description, nbRow, initialDirectory);
                break;

            case 3:
                CallJuigetfileWithoutMultipleSelection(mask, description, nbRow, initialDirectory, titleBox);
                break;

            case 4:
            {
                if (checkInputArgumentType(pvApiCtx, 4, sci_boolean) == FALSE)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 4);
                    freeArrayOfString(description, nbRow);
                    freeAllocatedMatrixOfString(nbRow, nbCol, mask);
                    freeAllocatedSingleString(initialDirectory);
                    freeAllocatedSingleString(titleBox);

                    return 0;
                }

                sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
                if (sciErr.iErr)
                {
                    freeArrayOfString(description, nbRow);
                    freeAllocatedMatrixOfString(nbRow, nbCol, mask);
                    freeAllocatedSingleString(initialDirectory);
                    freeAllocatedSingleString(titleBox);
                    printError(&sciErr, 0);
                    return 1;
                }

                if (getScalarBoolean(pvApiCtx, piAddr4, &multipleSelection))
                {
                    freeArrayOfString(description, nbRow);
                    freeAllocatedMatrixOfString(nbRow, nbCol, mask);
                    freeAllocatedSingleString(initialDirectory);
                    freeAllocatedSingleString(titleBox);
                    printError(&sciErr, 0);
                    return 1;
                }

                CallJuigetfile(mask, description, nbRow, initialDirectory, titleBox, BOOLtobool(multipleSelection));
            }
            break;

            default:
                // never here
                break;
        }

        // free pointer
        freeArrayOfString(description, nbRow);
        freeAllocatedMatrixOfString(nbRow, nbCol, mask);
        freeAllocatedSingleString(initialDirectory);
        freeAllocatedSingleString(titleBox);

        // Get return values
        selection = getJuigetfileSelection();
        selectionPathName = getJuigetfileSelectionPathName();
        selectionFileNames = getJuigetfileSelectionFileNames();
        selectionSize = getJuigetfileSelectionSize();
        filterIndex = getJuigetfileFilterIndex();
        menuCallback = getJuigetfileMenuCallback();
    }
    catch (const GiwsException::JniCallMethodException & exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return 0;
    }

    // nbColOutSelection
    nbColOutSelection = selectionSize;

    //if cancel is selected on the filechooser
    if (strcmp(selection[0], "") == 0)
    {
        // "" is returned as filename
        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, selection);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

        if (nbOutputArgument(pvApiCtx) > 1)
        {
            // "" is returned as pathname
            sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 1, 1, selection);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
        }

        if (nbOutputArgument(pvApiCtx) > 2)
        {
            // 0 is returned as pathname
            double tmp = 0;
            sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, 1, 1, &tmp);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
        }

        freePointersUigetfile();
        ReturnArguments(pvApiCtx);
        return 0;
    }

    // Only one output then it contains path+filenames
    if (nbOutputArgument(pvApiCtx) == 1)
    {
        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRowOutSelection, nbColOutSelection, selection);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        freePointersUigetfile();
        ReturnArguments(pvApiCtx);
        return 0;
    }

    // More than one output
    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRowOutSelection, nbColOutSelection, selectionFileNames);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 2, selectionPathName))
    {
        printError(&sciErr, 0);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    if (nbOutputArgument(pvApiCtx) > 2)
    {
        if (createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, filterIndex))
        {
            printError(&sciErr, 0);
            return 1;
        }
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
    }

    freePointersUigetfile();
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
