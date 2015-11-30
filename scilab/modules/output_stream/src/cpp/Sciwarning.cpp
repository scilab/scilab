/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2015 - Pierre-Aime Agnel
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "scilabexception.hxx"

extern "C"
{
#include "sciprint.h"
#include "Sciwarning.h"
#include "configvariable_interface.h"
}


int Sciwarning(const char *msg, ...)
{
    int retval = 0;
    if (getWarningMode())
    {
        //print warning message
        va_list ap;
        va_start(ap, msg);
        scivprint(msg, ap);
        va_end(ap);

        if (getWarningStop())
        {
            // Configuration variable WarningStop is set to true
            // Warning becomes an error and throws the ast exception
            // to retrieve the line the error was generated at
            throw ast::InternalError(_("*** Execution stopped after a warning. ***\nSet warning(\"on\") to continue execution after a warning.\n"));
        }
    }
    return retval;
}
