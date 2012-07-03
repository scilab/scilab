/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
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
#include "InitUIMenu.h"
#include "scilabmode.h"
#include "FigureList.h"
#include "getConsoleIdentifier.h"
/*--------------------------------------------------------------------------*/
int sci_delmenu(char *fname, unsigned long fname_len)
{
    int nbRow = 0, nbCol = 0, stkAdr = 0;
    int nbRow1 = 0, nbCol1 = 0, stkAdr1 = 0;

    // Check parameter number
    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 1)
    {

        // Error message in not in standard mode (we need figure index)
        if (getScilabMode() != SCILAB_STD)
        {
            Scierror(999, _("%s: Figure number must be given when not in '%s' mode.\n"), fname, "STD");
            return FALSE;
        }

        // Unset a Menu of Scilab Main Window
        if (VarType(1) != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return FALSE;
        }

        GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);

        if (nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
            return FALSE;
        }

        DeleteMenuWithName(getConsoleIdentifier(), cstk(stkAdr));
    }
    else
    {
        // Unset a Menu a Scilab Graphic Window
        if (VarType(1) != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
            return FALSE;
        }
        GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
            return FALSE;
        }

        if (VarType(2) != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return FALSE;
        }
        GetRhsVar(2, STRING_DATATYPE, &nbRow1, &nbCol1, &stkAdr1);

        DeleteMenuWithName((char*)getFigureFromIndex((int)*stk(stkAdr)), cstk(stkAdr1));
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
