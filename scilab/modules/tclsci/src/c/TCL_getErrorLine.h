/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008- Francois VOGEL
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

#ifndef __TCL_GETERRORLINE_H__
#define __TCL_GETERRORLINE_H__

#include "TCL_Global.h"

/**
* TCL_getErrorLine
* Wrapper around interp->errorLine - See Tcl TIP #336 and Scilab bug 3877
* @param[in] interp: pointer on a Tcl interpreter
* @return int containing the line number where the error occurred
*/
int TCL_getErrorLine(Tcl_Interp *interp);

#endif /* __TCL_GETERRORLINE_H__ */
/*--------------------------------------------------------------------------*/
