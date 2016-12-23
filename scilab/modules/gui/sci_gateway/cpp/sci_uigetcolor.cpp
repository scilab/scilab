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
#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "CallColorChooser.h"
}

/*--------------------------------------------------------------------------*/
int sci_uigetcolor(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    //WARNING ALL NEW DECALRATIONS ARE HERE IF YOUR HAVE MANY FUNCTIONS
    //IN THE FILE YOU HAVE PROBABLY TO MOVE DECLARATIONS IN GOOD FUNCTIONS
    int* piAddrredAdr = NULL;
    double* redAdr = NULL;
    int* piAddrtitleAdr = NULL;
    char* titleAdr = NULL;
    int* piAddrgreenAdr = NULL;
    double* greenAdr = NULL;
    int* piAddrblueAdr = NULL;
    double* blueAdr = NULL;

    int colorChooserID = 0;
    int firstColorIndex = 0;

    int nbRow = 0, nbCol = 0;

    double *selectedRGB = NULL;

    CheckInputArgument(pvApiCtx, 0, 4);

    if ((nbOutputArgument(pvApiCtx) != 1) && (nbOutputArgument(pvApiCtx) != 3)) /* Bad use */
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d or %d expected.\n"), fname, 1, 3);
        return FALSE;
    }

    /* Rhs==1: title or [R, G, B] given */
    if (nbInputArgument(pvApiCtx) == 1)
    {
        if ((checkInputArgumentType(pvApiCtx, 1, sci_matrix)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrredAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrredAdr, &nbRow, &nbCol, &redAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if ((nbRow != 1) || (nbCol != 3))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A 1 x %d real row vector expected.\n"), fname, 1, 3);
                return FALSE;
            }
        }
        else if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrtitleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrtitleAdr, &titleAdr))
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
    }

    /* Title and [R, G, B] given */
    if (nbInputArgument(pvApiCtx) == 2)
    {
        /* Title */
        if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrtitleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrtitleAdr, &titleAdr))
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

        /* [R, G, B] */
        if ((checkInputArgumentType(pvApiCtx, 2, sci_matrix)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrredAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrredAdr, &nbRow, &nbCol, &redAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            if (nbRow * nbCol != 3)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A 1 x %d real row vector expected.\n"), fname, 2, 3);
                freeAllocatedSingleString(titleAdr);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A 1 x %d real row vector expected.\n"), fname, 2, 3);
            return FALSE;
        }
    }

    /* No title given but colors given with separate values */
    if (nbInputArgument(pvApiCtx) == 3)
    {
        firstColorIndex = 1;
    }

    /* Title and colors given with separate values */
    if (nbInputArgument(pvApiCtx) == 4)
    {
        firstColorIndex = 2;

        /* Title */
        if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrtitleAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrtitleAdr, &titleAdr))
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
    }

    /* R, G, B given */
    if (nbInputArgument(pvApiCtx) >= 3)
    {
        /* Default red value */
        if ((checkInputArgumentType(pvApiCtx, firstColorIndex, sci_matrix)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, firstColorIndex, &piAddrredAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            // Retrieve a matrix of double at position firstColorIndex.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrredAdr, &nbRow, &nbCol, &redAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, firstColorIndex);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, firstColorIndex);
                freeAllocatedSingleString(titleAdr);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, firstColorIndex);
            freeAllocatedSingleString(titleAdr);
            return FALSE;
        }

        /* Default green value */
        if (checkInputArgumentType(pvApiCtx, firstColorIndex + 1, sci_matrix))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, firstColorIndex + 1, &piAddrgreenAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            // Retrieve a matrix of double at position firstColorIndex + 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrgreenAdr, &nbRow, &nbCol, &greenAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, firstColorIndex + 1);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, firstColorIndex + 1);
                freeAllocatedSingleString(titleAdr);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, firstColorIndex + 1);
            freeAllocatedSingleString(titleAdr);
            return FALSE;
        }

        /* Default blue value */
        if (checkInputArgumentType(pvApiCtx, firstColorIndex + 2, sci_matrix))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, firstColorIndex + 2, &piAddrblueAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            // Retrieve a matrix of double at position firstColorIndex + 2.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrblueAdr, &nbRow, &nbCol, &blueAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, firstColorIndex + 2);
                freeAllocatedSingleString(titleAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, firstColorIndex + 2);
                freeAllocatedSingleString(titleAdr);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, firstColorIndex + 2);
            freeAllocatedSingleString(titleAdr);
            return FALSE;
        }
    }

    /* Create the Java Object */
    colorChooserID = createColorChooser();

    /* Title */
    if (titleAdr != 0)
    {
        setColorChooserTitle(colorChooserID, titleAdr);
        freeAllocatedSingleString(titleAdr);
    }

    /* Default red value */
    if (redAdr != 0)
    {
        if (greenAdr != 0 ) /* All values given in first input argument */
        {
            setColorChooserDefaultRGBSeparateValues(colorChooserID, (int)redAdr[0], (int)greenAdr[0], (int)blueAdr[0]);
        }
        else
        {
            setColorChooserDefaultRGB(colorChooserID, redAdr);
        }
    }

    /* Display it and wait for a user input */
    colorChooserDisplayAndWait(colorChooserID);

    /* Return the selected color */
    /* Read the user answer */
    selectedRGB = getColorChooserSelectedRGB(colorChooserID);

    if (selectedRGB[0] >= 0) /* The user selected a color */
    {

        nbRow = 1;
        if (nbOutputArgument(pvApiCtx) == 1)
        {
            nbCol = 3;
            sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, selectedRGB);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        }

        if (nbOutputArgument(pvApiCtx) >= 2)
        {
            nbCol = 1;

            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &redAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }

            redAdr[0] = selectedRGB[0];

            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, nbRow, nbCol, &greenAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }

            greenAdr[0] = selectedRGB[1];

            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, nbRow, nbCol, &blueAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }

            blueAdr[0] = selectedRGB[2];

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
            AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
        }
    }
    else /* The user canceled */
    {
        nbRow = 0;
        nbCol = 0;
        if (nbOutputArgument(pvApiCtx) == 1)
        {
            /* Return [] */
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &redAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }


            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        }

        if (nbOutputArgument(pvApiCtx) >= 2)
        {
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &redAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }

            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, nbRow, nbCol, &greenAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }

            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, nbRow, nbCol, &blueAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                delete[] selectedRGB;
                return 1;
            }


            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
            AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
        }
    }

    delete[] selectedRGB;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
