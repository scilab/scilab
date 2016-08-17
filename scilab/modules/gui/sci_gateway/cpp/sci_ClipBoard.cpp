/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
* Copyright (C) 2008 - INRIA - Vincent COUVERT
* Copyright (C) 2010 - DIGITEO - Vincent COUVERT
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
#include <stdlib.h>
#include <stdio.h>
#include "gw_gui.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "sciprint.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "CallClipboard.h"
#include "os_string.h"
#include "FigureList.h"
}

/*--------------------------------------------------------------------------*/
int sci_ClipBoard(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddr1   = NULL;
    int* piAddrl1   = NULL;
    int* piAddrStr  = NULL;
    double* pdbll1  = NULL;
    int* pil1       = NULL;

    static int n1 = 0, m1 = 0;
    char* param1 = NULL;
    char* param2 = NULL;

    nbInputArgument(pvApiCtx) = Max(0, nbInputArgument(pvApiCtx));
    CheckInputArgument(pvApiCtx, 0, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if ( getScilabMode() != SCILAB_NWNI )
    {
        /*--------------------*/
        /* clipboard("paste") */
        /*--------------------*/
        if (nbInputArgument(pvApiCtx) == 1)
        {
            if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of double at position 1.
                if (getAllocatedSingleString(pvApiCtx, piAddrl1, &param1))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
                    return 1;
                }

                if ( ( strcmp(param1, "paste") == 0 ) || ( strcmp(param1, "pastespecial") == 0 ) )
                {
                    /* Use the Java clipboard (CallScilabBridge.java returns "" if clipboard could not be read) */
                    char *output = getClipboardContents();


                    m1 = (int)strlen(output);
                    n1 = 1;

                    if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, output))
                    {
                        freeAllocatedSingleString(param1);
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        return 1;
                    }

                    delete[] output;
                    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

                    freeAllocatedSingleString(param1);
                    ReturnArguments(pvApiCtx);
                    return TRUE;
                }
                else
                {
                    freeAllocatedSingleString(param1);

                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "paste", "pastespecial");
                    return FALSE;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else if (nbInputArgument(pvApiCtx) == 2)
        {
            if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
            {
                /* Get the first argument: should be "copy" or "do" */
                sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of double at position 1.
                if (getAllocatedSingleString(pvApiCtx, piAddrl1, &param1))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
                    return 1;
                }

                if (( strcmp(param1, "do") != 0 ) && ( strcmp(param1, "copy") != 0 ))
                {
                    freeAllocatedSingleString(param1);
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "do", "copy");
                    return FALSE;
                }

                if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
                {

                    /*-------------------------------------------*/
                    /* clipboard("do", {"paste","copy","empty"}) */
                    /*-------------------------------------------*/

                    /* @TODO : should be remplaced by an enum */
                    if ( strcmp(param1, "do") == 0 )
                    {
                        freeAllocatedSingleString(param1);

                        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl1);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            return 1;
                        }

                        // Retrieve a matrix of double at position 2.
                        if (getAllocatedSingleString(pvApiCtx, piAddrl1, &param2))
                        {
                            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                            return 1;
                        }

                        if ( strcmp(param2, "paste") == 0 )
                        {
                            /* Call Java to do the job */
                            pasteClipboardIntoConsole();
                        }
                        else if ( strcmp(param2, "copy") == 0 )
                        {
                            /* Call Java to do the job */
                            copyConsoleSelection();
                        }
                        else if ( strcmp(param2, "empty") == 0 )
                        {
                            /* Call Java to do the job */
                            emptyClipboard();
                        }
                        else
                        {
                            freeAllocatedSingleString(param2);
                            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 2, "copy", "paste", "empty");
                            return FALSE;
                        }

                        m1 = 0;
                        n1 = 0;

                        freeAllocatedSingleString(param2);

                        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, &pdbll1);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            return 1;
                        }

                        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                        ReturnArguments(pvApiCtx);
                        return TRUE;
                    }

                    /*-------------------------*/
                    /* clipboard("copy", data) */
                    /*-------------------------*/

                    else if ( strcmp(param1, "copy") == 0 )
                    {
                        char *TextToPutInClipboard = NULL;
                        char **Str = NULL;
                        int m2 = 0, n2 = 0;

                        freeAllocatedSingleString(param1);

                        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrStr);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            return 1;
                        }

                        // Retrieve a matrix of string at position 2.
                        if (getAllocatedMatrixOfString(pvApiCtx, piAddrStr, &m2, &n2, &Str))
                        {
                            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                            return 1;
                        }

                        if (m2 * n2 == 1) /* Single line copy */
                        {
                            TextToPutInClipboard = Str[0];
                            /* Call Java to do the job */
                            setClipboardContents(TextToPutInClipboard);
                        }
                        else /* Multi-line copy */
                        {
                            int i = 0, j = 0, l2 = 0;
                            char *TextToSendInClipboard = NULL;
                            int SizeofTextToSendInClipboard = 0;
                            char **buffer = (char**)MALLOC( (m2 * n2) * sizeof(char *) );
                            if (buffer == NULL)
                            {

                                freeAllocatedMatrixOfString(m2, n2, Str);
                                Scierror(999, _("%s: No more memory.\n"), fname);
                                return FALSE;
                            }

                            for (i = 0; i < m2; i++)
                            {
                                for (j = 0; j < n2; j++)
                                {
                                    SizeofTextToSendInClipboard = SizeofTextToSendInClipboard + (int)strlen(Str[j * m2 + i]) + (int)strlen("\n") + (int)strlen(" ");
                                    buffer[i * n2 + j] = os_strdup(Str[j * m2 + i]);
                                }
                            }

                            TextToSendInClipboard = (char*)MALLOC( (SizeofTextToSendInClipboard) * sizeof(char) );
                            if (TextToSendInClipboard == NULL)
                            {
                                freeAllocatedMatrixOfString(m2, n2, Str);
                                Scierror(999, _("%s: No more memory.\n"), fname);
                                freeArrayOfString(buffer, m2 * n2);

                                return FALSE;
                            }

                            strcpy(TextToSendInClipboard, "");

                            for (i = 0; i < m2; i++)
                            {
                                for (j = 0; j < n2; j++)
                                {
                                    strcat(TextToSendInClipboard, buffer[l2++]);
                                    strcat(TextToSendInClipboard, " ");
                                }
                                if ( i != (m2 - 1) )
                                {
                                    strcat(TextToSendInClipboard, "\n");
                                }
                            }

                            /* Call Java to do the job */
                            setClipboardContents(TextToSendInClipboard);

                            FREE(buffer);
                            buffer = NULL;
                            freeArrayOfString(buffer, m2 * n2);
                            FREE(TextToSendInClipboard);
                            TextToSendInClipboard = NULL;
                        }

                        freeAllocatedMatrixOfString(m2, n2, Str);
                        m1 = 0;
                        n1 = 0;

                        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, &pdbll1);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            return 1;
                        }

                        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                        ReturnArguments(pvApiCtx);
                        return TRUE;
                    }
                }
                else
                {
                    freeAllocatedSingleString(param1);
                    Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
                    return FALSE;
                }
            }

            /*----------------------------------*/
            /* clipboard(fignum, {"EMF","DIB"}) */
            /*----------------------------------*/

            else if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))
            {

                int num_win = -2;

                sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of double at position 1.
                sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr1, &m1, &n1, &pil1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                    return 1;
                }

                num_win = pil1[0];

                if (m1 * n1 != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                    return FALSE;
                }

                if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl1);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    // Retrieve a matrix of double at position 2.
                    if (getAllocatedSingleString(pvApiCtx, piAddrl1, &param2))
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                        return 1;
                    }

                    if ( ( strcmp(param2, "EMF") == 0 ) || ( strcmp(param2, "DIB") == 0 ) )
                    {
                        if (num_win >= 0)
                        {
                            /* Call Java */
                            if ( strcmp(param2, "EMF") == 0)
                            {
                                /* @TODO create EMF */
                                copyFigureToClipBoard(getFigureFromIndex(num_win));
                            }
                            else
                            {
                                /* @TODO create DIB */
                                copyFigureToClipBoard(getFigureFromIndex(num_win));
                            }

                            m1 = 0;
                            n1 = 0;

                            freeAllocatedSingleString(param2);

                            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, &pdbll1);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                return 1;
                            }
                            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                            ReturnArguments(pvApiCtx);
                            return TRUE;
                        }
                        else
                        {
                            freeAllocatedSingleString(param2);
                            Scierror(999, _("%s: Wrong value for input argument #%d: Must be >= %d expected.\n"), fname, 1, 0);
                            return FALSE;
                        }

                    }
                    else
                    {
                        freeAllocatedSingleString(param2);
                        Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "EMF", "DIB");
                        return FALSE;
                    }

                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
                    return FALSE;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string or real expected.\n"), fname, 1);
                return FALSE;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Function not available in NWNI mode.\n"), fname);
        return FALSE;
    }

    return FALSE;
}
/*--------------------------------------------------------------------------*/
