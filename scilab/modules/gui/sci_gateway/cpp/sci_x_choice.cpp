/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java implementation)
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
#include "sci_malloc.h"
#include "api_scilab.h"
#include "localization.h"
#include "CallMessageBox.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
int sci_x_choice(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrdefaultValuesAdr = NULL;
    int* piAddrlabelsAdr = NULL;
    int* piAddrlineLabelsAdr = NULL;
    double* emptyMatrixAdr = NULL;

    int nbRow = 0, nbCol = 0;
    int nbRowDefaultValues = 0, nbColDefaultValues = 0;
    int nbRowLineLabels = 0, nbColLineLabels = 0;

    int messageBoxID = 0;

    char **labelsAdr = NULL;
    char **lineLabelsAdr = NULL;
    double *defaultValues = NULL;
    int *defaultValuesInt = NULL;

    int userValueSize = 0;
    int *userValue = NULL;
    double *userValueDouble = NULL;

    int K = 0;

    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /* READ THE DEFAULT VALUES */
    if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrdefaultValuesAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrdefaultValuesAdr, &nbRowDefaultValues, &nbColDefaultValues, &defaultValues);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
            return 1;
        }

        defaultValuesInt = (int *)MALLOC(nbRowDefaultValues * nbColDefaultValues * sizeof(int));
        for (K = 0; K < nbRowDefaultValues * nbColDefaultValues; K++)
        {
            defaultValuesInt[K] = (int)defaultValues[K];
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real or complex vector expected.\n"), fname, 1);
        return FALSE;
    }

    /* READ THE MESSAGE */
    if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrlabelsAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            FREE(defaultValuesInt);
            return 1;
        }

        // Retrieve a matrix of string at position 2.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrlabelsAdr, &nbRow, &nbCol, &labelsAdr))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
            FREE(defaultValuesInt);
            return 1;
        }

    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
        FREE(defaultValuesInt);
        return FALSE;
    }

    /* Create the Java Object */
    messageBoxID = createMessageBox();

    /* Title is a default title */
    setMessageBoxTitle(messageBoxID, _("Scilab Choices Request"));

    /* Message */
    setMessageBoxMultiLineMessage(messageBoxID, labelsAdr, nbCol * nbRow);
    freeAllocatedMatrixOfString(nbRow, nbCol, labelsAdr);

    /* READ THE LABELS */
    if (checkInputArgumentType(pvApiCtx, 3, sci_strings))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrlineLabelsAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            FREE(defaultValuesInt);
            return 1;
        }

        // Retrieve a matrix of string at position 3.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrlineLabelsAdr, &nbRowLineLabels, &nbColLineLabels, &lineLabelsAdr))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
            FREE(defaultValuesInt);
            return 1;
        }

        if (nbRow != 1 && nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of strings expected.\n"), fname, 3);
            freeAllocatedMatrixOfString(nbRowLineLabels, nbColLineLabels, lineLabelsAdr);
            FREE(defaultValuesInt);
            return FALSE;
        }
        setMessageBoxLineLabels(messageBoxID, lineLabelsAdr, nbColLineLabels * nbRowLineLabels);
        freeAllocatedMatrixOfString(nbRowLineLabels, nbColLineLabels, lineLabelsAdr);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 3);
        FREE(defaultValuesInt);
        return FALSE;
    }

    /* Default selected buttons */
    setMessageBoxDefaultSelectedButtons(messageBoxID, defaultValuesInt, nbRowDefaultValues * nbColDefaultValues);
    FREE(defaultValuesInt);

    /* Display it and wait for a user input */
    messageBoxDisplayAndWait(messageBoxID);

    /* Read the user answer */
    userValueSize = getMessageBoxValueSize(messageBoxID);
    if (userValueSize == 0)
    {
        nbRow = 0;
        nbCol = 0;

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
        userValue = (int*)getMessageBoxUserSelectedButtons(messageBoxID);

        userValueDouble = (double *)MALLOC(nbRowDefaultValues * nbColDefaultValues * sizeof(double));
        for (K = 0; K < nbRowDefaultValues * nbColDefaultValues; K++)
        {
            userValueDouble[K] = userValue[K];
        }

        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRowDefaultValues, nbColDefaultValues, userValueDouble);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            delete[] userValue;
            return 1;
        }
    }

    delete[] userValue;
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
