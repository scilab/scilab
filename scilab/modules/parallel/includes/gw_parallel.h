/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#ifndef __GW_PARALLEL_H__
#define __GW_PARALLEL_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_parallel.h"
/*--------------------------------------------------------------------------*/
PARALLEL_IMPEXP int gw_parallel(void);
/*--------------------------------------------------------------------------*/
int sci_parallel_run(char *fname, void* pvApiCtx);
int sci_parallel_concurrency(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_PARALLEL_H__ */
/*--------------------------------------------------------------------------*/
