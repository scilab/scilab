/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
