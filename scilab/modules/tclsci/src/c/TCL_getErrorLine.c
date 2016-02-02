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

#include "TCL_Global.h"

/*--------------------------------------------------------------------------*/
int TCL_getErrorLine(Tcl_Interp *interp)
{
#ifdef TCL_MAJOR_VERSION
#ifdef TCL_MINOR_VERSION
#if TCL_MAJOR_VERSION >= 8
#if TCL_MINOR_VERSION >= 6
    return Tcl_GetErrorLine(interp);
#else
    /* 8.5 */
    return interp->errorLine;
#endif
#endif
#else
    return interp->errorLine;
#endif
#else
    /* USE_INTERP_ERRORLINE does normally not need to be defined
       since it's only used by Tcl >= 8.6 to restore access to
       interp->errorLine, but I define it anyway in case one of the
       ifdef above fails
    */
#define USE_INTERP_ERRORLINE
    return interp->errorLine;
#endif
}
/*--------------------------------------------------------------------------*/
