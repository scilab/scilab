/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

#define NOMINMAX
#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"
#include <algorithm>

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "gw_gui.h"
#include "getScilabJavaVM.h"
#include "FigureList.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
int sci_printfigure(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int n1 = 0, m1 = 0;
    int num_win = -2;
    int status  = 0;

    int * piAddr1   = NULL;
    double* l1      = NULL;

    nbInputArgument(pvApiCtx) = std::max(0, nbInputArgument(pvApiCtx));
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (getScilabMode() != SCILAB_NWNI)
    {
        if (nbInputArgument(pvApiCtx) == 1)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &l1);
            if (sciErr.iErr)
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                printError(&sciErr, 0);
                return 1;
            }

            if (!isScalar(pvApiCtx, piAddr1))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }
            num_win = (int) * l1;

            if (num_win >= 0)
            {
                /* Call Java */
                try
                {
                    status = (int)CallScilabBridge::printFigure(getScilabJavaVM(), getFigureFromIndex(num_win), true, true); /* postscript mode and display dialog */
                }
                catch (const GiwsException::JniException & e)
                {
                    Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
                    return 1;
                }

                if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, status))
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }

                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
                return 0;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be >= %d expected.\n"), fname, 1, 0);
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Function not available in NWNI mode.\n"), fname);
        return 1;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
