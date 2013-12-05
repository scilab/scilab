/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "StartPlotBrowser.hxx"
#include "GiwsException.hxx"
#include "ScilabView.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "FigureList.h"
}


/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_plotbrowser;

/*--------------------------------------------------------------------------*/
int sci_plotbrowser(char *fname, unsigned long fname_len)
{
    int iFigureUid  = 0;
    int* piAddr     = NULL;
    int* piData     = NULL;
    int m1          = 0;
    int n1          = 0;
    int iErr        = 0;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr, &m1, &n1, &piData);
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
        iFigureUid = ScilabView::getFigureFromIndex(piData[0]);

        if (!sciIsExistingFigure(piData[0]))
        {
            Scierror(999, "%s: Figure with figure_id %d does not exist.\n", fname, piData[0]);
            return 1;
        }

        StartPlotBrowser::plotBrowser(getScilabJavaVM(), iFigureUid);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arised:\n%s"), fname, e.whatStr().c_str());
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
