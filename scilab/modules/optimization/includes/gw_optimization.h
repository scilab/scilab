/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __GW_OPTIM__
#define __GW_OPTIM__
/*--------------------------------------------------------------------------*/
#include "dynlib_optimization.h"
/*--------------------------------------------------------------------------*/
OPTIMIZATION_IMPEXP int gw_optimization(void);
/*--------------------------------------------------------------------------*/
OPTIMIZATION_IMPEXP int sci_qld(char *fname,  void* pvApiCtx);
OPTIMIZATION_IMPEXP int sci_qp_solve(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_OPTIM__ */
/*--------------------------------------------------------------------------*/

