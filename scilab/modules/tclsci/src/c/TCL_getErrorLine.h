/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008- Francois VOGEL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
