/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
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

extern "C"
{
#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
int sci_x_mdialog(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrlabelsAdr = NULL;
    int* piAddrlineLabelsAdr = NULL;
    int* piAddrdefaultValuesAdr = NULL;
    int* piAddrcolumnLabelsAdr = NULL;
    double* emptyMatrixAdr = NULL;

    int nbRow = 0, nbCol = 0;
    int nbRowDefaultValues = 0, nbColDefaultValues = 0;
    int nbRowLineLabels = 0, nbColLineLabels = 0;
    int nbRowColumnLabels = 0, nbColColumnLabels = 0;

    int messageBoxID = 0;

    char **labelsAdr = NULL;
    char **lineLabelsAdr = NULL;
    char **columnLabelsAdr = NULL;
    char **defaultValuesAdr = NULL;

    int userValueSize = 0;
    char **userValue = NULL;

    CheckInputArgument(pvApiCtx, 3, 4);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /* READ THE LABELS */
    if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrlabelsAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 1.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrlabelsAdr, &nbRow, &nbCol, &labelsAdr))
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

    /* Create the Java Object */
    messageBoxID = createMessageBox();

    /* Title is a default title */
    setMessageBoxTitle(messageBoxID, _("Scilab Multiple Values Request"));
    /* Message */
    setMessageBoxMultiLineMessage(messageBoxID, labelsAdr, nbCol * nbRow);
    freeAllocatedMatrixOfString(nbRow, nbCol, labelsAdr);

    /* READ THE LINE LABELS */
    if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrlineLabelsAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 2.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrlineLabelsAdr, &nbRowLineLabels, &nbColLineLabels, &lineLabelsAdr))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
            return 1;
        }

        if (nbRowLineLabels != 1 && nbColLineLabels != 1)
        {
            freeAllocatedMatrixOfString(nbRowLineLabels, nbColLineLabels, lineLabelsAdr);
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 2);
            return FALSE;
        }
        setMessageBoxLineLabels(messageBoxID, lineLabelsAdr, nbColLineLabels * nbRowLineLabels);
        freeAllocatedMatrixOfString(nbRowLineLabels, nbColLineLabels, lineLabelsAdr);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
        return FALSE;
    }

    /* READ THE COLUMN LABELS or DEFAULT VALUES */
    if (checkInputArgumentType(pvApiCtx, 3, sci_strings))
    {
        if (nbInputArgument(pvApiCtx) == 3)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrdefaultValuesAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 3.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrdefaultValuesAdr, &nbRowDefaultValues, &nbColDefaultValues, &defaultValuesAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                return 1;
            }

            if ((nbRowDefaultValues != nbRowLineLabels) || (nbColDefaultValues != nbColLineLabels))
            {
                freeAllocatedMatrixOfString(nbRowDefaultValues, nbColDefaultValues, defaultValuesAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: It must have same dimensions as argument #%d.\n"), fname, 3, 2);
                return FALSE;
            }

            setMessageBoxDefaultInput(messageBoxID, defaultValuesAdr, nbColDefaultValues * nbRowDefaultValues);
            freeAllocatedMatrixOfString(nbRowDefaultValues, nbColDefaultValues, defaultValuesAdr);
        }
        else
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrcolumnLabelsAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 3.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrcolumnLabelsAdr, &nbRowColumnLabels, &nbColColumnLabels, &columnLabelsAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                return 1;
            }

            if (nbRowColumnLabels != 1 && nbColColumnLabels != 1)
            {
                freeAllocatedMatrixOfString(nbRowColumnLabels, nbColColumnLabels, columnLabelsAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 3);
                return FALSE;
            }
            setMessageBoxColumnLabels(messageBoxID, columnLabelsAdr, nbColColumnLabels * nbRowColumnLabels);
            freeAllocatedMatrixOfString(nbRowColumnLabels, nbColColumnLabels, columnLabelsAdr);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 3);
        return FALSE;
    }

    if (nbInputArgument(pvApiCtx) == 4)
    {
        /* READ  DEFAULT VALUES */
        if (checkInputArgumentType(pvApiCtx, 4, sci_strings))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrdefaultValuesAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 4.
            // DO NOT FORGET TO RELEASE MEMORY via freeAllocatedMatrixOfString(nbRowDefaultValues, nbColDefaultValues, defaultValuesAdr).
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrdefaultValuesAdr, &nbRowDefaultValues, &nbColDefaultValues, &defaultValuesAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 4);
                return 1;
            }

            if ((nbRowDefaultValues != nbRowLineLabels * nbColLineLabels) || (nbColDefaultValues != nbRowColumnLabels * nbColColumnLabels))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: %d x %d matrix of strings expected.\n"), fname, 4, nbRowLineLabels * nbColLineLabels, nbRowColumnLabels * nbColColumnLabels);
                freeArrayOfString(defaultValuesAdr, nbColDefaultValues * nbRowDefaultValues);
                return FALSE;
            }
            setMessageBoxDefaultInput(messageBoxID, defaultValuesAdr, nbColDefaultValues * nbRowDefaultValues);
            freeArrayOfString(defaultValuesAdr, nbColDefaultValues * nbRowDefaultValues);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 4);
            return FALSE;
        }
    }

    /* Display it and wait for a user input */
    messageBoxDisplayAndWait(messageBoxID);

    /* Read the user answer */
    userValueSize = getMessageBoxValueSize(messageBoxID);
    if (userValueSize == 0)
    {
        nbRow = 0;
        nbCol = 0;
        // YOU MUST REMOVE YOUR VARIABLE DECLARATION "int emptyMatrixAdr".
        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &emptyMatrixAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

    }
    else
    {
        userValue = getMessageBoxValue(messageBoxID);
        nbCol = 1;
        nbRowDefaultValues = nbColLineLabels * nbRowLineLabels;
        nbColDefaultValues = 1;
        if (nbInputArgument(pvApiCtx) == 4)
        {
            nbColDefaultValues = nbColColumnLabels * nbRowColumnLabels;
        }

        createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRowDefaultValues, nbColDefaultValues, userValue);
        delete[] userValue;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
