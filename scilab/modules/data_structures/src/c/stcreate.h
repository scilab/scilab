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

#include "machine.h"
int C2F(stcreate)(integer *lw, integer *nz, integer *sz, integer *nf, char *fnames[], integer *retval);

int sttyp(int *ivt, char *job, int *nf, char *fnames[]);

int creonevoid(integer *slw,integer *lw);

int crelistofvoids(integer *slw,integer *lw,integer *nels);

#endif /* __STCREATE_H__ */
