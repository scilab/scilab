/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GLOBALTCLINTERP_H___
#define __GLOBALTCLINTERP_H___

#include "tcl.h"
#include "tk.h"
#include "machine.h"


/*
** Initialize the global interpreter.
*/
void initTclInterp(void);

/*
** Delete the global interpreter.
*/
void deleteTclInterp(void);

/*
** Get the Global Interpreter
*/
Tcl_Interp *getTclInterp(void);

/*
** Release Tcl Interp after use.
*/
void releaseTclInterp(void);

/*
** Get the Global Interpreter
** without any Control
*/
Tcl_Interp *requestTclInterp(void);

/*
** Check if global interp exists.
*/
BOOL existsGlobalInterp(void);

/*
** Check if slave interp exists.
*/
BOOL existsSlaveInterp(char *name);

#endif /* !__GLOBALTCLINTERP_H___ */
