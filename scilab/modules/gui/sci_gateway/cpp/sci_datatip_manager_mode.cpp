/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro Arthur dos S. Souza
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

int sci_datatip_manager_mode(char *fname, unsigned long fname_len)
{
    int n = 0, nbRow = 0, nbCol = 0, l1 = 0, i = 0;
    int* piAddr	= NULL;;
    int stkAdr = 0;
    const char* pstFigureUID = NULL;
    int * pbValue = NULL;
    bool enabled = false;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 0, 2);
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
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return FALSE;
        }
        if (checkInputArgumentType(pvApiCtx, 1, sci_boolean))
        {

            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbValue);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return FALSE;
            }   
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
                return FALSE;
            }
            pstFigureUID = getCurrentFigure();
            if (pstFigureUID)
            {

                DatatipManager::setEnabled(getScilabJavaVM(), pstFigureUID, (bool)pbValue[0]);
            }
        } 
        else if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }
            pstFigureUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));
            if (pstFigureUID)
            {
                enabled = DatatipManager::isEnabled(getScilabJavaVM(), pstFigureUID);
                DatatipManager::setEnabled(getScilabJavaVM(), pstFigureUID, (!enabled));
            }
        }
    }
    else if (Rhs == 2)
    {
        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }
        pstFigureUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return FALSE;
        }
        sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbValue);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return FALSE;
        }   
        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
            return FALSE;
        }
        DatatipManager::setEnabled(getScilabJavaVM(), pstFigureUID, (bool)pbValue[0]);
    }
    else
    {
        Scierror(999, _("%s: Wrong number for input argument: %d expected.\n"), fname, 2);
        return FALSE;
    }


    LhsVar(1) = 0;
    PutLhsVar();

    return TRUE;
}

