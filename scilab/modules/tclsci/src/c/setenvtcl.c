/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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
#include "TCL_Global.h"
#include "GlobalTclInterp.h"
#include "setenvtcl.h"
/*--------------------------------------------------------------------------*/
int setenvtcl(char *string, char *value)
{
    char MyTclCommand[2048];

    sprintf(MyTclCommand, "env(%s)", string);

    if (getTclInterp() == NULL)
    {
        releaseTclInterp();
        return FALSE;
    }
    releaseTclInterp();
    if ( !Tcl_SetVar(getTclInterp(), MyTclCommand, value, TCL_GLOBAL_ONLY) )
    {
        releaseTclInterp();
        return FALSE;
    }
    else
    {
        releaseTclInterp();
        return TRUE;
    }
}
/*--------------------------------------------------------------------------*/
