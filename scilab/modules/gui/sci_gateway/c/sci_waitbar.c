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
int sci_waitbar(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrfractionAdr = NULL;
    double* fractionAdr = NULL;
    int* piAddrmessageAdr = NULL;
    int* piAddrhandleAdr = NULL;
    long long* handleAdr = NULL;
    long long* stkAdr = NULL;

    int iWaitbarUID = 0;

    int nbRow = 0;
    int nbCol = 0;
    int nbRowMessage = 0;
    int nbColMessage = 0;

    char **messageAdr = NULL;
    int iValue = 0;

    unsigned long GraphicHandle = 0;

    CheckInputArgument(pvApiCtx, 1, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {
        if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))   /* Fraction to display */
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrfractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrfractionAdr, &nbRow, &nbCol, &fractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else if (checkInputArgumentType(pvApiCtx, 1, sci_strings)) /* Message to display */
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
            Scierror(999, _("%s: Wrong type for input argument #%d: A real or a string expected.\n"), fname, 1);
            return FALSE;
        }

        /* Create a new waitbar */
        iWaitbarUID = createGraphicObject(__GO_WAITBAR__);
        GraphicHandle = getHandle(iWaitbarUID);

        if (fractionAdr != NULL)
        {
            iValue = (int)(fractionAdr[0] * 100);
            setGraphicObjectProperty(iWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
        }
        else if (messageAdr != NULL)
        {
            setGraphicObjectProperty(iWaitbarUID, __GO_UI_MESSAGE__, messageAdr, jni_string_vector, nbColMessage * nbRowMessage);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
        }
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        if ((checkInputArgumentType(pvApiCtx, 1, sci_matrix)) && (checkInputArgumentType(pvApiCtx, 2, sci_strings))) /* waitbar(x,mes) */
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrfractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrfractionAdr, &nbRow, &nbCol, &fractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
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


            iWaitbarUID = createGraphicObject(__GO_WAITBAR__);

            GraphicHandle = getHandle(iWaitbarUID);

            iValue = (int)(fractionAdr[0] * 100);
            setGraphicObjectProperty(iWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
            setGraphicObjectProperty(iWaitbarUID, __GO_UI_MESSAGE__, messageAdr, jni_string_vector, nbColMessage * nbRowMessage);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
        }
        else if ((checkInputArgumentType(pvApiCtx, 1, sci_matrix)) && (checkInputArgumentType(pvApiCtx, 2, sci_handles))) /* waitbar(x,winId) */
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrfractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrfractionAdr, &nbRow, &nbCol, &fractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrhandleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of handle at position 2.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrhandleAdr, &nbRow, &nbCol, &handleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 2);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle expected.\n"), fname, 2, "Waitbar");
                return FALSE;
            }

            GraphicHandle = (unsigned long) * handleAdr;
            iWaitbarUID = getObjectFromHandle(GraphicHandle);
            if (iWaitbarUID == 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A valid '%s' handle expected.\n"), fname, 2, "Waitbar");
                return FALSE;
            }

            iValue = (int)(fractionAdr[0] * 100);
            setGraphicObjectProperty(iWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
        }
        else if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)) && (checkInputArgumentType(pvApiCtx, 2, sci_handles)))   /* waitbar(mes,winId) */
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

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrhandleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            // Retrieve a matrix of handle at position 2.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrhandleAdr, &nbRow, &nbCol, &handleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 2);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle expected.\n"), fname, 2, "Waitbar");
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return FALSE;
            }

            GraphicHandle = (unsigned long) * handleAdr;
            iWaitbarUID = getObjectFromHandle(GraphicHandle);
            if (iWaitbarUID == 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A valid '%s' handle expected.\n"), fname, 2, "Waitbar");
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return FALSE;
            }

            setGraphicObjectProperty(iWaitbarUID, __GO_UI_MESSAGE__, messageAdr, jni_string_vector, nbColMessage * nbRowMessage);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
        }
        else
        {
            Scierror(999, _("%s: Wrong input arguments: '%s', '%s' or '%s' expected.\n"), fname, "(x, mes)", "(x, winId)", "(mes, winId)");
            return FALSE;
        }
    }
    else
    {
        if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))  /* Fraction */
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrfractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrfractionAdr, &nbRow, &nbCol, &fractionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
            return FALSE;
        }

        if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))  /* Message */
        {
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
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return FALSE;
        }

        if ((checkInputArgumentType(pvApiCtx, 3, sci_handles)))  /* Handle */
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrhandleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            // Retrieve a matrix of handle at position 3.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrhandleAdr, &nbRow, &nbCol, &handleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 3);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle expected.\n"), fname, 3, "Waitbar");
                return FALSE;
            }
        }
        else
        {
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 3, "Waitbar");
            return FALSE;
        }

        GraphicHandle = (unsigned long) * handleAdr;
        iWaitbarUID = getObjectFromHandle(GraphicHandle);
        if (iWaitbarUID != 0)
        {
            iValue = (int)(fractionAdr[0] * 100);
            setGraphicObjectProperty(iWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
            setGraphicObjectProperty(iWaitbarUID, __GO_UI_MESSAGE__, messageAdr, jni_string_vector, nbColMessage * nbRowMessage);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
        }
        else
        {
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
            Scierror(999, _("%s: Wrong value for input argument #%d: A valid '%s' handle expected.\n"), fname, 3, "Waitbar");
            return 0;
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

        *stkAdr = GraphicHandle;
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
