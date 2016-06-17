
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
/* file: sci_xlfontt.c                                                    */
/* desc : interface for xlfont routine                                    */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "RendererFontManager.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "FileExist.h"
/*--------------------------------------------------------------------------*/
static int xlfont_no_rhs(char * fname, void* pvApiCtx);
static int xlfont_one_rhs(char * fname, void* pvApiCtx);
static int xlfont_n_rhs(char * fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
int sci_xlfont(char * fname, void *pvApiCtx)
{
    CheckInputArgument(pvApiCtx, 0, 4);
    CheckOutputArgument(pvApiCtx, 0, 1);

    switch (nbInputArgument(pvApiCtx))
    {
        case 0:
            return xlfont_no_rhs(fname, pvApiCtx);
            break;
        case 1:
            return xlfont_one_rhs(fname, pvApiCtx);
            break;
        default:
            return xlfont_n_rhs(fname, pvApiCtx);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int xlfont_no_rhs(char * fname, void* pvApiCtx)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0;
    int nbElements = 0;
    char **fontsname = getInstalledFontsName(&nbElements);

    m1 = 1;
    n1 = nbElements;

    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, (const char * const*)fontsname);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    freeArrayOfString(fontsname, nbElements);

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
static int xlfont_one_rhs(char * fname, void* pvApiCtx)
{
    SciErr sciErr;
    if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
    {
        int* piAddrl1 = NULL;
        int* l1 = NULL;
        char* strl1 = NULL;
        int m1 = 0, n1 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        if (getAllocatedSingleString(pvApiCtx, piAddrl1, &strl1))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
            return 1;
        }

        if (strcmp(strl1, "AVAILABLE_FONTS") == 0)
        {
            int nbElements = 0;
            char **fontsname = getAvailableFontsName(&nbElements);
            freeAllocatedSingleString(strl1);

            m1 = nbElements;
            n1 = 1;

            sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, (const char * const*)fontsname);
            freeArrayOfString(fontsname, nbElements);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
            return 0;
        }
        else if (strcmp(strl1, "reset") == 0)
        {
            resetFontManager();
            freeAllocatedSingleString(strl1);
            AssignOutputVariable(pvApiCtx, 1) = 0;
            ReturnArguments(pvApiCtx);
            return 0;
        }
        else
        {
            if (isAvailableFontsName(strl1))
            {
                int fontID = addFont(strl1);
                freeAllocatedSingleString(strl1);

                m1 = 1;
                n1 = 1;

                sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx,  nbInputArgument(pvApiCtx) + 1, m1, n1, &l1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }

                l1[0] = fontID;

                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
                return 0;
            }
            else if (FileExist(strl1))
            {
                int fontID = addFontFromFilename(strl1);
                freeAllocatedSingleString(strl1);

                m1 = 1;
                n1 = 1;

                sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx,  nbInputArgument(pvApiCtx) + 1, m1, n1, &l1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }

                l1[0] = fontID;

                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
                return 0;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A valid fontname expected.\n"), fname, 1);
            }
        }

        freeAllocatedSingleString(strl1);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static int xlfont_n_rhs(char * fname, void* pvApiCtx)
{
    SciErr sciErr;
    BOOL isBold = FALSE;
    BOOL isItalic = FALSE;

    if (nbInputArgument(pvApiCtx) == 3)
    {
        int m3 = 0, n3 = 0;
        int* piAddrl3 = NULL;
        int* l3 = NULL;
        if ((!checkInputArgumentType(pvApiCtx, 3, sci_boolean)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of boolean at position 3.
        sciErr = getMatrixOfBoolean(pvApiCtx, piAddrl3, &m3, &n3, &l3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), fname, 3);
            return 1;
        }

        isBold = (BOOL) * l3;
    }

    if (nbInputArgument(pvApiCtx) == 4)
    {
        int m4 = 0, n4 = 0;
        int* piAddrl4 = NULL;
        int* l4 = NULL;
        if ((!checkInputArgumentType(pvApiCtx, 4, sci_boolean)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
            return 0;
        }
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrl4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of boolean at position 4.
        sciErr = getMatrixOfBoolean(pvApiCtx, piAddrl4, &m4, &n4, &l4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), fname, 4);
            return 1;
        }

        isItalic = (BOOL) * l4;
    }

    if (((checkInputArgumentType(pvApiCtx, 1, sci_strings))) && ((checkInputArgumentType(pvApiCtx, 2, sci_matrix))))
    {
        int m1 = 0, n1 = 0;
        int m2 = 0, n2 = 0;
        char* strl1 = NULL;
        double* l2 = NULL;
        int* l1 = NULL;

        int* piAddrl1 = NULL;
        int* piAddrl2 = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        if (getAllocatedSingleString(pvApiCtx, piAddrl1, &strl1))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(strl1);
            return 1;
        }

        // Retrieve a matrix of double at position 2.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
            freeAllocatedSingleString(strl1);
            return 1;
        }

        if ((m2 == 1) && (n2 == 1))
        {
            int fontIndex = (int)l2[0];
            char *fontname = strl1;
            if (fontIndex < 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Non-negative int expected.\n"), fname, 2);
                freeAllocatedSingleString(strl1);
                return 0;
            }

            if ((nbInputArgument(pvApiCtx) == 2) && FileExist(fontname))
            {
                int Id = changeFontFromFilename(fontIndex, fontname);
                m1 = 1;
                n1 = 1;

                sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx,  nbInputArgument(pvApiCtx) + 1, m1, n1, &l1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    freeAllocatedSingleString(strl1);
                    return 1;
                }

                l1[0] = Id;

                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
            }
            else if (isAvailableFontsName(fontname))
            {
                int Id = changeFontWithProperty(fontIndex, fontname, isBold, isItalic);
                m1 = 1;
                n1 = 1;

                sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx,  nbInputArgument(pvApiCtx) + 1, m1, n1, &l1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    freeAllocatedSingleString(strl1);
                    return 1;
                }

                l1[0] = Id;

                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A valid fontname expected.\n"), fname, 1);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
        }

        freeAllocatedSingleString(strl1);
    }
    else
    {
        if ((!checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return 0;
        }

        if ((checkInputArgumentType(pvApiCtx, 2, sci_matrix)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Non-negative int expected.\n"), fname, 2);
            return 0;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
