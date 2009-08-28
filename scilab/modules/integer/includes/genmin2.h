
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENMIN2_H__
#define __GENMIN2_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

INTEGER_IMPEXP void C2F(genmin2)(int *n, void *dx, int *incx, int *xtyp, void *dy, int *ytyp, int *ind, int *no);

#endif /* __GENMIN2_H__ */
