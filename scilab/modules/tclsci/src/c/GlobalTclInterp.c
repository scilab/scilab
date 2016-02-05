/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

/**
 ** This is to provide a global Tcl interpreter
 ** with Locking system in order it does not have any
 ** concurrential access
 **/

#include "GlobalTclInterp.h"
#include "Thread_Wrapper.h"

Tcl_Interp	*__globalTclInterp = NULL;
__threadLock	singleInterpAccess;

static int initialized = 0;

/*
** Initialize the global interpreter.
*/
void initTclInterp(void)
{
    __InitLock(&singleInterpAccess);
    __Lock(&singleInterpAccess);
    __globalTclInterp = Tcl_CreateInterp();
    __UnLock(&singleInterpAccess);

    /* indicate that we have in fact initialized our mutex */
    initialized = 1;
}

/*
** Delete the global interpreter.
*/
void deleteTclInterp(void)
{
    __globalTclInterp = NULL;

}

/*
** Get the Global Interpreter
*/
Tcl_Interp *getTclInterp(void)
{
    if (! initialized )
    {
        return NULL;
    }
    __Lock(&singleInterpAccess);
    return __globalTclInterp;
}

/*
** Release Tcl Interp after use.
*/
void releaseTclInterp(void)
{
    /* only try to unlock if we have already initialized our threading system */
    if ( initialized )
    {
        __UnLock(&singleInterpAccess);
    }
}

/*
** Check if global interp exists.
*/
BOOL existsGlobalInterp(void)
{
    if (__globalTclInterp != NULL)
    {
        return TRUE;
    }
    return FALSE;
}
/*
** Check if slave interp exists.
*/
BOOL existsSlaveInterp(char *name)
{
    if (Tcl_GetSlave(getTclInterp(), name) != NULL)
    {
        releaseTclInterp();
        return TRUE;
    }
    releaseTclInterp();
    return FALSE;
}

/*
** WARNING
** Get the Global Interpreter
** without any lock
** should not be used ouside of the main
** TCL loop (the one containing update)
*/
Tcl_Interp *requestTclInterp(void)
{
    return __globalTclInterp;
}
