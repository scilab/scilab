/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "sci_mode.h"
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
