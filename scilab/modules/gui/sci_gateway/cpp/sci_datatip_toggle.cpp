/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DatatipManager.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatip_toggle(char *fname, unsigned long fname_len)
{
    const char* pstFigureUID = NULL;
    bool enabled = false;
    int nbRow = 0, nbCol = 0, stkAdr = 0;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (Rhs == 0)
    {
        pstFigureUID = getCurrentFigure();
        if (pstFigureUID)
        {
            enabled = DatatipManager::isEnabled(getScilabJavaVM(), pstFigureUID);
            DatatipManager::setEnabled(getScilabJavaVM(), pstFigureUID, (!enabled));
        }
    }
    else if (Rhs == 1)
    {
        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
            return FALSE;
        }
        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            pstFigureUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));
            enabled = DatatipManager::isEnabled(getScilabJavaVM(), pstFigureUID);
            DatatipManager::setEnabled(getScilabJavaVM(), pstFigureUID, (!enabled));

        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number for input argument: %d or %d expected.\n"), fname, 0, 1);
        return FALSE;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return TRUE;
}