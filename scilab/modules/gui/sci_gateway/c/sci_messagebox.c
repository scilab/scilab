/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (java version)
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
#include "os_string.h"
#include "messageboxoptions.h"

/*--------------------------------------------------------------------------*/
int sci_messagebox(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrmessageAdr       = NULL;
    int* piAddrtitleAdr         = NULL;
    int* piAddriconAdr          = NULL;
    int* piAddrbuttonsTextAdr   = NULL;
    int* piAddrmodalOptionAdr   = NULL;
    double* buttonNumberAdr     = NULL;

    int messageBoxID = 0;

    /* Used to read input arguments */
    int nbRow = 0, nbCol = 0;
    int nbRowButtons = 0, nbColButtons = 0;
    int nbRowMessage = 0, nbColMessage = 0;

    char **buttonsTextAdr   = 0;
    char **messageAdr       = 0;
    char **titleAdr         = 0;
    char **modalOptionAdr   = 0;
    char **iconAdr          = 0;

    /* Used to write output argument */
    int buttonNumber = 0;

    CheckInputArgument(pvApiCtx, 1, 5);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /* Message to be displayed */
    if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
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
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return FALSE;
    }

    /* Title to be displayed */
    if (nbInputArgument(pvApiCtx) >= 2)
    {
        if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrtitleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            // Retrieve a matrix of string at position 2.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrtitleAdr, &nbRow, &nbCol, &titleAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 2);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                freeAllocatedMatrixOfString(nbRow, nbCol, titleAdr);
                return FALSE;
            }
            /* The title argument can be used to give the modal option */
            if (isModalOption(titleAdr[0]))
            {
                modalOptionAdr = titleAdr;
                titleAdr = NULL;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
            return FALSE;
        }
    }

    /* Icon to be displayed */
    if (nbInputArgument(pvApiCtx) >= 3)
    {
        if ((checkInputArgumentType(pvApiCtx, 3, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddriconAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            // Retrieve a matrix of string at position 3.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddriconAdr, &nbRow, &nbCol, &iconAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            if (nbRow * nbCol == 1)
            {
                /* The icon argument can be used to give the modal option or the buttons names */
                if (isModalOption(iconAdr[0]))
                {
                    modalOptionAdr = (char **)iconAdr;
                    iconAdr = NULL;
                }
                else if (!isIconName(iconAdr[0]))
                {
                    buttonsTextAdr = (char **)iconAdr;
                    nbRowButtons = nbRow;
                    nbColButtons = nbCol;
                    iconAdr = NULL;
                }
            }
            else  /* More than one string --> buttons names */
            {
                buttonsTextAdr = (char **)iconAdr;
                nbRowButtons = nbRow;
                nbColButtons = nbCol;
                iconAdr = NULL;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string or string vector expected.\n"), fname, 3);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
            return FALSE;
        }
    }

    /* Buttons names */
    if (nbInputArgument(pvApiCtx) >= 4)
    {
        if ((checkInputArgumentType(pvApiCtx, 4, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrbuttonsTextAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            // Retrieve a matrix of string at position 4.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrbuttonsTextAdr, &nbRowButtons, &nbColButtons, &buttonsTextAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 4);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            if (nbRow * nbCol == 1)
            {
                /* The buttons names argument can be used to give the modal option */
                if (isModalOption(buttonsTextAdr[0]))
                {
                    modalOptionAdr = buttonsTextAdr;
                    buttonsTextAdr = NULL;
                }
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string or string vector expected.\n"), fname, 3);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
            return FALSE;
        }
    }

    /* Modal option */
    if (nbInputArgument(pvApiCtx) == 5)
    {
        if ((checkInputArgumentType(pvApiCtx, 5, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrmodalOptionAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            // Retrieve a matrix of string at position 5.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrmodalOptionAdr, &nbRow, &nbCol, &modalOptionAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 5);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 5);
                freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 5);
            freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);
            return FALSE;
        }
    }
    /* Create the Java Object */
    messageBoxID = createMessageBox();

    /* Message */
    setMessageBoxMultiLineMessage(messageBoxID, messageAdr, nbColMessage * nbRowMessage);
    freeAllocatedMatrixOfString(nbRowMessage, nbColMessage, messageAdr);

    /* Title */
    if (titleAdr != NULL)
    {
        setMessageBoxTitle(messageBoxID, titleAdr[0]);
        freeAllocatedMatrixOfString(nbRow, nbCol, titleAdr);
    }
    else
    {
        setMessageBoxTitle(messageBoxID, _("Scilab Message"));
    }

    /* Icon */
    if (iconAdr != NULL)
    {
        setMessageBoxIcon(messageBoxID, iconAdr[0]);
        freeAllocatedMatrixOfString(nbRow, nbCol, iconAdr);
    }

    /* Buttons */
    if (buttonsTextAdr != NULL)
    {
        setMessageBoxButtonsLabels(messageBoxID, buttonsTextAdr, nbColButtons * nbRowButtons);
        freeAllocatedMatrixOfString(nbRowButtons, nbColButtons, buttonsTextAdr);
    }

    /* Modal ? */
    if (modalOptionAdr != NULL)
    {
        setMessageBoxModal(messageBoxID, !stricmp(modalOptionAdr[0], "modal"));
        freeAllocatedMatrixOfString(nbRow, nbCol, modalOptionAdr);
    }
    else
    {
        setMessageBoxModal(messageBoxID, FALSE);
    }

    /* Display it and wait for a user input */
    messageBoxDisplayAndWait(messageBoxID);

    /* Return the index of the button selected */
    if (nbOutputArgument(pvApiCtx) == 1)
    {
        /* Read the user answer */
        buttonNumber = getMessageBoxSelectedButton(messageBoxID);

        nbRow = 1;
        nbCol = 1;

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &buttonNumberAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        buttonNumberAdr[0] = buttonNumber;

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
