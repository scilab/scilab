/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}

/*--------------------------------------------------------------------------*/
int sci_xend(char * fname, unsigned long fname_len)
{
    CheckInputArgument(pvApiCtx, 0, 0);

    const char* pstCurrentFigureReference = ScilabView::getCurrentFigure();

    if (pstCurrentFigureReference != NULL)
    {
        char* uid = strdup(pstCurrentFigureReference);

        if (uid)
        {
            char * ret = org_scilab_modules_graphic_export::Driver::end(getScilabJavaVM(), uid);

            deleteGraphicObject(uid);
            free(uid);

            if (*ret != '\0')
            {
                Scierror(999, _("%s: An error occurred: %s\n"), fname, ret);
                return 0;
            }
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
