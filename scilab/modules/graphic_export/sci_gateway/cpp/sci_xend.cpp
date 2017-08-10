/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#include "ScilabView.hxx"
#include "SwingView.hxx"
#include "Driver.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "deleteGraphicObject.h"

#include "gw_graphic_export.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/
int sci_xend(char * fname, void *pvApiCtx)
{
    CheckInputArgument(pvApiCtx, 0, 0);

    int iCurrentFigureReference = ScilabView::getCurrentFigure();

    if (iCurrentFigureReference != 0)
    {
        int uid = iCurrentFigureReference;

        if (uid)
        {
            char * ret = org_scilab_modules_graphic_export::Driver::end(getScilabJavaVM(), uid);

            deleteGraphicObject(uid);

            if (*ret != '\0')
            {
                Scierror(999, _("%s: An error occurred: %s\n"), fname, ret);
                delete[] ret;
                return 0;
            }
            delete[] ret;
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
