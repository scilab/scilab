/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
#include "scilabWrite.hxx"

extern "C"
{
#include "sciprint.h"
#include "Sciwarning.h"
#include "Scierror.h"
#include "configvariable_interface.h"
}

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

int Sciwarning(const char *msg, ...)
{
    int retval = 0;
    if (getWarningMode())
    {
        //print warning message
        char s_buf[bsiz];
        va_list ap;
        va_start(ap, msg);
        retval = vsnprintf(s_buf, bsiz - 1, msg, ap);
        if (retval < 0)
        {
            s_buf[bsiz - 1] = '\0';
        }
        va_end(ap);

        scilabWarning(s_buf);

        if (getWarningStop())
        {
            // Configuration variable WarningStop is set to true
            // Warning becomes an error and throws the ast exception
            // to retrieve the line the error was generated at.
            // Location(-1,-1,-1,-1) means the locations are unknown at the throw time,
            // this can be use to set the locations in the first catch which know them.
            throw ast::InternalError(_W("*** Execution stopped after a warning. ***\nSet warning(\"on\") to continue execution after a warning.\n"), 999, Location(-1,-1,-1,-1));
        }
    }
    return retval;
}
