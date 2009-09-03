
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
#ifndef __GENINSERT1_H__
#define __GENINSERT1_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param mi
 * @param indx
 * @param to
 * @param from
 * @param inc2
 * @return 
 */
INTEGER_IMPEXP int C2F(geninsert1)(int *typ,int *mi,int *indx,int *to,int *from,int *inc2);
#endif /* __GENINSERT1_H__ */
