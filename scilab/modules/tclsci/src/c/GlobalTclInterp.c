/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** This is to provide a global Tcl interpreter
 ** with Locking system in order it does not have any
 ** concurrential access
 **/

#include "GlobalTclInterp.h"
#include "Thread_Wrapper.h"

Tcl_Interp	*__globalTclInterp;
__threadLock	singleInterpAccess;

/*
** Initialize the global interpreter.
*/
void initTclInterp(void) {
  __InitLock(&singleInterpAccess);
  __Lock(&singleInterpAccess);
  __globalTclInterp = Tcl_CreateInterp();
  __UnLock(&singleInterpAccess);
}

/*
** Delete the global interpreter.
*/
void deleteTclInterp(void) {
  __globalTclInterp = NULL;

}

/*
** Get the Global Interpreter
*/
Tcl_Interp *getTclInterp(void) {
  __Lock(&singleInterpAccess);
  return __globalTclInterp;
}

/*
** Release Tcl Interp after use.
*/
void releaseTclInterp(void) {
  __UnLock(&singleInterpAccess);
}

/*
** Check if global interp exists.
*/
BOOL existsGlobalInterp(void) {
  if(__globalTclInterp != NULL) {
    return TRUE;
  }
  return FALSE;
}
/*
** Check if slave interp exists.
*/
BOOL existsSlaveInterp(char *name) {
  if (Tcl_GetSlave(getTclInterp(), name) != NULL) {
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
Tcl_Interp *requestTclInterp(void) {
  return __globalTclInterp;
}
