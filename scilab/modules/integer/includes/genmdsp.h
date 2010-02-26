
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
#ifndef __GENMDSP_H__
#define __GENMDSP_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param x
 * @param nx
 * @param mm
 * @param nn
 * @param ll
 * @param lunit
 * @param cw
 * @param iw
 * @param cw_len
 * @return 
 */
INTEGER_IMPEXP int C2F(genmdsp)(int *typ, int *x, int *nx, int *mm, int *nn, int *ll, int *lunit, char cw[], int *iw, int cw_len);
#endif /* __GENMDSP_H__ */
