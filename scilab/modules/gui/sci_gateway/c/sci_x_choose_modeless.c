/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_x_choose_modeless(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddritemsAdr = NULL;
    int* piAddrmessageAdr = NULL;
    int* piAddrbuttonLabelAdr = NULL;
    double* userValueAdr = NULL;

    int nbRow = 0, nbCol = 0;
    int nbRowItems = 0, nbColItems = 0;

    int messageBoxID = 0;

    char **itemsAdr = NULL;
    char **buttonLabelAdr = NULL;

    char **messageAdr = NULL;

    int userValue = 0;

    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddritemsAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 1.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddritemsAdr, &nbRowItems, &nbColItems, &itemsAdr))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 1);
        return FALSE;
    }

    if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrmessageAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedMatrixOfString(nbRowItems, nbColItems, itemsAdr);
            return 1;
        }

        // Retrieve a matrix of string at position 2.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrmessageAdr, &nbRow, &nbCol, &messageAdr))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
            freeAllocatedMatrixOfString(nbRowItems, nbColItems, itemsAdr);
            return 1;
        }

    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
        freeAllocatedMatrixOfString(nbRowItems, nbColItems, itemsAdr);
        return FALSE;
    }

    /* Create the Java Object */
    messageBoxID = createMessageBox();

    /* Title is a default title */
    setMessageBoxTitle(messageBoxID, _("Scilab Choose Message"));
    /* Message */
    setMessageBoxMultiLineMessage(messageBoxID, messageAdr, nbCol * nbRow);
    /* ListBox Items */
    setMessageBoxListBoxItems(messageBoxID, itemsAdr, nbColItems * nbRowItems);
    /* Modality */
    setMessageBoxModal(messageBoxID, FALSE);

    freeAllocatedMatrixOfString(nbRowItems, nbColItems, itemsAdr);
    freeAllocatedMatrixOfString(nbRow, nbCol, messageAdr);

    if (nbInputArgument(pvApiCtx) == 3)
    {
        if (checkInputArgumentType(pvApiCtx, 3, sci_strings))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrbuttonLabelAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 3.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrbuttonLabelAdr, &nbRow, &nbCol, &buttonLabelAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                freeAllocatedMatrixOfString(nbRow, nbCol, buttonLabelAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 3);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
            return FALSE;
        }

        setMessageBoxButtonsLabels(messageBoxID, buttonLabelAdr, nbCol * nbRow);
        freeAllocatedMatrixOfString(nbRow, nbCol, buttonLabelAdr);
    }

    /* Display it and wait for a user input */
    messageBoxDisplayAndWait(messageBoxID);

    /* Read the user answer */
    userValue = getMessageBoxSelectedItem(messageBoxID);

    nbRow = 1;
    nbCol = 1;

    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &userValueAdr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    *userValueAdr = userValue;

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
