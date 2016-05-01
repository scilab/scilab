/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "console_gw.hxx"
#include "function.hxx"
#include "bool.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "ConsoleIsWaitingForInput.h"
#include "configvariable_interface.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_iswaitingforinput(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    BOOL res = FALSE;

    if (getScilabMode() == SCILAB_STD)
    {
        res = ConsoleIsWaitingForInput();
    }
    else
    {
        Scierror(999, _("%s: Not implemented in this mode.\n"), "iswaitingforinput");
        return types::Function::Error;
    }

    out.push_back(new types::Bool(res));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
