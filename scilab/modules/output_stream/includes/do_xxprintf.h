/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __DO_XXPRINTF_H__
#define __DO_XXPRINTF_H__

#include <stdio.h>

#define VPTR void * 
#define RET_END -2
#define RET_BUG -1 
#define FAIL 0
#define OK 1
#define MEM_LACK -3
#define MISMATCH -4
#define NOT_ENOUGH_ARGS -5

int do_xxprintf (char *fname, FILE *fp, char *format, int nargs, int argcnt, int lcount, char **strv);

#endif /* __DO_XXPRINTF_H__ */
