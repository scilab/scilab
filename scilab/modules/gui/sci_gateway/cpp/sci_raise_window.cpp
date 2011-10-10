/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "gw_gui.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
int sci_raise_window(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0;

    CheckLhs(1, 1);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);

    try
    {
        CallScilabBridge::raiseWindow(getScilabJavaVM(), (int)(*stk(l1)));
    }
    catch(const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return FALSE;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return TRUE;
}

/*--------------------------------------------------------------------------*/
