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

extern "C"
{
#include <string.h>
#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "CallFontChooser.h"
#include "getPropertyAssignedValue.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
int sci_uigetfont(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrfontNameAdr  = NULL;
    int* piAddrfontSizeAdr  = NULL;
    int* piAddrboldAdr      = NULL;
    int* boldAdr            = NULL;
    int* piAddritalicAdr    = NULL;
    int* italicAdr          = NULL;
    double* fontSizeAdr     = NULL;

    int fontChooserID = 0;
    int nbRow = 0;
    int nbCol = 0;

    char **fontNameAdr = NULL;
    int fontNameSize   = 0;

    char *selectedFontName  = NULL;
    int selectedFontSize    = 0;
    BOOL selectedBold       = FALSE;
    BOOL selectedItalic     = FALSE;

    CheckInputArgument(pvApiCtx, 0, 4);
    CheckOutputArgument(pvApiCtx, 1, 4);

    /* Default font name */
    if (nbInputArgument(pvApiCtx) >= 1)
    {
        if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrfontNameAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 1.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrfontNameAdr, &nbRow, &nbCol, &fontNameAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
                return 1;
            }

            fontNameSize = nbRow * nbCol;
            if (fontNameSize != 1)
            {
                freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return FALSE;
        }
    }

    /* Default font size */
    if (nbInputArgument(pvApiCtx) >= 2)
    {
        if ((checkInputArgumentType(pvApiCtx, 2, sci_matrix)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrfontSizeAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrfontSizeAdr, &nbRow, &nbCol, &fontSizeAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 2);
                return FALSE;
            }
        }
        else
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
            return FALSE;
        }
    }

    /* Is the default font bold ? */
    if (nbInputArgument(pvApiCtx) >= 3)
    {
        if ((checkInputArgumentType(pvApiCtx, 3, sci_boolean)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrboldAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of boolean at position 3.
            sciErr = getMatrixOfBoolean(pvApiCtx, piAddrboldAdr, &nbRow, &nbCol, &boldAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), fname, 3);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
                return FALSE;
            }

        }
        else
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
            return FALSE;
        }
    }

    /* Is the default font italic ? */
    if (nbInputArgument(pvApiCtx) >= 4)
    {
        if ((checkInputArgumentType(pvApiCtx, 4, sci_boolean)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddritalicAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of boolean at position 4.
            sciErr = getMatrixOfBoolean(pvApiCtx, piAddritalicAdr, &nbRow, &nbCol, &italicAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), fname, 4);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 4);
                return FALSE;
            }

        }
        else
        {
            freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 4);
            return FALSE;
        }
    }

    /* Create the Java Object */
    fontChooserID = createFontChooser();

    /* Default font */
    if (fontNameAdr != NULL)
    {
        setFontChooserFontName(fontChooserID, fontNameAdr[0]);
    }

    /* Default size */
    if (fontSizeAdr != 0)
    {
        setFontChooserFontSize(fontChooserID, (int)fontSizeAdr[0]);
    }

    /* Default bold */
    if (boldAdr != 0)
    {
        setFontChooserBold(fontChooserID, booltoBOOL(boldAdr[0]));
    }

    /* Default italic */
    if (italicAdr != 0)
    {
        setFontChooserItalic(fontChooserID, booltoBOOL(italicAdr[0]));
    }

    /* Display it and wait for a user input */
    fontChooserDisplayAndWait(fontChooserID);

    /* Return the selected font */

    /* Read the user answer */
    selectedFontName = getFontChooserFontName(fontChooserID);


    if (strcmp(selectedFontName, "")) /* The user selected a font */
    {
        selectedFontSize = getFontChooserFontSize(fontChooserID);
        selectedBold = getFontChooserBold(fontChooserID);
        selectedItalic = getFontChooserItalic(fontChooserID);

        nbRow = 1;
        nbCol = 1;
        if (nbOutputArgument(pvApiCtx) >= 1)
        {
            sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, (const char * const*) &selectedFontName);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }
        }

        delete[] selectedFontName;

        if (nbOutputArgument(pvApiCtx) >= 2)
        {
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, nbRow, nbCol, &fontSizeAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            *fontSizeAdr = selectedFontSize;
        }

        if (nbOutputArgument(pvApiCtx) >= 3)
        {
            sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 3, nbRow, nbCol, &boldAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            *boldAdr = selectedBold;
        }

        if (nbOutputArgument(pvApiCtx) >= 4)
        {
            sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 4, nbRow, nbCol, &italicAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            *italicAdr = selectedItalic;
        }
    }
    else /* The user canceled */
    {
        delete[] selectedFontName;
        nbRow = 0;
        nbCol = 0;
        if (nbOutputArgument(pvApiCtx) >= 1)
        {
            /* Return "" as font name */
            char* fontNameEmpty = NULL;
            if (allocSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow * nbCol, (const char**) &fontNameEmpty))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }
        }

        if (nbOutputArgument(pvApiCtx) >= 2)
        {
            /* Return [] as font size */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, nbRow, nbCol, &fontSizeAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }
        }

        if (nbOutputArgument(pvApiCtx) >= 3)
        {
            /* Return [] as bold value */
            sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 3, nbRow, nbCol, &boldAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }
        }

        if (nbOutputArgument(pvApiCtx) >= 4)
        {
            /* Return [] as italic value */
            sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 4, nbRow, nbCol, &italicAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;

    if (fontNameSize)
    {
        freeAllocatedMatrixOfString(nbRow, nbCol, fontNameAdr);
    }
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
