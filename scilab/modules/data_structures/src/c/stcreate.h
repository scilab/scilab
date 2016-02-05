/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
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

#ifndef  __STCREATE_H__
#define __STCREATE_H__

#include "dynlib_data_structures.h"

DATA_STRUCTURES_IMPEXP int C2F(stcreate)(int *lw, int *nz, int *sz, int *nf, char *fnames[], int *retval);

DATA_STRUCTURES_IMPEXP int sttyp(int *ivt, char *job, int *nf, char *fnames[]);

DATA_STRUCTURES_IMPEXP int creonevoid(int *slw, int *lw);

DATA_STRUCTURES_IMPEXP int crelistofvoids(int *slw, int *lw, int *nels);

#endif /* __STCREATE_H__ */
