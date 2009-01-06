/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008- Francois VOGEL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TCL_Global.h"

/*--------------------------------------------------------------------------*/
int TCL_getErrorLine(Tcl_Interp *interp)
{
#ifdef TCL_MAJOR_VERSION
#ifdef TCL_MINOR_VERSION
#if TCL_MAJOR_VERSION >= 8
#if TCL_MINOR_VERSION >= 6
    return Tcl_GetErrorLine(interp);
#endif
#endif
#endif
#endif
/* USE_INTERP_ERRORLINE does normally not need to be defined
   since it's only used by Tcl >= 8.6 to restore access to
   interp->errorLine, but I define it anyway in case one of the
   ifdef above fails
*/
#define USE_INTERP_ERRORLINE
    return interp->errorLine;
}
/*--------------------------------------------------------------------------*/
