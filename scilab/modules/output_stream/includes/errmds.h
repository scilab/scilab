
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __ERRMDS_H__
#define __ERRMDS_H__

#include "machine.h"

/* routine used by fortran */

/**
* this routine extract error modes out of errct variable
* @param imess : if 0 error message is displayed
* @param imode : error recovery mode
* @param num  : error to catch, if num=-1 all errors are catched
* @return 0
*/
int C2F(errmds)(int *num, int *imess, int *imode);

#endif /* __ERRMDS_H__ */
/*--------------------------------------------------------------------------*/
