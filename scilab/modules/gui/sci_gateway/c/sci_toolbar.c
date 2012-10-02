/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "Toolbar.h"
#include "FigureList.h"
#include "HandleManagement.h"
#include "GetProperty.h"
#include "freeArrayOfString.h"
#if _MSC_VER
#include "strdup_windows.h"
#endif
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getConsoleIdentifier.h"
/*--------------------------------------------------------------------------*/
int sci_toolbar(char *fname, unsigned long l)
{
    static int stkAdr = 0, nbCol = 0, nbRow = 0;

    char *Output = NULL;

    char **param = NULL;

    int figNum = -2;

    char *pParentUID = NULL;
    int iParentType = -1;
    int *piParentType = &iParentType;

    CheckRhs(1, 2);
    CheckLhs(0, 1);

    /* Figure number */
    if (GetType(1) == sci_matrix)
    {
        GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &nbRow, &nbCol, &stkAdr);
        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
            return FALSE;
        }
        figNum = *istk(stkAdr);

        if (figNum < -1)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be > %d expected.\n"), fname, 1, -1);
            return FALSE;
        }

        if (figNum != -1)       /* Check that the figure exists */
        {
            if (getFigureFromIndex(figNum) == NULL)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 'Graphic Window Number %d' does not exist.\n"), fname, 1, figNum);
                return FALSE;
            }
        }

        if (figNum == -1)
        {
            pParentUID = getConsoleIdentifier();
        }
        else
        {
            pParentUID = (char*)getFigureFromIndex(figNum);
        }
    }
    else if (GetType(1) == sci_handles)
    {
        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }
        pParentUID = (char*)getObjectFromHandle((long) * hstk(stkAdr));

        if (pParentUID == NULL)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: this handle does not exist.\n"), fname, 1);
            return FALSE;
        }

        getGraphicObjectProperty(pParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
        if (iParentType == __GO_FIGURE__)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real or a Figure handle expected.\n"), fname, 1);
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real or Figure handle expected.\n"), fname, 1);
        return FALSE;
    }

    if (Rhs == 2)               /* New status */
    {
        if ((GetType(2) == sci_strings))
        {
            GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &param);
            if (nbRow * nbCol != 1)
            {
                freeArrayOfString(param, nbRow * nbCol);
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
                return FALSE;
            }

            if ((strcmp(param[0], "off") == 0) || (strcmp(param[0], "on") == 0))
            {
                setToolbarVisible(pParentUID, strcmp(param[0], "on") == 0);
                freeArrayOfString(param, nbRow * nbCol);
            }
            else
            {
                freeArrayOfString(param, nbRow * nbCol);
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "on", "off");
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return FALSE;
        }
    }

    /* Returned value */
    if (isToolbarVisible(pParentUID))
    {
        Output = strdup("on");
    }
    else
    {
        Output = strdup("off");
    }

    nbCol = 1;
    nbRow = (int)strlen(Output);
    CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &nbRow, &nbCol, &Output);
    LhsVar(1) = Rhs + 1;

    if (Output)
    {
        FREE(Output);
        Output = NULL;
    }

    PutLhsVar();
    return TRUE;
}

/*--------------------------------------------------------------------------*/
