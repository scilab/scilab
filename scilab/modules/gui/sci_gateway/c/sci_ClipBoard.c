/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
* Copyright (C) 2008 - INRIA - Vincent COUVERT
* Copyright (C) 2010 - DIGITEO - Vincent COUVERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "gw_gui.h"
#include "stack-c.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "scilabmode.h"
#include "localization.h"
#include "IsAScalar.h"
#include "freeArrayOfString.h"
#include "CallClipboard.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_ClipBoard(char *fname, unsigned long l)
{
    static int l1 = 0, n1 = 0, m1 = 0;
    char *param1 = NULL, *param2 = NULL;

    Rhs = Max(0, Rhs);
    CheckRhs(0, 2);
    CheckLhs(0, 1);

    if ( getScilabMode() != SCILAB_NWNI )
    {
        /*--------------------*/
        /* clipboard("paste") */
        /*--------------------*/
        if (Rhs == 1)
        {
            if (GetType(1) == sci_strings)
            {
                GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
                param1 = cstk(l1);

                if ( ( strcmp(param1, "paste") == 0 ) || ( strcmp(param1, "pastespecial") == 0 ) )
                {
                    /* Use the Java clipboard (CallScilabBridge.java returns "" if clipbaord could not be read) */
                    char *output = getClipboardContents();


                    m1 = (int)strlen(output);
                    n1 = 1;

                    CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &m1, &n1, &output);
                    /* TO DO a delete [] and not a FREE */
                    FREE(output);
                    LhsVar(1) = Rhs + 1;

                    PutLhsVar();
                    return TRUE;
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "paste", "pastespecial");
                    return FALSE;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
                return FALSE;
            }
        }

        else if (Rhs == 2)
        {
            if (GetType(1) == sci_strings)
            {
                /* Get the first argument: should be "copy" or "do" */
                GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
                param1 = cstk(l1);

                if (n1 != 1)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "do", "copy");
                    return FALSE;
                }

                if (GetType(2) == sci_strings)
                {

                    /*-------------------------------------------*/
                    /* clipboard("do", {"paste","copy","empty"}) */
                    /*-------------------------------------------*/

                    /* @TODO : should be remplaced by an enum */
                    if ( strcmp(param1, "do") == 0 )
                    {
                        GetRhsVar(2, STRING_DATATYPE, &m1, &n1, &l1);
                        param2 = cstk(l1);

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
                            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 2, "copy", "paste", "empty");
                            return FALSE;
                        }

                        m1 = 0;
                        n1 = 0;
                        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
                        LhsVar(1) = Rhs + 1;
                        PutLhsVar();
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

                        GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &m2, &n2, &Str);

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
                                Scierror(999, _("%s: No more memory.\n"), fname);
                                return FALSE;
                            }

                            for (i = 0; i < m2; i++)
                            {
                                for (j = 0; j < n2; j++)
                                {
                                    SizeofTextToSendInClipboard = SizeofTextToSendInClipboard + (int)strlen(Str[j * m2 + i]) + (int)strlen("\n") + (int)strlen(" ");
                                    buffer[i * n2 + j] = strdup(Str[j * m2 + i]);
                                }
                            }

                            TextToSendInClipboard = (char*)MALLOC( (SizeofTextToSendInClipboard) * sizeof(char) );
                            if (TextToSendInClipboard == NULL)
                            {
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

                            freeArrayOfString(buffer, m2 * n2);

                            FREE(TextToSendInClipboard);
                            TextToSendInClipboard = NULL;
                        }

                        freeArrayOfString(Str, m2 * n2);
                        m1 = 0;
                        n1 = 0;
                        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
                        LhsVar(1) = Rhs + 1;
                        PutLhsVar();
                        return TRUE;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "do", "copy");
                        return FALSE;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
                    return FALSE;
                }
            }

            /*----------------------------------*/
            /* clipboard(fignum, {"EMF","DIB"}) */
            /*----------------------------------*/

            else if (GetType(1) == sci_matrix)
            {

                int num_win = -2;

                GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
                num_win = *istk(l1);

                if (m1*n1 != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                    return FALSE;
                }

                if (GetType(2) == sci_strings)
                {
                    GetRhsVar(2, STRING_DATATYPE, &m1, &n1, &l1);
                    param2 = cstk(l1);

                    if ( ( strcmp(param2, "EMF") == 0 ) || ( strcmp(param2, "DIB") == 0 ) )
                    {
                        if (num_win >= 0)
                        {
                            /* Call Java */
                            if ( strcmp(param2, "EMF") == 0)
                            {
                                /* @TODO create EMF */
                                copyFigureToClipBoard(num_win);
                            }
                            else
                            {
                                /* @TODO create DIB */
                                copyFigureToClipBoard(num_win);
                            }

                            m1 = 0;
                            n1 = 0;
                            CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
                            LhsVar(1) = Rhs + 1;
                            PutLhsVar();
                            return TRUE;
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong value for input argument #%d: Must be >= %d expected.\n"), fname, 1, 0);
                            return FALSE;
                        }

                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "EMF", "DIB");
                        return FALSE;
                    }

                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
                    return FALSE;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string or a real expected.\n"), fname, 1);
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
