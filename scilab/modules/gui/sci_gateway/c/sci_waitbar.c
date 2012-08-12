/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "freeArrayOfString.h"

#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
/*--------------------------------------------------------------------------*/
int sci_waitbar(char *fname, unsigned long fname_len)
{
    char *pWaitbarUID = NULL;

    int nbRow = 0, nbCol = 0;
    int nbRowMessage = 0, nbColMessage = 0;

    int fractionAdr = 0;
    char **messageAdr = NULL;
    int handleAdr = 0;
    int stkAdr = 0;

    int iValue = 0;

    unsigned long GraphicHandle = 0;

    CheckRhs(1, 3);
    CheckLhs(1, 1);

    if (Rhs == 1)
    {
        if (VarType(1) == sci_matrix)   /* Fraction to display */
        {
            GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else if (VarType(1) == sci_strings) /* Message to display */
        {
            GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real or a string expected.\n"), fname, 1);
            return FALSE;
        }

        /* Create a new waitbar */
        pWaitbarUID = createGraphicObject(__GO_WAITBAR__);

        GraphicHandle = getHandle(pWaitbarUID);

        if (fractionAdr != 0)
        {
            iValue = (int)(*(stk(fractionAdr)) * 100);
            setGraphicObjectProperty(pWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
        }
        else if (messageAdr != NULL)
        {
            setGraphicObjectProperty(pWaitbarUID, __GO_UI_MESSAGE__, getStringMatrixFromStack((size_t) messageAdr), jni_string_vector,
                                     nbColMessage * nbRowMessage);
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
        }
    }
    else if (Rhs == 2)
    {
        if (VarType(1) == sci_matrix && VarType(2) == sci_strings)  /* waitbar(x,mes) */
        {
            GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }
            GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);

            pWaitbarUID = createGraphicObject(__GO_WAITBAR__);

            GraphicHandle = getHandle(pWaitbarUID);

            iValue = (int)(*(stk(fractionAdr)) * 100);
            setGraphicObjectProperty(pWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
            setGraphicObjectProperty(pWaitbarUID, __GO_UI_MESSAGE__, getStringMatrixFromStack((size_t) messageAdr), jni_string_vector,
                                     nbColMessage * nbRowMessage);
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
        }
        else if (VarType(1) == sci_matrix && VarType(2) == sci_handles) /* waitbar(x,winId) */
        {
            GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }

            GetRhsVar(2, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle expected.\n"), fname, 2, "Waitbar");
                return FALSE;
            }

            GraphicHandle = (unsigned long) * hstk(handleAdr);
            pWaitbarUID = (char*)getObjectFromHandle(GraphicHandle);
            if (pWaitbarUID == NULL)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A valid '%s' handle expected.\n"), fname, 2, "Waitbar");
                return FALSE;
            }

            iValue = (int)(*(stk(fractionAdr)) * 100);
            setGraphicObjectProperty(pWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
        }
        else if (VarType(1) == sci_strings && VarType(2) == sci_handles)    /* waitbar(mes,winId) */
        {
            GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);

            GetRhsVar(2, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle expected.\n"), fname, 2, "Waitbar");
                return FALSE;
            }

            GraphicHandle = (unsigned long) * hstk(handleAdr);
            pWaitbarUID = (char*)getObjectFromHandle(GraphicHandle);
            if (pWaitbarUID == NULL)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A valid '%s' handle expected.\n"), fname, 2, "Waitbar");
                return FALSE;
            }

            setGraphicObjectProperty(pWaitbarUID, __GO_UI_MESSAGE__, getStringMatrixFromStack((size_t) messageAdr), jni_string_vector,
                                     nbColMessage * nbRowMessage);
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
        }
        else
        {
            Scierror(999, _("%s: Wrong input arguments: '%s', '%s' or '%s' expected.\n"), fname, "(x, mes)", "(x, winId)", "(mes, winId)");
            return FALSE;
        }
    }
    else
    {
        if (VarType(1) == sci_matrix)   /* Fraction */
        {
            GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &fractionAdr);
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

        if (VarType(2) == sci_strings)  /* Message */
        {
            GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return FALSE;
        }

        if (VarType(3) == sci_handles)  /* Handle */
        {
            GetRhsVar(3, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
            if (nbRow * nbCol != 1)
            {
                freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle expected.\n"), fname, 3, "Waitbar");
                return FALSE;
            }
        }
        else
        {
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 3, "Waitbar");
            return FALSE;
        }

        GraphicHandle = (unsigned long) * hstk(handleAdr);
        pWaitbarUID = (char*)getObjectFromHandle(GraphicHandle);
        if (pWaitbarUID != NULL)
        {
            iValue = (int)(*(stk(fractionAdr)) * 100);
            setGraphicObjectProperty(pWaitbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
            setGraphicObjectProperty(pWaitbarUID, __GO_UI_MESSAGE__, getStringMatrixFromStack((size_t) messageAdr), jni_string_vector,
                                     nbColMessage * nbRowMessage);
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
        }
        else
        {
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
            Scierror(999, _("%s: Wrong value for input argument #%d: A valid '%s' handle expected.\n"), fname, 3, "Waitbar");
            return 0;
        }
    }

    if (Lhs == 1)
    {
        nbRow = 1;
        nbCol = 1;
        CreateVar(Rhs + 1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
        *hstk(stkAdr) = GraphicHandle;
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        LhsVar(1) = 0;
    }

    PutLhsVar();

    return TRUE;

}

/*--------------------------------------------------------------------------*/
