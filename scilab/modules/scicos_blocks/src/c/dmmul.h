/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2012 - Allan CORNET <allan.cornet@scilab.org>
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
#ifndef __DMMUL_H__
#define __DMMUL_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP int dmmul(double *a, int *na, double *b, int *nb, double *c__,
                               int *nc, int *l, int *m, int *n);

#endif /* __DMMUL_H__ */
/*--------------------------------------------------------------------------*/
