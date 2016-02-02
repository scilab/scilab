
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

#ifndef __ERRMDS_H__
#define __ERRMDS_H__

#include "machine.h"
#include "dynlib_differential_equations.h"

/* routine used by fortran */

/**
* this routine extract error modes out of errct variable
* @param imess : if 0 error message is displayed
* @param imode : error recovery mode
* @param num  : error to catch, if num=-1 all errors are catched
* @return 0
*/
DIFFERENTIAL_EQUATIONS_IMPEXP int C2F(errmds)(int *num, int *imess, int *imode);

#endif /* __ERRMDS_H__ */
/*--------------------------------------------------------------------------*/
