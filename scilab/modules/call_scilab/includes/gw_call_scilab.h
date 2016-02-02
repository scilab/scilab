/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
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
#ifndef __GW_CALL_SCILAB__
#define __GW_CALL_SCILAB__
/*--------------------------------------------------------------------------*/
#include "dynlib_call_scilab.h"
/*--------------------------------------------------------------------------*/
CALL_SCILAB_IMPEXP int gw_call_scilab(void);
/*--------------------------------------------------------------------------*/
CALL_SCILAB_IMPEXP int sci_fromjava(char *fname, void* pvApiCtx);
CALL_SCILAB_IMPEXP int sci_fromc(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CALL_SCILAB__ */
/*--------------------------------------------------------------------------*/

