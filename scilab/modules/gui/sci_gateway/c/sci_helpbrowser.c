/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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
#include "CallHelpBrowser.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "freeArrayOfString.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
int sci_helpbrowser(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrhelpAdr      = NULL;
    int* piAddrkeywordAdr   = NULL;
    int* piAddrfullTextAdr  = NULL;
    int* fullTextAdr        = NULL;

    int nbRow = 0;
    int nbCol = 0;
    char** keywordAdr = NULL;

    int nbRowHelp       = 0;
    int nbColHelp       = 0;
    char** helpAdr      = NULL;
    char** languageAdr  = NULL;
    int ret = 1;

    CheckInputArgument(pvApiCtx, 2, 4);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /* We load SciNotes when calling javahelp because we have no way to know
     * to load it when using Javahelp because it can call SciNotes directly */
    if (!loadedDep)
    {
        loadOnUseClassPath("SciNotes");
        loadedDep = TRUE;
    }

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrhelpAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 1.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrhelpAdr, &nbRowHelp, &nbColHelp, &helpAdr))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 1);
            return 1;
        }
    }
    else if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))
    {
        helpAdr = NULL; /* No toolboxes installed */
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
        return FALSE;
    }

    if (nbInputArgument(pvApiCtx) == 2)
    {
        if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
        {
            int* piAddrlanguageAdr = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrlanguageAdr);
            if (sciErr.iErr)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 2.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrlanguageAdr, &nbRow, &nbCol, &languageAdr))
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
                return 1;
            }

            if (nbRow*nbCol != 1)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedMatrixOfString(nbRow, nbCol, languageAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
                return FALSE;
            }
        }
        else
        {
            if (helpAdr)
            {
                freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
            }
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return FALSE;
        }

        ret = launchHelpBrowser(helpAdr, nbRowHelp * nbColHelp, languageAdr[0]);

        if (helpAdr)
        {
            freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
        }
        freeAllocatedMatrixOfString(nbRow, nbCol, languageAdr);
    }
    else if (nbInputArgument(pvApiCtx) == 4)
    {
        if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrkeywordAdr);
            if (sciErr.iErr)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }

                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 2.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrkeywordAdr, &nbRow, &nbCol, &keywordAdr))
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }

                Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
                return 1;
            }

            if (nbRow*nbCol != 1)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedMatrixOfString(nbRow, nbCol, keywordAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
                return FALSE;
            }
        }
        else
        {
            if (helpAdr)
            {
                freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
            }
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return FALSE;
        }

        if ((checkInputArgumentType(pvApiCtx, 3, sci_strings)))
        {
            int* piAddrlanguageAdr = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrlanguageAdr);
            if (sciErr.iErr)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedSingleString(*keywordAdr);
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 3.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrlanguageAdr, &nbRow, &nbCol, &languageAdr))
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedSingleString(*keywordAdr);
                Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 3);
                return 1;
            }

            if (nbRow*nbCol != 1)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedSingleString(*keywordAdr);
                freeAllocatedMatrixOfString(nbRow, nbCol, languageAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
                return FALSE;
            }
        }
        else
        {
            if (helpAdr)
            {
                freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
            }
            freeAllocatedSingleString(*keywordAdr);
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
            return FALSE;
        }

        if ((checkInputArgumentType(pvApiCtx, 4, sci_boolean)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrfullTextAdr);
            if (sciErr.iErr)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedSingleString(*keywordAdr);
                freeAllocatedSingleString(*languageAdr);
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of boolean at position 4.
            sciErr = getMatrixOfBoolean(pvApiCtx, piAddrfullTextAdr, &nbRow, &nbCol, &fullTextAdr);
            if (sciErr.iErr)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedSingleString(*keywordAdr);
                freeAllocatedSingleString(*languageAdr);
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), fname, 4);
                return 1;
            }

            if (nbRow*nbCol != 1)
            {
                if (helpAdr)
                {
                    freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
                }
                freeAllocatedSingleString(*keywordAdr);
                freeAllocatedSingleString(*languageAdr);
                Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 4);
                return FALSE;
            }
        }
        else
        {
            if (helpAdr)
            {
                freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
            }
            freeAllocatedSingleString(*keywordAdr);
            freeAllocatedSingleString(*languageAdr);
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 4);
            return FALSE;
        }

        ret = searchKeyword(helpAdr, nbRowHelp * nbColHelp, keywordAdr[0], languageAdr[0], *fullTextAdr == 1);

        if (helpAdr)
        {
            freeAllocatedMatrixOfString(nbRowHelp, nbColHelp, helpAdr);
        }

        freeAllocatedSingleString(*keywordAdr);
        freeAllocatedSingleString(*languageAdr);
    }
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), fname, 2, 4);
        return FALSE;
    }

    if (ret == 0)
    {
        return FALSE;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return TRUE;
}
/*--------------------------------------------------------------------------*/
