/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "freeArrayOfString.h"

#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
/*--------------------------------------------------------------------------*/
int sci_progressionbar(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrhandleAdr = NULL;
    long long* handleAdr = NULL;
    int* piAddrmessageAdr = NULL;
    long long* stkAdr = NULL;

    int iProgressionbarUID = 0;

    int nbRow = 0, nbCol = 0;
    int nbRowMessage = 0, nbColMessage = 0;

    char **messageAdr = NULL;
    int iValue = 0;
    unsigned long GraphicHandle = 0;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrhandleAdr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (nbInputArgument(pvApiCtx) == 1)
    {
        if ((checkInputArgumentType(pvApiCtx, 1, sci_handles)))  /* ProgressionBar to update */
        {
            // Retrieve a matrix of handle at position 1.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrhandleAdr, &nbRow, &nbCol, &handleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else if ((checkInputArgumentType(pvApiCtx, 1, sci_strings))) /* Message to display */
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrmessageAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 1.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrmessageAdr, &nbRowMessage, &nbColMessage, &messageAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle or a string expected.\n"), fname, 1);
            return FALSE;
        }

        if (handleAdr == 0)
        {
            /* Create a new ProgressionBar */
            iProgressionbarUID = createGraphicObject(__GO_PROGRESSIONBAR__);
            GraphicHandle = getHandle(iProgressionbarUID);
            setGraphicObjectProperty(iProgressionbarUID, __GO_UI_MESSAGE__, messageAdr, jni_string_vector, nbColMessage * nbRowMessage);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
        }
        else
        {
            GraphicHandle = (unsigned long) * (handleAdr);
            iProgressionbarUID = getObjectFromHandle(GraphicHandle);
            setGraphicObjectProperty(iProgressionbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
        }
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        if ((checkInputArgumentType(pvApiCtx, 1, sci_handles)) && (checkInputArgumentType(pvApiCtx, 2, sci_strings))) /* progressionbar(id,mes) */
        {
            // Retrieve a matrix of handle at position 1.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrhandleAdr, &nbRow, &nbCol, &handleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrmessageAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 2.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrmessageAdr, &nbRowMessage, &nbColMessage, &messageAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                return 1;
            }

            GraphicHandle = (unsigned long) * handleAdr;
            iProgressionbarUID = getObjectFromHandle(GraphicHandle);

            setGraphicObjectProperty(iProgressionbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
            setGraphicObjectProperty(iProgressionbarUID, __GO_UI_MESSAGE__, messageAdr, jni_string_vector, nbColMessage * nbRowMessage);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
        }
        else
        {
            Scierror(999, _("%s: Wrong input arguments: '%s' expected.\n"), fname, "(id, mes)");
            return FALSE;
        }
    }

    if (nbOutputArgument(pvApiCtx) == 1)
    {
        nbRow = 1;
        nbCol = 1;

        sciErr = allocMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &stkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        *stkAdr = (long long)GraphicHandle;
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }
    else
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
    }

    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
