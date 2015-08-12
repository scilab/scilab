/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int sci_x_dialog(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrlabelsAdr = NULL;
    int* piAddrinitialValueAdr = NULL;
    double* emptyMatrixAdr = NULL;

    int nbRow = 0, nbCol = 0;

    int messageBoxID = 0;

    char **initialValueAdr = 0;

    char **labelsAdr = 0;

    int userValueSize = 0;
    char **userValue = NULL;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

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
            Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 1);
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
    setMessageBoxTitle(messageBoxID, _("Scilab Input Value Request"));
    /* Message */
    setMessageBoxMultiLineMessage(messageBoxID, labelsAdr, nbCol * nbRow);
    freeAllocatedMatrixOfString(nbRow, nbCol, labelsAdr);

    if (nbInputArgument(pvApiCtx) == 2)
    {
        if (VarType(2) ==  sci_strings)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrinitialValueAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 2.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrinitialValueAdr, &nbRow, &nbCol, &initialValueAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), fname, 2);
            return FALSE;
        }

        setMessageBoxInitialValue(messageBoxID, initialValueAdr, nbCol * nbRow);
        freeAllocatedMatrixOfString(nbRow, nbCol, initialValueAdr);
    }

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
        userValue = getMessageBoxValue(messageBoxID);

        nbCol = 1;
        CreateVarFromPtr(nbInputArgument(pvApiCtx) + 1, MATRIX_OF_STRING_DATATYPE, &userValueSize, &nbCol, userValue);
        /* TO DO : delete of userValue */
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
