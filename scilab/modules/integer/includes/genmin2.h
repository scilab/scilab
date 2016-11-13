
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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
#ifndef __GENMIN2_H__
#define __GENMIN2_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

INTEGER_IMPEXP void C2F(genmin2)(int *n, void *dx, int *incx, int *xtyp, void *dy, int *ytyp, int *ind, int *no);

#endif /* __GENMIN2_H__ */
