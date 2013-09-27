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

#include "DatatipCreate.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BOOL.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatip_set_interp(char *fname, unsigned long fname_len)
{

    int nbRow = 0, nbCol = 0, stkAdr = 0;
    char* datatipUID = NULL;
    int iType = 0;
    int *piType = &iType;
    int m1 = 0, n1 = 0;
    int interpIntBool = 0;
    bool interpMode;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 2)
    {

        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
        datatipUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {

            getGraphicObjectProperty(datatipUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_DATATIP__)
            {

                if (checkInputArgumentType(pvApiCtx, 2, sci_boolean))
                {

                    GetRhsVar(2, MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1, &interpIntBool);

                    if (m1 * n1 != 1)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 2);
                        return FALSE;
                    }
                    else
                    {
                        interpMode = BOOLtobool(*istk(interpIntBool));
                        DatatipCreate::datatipSetInterp(getScilabJavaVM(), (char*)datatipUID, interpMode);
                    }

                }
                else
                {

                    Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
                    return FALSE;

                }

            }
            else
            {

                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
                return FALSE;

            }

        }
        else
        {

            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
            return FALSE;

        }

    }
    else
    {

        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return FALSE;

    }

    LhsVar(1) = 0;
    PutLhsVar();

    return TRUE;

}