
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
int gengetcol(integer typ, integer j,integer m,integer n,integer *a,integer *b);
/**
 * TODO : comment
 * @param j
 * @return
 */
int C2F(inextj)(integer *j);

#endif /* __I_NEXTJ_H__ */
