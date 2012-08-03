/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2012 - Allan CORNET <allan.cornet@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __DMMUL_H__
#define __DMMUL_H__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP int dmmul(double *a, int *na, double *b, int *nb, double *c__, 
                               int *nc, int *l, int *m, int *n);

#endif /* __DMMUL_H__ */
/*--------------------------------------------------------------------------*/ 
