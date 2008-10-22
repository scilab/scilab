
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
#ifndef __GENMTRAN_H__
#define __GENMTRAN_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param a
 * @param na
 * @param b
 * @param nb
 * @param m
 * @param n
 * @return 
 */
int C2F(genmtran)(int *typ,int *a, int *na, int *b, int *nb, int *m, int *n);

#endif /* __GENMTRAN_H__ */
