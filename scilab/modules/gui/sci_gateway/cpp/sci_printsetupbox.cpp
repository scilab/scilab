/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
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

/*--------------------------------------------------------------------------*/
#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "api_scilab.h"
#include "localization.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "gw_gui.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
int sci_printsetupbox(char *fname, void* pvApiCtx)
{
    int paramoutINT = 0;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 0, 1);

    try
    {
        paramoutINT = (int)CallScilabBridge::pageSetup(getScilabJavaVM());
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return 1;
    }

    if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, paramoutINT))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
