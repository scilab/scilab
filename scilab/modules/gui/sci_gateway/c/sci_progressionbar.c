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
int sci_progressionbar(char *fname, unsigned long fname_len)
{
    char *pProgressionbarUID = NULL;

    int nbRow = 0, nbCol = 0;
    int nbRowMessage = 0, nbColMessage = 0;

    char **messageAdr = NULL;
    int handleAdr = 0;
    int stkAdr = 0;

    int iValue = 0;

    unsigned long GraphicHandle = 0;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 1)
    {
        if (VarType(1) == sci_handles)  /* ProgressionBar to update */
        {
            GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else if (VarType(1) == sci_strings) /* Message to display */
        {
            GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle or a string expected.\n"), fname, 1);
            return FALSE;
        }

        if (handleAdr == 0)
        {
            /* Create a new ProgressionBar */
            pProgressionbarUID = createGraphicObject(__GO_PROGRESSIONBAR__);

            GraphicHandle = getHandle(pProgressionbarUID);

            setGraphicObjectProperty(pProgressionbarUID, __GO_UI_MESSAGE__, getStringMatrixFromStack((size_t) messageAdr), jni_string_vector,
                                     nbColMessage * nbRowMessage);
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
        }
        else
        {
            GraphicHandle = (unsigned long)*hstk(handleAdr);
            pProgressionbarUID = (char*)getObjectFromHandle(GraphicHandle);

            setGraphicObjectProperty(pProgressionbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
        }
    }
    else if (Rhs == 2)
    {
        if (VarType(1) == sci_handles && VarType(2) == sci_strings) /* progressionbar(id,mes) */
        {
            GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &handleAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }
            GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRowMessage, &nbColMessage, &messageAdr);

            GraphicHandle = (unsigned long)*hstk(handleAdr);
            pProgressionbarUID = (char*)getObjectFromHandle(GraphicHandle);

            setGraphicObjectProperty(pProgressionbarUID, __GO_UI_VALUE__, &iValue, jni_int, 1);
            setGraphicObjectProperty(pProgressionbarUID, __GO_UI_MESSAGE__, getStringMatrixFromStack((size_t) messageAdr), jni_string_vector,
                                     nbColMessage * nbRowMessage);
            freeArrayOfString(messageAdr, nbColMessage * nbRowMessage);
        }
        else
        {
            Scierror(999, _("%s: Wrong input arguments: '%s' expected.\n"), fname, "(id, mes)");
            return FALSE;
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
