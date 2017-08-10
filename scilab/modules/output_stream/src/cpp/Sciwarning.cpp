/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2015 - Pierre-Aime Agnel
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
