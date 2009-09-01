
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
#ifndef __I_NEXTJ_H__
#define __I_NEXTJ_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param j
 * @param m
 * @param n
 * @param a
 * @param b
 * @return 
 */
INTEGER_IMPEXP int gengetcol(int typ, int j,int m,int n,int *a,int *b);
/**
 * TODO : comment
 * @param j
 * @return
 */
INTEGER_IMPEXP int C2F(inextj)(int *j);

#endif /* __I_NEXTJ_H__ */
