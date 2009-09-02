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

#include <tcl.h>
#include <tk.h>
#include "dynlib_tclsci.h"
#include "BOOL.h"


/*
** Initialize the global interpreter.
*/
TCLSCI_IMPEXP void initTclInterp(void);

/*
** Delete the global interpreter.
*/
TCLSCI_IMPEXP void deleteTclInterp(void);

/*
** Get the Global Interpreter
*/
TCLSCI_IMPEXP Tcl_Interp *getTclInterp(void);

/*
** Release Tcl Interp after use.
*/
TCLSCI_IMPEXP void releaseTclInterp(void);

/*
** Get the Global Interpreter
** without any Control
*/
TCLSCI_IMPEXP Tcl_Interp *requestTclInterp(void);

/*
** Check if global interp exists.
*/
TCLSCI_IMPEXP BOOL existsGlobalInterp(void);

/*
** Check if slave interp exists.
*/
TCLSCI_IMPEXP BOOL existsSlaveInterp(char *name);

#endif /* !__GLOBALTCLINTERP_H___ */
