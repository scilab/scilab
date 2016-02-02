/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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

#include "GiwsException.hxx"
#include "ScilabView.hxx"
#include "EditorManager.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
}


/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_editor;

/*--------------------------------------------------------------------------*/
int sci_useeditor(char *fname, void* pvApiCtx)
{
    int iFigureUid  = 0;
    int* piAddr     = NULL;
    int* piVar1     = NULL;
    int* piVar2     = NULL;
    int m1          = 0;
    int n1          = 0;
    int enable      = 0;
    int iErr        = 0;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr, &m1, &n1, &piVar1);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if (m1 * n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, m1 * n1);
        return 1;
    }

    try
    {
        iFigureUid = ScilabView::getFigureFromIndex(*piVar1);

        if (Rhs == 1)
        {
            enable = (int)(EditorManager::isModifyEnabled(getScilabJavaVM(), iFigureUid) == false);
        }
        else
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &m1, &n1, &piVar2);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 1;
            }

            if (m1 * n1 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 2);
                return 1;
            }
            enable = *piVar2;
        }

        if (enable)
        {
            EditorManager::enableModify(getScilabJavaVM(), iFigureUid);
        }
        else
        {
            EditorManager::disableModify(getScilabJavaVM(), iFigureUid);
        }
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return 1;
    }

    m1 = 1;
    n1 = 1;

    sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, &enable);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Can not create output argument #%d.\n"), fname, 1);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
