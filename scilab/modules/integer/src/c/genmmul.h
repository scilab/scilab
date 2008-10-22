
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
#ifndef __GENMMUL_H__
#define __GENMMUL_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param a
 * @param na
 * @param b
 * @param nb
 * @param c
 * @param nc
 * @param l
 * @param m
 * @param n
 * @return 
 */
int C2F(genmmul)(int *typ, int *a, int *na, int *b, int *nb, int *c, int *nc, int *l, int *m, int *n);

#endif /* __GENMMUL_H__ */
