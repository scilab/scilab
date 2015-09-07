
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_FUNCTIONS_H__
#define __GW_FUNCTIONS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_functions.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
FUNCTIONS_SCILAB_IMPEXP int gw_functions(void);
/*--------------------------------------------------------------------------*/
//Scilab 6
FUNCTIONS_SCILAB_IMPEXP int sci_libfunc(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_FUNCTIONS_H__ */
/*--------------------------------------------------------------------------*/
