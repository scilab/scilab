/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef  __STCREATE_H__
#define __STCREATE_H__

#include "dynlib_data_structures.h"

DATA_STRUCTURES_IMPEXP int C2F(stcreate)(int *lw, int *nz, int *sz, int *nf, char *fnames[], int *retval);

DATA_STRUCTURES_IMPEXP int sttyp(int *ivt, char *job, int *nf, char *fnames[]);

DATA_STRUCTURES_IMPEXP int creonevoid(int *slw,int *lw);

DATA_STRUCTURES_IMPEXP int crelistofvoids(int *slw,int *lw,int *nels);

#endif /* __STCREATE_H__ */
