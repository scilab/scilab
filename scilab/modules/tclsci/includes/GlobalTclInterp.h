/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

#ifndef __GLOBALTCLINTERP_H___
#define __GLOBALTCLINTERP_H___

#include <tcl.h>
#include <tk.h>
#include "BOOL.h"


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
