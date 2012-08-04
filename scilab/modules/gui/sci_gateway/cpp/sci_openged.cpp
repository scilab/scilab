/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "StartGED.hxx"
#include "GiwsException.hxx"
#include "ScilabView.hxx"

extern "C"
{
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "BOOL.h"
#include"MALLOC.h"
#include "FigureList.h"
}


/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_ged;

/*--------------------------------------------------------------------------*/
int sci_openged(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0, l2 = 0;
    int *status = NULL;
    char const * figureUid;
    bool enable;

    CheckLhs(0, 1);
    CheckRhs(1, 1);

    if (VarType(1) != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), fname, 1);
        return FALSE;
    }

    GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);

    if (m1 * n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A integer expected.\n"), fname, m1 * n1);
        return FALSE;
    }


    try
    {
        int figureID = *istk(l1);
        figureUid = ScilabView::getFigureFromIndex(figureID);

        if (!sciIsExistingFigure(figureID))
        {
            Scierror(999, "%s: Figure with figure_id %d does not exist.\n", fname, figureID);
            return -1;
        }

        StartGED::quickGED(getScilabJavaVM(), (char*)figureUid);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return FALSE;
    }

    if ((status = (int *)MALLOC(sizeof(int))) == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname, 0);
        return FALSE;
    }

    status[0] = booltoBOOL(enable);

    m1 = 1;
    n1 = 1;
    CreateVarFromPtr(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1, &status);

    FREE(status);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return TRUE;
}

/*--------------------------------------------------------------------------*/
